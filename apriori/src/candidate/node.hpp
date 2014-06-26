/* author: Victor Roth Cardoso - vroth
 * student sponsored by CNPq
 * 
 * in summer project at Aberystwyth University
 * 
 * 
 * supervisors: Robert Hoehndorf and Georgios Gkoutos
 */

#pragma once

#include "../itemset.hpp"
#include <string>

#include <iostream>

using std::cerr;
using std::endl;

using std::string;

typedef enum { node, bucket_node, itemset_node } type;

class Node {
	private:
		static const type tp = node;
		unsigned int depth;
		string identifier;
	public:
		Node(unsigned int depth, string identifier) : depth(depth), identifier(identifier) {};
		~Node() {};
		virtual type getType() { return tp; };
		virtual void insertItemSet(ItemSet) { cerr << "error: inserting on Node class " << __FILE__ << ":" << __LINE__ << endl; };
		
		unsigned int getDepth() { return depth; };
		string & getIdentifier() { return identifier; };
};
