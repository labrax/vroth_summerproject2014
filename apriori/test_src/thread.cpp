#include <string>
#include <iostream>
#include <thread>

using namespace std;

void task(string msg) {
    cout << "task says: " << msg;
}

int main() {
    thread * a;
    a = new thread(task, "hoho");
    thread t1(task, "hello");
    t1.join();
    a->join();
}
