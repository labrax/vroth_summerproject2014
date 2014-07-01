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
#include <vector>

using std::ifstream;
using std::map;
using std::string;

using std::vector;
using std::pair;

class Ontology {
	private:
		char * filename;
		ifstream file;
		map<string, NodeOntology *> ontologies;
	public:
		Ontology(char * filename);
		~Ontology();
		
		void processOntologies();
		bool checkAncestorOneAnother(string ontologyA, string ontologyB);

		void print();
		
		vector <pair <string, string>> * getNewOntologies(vector <pair <string, string>> & transaction); //this function will return the new block of the transaction
		
};
