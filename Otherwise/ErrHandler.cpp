#include "ErrHandler.h"

namespace Otherwise
{
	std::string dateTime()
	{
		time_t rawtime;
		struct tm timeinfo;
		char buffer[80];

		time(&rawtime);
		localtime_s(&timeinfo, &rawtime);

		strftime(buffer, 80, "%d-%m-%Y %H-%M-%S", &timeinfo);
		return std::string(buffer);
	}

	void throwFileError(std::string errorWord, std::string errorString)
	{
		std::string errorFileName = "Errors/" + dateTime() + " " + errorWord + ".txt";
		std::ofstream errorFile(errorFileName);
		errorFile << errorString;
		errorFile.close();

		/*Pop up a window indicating there was an error, print out the error word, as well as the filepath to the errorfile*/

		SDL_Quit();
		exit(60);
	}

	void throwConsoleError(std::string errorString)
	{
		std::cout << errorString << '\n' << "Do you want to continue?" << '\n';
		int a = 0;
		std::cin >> a;
		if (a == 0)
		{
			SDL_Quit();
			exit(60);
		}
	}

	void throwError(std::string errorWord, std::string errorString)
	{
		std::string debugString = "Debug";
		if (debugString == readInitFileLine(2))
		{
			throwConsoleError(errorString);
		}
		else
		{
			throwFileError(errorWord, errorString);
		}
	}
}