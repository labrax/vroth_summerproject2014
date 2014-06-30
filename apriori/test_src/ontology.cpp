
#include "../src/ontologies/ontology.hpp"

#include <iostream>

int main(int argc, char * argv[]) {
	Ontology a(argv[1]);
	a.processOntologies();
	a.print();
	return 0;
}
