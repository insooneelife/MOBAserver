#pragma once
#include "cocos2d.h"
#include "Shape.h"

//------------------------------------------------------------------------
//
//  Name:   Rect.h
//
//  Desc:   Functions for 2D rectangle.
//
//  Author: Insub Im (insooneelife@naver.com)
//------------------------------------------------------------------------

namespace geometry
{
	class Rect : public Shape
	{

	public:

		cocos2d::Vec2           origin;

		float                   width;

		float                   height;

		Rect();

		Rect(float x, float y, float width, float height);

		Rect(const cocos2d::Vec2& pos, float width, float height);

		Rect(const cocos2d::Vec2& bottom_left, const cocos2d::Vec2& top_right);

		Rect(const Rect& copy);

		Rect& operator=(const Rect& rhs);

		void setRect(float x, float y, float width, float height);

		float getMinX() const;

		float getMidX() const;

		float getMaxX() const;

		float getMinY() const;

		float getMidY() const;

		float getMaxY() const;

		bool equals(const Rect& rect) const;

		bool containPoint(const cocos2d::Vec2& point) const;

		static const Rect ZERO;

		virtual ~Rect() {}

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