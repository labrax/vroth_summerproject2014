/* author: Victor Roth Cardoso - vroth
 * student sponsored by CNPq
 * 
 * in summer project at Aberystwyth University
 * 
 * 
 * supervisors: Robert Hoehndorf and Georgios Gkoutos
 */

/** a general class for storing a itemset
 */
 
#pragma once

#include <map>
#include <string>
#include <vector>

using std::map;
using std::string;
using std::vector;

class ItemSet {
	private:
		map <string, bool> itemset;
		unsigned int support_count;
	public:
		ItemSet();
		ItemSet(ItemSet *);
		bool insert(string);
		
		map <string, bool> & getItemSet();
		unsigned int getAmountElements();
		
		void setSupportCount(unsigned int);
		unsigned int getSupportCount();
		
		string getNthString(unsigned int);
		bool contains(string);
		
		vector<ItemSet *> subItemSets();
};

bool ItemSetSort(ItemSet * c0, ItemSet * c1);
