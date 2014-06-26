/* author: Victor Roth Cardoso - vroth
 * student sponsored by CNPq
 * 
 * in summer project at Aberystwyth University
 * 
 * 
 * supervisors: Robert Hoehndorf and Georgios Gkoutos
 */

#include "node.hpp"

using std::pair;

Node::Node(unsigned int depth, string identifier, Node * father) {
	this->depth = depth;
	this->identifier = identifier;
	this->father = father;
	tp = node;
}

Node::~Node() {
	/*if(tp == bucket_node) {
		for(auto &i : children)
			delete(i.second);
	}
	else {
		for(auto &i : itemsets) {
			delete(i);
		}
	}*/
}

type Node::getType() {
	 return tp;
}

void Node::insertItemSet(ItemSet * a) {
	if(tp == node) { //not initialized
		tp = itemset_node;
	}
	else if(tp == itemset_node) { //itemset_node mode
		if(itemsets.size() <= BUCKET_THRESHOLD) { //stay itemset_node
			itemsets.insert(itemsets.end(), a); //there is no need to check for duplicates (ref: algorithm)
		}
		else { //transform into bucket_node!
			tp = bucket_node;
			for(auto &i : itemsets) {
				//will hash on the nth item
				unordered_map<string, Node*>::iterator try_existing = children.find(i->getNthString(depth));
				if(try_existing != children.end()) {
					try_existing->second->insertItemSet(a);
				}
				else {
					Node * child = new Node(depth+1, i->getNthString(depth), this);
					children.insert(pair<string, Node*>(i->getNthString(depth), child)); //initialize new node!
					child->insertItemSet(a);
				}
			}
			itemsets.clear(); //TODO: test if bug
			this->insertItemSet(a); //will hit the next block
		}
	}
	else { //bucket_node mode
		unordered_map<string, Node*>::iterator find = children.find(a->getNthString(depth));
		if(find != children.end()) { //if there is something on the hash destination
			find->second->insertItemSet(a);
		}
		else { //if there is nothing, need to add an element
			Node * child = new Node(depth+1, a->getNthString(depth), this);
			children.insert(pair<string, Node*>(a->getNthString(depth), child));
			child->insertItemSet(a);
		}
	}
}

unsigned int Node::getDepth() {
	return depth;
}

string & Node::getIdentifier() {
	return identifier;
}
