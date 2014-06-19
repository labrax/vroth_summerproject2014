/* author: Victor Roth Cardoso - vroth
 * student sponsored by CNPq
 * 
 * in summer project at Aberystwyth University
 * 
 * 
 * supervisors: Robert Hoehndorf and Georgios Gkoutos
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
	public:
		DatabaseNormalized(char * filename);
		~DatabaseNormalized();
		
		void processTransactions();
		void processNormalizedTransactions();
		void removeDuplicates(); //TOO SLOW!
		
		map <string, map<string, bool>> & getTransactions() { return transactions; }; //function for debug purpouses
		vector <pair <string, string>> & getNormalizedTransactions() { return normalized_transactions; };
};

bool normalizedCompare(const pair<string, string>& firstElem, const pair<string, string>& secondElem);
