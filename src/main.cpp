// Your First C++ Program

#include <iostream>
#include <fstream>

using namespace std;

int main(int argc, const char * argv[]) {
    for (int i = 0; i < argc; ++i) 
        cout << i << ":" << argv[i] << "\n";
    ifstream myfile;
    myfile.open(argv[1]);
    if (myfile){
        cout << "file exists" << "\n";
    }else{
        cout << "file doesn't exist" << "\n";
        return 0;
    }
    cout << "Hello World!" << "\n";
    return 0;
}
