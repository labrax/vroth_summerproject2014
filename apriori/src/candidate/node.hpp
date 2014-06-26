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
#include <unordered_map>
#include <vector>

#define BUCKET_THRESHOLD 5

using std::string;
using std::unordered_map;
using std::vector;

typedef enum { node, bucket_node, itemset_node } type;

class Node {
	private:
		type tp;
		unsigned int depth;
		
		string identifier;
		
		Node * father;
		
		unordered_map<string, Node *> children; //will be used in the bucket_node mode
		vector<ItemSet *> itemsets; //will be used for the itemset_node mode
		
	public:
		Node(unsigned int, string, Node *);
		~Node();
		
		type getType();
		
		void insertItemSet(ItemSet *);
		
		unsigned int getDepth();
		string & getIdentifier();
};
