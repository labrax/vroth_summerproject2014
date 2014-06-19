#pragma once

#include <vector>
#include <string>
#include "lists.hpp"

#define TYPE <std::string>

class Container {
	private:
		std::vector<List TYPE> listset;
	public:
		Container();
		~Container();
		void insert(List TYPE);
		List TYPE pop(void);
		bool empty() { return listset.empty(); };
};
