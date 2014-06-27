/* author: Victor Roth Cardoso - vroth
 * student sponsored by CNPq
 * 
 * in summer project at Aberystwyth University
 * 
 * 
 * supervisors: Robert Hoehndorf and Georgios Gkoutos
 */

/** a general class for loading a file with separated values for phenotypes
 */
 
#pragma once

#include <fstream>
#include <string>
#include <map>
#include <vector>

using std::ifstream;
using std::map;
using std::string;
using std::vector;
using std::pair;

class DatabaseNormalized {
	private:
		char * filename;
		ifstream file;
		map <string, map<string, bool>> transactions;
		vector <pair <string, string>> normalized_transactions;
		unsigned int amount_transactions;
		
	public:
		DatabaseNormalized(char * filename);
		~DatabaseNormalized();
		
		void removeDuplicates(); //remove in normalized_transactions duplicates //TOO SLOW!
		
		void processTransactions(); //not used!
		void processNormalizedTransactions();
		
		map <string, map<string, bool>> & getTransactions(); //function for debug purpouses
		vector <pair <string, string>> & getNormalizedTransactions();
		
		unsigned int getAmountTransactions(void);
		
		void printTransactions(); //print data in a proper manner for debug
		void printNormalizedTransactions();
};

bool normalizedCompare(const pair<string, string>& firstElem, const pair<string, string>& secondElem); //function to compare pairs with string type
