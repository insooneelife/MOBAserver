#pragma once

//------------------------------------------------------------------------
//
//  Name:   Shape.h
//
//  Desc:   Functions for 2D abstract shape.
//			With this class all shapes can intersect each other.
//
//  Author: Insub Im (insooneelife@naver.com)
//------------------------------------------------------------------------

#include <vector>
#include "cocos2d.h"

namespace geometry
{
	class Shape;
	class Circle;
	class Polygon;
	class Segment;
	class Rect;

	class Shape
	{
	public:

		virtual void move(cocos2d::Vec2 move_val) = 0;

		virtual float boundingRadius() const = 0;

		virtual void render(cocos2d::DrawNode* panel) = 0;

		virtual void render(cocos2d::DrawNode* panel, cocos2d::Color4F color) = 0;

		virtual bool intersect(const Shape* const other) const = 0;

		virtual bool intersect(const Rect* const other) const = 0;

		virtual bool intersect(const Segment* const other) const = 0;

		virtual bool intersect(const Circle* const other) const = 0;

		virtual bool intersect(const Polygon* const other) const = 0;

		virtual std::vector<cocos2d::Vec2>
			intersectPoints(const Shape* const other) const = 0;

		virtual std::vector<cocos2d::Vec2>
			intersectPoints(const Rect* const other) const = 0;

		virtual std::vector<cocos2d::Vec2>
			intersectPoints(const Segment* const other) const = 0;

		virtual std::vector<cocos2d::Vec2>
			intersectPoints(const Circle* const other) const = 0;

		virtual std::vector<cocos2d::Vec2>
			intersectPoints(const Polygon* const other) const = 0;

		virtual bool overlap(const Shape* const other) const = 0;

		virtual bool overlap(const Rect* const other) const = 0;

		virtual bool overlap(const Segment* const other) const = 0;

		virtual bool overlap(const Circle* const other) const = 0;

		virtual bool overlap(const Polygon* const other) const = 0;

		virtual cocos2d::Vec2 enforceFrom(const Shape* const other) const = 0;

		virtual cocos2d::Vec2 enforceFrom(const Rect* const other) const = 0;

		virtual cocos2d::Vec2 enforceFrom(const Segment* const other) const = 0;

		virtual cocos2d::Vec2 enforceFrom(const Circle* const other) const = 0;

		virtual cocos2d::Vec2 enforceFrom(const Polygon* const other) const = 0;

		virtual cocos2d::Vec2 enforceFromProxy(const Rect* const other) const = 0;

		virtual cocos2d::Vec2 enforceFromProxy(const Segment* const other) const = 0;

		virtual cocos2d::Vec2 enforceFromProxy(const Circle* const other) const = 0;

		virtual cocos2d::Vec2 enforceFromProxy(const Polygon* const other) const = 0;
	};
}