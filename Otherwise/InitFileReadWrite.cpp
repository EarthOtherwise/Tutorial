#include "InitFileReadWrite.h"

namespace Otherwise
{

	std::vector<std::string> Otherwise::readInitFile()
	{
		std::vector<std::string> returnVector;
		std::ifstream initFile("Init.txt");
		if (initFile.is_open())
		{
			int initFileLine = 0;
			while (getline(initFile, returnVector[initFileLine]))
			{
				initFileLine++;
			}
		}
		else
		{
			Otherwise::throwError("Initialization", "The initialization file is missing or corrupt.");
		}
		return returnVector;
	}

	std::string Otherwise::readInitFileLine(unsigned int lineNumber)
	{
		std::string returnString;
		std::ifstream initFile("Init.txt");
		if (initFile.is_open())
		{
			for (unsigned int i = 0; i < lineNumber; i++)
			{
				getline(initFile, returnString);
			}
		}
		else
		{
			Otherwise::throwError("Initialization", "The initialization file is missing or corrupt.");
		}

		return returnString;
	}

	void Otherwise::writeInitFile(std::vector<std::string> initContents)
	{
		std::ofstream initFile("Init.txt");
		if(initFile.is_open())
		{
			for (unsigned int i = 0; i < initContents.size(); i++)
			{
				initFile << initContents[i] << '\n';
			}
			initFile.close();
		}
		else
		{
			Otherwise::throwError("Initialization", "The initialization file could not be created.");
		}
	}

	void Otherwise::writeInitFileLine(unsigned int lineNumber, std::string replacementString)
	{
		std::string tempString;
		std::vector<std::string> tempFile;
		std::ifstream initFile("Init.txt");
		unsigned int iterator = 1;
		if (initFile.is_open())
		{
			while (getline(initFile, tempString))
			{
				if (iterator == lineNumber)
				{
					tempFile.push_back(replacementString);
				}
				else
				{
					tempFile.push_back(tempString);
				}
				iterator++;
			}
		}
		else
		{
			Otherwise::throwError("Initialization", "The initialization file is missing or corrupt.");
		}

		writeInitFile(tempFile);
	}

}