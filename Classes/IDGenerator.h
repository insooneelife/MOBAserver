#pragma once
#include <algorithm>
#include <set>
#include "Singleton.h"
#include "Util.h"
#include "cocos2d.h"

//------------------------------------------------------------------------
//
//  Name:   IDGenerator.h
//
//  Desc:   Generates unique ID.
//			0 is the ID which will not be generated.
//			So you can use it for checking invalid ID.
//			Note this class needs to be modified when entity population increases.
//
//  Author: Insub Im (insooneelife@naver.com)
//
//------------------------------------------------------------------------

//provide easy access
#define IDGen IDGenerator::getInstance()

class IDGenerator : public Singleton<IDGenerator> {

public:
	const unsigned int genID() {
		unsigned int temp = 
			(unsigned int)util::genRand<1, std::numeric_limits<unsigned int>::max()>();
		
		auto already = _using_id.find(temp);

		CC_ASSERT(_using_id.size() < std::numeric_limits<unsigned int>::max() / 2);

		if (already != end(_using_id)) {
			return genID();
		}
		else {
			_using_id.insert(temp);
			return temp;
		}
	}

private:
	IDGenerator(const IDGenerator&) = delete; // no copies
	IDGenerator& operator=(const IDGenerator&) = delete; // no self-assignments
	IDGenerator() {}
	friend class Singleton<IDGenerator>;

	std::set<unsigned int> _using_id;
};