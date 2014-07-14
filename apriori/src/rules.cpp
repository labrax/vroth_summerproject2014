/* author: Victor Roth Cardoso - vroth
 * student sponsored by CNPq
 * 
 * in summer project at Aberystwyth University
 * 
 * 
 * supervisors: Robert Hoehndorf and Georgios Gkoutos
 */

#include "rules.hpp"
#include "parameters.hpp"

#include <iostream>
#include <algorithm>

using std::cout;
using std::endl;

using std::get;

Rules::Rules(uint64_t amount_transactions, double confidence, Ontology * ontologies, map<string, uint64_t> & itemset_1) {
	this->amount_transactions = amount_transactions;
	this->confidence = confidence;
	this->ontologies = ontologies;
	this->itemset_1 = itemset_1;
}

Rules::~Rules() {
	for(auto & i : rules) {
		delete(get<1>(i));
		delete(get<2>(i));
	}
	for(auto & i : larges) {
		delete(i);
	}
}

void Rules::addLarge(LargeItemSet * a) {
	larges.insert(larges.end(), a);
}

void Rules::computeRules() {
	//!create the rules from the LargeItemSets
	measures m;
	m.sumDepth = 0;
	m.sumHeight = 0;
	m.confidence = 0;
	m.lift = 0;
	m.semantic_similarity = 0;
	for(auto & l : larges) {
		if(l->getIteration() == 1) {
			//cout << "skipping large 1" << endl;
			continue;
		}
		if(l->getAmountTransactions() == 0) //finish the loop
			break;
		//cout << "generating rules for iteration " << l->getIteration() << endl;
		for(auto & i : l->getItemSets()) {
			for(auto & s : i->subItemSets()) {
				ItemSet * b = new ItemSet(i, s);
				
				m.n_transactions = i->getSupportCount();
				m.sumDepth = 0;
				m.sumHeight = 0;

				for(auto & el : i->getItemSet()) { //obtain height and depth data
					NodeOntology * node = ontologies->getNode(el.first);
					if(node != NULL) {
						m.sumHeight += node->getHeight();
						m.sumDepth += node->getDepth();
					}
				}
				
				rules.insert(rules.end(), tuple<measures, ItemSet *, ItemSet *>(m, s, b));
			}
		}
	}
	//!create the rules from the LargeItemSets
	
	//!get the support for every part of all rules
	for(auto & r : rules) {
		get<0>(r).n_transactions_antecedent = getFrequency(get<1>(r));
		get<0>(r).n_transactions_consequent = getFrequency(get<2>(r));
	}
	//!get the support for every part of all rules
	
	//!remove the ones without the confidence
	for(uint64_t r = 0; r < rules.size(); r++) {
		auto & i = rules.at(r);
		get<0>(i).lift = ((double) amount_transactions*get<0>(i).n_transactions)/((double) get<0>(i).n_transactions_antecedent*get<0>(i).n_transactions_consequent);
		get<0>(i).confidence = (double)get<0>(i).n_transactions/(double)get<0>(i).n_transactions_antecedent;
		if(get<0>(i).confidence < confidence) {
			/*cout << "tuple should be removed" << endl;
			cout << "first element: " << (double)get<2>(i)->getSupportCount() << " ";
			cout << "second element: " << (double)get<1>(i)->getSupportCount();*/
			delete(get<1>(i));
			delete(get<2>(i));
			rules.erase(rules.begin() + r);
			r--; //to fix with element removed
		}
	}
	//!remove the ones without the confidence
	
	calculateSemanticSimilarity();
	
	//sort the rules
	std::sort(rules.begin(), rules.end(), rulesSort);
}

void Rules::print() {
	if(rules.size() != 0)
		cout << "confidence" << "\t" << "semantic similarity" << "\t" << "lift" << "\t" << "sumDepth" << "\t" << "sumHeight" << "\t" << "Elements ==>" << "\t" << "Elements" << endl;
	else
		cout << "NO RULES GENERATED!" << endl;

	for(auto &i : rules) {
		if((double)get<2>(i)->getSupportCount()/(double)get<1>(i)->getSupportCount() < confidence)
			continue;
			
		cout << get<0>(i).confidence << "\t" << get<0>(i).semantic_similarity << "\t" << get<0>(i).lift << "\t" << get<0>(i).sumDepth << "\t" << get<0>(i).sumHeight << "\t";
		get<1>(i)->printWithOntology(ontologies);
		cout << "===>\t";
		get<2>(i)->printWithOntology(ontologies);
		cout << endl;
	}
}

