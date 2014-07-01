/* author: Victor Roth Cardoso - vroth
 * student sponsored by CNPq
 * 
 * in summer project at Aberystwyth University
 * 
 * 
 * supervisors: Robert Hoehndorf and Georgios Gkoutos
 */

#include "node_ontology.hpp"

#include <iostream>

using std::cout;
using std::endl;

using std::pair;

NodeOntology::NodeOntology(string identifier, string name) : identifier(identifier), name(name) {
	parents.clear();
}

NodeOntology::~NodeOntology() {
	
}

string & NodeOntology::getIdentifier() {
	return identifier;
}

string & NodeOntology::getName() {
	return name;
}

void NodeOntology::insertParent(NodeOntology * parent) {
	parents.insert(parents.end(), parent);
}

bool NodeOntology::isSon(string & identifier) {
	if(identifier == this->identifier)
		return true;

	for(auto & i: parents) {
		if(i->isSon(identifier))
			return true;
	}
	return false;
}

void NodeOntology::print() {
	cout << identifier << ": \"" << name << "\" ";
	for(auto & p : parents) {
		cout << p->identifier << " ";
	}
	cout << endl;
}

map <string, bool> * NodeOntology::returnOntologies() {
	map <string, bool> * retOntol = new map<string, bool>();
	retOntol->insert(pair<string, bool> (identifier, true));
	
	if(parents.size() != 0) {	
		for(auto & p : parents) {
			map <string, bool> * newRetOntol = p->returnOntologies();
			for(auto & e : *newRetOntol) {
				if(retOntol->find(e.first) == retOntol->end()) {
					retOntol->insert(pair <string, bool>(e.first, true));
				}
			}
			delete(newRetOntol);
		}
	}
	return retOntol;
}
