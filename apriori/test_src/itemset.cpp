//g++ -std=c++11 itemset.cpp ../src/itemset.cpp

/** testing code
 * 
 */

#include "../src/itemset.hpp"
#include <iostream>

using std::cout;
using std::endl;

int main(void) {
	ItemSet a;
	a.insert("A");
	a.insert("B");
	a.insert("C");
	
	if(a.contains(string("B"))) {
		cout << "contains B" << endl;
	}
	
	if(a.contains(string("D"))) {
		cout << "contains D" << endl;
	}
	
	if(a.contains(string("A"))) {
		cout << "contains A" << endl;
	}
	
	for(auto & i: a.subItemSets()) {
		cout << "-> ";
		for(auto &j: i->getItemSet()) {
			cout << j.first << " ";
		}
		cout << endl;
		delete(i);
	}
}
