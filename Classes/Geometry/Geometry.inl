#include "Geometry.h"

namespace geometry
{
	//---------------------------- Rect Intersect -------------------------------//

	inline bool intersect(const Rect& a, const Rect& b)
	{
		return rectanglesIntersect(
			cocos2d::Vec2(a.getMinX(), a.getMinY()),
			cocos2d::Vec2(a.getMaxX(), a.getMaxY()),
			cocos2d::Vec2(b.getMinX(), b.getMinY()),
			cocos2d::Vec2(b.getMaxX(), b.getMaxY()));
	}

	inline bool intersect(const Rect& rect, const Circle& circle)
	{
		return rectangleCircleIntersect(
			rect.origin,
			rect.origin + cocos2d::Vec2(rect.width, rect.height),
			circle.origin,
			circle.radius);
	}

	inline bool intersect(const Rect& rect, const Segment& segment)
	{
		return rectangleSegmentIntersect(
			rect.origin,
			rect.origin + cocos2d::Vec2(rect.width, rect.height),
			segment.start,
			segment.end);
	}

	inline bool intersect(const Rect& rect, const Polygon& polygon)
	{
		return rectanglePolygonIntersect(
			rect.origin, 
			rect.origin + cocos2d::Vec2(rect.width, rect.height),
			polygon.vertices);
	}


	//----------------------------- Circle Intersect -------------------------------//

	inline bool intersect(const Circle& circle, const Rect& rect)
	{
		return intersect(rect, circle);
	}

	inline bool intersect(const Circle& circle1, const Circle& circle2)
	{
		return circlesIntersect(
			circle1.origin, circle1.radius,
			circle2.origin, circle2.radius);
	}

	inline bool intersect(const Circle& circle, const Segment& segment)
	{
		return segmentCircleIntersect(
			segment.start,
			segment.end,
			circle.origin,
			circle.radius);
	}

	inline bool intersect(const Circle& circle, const Polygon& polygon)
	{
		return circlePolygonIntersect(circle.origin, circle.radius, polygon.vertices);
	}


	//----------------------------- Segment Intersect -------------------------------//

	inline bool intersect(const Segment& segment, const Rect& rect)
	{
		return intersect(rect, segment);
	}

	inline bool intersect(const Segment& segment, const Circle& circle)
	{
		return intersect(circle, segment);
	}

	inline bool intersect(const Segment& segment1, const Segment& segment2)
	{
		return segmentsIntersect(segment1.start, segment1.end, segment2.start, segment2.end);
	}

	inline bool intersect(const Segment& segment, const Polygon& polygon)
	{
		return segmentPolygonIntersect(segment.start, segment.end, polygon.vertices);
	}


	//----------------------------- Polygon Intersect -------------------------------//

	inline bool intersect(const Polygon& polygon, const Rect& rect)
	{
		return intersect(rect, polygon);
	}

	inline bool intersect(const Polygon& polygon, const Circle& circle)
	{
		return intersect(circle, polygon);
	}

	inline bool intersect(const Polygon& polygon, const Segment& segment)
	{
		return intersect(segment, polygon);
	}

	inline bool intersect(const Polygon& polygon1, const Polygon& polygon2)
	{
		return polygonsIntersect(polygon1.vertices, polygon2.vertices);
	}


	//----------------------------- Intersect Points -------------------------------//

	inline std::vector<cocos2d::Vec2> intersectPoints(
		const Rect& rect1,
		const Rect& rect2)
	{
		return rectanglesIntersectPoints(
			rect1.origin, rect1.origin + cocos2d::Vec2(rect1.width, rect1.height),
			rect2.origin, rect2.origin + cocos2d::Vec2(rect2.width, rect2.height));
	}

	inline std::vector<cocos2d::Vec2> intersectPoints(
		const Rect& rect,
		const Circle& circle)
	{
		return rectangleCircleIntersectPoints(
			rect.origin, rect.origin + cocos2d::Vec2(rect.width, rect.height),
			circle.origin, circle.radius);
	}

	inline std::vector<cocos2d::Vec2> intersectPoints(
		const Rect& rect,
		const Segment& segment)
	{
		return rectangleSegmentIntersectPoints(
			rect.origin, rect.origin + cocos2d::Vec2(rect.width, rect.height),
			segment.start, segment.end);
	}

	inline std::vector<cocos2d::Vec2> intersectPoints(
		const Rect& rect,
		const Polygon& polygon)
	{
		return rectanglePolygonIntersectPoints(
			rect.origin, rect.origin + cocos2d::Vec2(rect.width, rect.height),
			polygon.vertices);
	}


	inline std::vector<cocos2d::Vec2> intersectPoints(
		const Circle& circle,
		const Rect& rect)
	{
		return intersectPoints(rect, circle);
	}

	inline std::vector<cocos2d::Vec2> intersectPoints(
		const Circle& circle1,
		const Circle& circle2)
	{
		return circlesIntersectPoints(
			circle1.origin, circle1.radius,
			circle2.origin, circle2.radius);
	}

	inline std::vector<cocos2d::Vec2> intersectPoints(
		const Circle& circle,
		const Segment& segment)
	{
		return segmentCircleIntersectPoints(
			segment.start, segment.end,
			circle.origin, circle.radius);
	}

	inline std::vector<cocos2d::Vec2> intersectPoints(
		const Circle& circle,
		const Polygon& polygon)
	{
		return circlePolygonIntersectPoints(
			circle.origin, circle.radius, polygon.vertices);
	}


	inline std::vector<cocos2d::Vec2> intersectPoints(
		const Segment& segment,
		const Rect& rect)
	{
		return intersectPoints(rect, segment);
	}

	inline std::vector<cocos2d::Vec2> intersectPoints(
		const Segment& segment,
		const Circle& circle)
	{
		return intersectPoints(circle, segment);
	}

	inline std::vector<cocos2d::Vec2> intersectPoints(
		const Segment& segment1,
		const Segment& segment2)
	{
		cocos2d::Vec2 temp;
		std::vector<cocos2d::Vec2> v;
		if (segmentsIntersect(
			segment1.start, segment1.end,
			segment2.start, segment2.end, temp))
		{
			v.emplace_back(temp);
		}
		return v;
	}

	inline std::vector<cocos2d::Vec2> intersectPoints(
		const Segment& segment,
		const Polygon& polygon)
	{
		return
			segmentPolygonIntersectPoints(segment.start, segment.end, polygon.vertices);
	}


	inline std::vector<cocos2d::Vec2> intersectPoints(
		const Polygon& polygon,
		const Rect& rect)
	{
		return intersectPoints(rect, polygon);
	}

	inline std::vector<cocos2d::Vec2> intersectPoints(
		const Polygon& polygon,
		const Circle& circle)
	{
		return intersectPoints(circle, polygon);
	}

	inline std::vector<cocos2d::Vec2> intersectPoints(
		const Polygon& polygon,
		const Segment& segment)
	{
		return intersectPoints(segment, polygon);
	}

	inline std::vector<cocos2d::Vec2> intersectPoints(
		const Polygon& polygon1,
		const Polygon& polygon2)
	{
		return polygonsIntersectPoints(polygon1.vertices, polygon2.vertices);
	}


	//----------------------------- Rect Overlap -------------------------------//

	inline bool overlap(const Rect& a, const Rect& b)
	{
		return rectanglesOverlap(
			cocos2d::Vec2(a.getMinX(), a.getMinY()),
			cocos2d::Vec2(a.getMaxX(), a.getMaxY()),
			cocos2d::Vec2(b.getMinX(), b.getMinY()),
			cocos2d::Vec2(b.getMaxX(), b.getMaxY()));
	}

	inline bool overlap(const Rect& rect, const Circle& circle)
	{
		return rectangleCircleOverlap(
			rect.origin,
			rect.origin + cocos2d::Vec2(rect.width, rect.height),
			circle.origin,
			circle.radius);
	}

	inline bool overlap(const Rect& rect, const Segment& segment)
	{
		return rectangleSegmentOverlap(
			rect.origin,
			rect.origin + cocos2d::Vec2(rect.width, rect.height),
			segment.start,
			segment.end);
	}

	inline bool overlap(const Rect& rect, const Polygon& polygon)
	{
		return rectanglePolygonOverlap(
			rect.origin,
			rect.origin + cocos2d::Vec2(rect.width, rect.height),
			polygon.vertices);
	}


	//------------------------------ Circle Overlap -------------------------------//

	inline bool overlap(const Circle& circle, const Rect& rect)
	{
		return overlap(rect, circle);
	}

	inline bool overlap(const Circle& circle1, const Circle& circle2)
	{
		return circlesOverlap(
			circle1.origin, circle1.radius,
			circle2.origin, circle2.radius);
	}

	inline bool overlap(const Circle& circle, const Segment& segment)
	{
		return segmentCircleOverlap(
			segment.start,
			segment.end,
			circle.origin,
			circle.radius);
	}

	inline bool overlap(const Circle& circle, const Polygon& polygon)
	{
		return circlePolygonOverlap(circle.origin, circle.radius, polygon.vertices);
	}


	//------------------------------ Segment Overlap -------------------------------//

	inline bool overlap(const Segment& segment, const Rect& rect)
	{
		return overlap(rect, segment);
	}

	inline bool overlap(const Segment& segment, const Circle& circle)
	{
		return overlap(circle, segment);
	}

	inline bool overlap(const Segment& segment1, const Segment& segment2)
	{
		return segmentsIntersect(segment1.start, segment1.end, segment2.start, segment2.end);
	}

	inline bool overlap(const Segment& segment, const Polygon& polygon)
	{
		return segmentPolygonOverlap(segment.start, segment.end, polygon.vertices);
	}



	//------------------------------ Polygon Overlap -------------------------------//

	inline bool overlap(const Polygon& polygon, const Rect& rect)
	{
		return overlap(rect, polygon);
	}

	inline bool overlap(const Polygon& polygon, const Circle& circle)
	{
		return overlap(circle, polygon);
	}

	inline bool overlap(const Polygon& polygon, const Segment& segment)
	{
		return overlap(segment, polygon);
	}

	inline bool overlap(const Polygon& polygon1, const Polygon& polygon2)
	{
		return polygonsOverlap(polygon1.vertices, polygon2.vertices);
	}


	//--------------------------- Rect enforceFrom -----------------------------//

	inline cocos2d::Vec2 enforceFrom(const Rect& rect1, const Rect& rect2)
	{
		//TODO
		return cocos2d::Vec2::ZERO;
	}

	inline cocos2d::Vec2 enforceFrom(const Rect& rect, const Circle& circle)
	{
		//TODO
		return cocos2d::Vec2::ZERO;
	}

	inline cocos2d::Vec2 enforceFrom(const Rect& rect, const Segment& segment)
	{
		//TODO
		return cocos2d::Vec2::ZERO;
	}

	inline cocos2d::Vec2 enforceFrom(const Rect& rect, const Polygon& polygon)
	{
		//TODO
		return cocos2d::Vec2::ZERO;
	}


	//--------------------------- Circle enforceFrom -----------------------------//

	inline cocos2d::Vec2 enforceFrom(const Circle& circle, const Rect& rect)
	{
		//TODO
		return cocos2d::Vec2::ZERO;
	}

	//------------------------------------------------------------------------
	//
	//  make force to move circle1 away from circle2
	//------------------------------------------------------------------------
	inline cocos2d::Vec2 enforceFrom(const Circle& circle1, const Circle& circle2)
	{
		cocos2d::Vec2 force;
		if (circlesEnforceAway(
			circle1.origin, circle1.radius,
			circle2.origin, circle2.radius,
			force))
			return force;
		return
			cocos2d::Vec2::ZERO;
	}

