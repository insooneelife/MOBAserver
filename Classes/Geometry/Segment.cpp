#include "Segment.h"
#include "Geometry.h"

namespace geometry
{
	Segment::Segment()
	{
		setSegment(0.0, 0.0, 0.0, 0.0);
	}

	Segment::Segment(float sx, float sy, float ex, float ey)
	{
		setSegment(sx, sy, ex, ey);
	}

	Segment::Segment(const cocos2d::Vec2& start, const cocos2d::Vec2& end)
	{
		setSegment(start.x, start.y, end.x, end.y);
	}

	Segment::Segment(const Segment& copy)
	{
		setSegment(copy.start.x, copy.start.y, copy.end.x, copy.end.y);
	}

	Segment& Segment::operator=(const Segment& rhs)
	{
		setSegment(rhs.start.x, rhs.start.y, rhs.end.x, rhs.end.y);
		return *this;
	}

	void Segment::setSegment(float sx, float sy, float ex, float ey)
	{
		this->start.x = sx;
		this->start.y = sy;
		this->end.x = ex;
		this->end.y = ey;
	}

	cocos2d::Vec2 Segment::getDirection() const
	{
		return (end - start).getNormalized();
	}

	float Segment::getDistance() const
	{
		return start.getDistance(end);
	}

	float Segment::getDistanceSq() const
	{
		return start.getDistanceSq(end);
	}

	bool Segment::containPoint(const cocos2d::Vec2& point) const
	{
		return pointOnSegment(point, start, end);
	}

	void Segment::move(cocos2d::Vec2 move_val)
	{
		start += move_val;
		end += move_val;
	}

	float Segment::boundingRadius() const
	{
		return (start - end).getLength();
	}

	void Segment::render(cocos2d::DrawNode* panel)
	{
		render(panel, cocos2d::Color4F::WHITE);
	}

	void Segment::render(cocos2d::DrawNode* panel, cocos2d::Color4F color)
	{
		panel->drawLine(start, end, color);
	}

	bool Segment::intersect(const Shape* const other) const
	{
		return other->intersect(this);
	}

	bool Segment::intersect(const Rect* const other) const
	{
		return geometry::intersect(*this, *other);
	}

	bool Segment::intersect(const Segment* const other) const
	{
		return geometry::intersect(*this, *other);
	}

	bool Segment::intersect(const Circle* const other) const
	{
		return geometry::intersect(*this, *other);
	}

	bool Segment::intersect(const Polygon* const other) const
	{
		return geometry::intersect(*this, *other);
	}

	std::vector<cocos2d::Vec2> Segment::intersectPoints(const Shape* const other) const
	{
		return other->intersectPoints(this);
	}

	std::vector<cocos2d::Vec2> Segment::intersectPoints(const Rect* const other) const
	{
		return geometry::intersectPoints(*this, *other);
	}

	std::vector<cocos2d::Vec2> Segment::intersectPoints(const Segment* const other) const
	{
		return geometry::intersectPoints(*this, *other);
	}

	std::vector<cocos2d::Vec2> Segment::intersectPoints(const Circle* const other) const
	{
		return geometry::intersectPoints(*this, *other);
	}

	std::vector<cocos2d::Vec2> Segment::intersectPoints(const Polygon* const other) const
	{
		return geometry::intersectPoints(*this, *other);
	}

	bool Segment::overlap(const Shape* const other) const
	{
		return other->overlap(this);
	}

	bool Segment::overlap(const Rect* const other) const
	{
		return geometry::overlap(*this, *other);
	}

	bool Segment::overlap(const Segment* const other) const
	{
		return geometry::overlap(*this, *other);
	}

	bool Segment::overlap(const Circle* const other) const
	{
		return geometry::overlap(*this, *other);
	}

	bool Segment::overlap(const Polygon* const other) const
	{
		return geometry::overlap(*this, *other);
	}


	cocos2d::Vec2 Segment::enforceFrom(const Shape* const other) const
	{
		return other->enforceFromProxy(this);
	}

	cocos2d::Vec2 Segment::enforceFrom(const Rect* const other) const
	{
		return geometry::enforceFrom(*this, *other);
	}

	cocos2d::Vec2 Segment::enforceFrom(const Segment* const other) const
	{
		return geometry::enforceFrom(*this, *other);
	}

	cocos2d::Vec2 Segment::enforceFrom(const Circle* const other) const
	{
		return geometry::enforceFrom(*this, *other);
	}

	cocos2d::Vec2 Segment::enforceFrom(const Polygon* const other) const
	{
		return geometry::enforceFrom(*this, *other);
	}


	cocos2d::Vec2 Segment::enforceFromProxy(const Rect* const other) const
	{
		return other->enforceFrom(this);
	}

	cocos2d::Vec2 Segment::enforceFromProxy(const Segment* const other) const
	{
		return other->enforceFrom(this);
	}

	cocos2d::Vec2 Segment::enforceFromProxy(const Circle* const other) const
	{
		return other->enforceFrom(this);
	}

	cocos2d::Vec2 Segment::enforceFromProxy(const Polygon* const other) const
	{
		return other->enforceFrom(this);
	}
}