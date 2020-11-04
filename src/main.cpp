//ECE474 Homework 2
//Due November 8, 2020

 #include <iostream>
 #include <fstream>
 #include <sstream>
 #include <string>
 #include <vector>
 #include <unordered_map>

//#include <bits/stdc++.h>
#include <sstream>

using namespace std;
enum Operation { REG = 'R', ADD = '+', SUB = '-', MUL = '*', GT = '>', LT = '<', EQ = '=', SHR = 'r', SHL = 'l' };
unordered_map<string, vector<string>> inputs;
unordered_map<string, vector<string>> outputs;
unordered_map<string, vector<string>> wires;
unordered_map<string, vector<string>> registers;

string getBitWidthOut(string variable);
string getBitWidthIn(string input1, string input2);
string getNumber(string variable);
void writeVerilogFile(string verilogFile, vector<string> results);
string getVariableNames();
bool isItSigned(string variable);

int ModuleIndex = 0; //Global Variable for use in the function convertExpression
// Converts math expressions in the format of 
//  d = a + b
// to a string of the fo
//  ADD #(.WIDTH(<BitWidth>))ADD<ModuleIndex>(a,b,d);
// Supports the operations: REG, ADD, SUB, MUL, COMP, MUX2x1, SHR, SHL
string convertExpresion(vector<string> expression) {
	string CLK = "CLK";
	string RST = "RST";
	string result = "";
	ModuleIndex = ModuleIndex + 1;
	string bitWidth = "";
	if (expression.size() != 3) {
		bitWidth = getBitWidthIn(expression[0], expression[4]); //Bitwidth is determined by inputs
		string op = expression[3];
		//COMP; Comparison
		result += isItSigned(expression[0]) ? "SCOMP" : "COMP";
		if(op.compare(">") == 0)
			result += "#(.WIDTH(" + (bitWidth) + "))line" + to_string(ModuleIndex) + "("
			+ expression[2] + "," + expression[4] + "," + expression[0] +"," + "1'b0" +"," + "1'b0" + ");";

		else if (op.compare("<") == 0) 
			result += "#(.WIDTH(" + (bitWidth) + "))line" + to_string(ModuleIndex) + "("
			+ expression[2] + "," + expression[4] + "," +"1'b0" + "," + expression[0] + "," + "1'b0" + ");";
		
		else if(op.compare("==") == 0)
			result += "#(.WIDTH(" + (bitWidth) + "))line" + to_string(ModuleIndex) + "("
			+ expression[2] + "," + expression[4] + "," + "1'b0" + "," + "1'b0" + "," + expression[0] + ");";

		else {
			bitWidth = getBitWidthOut(expression[0]); //Bitwidth is determined by output or wire
			result = isItSigned(expression[0]) ? "S" : "";//Empty result
			//ADD; Addition
			if (op.compare("+") == 0) 
				result += "ADD ";
			
			//SUB; Subtraction
			else if (op.compare("-") == 0) 
				result += "SUB ";
			
			//MUL; Multiplication
			else if (op.compare("*") == 0)
				result += "MUL ";
			
			//SHR; Shift Right
			else if (op.compare(">>") == 0) 
				result += "SHR ";
			
			//SHL; Shift Left
			else if (op.compare("<<") == 0) 
				result += "SHL ";

			//MUX2x1; Multiplex from 2 to 1. Special Case
			else if (op.compare("?") == 0) {
				result = isItSigned(expression[0]) ? "S" : "";
				return result + "MUX2x1 #(.WIDTH(" + (bitWidth)+"))line" + to_string(ModuleIndex) + "("
					+ expression[4] + "," + expression[6] + "," + expression[2] + "," + expression[0] + ");";
			}

			result += "#(.WIDTH(" + (bitWidth) + "))line" + to_string(ModuleIndex) + "(" 
				+ expression[2] + "," + expression[4] + "," + expression[0] + ");";
		}
		return result;
	}
	//REG
	else {
		bitWidth = getBitWidthOut(expression[0]);
		return "REG #(.WIDTH(" + (bitWidth) + "))line" + to_string(ModuleIndex) + "(" + CLK + ","
			+ RST + "," + expression[2] + "," + expression[0] + ");";
	}

	return "UNEXPECTED EXPRESSION or ERROR\n";
}


/**
Given a variable name, seaches through the outputs "and wires" global variable vector 
to find the bit width of the parameter variable
*/
string getBitWidthOut(string variable){
	string bitWidth = "0";
	if (outputs.count(variable) > 0) 
		bitWidth = outputs[variable][1];
	else if (wires.count(variable) > 0) 
		bitWidth = wires[variable][2];
	else if (registers.count(variable) > 0)
		bitWidth = registers[variable][2];
	return bitWidth;
}