uint64_t Rules::getFrequency(ItemSet * itemset) {
	for(auto & p : larges[itemset->getAmountElements()-1]->getItemSets()) {
		bool ok = true;
		for(auto & e : itemset->getItemSet()) {
			if(!(p->contains(e.first))) {
				ok = false;
				break;
			}
		}
		if(ok == true) {
			return p->getSupportCount();
			break;
		}
	}
	return 0; //this condition should never occur
}

void Rules::calculateSemanticSimilarity() {
	if(ontologies->isProcessed() == false) {
		if(Parameters::verbose)
			cout << "no semantic similarity was calculated" << endl;
		return;
	}
	for(auto & r : rules) {
		double similarity_intercession = 0;
		double similarity_union = 0;

		map <string, bool> * first_ontologies = NULL; //will store all the parents from the 1st itemset
		map <string, bool> * second_ontologies = NULL; //will store all the parents from the 2nd itemset
		
		for(auto & f : get<1>(r)->getItemSet()) { //get all the parent nodes from the 1st itemset (antecedent)
			NodeOntology * search_1 = ontologies->getNode(f.first);
			map <string, bool> * result_first = search_1->returnOntologies();
			if(result_first != NULL) {
				if(first_ontologies == NULL)
					first_ontologies = result_first;
				else {
					for(auto & e : *result_first) {
						if(first_ontologies->find(e.first) == first_ontologies->end()) {
							first_ontologies->insert(pair<string, bool> (e.first, true));
						}
					}
					delete(result_first);
				}
			}
		}
		for(auto & s : get<2>(r)->getItemSet()) { //get all the parent nodes from the 2nd itemset (consequent)
			NodeOntology * search_2 = ontologies->getNode(s.first);
			map <string, bool> * result_second = search_2->returnOntologies();
			if(result_second != NULL) {
				if(second_ontologies == NULL)
					second_ontologies = result_second;
				else {
					for(auto & e : *result_second) {
						if(second_ontologies->find(e.first) == second_ontologies->end()) {
							second_ontologies->insert(pair<string, bool> (e.first, true));
						}
					}
					delete(result_second);
				}
			}
		}

		if(Parameters::debug) {
			cout << "for the rule:" << endl;
			get<1>(r)->printWithOntology(ontologies);
			cout << "===>\t";
			get<2>(r)->printWithOntology(ontologies);
			cout << endl;			
		}

		if(Parameters::debug)		
			cout << "intercession" << endl;
		for(auto & i : *first_ontologies) { //get the intercession value
			if(second_ontologies->find(i.first) != second_ontologies->end()) {
				similarity_intercession += informationMeasure(i.first);
			}
		}
		
		//now i'll join the two sets to get the union
		for(auto & j : *second_ontologies) {
			first_ontologies->insert(pair<string, bool> (j.first, true));
		}
		//the sets are joined
		
		if(Parameters::debug)
			cout << "union" << endl;
		for(auto & i : *first_ontologies) { //get the union value
			similarity_union += informationMeasure(i.first);
		}
		
		if(Parameters::debug)
			cout << "similarity: " << similarity_intercession << "\t" << similarity_union << endl;
		get<0>(r).semantic_similarity = similarity_intercession/similarity_union;
		
		if(first_ontologies != NULL)
			delete(first_ontologies);
		if(second_ontologies != NULL)
			delete(second_ontologies);
	}
	return;
}

double Rules::informationMeasure(string identifier) {
	map <string, uint64_t>::iterator it = itemset_1.find(identifier);
	if(it == itemset_1.end()) {
		if(Parameters::debug)
			cout << identifier << " has no information measure" << endl;
		return 0;
	}
	if(Parameters::debug)
		cout << "M: " << -log2((double)it->second/(double)amount_transactions) << endl;
	return (-log2((double)it->second/(double)amount_transactions));
}

bool rulesSort(const tuple<measures, ItemSet *, ItemSet *> & a, const tuple<measures, ItemSet *, ItemSet *> & b) {
	if(get<0>(b).semantic_similarity == get<0>(a).semantic_similarity)
		return get<0>(b).confidence < get<0>(a).confidence;
	return get<0>(b).semantic_similarity > get<0>(a).semantic_similarity; //the less the better
	//return get<0>(b).confidence < get<0>(a).confidence; //to order by confidence
}
