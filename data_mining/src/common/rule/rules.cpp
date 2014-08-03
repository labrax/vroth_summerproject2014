/* author: Victor Roth Cardoso - vroth
 * student sponsored by CNPq
 * 
 * in summer project at Aberystwyth University
 * 
 * 
 * supervisors: Robert Hoehndorf and Georgios Gkoutos
 */

#include "rules.hpp"
#include "../parameters.hpp"

#include <iostream>
#include <algorithm>

using std::cout;
using std::endl;

Rules::Rules(uint64_t amount_transactions, double confidence, Ontology * ontologies, map<string, uint64_t> & itemset_1) {
	this->amount_transactions = amount_transactions;
	this->confidence = confidence;
	this->ontologies = ontologies;
	this->itemset_1 = itemset_1;
}

Rules::Rules(string file) { //TODO: implement Rules::Rules(string file)
	
}

Rules::~Rules() {
	for(auto & i : larges) {
		delete(i);
	}
	for(auto & r: rules) {
		delete(r);
	}
}

void Rules::addLarge(LargeItemSet * a) {
	larges.insert(larges.end(), a);
}

void Rules::computeRules() {
	//!create the rules from the LargeItemSets
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
				
				RuleNode * new_node = new RuleNode(s,b);
				new_node->setNTransactions(i->getSupportCount());
				
				rules.insert(rules.end(), new_node);
			}
		}
	}
	//!create the rules from the LargeItemSets
	
	//!get the support for every part of all rules
	for(auto & r : rules) {
		r->setNTransactionsAntecedent(getFrequency(r->getItemSetAntecedent())); //support for each side
		r->setNTransactionsConsequent(getFrequency(r->getItemSetConsequent()));
		
		r->calculateConfidence(); //remaining variables
		if(r->getConfidence() < confidence)
			continue;
		r->calculateSupport(amount_transactions); //calculate the other data
		r->calculateLift(amount_transactions);
		r->calculateDepthHeight(ontologies);
	}
	//!get the support for every part of all rules
	
	calculateSemanticSimilarity();
	
	if(Parameters::filter_results)
		filterRules();
	
	//sort the rules
	std::sort(rules.begin(), rules.end(), rulesSort);
}

void Rules::print() { //TODO: add std::setprecision(X) to output of rules
	if(rules.size() != 0)
		cout << "support" << "\t" << "confidence" << "\t" << "semantic similarity" << "\t" << "lift" << "\t" << "sumDepth" << "\t" << "sumHeight" << "\t" << "Elements ==>" << "\t" << "Elements" << endl;
	else
		cout << "NO RULES GENERATED!" << endl;

	for(auto &i : rules) {
		if(i->getConfidence() < confidence)
			continue;
			
		cout << i->getSupport() << "\t" << i->getConfidence() << "\t" << i->getSemanticSimilarity() << "\t" << i->getLift() << "\t" << i->getSumDepth() << "\t" << i->getSumHeight() << "\t";
		i->getItemSetAntecedent()->printWithOntology(ontologies);
		cout << "===>\t";
		i->getItemSetConsequent()->printWithOntology(ontologies);
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
		if(r->getConfidence() < confidence)
			continue;
			
		double similarity_intercession = 0;
		double similarity_union = 0;

		map <string, bool> * first_ontologies = NULL; //will store all the parents from the 1st itemset
		map <string, bool> * second_ontologies = NULL; //will store all the parents from the 2nd itemset
		
		for(auto & f : r->getItemSetAntecedent()->getItemSet()) { //get all the parent nodes from the 1st itemset (antecedent)
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
		for(auto & s : r->getItemSetConsequent()->getItemSet()) { //get all the parent nodes from the 2nd itemset (consequent)
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
			r->getItemSetAntecedent()->printWithOntology(ontologies);
			cout << "===>\t";
			r->getItemSetConsequent()->printWithOntology(ontologies);
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
			
		r->setSemanticSimilarity(similarity_intercession/similarity_union);
		
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

vector<RuleNode *> & Rules::getRules() {
	return rules;
}

map<string, vector<RuleNode *>> & Rules::getRulesMap() { //will go through the rules and generate the map for faster search
	 //TODO: check, the elements should be sorted, so this hash will be fine
	 
	for(auto & r : rules) {
		map<string, vector<RuleNode *>>::iterator it = rulesMap.find(r->getItemSetAntecedent()->getItemSet().begin()->first);
		if(it == rulesMap.end()) {
			rulesMap.insert(pair<string, vector<RuleNode *>> (r->getItemSetAntecedent()->getItemSet().begin()->first, vector<RuleNode *>() ));
		}
		it = rulesMap.find(r->getItemSetAntecedent()->getItemSet().begin()->first); //now will get the iterator where it was inserted
		
		ItemSet * antecedent = new ItemSet(r->getItemSetAntecedent());
		ItemSet * consequent = new ItemSet(r->getItemSetConsequent());
		it->second.insert(it->second.end(), new RuleNode(antecedent, consequent));
	}
	return rulesMap;
}

void Rules::filterRules() { //TODO: implement Rules::filterRules()
	cout << "filterRules() is disabled" << endl;
	return;
	
	unsigned int removed = 0;

	if(Parameters::verbose)
		cout << removed << " removed" << endl;
}

bool rulesSort(const RuleNode * a, const RuleNode * b) {
	if(b->getSemanticSimilarity() == a->getSemanticSimilarity())
		return b->getConfidence() < a->getConfidence();
	return b->getSemanticSimilarity() > a->getSemanticSimilarity(); //the less the better
	//return get<0>(b).confidence < get<0>(a).confidence; //to order by confidence
}
