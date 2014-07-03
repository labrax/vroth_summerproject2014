//g++ -std=c++11 -Wall -g database.cpp ../src/database_normalized.cpp ../src/ontologies/ontology.cpp ../src/ontologies/node_ontology.cpp ../src/parameters.cpp

#include "../src/database_normalized.hpp"
#include "../src/ontologies/ontology.hpp"

#include <vector>
#include <string>
#include <iostream>
#include <algorithm>

#include <cstdint>

using std::string;
using std::pair;

using std::cout;
using std::endl;

//OBJECTIVE: of this test: try to get all the transactions without the MP:0000001, as there are 2 that are supposed not to have (as in the normal execution)

void printTransaction(vector<pair<string, string>> & transaction) {
	cout << transaction[0].first << ": ";
	for(auto & i: transaction) {
		cout << i.second << " ";
	}
	cout << endl;
}

bool testTransaction(vector<pair<string, string>> & transaction) {
	for(auto & i : transaction) {
		if(i.second == "MP:0000001")
			return true;
	}
	return false;
}

int main(int argc, char * argv[]) {
	DatabaseNormalized a(argv[1]);
	a.processNormalizedTransactions();
	
	cout << "initial size: " << a.getNormalizedTransactions().size() << endl;
	
	Ontology o(argv[2]);
	o.processOntologies();
	o.appendOntologies(&a.getNormalizedTransactions());
	
	cout << "appended size: " << a.getNormalizedTransactions().size() << endl;
	
	std::sort(a.getNormalizedTransactions().begin(), a.getNormalizedTransactions().end(), normalizedCompare);
	
	vector<pair <string, string>> * normalized_transactions = &a.getNormalizedTransactions();
	
	/*cout << "---printing everything---" << endl;
	for(auto & i : *normalized_transactions) {
		cout << i.first << "," << i.second << endl;
	}*/
	
	cout << "---testing---" << endl;
	
	uint64_t j=0;
	for(uint64_t i=0; i < normalized_transactions->size(); i=j) { //will scan transaction by transaction
		for(j=i; j < normalized_transactions->size() && (*normalized_transactions)[j].first == (*normalized_transactions)[i].first; ++j);
		
		vector<pair <string, string>>::const_iterator first = normalized_transactions->begin() + i; //get the first element in transaction
		vector<pair <string, string>>::const_iterator last = normalized_transactions->begin() + j; //get the last element in transaction
		
		vector<pair <string, string>> transaction(first, last);
		
		/*cout << endl << transaction[0].first << ": "; //would print all the transactions in blocks
		for(uint64_t b=0; b<transaction.size(); b++) {
			cout << transaction[b].second << endl;
		}*/
		
		//tests++;
		if(testTransaction(transaction) == false) {
			//printTransaction(transaction);
		}
	}
	
	return 0;
}
