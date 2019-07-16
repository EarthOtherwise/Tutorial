#pragma once
#include "OSInterface.h"

namespace Otherwise
{

class ProgramClock
{
public:
	ProgramClock();
	~ProgramClock();

	void set();

	unsigned int getDeltaTime();

	unsigned int getSet();

	unsigned int getProgramUpTime();

private:
	unsigned int mPreviousTime;
};

}