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
#include <map>

using std::string;
using std::vector;

using std::map;

class NodeOntology {
	private:
		string identifier;
		string name;
		bool is_obsolete;
		vector <NodeOntology *> parents;
		
		vector <NodeOntology *> children;
		unsigned int height, depth;
	public:
		NodeOntology(string identifier, string name, bool is_obsolete);
		~NodeOntology();

		string & getIdentifier();
		string & getName();
		
		void insertParent(NodeOntology *); //will be used for height
		void insertChild(NodeOntology *); //this is an additional relation for the use of depth
		
		const unsigned int getAmountParents();
		const unsigned int getAmountChildren();
		
		const unsigned int & getDepth();
		void setDepth(const unsigned int &);
		
		const unsigned int & getHeight();
		void setHeight(const unsigned int &);
		
		bool isObsolete();
		
		bool isSon(string &); //will return true for a check of the same value!
		
		void print();
		
		map <string, bool> * returnOntologies();
};
