#ifndef SMOOTHER
#define SMOOTHER
//------------------------------------------------------------------------
//
//  Name: Smoother.h
//
//  Desc: Template class to help calculate the average value of a history
//        of values. This can only be used with types that have a 'zero'
//        value and that have the += and / operators overloaded.
//
//        Example: Used to smooth frame rate calculations.
//
//  Author: Mat Buckland (fup@ai-junkie.com)
//
//------------------------------------------------------------------------
#include <vector>

template <class T>
class Smoother {
private:
	//this holds the history
	std::vector<T>	_history;

	int				_next_update_slot;

	//an example of the 'zero' value of the type to be smoothed. This
	//would be something like Vector2D(0,0)
	T				_zero_value;

public:

	//to instantiate a Smoother pass it the number of samples you want
	//to use in the smoothing, and an exampe of a 'zero' type
	Smoother(int sample_size, T zero_value)
		:
		_history(sample_size, zero_value),
		_zero_value(zero_value),
		_next_update_slot(0)
	{}

	//each time you want to get a new average, feed it the most recent value
	//and this method will return an average over the last SampleSize updates
	T update(const T& most_recent_value) {
		//overwrite the oldest value with the newest
		_history[_next_update_slot++] = most_recent_value;

		//make sure m_iNextUpdateSlot wraps around. 
		if (_next_update_slot == _history.size()) _next_update_slot = 0;

		//now to calculate the average of the history list
		T sum = _zero_value;

		std::vector<T>::iterator it = _history.begin();

		for (it; it != _history.end(); ++it) {
			sum += *it;
		}

		return sum / (double)_history.size();
	}
};


#endif