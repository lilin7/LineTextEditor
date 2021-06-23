/*
* @ClassName: Led.h
* @Function: It creates a list (string type) named buffer from txt file if exists, if not, create one if user choose to save buffer at quitting.
*			It takes the information passed by an instance of Command class, and operate in the buffer.
*			Continue for asking user input until user choose to quit.
*			Promote user to save back to file when quitting.
* @author: Lin Li
* @version: 1.0
* @date: Jun 25, 2018
*/
#pragma once
#include<vector>
#include <list>
#include "Command.h"
using namespace std;

class Led {
public:
	/*
	 * public constructor that take an optional file name as a parameter and a member function 
	 * that initiates an editing session
	 * @param input : optional filename where the user will write to on exit
	 * @return void
	 */
	Led(const string& input = "");

	/*
	* public method to execute the Led editor session
	* @param void
	* @return void
	*/
	void run();
	
private:
	/*
	* private method to execute an instance c of Command.cpp class
	* @param the address of an instance c of Command.cpp class
	* @return void
	*/
	void execute(Command& c);

	/*
	* write the content in the buffer back to file
	* @param void
	* @return void
	*/
	void writeToFile();

	//member variable to take the file name from which the buffer creates
	string fileName;	

	//a string type list which is the buffer of the program, process user command in it
	list <string> buffer;	

	/*
	 * a string type vector which is the clipboard, user command x will cut content from buffer
	 * and paste into clipboard, and user command u and v will paste the content in the clipboard to buffer
	 */
	vector <string > clipboard; 
	
	//a central concept currentLine, different user commands set it to different lines after execute
	int currentLine; 

	//a bool to tell if user want to keep input command or want to quit
	bool finished; 
};
