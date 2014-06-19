
#include "database_normalized.hpp"
#include "itemset.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <map>
#include <map>
#include <vector>

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
	
	FileTransactions a(argv[1]);
	double support = atof(argv[2]), confidence = atof(argv[3]);
	
	a.run();
	
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
	
	cout << a.getTransactions().size() << " transactions have been stored." << endl;
	
	return 0;
}
