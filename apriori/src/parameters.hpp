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

class Parameters {
	private:
		bool use_thread;
		bool use_ontology;
		
		bool preprocessed;
		
		char * file;
		char * ontologies_file;
		double support, confidence;
		
	public:
		Parameters(int argc, char * argv[]);
		~Parameters();
		
		static bool verbose;
		static bool debug;
		static unsigned int thread_number;
		
		const bool & useThread();
		const bool & useOntology();
		const bool & isPreprocessed();
		const char * phenotypesFile();
		const char * ontologiesFile();
		
		const double & getSupport();
		const double & getConfidence();
};
