#pragma once

#include <unordered_map>

template <class TYPE>
class List {
	std::unordered_map <TYPE, bool> itemset;
	unsigned int support_count;
	
	public:
		List() { };
		~List() { };
		unsigned int getSupport() {	return support_count; };
		
		bool contains(TYPE elem) {
			if(itemset.find(elem) == itemset.end())
				return false;
			else
				return true;
		};
		void insert(TYPE elem) { itemset.insert(std::pair<TYPE, bool>(elem, true)); }; //keys are unique in unordered_map
		void remove(TYPE elem) { itemset.erase(itemset.find(elem)); };
		TYPE pop(void) {
			if(!itemset.empty()) {				
				TYPE a = itemset.at(itemset.begin());
				itemset.erase(itemset.begin());
				return a;
			}
			else {
				return TYPE();
			}
		}
		bool empty(void) { return itemset.empty(); };
};
