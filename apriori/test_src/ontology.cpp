
#include "../src/ontologies/ontology.hpp"

#include <iostream>

#include <vector>

using std::vector;
using std::pair;

using std::cout;
using std::endl;

int main(int argc, char * argv[]) {
	Ontology a(argv[1]);
	a.processOntologies();
	//a.print();
	
	vector<pair<string, string>> tra;
	tra.insert(tra.end(), pair<string, string> ("TRANSACTION A", "MP:0020197"));
	tra.insert(tra.end(), pair<string, string> ("TRANSACTION A", "MP:0020200"));
	tra.insert(tra.end(), pair<string, string> ("TRANSACTION B", "MP:0020200"));
	tra.insert(tra.end(), pair<string, string> ("TRANSACTION C", "MP:0002111"));
	tra.insert(tra.end(), pair<string, string> ("TRANSACTION C", "MP:0000180"));
	tra.insert(tra.end(), pair<string, string> ("TRANSACTION D", "MP:0000592"));
	
	cout << "initial: " << tra.size() << endl;
	for(auto & i : tra) {
		cout << i.first << " " << i.second << endl;
	}
	
	a.appendOntologies(&tra);
	
	cout << "final: " << tra.size() << endl;
	for(auto & i : tra) {
		cout << i.first << " " << i.second << endl;
	}
	
	
	/*for(auto & i : tra) {
		for(auto & j: tra) {
			cout << i.second << ".checkAncestorOneAnother(" << j.second << ") = ";
			if(a.checkAncestorOneAnother(i.second, j.second))
				cout << "true" << endl;
			else
				cout << "false" << endl;
		}
	}*/
	
	return 0;
}
