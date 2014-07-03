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

#include <cstdio>
#include <cstdlib>

using namespace std;

#ifndef _TEST_
int main(int argc, char *argv[]) {
	Main program(argc, argv);
	
	program.setup();
	program.run();
	
	return 0;
}
#endif

Main::Main(int argc, char * argv[]) {
	parameters = new Parameters(argc, argv);
}

Main::~Main() {
	delete(parameters);
}

void Main::setup() {
	if(parameters->isPreprocessed() == false) {
		char tempDir[32] = "apriori.XXXXXX";
		char * new_dir = mkdtemp(tempDir);

		char command[512]; //sort and remove duplicates for a file
		sprintf(command, "sort %s | uniq > %s/%s", parameters->phenotypesFile(), new_dir, parameters->phenotypesFile());
		system(command); //TODO: fetch value from system()

		sprintf(command, "%s/%s", new_dir, parameters->phenotypesFile());
		database = new DatabaseNormalized(command);

		sprintf(command, "rm -rf %s", new_dir); //remove temporary folder and files
		system(command);
	}
	else {
		database = new DatabaseNormalized(parameters->phenotypesFile());
	}
	
	database->processNormalizedTransactions(); //to store in a <TID, item> manner
	if(Parameters::verbose)
		cout << database->getNormalizedTransactions().size() << " normalized transactions obtained" << endl;
	
	ontologies = NULL;
	if(parameters->useOntology()) {
		ontologies = new Ontology(parameters->ontologiesFile());
		ontologies->processOntologies();
		if(Parameters::verbose)
			cout << "ontologies loaded" << endl;
		ontologies->appendOntologies(&database->getNormalizedTransactions());
		if(Parameters::verbose)
			cout << "ontologies inserted into transactions" << endl;
	}
	
	if(parameters->useOntology())
		if(Parameters::verbose)
			cout << database->getNormalizedTransactions().size() << " normalized transactions obtained with ontologies added" << endl;
	
	std::sort(database->getNormalizedTransactions().begin(), database->getNormalizedTransactions().end(), normalizedCompare); //probably it will already be sorted, just in case this function is still here
	if(Parameters::verbose)
		cout << "transactions sorted" << endl;
		
	//database->printNormalizedTransactions();
		
	/*a.removeDuplicates(); //TOO SLOW, USING uniq IN TERMINAL
	cout << "removed duplicates!" << endl;*/

	if(parameters->useThread())
		startThreadSettings(&database->getNormalizedTransactions()); //setup threads
	
	if(Parameters::verbose)
		cout << "Amount of transactions is " << database->getAmountTransactions() << endl;
	
	minimum_transactions = database->getAmountTransactions()*parameters->getSupport();
	if(Parameters::verbose)
		cout << "The minimum support is obtained with " << minimum_transactions  << " transactions" << endl;
}

void Main::run() {
	//!OBTAINING 1-ITEMSETS
	std::map<string, uint64_t> itemset_1;
	for(auto &i : database->getNormalizedTransactions()) {
		std::map<string, uint64_t>::iterator it = itemset_1.find(i.second);
		if(it != itemset_1.end()) {
			itemset_1.find(i.second)->second++;
		}
		else {
			itemset_1.insert(pair<string, uint64_t> (i.second, 1));
		}
	}
	
	//this part will remove the elements without the support

	for(auto it = itemset_1.begin(); it != itemset_1.end();) {
		if((*it).second/(double) database->getAmountTransactions() < parameters->getSupport()) {
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
	
	if(Parameters::verbose) {
		large_1->printinfo();
		large_1->print();
	}
	
	Rules rules(parameters->getConfidence());
	LargeItemSet * large_obtained = large_1; //every large obtained will be passed to Rules::addLarge; where it will be destroyed on the object end
	do {
		CandidateItemSet cis;
		LargeItemSet * large_temp;
		
		if(!parameters->useThread())
			large_temp = cis.apriori_gen(large_obtained); //TODO: verify with the threaded || TODO: add ontologies
		else
			large_temp = cis.apriori_genThreaded(large_obtained); //TODO: verify if there is gain in comparison without threads || TODO: add ontologies
		
		rules.addLarge(large_obtained);
		
		if(Parameters::verbose)
			cout << "large_temp->size() = " << large_temp->getItemSets().size() << endl;

		if(!parameters->useThread())
			large_obtained = cis.subset(large_temp, &database->getNormalizedTransactions(), minimum_transactions);
		else
			large_obtained = cis.subsetThreaded(large_temp, &database->getNormalizedTransactions(), minimum_transactions);
		
		delete(large_temp);
		
		if(Parameters::verbose)
			cout << "sorting results" << endl;
		large_obtained->sort();
		
		//if(Parameters::verbose) {
			large_obtained->printinfo();
			large_obtained->print();
		//}
	} while(large_obtained->getItemSets().size() > 0);
	
	rules.addLarge(large_obtained);
	
	rules.computeRules();
	rules.print();
	
	if(ontologies)
		delete(ontologies);
	
	delete(database);
}
