//g++ -std=c++11 -Wall -g sort.cpp ../src/database_normalized.cpp ../src/ontologies/ontology.cpp ../src/ontologies/node_ontology.cpp ../src/parameters.cpp

/** the objective of this test is to make sure of the expansion of the itemsets, and the correct sorting of it, without ignoring any pair of transactions
 */

#include "../src/database_normalized.hpp"
#include "../src/ontologies/ontology.hpp"

#include <vector>
#include <string>
#include <iostream>
#include <algorithm>

using std::string;
using std::pair;

using std::cout;
using std::endl;

int main(int argc, char * argv[]) {
	Ontology o(argv[1]);
	o.processOntologies();


	vector<pair<string, string>> tra;
	tra.insert(tra.end(), pair<string, string> ("TRANSACTION A", "MP:0020197"));
	tra.insert(tra.end(), pair<string, string> ("TRANSACTION A", "MP:0020200"));
	tra.insert(tra.end(), pair<string, string> ("TRANSACTION B", "MP:0020200"));
	tra.insert(tra.end(), pair<string, string> ("TRANSACTION C", "MP:0002111"));
	tra.insert(tra.end(), pair<string, string> ("TRANSACTION C", "MP:0000180"));
	tra.insert(tra.end(), pair<string, string> ("TRANSACTION D", "MP:0000592"));


	cout << "initial size: " << tra.size() << endl;
	o.appendOntologies(&tra);
	cout << "appended size: " << tra.size() << endl;
	
	std::sort(tra.begin(), tra.end(), normalizedCompare); //is working, as printed below
	
	cout << "---" << endl;
	
	for(auto & t : tra) {
		cout << t.first << "," << t.second << endl;
	}
	
	cout << "---" << endl;
	
	return 0;
}
