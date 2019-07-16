#pragma once

#include<iostream>
#include<fstream>
#include<string>
#include<ctime>
#include"InitFileReadWrite.h"

namespace Otherwise
{

	/*Returns the date and time in the format: day-month-year hour-minute-second for the purposes of dating error output.*/
	std::string dateTime();

	/*Uses the dateTime function to print the error to a file that's named the date and time so that you have errors in chronological order.*/
	void throwFileError(std::string errorWord, std::string errorString);

	/*Prints the error to the console and asks if the error is minor enough to be overlooked, debug mode only*/
	void throwConsoleError(std::string errorString);

	/*The function called everywhere else in any program that uses this engine, takes an errorString 
	to define the error and an errorWord that indicates the type of error,  checks initialization file to see if we're in debug mode*/
	void throwError(std::string errorWord, std::string errorString);
}

