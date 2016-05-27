#include "Rect.h"
#include "Geometry.h"

namespace geometry
{
	Rect::Rect()
	{
		setRect(0.0, 0.0, 0.0, 0.0);
	}

	Rect::Rect(float x, float y, float width, float height)
	{
		setRect(x, y, width, height);
	}

	Rect::Rect(const cocos2d::Vec2& pos, float width, float height)
	{
		setRect(pos.x, pos.y, width, height);
	}

	Rect::Rect(const cocos2d::Vec2& bottom_left, const cocos2d::Vec2& top_right)
	{
		setRect(bottom_left.x, bottom_left.y, top_right.x - bottom_left.x, top_right.y - bottom_left.y);
	}

	Rect::Rect(const Rect& copy)
	{
		setRect(copy.origin.x, copy.origin.y, copy.width, copy.height);
	}

	Rect& Rect::operator=(const Rect& rhs)
	{
		setRect(rhs.origin.x, rhs.origin.y, rhs.width, rhs.height);
		return *this;
	}

	void Rect::setRect(float x, float y, float width, float height)
	{
		origin.x = x;
		origin.y = y;
		this->width = width;
		this->height = height;
	}

	float Rect::getMinX() const
	{
		return origin.x;
	}

	float Rect::getMidX() const
	{
		return origin.x + width / 2.0;
	}

	float Rect::getMaxX() const
	{
		return origin.x + width;
	}

	float Rect::getMinY() const
	{
		return origin.y;
	}

	float Rect::getMidY() const
	{
		return origin.y + height / 2.0;
	}

	float Rect::getMaxY() const
	{
		return origin.y + height;
	}

	bool Rect::equals(const Rect& rect) const
	{
		return (origin == rect.origin && width == rect.width && height == rect.height);
	}

	bool Rect::containPoint(const cocos2d::Vec2& point) const
	{
		return (point.x >= getMinX() && point.x <= getMaxX() && point.y >= getMinY() && point.y <= getMaxY());
	}

	const Rect Rect::ZERO = Rect(0.0, 0.0, 0.0, 0.0);

	void Rect::move(cocos2d::Vec2 move_val)
	{
		origin += move_val;
	}

	float Rect::boundingRadius() const
	{
		return (origin - cocos2d::Vec2(origin.x + width, origin.y + height)).getLength();
	}

	void Rect::render(cocos2d::DrawNode* panel)
	{
		render(panel, cocos2d::Color4F::WHITE);
	}

	void Rect::render(cocos2d::DrawNode* panel, cocos2d::Color4F color)
	{
		panel->drawLine(
			origin,
			cocos2d::Vec2(origin.x + width, origin.y),
			color);
		panel->drawLine(
			cocos2d::Vec2(origin.x + width, origin.y),
			cocos2d::Vec2(origin.x + width, origin.y + height),
			color);
		panel->drawLine(
			cocos2d::Vec2(origin.x + width, origin.y + height),
			cocos2d::Vec2(origin.x, origin.y + height),
			color);
		panel->drawLine(
			cocos2d::Vec2(origin.x, origin.y + height),
			origin,
			color);
	}

	bool Rect::intersect(const Shape* const other) const
	{
		return other->intersect(this);
	}

	bool Rect::intersect(const Rect* const other) const
	{
		return geometry::intersect(*this, *other);
	}

	bool Rect::intersect(const Segment* const other) const
	{
		return geometry::intersect(*this, *other);
	}

	bool Rect::intersect(const Circle* const other) const
	{
		return geometry::intersect(*this, *other);
	}

	bool Rect::intersect(const Polygon* const other) const
	{
		return geometry::intersect(*this, *other);
	}

	std::vector<cocos2d::Vec2> Rect::intersectPoints(const Shape* const other) const
	{
		return other->intersectPoints(this);
	}

	std::vector<cocos2d::Vec2> Rect::intersectPoints(const Rect* const other) const
	{
		return geometry::intersectPoints(*this, *other);
	}

	std::vector<cocos2d::Vec2> Rect::intersectPoints(const Segment* const other) const
	{
		return geometry::intersectPoints(*this, *other);
	}

	std::vector<cocos2d::Vec2> Rect::intersectPoints(const Circle* const other) const
	{
		return geometry::intersectPoints(*this, *other);
	}

	std::vector<cocos2d::Vec2> Rect::intersectPoints(const Polygon* const other) const
	{
		return geometry::intersectPoints(*this, *other);
	}

	bool Rect::overlap(const Shape* const other) const
	{
		return other->overlap(this);
	}

	bool Rect::overlap(const Rect* const other) const
	{
		return geometry::overlap(*this, *other);
	}

	bool Rect::overlap(const Segment* const other) const
	{
		return geometry::overlap(*this, *other);
	}

	bool Rect::overlap(const Circle* const other) const
	{
		return geometry::overlap(*this, *other);
	}

	bool Rect::overlap(const Polygon* const other) const
	{
		return geometry::overlap(*this, *other);
	}


	cocos2d::Vec2 Rect::enforceFrom(const Shape* const other) const
	{
		return other->enforceFromProxy(this);
	}

	cocos2d::Vec2 Rect::enforceFrom(const Rect* const other) const
	{
		return geometry::enforceFrom(*this, *other);
	}

	cocos2d::Vec2 Rect::enforceFrom(const Segment* const other) const
	{
		return geometry::enforceFrom(*this, *other);
	}

	cocos2d::Vec2 Rect::enforceFrom(const Circle* const other) const
	{
		return geometry::enforceFrom(*this, *other);
	}

	cocos2d::Vec2 Rect::enforceFrom(const Polygon* const other) const
	{
		return geometry::enforceFrom(*this, *other);
	}


	cocos2d::Vec2 Rect::enforceFromProxy(const Rect* const other) const
	{
		return other->enforceFrom(this);
	}

	cocos2d::Vec2 Rect::enforceFromProxy(const Segment* const other) const
	{
		return other->enforceFrom(this);
	}

	cocos2d::Vec2 Rect::enforceFromProxy(const Circle* const other) const
	{
		return other->enforceFrom(this);
	}

	cocos2d::Vec2 Rect::enforceFromProxy(const Polygon* const other) const
	{
		return other->enforceFrom(this);
	}
}