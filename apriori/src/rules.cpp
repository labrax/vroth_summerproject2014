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

Rules::Rules(double confidence) : confidence(confidence) {
	rules.clear();
	larges.clear();
}

Rules::~Rules() {
	for(auto & i : rules) {
		delete(i.first);
		delete(i.second);
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
				b->setSupportCount(i->getSupportCount());
				rules.insert(rules.end(), pair<ItemSet *, ItemSet *>(s, b));
			}
		}
	}
	//!create the rules from the LargeItemSets
	
	
	//TODO ? parallel: small amount of data, maybe not needed
	//!get the data for the confidence
	for(auto & r : rules) {
		for(auto & p : larges[r.first->getAmountElements()-1]->getItemSets()) {
			bool ok = true;
			for(auto & e : r.first->getItemSet()) {
				if(!(p->contains(e.first))) {
					ok = false;
					break;
				}
			}
			if(ok == true) {
				r.first->setSupportCount(p->getSupportCount());
				break;
			}
		}
	}
	//!get the data for the confidence
	
	//!remove the ones without the support
	for(uint64_t r = 0; r < rules.size(); r++) {
		auto & i = rules.at(r);
		if((double)i.second->getSupportCount()/(double)i.first->getSupportCount() < confidence) {
			delete(i.first);
			delete(i.second);
			rules.erase(rules.begin() + r);
		}
	}
	//!remove the ones without the support
	
	//sort the rules
	std::sort(rules.begin(), rules.end(), rulesSort);
}

void Rules::print() {
	for(auto &i : rules) {
		if((double)i.second->getSupportCount()/(double)i.first->getSupportCount() < confidence)
			continue;
			
		cout << "#CONFIDENCE: " << (double)i.second->getSupportCount()/(double)i.first->getSupportCount() << "\t";
		i.first->print();
		cout << "==> ";
		i.second->print();
		cout << endl;
	}
}

bool rulesSort(const pair<ItemSet *, ItemSet *> & a, const pair<ItemSet *, ItemSet *> & b) {
	return (double)b.second->getSupportCount()/(double)b.first->getSupportCount() < (double)a.second->getSupportCount()/(double)a.first->getSupportCount();
}
