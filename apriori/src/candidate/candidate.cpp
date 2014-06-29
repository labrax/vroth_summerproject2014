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
#include <thread>

using std::vector;

using std::cout;
using std::endl;

using std::thread;

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
				for(auto &s: subsets)
					delete(s); //after verifying the subsets delete it
					
				
				//cout << "prune results were " << ok << endl;
				
				if(ok) 
					new_candidate->insertSet(new_itemset); //!this block will prune candidates
				else
					delete(new_itemset); //if the candidate itemset is not used delete the memory allocated
			}
		}
	}
	
	//if(new_large->getItemSets().size() == 0) {
	
	//}
	
	return new_candidate;
}

LargeItemSet * CandidateItemSet::subset(LargeItemSet * a, vector <pair <string, string>> * normalized_transactions, unsigned int support) {
	for(auto &i: a->getItemSets()) {
		ItemSet * e = new ItemSet(i);
		root->insertItemSet(e);
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
	
	root->grabMinimumSupport(new_large, support);
	
	return new_large;
}

void run_threaded(Node * root, vector <pair <string, string>> * normalized_transactions, unsigned int rangeLow, unsigned int rangeHigh) {
	unsigned int j=0;
	for(unsigned int i=rangeLow; i < rangeHigh; i=j) { //will scan transaction by transaction
		for(j=i; j < normalized_transactions->size() && (*normalized_transactions)[j].first == (*normalized_transactions)[i].first; ++j);
		
		vector<pair <string, string>>::const_iterator first = normalized_transactions->begin() + i; //get the first element in transaction
		vector<pair <string, string>>::const_iterator last = normalized_transactions->begin() + j; //get the last element in transaction
		
		vector<pair <string, string>> transaction(first, last);
		
		root->transactionScan(&transaction);
	}
}

vector<pair <unsigned int, unsigned int>> & startThreadSettings(vector <pair <string, string>> * normalized_transactions) {
	static vector<pair<unsigned int, unsigned int>> thread_blocks;
	
	if(thread_blocks.size() > 0)
		return thread_blocks;
		
	unsigned concurentThreadsSupported = thread::hardware_concurrency();
	cout << concurentThreadsSupported << " concurrent threads will be used" << endl;
	
	unsigned int initial=0;
	while(thread_blocks.size() < concurentThreadsSupported-1) {
		unsigned int new_initial = normalized_transactions->size()*thread_blocks.size()/concurentThreadsSupported;
		if(new_initial < initial)
			new_initial = initial;

		unsigned int new_end = normalized_transactions->size()*(thread_blocks.size()+1)/concurentThreadsSupported;
		for(initial = new_end; initial < normalized_transactions->size() && (*normalized_transactions)[initial].first == (*normalized_transactions)[new_end].first; ++initial);
		
		thread_blocks.insert(thread_blocks.end(), pair<unsigned int, unsigned int>(new_initial, initial));
	}
	thread_blocks.insert(thread_blocks.end(), pair<unsigned int, unsigned int>(initial, normalized_transactions->size()));
	
	for(auto & i: thread_blocks) {
		cout << "thread has range: " << i.first << " to " << i.second << endl;
	}
	
	return thread_blocks;
}

LargeItemSet * CandidateItemSet::subsetThreaded(LargeItemSet * a, vector <pair <string, string>> * normalized_transactions, unsigned int support) {
	for(auto &i: a->getItemSets()) {
		ItemSet * e = new ItemSet(i);
		root->insertItemSet(e);
	}
	LargeItemSet * new_large = new LargeItemSet(a->getIteration());

	vector<pair <unsigned int, unsigned int>> thread_blocks = startThreadSettings(normalized_transactions);
	
	vector<thread *> threads;
	
	for(unsigned int i=0; i<thread_blocks.size(); i++) {
		threads.insert(threads.end(), new thread(run_threaded, root, normalized_transactions, thread_blocks[i].first, thread_blocks[i].second));
	}
	
	for(auto & t: threads)
		t->join();

	for(auto & t: threads)
		delete(t);
	
	root->grabMinimumSupport(new_large, support);
	
	return new_large;
}
