//ECE474 Homework 2
//Due November 8, 2020

// #include <iostream>
// #include <fstream>
// #include <sstream>
// #include <string>
// #include <vector>

#include <bits/stdc++.h>
 
using namespace std;
enum Operation {ADD = '+', SUB = '-', MUL = '*', GT = '>', LT = '<', EQ = '='};

// string convertExpresion(vector<string> expression){
//     enum Operation op;s
//     op = expression[3];
//     //d = a + b -> {'d','=','a','+','b'}
//     //ADD #(.WIDTH(8))ADD0(a,b,d);
//     switch(op){
//         case ('+'): //Adder
//             return "ADD #(.WIDTH(" + /*WIDTH +*/ "))ADD" + /*MODULE NUMBER*/ + "(" + expression(2) + "," + expression(4) + "," + expression(0) + ");";
//             //break;
            
//         case ('-'): //Subtractor

//             break;
//         case '*': //Multiplier

//             break;
//         case '>': //Greater Than

//             break;
//         case '<': //Less Than

//             break;
//         case "==": //Equality

//             break;
//         case "": //Multiplexor

//             break;
//         case ">>": //Shift Right

//             break;
//         case "<<": //Shift Left

//             break;
//     }
    
//     return "test";
// }

int readFile(string filename){
    
    string line;
    ifstream myfile;
    myfile.open(filename);

    cout << "`timescal 1ns / 1ps";

    //while(getline(myfile, line)){
    while(getline(myfile, line,'\n')){
        cout << line << "\n";
        // istringstream split(line);
        vector<string> lineSplit;
        // string tmp;
        /*while(getline(check1, tmp, " ")){
            lineSplit.push_back
        }*/
        
        string token;
        string delimiter = " ";
        size_t pos = 0;
        do {
            pos = line.find(delimiter);
            token = line.substr(0, pos);
            lineSplit.push_back(token);
            cout << token << endl;
            line.erase(0, pos + delimiter.length());
            // if((pos = line.find(delimiter)) == string::npos){
            //     token = line.substr(0, pos);

            //     cout << token << endl;
            // }
        }
        while ((pos) != string::npos);
        
        // while ((pos = line.find(delimiter)) != string::npos) {
        //     token = line.substr(0, pos);
        //     lineSplit.push_back(token);
        //     cout << token << endl;
        //     line.erase(0, pos + delimiter.length());
        //     if((pos = line.find(delimiter)) == string::npos){
        //         token = line.substr(0, pos);

        //         cout << token << endl;
        //     }
        // }
        // boost::split(lineSplit, line, boost::is_any_of(" "));
    }

    return 1;
}



int main(int argc, const char * argv[]) {
    for (int i = 0; i < argc; ++i) 
        cout << i << ":" << argv[i] << "\n";
    ifstream myfile;
    myfile.open(argv[1]);
    if (myfile){
        cout << "file exists" << "\n";
        readFile(argv[1]);
    }
    
    else{
        cout << "file doesn't exist" << "\n";
        return 0;
    }
    cout << "Hello World!" << "\n";
    cout << readFile("Test");
    return 0;
}
