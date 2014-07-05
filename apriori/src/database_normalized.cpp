/* author: Victor Roth Cardoso - vroth
 * student sponsored by CNPq
 * 
 * in summer project at Aberystwyth University
 * 
 * 
 * supervisors: Robert Hoehndorf and Georgios Gkoutos
 */
 
#include "database_normalized.hpp"
#include "parameters.hpp"

#include <iostream>
#include <cstring>
#include <unordered_map>

#include <cstdint>

using std::cout;
using std::cerr;
using std::endl;
using std::pair;

DatabaseNormalized::DatabaseNormalized(string filename) : filename(filename) {
	file.open(filename);
	if(!file.is_open()) {
		cerr << "Error opening \"" << filename << "\" transactions file" << endl;
	}
	amount_transactions = 0;
}

DatabaseNormalized::~DatabaseNormalized() {
	if(file.is_open()) {
		file.close();
	}
}

void DatabaseNormalized::processNormalizedTransactions() {
	string str_count=""; //this will be used to count the amount of transactions
	amount_transactions=0;
	
	static uint64_t buggy_line=0; //count the amount of "buggy lines"
	
	if(file.is_open()) {
		string line;
		char first[32], second[32];
		
		while (getline(file, line)) {
			first[0] = '\0';
			second[0] = '\0'; //this will avoid some errors
			
			if(line.find('\t') != string::npos) {
				sscanf(line.c_str(), "%s %s", first, second);
			}
			
			if(strlen(first) > 0 && strlen(second) > 0)
				normalized_transactions.insert(normalized_transactions.end(), pair<string, string> (string(first), string(second)));
			else {
				//cerr << "buggy line not inserted: (" << first << "," << second << ")" << endl;
				buggy_line++;
				continue; //so don't count it or anything
			}
			
			//!count the amount of transactions
			if(str_count == "") {
				str_count = first;
				amount_transactions++;
			}
			else if(str_count != first){
				str_count = first;
				amount_transactions++;
			}
			//!count the amount of transactions
		}
		
		if(Parameters::verbose)
			cout << buggy_line << " buggy lines ignored" << endl;
	}
	else {
		cerr << "File is not open -- DatabaseNormalized::processNormalizedTransactions()" << endl;
	}
}

/**this function is VERY slow, probably because of a massive number of /delete/s in map
 * another solution is used, involving the use of sort and uniq before accessing the file
*/
void DatabaseNormalized::removeDuplicates() {
	string previous("");
	for(uint64_t i=0; i<normalized_transactions.size(); i++) {
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

vector <pair <string, string>> & DatabaseNormalized::getNormalizedTransactions() {
	return normalized_transactions;
}
		
uint64_t DatabaseNormalized::getAmountTransactions(void) {
	return amount_transactions;
}

void DatabaseNormalized::printNormalizedTransactions() {
	for(auto i : normalized_transactions) {
		cout << "(" << i.first << "," << i.second << ")" << endl;	
	}
}
