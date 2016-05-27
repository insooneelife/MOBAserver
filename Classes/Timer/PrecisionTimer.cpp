#include "PrecisionTimer.h"


//---------------------- default constructor ------------------------------
//
//-------------------------------------------------------------------------
PrecisionTimer::PrecisionTimer() : _normal_FPS(0.0),
_slow_FPS(1.0),
_time_elapsed(0.0),
_frame_time(0),
_last_time(0),
_last_time_in_time_elapsed(0),
_perf_count_freq(0),
_started(false),
_start_time(0),
_last_time_elapsed(0.0),
_smooth_updates(false)
{
	//how many ticks per sec do we get
	QueryPerformanceFrequency((LARGE_INTEGER*)&_perf_count_freq);

	_time_scale = 1.0 / _perf_count_freq;
}

//---------------------- constructor -------------------------------------
//
//  use to specify FPS
//
//-------------------------------------------------------------------------
PrecisionTimer::PrecisionTimer(double fps) : _normal_FPS(fps),
_slow_FPS(1.0),
_time_elapsed(0.0),
_frame_time(0),
_last_time(0),
_last_time_in_time_elapsed(0),
_perf_count_freq(0),
_started(false),
_start_time(0),
_last_time_elapsed(0.0),
_smooth_updates(false)
{

	//how many ticks per sec do we get
	QueryPerformanceFrequency((LARGE_INTEGER*)&_perf_count_freq);

	_time_scale = 1.0 / _perf_count_freq;

	//calculate ticks per frame
	_frame_time = (LONGLONG)(_perf_count_freq / _normal_FPS);
}




//------------------------Start()-----------------------------------------
//
//  call this immediately prior to game loop. Starts the timer (obviously!)
//
//--------------------------------------------------------------------------
void PrecisionTimer::start()
{
	_started = true;

	_time_elapsed = 0.0;

	//get the time
	QueryPerformanceCounter((LARGE_INTEGER*)&_last_time);

	//keep a record of when the timer was started
	_start_time = _last_time_in_time_elapsed = _last_time;

	//update time to render next frame
	_next_time = _last_time + _frame_time;

	return;
}



//-------------------------ReadyForNextFrame()-------------------------------
//
//  returns true if it is time to move on to the next frame step. getTo be used if
//  FPS is set.
//
//----------------------------------------------------------------------------
inline bool PrecisionTimer::readyForNextFrame()
{
	CCASSERT(_normal_FPS, "PrecisionTimer::ReadyForNextFrame<No FPS set in timer>");

	QueryPerformanceCounter((LARGE_INTEGER*)&_current_time);

	if (_current_time > _next_time) {

		_time_elapsed = (_current_time - _last_time) * _time_scale;
		_last_time = _current_time;

		//update time to render next frame
		_next_time = _current_time + _frame_time;

		return true;
	}

	return false;
}

//--------------------------- TimeElapsed --------------------------------
//
//  returns time elapsed since last call to this function.
//-------------------------------------------------------------------------
double PrecisionTimer::timeElapsed()
{
	_last_time_elapsed = _time_elapsed;

	QueryPerformanceCounter((LARGE_INTEGER*)&_current_time);

	_time_elapsed = (_current_time - _last_time_in_time_elapsed) * _time_scale;

	_last_time_in_time_elapsed = _current_time;

	const double Smoothness = 5.0;

	if (_smooth_updates) {
		if (_time_elapsed < (_last_time_elapsed * Smoothness)) {
			return _time_elapsed;
		}
		else {
			return 0.0;
		}
	}
	else {
		return _time_elapsed;
	}
}

double PrecisionTimer::currentTime()
{
	QueryPerformanceCounter((LARGE_INTEGER*)&_current_time);

	return (_current_time - _start_time) * _time_scale;
}

bool PrecisionTimer::started() const 
{
	return _started; 
}

void PrecisionTimer::smoothUpdatesOn()
{
	_smooth_updates = true; 
}

void PrecisionTimer::smoothUpdatesOff() 
{
	_smooth_updates = false; 
}