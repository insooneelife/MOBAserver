#ifndef FRAMECOUNTER_H
#define FRAMECOUNTER_H

//------------------------------------------------------------------------
//
//  Name:   EntityManager.h
//
//  Desc:   An clock for message time handling.          
//
//  Author: Insub Im (insooneelife@naver.com)
//
//------------------------------------------------------------------------

#include "../Singleton.h"

//provide easy access
#define FrameCnt FrameCounter::getInstance()

class FrameCounter : public Singleton<FrameCounter> {
public:
	inline void update() {
		++_count; ++_frames_elapsed;
	}

	inline long getCurrentFrame() {
		return _count;
	}

	inline void reset() {
		_count = 0;
	}

	inline void start() {
		_frames_elapsed = 0;
	}

	inline int  framesElapsedSinceStartCalled() const {
		return _frames_elapsed;
	}

private:
	FrameCounter(const FrameCounter&) = delete; // no copies
	FrameCounter& operator=(const FrameCounter&) = delete; // no self-assignments
	FrameCounter() : _count(0), _frames_elapsed(0) {}
	friend class Singleton<FrameCounter>;

	long _count;
	int  _frames_elapsed;
};

#endif