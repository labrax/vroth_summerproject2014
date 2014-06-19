
#pragma once

#include <fstream>
#include <string>
#include <map>

using std::ifstream;
using std::map;
using std::string;

class FileTransactions {
	private:
		ifstream file;
		map <string, map<string, bool>> transactions;
	protected:
	public:
		FileTransactions(char *);
		~FileTransactions();
		map <string, map<string, bool>> & getTransactions();
		bool run();
};