	inline cocos2d::Vec2 enforceFrom(const Circle& circle, const Segment& segment)
	{
		//TODO
		return cocos2d::Vec2::ZERO;
	}

	//------------------------------------------------------------------------
	//
	//  make force for move circle away from polygon
	//------------------------------------------------------------------------
	inline cocos2d::Vec2 enforceFrom(const Circle& circle, const Polygon& polygon)
	{
		cocos2d::Vec2 force;
		if (geometry::circlePolygonEnforceAway(
			circle.origin,
			circle.radius,
			polygon.vertices,
			force))
			return force;

		return cocos2d::Vec2::ZERO;
	}


	//--------------------------- Segment enforceFrom -----------------------------//

	inline cocos2d::Vec2 enforceFrom(const Segment& segment, const Rect& rect)
	{
		//TODO
		return cocos2d::Vec2::ZERO;
	}

	inline cocos2d::Vec2 enforceFrom(const Segment& segment, const Circle& circle)
	{
		//TODO
		return cocos2d::Vec2::ZERO;
	}

	inline cocos2d::Vec2 enforceFrom(const Segment& segment1, const Segment& segment2)
	{
		//TODO
		return cocos2d::Vec2::ZERO;
	}

	inline cocos2d::Vec2 enforceFrom(const Segment& segment, const Polygon& polygon)
	{
		//TODO
		return cocos2d::Vec2::ZERO;
	}


	//--------------------------- Polygon enforceFrom -----------------------------//

	inline cocos2d::Vec2 enforceFrom(const Polygon& polygon, const Rect& rect)
	{
		//TODO
		return cocos2d::Vec2::ZERO;
	}

	//------------------------------------------------------------------------
	//
	//  make force for move polygon away from circle
	//------------------------------------------------------------------------
	inline cocos2d::Vec2 enforceFrom(const Polygon& polygon, const Circle& circle)
	{
		cocos2d::Vec2 force;
		if (geometry::circlePolygonEnforceAway(
			circle.origin,
			circle.radius,
			polygon.vertices,
			force))
			return -force;
		else
			return  cocos2d::Vec2::ZERO;
	}

	inline cocos2d::Vec2 enforceFrom(const Polygon& polygon, const Segment& segment)
	{
		//TODO
		return cocos2d::Vec2::ZERO;
	}

	inline cocos2d::Vec2 enforceFrom(const Polygon& polygon1, const Polygon& polygon2)
	{
		//TODO
		return cocos2d::Vec2::ZERO;
	}


	//------------------------------- Globals -----------------------------------//

	inline bool isContainPointInDiamond(
		const cocos2d::Vec2& diamondCenter,
		float halfLen,
		const cocos2d::Vec2& p)
	{
		int m = 1;
		float b[4];

		cocos2d::Vec2 leftPoint = cocos2d::Vec2(diamondCenter.x - halfLen, diamondCenter.y);
		cocos2d::Vec2 rightPoint = cocos2d::Vec2(diamondCenter.x + halfLen, diamondCenter.y);
		cocos2d::Vec2 topPoint = cocos2d::Vec2(diamondCenter.x, diamondCenter.y + halfLen);
		cocos2d::Vec2 botPoint = cocos2d::Vec2(diamondCenter.x, diamondCenter.y - halfLen);

		b[0] = leftPoint.y - m * leftPoint.x;
		b[1] = topPoint.y + m * topPoint.x;
		b[2] = rightPoint.y - m * rightPoint.x;
		b[3] = botPoint.y + m * botPoint.x;

		if (p.y - (m * p.x) - b[0] < 0.f
			&& p.y - (-m * p.x) - b[1] < 0.f
			&& p.y - (m * p.x) - b[2] > 0.f
			&& p.y - (-m * p.x) - b[3] > 0.f)
		{
			return true;
		}

		return false;
	}


	inline float getAngleFromAxis(
		const cocos2d::Vec2& dir, 
		const cocos2d::Vec2& axis)
	{
		cocos2d::Vec2 a = axis.getNormalized();
		if (a.cross(dir) > 0.0f)
		{
			return MATH_RAD_TO_DEG(acosf(dir.dot(a)));
		}
		return MATH_RAD_TO_DEG(acosf(dir.dot(-a)) + 3.141592f);
	}

	inline bool isEqual(float a, float b)
	{
		if (fabs(a - b) < 1E-12)
		{
			return true;
		}

		return false;
	}

	inline bool isZero(float val)
	{
		return (-std::numeric_limits<float>::min() < val &&
			val < std::numeric_limits<float>::min());
	}

	inline cocos2d::Vec2 rotatePoint(float cx, float cy, float angle, cocos2d::Vec2 p)
	{
		return cocos2d::Vec2(cos(angle) * (p.x - cx) - sin(angle) * (p.y - cy) + cx,
			sin(angle) * (p.x - cx) + cos(angle) * (p.y - cy) + cy);
	}

	//------------------------------ccw-------------------------------
	//
	//	Given 3 points in 2D space, this returns if those points are
	//	in clock-wise or anti-clock-wise.
	//  return value > 0 : anti-clock-wise
	//  return value = 0 : parallel
	//  return value < 0 : clock-wise
	//----------------------------------------------------------------
	inline float ccw(float x1, float y1, float x2, float y2, float x3, float y3) 
	{
		return (x1 * y2 - y1 * x2) + (x2 * y3 - y2 * x3) + (x3 * y1 - y3 * x1);
	}

	inline float ccw(cocos2d::Vec2 p1, cocos2d::Vec2 p2, cocos2d::Vec2 p3)
	{
		return ccw(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y);
	}

	//----------------------- pointInCircle ----------------------------------
	//
	//  returns true if the point p is within the radius of the given circle
	//  if the point is on the boundary it returns false
	//------------------------------------------------------------------------
	inline bool pointInCircle(
		cocos2d::Vec2 pos,
		float    radius,
		cocos2d::Vec2 p)
	{
		CCASSERT(radius > 0, "invalid circle radius");

		float dist_from_center_squared = (p - pos).getLengthSq();
		if (dist_from_center_squared < (radius*radius))
		{
			return true;
		}
		return false;
	}

	//----------------------- isFacing -------------------------------
	//
	//  returns true if the agent is facing target.
	//------------------------------------------------------------------------
	inline bool isFacing(
		cocos2d::Vec2 pos,
		cocos2d::Vec2 heading,
		cocos2d::Vec2 target)
	{
		return (target - pos).dot(heading) >= 0;
	}

	//----------------------- perpendiculaFoot -------------------------------
	//
	//  returns perpendicula foot point of point P to line A, B
	//------------------------------------------------------------------------
	inline cocos2d::Vec2 perpendiculaFoot(
		cocos2d::Vec2 A,
		cocos2d::Vec2 B,
		cocos2d::Vec2 P)
	{
		return (P - A).project(B - A) + A;
	}

	//--------------------distLineAndPoint-------------------------
	//
	//	Given 1 lines in 2D space AB and a point P, this function 
	//	makes the distance between them.
	//-------------------------------------------------------------
	inline float distLineAndPoint(
		cocos2d::Vec2 A,
		cocos2d::Vec2 B,
		cocos2d::Vec2 P)
	{
		return abs((A - B).cross(B - P)) / (A - B).getLength();
	}

	//--------------------lineIntersect---------------------------
	//
	//	Given 2 lines in 2D space AB, CD this returns true if an 
	//	intersection occurs.
	//------------------------------------------------------------

	inline bool lineIntersect(
		cocos2d::Vec2 A,
		cocos2d::Vec2 B,
		cocos2d::Vec2 C,
		cocos2d::Vec2 D)
	{
		CCASSERT(A != B, "invalid line A B");
		CCASSERT(C != D, "invalid line C D");

		float det = (B - A).cross(D - C);
		if (fabs(det) < std::numeric_limits<float>::epsilon())
			return false;
		return true;
	}

	//--------------------lineIntersect-------------------------------
	//
	//	Given 2 lines in 2D space AB, CD this returns true if an 
	//	intersection occurs and sets P to the point the intersection
	//  occurs along line AB and CD
	//----------------------------------------------------------------
	inline bool lineIntersect(
		cocos2d::Vec2 A,
		cocos2d::Vec2 B,
		cocos2d::Vec2 C,
		cocos2d::Vec2 D,
		cocos2d::Vec2& P)
	{
		CCASSERT(A != B, "invalid line A B");
		CCASSERT(C != D, "invalid line C D");

		float det = (B - A).cross(D - C);
		if (fabs(det) < std::numeric_limits<float>::epsilon())
			return false;
		P = A + (B - A) * ((C - A).cross(D - C) / det);
		return true;
	}

	//------------------parallelSegmentsIntersect--------------------
	//
	//	Given 2 segments in 2D space AB, CD this returns true if two
	//  segments are parallel and an intersection occurs.
	//  returns one of point that included in intersecting area.
	//---------------------------------------------------------------
	inline bool parallelSegmentsIntersect(
		cocos2d::Vec2 A,
		cocos2d::Vec2 B,
		cocos2d::Vec2 C,
		cocos2d::Vec2 D,
		cocos2d::Vec2& P)
	{
		CCASSERT(A != B, "invalid segment A B");
		CCASSERT(C != D, "invalid segment C D");

		if (B < A)std::swap(A, B);
		if (D < C)std::swap(C, D);

		//returns false if they are not parallel or parallel but not intersects.
		if (ccw(A, B, C) != 0 || B < C || D < A)
			return false;

		if (A < C)
			P = C;
		else
			P = A;

		return true;
	}


	//--------------------lineCircleIntersectPoints--------------------
	//
	//	Checks if line and circle intersects.
	//  And returns the intersecting points with vector.
	//-----------------------------------------------------------------
	inline std::vector<cocos2d::Vec2> lineCircleIntersectPoints(
		cocos2d::Vec2 A,
		cocos2d::Vec2 B,
		cocos2d::Vec2 pos,
		float R)
	{
		cocos2d::Vec2 to_b_norm = (B - A).getNormalized();
		std::vector<cocos2d::Vec2> intersect_points;

		//move the circle into the local space defined by the vector B-A with origin at A.
		cocos2d::Vec2 local =
			PointToLocalSpace(pos, to_b_norm, to_b_norm.getPerp(), A);

		if (fabs(local.y) == R)
		{
			intersect_points.emplace_back(
				PointToWorldSpace(
					cocos2d::Vec2(local.x, 0),
					to_b_norm,
					to_b_norm.getPerp(),
					A));
		}
		else if (fabs(local.y) < R)
		{
			intersect_points.emplace_back(
				PointToWorldSpace(
					cocos2d::Vec2(local.x + sqrt(R * R - local.y * local.y), 0),
					to_b_norm,
					to_b_norm.getPerp(),
					A));
			intersect_points.emplace_back(
				PointToWorldSpace(
					cocos2d::Vec2(local.x - sqrt(R * R - local.y * local.y), 0),
					to_b_norm,
					to_b_norm.getPerp(),
					A));
		}

		return intersect_points;
	}


	//--------------------pointInRectangle---------------------------
	//
	//	Checks if the point P is in rectangle which is bounded by
	//  A to B. It doesn't care about the ordering.
	//  It also checks about lines.
	//  It returns true about boundary.
	//--------------------------------------------------------------- 
	inline bool pointInRectangle(
		cocos2d::Vec2 P,
		cocos2d::Vec2 A,
		cocos2d::Vec2 B)
	{
		return std::min(A.x, B.x) <= P.x && P.x <= std::max(A.x, B.x) &&
			std::min(A.y, B.y) <= P.y && P.y <= std::max(A.y, B.y);
	}

