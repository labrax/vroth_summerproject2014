/* author: Victor Roth Cardoso - vroth
 * student sponsored by CNPq
 * 
 * in summer project at Aberystwyth University
 * 
 * 
 * supervisors: Robert Hoehndorf and Georgios Gkoutos
 */
 
#include "database_normalized.hpp"
#include "itemset.hpp"
#include "large.hpp"
#include "candidate/candidate.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <map>
#include <unordered_map>
#include <vector>
#include <algorithm>

#include <cstdio>
#include <cstdlib>

using namespace std;

int main(int argc, char *argv[]) {
	if(argc < 4) { //check the parameters
		cerr << "Not enough parameters." << endl <<
		"Run with transactions file to open." << endl <<
		"./apriori <file> <support> <confidence> <threaded>" << endl <<
		"support and confidence in a range 0.01 to 1" << endl <<
		"threaded yes or no" << endl
		;
		return -1;
	}
	
	char tempDir[32] = "apriori.XXXXXX";
	char * new_dir = mkdtemp(tempDir);
	
	char command[512]; //sort and remove duplicates for a file
	sprintf(command, "sort %s | uniq > %s/%s", argv[1], new_dir, argv[1]);
	system(command);
	
	sprintf(command, "%s/%s", new_dir, argv[1]);
	DatabaseNormalized a(command);
	
	sprintf(command, "rm -rf %s", new_dir); //remove temporary folder and files
	system(command);
	
	bool USE_THREAD;
	if(argv[4] == string("yes")) {
		USE_THREAD = true;
	}
	else
		USE_THREAD = false;

	if(USE_THREAD)
		cout << "Using threads!" << endl;
	else
		cout << "Not using threads!" << endl;
	
	double support = atof(argv[2]), confidence = atof(argv[3]);
	cout << "Using support: " << support << endl
		 << "Using confidence: " << confidence << endl;
	
	/*a.processTransactions(); //to store then in a map with difference transactions
	cout << a.getTransactions().size() << " transactions have been stored." << endl; */
		
	a.processNormalizedTransactions(); //to store in a <TID, item> manner
	cout << a.getNormalizedTransactions().size() << " normalized transactions obtained" << endl;
	
	std::sort(a.getNormalizedTransactions().begin(), a.getNormalizedTransactions().end(), normalizedCompare); //probably it will already be sorted, just in case this function is still here
	cout << "Transactions sorted!" << endl;
	
	//a.removeDuplicates(); //TOO SLOW, USING uniq IN CONSOLE
	//cout << "removed duplicates!" << endl;
	
	if(USE_THREAD)
		startThreadSettings(&a.getNormalizedTransactions()); //setup threads
		
	cout << "Amount of transactions is " << a.getAmountTransactions() << endl;
	unsigned int minimum_transactions = a.getAmountTransactions()*support;
	cout << "The minimum support is obtained with " << minimum_transactions  << " transactions" << endl;

	cout << "------" << endl;
	
	//!OBTAINING 1-ITEMSETS
	std::map<string, unsigned int> itemset_1;
	for(auto &i : a.getNormalizedTransactions()) {
		std::map<string, unsigned int>::iterator it = itemset_1.find(i.second);
		if(it != itemset_1.end()) {
			itemset_1.find(i.second)->second++;
		}
		else {
			itemset_1.insert(pair<string, unsigned int> (i.second, 1));
		}
	}
	
	//this part will remove the elements without the support

	for(auto it = itemset_1.begin(); it != itemset_1.end();) {
		if((*it).second/(double) a.getAmountTransactions() < support) {
			//cout << "eliminating " << (*it).first << ", " << (*it).second << " because support is " <<  (*it).second/(double) a.getAmountTransactions() <<  endl;
			
			it = itemset_1.erase(it); //prunning of the 1 itemsets
		}
		else {
			++it; //when its not eliminated may follow the list
		}
	}
	
	LargeItemSet * large_1 = new LargeItemSet(1);
	//this will print the count and transfer each item that /survived/
	for(auto &i : itemset_1) {
		//cout << i.first << " #SUP:" << i.second << " " << i.second/(double) a.getAmountTransactions() << endl;
		ItemSet * a = new ItemSet();
		a->insert(i.first);
		a->setSupportCount(i.second);
		
		large_1->insertSet(a);
	}
	//!OBTAINING 1-ITEMSETS

	large_1->printinfo();
	large_1->print();
	
	LargeItemSet * large_obtained = large_1;
	do {
		CandidateItemSet cis;
		LargeItemSet * large_temp = cis.apriori_gen(large_obtained);
		
		delete(large_obtained);
		cout << "large_temp->size() = " << large_temp->getItemSets().size() << endl;

		if(!USE_THREAD)
			large_obtained = cis.subset(large_temp, &a.getNormalizedTransactions(), minimum_transactions);
		else
			large_obtained = cis.subsetThreaded(large_temp, &a.getNormalizedTransactions(), minimum_transactions);
		
		delete(large_temp);
		
		large_obtained->sort();
		
		large_obtained->printinfo();
		large_obtained->print();
	} while(large_obtained->getItemSets().size() > 0);
	
	delete(large_obtained);
	
	return 0;
}
