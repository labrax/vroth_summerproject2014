/* author: Victor Roth Cardoso - vroth
 * student sponsored by CNPq
 * 
 * in summer project at Aberystwyth University
 * 
 * 
 * supervisors: Robert Hoehndorf and Georgios Gkoutos
 */
 
#include "main.hpp"
 
#include "database_normalized.hpp"
#include "itemset.hpp"
#include "large.hpp"
#include "candidate/candidate.hpp"
#include "rules.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <map>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <thread>

#include <cstdio>
#include <cstdlib>

using namespace std;

int main(int argc, char *argv[]) {
	Main program(argc, argv);
	
	program.run();
	
	return 0;
}

bool Main::verbose = false;
unsigned int Main::thread_number = 0;

Main::Main(int argc, char * argv[]) {
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
			 << "-p\t\t to indicate that the file is preprocessed" << endl
			 << "-s <support>\t to select the support (default 0.5)" << endl
			 << "-c <confidence>\t to select the confidence (default 0.8)" << endl
			 << "-t <number>\t to select threads (number is optional)" << endl
			 << "-v\t\t enable verbose" << endl
			 << endl;
		exit(-1);
	}
	
	cerr << "running with" << endl
		 << "file: " << file << " preprocessed: ";
	if(preprocessed)
		cerr << "true" << endl;
	else
		cerr << "false" << endl;
	cerr << "support: " << support << endl
		 << "confidence: " << confidence << endl;
	cerr << "threads: ";
	if(use_thread)
		cerr << "true";
	else
		cerr << "false";
	if(thread_number != 0)
		cerr << " with " << thread_number << " threads" << endl;
	else
		cerr << endl;
		
	cerr << "verbose: ";
	if(verbose)
		cerr << "true" << endl;
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

Main::~Main() {
	
}

void Main::run() {
	if(preprocessed == false) {
		char tempDir[32] = "apriori.XXXXXX";
		char * new_dir = mkdtemp(tempDir);

		char command[512]; //sort and remove duplicates for a file
		sprintf(command, "sort %s | uniq > %s/%s", file, new_dir, file);
		system(command);

		sprintf(command, "%s/%s", new_dir, file);
		database = new DatabaseNormalized(command);

		sprintf(command, "rm -rf %s", new_dir); //remove temporary folder and files
		system(command);
	}
	else {
		database = new DatabaseNormalized(file);
	}
	
	database->processNormalizedTransactions(); //to store in a <TID, item> manner
	if(verbose)
		cout << database->getNormalizedTransactions().size() << " normalized transactions obtained" << endl;
	
	std::sort(database->getNormalizedTransactions().begin(), database->getNormalizedTransactions().end(), normalizedCompare); //probably it will already be sorted, just in case this function is still here
	if(verbose)
		cout << "transactions sorted" << endl;
		
	/*a.removeDuplicates(); //TOO SLOW, USING uniq IN TERMINAL
	cout << "removed duplicates!" << endl;*/

	if(use_thread)
		startThreadSettings(&database->getNormalizedTransactions()); //setup threads
	
	if(verbose)	
		cout << "Amount of transactions is " << database->getAmountTransactions() << endl;
	
	unsigned int minimum_transactions = database->getAmountTransactions()*support;
	if(verbose)
		cout << "The minimum support is obtained with " << minimum_transactions  << " transactions" << endl;
		
	//!OBTAINING 1-ITEMSETS
	std::map<string, unsigned int> itemset_1;
	for(auto &i : database->getNormalizedTransactions()) {
		std::map<string, unsigned int>::iterator it = itemset_1.find(i.second);
		if(it != itemset_1.end()) {
			itemset_1.find(i.second)->second++;
		}
		else {
			itemset_1.insert(pair<string, unsigned int> (i.second, 1));
		}
	}
	
	//this part will remove the elements without the support

	for(auto it = itemset_1.begin(); it != itemset_1.end();) {
		if((*it).second/(double) database->getAmountTransactions() < support) {
			//cout << "eliminating " << (*it).first << ", " << (*it).second << " because support is " <<  (*it).second/(double) a.getAmountTransactions() <<  endl;
			
			it = itemset_1.erase(it); //prunning of the 1 itemsets
		}
		else {
			++it; //when its not eliminated may follow the list
		}
	}
	
	LargeItemSet * large_1 = new LargeItemSet(1);
	//this will print the count and transfer each item that /survived/
	for(auto &i : itemset_1) {
		//cout << i.first << " #SUP:" << i.second << " " << i.second/(double) a.getAmountTransactions() << endl;
		ItemSet * a = new ItemSet();
		a->insert(i.first);
		a->setSupportCount(i.second);
		
		large_1->insertSet(a);
	}
	//!OBTAINING 1-ITEMSETS
	
	if(verbose) {
		large_1->printinfo();
		large_1->print();
	}
	
	Rules rules(confidence);
	LargeItemSet * large_obtained = large_1; //every large obtained will be passed to Rules::addLarge; where it will be destroyed on the object end
	do {
		CandidateItemSet cis;
		LargeItemSet * large_temp;
		
		if(!use_thread)
			large_temp = cis.apriori_gen(large_obtained); //TODO: verify with the threaded
		else
			large_temp = cis.apriori_genThreaded(large_obtained); //TODO: verify if there is gain in comparison without threads
		
		rules.addLarge(large_obtained);
		
		if(verbose)
			cout << "large_temp->size() = " << large_temp->getItemSets().size() << endl;

		if(!use_thread)
			large_obtained = cis.subset(large_temp, &database->getNormalizedTransactions(), minimum_transactions);
		else
			large_obtained = cis.subsetThreaded(large_temp, &database->getNormalizedTransactions(), minimum_transactions);
		
		delete(large_temp);
		
		if(verbose)
			cout << "sorting results" << endl;
		large_obtained->sort();
		
		if(verbose) {
			large_obtained->printinfo();
			large_obtained->print();
		}
	} while(large_obtained->getItemSets().size() > 0);
	
	rules.addLarge(large_obtained);
	
	rules.computeRules();
	rules.print();
	
	delete(database);
}
