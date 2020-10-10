// Your First C++ Program

#include <iostream>
#include <fstream>

using namespace std;

int main(int argc, const char * argv[]) {
    for (int i = 0; i < argc; ++i) 
        cout << i << ":" << argv[i] << "\n";
    ofstream myfile;
    myfile.open(argv[1]);
    cout << "Hello World!" << "\n";
    return 0;
}
