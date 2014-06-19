
#include "container.hpp"

Container::Container() {
	
}

Container::~Container() {
	
}

void Container::insert(List TYPE a) {
	listset.insert(listset.begin(), a);
}

List TYPE Container::pop(void) {
	if(!listset.empty()) {
		List TYPE a = listset.back();
		listset.pop_back();
		return a;
	}
	return List TYPE ();
}