/*
Check if the output variable of an operations is signed 
or unsigned. CHECK if output of component determines if 
component is unsgined or signed.
*/
bool isItSigned(string variable) {
	bool signedV = false;
	string x = "s";
	if (outputs.count(variable) > 0)
		signedV = (x.compare(outputs[variable][0]) == 0);
	else if (wires.count(variable) > 0)
		signedV = (x.compare(wires[variable][1]) == 0);
	else if (registers.count(variable) > 0)
		signedV = (x.compare(registers[variable][0]) == 0);
	return signedV;
}

/**
Returns the highest bitwidth of COMP operation.
CHECK if COMP operations take inputs varaibles as 
inputs to the modules.
*/
string getBitWidthIn(string input1, string input2) {
	string bitWidth1 = "0", bitWidth2 = "0";
	if (wires.count(input1) > 0)
		bitWidth1 = wires[input1][2];
	else if (inputs.count(input1) > 0)
		bitWidth1 = inputs[input1][1];
	if (wires.count(input2) > 0)
		bitWidth2 = wires[input2][2];
	else if (inputs.count(input2) > 0)
		bitWidth2 = inputs[input2][1];
	return (bitWidth1 > bitWidth2) ? bitWidth1 : bitWidth2;
}

/**
Gets numbers from a string. (e.g. Int8 -> 8; Int64 -> 64;
Int128 -> 128; H3LL0 -> 30)
*/
string getNumber(string x) {
	string result = "";
	for (int i = 0; i < x.size(); i++) 
		if (isdigit(x[i]))
			result += x[i];
	return result;
}

string convertDeclaration(vector<string> input) {
	string declaration = "";
	string bits;
	int x;
	if (input.at(0) == "input") {
		declaration += "input ";
	}
	else if (input.at(0) == "output") {
		declaration += "output ";
	}
	else if (input.at(0) == "register") {
		declaration += "reg ";
	}
	else {
		declaration += "wire ";
	}
	if (input.at(1) != "Int1") {
		bits = input.at(1);
		bits = bits.substr(3, bits.size());
		stringstream degree(bits);
		degree >> x;
		x--;
		bits = to_string(x);
		string temp = "[" + bits + ":0] ";
		declaration += temp;
	}

	for (int i = 2; i < input.size(); i++) {
		declaration += input.at(i);
		if (i != input.size() - 1) {
			declaration += " ";
		}
	}
	declaration += ";";
	return declaration;
}

// inputs["a":<sign, bits>, "b":<sign, bit>]

int readFile(string input_filename, string output_filename= "verilogFile") {
	string tempString = "";
	string line;
	ifstream myfile; 
	vector<string> results;
	myfile.open(input_filename);

	//while(getline(myfile, line)){
	while (getline(myfile, line, '\n')) {
		cout << "line:" << line << "\n";
		vector<string> lineSplit;

		string token;
		string delimiter = " ";
		size_t pos = 0;
		do {
			pos = line.find(delimiter);
			token = line.substr(0, pos);
			lineSplit.push_back(token);
			//cout << token << endl;
			line.erase(0, pos + delimiter.length());
		} while ((pos) != string::npos);
		if (lineSplit[0] == "input" || lineSplit[0] == "output" || lineSplit[0] == "wire" || lineSplit[0] == "register") {
			if (lineSplit[0] == "input") {
				for (int i = 2; i < lineSplit.size(); i++) {
					vector<string>temp;
					char first = lineSplit[1][0];
					string sign = "s";
					if (first == 'U') {
						sign = "u";
					}
					// temp.push_back(lineSplit[i]);
					temp.push_back(sign);
					string bitWidth = getNumber(lineSplit[1].substr(lineSplit[1].size() - 2));
					temp.push_back(bitWidth);
					// inputs.push_back(temp);
					string key = lineSplit[i];
					if (lineSplit[i][lineSplit[i].size() - 1] == ',') {

						key = lineSplit[i].substr(0, lineSplit[i].size() - 1);
					}
					inputs[key] = temp;
					// temp = <"a", "32", "s">
				}
			}
			else if (lineSplit[0] == "output") {
				for (int i = 2; i < lineSplit.size(); i++) {
					vector<string>temp;
					char first = lineSplit[1][0];
					string sign = "s";
					if (first == 'U') {
						sign = "u";
					}
					// temp.push_back(lineSplit[i]);
					temp.push_back(sign);
					string bitWidth = getNumber(lineSplit[1].substr(lineSplit[1].size() - 2));
					temp.push_back(bitWidth);
					// outputs.push_back(temp);
					string key = lineSplit[i];
					if (lineSplit[i][lineSplit[i].size() - 1] == ',') {
						key = lineSplit[i].substr(0, lineSplit[i].size() - 1);
					}
					outputs[key] = temp;
				}
			}
			else if (lineSplit[0] == "wire") {
				for (int i = 2; i < lineSplit.size(); i++) {
					vector<string>temp;
					char first = lineSplit[1][0];
					string sign = "s";
					if (first == 'U') {
						sign = "u";
					}
					temp.push_back(lineSplit[i]);
					temp.push_back(sign);
					string bitWidth = getNumber(lineSplit[1].substr(lineSplit[1].size() - 2));
					temp.push_back(bitWidth);
					// wires.push_back(temp);
					string key = lineSplit[i];
					if (lineSplit[i][lineSplit[i].size() - 1] == ',') {
						key = lineSplit[i].substr(0, lineSplit[i].size() - 1);
					}
					wires[key] = temp;
				}
			}
			else if (lineSplit[0] == "register") {
				for (int i = 2; i < lineSplit.size(); i++) {
					vector<string>temp;
					char first = lineSplit[1][0];
					string sign = "s";
					if (first == 'U') {
						sign = "u";
					}
					temp.push_back(lineSplit[i]);
					temp.push_back(sign);
					string bitWidth = getNumber(lineSplit[1].substr(lineSplit[1].size() - 2));
					temp.push_back(bitWidth);
					// registers.push_back(temp);
					string key = lineSplit[i];
					if (lineSplit[i][lineSplit[i].size() - 1] == ',') {
						key = lineSplit[i].substr(0, lineSplit[i].size() - 1);
					}
					registers[key] = temp;
				}
			}
			tempString += "\t" + convertDeclaration(lineSplit) + "\n";
			results.push_back(tempString);

		}
		else if(lineSplit[0] != ""){ //Some lines are empty in the netlist
			tempString += "\t" + convertExpresion(lineSplit) + "\n";
			results.push_back(tempString);
		}
		cout << tempString;
		tempString = "";
	}
	writeVerilogFile(output_filename, results);
	return 0;
}