	//---------------------rectanglesIntersect----------------------
	//
	//	Given 2 rectangles in 2D space
	//  bottom_left1, top_right1 = rectangle1 and
	//  bottom_left2, top_right2 = rectangle2
	//  this returns true if an intersection occurs.
	//--------------------------------------------------------------
	inline bool rectanglesIntersect(
		cocos2d::Vec2 bottom_left1,
		cocos2d::Vec2 top_right1,
		cocos2d::Vec2 bottom_left2,
		cocos2d::Vec2 top_right2)
	{
		CCASSERT(bottom_left1.x < top_right1.x && bottom_left1.y < top_right1.y,
			"invalid rectangle1");
		CCASSERT(bottom_left2.x < top_right2.x && bottom_left2.y < top_right2.y,
			"invalid rectangle2");

		return !((top_right2.y < bottom_left1.y) ||
				(bottom_left2.y > top_right1.y) ||
				(bottom_left2.x > top_right1.x) ||
				(top_right2.x < bottom_left1.x));
	}

	//------------------rectanglesIntersectPoints-------------------
	//
	//	Given 2 rectangles in 2D space
	//  bottom_left1, top_right1 = rectangle1 and
	//  bottom_left2, top_right2 = rectangle2
	//  this returns the vector of intersection points.
	//--------------------------------------------------------------
	inline std::vector<cocos2d::Vec2> rectanglesIntersectPoints(
		cocos2d::Vec2 bottom_left1,
		cocos2d::Vec2 top_right1,
		cocos2d::Vec2 bottom_left2,
		cocos2d::Vec2 top_right2)
	{
		CCASSERT(bottom_left1.x < top_right1.x && bottom_left1.y < top_right1.y,
			"invalid rectangle1");
		CCASSERT(bottom_left2.x < top_right2.x && bottom_left2.y < top_right2.y,
			"invalid rectangle2");

		float minX = bottom_left2.x;
		float minY = bottom_left2.y;
		float maxX = top_right2.x;
		float maxY = top_right2.y;

		auto points1 = rectangleSegmentIntersectPoints(
			bottom_left1, top_right1,
			cocos2d::Vec2(minX, minY), cocos2d::Vec2(maxX, minY));

		auto points2 = rectangleSegmentIntersectPoints(
			bottom_left1, top_right1,
			cocos2d::Vec2(maxX, minY), cocos2d::Vec2(maxX, maxY));

		auto points3 = rectangleSegmentIntersectPoints(
			bottom_left1, top_right1,
			cocos2d::Vec2(maxX, maxY), cocos2d::Vec2(minX, maxY));

		auto points4 = rectangleSegmentIntersectPoints(
			bottom_left1, top_right1,
			cocos2d::Vec2(minX, maxY), cocos2d::Vec2(minX, minY));
		
		std::vector<cocos2d::Vec2> points;
		for (auto e : points1)
			points.emplace_back(e);
		for (auto e : points2)
			points.emplace_back(e);
		for (auto e : points3)
			points.emplace_back(e);
		for (auto e : points4)
			points.emplace_back(e);

		return points;
	}

	//---------------------rectanglesOverlap----------------------
	//
	//	Given 2 rectangles in 2D space
	//  bottom_left1, top_right1 = rectangle1 and
	//  bottom_left2, top_right2 = rectangle2
	//  this returns true if an overlap occurs.
	//--------------------------------------------------------------
	inline bool rectanglesOverlap(
		cocos2d::Vec2 bottom_left1,
		cocos2d::Vec2 top_right1,
		cocos2d::Vec2 bottom_left2,
		cocos2d::Vec2 top_right2)
	{
		CCASSERT(bottom_left1.x < top_right1.x && bottom_left1.y < top_right1.y,
			"invalid rectangle1");
		CCASSERT(bottom_left2.x < top_right2.x && bottom_left2.y < top_right2.y,
			"invalid rectangle2");

		return rectanglesIntersect(bottom_left1, top_right1, bottom_left2, top_right2) ||
			rectangleInRectangle(bottom_left1, top_right1, bottom_left2, top_right2) ||
			rectangleInRectangle(bottom_left2, top_right2, bottom_left1, top_right1);
	}

	//---------------------rectangleInRectangle----------------------
	//
	//	Given 2 rectangles in 2D space
	//  bottom_left1, top_right1 = rectangle1 and
	//  bottom_left2, top_right2 = rectangle2
	//  this returns true if a rectangle2 include rectangle1.
	//--------------------------------------------------------------
	inline bool rectangleInRectangle(
		cocos2d::Vec2 bottom_left1,
		cocos2d::Vec2 top_right1,
		cocos2d::Vec2 bottom_left2,
		cocos2d::Vec2 top_right2)
	{
		CCASSERT(bottom_left1.x < top_right1.x && bottom_left1.y < top_right1.y,
			"invalid rectangle1");
		CCASSERT(bottom_left2.x < top_right2.x && bottom_left2.y < top_right2.y,
			"invalid rectangle2");

		return 
			bottom_left2.x < bottom_left1.x &&
			top_right1.x < top_right2.x &&
			bottom_left2.y < bottom_left1.y &&
			top_right1.y < top_right2.y;
	}


	//--------------------rectangleCircleIntersect---------------------
	//
	//	Given a rectangle and a circle in 2D space
	//  this returns true if an intersection occurs.
	//-----------------------------------------------------------------
	inline bool rectangleCircleIntersect(
		cocos2d::Vec2 bottom_left,
		cocos2d::Vec2 top_right,
		cocos2d::Vec2 p,
		float r)
	{
		CCASSERT(bottom_left.x < top_right.x && bottom_left.y < top_right.y,
			"invalid rectangle");
		CCASSERT(r > 0, "invalid circle radius");

		float minX = bottom_left.x;
		float minY = bottom_left.y;
		float maxX = top_right.x;
		float maxY = top_right.y;

		return
		segmentCircleIntersect(cocos2d::Vec2(minX, minY), cocos2d::Vec2(maxX, minY), p, r) ||
		segmentCircleIntersect(cocos2d::Vec2(maxX, minY), cocos2d::Vec2(maxX, maxY), p, r) ||
		segmentCircleIntersect(cocos2d::Vec2(maxX, maxY), cocos2d::Vec2(minX, maxY), p, r) ||
		segmentCircleIntersect(cocos2d::Vec2(minX, maxY), cocos2d::Vec2(minX, minY), p, r);
	}

	//-----------------rectangleCircleIntersectPoints------------------
	//
	//	Given a rectangle and a circle in 2D space
	//  this returns intersecting points with container.
	//-----------------------------------------------------------------
	inline std::vector<cocos2d::Vec2> rectangleCircleIntersectPoints(
		cocos2d::Vec2 bottom_left,
		cocos2d::Vec2 top_right,
		cocos2d::Vec2 pos,
		float radius)
	{
		float minX = bottom_left.x;
		float minY = bottom_left.y;
		float maxX = top_right.x;
		float maxY = top_right.y;

		std::vector<cocos2d::Vec2> points1 = segmentCircleIntersectPoints(
			cocos2d::Vec2(minX, minY), cocos2d::Vec2(maxX, minY), pos, radius);

		std::vector<cocos2d::Vec2> points2 = segmentCircleIntersectPoints(
			cocos2d::Vec2(maxX, minY), cocos2d::Vec2(maxX, maxY), pos, radius);

		std::vector<cocos2d::Vec2> points3 = segmentCircleIntersectPoints(
			cocos2d::Vec2(maxX, maxY), cocos2d::Vec2(minX, maxY), pos, radius);

		std::vector<cocos2d::Vec2> points4 = segmentCircleIntersectPoints(
			cocos2d::Vec2(minX, maxY), cocos2d::Vec2(minX, minY), pos, radius);

		std::vector<cocos2d::Vec2> points;
		for (auto e : points1)
			points.emplace_back(e);
		for (auto e : points2)
			points.emplace_back(e);
		for (auto e : points3)
			points.emplace_back(e);
		for (auto e : points4)
			points.emplace_back(e);

		return points;
	}

	//--------------------rectangleInCircle---------------------
	//
	//	Given a rectangle and a circle in 2D space
	//  this returns true if circle includes rectangle.
	//	it returns false about boundary
	//-----------------------------------------------------------------
	inline bool rectangleInCircle(
		cocos2d::Vec2 bottom_left,
		cocos2d::Vec2 top_right,
		cocos2d::Vec2 pos,
		float radius)
	{
		CCASSERT(bottom_left.x < top_right.x && bottom_left.y < top_right.y,
			"invalid rectangle");
		CCASSERT(radius > 0, "invalid circle radius");

		return
			pointInCircle(pos, radius, bottom_left) &&
			pointInCircle(pos, radius, cocos2d::Vec2(bottom_left.x, top_right.y)) &&
			pointInCircle(pos, radius, cocos2d::Vec2(top_right.x, bottom_left.y)) &&
			pointInCircle(pos, radius, top_right);
	}

	//--------------------circleInRectangle---------------------
	//
	//	Given a rectangle and a circle in 2D space
	//  this returns true if rectangle includes circle.
	//	it returns false about boundary
	//-----------------------------------------------------------------
	inline bool circleInRectangle(
		cocos2d::Vec2 bottom_left,
		cocos2d::Vec2 top_right,
		cocos2d::Vec2 pos,
		float radius)
	{
		CCASSERT(bottom_left.x < top_right.x && bottom_left.y < top_right.y,
			"invalid rectangle");
		CCASSERT(radius > 0, "invalid circle radius");

		return pointInRectangle(pos, bottom_left, top_right) &&
			fabs(bottom_left.x - pos.x) > radius &&
			fabs(top_right.x - pos.x) > radius &&
			fabs(bottom_left.y - pos.y) > radius &&
			fabs(top_right.y - pos.y) > radius;
	}

	//--------------------rectangleCircleOverlap---------------------
	//
	//	Given a rectangle and a circle in 2D space
	//  this returns true if an overlap occurs.
	//  if the rectangle located inside of circle or
	//  if the circle located inside of rectangle it also returns true
	//-----------------------------------------------------------------
	inline bool rectangleCircleOverlap(
		cocos2d::Vec2 bottom_left,
		cocos2d::Vec2 top_right,
		cocos2d::Vec2 pos,
		float radius)
	{
		CCASSERT(bottom_left.x < top_right.x && bottom_left.y < top_right.y,
			"invalid rectangle");
		CCASSERT(radius > 0, "invalid circle radius");

		return
			segmentCircleOverlap(
				cocos2d::Vec2(bottom_left.x, bottom_left.y),
				cocos2d::Vec2(top_right.x, bottom_left.y),
			pos, radius) ||
			segmentCircleOverlap(
				cocos2d::Vec2(top_right.x, bottom_left.y),
				cocos2d::Vec2(top_right.x, top_right.y),
				pos, radius) ||
			segmentCircleOverlap(
				cocos2d::Vec2(top_right.x, top_right.y),
				cocos2d::Vec2(bottom_left.x, top_right.y),
				pos, radius) ||
			segmentCircleOverlap(
				cocos2d::Vec2(bottom_left.x, top_right.y),
				cocos2d::Vec2(bottom_left.x, bottom_left.y),
				pos, radius) ||
			circleInRectangle(bottom_left, top_right, pos, radius);
	}

