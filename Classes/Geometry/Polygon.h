#pragma once
#include "cocos2d.h"
#include "Shape.h"

//------------------------------------------------------------------------
//
//  Name:   Polygon.h
//
//  Desc:   Functions for 2D polygon.
//
//  Author: Insub Im (insooneelife@naver.com)
//------------------------------------------------------------------------

namespace geometry
{
	class Polygon : public Shape
	{
	public:

		std::vector<cocos2d::Vec2> vertices;

		Polygon();

		Polygon(const std::vector<cocos2d::Vec2>& segs);

		Polygon(const Polygon& copy);

		Polygon& operator=(const Polygon& rhs);

		void setPolygon(const std::vector<cocos2d::Vec2>& segs);

		void pushVertex(const cocos2d::Vec2 point);

		bool containPoint(const cocos2d::Vec2& point) const;

		virtual ~Polygon() { vertices.clear(); }

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