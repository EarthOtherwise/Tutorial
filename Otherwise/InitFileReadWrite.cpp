#include "InitFileReadWrite.h"

namespace Otherwise
{
	//Do not use regular throwError as it created an infinite recursion.
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
			Otherwise::throwFileError("Initialization", "The initialization file is missing or corrupt.");
		}
		return returnVector;
	}

	std::string Otherwise::readInitFileLine(unsigned int lineNumber)
	{
		std::string returnString = "";
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

			Otherwise::throwFileError("Initialization", "The initialization file is missing or corrupt.");
		}

		return returnString;
	}

	std::vector<std::string> Otherwise::readFile(std::string & filePath)
	{
		std::vector<std::string> returnVector;
		std::string tempString;
		std::ifstream initFile(filePath);
		if (initFile.is_open())
		{
			while (getline(initFile, tempString))
			{
				returnVector.push_back(tempString);
			}
		}
		else
		{
			Otherwise::throwFileError("Initialization", "The initialization file is missing or corrupt.");
		}
		return returnVector;
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
			Otherwise::throwFileError("Initialization", "The initialization file could not be created.");
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
			Otherwise::throwFileError("Initialization", "The initialization file is missing or corrupt.");
		}

		writeInitFile(tempFile);
	}
	void extractLinesFromFile(std::vector<std::string>* addTo, std::string & startLineMinusOne, std::string & endLinePlusOne, std::string & filePath)
	{
		unsigned int initSize = addTo->size();
		std::string tempString;
		std::fstream fileStream(filePath);
		if (fileStream.is_open())
		{
			while (getline(fileStream, tempString))
			{
				if (tempString == startLineMinusOne)
				{
					while (getline(fileStream, tempString))
					{
						if (tempString == endLinePlusOne)
						{
							break;
						}
						else
						{
							addTo->push_back(tempString);
						}
					}
				}
			}
		}
		else
		{
			throwError(filePath, filePath + " file was missing or currupted.");
		}

		if (initSize == addTo->size())
		{
			throwError("ExtractLinesFromFile", "When looking in file " + filePath + 
				" we could not find anything between the lines " + startLineMinusOne +
				" and " + endLinePlusOne);
		}
	}
}