	//--------------------rectangleSegmentIntersect---------------------
	//
	//	Given 2 rectangles in 2D space
	//  bottom_left1, top_right1 = rectangle1 and
	//  bottom_left2, top_right2 = rectangle2
	//  this returns true if an intersection occurs.
	//------------------------------------------------------------------
	inline bool rectangleSegmentIntersect(
		cocos2d::Vec2 bottom_left,
		cocos2d::Vec2 top_right,
		cocos2d::Vec2 A,
		cocos2d::Vec2 B)
	{
		CCASSERT(bottom_left.x < top_right.x && bottom_left.y < top_right.y,
			"invalid rectangle");
		CCASSERT(A != B, "invalid segment A B");

		float minX = bottom_left.x;
		float minY = bottom_left.y;
		float maxX = top_right.x;
		float maxY = top_right.y;

		return 
			segmentsIntersect(A, B, cocos2d::Vec2(minX, minY), cocos2d::Vec2(maxX, minY)) ||
			segmentsIntersect(A, B, cocos2d::Vec2(maxX, minY), cocos2d::Vec2(maxX, maxY)) ||
			segmentsIntersect(A, B, cocos2d::Vec2(maxX, maxY), cocos2d::Vec2(minX, maxY)) ||
			segmentsIntersect(A, B, cocos2d::Vec2(minX, maxY), cocos2d::Vec2(minX, minY));
	}


	//-------------------rectangleSegmentIntersectPoints-------------------
	//
	//	Given two rectangles in 2D space
	//  this returns the intersecting point with container.
	//--------------------------------------------------------------------
	inline std::vector<cocos2d::Vec2> rectangleSegmentIntersectPoints(
		cocos2d::Vec2 bottom_left,
		cocos2d::Vec2 top_right,
		cocos2d::Vec2 A,
		cocos2d::Vec2 B)
	{
		float minX = bottom_left.x;
		float minY = bottom_left.y;
		float maxX = top_right.x;
		float maxY = top_right.y;

		std::vector<cocos2d::Vec2> points;
		cocos2d::Vec2 temp;

		if (segmentsIntersect(
			A, B, cocos2d::Vec2(minX, minY), cocos2d::Vec2(maxX, minY), temp))
			points.emplace_back(temp);

		if (segmentsIntersect(
			A, B, cocos2d::Vec2(maxX, minY), cocos2d::Vec2(maxX, maxY), temp))
			points.emplace_back(temp);

		if (segmentsIntersect(
			A, B, cocos2d::Vec2(maxX, maxY), cocos2d::Vec2(minX, maxY), temp))
			points.emplace_back(temp);

		if (segmentsIntersect(
			A, B, cocos2d::Vec2(minX, maxY), cocos2d::Vec2(minX, minY), temp))
			points.emplace_back(temp);

		return points;
	}


	//--------------------rectangleSegmentOverlap---------------------
	//
	//  this returns true if 
	//  segment intersects to rectangle or
	//  locates inside of rectangle.
	//------------------------------------------------------------------
	inline bool rectangleSegmentOverlap(
		cocos2d::Vec2 bottom_left,
		cocos2d::Vec2 top_right,
		cocos2d::Vec2 A,
		cocos2d::Vec2 B)
	{
		CCASSERT(bottom_left.x < top_right.x && bottom_left.y < top_right.y,
			"invalid rectangle");
		CCASSERT(A != B, "invalid segment A B");

		return rectangleSegmentIntersect(bottom_left, top_right, A, B) ||
			pointInRectangle(A, bottom_left, top_right);
	}


	//--------------------rectanglePolygonIntersect---------------------
	//
	//	Given a rectangle and a polygon in 2D space, this function
	//  checks if those are intersecting.
	//------------------------------------------------------------------
	inline bool rectanglePolygonIntersect(
		cocos2d::Vec2 bottom_left,
		cocos2d::Vec2 top_right,
		const std::vector<cocos2d::Vec2>& poly)
	{
		CCASSERT(bottom_left.x < top_right.x && bottom_left.y < top_right.y,
			"invalid rectangle");
		CCASSERT(poly.size() > 2, "poly is not a polygon");

		float minX = bottom_left.x;
		float minY = bottom_left.y;
		float maxX = top_right.x;
		float maxY = top_right.y;

		return
			(segmentPolygonIntersect(
				cocos2d::Vec2(minX, minY),
				cocos2d::Vec2(maxX, minY),
				poly) ||
				segmentPolygonIntersect(
					cocos2d::Vec2(maxX, minY),
					cocos2d::Vec2(maxX, maxY),
					poly) ||
				segmentPolygonIntersect(
					cocos2d::Vec2(maxX, maxY),
					cocos2d::Vec2(minX, maxY),
					poly) ||
				segmentPolygonIntersect(
					cocos2d::Vec2(minX, maxY),
					cocos2d::Vec2(minX, minY),
					poly));
	}

	//------------------ rectanglePolygonIntersectPoints ---------------------
	//
	//  returns the intersecting points between rectangle and polygon.
	//----------------------------------------------------------------------
	inline std::vector<cocos2d::Vec2> rectanglePolygonIntersectPoints(
		cocos2d::Vec2 bottom_left,
		cocos2d::Vec2 top_right,
		const std::vector<cocos2d::Vec2>& poly)
	{
		float minX = bottom_left.x;
		float minY = bottom_left.y;
		float maxX = top_right.x;
		float maxY = top_right.y;

		auto points1 = segmentPolygonIntersectPoints(
			cocos2d::Vec2(minX, minY),
			cocos2d::Vec2(maxX, minY),
			poly);
		auto points2 = segmentPolygonIntersectPoints(
			cocos2d::Vec2(maxX, minY),
			cocos2d::Vec2(maxX, maxY),
			poly);
		auto points3 = segmentPolygonIntersectPoints(
			cocos2d::Vec2(maxX, maxY),
			cocos2d::Vec2(minX, maxY),
			poly);
		auto points4 = segmentPolygonIntersectPoints(
			cocos2d::Vec2(minX, maxY),
			cocos2d::Vec2(minX, minY),
			poly);

		std::vector<cocos2d::Vec2> points;

		for (auto e : points1)
			points.emplace_back(e);
		for (auto e : points2)
			points.emplace_back(e);
		for (auto e : points3)
			points.emplace_back(e);
		for (auto e : points4)
			points.emplace_back(e);

		return points;
	}

	//--------------------- rectanglePolygonOverlap -------------------
	//
	//  returns true if the rectangle overlap to polygon.
	//  overlap means rectangle can be located inside to polygon or
	//  polygon can be located inside to rectangle.
	//  it includes the collision of boundary.
	//-----------------------------------------------------------------
	inline bool rectanglePolygonOverlap(
		cocos2d::Vec2 bottom_left,
		cocos2d::Vec2 top_right,
		const std::vector<cocos2d::Vec2>& poly)
	{
		CCASSERT(bottom_left.x < top_right.x && bottom_left.y < top_right.y,
			"invalid rectangle");
		CCASSERT(poly.size() > 2, "poly is not a polygon");

		if (rectanglePolygonIntersect(bottom_left, top_right, poly))
			return true;

		if (pointInRectangle(poly[0], bottom_left, top_right))
			return true;

		if (pointInPolygon(bottom_left, poly))
			return true;

		return false;
	}

	//--------------------segmentIntersect-------------------------
	//
	//	Given 2 segments in 2D space AB, CD this returns true if an 
	//	intersection occurs.
	//------------------------------------------------------------- 
	inline bool segmentsIntersect(
		cocos2d::Vec2 A,
		cocos2d::Vec2 B,
		cocos2d::Vec2 C,
		cocos2d::Vec2 D)
	{
		CCASSERT(A != B, "invalid segment A B");
		CCASSERT(C != D, "invalid segment C D");

		float ab = ccw(A, B, C) * ccw(A, B, D);
		float cd = ccw(C, D, A) * ccw(C, D, B);

		if (ab == 0 && cd == 0)
		{
			if (B < A)std::swap(A, B);
			if (D < C)std::swap(C, D);
			return !(B < C || D < A);
		}
		return ab <= 0 && cd <= 0;
	}

	//--------------------segmentIntersect-------------------------
	//
	//	Given 2 segments in 2D space AB, CD this returns true if an 
	//	intersection occurs and sets P to the point the intersection
	//  occurs along segment AB and CD
	//------------------------------------------------------------- 
	inline bool segmentsIntersect(
		cocos2d::Vec2 A,
		cocos2d::Vec2 B,
		cocos2d::Vec2 C,
		cocos2d::Vec2 D,
		cocos2d::Vec2& P)
	{
		CCASSERT(A != B, "invalid segment A B");
		CCASSERT(C != D, "invalid segment C D");

		//process the paralleling case first
		if (!lineIntersect(A, B, C, D, P))
			return parallelSegmentsIntersect(A, B, C, D, P);

		//if the segment includes point P then return true
		return pointInRectangle(P, A, B) && pointInRectangle(P, C, D);
	}

	//-------------------- segmentsIntersect-------------------------
	//
	//	Given 2 lines in 2D space AB, CD this returns true if an 
	//	intersection occurs and sets dist to the distance the intersection
	//  occurs along AB. Also sets the 2d vector point to the point of
	//  intersection
	//-----------------------------------------------------------------
	inline bool segmentsIntersect(
		cocos2d::Vec2 A,
		cocos2d::Vec2 B,
		cocos2d::Vec2 C,
		cocos2d::Vec2 D,
		float& dist,
		cocos2d::Vec2& P)
	{
		CCASSERT(A != B, "invalid segment A B");
		CCASSERT(C != D, "invalid segment C D");

		dist = 0;
		//process the paralleling case first
		if (!lineIntersect(A, B, C, D, P))
			return parallelSegmentsIntersect(A, B, C, D, P);

		dist = (A - P).getLengthSq();

		//if the segment includes point P then return true
		return pointInRectangle(P, A, B) && pointInRectangle(P, C, D);
	}

	//--------------------------- distToSegment ------------------------------
	//
	//  given a line segment AB and a point P, this function calculates the 
	//  perpendicular distance between them
	//------------------------------------------------------------------------
	inline float distToSegment(
		cocos2d::Vec2 A,
		cocos2d::Vec2 B,
		cocos2d::Vec2 P)
	{
		CCASSERT(A != B, "invalid segment");

		//if the angle is obtuse between PA and AB is obtuse then the closest
		//vertex must be A
		float dotA = (P.x - A.x)*(B.x - A.x) + (P.y - A.y)*(B.y - A.y);

		if (dotA <= 0) return (P - A).getLength();

		//if the angle is obtuse between PB and AB is obtuse then the closest
		//vertex must be B
		float dotB = (P.x - B.x)*(A.x - B.x) + (P.y - B.y)*(A.y - B.y);

		if (dotB <= 0) return (P - B).getLength();

		//calculate the point along AB that is the closest to P
		cocos2d::Vec2 Point = A + ((B - A) * dotA) / (dotA + dotB);

		//calculate the distance P-Point
		return (P - Point).getLength();
	}

	//--------------------------- distToSegmentSq ----------------------------
	//
	//  as above, but avoiding sqrt
	//------------------------------------------------------------------------
	inline float distToSegmentSq(
		cocos2d::Vec2 A,
		cocos2d::Vec2 B,
		cocos2d::Vec2 P)
	{
		CCASSERT(A != B, "invalid segment");

		//if the angle is obtuse between PA and AB is obtuse then the closest
		//vertex must be A
		float dotA = (P.x - A.x)*(B.x - A.x) + (P.y - A.y)*(B.y - A.y);

		if (dotA <= 0) return (P - A).getLengthSq();

		//if the angle is obtuse between PB and AB is obtuse then the closest
		//vertex must be B
		float dotB = (P.x - B.x)*(A.x - B.x) + (P.y - B.y)*(A.y - B.y);

		if (dotB <= 0) return (P - B).getLengthSq();

		//calculate the point along AB that is the closest to P
		cocos2d::Vec2 Point = A + ((B - A) * dotA) / (dotA + dotB);

		//calculate the distance P-Point
		return (Point - P).getLengthSq();
	}

