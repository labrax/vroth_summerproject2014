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

using std::cout;
using std::endl;

LargeItemSet::LargeItemSet(unsigned int iteration) {
	this->k = iteration;
}

LargeItemSet::~LargeItemSet() {
	for(auto &i : itemset) {
		delete(i);
	}
}

void LargeItemSet::insertSet(ItemSet * set) {
	itemset.insert(itemset.end(), set);
}

vector<ItemSet *> & LargeItemSet::getItemSets() {
	return itemset;
}

unsigned int LargeItemSet::getIteration() {
	return k;
}

void LargeItemSet::print() {
	for(auto &i : itemset) {
		cout << "With support " << i->getSupportCount() << ": ";
		for(auto &j: i->getItemSet()) {
			cout << j.first << " ";
		}
		cout << endl;
	}
}

void LargeItemSet::printinfo() {
	cout << "iteraction number: " << this->getIteration() << " has " << this->getItemSets().size() << " itemsets" << endl;
}
