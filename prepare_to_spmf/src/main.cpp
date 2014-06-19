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

#include <cstdio>
#include <cstring>

using std::cout;
using std::cerr;
using std::endl;

int main(int argc, char * argv[]) {
	if(argc < 2) { //parameters check
		cerr << "Not enough parameters" << endl
		<< "Run as:" << endl
		<< "./prepare_spmf <file>" << endl;
		return -1;
	}
	
	char command[512]; //prepare the input file, removing duplicates
	sprintf(command, "sort %s | uniq > .temp_%s", argv[1], argv[1]);
	system(command);

	sprintf(command, ".temp_%s", argv[1]);
	
	FileTransactions a(command);
	a.run();
	
	sprintf(command, "rm .temp_%s", argv[1]); //remove the temporary file
	system(command);
	
	for(auto &i : a.getTransactions()) {
		for(auto &j : i.second) {
			cout << atoi(j.first.substr(j.first.find(':')+1).c_str()) << " "; //output the data
			//cout << j.first << " ";
		}
		cout << endl;
	}
	
	return 0;
}
