#include "ProgramClock.h"

namespace Otherwise
{

ProgramClock::ProgramClock()
{
}


ProgramClock::~ProgramClock()
{
}

void ProgramClock::set()
{
	mPreviousTime = millisecondsSinceProgramStart();
}

unsigned ProgramClock::getDeltaTime()
{
	return millisecondsSinceProgramStart() - mPreviousTime;
}

unsigned ProgramClock::getSet()
{
	unsigned int returnUInt = getDeltaTime();
	set();
	return returnUInt;
}

unsigned ProgramClock::getProgramUpTime()
{
	return millisecondsSinceProgramStart();
}
}
