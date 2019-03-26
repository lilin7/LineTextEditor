/*
* @ClassName: Command.cpp
* @Function: Class to process user input (command line)
* @author: Lin Li 40044486
* @version: 1.0
* @date: Jun 25, 2018
*/
#include<iostream>
#include<string>
#include<sstream>
#include <iterator>
#include <vector>
#include <list>
#include <ctype.h>
#include <algorithm>
using namespace std;
#include "Command.h"

//utilitary functions
namespace {
	/*
	* the method is to tell if the input string is consist of one or more spaces
	* @param the string which need to be tell if  is consist of one or more spaces
	* @return return a bool (true if yes, return false if no)
	*/
	bool allAreSpaces(string str) {
		for (size_t i = 0; i < str.size(); ++i) {
			if (str[i] != ' ') {
				return false;
			}
		}
		return true;
	}

	/*
	* the method is to tell if the input string in the parameter contains only numeric symbol(s)
	* @param the string which need to be tell if contains only numeric symbol(s)
	* @return return a bool (true if yes, return false if no)
	*/
	bool isNumber(string str) {
		// Checks if first char in buffer contains invalid digits 
		if (!(str[0] >= '0' && str[0] <= '9') )
			return false;
		// Checks if each digit after the first is anything but 0-9
		for (int i = 1; str[i] != '\0'; i++) {
			if (str[i] < '0' || str[i] > '9')
				return false;
		}
		// If neither statement returns false, returns true by default
		return true;
	}

	/*
	* the method is to trim all the spaces in a given string
	* @param the string which need to be trimmed out all the spaces
	* @return a string after the original string is trimmed all the spaces
	*/
	string removeSpaces(string str)
	{
		str.erase(remove(str.begin(), str.end(), ' '), str.end());
		return str;
	}
}

