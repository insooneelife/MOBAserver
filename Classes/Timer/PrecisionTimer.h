#ifndef PRECISION_TIMER_H
#define PRECISION_TIMER_H

//-----------------------------------------------------------------------
//
//  Name: PrecisionTimer.h
//
//  Author: Mat Buckland 2002
//
//  Desc: Windows timer class.
//
//        nb. this only uses the high performance timer. There is no
//        support for ancient computers. I know, I know, I should add
//        support, but hey, I have shares in AMD and Intel... Go upgrade ;o)
//
//-----------------------------------------------------------------------

#include "cocos2d.h"

class PrecisionTimer {
public:
	//ctors
	PrecisionTimer();
	PrecisionTimer(double fps);

	//whatdayaknow, this starts the timer
	void    start();

	//determines if enough time has passed to move onto next frame
	inline bool    readyForNextFrame();

	//only use this after a call to the above.
	//double  GetTimeElapsed(){return _time_elapsed;}

	double  timeElapsed();

	double  currentTime();

	bool    started() const;

	void    smoothUpdatesOn();

	void    smoothUpdatesOff();

private:
	long long int
			_current_time,
			_last_time,
			_last_time_in_time_elapsed,
			_next_time,
			_start_time,
			_frame_time,
			_perf_count_freq;

	double	
			_time_elapsed,
			_last_time_elapsed,
			_time_scale;

	double	_normal_FPS;

	double	_slow_FPS;

	bool	_started;

	//if true a call to TimeElapsed() will return 0 if the current
	//time elapsed is much smaller than the previous. Used to counter
	//the problems associated with the user using menus/resizing/moving 
	//a window etc
	bool	_smooth_updates;
};



#endif


