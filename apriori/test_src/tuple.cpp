//g++ -Wall -std=c++11 tuple.cpp

/** this test is to make sure on how to use tuple and struct
 */

#include <iostream>
#include <vector>
#include <tuple>
#include <string>

using std::vector;
using std::tuple;

using std::string;

using std::get;

using std::cout;
using std::endl;

typedef struct {
	unsigned int ui;
	char c;
} measures;

int main(int argc, char * argv[]) { //TODO: implement this in the real code
	vector<tuple<measures, string, string>> v;
	
	measures m;
	m.ui = 0;
	m.c = 'a';
	
	v.insert(v.end(), tuple<measures, string, string>(m, string("first_A"), string("first_B")));
	
	m.ui = 1;
	m.c = 'b';
	v.insert(v.end(), tuple<measures, string, string>(m, string("second_A"), string("second_B")));
	
	for(auto & i: v) {
		cout << get<0>(i).ui << " " << get<0>(i).c << " " << get<1>(i) << " " << get<2>(i) << endl;
	}
	return 0;
}
