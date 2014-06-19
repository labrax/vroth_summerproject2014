
#include "file_transactions.hpp"

#include <iostream>
#include <cstring>

using std::cerr;
using std::endl;
using std::pair;

FileTransactions::FileTransactions(char * file_name) {
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