/*
* Command constructor who takes 3 parameters, and the default value for member variables are: 
* cmdType is INVALID, cmdStartLine is -1, cmdEndLine is -1
* process user input for different situations, generate different enum Type for different kind of commands
* and pass to Led.cpp in an instance of Command class for further process
* @param user input for command line
* @param takes an int as initial current line for each instance, and work from there for modifying it based on user command
* @param takes an int as endLine for each instance, and work from there for modifying it based on user command
*/
Command::Command(const string& userInput, const int currentLine, const int endLine): cmdType(INVALID),cmdStartLine(-1),cmdEndLine(-1){
	if (userInput.empty()) {	//to tell if unser input is empty. If yes, return
		return;
	}

	if (allAreSpaces(userInput)) { //if user input is only space (one or many), regard as 1+
		cmdType = MOVEDOWN;
		cmdStartLine = 1;
		return;
	}

	string trimmedInput = removeSpaces(userInput); //remove all spaces from user input

	if (isNumber(trimmedInput)) { //if user input contains only number "x", means "x,xg". "isNumber(String s)" is a method in this class.
		cmdType = GOTO;
		cmdStartLine = cmdEndLine = stoi(trimmedInput);
		return;
	}

	if (trimmedInput.size() == 1) { //if user only input one symbol (+ - g w q * , d x r c i a p)
		char c = tolower(*(trimmedInput.end() - 1));
		switch (c) {
		case 'g': { //equals to go to current line
			cmdType = GOTO;
			cmdStartLine = cmdEndLine = currentLine;
			break;
		}
		case 'p': { //equals to print current line
			cmdType = PRINT;
			cmdStartLine = cmdEndLine = currentLine;
			break;
		}
		case ',': { //',' equals to '.,.p', means print current line
			cmdType = PRINT;
			cmdStartLine = cmdEndLine = currentLine;
			break;
		}
		case '.': { //',' equals to '.,.p', means print current line
			cmdType = PRINT;
			cmdStartLine = cmdEndLine = currentLine;
			break;
		}
		case '*': { //'*' equals to '1,$p', means print whole buffer
			cmdType = PRINT;
			cmdStartLine = 1;
			cmdEndLine = endLine;
			break;
		}
		case '+': { //'+' equals to '1,1+', means moves the current line down by 1 line
			cmdType = MOVEDOWN;
			cmdStartLine = cmdEndLine = 1;
			break;
		}
		case '-': { //'-' equals to '1,1-', means moves the current line up by 1 line
			cmdType = MOVEUP;
			cmdStartLine = cmdEndLine = 1;
			break;
		}
		case 'd': { //delete current line
			cmdType = DELETE;
			cmdStartLine = cmdEndLine = currentLine;
			break;
		}
		case 'x': { //cut current line into clipboard
			cmdType = CUT;
			cmdStartLine = cmdEndLine = currentLine;
			break;
		}
		case 'r': { //replace current line
			cmdType = REPLACE;
			cmdStartLine = cmdEndLine = currentLine;
			break;
		}
		case 'c': { //change the current line
			cmdType = CHANGE;
			cmdStartLine = cmdEndLine = currentLine;
			break;
		}
		case 'i': { //insert before the current line
			cmdType = APPENDINSERT;
			cmdStartLine = cmdEndLine = currentLine;
			break;
		}
		case 'a': { //append after the current line
			cmdType = APPENDINSERT;
			int i = currentLine + 1;
			cmdStartLine = cmdEndLine = i;
			break;
		}
		case 'w': { //write out buffer to file
			cmdType = WRITEOUT;
			break;
		}
		case 'q': { //quit 
			cmdType = QUIT;
			break;
		}

		default: {  //if there is only one symbol but not the letters above, judge input as invalid
			cmdType = INVALID;
			return;
		}
		}
	}

	else { //if user input is not space(one or many), numeric, or single symbol
		char lastChar = tolower(*(trimmedInput.end() - 1)); //extract the last char of user input and change to lower case

		if (isdigit(lastChar)) { //if the last char in command line is a number, only applies to ",y" and "x,y" means Print
			const string lineInfo_1 = trimmedInput.substr(0, trimmedInput.size()); //get line(s) information
			if (lineInfo_1.find(',') == std::string::npos) {
				cmdType = INVALID;//there must be a , in the lineInfo, otherwise invalid input
				return;
			}
			else { //for ",y" and "x,y", means Print
				const int pos_1 = lineInfo_1.find(','); //find the position of comma
				cmdEndLine = stoi(lineInfo_1.substr(pos_1 + 1));  //let end line equals to the number after comma
				
				if (lineInfo_1.find(',') != 0) { //if there is something before comma, for "x,y"
					cmdStartLine = stoi(lineInfo_1.substr(0, lineInfo_1.find(','))); //let start line number equals to the number before comma
				}
				else { //if there is nothing before comma, for ",y"
					cmdStartLine = currentLine;					
				}
				cmdType = PRINT; //for here only  ",y" and "x,y" apply, which means Print
				return;
			}
		}

		if (lastChar == ',') { //situation for "x," which equals to		x,.p
			const string lineInfo_2 = trimmedInput.substr(0, trimmedInput.size() - 1); //get line(s) information
			if (isNumber(lineInfo_2)) { //if there is a number before comma
				cmdType = PRINT; //type is print
				cmdStartLine = stoi(lineInfo_2); //let start line equals to the number before comma
				cmdEndLine = currentLine; //end line is omitted, so it equals to current line
				return;
			}
			else
				cmdType = INVALID;
			return;
		}

		//form here, last char can only be a, i, d, x, r, j, p, c, -, +, g, u, v
		string lastCharStr;
		lastCharStr.push_back(lastChar); 
		//judge if last char is any one of these symbols, if not, the input command is wrong and invalid
		if (lastCharStr.find_first_of("aidxrjpc-+guv") == std::string::npos) { 
			cmdType = INVALID;
			return;
		}

		//from here, last char could only be a, i, d, x, r, j, p, c, -, +, g, with something before it
		const string lineInfo = trimmedInput.substr(0, trimmedInput.size() - 1); //get line(s) information

		if (isNumber(lineInfo)) { //if there is pure number before last command char
			cmdStartLine = cmdEndLine = stoi(lineInfo);  //let start line and begin line equals to this number
		}
		else if (lineInfo == ".") { //if it is only a . before last command char
			cmdStartLine = cmdEndLine = currentLine; //let start line and begin line equals to current line
		}
		else if (lineInfo == "$") { //if it is only a $ before last command char
			cmdStartLine = cmdEndLine = endLine; //let start line and begin line equals to end line
		}
		else {
			//if the line info is not pure number, need to seperate the 2 numbers by comma
			string xStr = lineInfo.substr(0, lineInfo.find(','));
			string yStr;
			const int pos = lineInfo.find(',');
			if (pos != std::string::npos) {
				yStr = lineInfo.substr(pos + 1); //seperate the lineInfo by ","
			}

			//if the x and y are only numbers, or ., or $, set start line and end line accordingly:
			if (xStr == "." || xStr == "$" || yStr == "." || yStr == "$" || isNumber(xStr) || isNumber(yStr)) {
				if (xStr == ".") {
					cmdStartLine = currentLine;
				}
				if (xStr == "$") {
					cmdStartLine = endLine;
				}
				if (yStr == ".") {
					cmdEndLine = currentLine;
				}
				if (yStr == "$") {
					cmdEndLine = endLine;
				}

				if (isNumber(xStr)) {
					cmdStartLine = stoi(xStr);
				}

				if (isNumber(yStr)) {
					cmdEndLine = stoi(yStr);
				}
			}
			else {
				cmdType = INVALID;
			}
		}

		//tell if x and y exceed the limit (1<=x<=y<=last line)
		if (cmdStartLine<1 || cmdStartLine>cmdEndLine || cmdEndLine > endLine) {
			//cmdStartLine = std::max(1, cmdStartLine);
			cmdType = INVALIDRANGE;
			return;
		}

		//at this stage we have found the value of x and y
		switch (lastChar) // case a, i, d, x, r, j, p, c, -, +, g for command char
		{
			case 'g': { //g means GOTO
				cmdType = GOTO;
				break;
			}
			case 'p': { //p means PRINT
				cmdType = PRINT;
				break;
			}
			case '+': { //moves the current line down by x line
				cmdType = MOVEDOWN;
				break;
			}
			case '-': { //moves the current line up by x line
				cmdType = MOVEUP;
				break;
			}
			case 'd': { //delete line x-y
				cmdType = DELETE;
				break;
			}
			case 'x': { //cut line x-y to clipboard
				cmdType = CUT;
				break;
			}
			case 'i': { //insert text before line x
				cmdType = APPENDINSERT;
				break;
			}
			case 'a': { //append text after line x, as it should be after line x, increment start line by 1 here
				cmdType = APPENDINSERT;
				cmdStartLine++;
				break;
			}
			case 'u': { //paste text from clipboard above line x
				cmdType = PASTE;
				break;
			}
			case 'v': { //paste text from clipboard below line x
				cmdType = PASTE;
				cmdStartLine++;
				break;
			}
			case 'r': { //paste text from clipboard below line x
				cmdType = REPLACE;
				break;
			case 'j': { //join x-y to x
				cmdType = JOIN;
				break;
			}
			case 'c': { //change the current line
				cmdType = CHANGE;
				break;
			}

			default: { //if last char doesn't match any above, cmdType is INVALID and return
				cmdType = INVALID;
				break;
			}
		}
		}
	}
}
