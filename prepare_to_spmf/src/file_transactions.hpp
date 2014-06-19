/* author: Victor Roth Cardoso - vroth
 * student sponsored by CNPq
 * 
 * in summer project at Aberystwyth University
 * 
 * 
 * supervisors: Robert Hoehndorf and Georgios Gkoutos
 */

/** This class extracts data from a file to prepare to use in spmf program for data mining
 * ref: http://www.philippe-fournier-viger.com/spmf/
 * 
 */

#pragma once

#include <fstream>
#include <string>
#include <map>

using std::map;
using std::string;

class FileTransactions {
	private:
		std::ifstream file;
		map <string, map<string, bool>> transactions;
	protected:
	public:
		FileTransactions(char *);
		~FileTransactions();
		map <string, map<string, bool>> & getTransactions();
		bool run();
};
