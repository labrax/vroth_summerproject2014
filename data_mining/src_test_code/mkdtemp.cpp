
/** the objective of this test is to make sure of the mkdtemp function
 */

#include <iostream>
#include <cstdlib>

using namespace std;

int main() {
    char temp[32] = "hueheuhueXXXXXX";

    char * dir;
    dir = mkdtemp(temp);

    cout << dir << endl;
}
