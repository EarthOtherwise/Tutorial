#pragma once
#include<string>
#include<vector>
#include "ErrHandler.h"
#include <fstream>

namespace Otherwise
{

	/*Read the initialization file and make it availible for use.*/
	std::vector<std::string> readInitFile();

	/*Make a single line from the init file availible for reading.*/
	std::string readInitFileLine(unsigned int lineNumber);

	/*Make or Replace the initialization file.*/
	void writeInitFile(std::vector<std::string> initContents);

	/*Replace a single line in the init file.*/
	void writeInitFileLine(unsigned int lineNumber, std::string replacementString);
}

