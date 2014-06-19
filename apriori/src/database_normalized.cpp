/* author: Victor Roth Cardoso - vroth
 * student sponsored by CNPq
 * 
 * in summer project at Aberystwyth University
 * 
 * 
 * supervisors: Robert Hoehndorf and Georgios Gkoutos
 */
 
#include "database_normalized.hpp"

#include <iostream>
#include <cstring>

using std::cout;
using std::cerr;
using std::endl;
using std::pair;

DatabaseNormalized::DatabaseNormalized(char * filename) : filename(filename) {
	file.open(filename);
	if(!file.is_open()) {
		cerr << "Error opening \"" << filename << "\"" << endl;
	}
}

DatabaseNormalized::~DatabaseNormalized() {
	if(file.is_open()) {
		file.close();
	}
}

void DatabaseNormalized::processTransactions() {
	if(file.is_open()) {
		string line;
		char first[32], second[32];
		
		while (getline(file, line)) {
			map <string, bool> new_transaction;
			
			if(line.find('\t') != string::npos) {
				sscanf(line.c_str(), "%s %s", first, second);
			}
			//cout << "F: <" << first << "> S: <" << second << ">" << endl;
			
			map <string, map<string, bool>>::iterator it = transactions.find(first);
			
			if(it != transactions.end()) {
				it->second.insert(pair<string, bool>(second, true));
			}
			else {
				new_transaction.insert(pair<string, bool>(second, true));
				transactions.insert(pair<string, map<string, bool>>(first, new_transaction));
			}
		}
	}
	else {
		cerr << "File is not open -- processTransactions()" << endl;
	}
}

void DatabaseNormalized::processNormalizedTransactions() {
	if(file.is_open()) {
		string line;
		char first[32], second[32];
		
		while (getline(file, line)) {
			if(line.find('\t') != string::npos) {
				sscanf(line.c_str(), "%s %s", first, second);
			}
			
			normalized_transactions.insert(normalized_transactions.end(), pair<string, string> (string(first), string(second)));
		}
	}
	else {
		cerr << "File is not open -- processNormalizedTransactions()" << endl;
	}
}

/**this function is VERY slow, probably because of a massive number of /delete/s in map
 * another solution is used, involving the use of sort and uniq before accessing the file
*/
void DatabaseNormalized::removeDuplicates() {
	string previous("");
	for(unsigned int i=0; i<normalized_transactions.size(); i++) {
		string current = normalized_transactions.at(i).second;
		if(strcmp(previous.c_str(), current.c_str()) == 0) {
			normalized_transactions.erase(normalized_transactions.begin() + i);
			--i;
		}
		previous = current;
	}
}

bool normalizedCompare(const pair<string, string>& firstElem, const pair<string, string>& secondElem) {
	if(firstElem.first == secondElem.first) {
		return firstElem.second < secondElem.second;
	}
	else
		return firstElem.first < secondElem.first;
}

void DatabaseNormalized::printTransactions() {
	for(auto i : transactions) {
		cout << i.first << ": ";
		for(auto j : i.second) {
			cout << j.first << " ";
		}
		cout << endl;
	}
}

void DatabaseNormalized::printNormalizedTransactions() {
	for(auto i : normalized_transactions) {
		cout << "(" << i.first << "," << i.second << ")" << endl;	
	}
}