	inline bool pointOnSegment(
		const cocos2d::Vec2 P,
		const cocos2d::Vec2 A,
		const cocos2d::Vec2 B)
	{
		CCASSERT(A != B, "invalid segment");

		return 
			((P.x == A.x) && (P.y == A.y)) ||
			((P.x == B.x) && (P.y == B.y)) ||
			(((P.x > A.x) == (P.x < B.x)) &&
			((P.y > A.y) == (P.y < B.y)) &&
			((P.x - A.x) * (B.y - A.y) == (B.x - A.x) * (P.y - A.y)));
	}

	//--------------------- segmentCircleIntersect ------------------------
	//
	//  returns true if the line segemnt AB intersects with a circle at
	//  position P with radius radius
	//  note that this functions includes boundarys too.
	//------------------------------------------------------------------------
	inline bool segmentCircleIntersect(
		cocos2d::Vec2 A,
		cocos2d::Vec2 B,
		cocos2d::Vec2 P,
		float radius)
	{
		CCASSERT(A != B, "invalid segment");
		CCASSERT(radius > 0, "invalid circle radius");

		return segmentCircleOverlap(A, B, P, radius) &&
			!(pointInCircle(P, radius, A) && pointInCircle(P, radius, B));
	}

	//------------------- segmentCircleIntersectPoints ---------------------
	//
	//  returns the intersecting point of line segement AB with a circle at
	//  position P with radius.
	//----------------------------------------------------------------------
	inline std::vector<cocos2d::Vec2> segmentCircleIntersectPoints(
		cocos2d::Vec2 A,
		cocos2d::Vec2 B,
		cocos2d::Vec2 P,
		float R)
	{
		std::vector<cocos2d::Vec2> points =
			geometry::lineCircleIntersectPoints(A, B, P, R);

		std::vector<cocos2d::Vec2> ret;

		for (auto e : points)
			if (geometry::pointInRectangle(e, A, B))
				ret.push_back(e);
		return ret;
	}

	//----------------------- segmentCircleOverlap ---------------------------
	//
	//  returns true if the line segemnt AB intersects or included in a circle 
	//  at position P with radius radius
	//  note that this functions includes boundarys too.
	//------------------------------------------------------------------------
	inline bool segmentCircleOverlap(
		cocos2d::Vec2 A,
		cocos2d::Vec2 B,
		cocos2d::Vec2 P,
		float radius)
	{
		CCASSERT(A != B, "invalid segment");
		CCASSERT(radius > 0, "invalid circle radius");

		//first determine the distance from the center of the circle to
		//the line segment (working in distance squared space)
		float dist_to_line_sq = distToSegmentSq(A, B, P);

		if (dist_to_line_sq <= radius*radius)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	//--------------------- segmentPolygonIntersect ------------------------
	//
	//  returns true if the given polygon and line AB are intersecting.
	//----------------------------------------------------------------------
	inline bool segmentPolygonIntersect(
		cocos2d::Vec2 A,
		cocos2d::Vec2 B,
		const std::vector<cocos2d::Vec2>& poly)
	{
		CCASSERT(A != B, "invalid segment");
		CCASSERT(poly.size() > 2, "poly is not a polygon");

		for (size_t i = 0; i < poly.size(); ++i)
		{
			size_t j = (i + 1) % poly.size();
			if (segmentsIntersect(A, B, poly[i], poly[j]))
				return true;
		}
		return false;
	}

	//------------------ segmentPolygonIntersectPoints ---------------------
	//
	//  returns the intersecting points between polygon and line AB.
	//----------------------------------------------------------------------
	inline std::vector<cocos2d::Vec2> segmentPolygonIntersectPoints(
		cocos2d::Vec2 A,
		cocos2d::Vec2 B,
		const std::vector<cocos2d::Vec2>& poly)
	{
		CCASSERT(A != B, "invalid segment");
		CCASSERT(poly.size() > 2, "poly is not a polygon");

		std::vector<cocos2d::Vec2> points;

		for (size_t i = 0; i < poly.size(); ++i)
		{
			size_t j = (i + 1) % poly.size();
			cocos2d::Vec2 temp;

			if (segmentsIntersect(A, B, poly[i], poly[j], temp))
			{
				points.emplace_back(temp);
			}
		}

		return points;
	}


	//---------------------- segmentPolygonOverlap -------------------------
	//
	//  returns true if the given polygon and line AB are intersecting.
	//  it also returns true if one is located inside to other one.
	//----------------------------------------------------------------------
	inline bool segmentPolygonOverlap(
		cocos2d::Vec2 A,
		cocos2d::Vec2 B,
		const std::vector<cocos2d::Vec2>& poly)
	{
		CCASSERT(A != B, "invalid segment");
		CCASSERT(poly.size() > 2, "poly is not a polygon");
		return pointInPolygon(A, poly) || segmentPolygonIntersect(A, B, poly);
	}


	//--------------------- circlesIntersect ----------------------
	//
	//  returns true if the circle intersect to other.
	//  if one circle is inside of other circle, it returns false.
	//-------------------------------------------------------------
	inline bool circlesIntersect(
		cocos2d::Vec2 c1, float r1,
		cocos2d::Vec2 c2, float r2)
	{
		CCASSERT(r1 > 0, "invalid circle radius r1");
		CCASSERT(r2 > 0, "invalid circle radius r2");

		float dist_between_centers_sq = ((c1.x - c2.x) * (c1.x - c2.x) +
			(c1.y - c2.y) * (c1.y - c2.y));

		if ((r1 - r2)*(r1 - r2) <= dist_between_centers_sq &&
			dist_between_centers_sq <= (r1 + r2)*(r1 + r2))
		{
			return true;
		}
		return false;
	}

	// Find the points where the two circles intersect.
	inline int circlesIntersectPoints(
		float cx0, float cy0, float radius0,
		float cx1, float cy1, float radius1,
		cocos2d::Vec2& intersection1, cocos2d::Vec2& intersection2)
	{
		// Find the distance between the centers.
		float dx = cx0 - cx1;
		float dy = cy0 - cy1;
		float dist = sqrt(dx * dx + dy * dy);

		// See how many solutions there are.
		if (dist > radius0 + radius1)
		{
			// No solutions, the circles are too far apart.
			intersection1 = cocos2d::Vec2(NAN, NAN);
			intersection2 = cocos2d::Vec2(NAN, NAN);
			return 0;
		}
		else if (dist < abs(radius0 - radius1))
		{
			// No solutions, one circle contains the other.
			intersection1 = cocos2d::Vec2(NAN, NAN);
			intersection2 = cocos2d::Vec2(NAN, NAN);
			return 0;
		}
		else if ((dist == 0) && (radius0 == radius1))
		{
			// No solutions, the circles coincide.
			intersection1 = cocos2d::Vec2(NAN, NAN);
			intersection2 = cocos2d::Vec2(NAN, NAN);
			return 0;
		}
		else
		{
			// Find a and h.
			float a = (radius0 * radius0 -
				radius1 * radius1 + dist * dist) / (2 * dist);
			float h = sqrt(radius0 * radius0 - a * a);

			// Find P2.
			float cx2 = cx0 + a * (cx1 - cx0) / dist;
			float cy2 = cy0 + a * (cy1 - cy0) / dist;

			// Get the points P3.
			intersection1 = cocos2d::Vec2(
				(cx2 + h * (cy1 - cy0) / dist),
				(cy2 - h * (cx1 - cx0) / dist));

			intersection2 = cocos2d::Vec2(
				(cx2 - h * (cy1 - cy0) / dist),
				(cy2 + h * (cx1 - cx0) / dist));

			// See if we have 1 or 2 solutions.
			if (dist == radius0 + radius1) return 1;
			return 2;
		}
	}

	//------------------------ circlesIntersectPoints ---------------------
	//
	//  Given two circles this function calculates the intersection points
	//  of any overlap. Returns the vector of intersections points.
	//  It returns no point when they are same.
	//---------------------------------------------------------------------
	inline std::vector<cocos2d::Vec2> circlesIntersectPoints(
		cocos2d::Vec2 p1, float r1,
		cocos2d::Vec2 p2, float r2)
	{
		cocos2d::Vec2 t1, t2;
		int result = circlesIntersectPoints(p1.x, p1.y, r1, p2.x, p2.y, r2, t1, t2);
		std::vector<cocos2d::Vec2> points;

		if (result == 0)
		{
			return points;
		}
		else if (result == 1)
		{
			points.push_back(t1);
			return points;
		}
		else
		{
			points.push_back(t1);
			points.push_back(t2);
			return points;
		}
	}

	//------------------------ circlesIntersect ------------------
	//
	//  Given two circles this function calculates the intersection points
	//  of any overlap.
	//
	//  returns false if no overlap found
	//
	// see http://astronomy.swin.edu.au/~pbourke/geometry/2circle/
	//------------------------------------------------------------------------ 
	inline bool circlesIntersect(
		float x1, float y1, float r1,
		float x2, float y2, float r2,
		float &p3X, float &p3Y,
		float &p4X, float &p4Y)
	{
		CCASSERT(!(x1 == x2 && y1 == y2 && r1 == r2), "two circles are same!");
		//first check to see if they overlap
		if (!circlesOverlap(cocos2d::Vec2(x1, y1), r1, cocos2d::Vec2(x2, y2), r2))
		{
			return false;
		}

		//calculate the distance between the circle centers
		float d = sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));

		//Now calculate the distance from the center of each circle to the center
		//of the line which connects the intersection points.
		float a = (r1 - r2 + (d * d)) / (2 * d);
		float b = (r2 - r1 + (d * d)) / (2 * d);

		//calculate the point P2 which is the center of the line which 
		//connects the intersection points
		float p2X, p2Y;

		p2X = x1 + a * (x2 - x1) / d;
		p2Y = y1 + a * (y2 - y1) / d;

		//calculate first point
		float h1 = sqrt((r1 * r1) - (a * a));

		p3X = p2X - h1 * (y2 - y1) / d;
		p3Y = p2Y + h1 * (x2 - x1) / d;

		//calculate second point
		float h2 = sqrt((r2 * r2) - (a * a));

		p4X = p2X + h2 * (y2 - y1) / d;
		p4Y = p2Y - h2 * (x2 - x1) / d;

