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

bool ItemSet::remove(string elem) {
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
}
