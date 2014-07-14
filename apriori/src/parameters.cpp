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

#include <getopt.h>
#define no_argument 0
#define required_argument 1
#define optional_argument 2

using std::cout;
using std::cerr;
using std::endl;

bool Parameters::debug = false;
bool Parameters::verbose = false;
unsigned int Parameters::thread_number = 0;

bool Parameters::dont_append_ontologies = false;

Parameters::Parameters(int argc, char * argv[]) {
	static struct option long_options[] = {
		{"dont-append-ontologies", no_argument, 0, 'a'}, //to have ontologies data for similarity semantic comparison without appending transaction data
		
		{"help",			no_argument, 0, 'h'},
		{"preprocessed",	no_argument, 0, 'p'},
		{"verbose",			no_argument, 0, 'v'},
		{"debug",			no_argument, 0, 'd'},
		
		{"file", 			required_argument, 0, 'f'},
		{"ontologies-file", required_argument, 0, 'o'},
		
		{"confidence", 		required_argument, 0, 'c'},
		
		{"minsupport", 		required_argument, 0, 'l'}, //TODO: use the amount directly
		{"maxsupport", 		required_argument, 0, 'u'},
		
		{"thread",			optional_argument, 0, 't'}, //TODO: change the way argument is obtained, so it can be spaced (as required_argument with a hack)
		{0, 0, 0, 0},
	};
	int option_index = 0;
    
    file.assign("");
    ontologies_file.assign("");
    use_thread = false;
    thread_number = 0;
    use_ontology = false;
    preprocessed = false;
    min_support = 0.5;
    max_support = 1;
    confidence = 0.8;
    
    int c = getopt_long(argc, argv, "ahpvdf:o:c:l:u:t::", long_options, &option_index);
    
    while(c != -1) {
		switch(c) {
			case 0:
				if(long_options[option_index].flag != 0)
					break;
				cout << "option " << long_options[option_index].name;
				if(optarg)
					cout << " with arg " << std::boolalpha << optarg << endl;
				break;
			case 'a':
				dont_append_ontologies = true;
				break;
			case 'h':
				print_instructions();
				break;
			case 'p':
				preprocessed = true;
				break;
			case 'v':
				verbose = true;
				break;
			case 'd':
				debug = true;
				break;
			case 'f':
				file.assign(optarg);
				break;
			case 'o':
				use_ontology = true;
				ontologies_file.assign(optarg);
				break;
			case 'c':
				confidence = atof(optarg);
				break;
			case 'l':
				min_support = atof(optarg);
				break;
			case 'u':
				max_support = atof(optarg);
				break;
			case 't':
				use_thread = true;				
				if(optarg) {
					int getpos;
					for(getpos = 0; (optarg[getpos] < '0' || optarg[getpos] > '9') && optarg[getpos] != ' '; ++getpos);
								
					if(isdigit(optarg[getpos]))
						thread_number = atoi(optarg + getpos);
				}
				break;
			default:
				print_instructions();
				break;
		}
		c = getopt_long(argc, argv, "ahpvdf:o:c:l:u:t::", long_options, &option_index);
	}
	
	if(optind < argc) {
		cerr << "Guessing the remaining parameters: " << endl;
		while(optind < argc) {
			string par(argv[optind++]);
			if(par.find(".obo") != string::npos) {
				cerr << "\tconsidering " << par << " as the ontologies file" << endl;
				ontologies_file = par;
				use_ontology = true;
			}
			else if(par.find(".txt") != string::npos) {
				cerr << "\tconsidering " << par << " as the phenotypes file" << endl;
				file = par;
			}
			else {
				cerr << "\tdoesn't know what " << par << " means" << endl;
			}
		}
	}

	if(confidence < 0 || confidence > 1) {
		print_instructions();
	}
	
	if(min_support < 0 || min_support > 1) {
		print_instructions();
	}
	
	if(max_support < 0 || max_support > 1) {
		print_instructions();
	}
	
	if(file == "") {
		print_instructions();
	}
	
	cerr << std::boolalpha;
	cerr << "running with" << endl
		 << "file: " << file << " preprocessed: " << preprocessed << endl;
	cerr << "min-support: " << min_support << endl
		 << "max-support: " << max_support << endl
		 << "confidence: " << confidence << endl;
	cerr << "threads: " << use_thread;
	if(thread_number != 0)
		cerr << " with " << thread_number << " threads" << endl;
	else
		cerr << endl;
		
	cerr << "verbose: " << verbose << endl;
	cerr << "debug: " << debug << endl;
		
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
				use_thread = false;
				thread_number = 0;
			}
		}
		
	if(dont_append_ontologies)
		cerr << "dont-append-ontologies: true" << endl;
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

const string & Parameters::phenotypesFile() {
	return file;
}

const string & Parameters::ontologiesFile() {
	return ontologies_file;
}

const double & Parameters::getMinSupport() {
	return min_support;
}

const double & Parameters::getMaxSupport() {
	return max_support;
}

const double & Parameters::getConfidence() {
	return confidence;
}

void print_instructions() {
	cerr << "use format:" << endl //TODO: rewrite print_instructions() with longer names
		 << "-f <file>\t to select the phenotypes file" << endl
		 << "-o <file>\t to select and use ontologies file" << endl
		 << "-p\t\t to indicate the phenotypes file is sorted with no duplicates" << endl
		 << "-l <min-support>\t to select the minimum support (default 0.5)" << endl
		 << "-u <max-support>\t to select the maximum support (default 1)" << endl
		 << "-c <confidence>\t to select the confidence (default 0.8)" << endl
		 << "-t <number>\t to select threads (number is optional)" << endl
		 << "-v\t\t enable verbose" << endl
		 << endl
		 << "support and confidence must be in range 0 to 1" << endl
		 << endl
		 << "options can be guessed, if:" << endl
		 << "the phenotypes file is .txt it doesn't need -f" << endl
		 << "the ontologies file is .obo it doesn't need -o" << endl
		 << endl;

	cerr << "example of use: " << endl
		 << "./apriori -f mousephenotypes.txt -o mammalian_phenotype.obo -t=20" << endl
		 << "./apriori mousephenotypes.txt mammalian_phenotype.obo -t -s 0.01 -c 0.3" << endl
		 << endl;
		 
	exit(-1);
}