		return true;
	}

	//--------------------- circlesOverlap ------------------------
	//
	//  returns true if the circle overlap to other.
	//  overlap means one circle can be inside to other.
	//  it includes the collision of boundary.
	//-------------------------------------------------------------
	inline bool circlesOverlap(
		cocos2d::Vec2 c1, float r1,
		cocos2d::Vec2 c2, float r2)
	{
		CCASSERT(r1 > 0, "invalid circle radius r1");
		CCASSERT(r2 > 0, "invalid circle radius r2");

		float dist_between_centers_sq = ((c1.x - c2.x) * (c1.x - c2.x) +
			(c1.y - c2.y) * (c1.y - c2.y));

		if ((dist_between_centers_sq <= (r1 + r2)*(r1 + r2)) ||
			(dist_between_centers_sq <= (r1 - r2)*(r1 - r2)))
		{
			return true;
		}
		return false;
	}


	//--------------------- circlesEnforceAway ----------------------
	//
	//  returns true if the polygon and circle intersects.
	//  it makes enforce force Vec2 to enforce each other not penetrate.
	//---------------------------------------------------------------------
	inline bool circlesEnforceAway(
		cocos2d::Vec2 pos1,
		float radius1,
		cocos2d::Vec2 pos2,
		float radius2,
		cocos2d::Vec2& force)
	{
		force = cocos2d::Vec2::ZERO;
		float amount_of_overlap = radius1 + radius2 - (pos1 - pos2).getLength();

		if (amount_of_overlap < 0)
			return false;

		force = (pos1 - pos2).getNormalized() * amount_of_overlap;
		return true;
	}

	//--------------------- circlePolygonIntersect ----------------------
	//
	//  returns true if the polygon and circle intersects.
	//  also returns true if the boundary intersects.
	//  if polygon or circle are inside of others then it returns false.
	//-------------------------------------------------------------------
	inline bool circlePolygonIntersect(
		cocos2d::Vec2 pos,
		float radius,
		const std::vector<cocos2d::Vec2>& poly)
	{
		CCASSERT(radius > 0, "invalid circle radius");
		CCASSERT(poly.size() > 2, "poly is not a polygon");

		for (size_t i = 0; i < poly.size(); ++i)
		{
			size_t j = (i + 1) % poly.size();
			if (segmentCircleIntersect(poly[i], poly[j], pos, radius))
				return true;
		}
		return false;
	}

	//-----------------circlePolygonIntersectPoints-----------------
	//
	//	Given a circle and a polygon in 2D space
	//  this returns the vector of intersection points.
	//  Sometimes this function calculates the same points
	//  as different, because of checking floats.
	//--------------------------------------------------------------
	inline std::vector<cocos2d::Vec2> circlePolygonIntersectPoints(
		cocos2d::Vec2 pos,
		float radius,
		const std::vector<cocos2d::Vec2>& poly)
	{
		CCASSERT(radius > 0, "invalid circle radius");
		CCASSERT(poly.size() > 2, "poly is not a polygon");

		std::vector<cocos2d::Vec2> points;

		for (size_t i = 0; i < poly.size(); ++i)
		{
			size_t j = (i + 1) % poly.size();
			std::vector<cocos2d::Vec2> temp =
				segmentCircleIntersectPoints(poly[i], poly[j], pos, radius);

			for (auto e : temp)
				points.emplace_back(e);
		}

		return points;
	}

	//--------------------- circlePolygonOverlap ----------------------
	//
	//  returns true if the polygon and circle intersects.
	//  if polygon or circle are inside of others then it also returns true.
	//-------------------------------------------------------------------
	inline bool circlePolygonOverlap(
		cocos2d::Vec2 pos,
		float radius,
		const std::vector<cocos2d::Vec2>& poly)
	{
		CCASSERT(radius > 0, "invalid circle radius");
		CCASSERT(poly.size() > 2, "poly is not a polygon");

		for (size_t i = 0; i < poly.size(); i++)
		{
			size_t j = (i + 1) % poly.size();
			if (segmentCircleOverlap(poly[i], poly[j], pos, radius))
			{
				return true;
			}
		}
		if (circleInPolygon(pos, radius, poly))
			return true;
		return false;
	}

	//--------------------- circlePolygonEnforceAway ----------------------
	//
	//  returns true if the polygon and circle intersects.
	//  it makes enforce force Vec2 to enforce each other not penetrate.
	//  make force for move circle away from polygon.
	//---------------------------------------------------------------------
	inline bool circlePolygonEnforceAway(
		cocos2d::Vec2 pos,
		float radius,
		const std::vector<cocos2d::Vec2>& poly,
		cocos2d::Vec2& force)
	{
		CCASSERT(radius > 0, "invalid circle radius");
		CCASSERT(poly.size() > 2, "poly is not a polygon");

		std::vector<std::pair<cocos2d::Vec2, int> > end_to_end;
		std::vector<cocos2d::Vec2> inner_points;
		bool flag = false;
		force = cocos2d::Vec2::ZERO;

		for (size_t i = 0; i < poly.size(); ++i)
		{
			size_t j = (i + 1) % poly.size();

			if (geometry::pointInCircle(pos, radius, poly[i]))
			{
				inner_points.push_back(poly[i]);
			}

			std::vector<cocos2d::Vec2> v =
				geometry::lineCircleIntersectPoints(poly[i], poly[j], pos, radius);

			for (auto e : v)
			{
				if (geometry::pointInRectangle(e,poly[i],poly[j]))
						end_to_end.emplace_back(std::make_pair(e, i));
			}
		}

		if (end_to_end.size() < 2)
			return false;

		float max_segment_dist = 0;
		cocos2d::Vec2 A, B;

		for (size_t i = 0; i < end_to_end.size(); i++)
		{
			for (size_t j = i + 1; j <end_to_end.size(); j++)
			{
				float dist = (end_to_end[i].first - end_to_end[j].first).getLengthSq();
				if (dist > max_segment_dist)
				{
					max_segment_dist = dist;

					int a = end_to_end[i].second;
					int b = end_to_end[j].second;

					if ((b - a + poly.size()) % poly.size() <
						(a - b + poly.size()) % poly.size())
					{
						A = end_to_end[i].first;
						B = end_to_end[j].first;
					}
					else
					{
						B = end_to_end[i].first;
						A = end_to_end[j].first;
					}
				}
			}
		}

		float max_dist = std::numeric_limits<float>::min();
		cocos2d::Vec2 max_point;
		for (auto e : inner_points)
		{
			float dist = geometry::distLineAndPoint(A, B, e);
			if (max_dist < dist)
			{
				max_dist = dist;
				max_point = e;
			}
		}

		cocos2d::Vec2 force_heading = (A - B).getPerp().getNormalized();
		cocos2d::Vec2 mid = (A + B) / 2;
		float force_dist;

		if (max_dist == std::numeric_limits<float>::min())
		{
			if (geometry::isFacing(mid, force_heading, pos))
			{
				force_dist = radius - geometry::distLineAndPoint(A, B, pos);
			}
			else
			{
				force_dist = radius + geometry::distLineAndPoint(A, B, pos);
			}
		}
		else
			//if point exists between A and B
		{
			if (geometry::isFacing(mid, force_heading, max_point))
			{
				if (geometry::isFacing(mid, force_heading, pos))
				{
					force_dist = geometry::distLineAndPoint(A, B, max_point) +
						radius - geometry::distLineAndPoint(A, B, pos);
				}
				else
				{
					force_dist = geometry::distLineAndPoint(A, B, max_point) +
						radius + geometry::distLineAndPoint(A, B, pos);
				}
			}
			else
			{
				if (geometry::isFacing(mid, force_heading, pos))
				{
					force_dist = -geometry::distLineAndPoint(A, B, max_point) +
						radius - geometry::distLineAndPoint(A, B, pos);
				}
				else
				{
					force_dist = -geometry::distLineAndPoint(A, B, max_point) +
						radius + geometry::distLineAndPoint(A, B, pos);
				}
			}
		}

		force = force_heading * force_dist;

		return true;
	}

	//---------------------- pointOnPolygon -----------------------
	//
	//  returns true if the point P is on the polygon boundary.
	//-------------------------------------------------------------
	inline bool pointOnPolygon(
		cocos2d::Vec2 pt,
		const std::vector<cocos2d::Vec2>& poly)
	{
		CCASSERT(poly.size() > 2, "poly is not a polygon");

		for (size_t i = 0; i < poly.size(); i++)
		{
			size_t j = (i + 1) % poly.size();
			if (pointOnSegment(pt, poly[i], poly[j]))
				return true;
		}
		return false;
	}


	//---------------------- pointInPolygon -----------------------
	//
	//  returns true if the point P is in the polygon.
	//  it doesn't ensure about boundary
	//-------------------------------------------------------------
	inline bool pointInPolygon(
		cocos2d::Vec2 P,
		const std::vector<cocos2d::Vec2>& poly)
	{
		CCASSERT(poly.size() > 2, "poly is not a polygon");

		int crosses = 0;

		for (size_t i = 0; i < poly.size(); i++)
		{
			size_t j = (i + 1) % poly.size();

			if ((poly[i].y > P.y) != (poly[j].y > P.y))
			{
				float crossX =
					(poly[j].x - poly[i].x) * (P.y - poly[i].y) /
					(poly[j].y - poly[i].y) + poly[i].x;
				if (P.x < crossX)
					++crosses;
			}
		}
		return crosses % 2 > 0;
	}


	//---------------------- circleInPolygon ----------------------
	//
	//  returns true if the given circle is in the polygon.
	//  it returns false about boundary
	//-------------------------------------------------------------
	inline bool circleInPolygon(
		cocos2d::Vec2 pos,
		float radius,
		const std::vector<cocos2d::Vec2>& poly)
	{
		CCASSERT(radius > 0, "invalid circle radius");
		CCASSERT(poly.size() > 2, "poly is not a polygon");

		if (!pointInPolygon(pos, poly))
			return false;
		for (size_t i = 0; i < poly.size(); i++)
		{
			size_t j = (i + 1) % poly.size();
			if (distToSegmentSq(poly[i], poly[j], pos) <= radius * radius)
			{
				return false;
			}
		}
		return true;
	}

	//---------------------- polygonInCircle -----------------------
	//
	//  returns true if the given polygon is in the circle.
	//  it returns false about boundary
	//-------------------------------------------------------------
	inline bool polygonInCircle(
		cocos2d::Vec2 pos,
		float radius,
		const std::vector<cocos2d::Vec2>& poly)
	{
		CCASSERT(radius > 0, "invalid circle radius");
		CCASSERT(poly.size() > 2, "poly is not a polygon");

		for (size_t i = 0; i < poly.size(); i++)
		{
			if (!pointInCircle(pos, radius, poly[i]))
			{
				return false;
			}
		}
		return true;
	}

	//----------------------- polygonsIntersect -------------------------
	//
	//  returns true if the polygon and polygon intersects.
	//  also returns true if the boundary intersects.
	//  if one polygon is located inside of other, then it returns false.
	//  =>(of course no intersections in inside)
	//-------------------------------------------------------------------
	inline bool polygonsIntersect(
		const std::vector<cocos2d::Vec2>& poly1,
		const std::vector<cocos2d::Vec2>& poly2)
	{
		CCASSERT(poly1.size() > 2, "poly1 is not a polygon");
		CCASSERT(poly2.size() > 2, "poly2 is not a polygon");

		//test each line segment of object1 against each segment of object2
		for (size_t i = 0; i < poly1.size(); ++i)
		{
			size_t next_i = (i + 1) % poly1.size();

			for (size_t j = 0; j < poly2.size(); ++j)
			{
				size_t next_j = (j + 1) % poly2.size();

				if (segmentsIntersect(
					poly1[i], poly1[next_i],
					poly2[j], poly2[next_j]))
				{
					return true;
				}
			}
		}
		return false;
	}


	//--------------------polygonsIntersectPoints-------------------
	//
	//	Given two polygons in 2D space
	//  this returns the container of intersection points.
	//--------------------------------------------------------------
	inline std::vector<cocos2d::Vec2> polygonsIntersectPoints(
		const std::vector<cocos2d::Vec2>& poly1,
		const std::vector<cocos2d::Vec2>& poly2)
	{
		CCASSERT(poly1.size() > 2, "poly1 is not a polygon");
		CCASSERT(poly2.size() > 2, "poly2 is not a polygon");

		std::vector<cocos2d::Vec2> points;

		//test each line segment of object1 against each segment of object2
		for (size_t i = 0; i < poly1.size(); ++i)
		{
			size_t next_i = (i + 1) % poly1.size();

			for (size_t j = 0; j < poly2.size(); ++j)
			{
				size_t next_j = (j + 1) % poly2.size();

				cocos2d::Vec2 temp;
				if (segmentsIntersect(
					poly1[i], poly1[next_i],
					poly2[j], poly2[next_j], temp))
				{
					points.emplace_back(temp);
				}
			}
		}

		return points;
	}


	//----------------------- polygonsOverlap -------------------------
	//
	//  returns true if the polygon and polygon intersects.
	//  also returns true if the boundary intersects.
	//  if one polygon is located inside of other, then it returns false.
	//  =>(of course no intersections in inside)
	//-------------------------------------------------------------------
	inline bool polygonsOverlap(
		const std::vector<cocos2d::Vec2>& poly1,
		const std::vector<cocos2d::Vec2>& poly2)
	{
		CCASSERT(poly1.size() > 2, "poly1 is not a polygon");
		CCASSERT(poly2.size() > 2, "poly2 is not a polygon");

		if (polygonsIntersect(poly1, poly2))
			return true;

		if (pointInPolygon(poly1[0], poly2))
			return true;

		if (pointInPolygon(poly2[0], poly1))
			return true;

		return false;
	}

	


	//----------------------- polygonMaxDist --------------------------
	//
	//  returns the longest distance between polygon vertices.
	//  time complexity is O(n)
	//-----------------------------------------------------------------
	inline float polygonMaxDist(const std::vector<cocos2d::Vec2>& poly)
	{
		CCASSERT(poly.size() > 2, "poly is not a polygon");

		float max_dist = 0;
		size_t i = 0;
		size_t j = 0;
		size_t length = poly.size();

		for (; i < length; i++) {
			bool flag = true;
			float increasing = 0;

			for (j = (j - 2 + length) % length; flag; j = (j + 1) % length)
			{
				float dist = (poly[i] - poly[j]).getLength();
				if (increasing <= dist) {
					increasing = dist;

					if (max_dist < dist)
						max_dist = dist;
				}
				else {
					flag = false;
				}
			}
		}
		return max_dist;
	}


	//----------------------- polygonMaxDist --------------------------
	//
	//  makes the segment(A, B) which has longest distance between
	//  polygon vertices.
	//  time complexity is O(n)
	//-----------------------------------------------------------------
	inline float polygonMaxDist(
		const std::vector<cocos2d::Vec2>& poly,
		cocos2d::Vec2& A,
		cocos2d::Vec2& B)
	{
		CCASSERT(poly.size() > 2, "poly is not a polygon");

		float max_dist = 0;
		size_t maxi = 0;
		size_t maxj = 0;
		size_t i = 0;
		size_t j = 0;
		size_t length = poly.size();

		for (; i < length; i++) {
			bool flag = true;
			float increasing = 0;

			for (j = (j - 2 + length) % length; flag; j = (j + 1) % length)
			{
				float dist = (poly[i] - poly[j]).getLength();
				if (increasing <= dist) {
					increasing = dist;

					if (max_dist < dist)
					{
						maxi = i;
						maxj = j;
						max_dist = dist;
					}
				}
				else {
					flag = false;
				}
			}
		}
		A = poly[maxi];
		B = poly[maxj];

		return max_dist;
	}

	//----------------------- polygonCentroid --------------------------
	//
	//  returns the centroid point of polygon.
	//  
	//-----------------------------------------------------------------
	inline cocos2d::Vec2 polygonCentroid(const std::vector<cocos2d::Vec2>& poly)
	{
		float centerX = 0, centerY = 0;
		float area = 0;

		cocos2d::Vec2 center;

		cocos2d::Vec2  first_point;
		cocos2d::Vec2  second_point;

		float factor = 0;

		for (size_t i = 0; i < poly.size(); i++)
		{
			size_t j = (i + 1) % poly.size();

			first_point = poly[i];
			second_point = poly[j];

			factor = (first_point.x * second_point.y) - (second_point.x * first_point.y);
			area += factor;

			centerX += (first_point.x + second_point.x) * factor;
			centerY += (first_point.y + second_point.y) * factor;
		}

		area /= 2.0;
		area *= 6.0f;

		factor = 1 / area;

		centerX *= factor;
		centerY *= factor;

		return cocos2d::Vec2(centerX, centerY);
	}


	//-------------------------- makeConvexHull -----------------------------
	//
	// Returns a list of points on the convex hull in counter-clockwise order.
	// Note: the last point in the returned list is the same as the first one. 
	//------------------------------------------------------------------------
	inline std::vector<cocos2d::Vec2> makeConvexHull(
		const std::vector<cocos2d::Vec2>& poly)
	{
		int n = poly.size(), k = 0;

		std::vector<cocos2d::Vec2> polytemp(n);
		for (int i = 0; i < n; i++)
			polytemp[i] = poly[i];

		std::vector<cocos2d::Vec2> hull(2 * n);

		// Sort points lexicographically
		std::sort(polytemp.begin(), polytemp.end());

		// Build lower hull
		for (int i = 0; i < n; ++i)
		{
			while (k >= 2 && ccw(hull[k - 2], hull[k - 1], polytemp[i]) <= 0) k--;
			hull[k++] = polytemp[i];
		}

		// Build upper hull
		for (int i = n - 2, t = k + 1; i >= 0; i--)
		{
			while (k >= t && ccw(hull[k - 2], hull[k - 1], polytemp[i]) <= 0) k--;
			hull[k++] = polytemp[i];
		}

		hull.resize(k);
		return hull;
	}

	//------------------------ enclosingCircleNaive --------------------------
	//
	// Smallest enclosing circle - naive algorithm (O(n^4) time)
	//------------------------------------------------------------------------
	inline Circle enclosingCircleNaive(std::vector<cocos2d::Vec2> points)
	{
		CCASSERT(points.size() > 0, "no points");

		if (points.size() == 1)
			return Circle(points[0], 0);

		// Try all unique pairs
		Circle best(0, 0, 0);
		for (size_t i = 0; i < points.size(); i++)
		{
			for (size_t j = i + 1; j < points.size(); j++)
			{
				Circle c = makeDiameterCircle(points[i], points[j]);
				if (c.containPoint(points) && (best.radius == 0 ||
					c.radius < best.radius))
					best = c;
			}
		}
		if (best.radius != 0)
			return best;  // This optimization is not mathematically proven

						  // Try all unique triples
		for (size_t i = 0; i < points.size(); i++)
		{
			for (size_t j = i + 1; j < points.size(); j++)
			{
				for (size_t k = j + 1; k < points.size(); k++)
				{
					Circle c = makeCircumcircle(points[i], points[j], points[k]);
					if (c.radius != 0 &&
						c.containPoint(points) &&
						(best.radius == 0 || c.radius < best.radius))
						best = c;
				}
			}
		}
		if (best.radius == 0)
			CCASSERT(false, "impossible");
		return best;
	}

	//------------------------ enclosingCircle --------------------------
	//
	// Returns the smallest circle that encloses all the given points. 
	// Runs in expected O(n) time, randomized.
	// Note: If 0 points are given, null is returned. 
	// If 1 point is given, a circle of radius 0 is returned.
	// Initially: No boundary points known
	//------------------------------------------------------------------------
	inline Circle enclosingCircle(const std::vector<cocos2d::Vec2>& points)
	{
		std::random_device rd;
		std::mt19937 g(rd());

		// Clone list to preserve the caller's data, randomize order
		std::vector<cocos2d::Vec2> shuffled(points);
		std::shuffle(shuffled.begin(), shuffled.end(), g);

		// Progressively add points to circle or recompute circle
		Circle c(0, 0, -1);
		for (size_t i = 0; i < shuffled.size(); i++)
		{
			cocos2d::Vec2 p = shuffled[i];
			if (c.radius == -1 || !c.containPoint(p))
			{
				c = makeCircleOnePoint(shuffled, i + 1, p);
			}
		}

		return c;
	}


	// One boundary point known
	inline Circle makeCircleOnePoint(
		const std::vector<cocos2d::Vec2>& points,
		size_t size,
		cocos2d::Vec2 p)
	{
		Circle c(p, 0);

		for (size_t i = 0; i < size; i++)
		{
			cocos2d::Vec2 q = points[i];

			if (!c.containPoint(q))
			{
				if (c.radius == 0)
					c = makeDiameterCircle(p, q);
				else
					c = makeCircleTwoPoints(points, i + 1, p, q);
			}
		}

		return c;
	}

	// Two boundary points known
	inline Circle makeCircleTwoPoints(
		const std::vector<cocos2d::Vec2>& points,
		size_t size,
		cocos2d::Vec2 p,
		cocos2d::Vec2 q)
	{
		Circle temp = makeDiameterCircle(p, q);

		if (temp.containPoint(points))
			return temp;

		Circle left(0, 0, -1);
		Circle right(0, 0, -1);
		for (size_t i = 0; i < size; i++)
			// Form a circumcircle with each point
		{
			cocos2d::Vec2 pq = q - p;
			float cross = pq.cross(points[i] - p);
			Circle c = makeCircumcircle(p, q, points[i]);
			if (c.radius == -1)
				continue;
			else if (cross > 0 &&
				(left.radius == -1 || pq.cross(c.origin - p) > pq.cross(left.origin - p)))
				left = c;
			else if (cross < 0 &&
				(right.radius == -1 || pq.cross(c.origin - p) < pq.cross(right.origin - p)))
				right = c;
		}
		return right.radius == -1 ||
			left.radius != -1 &&
			left.radius <= right.radius ? left : right;
	}

	//-------------------------- makeDiameterCircle -----------------------------
	//
	// returns a circle made by two points
	//------------------------------------------------------------------------
	inline Circle makeDiameterCircle(cocos2d::Vec2 a, cocos2d::Vec2 b)
	{
		return Circle(cocos2d::Vec2((a.x + b.x) / 2, (a.y + b.y) / 2), a.distance(b) / 2);
	}

	//-------------------------- makeCircumcircle -----------------------------
	//
	// returns a circle made by three points
	//------------------------------------------------------------------------
	inline Circle makeCircumcircle(cocos2d::Vec2 a, cocos2d::Vec2 b, cocos2d::Vec2 c)
	{
		// Mathematical algorithm from Wikipedia: Circumscribed circle
		float d = (a.x * (b.y - c.y) + b.x * (c.y - a.y) + c.x * (a.y - b.y)) * 2;
		if (d == 0)
			return Circle(0, 0, -1);
		float x = (a.getLengthSq() * (b.y - c.y) + b.getLengthSq() * (c.y - a.y) + c.getLengthSq() * (a.y - b.y)) / d;
		float y = (a.getLengthSq() * (c.x - b.x) + b.getLengthSq() * (a.x - c.x) + c.getLengthSq() * (b.x - a.x)) / d;
		cocos2d::Vec2 p = cocos2d::Vec2(x, y);
		return Circle(p, p.distance(a) + 0.1f);
	}


	//given a plane and a ray this function determins how far along the ray 
	//an interestion occurs. Returns negative if the ray is parallel
	inline float distToRayPlaneIntersect(
		cocos2d::Vec2 RayOrigin,
		cocos2d::Vec2 RayHeading,
		cocos2d::Vec2 PlanePoint,  //any point on the plane
		cocos2d::Vec2 PlaneNormal)
	{
		float d = -PlaneNormal.dot(PlanePoint);
		float numer = PlaneNormal.dot(RayOrigin) + d;
		float denom = PlaneNormal.dot(RayHeading);

		// NORMAL is parallel to vector
		if ((denom < 0.000001) && (denom > -0.000001))
		{
			return (-1.0);
		}

		return -(numer / denom);
	}

	//------------------------- whereIsPoint --------------------------------------
	inline span_type whereIsPoint(
		cocos2d::Vec2 point,
		cocos2d::Vec2 PointOnPlane, //any point on the plane
		cocos2d::Vec2 PlaneNormal)
	{
		cocos2d::Vec2 dir = PointOnPlane - point;

		float d = dir.dot(PlaneNormal);

		if (d<-0.000001)
		{
			return plane_front;
		}

		else if (d>0.000001)
		{
			return plane_backside;
		}

		return on_plane;
	}

	//-------------------------- rayCircleIntersect -----------------------------
	inline float rayCircleIntersect(
		cocos2d::Vec2 RayOrigin,
		cocos2d::Vec2 RayHeading,
		cocos2d::Vec2 CircleOrigin,
		float  radius,
		float& dist)
	{
		cocos2d::Vec2 ToCircle = CircleOrigin - RayOrigin;
		float length = ToCircle.getLength();
		float v = ToCircle.dot(RayHeading);
		float d = radius*radius - (length*length - v*v);

		// the distance to the [first] intersecting point
		dist = (v - sqrt(d));
		return (d < 0.0);
	}

	//----------------------------- rayCircleIntersect --------------------------
	inline bool rayCircleIntersect(
		cocos2d::Vec2 RayOrigin,
		cocos2d::Vec2 RayHeading,
		cocos2d::Vec2 CircleOrigin,
		float     radius)
	{
		cocos2d::Vec2 ToCircle = CircleOrigin - RayOrigin;
		float length = ToCircle.getLength();
		float v = ToCircle.dot(RayHeading);
		float d = radius*radius - (length*length - v*v);

		return (d < 0.0);
	}


	//---------------------------getTangentPoints-----------------------------
	//  Given a point P and a circle of radius R centered at C this function
	//  determines the two points on the circle that intersect with the 
	//  tangents from P to the circle. Returns false if P is within the circle.
	//
	//  thanks to Dave Eberly for this one.
	//------------------------------------------------------------------------
	inline bool getTangentPoints(
		cocos2d::Vec2 C,
		float R,
		cocos2d::Vec2 P,
		cocos2d::Vec2& T1,
		cocos2d::Vec2& T2)
	{
		cocos2d::Vec2 PmC = P - C;
		float SqrLen = PmC.getLengthSq();
		float RSqr = R*R;
		if (SqrLen <= RSqr)
		{
			// P is inside or on the circle
			return false;
		}

		float InvSqrLen = 1 / SqrLen;
		float Root = sqrt(fabs(SqrLen - RSqr));

		T1.x = C.x + R*(R*PmC.x - PmC.y*Root)*InvSqrLen;
		T1.y = C.y + R*(R*PmC.y + PmC.x*Root)*InvSqrLen;
		T2.x = C.x + R*(R*PmC.x + PmC.y*Root)*InvSqrLen;
		T2.y = C.y + R*(R*PmC.y - PmC.x*Root)*InvSqrLen;

		return true;
	}

	//------------------------ circlesIntersectArea --------------------
	//
	//  Tests to see if two circles overlap and if so calculates the area
	//  defined by the union
	//
	// see http://mathforum.org/library/drmath/view/54785.html
	//-----------------------------------------------------------------------
	inline float circlesIntersectArea(
		float x1, float y1, float r1,
		float x2, float y2, float r2)
	{
		//first calculate the intersection points
		float iX1, iY1, iX2, iY2;

		if (!circlesIntersect(x1, y1, r1, x2, y2, r2, iX1, iY1, iX2, iY2))
		{
			return 0.0; //no overlap
		}

		//calculate the distance between the circle centers
		float d = sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));

		//find the angles given that A and B are the two circle centers
		//and C and D are the intersection points
		float CBD = 2 * acos((r2*r2 + d*d - r1*r1) / (r2 * d * 2));
		float CAD = 2 * acos((r1*r1 + d*d - r2*r2) / (r1 * d * 2));

		//Then we find the segment of each of the circles cut off by the 
		//chord CD, by taking the area of the sector of the circle BCD and
		//subtracting the area of triangle BCD. Similarly we find the area
		//of the sector ACD and subtract the area of triangle ACD.

		float area = 0.5f*CBD*r2*r2 - 0.5f*r2*r2*sin(CBD) +
			0.5f*CAD*r1*r1 - 0.5f*r1*r1*sin(CAD);

		return area;
	}

	//-------------------------------- circleArea ---------------------------
	//
	//  given the radius, calculates the area of a circle
	//-----------------------------------------------------------------------
	inline float circleArea(float radius)
	{
		return M_PI * radius * radius;
	}


	//------------------- getSegmentCircleClosestIntersectPoint ------------
	//
	//  given a line segment AB and a circle position and radius, this function
	//  determines if there is an intersection and stores the position of the 
	//  closest intersection in the reference IntersectionPoint
	//
	//  returns false if no intersection point is found
	//-----------------------------------------------------------------------------
	inline bool getSegmentCircleClosestIntersectPoint(
		cocos2d::Vec2 A,
		cocos2d::Vec2 B,
		cocos2d::Vec2 pos,
		float    radius,
		cocos2d::Vec2& IntersectionPoint)
	{
		cocos2d::Vec2 toBNorm = (B - A).getNormalized();

		//move the circle into the local space defined by the vector B-A with origin
		//at A
		cocos2d::Vec2 LocalPos = PointToLocalSpace(pos, toBNorm, toBNorm.getPerp(), A);

		bool ipFound = false;

		//if the local position + the radius is negative then the circle lays behind
		//point A so there is no intersection possible. If the local x pos minus the 
		//radius is greater than length A-B then the circle cannot intersect the 
		//line segment
		if ((LocalPos.x + radius >= 0) &&
			((LocalPos.x - radius)*(LocalPos.x - radius) <= (A - B).getLengthSq()))
		{
			//if the distance from the x axis to the object's position is less
			//than its radius then there is a potential intersection.
			if (fabs(LocalPos.y) < radius)
			{
				//now to do a line/circle intersection test. The center of the 
				//circle is represented by A, B. The intersection points are 
				//given by the formulae x = A +/-sqrt(r^2-B^2), y=0. We only 
				//need to look at the smallest positive value of x.
				float a = LocalPos.x;
				float b = LocalPos.y;

				float ip = a - sqrt(radius*radius - b*b);

				if (ip <= 0)
				{
					ip = a + sqrt(radius*radius - b*b);
				}

				ipFound = true;

				IntersectionPoint = A + toBNorm*ip;
			}
		}

		return ipFound;
	}

	// Find the tangent points for this circle and external point.
	// Return true if we find the tangents, false if the point is
	// inside the circle.
	inline bool findTangents(cocos2d::Vec2 center, float radius,
		cocos2d::Vec2 external_point, cocos2d::Vec2& pt1, cocos2d::Vec2& pt2)
	{
		// Find the distance squared from the
		// external point to the circle's center.
		double dx = center.x - external_point.x;
		double dy = center.y - external_point.y;
		double D_squared = dx * dx + dy * dy;
		if (D_squared < radius * radius)
		{
			pt1 = cocos2d::Vec2(-1, -1);
			pt2 = cocos2d::Vec2(-1, -1);
			return false;
		}

		// Find the distance from the external point
		// to the tangent points.
		double L = sqrt(D_squared - radius * radius);

		// Find the points of intersection between
		// the original circle and the circle with
		// center external_point and radius dist.
		circlesIntersectPoints(
			center.x, center.y, radius,
			external_point.x, external_point.y, (float)L,
			pt1, pt2);

		return true;
	}

	// Find the tangent points for these two circles.
	// Return the number of tangents: 4, 2, or 0.
	inline int findTangentsInTwoCircle(
		cocos2d::Vec2 c1, float radius1, cocos2d::Vec2 c2, float radius2,
		cocos2d::Vec2& outer1_p1, cocos2d::Vec2& outer1_p2,
		cocos2d::Vec2& outer2_p1, cocos2d::Vec2& outer2_p2,
		cocos2d::Vec2& inner1_p1, cocos2d::Vec2& inner1_p2,
		cocos2d::Vec2& inner2_p1, cocos2d::Vec2& inner2_p2)
	{
		CCASSERT(!(c1 == c2 && radius1 == radius2), "two circles are same!");
		// Make sure radius1 <= radius2.
		if (radius1 > radius2)
		{
			// Call this method switching the circles.
			return findTangentsInTwoCircle(
				c2, radius2, c1, radius1,
				outer1_p2, outer1_p1,
				outer2_p2, outer2_p1,
				inner1_p2, inner1_p1,
				inner2_p2, inner2_p1);
		}

		// initialize the return values in case
		// some tangents are missing.
		outer1_p1 = cocos2d::Vec2(-1, -1);
		outer1_p2 = cocos2d::Vec2(-1, -1);
		outer2_p1 = cocos2d::Vec2(-1, -1);
		outer2_p2 = cocos2d::Vec2(-1, -1);
		inner1_p1 = cocos2d::Vec2(-1, -1);
		inner1_p2 = cocos2d::Vec2(-1, -1);
		inner2_p1 = cocos2d::Vec2(-1, -1);
		inner2_p2 = cocos2d::Vec2(-1, -1);

		// ***************************
		// * Find the outer tangents *
		// ***************************
		{
			float radius2a = radius2 - radius1;
			if (!findTangents(c2, radius2a, c1,
				outer1_p2, outer2_p2))
			{
				// There are no tangents.
				return 0;
			}

			// Get the vector perpendicular to the
			// first tangent with length radius1.
			float v1x = -(outer1_p2.y - c1.y);
			float v1y = outer1_p2.x - c1.x;
			float v1_length = (float)sqrt(v1x * v1x + v1y * v1y);
			v1x *= radius1 / v1_length;
			v1y *= radius1 / v1_length;
			// Offset the tangent vector's points.
			outer1_p1 = cocos2d::Vec2(c1.x + v1x, c1.y + v1y);
			outer1_p2 = cocos2d::Vec2(
				outer1_p2.x + v1x,
				outer1_p2.y + v1y);

			// Get the vector perpendicular to the
			// second tangent with length radius1.
			float v2x = outer2_p2.y - c1.y;
			float v2y = -(outer2_p2.x - c1.x);
			float v2_length = (float)sqrt(v2x * v2x + v2y * v2y);
			v2x *= radius1 / v2_length;
			v2y *= radius1 / v2_length;
			// Offset the tangent vector's points.
			outer2_p1 = cocos2d::Vec2(c1.x + v2x, c1.y + v2y);
			outer2_p2 = cocos2d::Vec2(
				outer2_p2.x + v2x,
				outer2_p2.y + v2y);
		}

		// If the circles intersect, then there are no inner tangents.
		float dx = c2.x - c1.x;
		float dy = c2.y - c1.y;
		double dist = sqrt(dx * dx + dy * dy);
		if (dist <= radius1 + radius2) return 2;

		// ***************************
		// * Find the inner tangents *
		// ***************************
		{
			float radius1a = radius1 + radius2;
			findTangents(c1, radius1a, c2,
				inner1_p2, inner2_p2);

			// Get the vector perpendicular to the
			// first tangent with length radius2.
			float v1x = inner1_p2.y - c2.y;
			float v1y = -(inner1_p2.x - c2.x);
			float v1_length = (float)sqrt(v1x * v1x + v1y * v1y);
			v1x *= radius2 / v1_length;
			v1y *= radius2 / v1_length;
			// Offset the tangent vector's points.
			inner1_p1 = cocos2d::Vec2(c2.x + v1x, c2.y + v1y);
			inner1_p2 = cocos2d::Vec2(
				inner1_p2.x + v1x,
				inner1_p2.y + v1y);

			// Get the vector perpendicular to the
			// second tangent with length radius2.
			float v2x = -(inner2_p2.y - c2.y);
			float v2y = inner2_p2.x - c2.x;
			float v2_length = (float)sqrt(v2x * v2x + v2y * v2y);
			v2x *= radius2 / v2_length;
			v2y *= radius2 / v2_length;
			// Offset the tangent vector's points.
			inner2_p1 = cocos2d::Vec2(c2.x + v2x, c2.y + v2y);
			inner2_p2 = cocos2d::Vec2(
				inner2_p2.x + v2x,
				inner2_p2.y + v2y);
		}

		return 4;
	}
}