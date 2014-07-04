/* author: Victor Roth Cardoso - vroth
 * student sponsored by CNPq
 * 
 * in summer project at Aberystwyth University
 * 
 * 
 * supervisors: Robert Hoehndorf and Georgios Gkoutos
 */

#include "rules.hpp"

#include <iostream>
#include <algorithm>

#include <cstdint>

using std::cout;
using std::endl;

using std::get;

Rules::Rules(double confidence, Ontology * ontologies) : confidence(confidence), ontologies(ontologies) {
	rules.clear();
	larges.clear();
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
				b->setSupportCount(i->getSupportCount()); //TODO: move to the right place //<---------------------------------------------------------first part here
				
				m.sumDepth = 0;
				m.sumHeight = 0;
				for(auto & el : i->getItemSet()) { //obtain height and depth data
					m.sumHeight += ontologies->getNode(el.first)->getHeight();
					m.sumDepth += ontologies->getNode(el.first)->getDepth();
				}
				
				rules.insert(rules.end(), tuple<measures, ItemSet *, ItemSet *>(m, s, b));
			}
		}
	}
	//!create the rules from the LargeItemSets
	
	
	//TODO ? parallel: small amount of data, maybe not needed
	//!get the data for the confidence
	for(auto & r : rules) {
		for(auto & p : larges[get<1>(r)->getAmountElements()-1]->getItemSets()) {
			bool ok = true;
			for(auto & e : get<1>(r)->getItemSet()) {
				if(!(p->contains(e.first))) {
					ok = false;
					break;
				}
			}
			if(ok == true) {
				get<1>(r)->setSupportCount(p->getSupportCount());                         //<---------------------------------------------------------second part here
				break;
			}
		}
	}
	//!get the data for the confidence
	
	//!remove the ones without the confidence
	for(uint64_t r = 0; r < rules.size(); r++) {
		auto & i = rules.at(r);
		get<0>(i).confidence = (double)get<2>(i)->getSupportCount()/(double)get<1>(i)->getSupportCount();
		if(get<0>(i).confidence < confidence) {
			/*cout << "tuple should be removed" << endl;
			cout << "first element: " << (double)get<2>(i)->getSupportCount() << " ";
			cout << "second element: " << (double)get<1>(i)->getSupportCount();*/
			delete(get<1>(i));
			delete(get<2>(i));
			rules.erase(rules.begin() + r); //TODO: why is it not removing?
			r--; //as an element was removed
		}
	}
	//!remove the ones without the confidence
	
	//sort the rules
	std::sort(rules.begin(), rules.end(), rulesSort);
}

void Rules::print() {
	if(rules.size() != 0)
		cout << "confidence" << "\t" << "sumDepth" << "\t" << "sumHeight" << "\t" << "Elements ==>" << "\t" << "Elements" << endl;
	else
		cout << "NO RULES GENERATED!" << endl;
	for(auto &i : rules) {
		if((double)get<2>(i)->getSupportCount()/(double)get<1>(i)->getSupportCount() < confidence)
			continue;
			
		cout << get<0>(i).confidence << "\t" << get<0>(i).sumDepth << "\t" << get<0>(i).sumHeight << "\t";
		get<1>(i)->printWithOntology(ontologies);
		cout << "===>\t";
		get<2>(i)->printWithOntology(ontologies);
		cout << endl;
	}
}

bool rulesSort(const tuple<measures, ItemSet *, ItemSet *> & a, const tuple<measures, ItemSet *, ItemSet *> & b) {
	if(get<0>(b).sumHeight == get<0>(a).sumHeight)
		return get<0>(b).confidence < get<0>(a).confidence;
	return get<0>(b).sumHeight > get<0>(a).sumHeight; //to order by lower height
	//return get<0>(b).confidence < get<0>(a).confidence; //to order by confidence
}
