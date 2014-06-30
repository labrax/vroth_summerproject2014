/* author: Victor Roth Cardoso - vroth
 * student sponsored by CNPq
 * 
 * in summer project at Aberystwyth University
 * 
 * 
 * supervisors: Robert Hoehndorf and Georgios Gkoutos
 */

#pragma once

#include <string>
#include <vector>

using std::string;
using std::vector;

class NodeOntology {
	private:
		string identifier;
		string name;
		vector <NodeOntology *> parents;
	public:
		NodeOntology(string identifier, string name);
		~NodeOntology();

		string & getIdentifier();
		string & getName();
		
		void insertParent(NodeOntology *);
		bool isSon(string &);
		
		void print();
};
