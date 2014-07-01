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

using std::cerr;
using std::cout;
using std::endl;

using std::pair;

Ontology::Ontology(char * filename) : filename(filename) {
	ontologies.clear();
	file.open(filename);
	if(!file.is_open()) {
		cerr << "Error opening \"" << filename << "\" ontologies file" << endl;
	}
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
	}
}

bool Ontology::checkAncestorOneAnother(string ontologyA, string ontologyB) {
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

void Ontology::print() {
	for(auto & o : ontologies) {
		o.second->print();
	}
}

vector <pair <string, string>> * Ontology::getNewOntologies(vector <pair <string, string>> & transaction) {
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
