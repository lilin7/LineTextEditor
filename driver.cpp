/*
* @ClassName: Command.h
* @Function: Driver program to test the Led class implemented in assignment 2
* @author: Lin Li
* @version: 1.0
* @date: Jun 25, 2018
*/

#include<iostream>
#include<string>
#include<algorithm>
#include"Command.h"
#include"Led.h"
#include<cassert>
using namespace std;

int main(int argc, char * argv[])
{ 
	string filename; // an empty filename
	switch (argc) { // determine the filename
		case 1: // no file name
			break;
		case 2: filename = argv[1]; // initialize filename from argument
			break;
		default: cout << ("too many arguments - all discarded ") << endl;
			break;
	}
	Led editor(filename); // create a Led named editor
	editor.run(); // run our editor
	return 0; // report success
}
