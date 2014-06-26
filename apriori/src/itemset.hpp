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

using std::map;
using std::string;

class ItemSet {
	private:
		map <string, bool> itemset;
		unsigned int support_count;
	public:
		bool insert(string);
		
		map <string, bool> & getItemSet();
		
		void setSupportCount(unsigned int);
		unsigned int getSupportCount();
		
		string getNthString(unsigned int);
};
