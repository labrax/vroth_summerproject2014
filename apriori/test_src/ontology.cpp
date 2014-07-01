
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
	
	vector<pair<string, string>> * newtra = a.getNewOntologies(tra);
	
	cout << "initial:" << tra.size() << endl;
	for(auto & i : tra) {
		cout << i.first << " " << i.second << endl;
	}
	
	cout << "additional: " << newtra->size() << endl;
	for(auto & i : *newtra) {
		cout << i.first << " " << i.second << endl;
	}
	
	//!merge the initial with the additional
	unsigned int i, j;
	for(i=0, j=0; j<newtra->size(); ) {
		if(tra[i].second < (*newtra)[j].second) {
			i++;
		}
		else {
			tra.insert(tra.begin() + i, (*newtra)[j]);
			j++;
		}
	}
	//!merge the initial with the additional
	delete(newtra);
	
	cout << "final: " << tra.size() << endl;
	for(auto & i : tra) {
		cout << i.first << " " << i.second << endl;
	}
	
	for(auto & i : tra) {
		for(auto & j: tra) {
			cout << i.second << ".checkAncestorOneAnother(" << j.second << ") = ";
			if(a.checkAncestorOneAnother(i.second, j.second))
				cout << "true" << endl;
			else
				cout << "false" << endl;
		}
	}
	
	return 0;
}
