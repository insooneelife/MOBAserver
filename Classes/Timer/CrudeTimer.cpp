#pragma comment(lib, "winmm.lib")
#include "CrudeTimer.h"
#include <windows.h>

//set the start time
CrudeTimer::CrudeTimer()
{
	_start_time = timeGetTime() * 0.001;
}

double CrudeTimer::getCurrentTime() 
{
	return timeGetTime() * 0.001 - _start_time; 
}