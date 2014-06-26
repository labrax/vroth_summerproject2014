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

using namespace std;

int main(int argc, char *argv[]) {
	if(argc < 4) { //check the parameters
		cerr << "Not enough parameters." << endl <<
		"Run with transactions file to open." << endl <<
		"./apriori <file> <support> <confidence>" << endl <<
		"support and confidence in a range 0.01 to 1" << endl
		;
		return -1;
	}
	
	char command[512]; //sort and remove duplicates for a file
	sprintf(command, "sort %s | uniq > .temp_%s", argv[1], argv[1]);
	system(command);
	
	sprintf(command, ".temp_%s", argv[1]);
	DatabaseNormalized a(command);
	
	sprintf(command, "rm .temp_%s", argv[1]); //remove temporary file
	system(command);
	
	double support = atof(argv[2]), confidence = atof(argv[3]);
	
	
	cout << "support: " << support << endl
		 << "confidence: " << confidence << endl;
	
	/*a.processTransactions(); //to store then in a map with difference transactions
	cout << a.getTransactions().size() << " transactions have been stored." << endl; */
		
	a.processNormalizedTransactions(); //to store in a <TID, item> manner
	cout << a.getNormalizedTransactions().size() << " normalized transactions have been obtained." << endl;
	
	std::sort(a.getNormalizedTransactions().begin(), a.getNormalizedTransactions().end(), normalizedCompare); //probably it will already be sorted, just in case this function is still here
	cout << "sorted!" << endl;
	
	//a.removeDuplicates(); //TOO SLOW, USING uniq IN CONSOLE
	//cout << "removed duplicates!" << endl;
	
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
	cout << "amount of transactions is " << a.getAmountTransactions() << endl;
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
	
	CandidateItemSet cis;
	LargeItemSet * large_2 = cis.apriori_gen(large_1);
	
	large_1->printinfo();
	large_2->printinfo();
	
	return 0;
}
