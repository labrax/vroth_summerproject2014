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

#include <mutex>

#include <cstdint>

using std::map;
using std::string;
using std::vector;

using std::mutex;

class ItemSet {
	private:
		map <string, bool> itemset;
		uint64_t support_count;
		
		mutex sp_lock; //to lock increases in count (subsetThreaded)
	public:
		ItemSet();
		ItemSet(ItemSet * copy);
		ItemSet(ItemSet * original, ItemSet * without);
		bool insert(string);
		
		map <string, bool> & getItemSet();
		unsigned int getAmountElements();
		
		void setSupportCount(uint64_t);
		uint64_t getSupportCount();
		void increaseSupportCount();
		
		string getNthString(unsigned int); //to get the identifier for the node
		bool contains(string);
		
		vector<ItemSet *> subItemSets();
		
		void print();
};

bool ItemSetSort(ItemSet *, ItemSet *);