/**
Creates the Verilog file given the results from the convertExpression
convertDeclaration functions.
*/
void writeVerilogFile(string verilogFile, vector<string> results) {
	ofstream file;
	file.open(verilogFile + ".v");
	file << "`timescale 1ns / 1ps" << endl;
	file << "module " << verilogFile << "(" << getVariableNames() << ");" << endl;
	for (int i = 0; i < results.size(); i++) {
		file << results[i];
	}
	file << "endmodule";
	return;
}

/**
Gets all variables names required to create the module intance in the Verilog file
from the inputs and outputs.  
*/
string getVariableNames() {
	string variables = "";
	for(const auto &myPair : inputs) {
		variables += myPair.first;
		variables += ", ";
	}
	for (const auto &myPair : outputs) {
		variables += myPair.first;
		variables += ", ";
	}
	variables = variables.substr(0, variables.size() - 2);
	return variables;
}


// 1. Instantiate global vaiables, map[input], map[output], FinalString, counter
// 2. read line
// 3. split and store in a vector<string>
//      a. check first index
//          - if input/output/wire/reg
//              - create 2 maps-> map[output] = vector<vector<string>> (<"a","32","u">) //<variable name, bit width, signed or unsgned>
//                  map[input] = vector<vector> (<a,32>,<b,32>,<c,32>)
//                  map[output] = vector<vector<string>> (<"a","32","u">,<"b","32","s">,<"c","32","s">)
// 4. convert to verilog
//      a. convert input vector to string
//      a. check first index
//       - if input/output/wire/reg
//          - use map to convert to verilog convertDeclarations()
//       - if not input/output/wire/reg
//              First Time:
//                 module
//                  - read map
//                  - Module+=map
//              call convertExpression()
//                  -check for register assignments;
//      b. concatenate to global string 
// input Int32 a, b, c
//vector<vector<string>>
//map["input"]: <:

// PseudoCode:
// ModuleString
// UpdateString
// TempString
// FinalString =ModuleString
// FinalString += UpdateString
// while (getlines){
//     split to create vector
//     if vactor[0] == input:
//         Add to vector[input/output]
//         TempString += convertDeclaration(vector<) //input Int32 a, b, c    ---->  input [31:0] a,b,c;
//     else:
//         if i==0: //check to see if this is the first time entering this else statement
//             ModuleString = createModuleString()
//             FinalString += ModuleString + TempString
//             i=1
//         UpdateString = convertExpression()
//     FinalString += UpdateString
// }

int main(int argc, const char * argv[]) {
	ifstream myfile;
	myfile.open(argv[1]);
	if (myfile) {
		cout << "file " << argv[1] << " exists" << "\n";
		if(argc == 3)
			readFile(argv[1], argv[2]);
		else
			readFile(argv[1]);
	}
	else {
		cout << "file doesn't exist" << "\n";
		return 0;
	}
	cout << "Press a key to continue" << endl;
	cin.get();
	return 0;
}