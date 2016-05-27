#include "Polygon.h"
#include "Geometry.h"

namespace geometry
{
	Polygon::Polygon()
	{}

	Polygon::Polygon(const std::vector<cocos2d::Vec2>& segs)
	{
		setPolygon(segs);
	}

	Polygon::Polygon(const Polygon& copy)
	{
		setPolygon(copy.vertices);
	}

	Polygon& Polygon::operator=(const Polygon& rhs)
	{
		setPolygon(rhs.vertices);
		return *this;
	}

	void Polygon::setPolygon(const std::vector<cocos2d::Vec2>& segs)
	{
		this->vertices.clear();

		for (std::vector<cocos2d::Vec2>::size_type i = 0; i < segs.size(); ++i)
		{
			this->vertices.push_back(segs[i]);
		}
	}

	void Polygon::pushVertex(const cocos2d::Vec2 point)
	{
		this->vertices.push_back(point);
	}

	//need some more tests 
	//because boundarys are not included in functions pointInPolygon.
	bool Polygon::containPoint(const cocos2d::Vec2& point) const
	{
		return pointInPolygon(point, this->vertices);
	}

	void Polygon::move(cocos2d::Vec2 move_val)
	{
		for (auto e : vertices)
			e += move_val;
	}

	float Polygon::boundingRadius() const
	{
		return polygonMaxDist(vertices);
	}

	void Polygon::render(cocos2d::DrawNode* panel)
	{
		render(panel, cocos2d::Color4F::WHITE);
	}

	void Polygon::render(cocos2d::DrawNode* panel, cocos2d::Color4F color)
	{
		for (size_t i = 0; i < vertices.size(); i++)
		{
			size_t j = (i + 1) % vertices.size();
			panel->drawLine(vertices[i], vertices[j], color);
		}
	}

	bool Polygon::intersect(const Shape* const other) const
	{
		return other->intersect(this);
	}

	bool Polygon::intersect(const Rect* const other) const
	{
		return geometry::intersect(*this, *other);
	}

	bool Polygon::intersect(const Segment* const other) const
	{
		return geometry::intersect(*this, *other);
	}

	bool Polygon::intersect(const Circle* const other) const
	{
		return geometry::intersect(*this, *other);
	}

	bool Polygon::intersect(const Polygon* const other) const
	{
		return geometry::intersect(*this, *other);
	}

	std::vector<cocos2d::Vec2> Polygon::intersectPoints(const Shape* const other) const
	{
		return other->intersectPoints(this);
	}

	std::vector<cocos2d::Vec2> Polygon::intersectPoints(const Rect* const other) const
	{
		return geometry::intersectPoints(*this, *other);
	}

	std::vector<cocos2d::Vec2> Polygon::intersectPoints(const Segment* const other) const
	{
		return geometry::intersectPoints(*this, *other);
	}

	std::vector<cocos2d::Vec2> Polygon::intersectPoints(const Circle* const other) const
	{
		return geometry::intersectPoints(*this, *other);
	}

	std::vector<cocos2d::Vec2> Polygon::intersectPoints(const Polygon* const other) const
	{
		return geometry::intersectPoints(*this, *other);
	}

	bool Polygon::overlap(const Shape* const other) const
	{
		return other->overlap(this);
	}

	bool Polygon::overlap(const Rect* const other) const
	{
		return geometry::overlap(*this, *other);
	}

	bool Polygon::overlap(const Segment* const other) const
	{
		return geometry::overlap(*this, *other);
	}

	bool Polygon::overlap(const Circle* const other) const
	{
		return geometry::overlap(*this, *other);
	}

	bool Polygon::overlap(const Polygon* const other) const
	{
		return geometry::overlap(*this, *other);
	}


	cocos2d::Vec2 Polygon::enforceFrom(const Shape* const other) const
	{
		return other->enforceFromProxy(this);
	}

	cocos2d::Vec2 Polygon::enforceFrom(const Rect* const other) const
	{
		return geometry::enforceFrom(*this, *other);
	}

	cocos2d::Vec2 Polygon::enforceFrom(const Segment* const other) const
	{
		return geometry::enforceFrom(*this, *other);
	}

	cocos2d::Vec2 Polygon::enforceFrom(const Circle* const other) const
	{
		return geometry::enforceFrom(*this, *other);
	}

	cocos2d::Vec2 Polygon::enforceFrom(const Polygon* const other) const
	{
		return geometry::enforceFrom(*this, *other);
	}


	cocos2d::Vec2 Polygon::enforceFromProxy(const Rect* const other) const
	{
		return other->enforceFrom(this);
	}

	cocos2d::Vec2 Polygon::enforceFromProxy(const Segment* const other) const
	{
		return other->enforceFrom(this);
	}

	cocos2d::Vec2 Polygon::enforceFromProxy(const Circle* const other) const
	{
		return other->enforceFrom(this);
	}

	cocos2d::Vec2 Polygon::enforceFromProxy(const Polygon* const other) const
	{
		return other->enforceFrom(this);
	}
}