/* author: Victor Roth Cardoso - vroth
 * student sponsored by CNPq
 * 
 * in summer project at Aberystwyth University
 * 
 * 
 * supervisors: Robert Hoehndorf and Georgios Gkoutos
 */

#include "parameters.hpp"

#include <iostream>
#include <thread>

#include <cstdio>
#include <cstring>

using std::cerr;
using std::endl;

bool Parameters::debug = false;
bool Parameters::verbose = false;
unsigned int Parameters::thread_number = 0;

Parameters::Parameters(int argc, char * argv[]) {
	bool error = false;
	
	static char default_file[16] = "phenotypes.txt";
	file = default_file;
	preprocessed = false;

	support = 0.5;
	confidence = 0.8;
	
	use_thread = false;
	
	for(int i = 0; i<argc; i++) {
		if(strcmp(argv[i], "-f") == 0) {
			if(i+1 >= argc) {
				error = true;
				break;
			}
			else {
				file = argv[i+1];
			}
		}
		else if(strcmp(argv[i], "-o") == 0) {
			use_ontology = true;
			if(i+1 >= argc) {
				error = true;
				break;
			}
			else {
				ontologies_file = argv[i+1];
			}
		}
		else if(strcmp(argv[i], "-s") == 0) {
			if(i+1 >= argc) {
				error = true;
				break;
			}
			else {
				support = atof(argv[i+1]);
			}
		}
		else if(strcmp(argv[i], "-c") == 0) {
			if(i+1 >= argc) {
				error = true;
				break;
			}
			else {
				confidence = atof(argv[i+1]);
			}
		}
		else if(strcmp(argv[i], "-t") == 0) {
			use_thread = true;
			if(i+1 < argc) {
				if(isdigit(argv[i+1][0]))
					thread_number = atoi(argv[i+1]);
			}
		}
		else if(strcmp(argv[i], "-v") == 0) {
			verbose = true;
		}
		else if(strcmp(argv[i], "-p") == 0) {
			preprocessed = true;
		}
		else if(strcmp(argv[i], "-h") == 0) {
			error = true;
		}
	}
	
	if(confidence <= 0 || confidence > 1)
		error = true;
		
	if(support <= 0 || support > 1)
		error = true;
	
	if(error) {
		cerr << "use format:" << endl
			 << "-f <file>\t to select the phenotypes file (default phenotypes.txt)" << endl
			 << "-o <file>\t to select and use ontologies file" << endl
			 << "-p\t\t to indicate that the file is preprocessed" << endl
			 << "-s <support>\t to select the support (default 0.5)" << endl
			 << "-c <confidence>\t to select the confidence (default 0.8)" << endl
			 << "-t <number>\t to select threads (number is optional)" << endl
			 << "-v\t\t enable verbose" << endl
			 << endl;
		exit(-1);
	}
	
	cerr << std::boolalpha;
	cerr << "running with" << endl
		 << "file: " << file << " preprocessed: " << preprocessed << endl;
	cerr << "support: " << support << endl
		 << "confidence: " << confidence << endl;
	cerr << "threads: " << use_thread;
	if(thread_number != 0)
		cerr << " with " << thread_number << " threads" << endl;
	else
		cerr << endl;
		
	cerr << "verbose: " << verbose << endl;
		
	cerr << "ontologies: ";
	if(use_ontology)
		cerr << ontologies_file << endl;
	else
		cerr << "false" << endl;
	
	if(use_thread)
		if(thread_number == 0) {
			thread_number = std::thread::hardware_concurrency();
			if(thread_number == 0) {
				cerr << "Error initializing thread_number, running without threads!" << endl;
				thread_number = 0;
			}
		}
}

Parameters::~Parameters() {
	
}

const bool & Parameters::useThread() {
	return use_thread;
}

const bool &  Parameters::useOntology() {
	return use_ontology;
}

const bool & Parameters::isPreprocessed() {
	return preprocessed;
}

const char * Parameters::phenotypesFile() {
	return file;
}

const char * Parameters::ontologiesFile() {
	return ontologies_file;
}

const double & Parameters::getSupport() {
	return support;
}

const double & Parameters::getConfidence() {
	return confidence;
}
