/* author: Victor Roth Cardoso - vroth
 * student sponsored by CNPq
 * 
 * in summer project at Aberystwyth University
 * 
 * 
 * supervisors: Robert Hoehndorf and Georgios Gkoutos
 */
 
#include "itemset.hpp"
#include <iostream>

using std::pair;
using std::cerr;
using std::endl;

ItemSet::ItemSet() {
	support_count = 0;
}

ItemSet::ItemSet(ItemSet * copy) {
	support_count = 0;
	for(auto &i : copy->getItemSet()) {
		itemset.insert(pair<string, bool>(i.first, true));
	}
}

bool ItemSet::insert(string elem) {
	if(itemset.find(elem) == itemset.end()) {
		itemset.insert(pair<string, bool>(elem, true));
		return true;
	}
	else
		return false;
}

map <string, bool> & ItemSet::getItemSet() {
	return itemset;
}

void ItemSet::setSupportCount(unsigned int support_count) {
	this->support_count = support_count;
}

unsigned int ItemSet::getSupportCount() {
	return support_count;
}

string ItemSet::getNthString(unsigned int n) {
	unsigned int i=0;
	
	string last = itemset.rbegin()->first;
	map<string, bool>::iterator it = itemset.begin();
	do {
		if(i == n)
			return it->first;
		else
			i++;
	} while(itemset.key_comp()((*it++).first, last));
	
	
	return "";
}

/* bool ItemSet::remove(string elem) {
	map <string, bool>::iterator it = itemset.find(elem);
	if(it != itemset.end()) {
		itemset.erase(it);
		return true;
	}
	else
		return false;
}*/

bool ItemSet::contains(string elem) {
	if(itemset.find(elem) != itemset.end())
		return true;
	else
		return false;
}

vector<ItemSet *> ItemSet::subItemSets() {
	vector<ItemSet *> subsets;
	
	string last = itemset.rbegin()->first;
	map<string, bool>::iterator it = itemset.begin();
	do {
		if(subsets.size() == 0) {
			ItemSet * a0 = new ItemSet();
			ItemSet * a1 = new ItemSet();
			a1->insert(it->first);

			subsets.insert(subsets.end(), a0);
			subsets.insert(subsets.end(), a1);
		}
		else {
			unsigned int old_size = subsets.size();
			for(unsigned int i=0; i<old_size; i++) {
				ItemSet * a0 = new ItemSet(subsets[i]);
				a0->insert(it->first);
				subsets.insert(subsets.end(), a0);
			}
		}
	} while(itemset.key_comp()((*it++).first, last));
	
	
	delete(subsets[0]); //was needed in the beginning, now deleting
	subsets.erase(subsets.begin());

	cerr << "itemset.size() " << itemset.size() << "\tsubsets.size() " << subsets.size() << endl;
	return subsets;
}
