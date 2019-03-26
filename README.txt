/*
* Assignment 2
* Student Name: Lin Li
* Student ID: 40044486
*/

This program uses 4 classes (Command.h, Command.cpp, Led.h, Led.cpp) and a driver.cpp file to promote user input to interact with a txt file if exists, create one if doesn't exist.

Command class:
It takes and processes user input, convert it to be the member variables of this class, and passes to Led class by an instance of this class.
The information passes to Led class includes command type, command start line, command end line.

Led class:
It creates a list (string type) named buffer from txt file if exists, if not, create one if user choose to save buffer at quitting.
It takes the information passed by an instance of Command class, and operate in the buffer.
Continue for asking user input until user choose to quit.
Promote user to save back to file when quitting.

Driver class:
Driver program to test the Led class implemented above.