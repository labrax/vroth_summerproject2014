/* author: Victor Roth Cardoso - vroth
 * student sponsored by CNPq
 * 
 * in summer project at Aberystwyth University
 * 
 * 
 * supervisors: Robert Hoehndorf and Georgios Gkoutos
 */
 
#include "database_normalized.hpp"
#include "k_itemset.hpp"

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
	if(argc < 4) {
		cerr << "Not enough parameters." << endl <<
		"Run with transactions file to open." << endl <<
		"./apriori <file> <support> <confidence>" << endl <<
		"support and confidence in a range 0.01 to 1" << endl
		;
		return -1;
	}
	
	char command[512];
	sprintf(command, "sort %s | uniq > .temp_%s", argv[1], argv[1]);
	system(command);
	
	sprintf(command, ".temp_%s", argv[1]);
	DatabaseNormalized a(command);
	sprintf(command, "rm .temp_%s", argv[1]);
	system(command);
	
	double support = atof(argv[2]), confidence = atof(argv[3]);
	
	
	cout << "support: " << support << endl
		 << "confidence: " << confidence << endl;
	/*
	for(auto i : transactions) {
		cout << i.first << ": ";
		for(auto j : i.second) {
			cout << j.first << " ";
		}
		cout << endl;
	}
	*/
	
	/*a.processTransactions();
	cout << a.getTransactions().size() << " transactions have been stored." << endl; */
	
	a.processNormalizedTransactions();
	cout << a.getNormalizedTransactions().size() << " normalized transactions have been obtained." << endl;
	
	std::sort(a.getNormalizedTransactions().begin(), a.getNormalizedTransactions().end(), normalizedCompare);
	cout << "sorted!" << endl;
	//a.removeDuplicates(); //TOO SLOW, USING uniq IN CONSOLE
	cout << "removed duplicates!" << endl;
	
	//!OBTAINING 1-ITEMSETS
	std::unordered_map<string, unsigned int> itemset_1;
	for(auto &i : a.getNormalizedTransactions()) {
		std::unordered_map<string, unsigned int>::iterator it = itemset_1.find(i.second);
		if(it != itemset_1.end()) {
			itemset_1.find(i.second)->second++;
		}
		else {
			itemset_1.insert(pair<string, unsigned int> (i.second, 1));
		}
	}
	
	//this will print the count of each item
	for(auto &i : itemset_1) {
		cout << "(" << i.first << "," << i.second << ")" << endl;
	}
	//!OBTAINING 1-ITEMSETS
	
	
	
	for(auto i : a.getNormalizedTransactions()) { //debug
		//cout << "(" << i.first << "," << i.second << ")" << endl;	
	}
	
	return 0;
}
