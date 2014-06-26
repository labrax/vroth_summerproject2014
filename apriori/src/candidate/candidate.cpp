/* author: Victor Roth Cardoso - vroth
 * student sponsored by CNPq
 * 
 * in summer project at Aberystwyth University
 * 
 * 
 * supervisors: Robert Hoehndorf and Georgios Gkoutos
 */

#include "candidate.hpp"

#include <vector>
#include <iostream>

using std::vector;

using std::cout;
using std::endl;

CandidateItemSet::CandidateItemSet() {
	root = new Node(0, "", NULL);
}

CandidateItemSet::~CandidateItemSet() {
	delete(root);
}

LargeItemSet * CandidateItemSet::generateCandidates(LargeItemSet * a) {
	LargeItemSet * new_large = new LargeItemSet(a->getInteration()+1); //TODO: pass to the internal structure!
	vector<ItemSet *> last_large = a->getItemSets();

	for(unsigned int i=0; i<last_large.size(); i++) {
		ItemSet * first = last_large[i];
		for(unsigned int j=i+1; j<last_large.size(); j++) {
			ItemSet * second = last_large[j];
			
			//!this will test if they are similar until the last element
			bool nice = true;
			string last_first = first->getItemSet().rbegin()->first;
			map<string, bool>::iterator it = first->getItemSet().begin();
			map<string, bool>::iterator it2 = second->getItemSet().begin();
			
			while(it->first != last_first) { //while is not the last
				if(it->first != it2->first) {//its differente, we are not cool
					nice = false;
					break;
				}
				it++; //increment both iterators
				it2++;
			}
			//!this will test if they are similar until the last element
			if(nice == true) {
				ItemSet * new_itemset = new ItemSet(first);
				new_itemset->insert(it2->first);
				
				new_large->insertSet(new_itemset);
				root->insertItemSet(new_itemset);
			}
		}
	}
	
	//if(new_large->getItemSets().size() == 0) {
	
	//}
	
	new_large->printinfo();
	new_large->print();
	
	return new_large;
}
