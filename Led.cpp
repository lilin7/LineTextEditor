/*
* @ClassName: Led.cpp
* @Function: It creates a list (string type) named buffer from txt file if exists, if not, create one if user choose to save buffer at quitting.
*			It takes the information passed by an instance of Command class, and operate in the buffer.
*			Continue for asking user input until user choose to quit.
*			Promote user to save back to file when quitting.
* @author: Lin Li
* @version: 1.0
* @date: Jun 25, 2018
*/
#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include <iterator>
#include <algorithm>
#include <iomanip>
#include <string>
using namespace std;
#include "Command.h"
#include "Led.h"

/*
* public constructor that take an optional file name as a parameter and a member function
* that initiates an editing session
* @param input : optional filename where the user will write to on exit
* @return void
*/
Led::Led(const string& inputFilename) : fileName(inputFilename), currentLine(1), finished(false) {
	string line;
	int count=0; //use counter to count how many lines are entered, for priting purpose
	std::ifstream myFile(inputFilename); //use ifstream to input file into buffer
	if (myFile.is_open())  //if a file is open and associated with this stream object.
	{		
		// read every lines and create initial buffer
		while (getline(myFile, line)){ //when there are lines left in my file
			buffer.push_back(line);		//append the lines into buffer list
			count++;
		}
		myFile.close();
		cout << "\"" << inputFilename << "\"" << " ";
		cout << " " << count << " lines" << endl;
		cout << "Entering command mode." << endl;
		currentLine = count;
	}

	// user input a nonexist file, creats an empty buffer.
	// Promote user to save to file only at quitting time
	else if (!myFile.is_open() && inputFilename != "") { 
		cout << "\"" << inputFilename << "\"" << " ";
		cout << "[New File]" << endl;
		cout << "Entering command mode." << endl;
	}
	else //user start without a filename, creates an empty buffer
	{
		cout << "\"" << "?" << "\"" << " " << "[New File]" << endl;
		cout << "Entering command mode." << endl;
	}
}

/*
* public method to execute the Led editor session
* @param void
* @return void
*/
void Led::run() {
	// runs commands until the user terminates the session
	while (!finished) { //use a loop to ask for user input until user command quit
		cout << "? ";
		string userInput;
		getline(cin, userInput);  //get unser input
		Command c(userInput, currentLine, buffer.size());  //use unser input to construct an instance of Command class
		execute(c); //execute the instance of command class to process user command
	}
}

