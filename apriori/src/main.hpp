/* author: Victor Roth Cardoso - vroth
 * student sponsored by CNPq
 * 
 * in summer project at Aberystwyth University
 * 
 * 
 * supervisors: Robert Hoehndorf and Georgios Gkoutos
 */
 
#pragma once

#include "database_normalized.hpp"
#include "large.hpp"

class Main {
	private:
		bool use_thread;
		unsigned int thread_number; //TODO: use Main::thread_number
		
		bool preprocessed;
		
		char * file;
		double support, confidence;


		DatabaseNormalized * database;
		vector<LargeItemSet *> largesets;

	public:
		bool verbose;
		Main(int argc, char * argv[]);
		~Main();
		
		void run();
};
