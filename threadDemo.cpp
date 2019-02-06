#include <thread>
#include <iostream>

using namespace std;

void print(){
    cout << "Hello thread" << endl;
}

int main(){
    thread th(print);
    th.join();

    return 0;
}