/*
* private method to execute an instance c of Command.cpp class
* @param the address of an instance c of Command.cpp class
* @return void
*/
void Led::execute(Command& c) {
	if (buffer.empty()) { //if buffer is empty, can only do append, insert, quit type actions
		if (c.getCmdType() != Command::Type::APPENDINSERT && c.getCmdType() != Command::Type::QUIT) {
			cout << "empty buffer" << endl;
			return;
		}			
	}

	if (c.getCmdType() == Command::Type::GOTO) { //if the command contains g or pure numeric
		currentLine = c.getCmdStartLine();
		cout << "Goes to Line " << currentLine << " and current line set to it." << endl; //need to print current line
	}

	else if (c.getCmdType() == Command::Type::PRINT) { //if the command contains p
		cout << "Print the content: " << endl;
		// iterate over the buffer
		int i = 1;
		for (list<string>::iterator it = buffer.begin(); //use iterator to print lines in the command
			it != buffer.end();
			++it, ++i)
		{
			if (i > c.getCmdEndLine()) break;
			if (i >= c.getCmdStartLine()) {
				cout << i << ((i == currentLine) ? "> " : ": ") << *it << endl; //print cur iterator			
			}
		}
	}
	else if (c.getCmdType() == Command::Type::MOVEDOWN) { //if the command contains +
		currentLine = currentLine + c.getCmdStartLine(); //move currentline
	}
	else if (c.getCmdType() == Command::Type::MOVEUP) { //if the command contains -
		currentLine = currentLine - c.getCmdStartLine();//move currentline
	}

	else if (c.getCmdType() == Command::Type::DELETE) { //if the command contains d
												   //delete from c.getCmdStartLine() to c.getCmdEndLine() from list
		int i = 1;
		//use iterator of list to delete certain elements from container
		for (list<string>::iterator it = buffer.begin(); it != buffer.end(); ++it, ++i)
		{
			if (i == c.getCmdStartLine()) {
				for (; i <= c.getCmdEndLine(); ++i) {
					it = buffer.erase(it);
				}
				//if there is still line after the deleted line(s), set it to be current line
				//otherwise set the last line of the buffer to be current line
				currentLine = std::min(static_cast<int>(buffer.size()), i);
				break;
			}
		}
	}

	else if (c.getCmdType() == Command::Type::CUT) { //if the command contains x
		clipboard.clear(); //clear whatever is in the clipboard from past actions
		clipboard.reserve(c.getCmdEndLine() - c.getCmdStartLine() + 1); //reserver certain space for clipboard
		int i = 1;
		//use iterator of list to find where to begin cutting from container
		for (list<string>::iterator it = buffer.begin(); it != buffer.end(); ++it, ++i)
		{
			if (i == c.getCmdStartLine()) { //find the start line
				for (; i <= c.getCmdEndLine(); ++i) { //before reaching end line
					clipboard.push_back(*it); //move element from list buffer to vector clipboard
					it = buffer.erase(it); //delete element in buffer
				}
				//if there is still line after the cut line(s), set it to be current line
				//otherwise set the last line of the buffer to be current line
				currentLine = std::min(static_cast<int>(buffer.size()), i); 
				break;
			}
		}
	}

	else if (c.getCmdType() == Command::Type::APPENDINSERT) { //if the command contains i or a
		int i = 1;
		//creat iterator from begin of buffer list
		list<string>::iterator it = buffer.begin();

		for (; it != buffer.end(); ++it, ++i) {
			if (i == c.getCmdStartLine()) { //find where to start
				break;
			}
		}
		while (true) {
			string newInput;
			getline(cin, newInput);
			if ((newInput == "" && cin.eof()) || newInput == "^Z" || newInput == "^z") { //once user input CTRL+Z, stop input mode
				cin.clear();
				break;
			}
			buffer.insert(it, newInput); //insert or append user input to buffer
			currentLine = i; //set current line
			i++;
		}
	}

	else if (c.getCmdType() == Command::Type::PASTE) { //if the command contains u or v
		int i = 1;
		//use iterator of list to paste certain elements to container
		for (list<string>::iterator it = buffer.begin(); it != buffer.end(); ++it, ++i) {
			if (i == c.getCmdStartLine()) { //find the start line
				//use iterator to paste what is in the clipboard to buffer
				for (vector<string>::iterator itClipboard = clipboard.begin(); itClipboard != clipboard.end(); ++itClipboard) {
					buffer.insert(it, *itClipboard);
					currentLine = i; //set current line
					i++;
				}
				break;
			}
		}
	}

	else if (c.getCmdType() == Command::Type::REPLACE) { //if the command contains r		
		int i = 1;
		//delete from start line to end line from buffer, same as DELETE
		for (list<string>::iterator it = buffer.begin(); it != buffer.end(); ++it, ++i)
		{
			if (i == c.getCmdStartLine()) {
				for (; i <= c.getCmdEndLine(); ++i) {
					it = buffer.erase(it);
				}
				currentLine = std::min(static_cast<int>(buffer.size()), i);
				break;
			}
		}
		//insert to buffer, same as APPENDINSERT
		int j = 1;
		list<string>::iterator it = buffer.begin();
		for (; it != buffer.end(); ++it, ++j) {
			if (j == c.getCmdStartLine()) {
				break;
			}
		}
		while (true) {
			string newInput;
			getline(cin, newInput);
			if ((newInput == "" && cin.eof()) || newInput == "^Z" || newInput == "^z") {
				cin.clear();
				break;
			}
			buffer.insert(it, newInput);
			currentLine = j;
			j++;
		}
	}

	else if (c.getCmdType() == Command::Type::JOIN) { //if the command contains j
		currentLine = c.getCmdStartLine(); //set current line
		int i = 1;
		//use iterator of list to join certain elements together
		for (list<string>::iterator it = buffer.begin(); it != buffer.end(); ++it, ++i){
			if (i == c.getCmdStartLine()) { //find the first line to which we join everything
				list<string>::iterator joinedIt = it;
				++i;
				++it;
				for (; it != buffer.end(); ++i) { 
					if (i > c.getCmdEndLine()) { //when reaches end line, break
						break;
					}
					*joinedIt += *it;
					it = buffer.erase(it);		//delete from the second line because everything is append to first line already		
				}
				break;
			}
		}
	}

	else if (c.getCmdType() == Command::Type::CHANGE) { //if the command contains c		
		string originalStr;
		string targetStr;
		string elementListStr;
		cout << setfill(' ') << setw(12) << "Change what?" << endl; //promot user to input original string
		getline(cin, originalStr);
		cout << setfill(' ') << setw(12) << "To what?" << endl;//promot user to input target string
		getline(cin, targetStr);

		int i = 1;
		int count = 0;
		int position = 0;
		//use iterator to go through selected line to replace
		for (list<string>::iterator it = buffer.begin(); it != buffer.end(); ++it, ++i) {
			if (i == c.getCmdStartLine()) {
				string& elementListStr = *it;
				//when find the position of original string, replace it with target string
				//and while loop makes all original strings are replaced
				while ((position = elementListStr.find(originalStr, position)) != std::string::npos) {
					elementListStr.replace(position, originalStr.length(), targetStr);
					position += targetStr.length();
					++count;
				}		
			}			
		}
		if (count != 0) { //if replace happened, print information
			cout << "Changed " << count << " occurence(s)." << endl;
		}
		else { //if replace did not happen, print information
			cout << "Search not found, didn't change anything." << endl;
		}
	}
	
	else if (c.getCmdType() == Command::Type::WRITEOUT) { //if the command contains w
		writeToFile();	 //write out to buffer
	}

	else if (c.getCmdType() == Command::Type::QUIT) { //if the command contains q	
		bool flag = true;
		string choice;
		while (flag) {
			//promote user to save
			cout << "Save changes to file " << fileName << " (y/n)?" << endl;
			getline(cin, choice);
			//get user input and transfer to lower case
			transform(choice.begin(), choice.end(), choice.begin(), ::tolower);
			
			if (choice == "y" ) {
				writeToFile(); //user input y to write to file
				flag = false;
			}
			else if (choice == "n") {
				cout << "Changes not saved to File." << endl; //user input n to not to write to file
				flag = false;
			}
			else {
				cout << "Bad answer : " << choice << endl; //user input other things, print info and loop again for input
				cout << "Enter y for yes and n for no." << endl;
			}	
		}
		cout << "Bye" << endl; //after user choose to save or don't save, print info
		finished = true; // and finish the program loop, so the program is terminated in driver class
	}	

	if (c.getCmdType() == Command::Type::INVALID) { //if the command is invalid
		cout << "Your input is not correct, please try again!" << endl; //print wrong input
	}
	if (c.getCmdType() == Command::Type::INVALIDRANGE) { //if input line number exceeds
		cout << "Invalid range." << endl; 
	}
}

/*
* write the content in the buffer back to file
* @param void
* @return void
*/
void Led::writeToFile() {
	ofstream myFile(fileName);

	if (myFile.is_open()) { //if my file is open and associated with this stream object.
		//use iterator of buffer to write buffer to my file
		for (list<string>::iterator it = buffer.begin(); it != buffer.end(); ++it) {
			myFile << *it << endl;
		}
		myFile.close();
	}

	else { //if there is no file yet
		string newFileName;
		cout << "Enter a file Name: " << endl; //promote user to enter a file name
		getline(cin, newFileName); //get user input
	
		ofstream myNewfile(newFileName); //out put file name and creat my new file

		if (myNewfile.is_open()) { //if my new file is open and associated with this stream object.
			//use iterator of buffer to write buffer to my file
			for (list<string>::iterator it = buffer.begin(); it != buffer.end(); it++) {
				myNewfile << *it << endl;
			}
			myNewfile.close();
		}
		fileName = newFileName; //give value of my new file name to file name
	}
	cout << buffer.size() << " lines written to file: " << fileName << endl; //print info
}




