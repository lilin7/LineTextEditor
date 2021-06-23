/*
* @ClassName: Command.h
* @Function: header file of Command.cpp to process user input (command line)
* @author: Lin Li 
* @version: 1.0
* @date: Jun 25, 2018
*/

#pragma once
using namespace std;

class Command {
public:
	/*
	* public enum Type to categorize the command type in command line
	* @param void
	*/
	enum Type {
		GOTO,		//for g (g, *g, $g	x		x,yg	)
		MOVEDOWN,	//for + (+, x+)
		MOVEUP,		//for - (-, x-)
		PRINT,		//for p (p	 x,yp	 x,		,y		x,y		,	.	*)

		DELETE,		//for d
		CUT,		//for x
		APPENDINSERT, //for a, i
		PASTE,		//for u, v
		REPLACE,	//for r
		JOIN,		//for j
		CHANGE,		//for c

		WRITEOUT, //for w
		QUIT, //for q
		INVALIDRANGE, //for input line number exceeds required
		INVALID = -1  //for invalid input of command
	};

	/*
	* Command constructor who takes 3 parameters:
	* @param user input for command line
	* @param takes an int as initial current line for each instance, and work from there for modifying it based on user command
	* @param takes an int as endLine for each instance, and work from there for modifying it based on user command
	*/
	Command(const string& userInput, const int currentLine, const int endLine);

	Type getCmdType() {
		return cmdType;
	}
	int getCmdStartLine() {
		return cmdStartLine;
	}
	int getCmdEndLine() {
		return cmdEndLine;
	}

private:
	//enum Type which categorize the command type in command line and transfers to Led class by an instance of Command class
	Type cmdType; 
	int cmdStartLine; // parsed start line of user command, indicate to process from which line in the buffer
	int cmdEndLine; // parsed start line of user command, indicate to process until which line in the buffer
};
