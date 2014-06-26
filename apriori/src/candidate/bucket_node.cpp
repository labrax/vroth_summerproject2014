/* author: Victor Roth Cardoso - vroth
 * student sponsored by CNPq
 * 
 * in summer project at Aberystwyth University
 * 
 * 
 * supervisors: Robert Hoehndorf and Georgios Gkoutos
 */

#include "bucket_node.hpp"

#include <iostream>

using std::cerr;
using std::endl;

void BucketNode::insertItemSet(ItemSet set) {
	//TODO: create insertItemSet function
	
	/*
	 * -> hash to the right position
	 * -> pass it to the child
	 */
	 
	 
}

void BucketNode::updateChild(Node * dest, Node * orig) {
	unordered_map<string, Node>::iterator it = children.find(orig.getIdentifier());
	if(it != children.end()) { //TODO: TEST for speed, this might be slow for many iteractions where this copy involves many allocs and deallocs
		children.erase(it);
		children.insert(children.end(), std::pair<string, Node> (dest.getIdentifier(), dest));
	}
	else {
		//ERROR
		cerr << "child not found on father: " << __FILE__ << ":" << __LINE__ << endl;
	}
}
