#pragma once

//------------------------------------------------------------------------
//
//  Name:   Circle.h
//
//  Desc:   Functions for 2D circle.
//
//  Author: Insub Im (insooneelife@naver.com)
//------------------------------------------------------------------------

#include "cocos2d.h"
#include "Shape.h"

namespace geometry
{
	class Circle : public Shape
	{
	public:

		cocos2d::Vec2           origin;

		float                   radius;

		Circle();

		Circle(float x, float y, float r);

		Circle(const cocos2d::Vec2& origin, float r);

		Circle(const Circle& copy);

		Circle& operator=(const Circle& rhs);

		void setCircle(float x, float y, float r);

		Circle getTranslatedCircle(const cocos2d::Vec2& dir, float distance);

		bool containPoint(const cocos2d::Vec2& point) const;

		bool containPoint(const std::vector<cocos2d::Vec2>& ps);

		static const Circle ZERO;

		virtual ~Circle() {}

		virtual void move(cocos2d::Vec2 move_val) override;

		virtual float boundingRadius() const override;

		virtual void render(cocos2d::DrawNode* panel) override;

		virtual void render(cocos2d::DrawNode* panel, cocos2d::Color4F color) override;

		virtual bool intersect(const Shape* const other) const override;

		virtual bool intersect(const Rect* const other) const override;

		virtual bool intersect(const Segment* const other) const override;

		virtual bool intersect(const Circle* const other) const override;

		virtual bool intersect(const Polygon* const other) const override;

		virtual std::vector<cocos2d::Vec2>
			intersectPoints(const Shape* const other) const override;

		virtual std::vector<cocos2d::Vec2>
			intersectPoints(const Rect* const other) const override;

		virtual std::vector<cocos2d::Vec2>
			intersectPoints(const Segment* const other) const override;

		virtual std::vector<cocos2d::Vec2>
			intersectPoints(const Circle* const other) const override;

		virtual std::vector<cocos2d::Vec2>
			intersectPoints(const Polygon* const other) const override;


		virtual bool overlap(const Shape* const other) const override;

		virtual bool overlap(const Rect* const other) const override;

		virtual bool overlap(const Segment* const other) const override;

		virtual bool overlap(const Circle* const other) const override;

		virtual bool overlap(const Polygon* const other) const override;


		virtual cocos2d::Vec2 enforceFrom(const Shape* const other) const override;

		virtual cocos2d::Vec2 enforceFrom(const Rect* const other) const override;

		virtual cocos2d::Vec2 enforceFrom(const Segment* const other) const override;

		virtual cocos2d::Vec2 enforceFrom(const Circle* const other) const override;

		virtual cocos2d::Vec2 enforceFrom(const Polygon* const other) const override;


		virtual cocos2d::Vec2 enforceFromProxy(const Rect* const other) const override;

		virtual cocos2d::Vec2 enforceFromProxy(const Segment* const other) const override;

		virtual cocos2d::Vec2 enforceFromProxy(const Circle* const other) const override;

		virtual cocos2d::Vec2 enforceFromProxy(const Polygon* const other) const override;
	};
}