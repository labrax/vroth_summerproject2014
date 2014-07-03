/* author: Victor Roth Cardoso - vroth
 * student sponsored by CNPq
 * 
 * in summer project at Aberystwyth University
 * 
 * 
 * supervisors: Robert Hoehndorf and Georgios Gkoutos
 */

#include "ontology.hpp"
#include "node_ontology.hpp"

#include <iostream>
#include <cstdint>

using std::cerr;
using std::cout;
using std::endl;

using std::pair;

Ontology::Ontology(const char * filename) : filename(filename) {
	ontologies.clear();
	file.open(filename);
	if(!file.is_open()) {
		cerr << "Error opening \"" << filename << "\" ontologies file" << endl;
	}
	processed = false;
}

Ontology::~Ontology() {
	for(auto & o : ontologies) {
		delete(o.second);
	}
}

void Ontology::processOntologies() {
	if(file.is_open()) {
		string line;
		
		NodeOntology * nNode;
		string id, name;
		while (getline(file, line)) {
			unsigned int twopoints_pos = line.find_first_of(':');
			
			if(line == "[Term]") { //TODO: bug with [Typedef] in the end of file read (need to ignore that value)
				id = "";
				name = "";
			}
			else if(line.substr(0, twopoints_pos) == "id") {
				id = line.substr(twopoints_pos+2);
			}
			else if(line.substr(0, twopoints_pos) == "name") {
				name = line.substr(twopoints_pos+2);
				
				//cout << ">" << id << " " << name << endl;
				
				map<string, NodeOntology *>::iterator it = ontologies.find(id);
				if(it == ontologies.end()) {
					//cout << "doesnt exist" << endl;
					nNode = new NodeOntology(id, name);
					ontologies.insert(pair<string, NodeOntology *>(id, nNode));
				}
				else {
					//cout << "exists" << endl;
					nNode = it->second;
				}
			}
			else if(line.substr(0, twopoints_pos) == "is_a") {
				unsigned int exclamation_pos = line.find_first_of('!');
				//cout << "<" << line.substr(twopoints_pos+2, exclamation_pos-(twopoints_pos+3)) << "> <" << line.substr(exclamation_pos+2) << ">" << endl;
				
				map<string, NodeOntology *>::iterator it = ontologies.find(line.substr(twopoints_pos+2, exclamation_pos-(twopoints_pos+3)));
				if(it == ontologies.end()) {
					NodeOntology * father = new NodeOntology(line.substr(twopoints_pos+2, exclamation_pos-(twopoints_pos+3)), line.substr(exclamation_pos+2));
					ontologies.insert(pair<string, NodeOntology *>(line.substr(twopoints_pos+2, exclamation_pos-(twopoints_pos+3)), father));
					nNode->insertParent(father);
				}
				else {
					NodeOntology * father = it->second;
					nNode->insertParent(father);
				}
			}
		}
		processed = true;
	}
}

bool Ontology::checkAncestorOneAnother(string ontologyA, string ontologyB) {
	if(processed == false)
		cerr << "using ontologies without being loaded!" << endl;
	map<string, NodeOntology *>::iterator itA = ontologies.find(ontologyA);
	map<string, NodeOntology *>::iterator itB = ontologies.find(ontologyB);
	
	if(itA == ontologies.end() || itB == ontologies.end()) { //both doesn't exist on the relations
		return false;
	}
	else {
		if(itA->second->isSon(ontologyB))
			return true;
		else if(itB->second->isSon(ontologyA))
			return true;
		return false;
	}
}

void Ontology::appendOntologies(vector<pair<string, string>> * normalized_transactions) {
	if(processed == false)
		cerr << "using ontologies without being loaded!" << endl;
	uint64_t end=0, increased_size=0; //begin and end indicate the range of a transaction; increased_size the amount of new values inserted into normalized_transactions
	uint64_t initial_size = normalized_transactions->size();
	for(uint64_t begin=0; begin < initial_size; begin=end/*+increased_size*/) { //will scan transaction by transaction
		for(end=begin; end < normalized_transactions->size() && (*normalized_transactions)[end].first == (*normalized_transactions)[begin].first; ++end);
		
		vector<pair <string, string>>::const_iterator first = normalized_transactions->begin() + begin; //get the first element in a transaction
		vector<pair <string, string>>::const_iterator last = normalized_transactions->begin() + end; //get the last element in a transaction
		
		vector<pair <string, string>> transaction(first, last); //new sub-transaction
	
		vector<pair<string, string>> * newtra = getNewOntologies(transaction); //get new ontologies
	
		/*cout << "initial:" << transaction.size() << endl; //print the initial transaction
		for(auto & i : transaction) {
			cout << i.first << " " << i.second << endl;
		}
		
		cout << "additional: " << newtra->size() << endl; //print the new ontologies information
		for(auto & i : *newtra) {
			cout << i.first << " " << i.second << endl;
		} */
		
		/*//!merge the initial with the additional
		unsigned int i, j;
		increased_size = newtra->size();
		for(i=begin, j=0; j<increased_size; ) { //this will pass through normalized_transactions
			if(i < end && (*normalized_transactions)[i].second < (*newtra)[j].second) {
				i++;
			}
			else {
				(*normalized_transactions).insert((*normalized_transactions).begin() + i, (*newtra)[j]);
				j++;
			}
		}
		//!merge the initial with the additional*/
		
		increased_size = newtra->size();
		for(auto & i: *newtra) {
			normalized_transactions->insert(normalized_transactions->end(), i);
		}
		
		delete(newtra);
	}
}

void Ontology::print() {
	if(processed == false)
		cerr << "using ontologies without being loaded!" << endl;
	for(auto & o : ontologies) {
		o.second->print();
	}
}

vector <pair <string, string>> * Ontology::getNewOntologies(vector <pair <string, string>> & transaction) {
	if(processed == false)
		cerr << "using ontologies without being loaded!" << endl;
	map<string, bool> * newOntologies = new map <string, bool>();
	
	for(auto &t : transaction) {
		map<string, bool> * newOntologiesElement;
		
		map<string, NodeOntology *>::iterator it = ontologies.find(t.second);
		if(it != ontologies.end()) {
			newOntologiesElement = it->second->returnOntologies();
			
			for(auto & ne : *newOntologiesElement) { //go through the new "list" of elements
				if(newOntologies->find(ne.first) == newOntologies->end()) { //if it is not on the list known, insert
					newOntologies->insert(ne);
				}
			}
			
			delete(newOntologiesElement);
		}
	}
	
	vector <pair <string, string>> * newTransactions = new vector<pair <string, string>>();
	for(auto & no : *newOntologies) {
		bool ok = true;
		for(auto & tra : transaction) {
			if(tra.second == no.first) {
				ok = false;
				break;
			}
		}
		if(ok == true)
			newTransactions->insert(newTransactions->end(), pair<string, string>(transaction[0].first, no.first));
	}
	delete(newOntologies);
	
	return newTransactions;
}
