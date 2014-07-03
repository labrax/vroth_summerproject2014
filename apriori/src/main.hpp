/* author: Victor Roth Cardoso - vroth
 * student sponsored by CNPq
 * 
 * in summer project at Aberystwyth University
 * 
 * 
 * supervisors: Robert Hoehndorf and Georgios Gkoutos
 */
 
#pragma once

#include <cstdint>

#include "database_normalized.hpp"
#include "large.hpp"
#include "parameters.hpp"

#include "ontologies/ontology.hpp"

class Main {
	private:
		Parameters * parameters;

		DatabaseNormalized * database;
		vector<LargeItemSet *> largesets;
		
		Ontology * ontologies;

		uint64_t minimum_transactions;

	public:		
		Main(int argc, char * argv[]);
		~Main();
		
		void setup();
		void run();
};
