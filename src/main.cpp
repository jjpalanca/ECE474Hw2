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

string getBitWidth(string variable);
string getNumber(string variable);
string getVariableNames();
string appendToInput(string input, string output);
void writeVerilogFile(string verilogFile, vector<string> results);
bool isItSigned(string variable);


int ModuleIndex = 0; //Global Variable for use in the function convertExpression


bool isValidOperator(string op) {
	bool isValid = false;
	vector<string> ops{"+", "-", "*", ">>", "<<", "?"};
	for (string x: ops){
		if (op.compare(x)==0){
			isValid = true;
			break;
		}
	}
	return isValid;
}

//bool validVariables(vector<string> expression){
//	return true;
//}
// Converts math expressions in the format of 
//  d = a + b
// to a string of the fo
//  ADD #(.WIDTH(<BitWidth>))line<ModuleIndex>(a,b,d);
// Supports the operations: REG, ADD, SUB, MUL, COMP, MUX2x1, SHR, SHL
string convertExpresion(vector<string> expression) {
	string CLK = "CLK";
	string RST = "RST";
	string result = "";
	ModuleIndex = ModuleIndex + 1;
	string bitWidth = "";
	
	// if not register expression 
	if (expression.size() != 3/* && validateVariables() == true*/) {
		string op = expression[3];
		//validVariables(expression);
		bitWidth = (getBitWidth(expression[0]) > getBitWidth(expression[4])) ? 
			getBitWidth(expression[0]) : getBitWidth(expression[4]); //Bitwidth is determined by inputs
		string inputsToOp = appendToInput(expression[2], bitWidth) + "," + appendToInput(expression[4], bitWidth);
		//COMP; Comparison
		result += (isItSigned(expression[2]) || isItSigned(expression[4])) ? "S" : ""; //Inputs signed?
		if(op.compare(">") == 0)	
			result += "COMP #(.WIDTH(" + (bitWidth) + "))line" + to_string(ModuleIndex) + "("
			+ inputsToOp + "," + expression[0] +"," + "1'b0" +"," + "1'b0" + ");";

		else if (op.compare("<") == 0) 
			result += "COMP #(.WIDTH(" + (bitWidth) + "))line" + to_string(ModuleIndex) + "("
			+ inputsToOp +"," +"1'b0" + "," + expression[0] + "," + "1'b0" + ");";
		
		else if(op.compare("==") == 0)
			result += "COMP #(.WIDTH(" + (bitWidth) + "))line" + to_string(ModuleIndex) + "("
			+ inputsToOp + "," + "1'b0" + "," + "1'b0" + "," + expression[0] + ");";

		else if (isValidOperator(op)){
			bitWidth = getBitWidth(expression[0]); //Bitwidth is determined by output or wire
			inputsToOp = appendToInput(expression[2], bitWidth) + "," + appendToInput(expression[4], bitWidth);
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
				result = (isItSigned(expression[4]) || isItSigned(expression[6])) ? "S" : "";
				return result + "MUX2x1 #(.WIDTH(" + (bitWidth)+"))line" + to_string(ModuleIndex) + "("
				+appendToInput(expression[4], bitWidth) + "," + appendToInput(expression[6], bitWidth)+//inputs
					"," + expression[2] + "," + expression[0] + ");";
			}

			result += "#(.WIDTH(" + (bitWidth) + "))line" + to_string(ModuleIndex) + "(" 
			+ inputsToOp + "," + expression[0] + ");";
		}
		else {
			cout << "Error: Invalid operator" << endl;
			exit(0);
		}
		
		return result;
	}
	//REG
	else {
		bitWidth = getBitWidth(expression[0]);
		return "REG #(.WIDTH(" + (bitWidth) + "))line" + to_string(ModuleIndex) + "(" + CLK + ","
			+ RST + "," + appendToInput(expression[2], bitWidth) + "," + expression[0] + ");";
	}

	return "UNEXPECTED EXPRESSION or ERROR\n";
}


/*
Creates a string which will append/concaneted a input according
to the output variable which can be signed or unsigned.
Verilog sign extension:
	extended[15:0] <= {{8{a[7]}}, a[7:0]};
Padd zero:
	extended[15:0] <= {8'b0, a[7:0]};
*/
string appendToInput(string input, string bitWidth) {
	string result = "", mostSigBit = "";
	int difference = stoi(bitWidth) - stoi(getBitWidth(input));
	if (difference == 0)
		result = input;
	else if (difference > 0) { //Need to add bits
		mostSigBit = to_string(stoi(getBitWidth(input)) - 1);
		if (isItSigned(input)) { //sign extend
			if(mostSigBit != "0")
				result = "{{" + to_string(difference) + "{" + input + "[" + mostSigBit + "]}}," +
					input + "[" + mostSigBit + ":0]}";
			else //Sign extending a bit of size 1
				result = "{" + to_string(difference) + "'b0," +
				input + "}";
		}
		else { //padd zeros
			result = "{" + to_string(difference) + "'b0," +
				input + "[" + mostSigBit + ":0]}";
		}
	}
	else { //Need to remove bits
		mostSigBit = to_string(stoi(bitWidth) - 1);
		result = input + "[" + mostSigBit + ":0]";
	}
	return result;
}

/**
Given a variable name, seaches through all global variable vectors 
to find the bit width of the parameter variable.
All variables names are "SHOULD" be uniquely named, so we can search through all
vectors until we find it.
*/
string getBitWidth(string variable){
	string bitWidth = "0";
	if (outputs.count(variable) > 0) 
		bitWidth = outputs[variable][1];
	else if (wires.count(variable) > 0) 
		bitWidth = wires[variable][2];
	else if (registers.count(variable) > 0)
		bitWidth = registers[variable][2];
	else if (inputs.count(variable) > 0)
		bitWidth = inputs[variable][1];
	return bitWidth;
}


/*
Check if the output variable of an operations is signed 
or unsigned. CHECK if output of component determines if 
component is unsgined or signed.
*/
bool isItSigned(string variable) {
	bool signedV = false;
	if (outputs.count(variable) > 0)
		signedV = (outputs[variable][0] == "s");
	else if (wires.count(variable) > 0)
		signedV = (wires[variable][1] == "s");
	else if (registers.count(variable) > 0)
		signedV = (registers[variable][0] == "s");
	else if (inputs.count(variable) > 0)
		signedV = (inputs[variable][0] == "s");
	return signedV;
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
		bits = getNumber(input.at(1));
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
		//cout << "line:" << line << "\n";
		vector<string> lineSplit;

		string token;
		string delimiter = " ";
		size_t pos = 0;

		//The following two lines of code remove white space
		// characters at the end of the line
		std::size_t endWhite = line.find_last_not_of(" \t\f\v\n\r");
		if (endWhite!=std::string::npos){line.erase(endWhite+1);};

		do {
			pos = line.find(delimiter);
			token = line.substr(0, pos);
			lineSplit.push_back(token);
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