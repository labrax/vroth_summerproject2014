/* author: Victor Roth Cardoso - vroth
 * student sponsored by CNPq
 * 
 * in summer project at Aberystwyth University
 * 
 * 
 * supervisors: Robert Hoehndorf and Georgios Gkoutos
 */

#include "file_transactions.hpp"

#include <iostream>
#include <cstring>

using std::cerr;
using std::endl;
using std::pair;

FileTransactions::FileTransactions(char * file_name) { //open file
	file.open(file_name);
	if(!file.is_open()) {
		cerr << "Error opening \"" << file_name << "\"" << endl;
	}
}

FileTransactions::~FileTransactions() {
	if(file.is_open()) {
		file.close();
	}
}
	
bool FileTransactions::run() {
	if(file.is_open()) {
		string line;
		char first[32], second[32];
		
		while (getline(file, line)) { //read line by line
			map <string, bool> new_transaction; //in case the transaction doesn't exist
			
			if(line.find('\t') != string::npos) { //verify for the separator - just to avoid some bugs
				sscanf(line.c_str(), "%s %s", first, second);
			}
			
			map <string, map<string, bool>>::iterator it = transactions.find(first); //will try to find the TID
			
			if(it != transactions.end()) { //if the transaction exists, add the new item
				it->second.insert(pair<string, bool>(second, true));
			}
			else { //if the transaction doesn't exist, insert the new transaction group
				new_transaction.insert(pair<string, bool>(second, true));
				transactions.insert(pair<string, map<string, bool>>(first, new_transaction));
			}
		}
		return true;
	}
	else {
		cerr << "File is not open." << endl;
		return false;
	}
}

map <string, map<string, bool>> & FileTransactions::getTransactions() {
	return transactions;
}
