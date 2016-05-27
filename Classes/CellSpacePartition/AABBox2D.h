#ifndef INVAABBOX2D_H
#define INVAABBOX2D_H
//-----------------------------------------------------------------------------
//
//  Name:   AABBox2D.h
//
//  Author: Mat Buckland (www.ai-junkie.com)
//
//  Desc:   v simple inverted (y increases down screen) axis aligned bounding
//          box class
//-----------------------------------------------------------------------------

#include "cocos2d.h"

class AABBox2D
{
private:
	cocos2d::Vec2		_bottom_left;
	cocos2d::Vec2		_top_right;
	cocos2d::Vec2		_center;

public:
	AABBox2D(
		cocos2d::Vec2 bl,
		cocos2d::Vec2 tr)
		:
		_bottom_left(bl),
		_top_right(tr),
		_center((bl + tr) / 2.0)
	{
	}

	//returns true if the bbox described by other intersects with this one
	bool isOverlappedWith(const AABBox2D& other) const
	{
		return !((other.getTop() < this->getBottom()) ||
				(other.getBottom() > this->getTop()) ||
				(other.getLeft() > this->getRight()) ||
				(other.getRight() < this->getLeft()));
	}

	const cocos2d::Vec2& getTopRight()		const { return _top_right; }
	const cocos2d::Vec2& getBottomLeft()	const { return _bottom_left; }
	const cocos2d::Vec2& getCenter()		const { return _center; }

	double    getTop()		const { return _top_right.y; }
	double    getLeft()		const { return _bottom_left.x; }
	double    getBottom()	const { return _bottom_left.y; }
	double    getRight()	const { return _top_right.x; }

};

#endif