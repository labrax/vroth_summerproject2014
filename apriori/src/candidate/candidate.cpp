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
				ItemSet * new_itemset = new ItemSet(first); //create new itemset
				new_itemset->insert(it2->first); //with new element
				
				bool ok = true; //!this block will prune candidates
				vector<ItemSet *> subsets = new_itemset->subItemSets(); //get the subsets (without empty and full)
				
				for(auto &s : subsets) { //verify if all the subsets, ItemSets exists on the large
					if(s->getAmountElements() > 1) { //if has size 1 can be skipped
						if(a->contains(s) == false) { //if doesnt contain, doesn't add
							ok = false;
							break;
						}
					}
				}
				
				//cout << "prune results were " << ok << endl;
				
				if(ok) 
					new_candidate->insertSet(new_itemset); //!this block will prune candidates
			}
		}
	}
	
	//if(new_large->getItemSets().size() == 0) {
	
	//}
	
	return new_candidate;
}

LargeItemSet * CandidateItemSet::subset(LargeItemSet * a, vector <pair <string, string>> * normalized_transactions, unsigned int support) {
	for(auto &i: a->getItemSets()) {
		root->insertItemSet(i);
	}
	LargeItemSet * new_large = new LargeItemSet(a->getIteration());

	unsigned int tests = 0;
	cout << "normalized_transactions->size() = " << normalized_transactions->size() << endl;

	unsigned int j=0;
	for(unsigned int i=0; i < normalized_transactions->size(); i=j) { //will scan transaction by transaction
		for(j=i; j < normalized_transactions->size() && (*normalized_transactions)[j].first == (*normalized_transactions)[i].first; ++j);
		
		vector<pair <string, string>>::const_iterator first = normalized_transactions->begin() + i; //get the first element in transaction
		vector<pair <string, string>>::const_iterator last = normalized_transactions->begin() + j; //get the last element in transaction
		
		vector<pair <string, string>> transaction(first, last);
		
		/*cout << endl << transaction[0].first << ": "; //would print all the transactions in blocks
		for(unsigned int b=0; b<transaction.size(); b++) {
			cout << transaction[b].second << endl;
		}*/
		
		tests++;
		root->transactionScan(&transaction);
	}
	
	cout << "transactions tested = " << tests << endl;
	
	//TODO: get all the transactions with the minimum support!
	root->grabMinimumSupport(new_large, support);
	
	return new_large;
}
