/* author: Victor Roth Cardoso - vroth
 * student sponsored by CNPq
 * 
 * in summer project at Aberystwyth University
 * 
 * 
 * supervisors: Robert Hoehndorf and Georgios Gkoutos
 */

#pragma once

//#include <iostream>
#include <string>

using std::string;

class Parameters {
	private:
		bool use_thread;
		bool use_ontology;
		
		bool preprocessed;
		
		string file;
		string ontologies_file;
		double min_support, max_support, confidence;
		
	public:
		Parameters(int argc, char * argv[]);
		~Parameters();
		
		static bool verbose;
		static bool debug; //TODO: use debug flag
		static unsigned int thread_number;
		
		const bool & useThread();
		const bool & useOntology();
		const bool & isPreprocessed();

		const string & phenotypesFile();
		const string & ontologiesFile();
		
		const double & getMinSupport();
		const double & getMaxSupport();
		
		const double & getConfidence();
};

void print_instructions();
