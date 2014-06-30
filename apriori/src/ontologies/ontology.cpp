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
			
			if(line == "[Term]") {
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

void Ontology::print() {
	for(auto & o : ontologies) {
		o.second->print();
	}
}
