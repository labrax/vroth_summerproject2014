/* author: Victor Roth Cardoso - vroth
 * student sponsored by CNPq
 * 
 * in summer project at Aberystwyth University
 * 
 * 
 * supervisors: Robert Hoehndorf and Georgios Gkoutos
 */
 
#include "large.hpp"

#include <iostream>
#include <algorithm>

using std::cout;
using std::endl;

LargeItemSet::LargeItemSet(unsigned int iteration) {
	this->k = iteration;
	itemset.clear();
}

LargeItemSet::~LargeItemSet() {
	for(auto &i : itemset) {
		delete(i);
	}
}

void LargeItemSet::insertSet(ItemSet * set) {
	large_lock.lock();
	itemset.insert(itemset.end(), set);
	large_lock.unlock();
}

vector<ItemSet *> & LargeItemSet::getItemSets() {
	return itemset;
}

unsigned int LargeItemSet::getAmountTransactions() {
	return itemset.size();
}

unsigned int LargeItemSet::getIteration() {
	return k;
}

bool LargeItemSet::contains(ItemSet * a) {
	for(auto &i : itemset) {
		bool cool = true;
		for(auto &j : a->getItemSet()) {
			if(i->contains(j.first) == false) {
				cool = false;
				break;
			}
		}
		if(cool == true) {
			return true;
		}
	}
	return false;
}

void LargeItemSet::print() {
	for(auto &i : itemset) {
		for(auto &j: i->getItemSet()) {
			cout << j.first << " ";
		}
		cout << "#SUP: " << i->getSupportCount() << endl;
	}
}

void LargeItemSet::printinfo() {
	if(this->getItemSets().size() != 0)
		cout << "iteraction number: " << this->getIteration() << " results in " << this->getItemSets().size() << " itemsets" << endl;
	else
		cout << "no more itemsets" << endl;

}

void LargeItemSet::sort() {
	std::sort(itemset.begin(), itemset.end(), ItemSetSort);
}
