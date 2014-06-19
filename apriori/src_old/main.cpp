
#include <iostream>
#include <string>

#include "lists.hpp"
#include "container.hpp"

using std::cout;

int main(int argc, char ** argv) {
	Container a;
	List <std::string> l;
	l.insert("aaa");
	l.insert("bbb");
	
	a.insert(l);
	
	List <std::string> h = a.pop();
	
	while(!h.empty()) {
		cout << h.pop();
	}
	return 0;
}
