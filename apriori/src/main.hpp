/* author: Victor Roth Cardoso - vroth
 * student sponsored by CNPq
 * 
 * in summer project at Aberystwyth University
 * 
 * 
 * supervisors: Robert Hoehndorf and Georgios Gkoutos
 */
 
#pragma once

#include "database_normalized.hpp"
#include "large.hpp"

class Main {
	private:
		bool use_thread;
		
		bool preprocessed;
		
		char * file;
		char * ontologies_file;
		double support, confidence;


		DatabaseNormalized * database;
		vector<LargeItemSet *> largesets;

	public:
		static bool use_ontology;
		static bool verbose;
		static unsigned int thread_number;
		
		Main(int argc, char * argv[]);
		~Main();
		
		void run();
};
