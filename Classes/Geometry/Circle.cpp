#include "Circle.h"
#include "Geometry.h"

namespace geometry
{
	Circle::Circle()
	{
		setCircle(0.0, 0.0, 0.0);
	}

	Circle::Circle(float x, float y, float r)
	{
		setCircle(x, y, r);
	}

	Circle::Circle(const cocos2d::Vec2& origin, float r)
	{
		setCircle(origin.x, origin.y, r);
	}

	Circle::Circle(const Circle& copy)
	{
		setCircle(copy.origin.x, copy.origin.y, copy.radius);
	}

	Circle& Circle::operator=(const Circle& rhs)
	{
		setCircle(rhs.origin.x, rhs.origin.y, rhs.radius);
		return *this;
	}

	void Circle::setCircle(float x, float y, float r)
	{
		this->origin.x = x;
		this->origin.y = y;
		this->radius = r;
	}

	Circle Circle::getTranslatedCircle(const cocos2d::Vec2& dir, float distance)
	{
		return Circle(origin.x + dir.x * distance, origin.y + dir.y * distance, radius);
	}

	bool Circle::containPoint(const cocos2d::Vec2& point) const
	{
		return (origin.getDistanceSq(point) <=
			radius * radius + std::numeric_limits<float>::epsilon());
	}

	bool Circle::containPoint(const std::vector<cocos2d::Vec2>& ps)
	{
		for (auto p : ps)
		{
			if (!this->containPoint(p))
				return false;
		}
		return true;
	}

	const Circle Circle::ZERO = Circle(cocos2d::Vec2::ZERO, 0.0f);

	void Circle::move(cocos2d::Vec2 move_val)
	{
		origin += move_val;
	}

	float Circle::boundingRadius() const
	{
		return radius;
	}

	void Circle::render(cocos2d::DrawNode* panel)
	{
		render(panel, cocos2d::Color4F::WHITE);
	}

	void Circle::render(cocos2d::DrawNode* panel, cocos2d::Color4F color)
	{
		panel->drawCircle(origin, radius, (float)(M_PI * 2), 40, false, color);
	}

	bool Circle::intersect(const Shape* const other) const
	{
		return other->intersect(this);
	}

	bool Circle::intersect(const Rect* const other) const
	{
		return geometry::intersect(*this, *other);
	}

	bool Circle::intersect(const Segment* const other) const
	{
		return geometry::intersect(*this, *other);
	}

	bool Circle::intersect(const Circle* const other) const
	{
		return geometry::intersect(*this, *other);
	}

	bool Circle::intersect(const Polygon* const other) const
	{
		return geometry::intersect(*this, *other);
	}

	std::vector<cocos2d::Vec2> Circle::intersectPoints(const Shape* const other) const
	{
		return other->intersectPoints(this);
	}

	std::vector<cocos2d::Vec2> Circle::intersectPoints(const Rect* const other) const
	{
		return geometry::intersectPoints(*this, *other);
	}

	std::vector<cocos2d::Vec2> Circle::intersectPoints(const Segment* const other) const
	{
		return geometry::intersectPoints(*this, *other);
	}

	std::vector<cocos2d::Vec2> Circle::intersectPoints(const Circle* const other) const
	{
		return geometry::intersectPoints(*this, *other);
	}

	std::vector<cocos2d::Vec2> Circle::intersectPoints(const Polygon* const other) const
	{
		return geometry::intersectPoints(*this, *other);
	}

	bool Circle::overlap(const Shape* const other) const
	{
		return other->overlap(this);
	}

	bool Circle::overlap(const Rect* const other) const
	{
		return geometry::overlap(*this, *other);
	}

	bool Circle::overlap(const Segment* const other) const
	{
		return geometry::overlap(*this, *other);
	}

	bool Circle::overlap(const Circle* const other) const
	{
		return geometry::overlap(*this, *other);
	}

	bool Circle::overlap(const Polygon* const other) const
	{
		return geometry::overlap(*this, *other);
	}


	cocos2d::Vec2 Circle::enforceFrom(const Shape* const other) const
	{
		return other->enforceFromProxy(this);
	}

	cocos2d::Vec2 Circle::enforceFrom(const Rect* const other) const
	{
		return geometry::enforceFrom(*this, *other);
	}

	cocos2d::Vec2 Circle::enforceFrom(const Segment* const other) const
	{
		return geometry::enforceFrom(*this, *other);
	}

	cocos2d::Vec2 Circle::enforceFrom(const Circle* const other) const
	{
		return geometry::enforceFrom(*this, *other);
	}

	cocos2d::Vec2 Circle::enforceFrom(const Polygon* const other) const
	{
		return geometry::enforceFrom(*this, *other);
	}


	cocos2d::Vec2 Circle::enforceFromProxy(const Rect* const other) const
	{
		return other->enforceFrom(this);
	}

	cocos2d::Vec2 Circle::enforceFromProxy(const Segment* const other) const
	{
		return other->enforceFrom(this);
	}

	cocos2d::Vec2 Circle::enforceFromProxy(const Circle* const other) const
	{
		return other->enforceFrom(this);
	}

	cocos2d::Vec2 Circle::enforceFromProxy(const Polygon* const other) const
	{
		return other->enforceFrom(this);
	}
}