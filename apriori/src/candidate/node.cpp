/* author: Victor Roth Cardoso - vroth
 * student sponsored by CNPq
 * 
 * in summer project at Aberystwyth University
 * 
 * 
 * supervisors: Robert Hoehndorf and Georgios Gkoutos
 */

#include "node.hpp"

#include <map>
#include <string>

#include <iostream>

using std::pair;
using std::map;
using std::string;

using std::cout;
using std::endl;

Node::Node(unsigned int depth, string identifier, Node * father) {
	this->depth = depth;
	this->identifier = identifier;
	this->father = father;
	
	itemsets.clear();
	children.clear();
	
	tp = itemset_node;
}

Node::~Node() {
	if(tp == bucket_node) {
		for(auto &i : children)
			delete(i.second);
	}
	else {
		for(auto &i : itemsets) {
			delete(i);
		}
	}
}

type Node::getType() {
	 return tp;
}

void Node::insertItemSet(ItemSet * a) {
	if(tp == node) { //not initialized
		tp = itemset_node;
		cout << "some sort of bug " << __FILE__ << ":" << __LINE__ << endl;
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
					try_existing->second->insertItemSet(i);
				}
				else {
					Node * child = new Node(depth+1, i->getNthString(depth), this);
					children.insert(pair<string, Node*>(i->getNthString(depth), child)); //initialize new node!
					child->insertItemSet(i);
				}
			}
			itemsets.clear(); //TODO: test if bug -- probably not, as it will only remove the pointers, not delete(it)!
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

bool Node::contains(ItemSet * a) {	
	if(tp == itemset_node) {
		for(auto &i: itemsets) {
			bool nice = true;
			for(auto &j : a->getItemSet()) {
				if(i->contains(j.first) == false) {
					nice = false;
					break;
				}
			}
			if(nice == true)
				return true;
		}
	}
	else if(tp == bucket_node) {
		for(auto &i: children) {
			if(i.second->contains(a) == true)
				return true;
		}
	}
	
	return false;
}

void Node::transactionScan(vector <pair <string, string>> * transaction) {
	if(tp == itemset_node) {
		for(auto &i : itemsets) {
			//if itemset is contained in transaction, it is cool
			
			bool cool = true; //for the itemset
			for(auto &e : i->getItemSet()) {
				bool found = false; //for each element
				for(unsigned int t=0; t<transaction->size(); t++) {
					if((*transaction)[t].second == e.first) { //element is in transaction
						found = true;
						break;
					}
				}
				if(found == false) {
					cool = false;
					break;
				}
			}
			if(cool == true) {
				i->increaseSupportCount();
				//cout << "updating support to " << i->getSupportCount() << endl;
			}
		}
	}
	else if(tp == bucket_node) {		
		/*
		for(auto &i : children) { //paranoid mode
			i.second->transactionScan(transaction);
		}
		*/
		
		for(unsigned int i=0; i<transaction->size(); i++) { //use the tree!
			unordered_map<string, Node*>::iterator find = children.find((*transaction)[i].second);
			
			if(find != children.end()) { //if there is something on the hash destination
				find->second->transactionScan(transaction);
			}
		}
	}
}

void Node::grabMinimumSupport(LargeItemSet * dest, unsigned int support) {
	if(tp == itemset_node) {
		for(auto &i : itemsets) {
			if(i->getSupportCount() > support) {
				ItemSet * e  = new ItemSet(i); //to avoid memory leak
				e->setSupportCount(i->getSupportCount());
				dest->insertSet(e);
				//cout << "found one good!" << endl;
			}
		}
	}
	else if(tp == bucket_node) {
		for(auto &i : children) {
			i.second->grabMinimumSupport(dest, support);
		}
	}
}
