/* author: Victor Roth Cardoso - vroth
 * student sponsored by CNPq
 * 
 * in summer project at Aberystwyth University
 * 
 * 
 * supervisors: Robert Hoehndorf and Georgios Gkoutos
 */
 
#include "itemset.hpp"

using std::pair;

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
}

bool ItemSet::contains(string elem) {
	if(itemset.find(elem) != itemset.end())
		return true;
	else
		return false;
} */
