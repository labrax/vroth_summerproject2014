/* author: Victor Roth Cardoso - vroth
 * student sponsored by CNPq
 * 
 * in summer project at Aberystwyth University
 * 
 * 
 * supervisors: Robert Hoehndorf and Georgios Gkoutos
 */

#pragma once

#include "node_ontology.hpp"

#include <fstream>
#include <map>
#include <string>

using std::ifstream;
using std::map;
using std::string;

class Ontology {
	private:
		char * filename;
		ifstream file;
		map<string, NodeOntology *> ontologies;
	public:
		Ontology(char * filename);
		~Ontology();
		
		void processOntologies();
		void print();
};
