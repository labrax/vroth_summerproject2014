/* author: Victor Roth Cardoso - vroth
 * student sponsored by CNPq
 * 
 * in summer project at Aberystwyth University
 * 
 * 
 * supervisors: Robert Hoehndorf and Georgios Gkoutos
 */

/** a general class for storing a set of k-itemsets
 */

#pragma once

#include "itemset.hpp"
#include "itemset_tree.hpp"

#include <vector>
#include <mutex>

using std::vector;

using std::mutex;


class LargeItemSet {
	private:
		vector<ItemSet *> itemset;
		ItemSetTree * root; //new approach to solve the problem with the function bool contains(ItemSet *);
		
		unsigned int k; //to identify the iteration, also the size
		
		mutex large_lock; //to lock insertSet (apriori_genThreaded)
	public:
		LargeItemSet(unsigned int);
		~LargeItemSet();
		void insertSet(ItemSet * set);
		
		vector<ItemSet *> & getItemSets();
		unsigned int getAmountTransactions();
		unsigned int getIteration();
		
		bool contains(ItemSet *);
		
		void print();
		void printinfo();
		
		void sort();
};
