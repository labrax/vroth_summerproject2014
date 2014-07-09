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
		sprintf(command, "sort %s | uniq > %s/%s", parameters->phenotypesFile().c_str(), new_dir, parameters->phenotypesFile().c_str());
		system(command); //TODO: fetch value from system()

		sprintf(command, "%s/%s", new_dir, parameters->phenotypesFile().c_str());
		database = new DatabaseNormalized(string(command));

		sprintf(command, "rm -rf %s", new_dir); //remove temporary folder and files
		system(command);
	}
	else {
		database = new DatabaseNormalized(string(parameters->phenotypesFile()));
	}
	
	database->processNormalizedTransactions(); //to store in a <TID, item> manner
	
	if(parameters->useOntology()) {
		ontologies = new Ontology(parameters->ontologiesFile());
		ontologies->processOntologies();
		ontologies->appendOntologies(&database->getNormalizedTransactions());
		if(Parameters::debug)
			cout << "ontologies have been loaded and inserted into transactions" << endl;
		if(Parameters::verbose)
			cout << database->getNormalizedTransactions().size() << " normalized transactions obtained with ontologies added" << endl;
	}
	else {
		ontologies = new Ontology();
		if(Parameters::debug)
			cout << "ontologies are not being used" << endl;
		if(Parameters::verbose)
			cout << database->getNormalizedTransactions().size() << " normalized transactions obtained" << endl;
	}
	
	std::sort(database->getNormalizedTransactions().begin(), database->getNormalizedTransactions().end(), normalizedCompare); //probably it will already be sorted, just in case this function is still here
	if(Parameters::debug)
		cout << "transactions sorted" << endl;
		
	//database->printNormalizedTransactions();
		
	/*a.removeDuplicates(); //TOO SLOW, USING uniq IN TERMINAL
	cout << "removed duplicates!" << endl;*/
	
	if(Parameters::verbose)
		cout << "Amount of transactions is " << database->getAmountTransactions() << endl;
	
	min_transactions = database->getAmountTransactions()*parameters->getMinSupport();
	if(Parameters::verbose)
		cout << "The minimum support is obtained with " << min_transactions  << " transactions" << endl;
		
	max_transactions = database->getAmountTransactions()*parameters->getMaxSupport();
	if(Parameters::verbose)
		cout << "The maximum support is obtained with " << max_transactions  << " transactions" << endl;
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
		if((*it).second < min_transactions || (*it).second > max_transactions) {
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
	
	if(Parameters::verbose)
		large_1->printinfo();
	if(Parameters::debug)
		large_1->print();
	
	Rules rules(database->getAmountTransactions(), parameters->getConfidence(), ontologies);
	LargeItemSet * large_obtained = large_1; //every large obtained will be passed to Rules::addLarge; where it will be destroyed on the object end
	do {
		CandidateItemSet cis(*ontologies);
		LargeItemSet * large_temp;
		
		if(parameters->useThread())
			large_temp = cis.apriori_genThreaded(large_obtained);
		else
			large_temp = cis.apriori_gen(large_obtained);
		
		rules.addLarge(large_obtained);
		
		if(Parameters::debug)
			cout << "large_temp->size() = " << large_temp->getItemSets().size() << endl;

		if(parameters->useThread())
			large_obtained = cis.subsetThreaded(large_temp, &database->getNormalizedTransactions(), min_transactions, max_transactions);
		else
			large_obtained = cis.subset(large_temp, &database->getNormalizedTransactions(), min_transactions, max_transactions);
		
		delete(large_temp);
		
		if(Parameters::debug)
			cout << "sorting results" << endl;
		large_obtained->sort();
		
		if(Parameters::verbose)
			large_obtained->printinfo();
		if(Parameters::debug)
			large_obtained->print();
	} while(large_obtained->getItemSets().size() > 0);
	
	rules.addLarge(large_obtained);
	
	rules.computeRules();
	rules.print();
	
	if(ontologies)
		delete(ontologies);
	
	delete(database);
}
