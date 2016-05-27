#ifndef REGULATOR
#define REGULATOR
//------------------------------------------------------------------------
//
//  Name:   Regulator.h
//
//  Desc:   Use this class to regulate code flow (for an update function say)
//          Instantiate the class with the frequency you would like your code
//          section to flow (like 10 times per second) and then only allow 
//          the program flow to continue if Ready() returns true
//
//  Author: Mat Buckland 2003 (fup@ai-junkie.com)
//
//------------------------------------------------------------------------

class Regulator
{
private:

	//the time period between updates 
	double	_update_period;

	//the next time the regulator allows code flow
	unsigned long	_next_update_time;

public:

	explicit Regulator(double num_updates_per_second);

	//returns true if the current time exceeds _next_update_time
	bool isReady();
};



#endif