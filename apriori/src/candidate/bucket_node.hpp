/* author: Victor Roth Cardoso - vroth
 * student sponsored by CNPq
 * 
 * in summer project at Aberystwyth University
 * 
 * 
 * supervisors: Robert Hoehndorf and Georgios Gkoutos
 */

#pragma once

#include "node.hpp"
#include "itemset_node.hpp"
#include "../itemset.hpp"
#include <unordered_map>
#include <string>
#include <vector>

using std::unordered_map;
using std::string;

class BucketNode : public Node {
	private:
		static const type tp = bucket_node;
		unordered_map<string, Node> children; //note that the children will have reference to the father
	public:
		BucketNode(unsigned int depth, string identifier) : Node(depth, identifier) {};
		virtual type getType() { return tp; };
		
		virtual void insertItemSet(ItemSet);
		void updateChild(Node, Node);
};
