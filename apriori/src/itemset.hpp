/* author: Victor Roth Cardoso - vroth
 * student sponsored by CNPq
 * 
 * in summer project at Aberystwyth University
 * 
 * 
 * supervisors: Robert Hoehndorf and Georgios Gkoutos
 */
 
#pragma once

#include <map>
#include <string>

using std::map;
using std::string;

class ItemSet {
	private:
		map <string, bool> itemset;
	public:
		bool insert(string);
		bool remove(string);
		bool contains(string);
		map <string, bool> & getItemset() { return itemset; };
};
