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

LargeItemSet * CandidateItemSet::apriori_gen(LargeItemSet * a) {
	LargeItemSet * new_candidate = new LargeItemSet(a->getIteration()+1);
	
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
				
				bool ok = true; //!this block will prune candidates
				vector<ItemSet *> subsets = new_itemset->subItemSets();
				
				for(auto &i : subsets) {
					if(i->getAmounElements() > 1) {
						if(a->contains(i) == false) {
							ok = false;
							break;
						}
					}
				}
				
				//cout << "prune results were " << ok << endl;
				
				if(ok) //!this block will prune candidates
					new_candidate->insertSet(new_itemset);
			}
		}
	}
	
	//if(new_large->getItemSets().size() == 0) {
	
	//}
	
	return new_candidate;
}

LargeItemSet * CandidateItemSet::subset(LargeItemSet * a, vector <pair <string, string>> * normalized_transactions) {
	//root->insertItemSet(new_itemset); //TODO:create function
	return NULL;
}
