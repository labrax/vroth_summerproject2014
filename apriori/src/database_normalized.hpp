
#pragma once

#include <fstream>
#include <string>
#include <map>

using std::ifstream;
using std::map;
using std::string;

class DatabaseNormalized {
	private:
		ifstream file;
		map <string, map<string, bool>> transactions;
	public:
		DatabaseNormalized(char * file);
		~DatabaseNormalized();
		map <string, map<string, bool>> & getTransactions() { return transactions; };
};
