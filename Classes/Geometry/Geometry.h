#pragma once

//------------------------------------------------------------------------
//
//  Name:   Geometry.h
//
//  Desc:   Functions for 2D geometry collisions
//
//  Author: Insub Im (insooneelife@naver.com)
//------------------------------------------------------------------------

#include "cocos2d.h"
#include "Shape.h"
#include "Rect.h"
#include "Circle.h"
#include "Segment.h"
#include "Polygon.h"
#include "Transformations.h"

namespace geometry
{
	//--------------------------- Shape Functions --------------------------------//
	inline bool intersect(const Rect& rect1, const Rect& rect2);

	inline bool intersect(const Rect& rect, const Circle& circle);

	inline bool intersect(const Rect& rect, const Segment& segment);

	inline bool intersect(const Rect& rect, const Polygon& polygon);


	inline bool intersect(const Circle& circle, const Rect& rect);

	inline bool intersect(const Circle& circle1, const Circle& circle2);

	inline bool intersect(const Circle& circle, const Segment& segment);

	inline bool intersect(const Circle& circle, const Polygon& polygon);


	inline bool intersect(const Segment& segment, const Rect& rect);

	inline bool intersect(const Segment& segment, const Circle& circle);

	inline bool intersect(const Segment& segment1, const Segment& segment2);

	inline bool intersect(const Segment& segment, const Polygon& polygon);


	inline bool intersect(const Polygon& polygon, const Rect& rect);

	inline bool intersect(const Polygon& polygon, const Circle& circle);

	inline bool intersect(const Polygon& polygon, const Segment& segment);

	inline bool intersect(const Polygon& polygon1, const Polygon& polygon2);


	inline std::vector<cocos2d::Vec2> intersectPoints(
		const Rect& rect1,
		const Rect& rect2);

	inline std::vector<cocos2d::Vec2> intersectPoints(
		const Rect& rect,
		const Circle& circle);

	inline std::vector<cocos2d::Vec2> intersectPoints(
		const Rect& rect,
		const Segment& segment);

	inline std::vector<cocos2d::Vec2> intersectPoints(
		const Rect& rect,
		const Polygon& polygon);


	inline std::vector<cocos2d::Vec2> intersectPoints(
		const Circle& circle,
		const Rect& rect);

	inline std::vector<cocos2d::Vec2> intersectPoints(
		const Circle& circle1,
		const Circle& circle2);

	inline std::vector<cocos2d::Vec2> intersectPoints(
		const Circle& circle,
		const Segment& segment);

	inline std::vector<cocos2d::Vec2> intersectPoints(
		const Circle& circle,
		const Polygon& polygon);


	inline std::vector<cocos2d::Vec2> intersectPoints(
		const Segment& segment,
		const Rect& rect);

	inline std::vector<cocos2d::Vec2> intersectPoints(
		const Segment& segment,
		const Circle& circle);

	inline std::vector<cocos2d::Vec2> intersectPoints(
		const Segment& segment1,
		const Segment& segment2);

	inline std::vector<cocos2d::Vec2> intersectPoints(
		const Segment& segment,
		const Polygon& polygon);


	inline std::vector<cocos2d::Vec2> intersectPoints(
		const Polygon& polygon,
		const Rect& rect);

	inline std::vector<cocos2d::Vec2> intersectPoints(
		const Polygon& polygon,
		const Circle& circle);

	inline std::vector<cocos2d::Vec2> intersectPoints(
		const Polygon& polygon,
		const Segment& segment);

	inline std::vector<cocos2d::Vec2> intersectPoints(
		const Polygon& polygon1,
		const Polygon& polygon2);


	inline bool overlap(const Rect& rect1, const Rect& rect2);

	inline bool overlap(const Rect& rect, const Circle& circle);

	inline bool overlap(const Rect& rect, const Segment& segment);

	inline bool overlap(const Rect& rect, const Polygon& polygon);


	inline bool overlap(const Circle& circle, const Rect& rect);

	inline bool overlap(const Circle& circle1, const Circle& circle2);

	inline bool overlap(const Circle& circle, const Segment& segment);

	inline bool overlap(const Circle& circle, const Polygon& polygon);


	inline bool overlap(const Segment& segment, const Rect& rect);

	inline bool overlap(const Segment& segment, const Circle& circle);

	inline bool overlap(const Segment& segment1, const Segment& segment2);

	inline bool overlap(const Segment& segment, const Polygon& polygon);


	inline bool overlap(const Polygon& polygon, const Rect& rect);

	inline bool overlap(const Polygon& polygon, const Circle& circle);

	inline bool overlap(const Polygon& polygon, const Segment& segment);

	inline bool overlap(const Polygon& polygon1, const Polygon& polygon2);


	inline cocos2d::Vec2 enforceFrom(const Rect& rect1, const Rect& rect2);

	inline cocos2d::Vec2 enforceFrom(const Rect& rect, const Circle& circle);

	inline cocos2d::Vec2 enforceFrom(const Rect& rect, const Segment& segment);

	inline cocos2d::Vec2 enforceFrom(const Rect& rect, const Polygon& polygon);


	inline cocos2d::Vec2 enforceFrom(const Circle& circle, const Rect& rect);

	inline cocos2d::Vec2 enforceFrom(const Circle& circle1, const Circle& circle2);

	inline cocos2d::Vec2 enforceFrom(const Circle& circle, const Segment& segment);

	inline cocos2d::Vec2 enforceFrom(const Circle& circle, const Polygon& polygon);


	inline cocos2d::Vec2 enforceFrom(const Segment& segment, const Rect& rect);

	inline cocos2d::Vec2 enforceFrom(const Segment& segment, const Circle& circle);

	inline cocos2d::Vec2 enforceFrom(const Segment& segment1, const Segment& segment2);

	inline cocos2d::Vec2 enforceFrom(const Segment& segment, const Polygon& polygon);


	inline cocos2d::Vec2 enforceFrom(const Polygon& polygon, const Rect& rect);

	inline cocos2d::Vec2 enforceFrom(const Polygon& polygon, const Circle& circle);

	inline cocos2d::Vec2 enforceFrom(const Polygon& polygon, const Segment& segment);

	inline cocos2d::Vec2 enforceFrom(const Polygon& polygon1, const Polygon& polygon2);


	//------------------------------- Others -----------------------------------//
	   
	inline bool isContainPointInDiamond(
		const cocos2d::Vec2& diamondCenter,
		float halfLen,
		const cocos2d::Vec2& p);

	inline bool isEqual(float a, float b);

	inline bool isZero(float val);

	inline cocos2d::Vec2 rotatePoint(float cx, float cy, float angle, cocos2d::Vec2 p);

	inline float ccw(float x1, float y1, float x2, float y2, float x3, float y3);

	inline float ccw(cocos2d::Vec2 p1, cocos2d::Vec2 p2, cocos2d::Vec2 p3);

	inline bool pointInCircle(
		cocos2d::Vec2 pos,
		float radius,
		cocos2d::Vec2 p);

	inline bool isFacing(
		cocos2d::Vec2 pos,
		cocos2d::Vec2 heading,
		cocos2d::Vec2 target);

	inline cocos2d::Vec2 perpendiculaFoot(
		cocos2d::Vec2 A,
		cocos2d::Vec2 B,
		cocos2d::Vec2 P);

	inline float distLineAndPoint(
		cocos2d::Vec2 A,
		cocos2d::Vec2 B,
		cocos2d::Vec2 P);

	inline bool lineIntersect(
		cocos2d::Vec2 A,
		cocos2d::Vec2 B,
		cocos2d::Vec2 C,
		cocos2d::Vec2 D);

	inline bool lineIntersect(
		cocos2d::Vec2 A,
		cocos2d::Vec2 B,
		cocos2d::Vec2 C,
		cocos2d::Vec2 D,
		cocos2d::Vec2& P);

	inline bool parallelSegmentsIntersect(
		cocos2d::Vec2 A,
		cocos2d::Vec2 B,
		cocos2d::Vec2 C,
		cocos2d::Vec2 D,
		cocos2d::Vec2& P);

	inline std::vector<cocos2d::Vec2> lineCircleIntersectPoints(
		cocos2d::Vec2 A,
		cocos2d::Vec2 B,
		cocos2d::Vec2 P,
		float R);

	inline bool pointInRectangle(
		cocos2d::Vec2 P,
		cocos2d::Vec2 A,
		cocos2d::Vec2 B);

	inline bool rectanglesIntersect(
		cocos2d::Vec2 bottom_left1,
		cocos2d::Vec2 top_right1,
		cocos2d::Vec2 bottom_left2,
		cocos2d::Vec2 top_right2);

	inline std::vector<cocos2d::Vec2> rectanglesIntersectPoints(
		cocos2d::Vec2 bottom_left1,
		cocos2d::Vec2 top_right1,
		cocos2d::Vec2 bottom_left2,
		cocos2d::Vec2 top_right2);

	inline bool rectanglesOverlap(
		cocos2d::Vec2 bottom_left1,
		cocos2d::Vec2 top_right1,
		cocos2d::Vec2 bottom_left2,
		cocos2d::Vec2 top_right2);

	inline bool rectangleInRectangle(
		cocos2d::Vec2 bottom_left1,
		cocos2d::Vec2 top_right1,
		cocos2d::Vec2 bottom_left2,
		cocos2d::Vec2 top_right2);

	inline bool rectangleCircleIntersect(
		cocos2d::Vec2 bottom_left,
		cocos2d::Vec2 top_right,
		cocos2d::Vec2 pos,
		float radius);

	inline std::vector<cocos2d::Vec2> rectangleCircleIntersectPoints(
		cocos2d::Vec2 bottom_left,
		cocos2d::Vec2 top_right,
		cocos2d::Vec2 pos,
		float radius);

	inline bool rectangleInCircle(
		cocos2d::Vec2 bottom_left,
		cocos2d::Vec2 top_right,
		cocos2d::Vec2 pos,
		float radius);

	inline bool circleInRectangle(
		cocos2d::Vec2 bottom_left,
		cocos2d::Vec2 top_right,
		cocos2d::Vec2 pos,
		float radius);

	inline bool rectangleCircleOverlap(
		cocos2d::Vec2 bottom_left,
		cocos2d::Vec2 top_right,
		cocos2d::Vec2 pos,
		float radius);

	inline bool rectangleSegmentIntersect(
		cocos2d::Vec2 bottom_left,
		cocos2d::Vec2 top_right,
		cocos2d::Vec2 A,
		cocos2d::Vec2 B);

	inline std::vector<cocos2d::Vec2> rectangleSegmentIntersectPoints(
		cocos2d::Vec2 bottom_left,
		cocos2d::Vec2 top_right,
		cocos2d::Vec2 A,
		cocos2d::Vec2 B);

	inline bool rectangleSegmentOverlap(
		cocos2d::Vec2 bottom_left,
		cocos2d::Vec2 top_right,
		cocos2d::Vec2 A,
		cocos2d::Vec2 B);

	inline bool rectanglePolygonIntersect(
		cocos2d::Vec2 bottom_left,
		cocos2d::Vec2 top_right,
		const std::vector<cocos2d::Vec2>& poly);

	inline std::vector<cocos2d::Vec2> rectanglePolygonIntersectPoints(
		cocos2d::Vec2 bottom_left,
		cocos2d::Vec2 top_right,
		const std::vector<cocos2d::Vec2>& poly);

	inline bool rectanglePolygonOverlap(
		cocos2d::Vec2 bottom_left,
		cocos2d::Vec2 top_right,
		const std::vector<cocos2d::Vec2>& poly);

	inline bool segmentsIntersect(
		cocos2d::Vec2 A,
		cocos2d::Vec2 B,
		cocos2d::Vec2 C,
		cocos2d::Vec2 D);

	inline bool segmentsIntersect(
		cocos2d::Vec2 A,
		cocos2d::Vec2 B,
		cocos2d::Vec2 C,
		cocos2d::Vec2 D,
		cocos2d::Vec2& P);

	inline bool segmentsIntersect(
		cocos2d::Vec2 A,
		cocos2d::Vec2 B,
		cocos2d::Vec2 C,
		cocos2d::Vec2 D,
		float& dist,
		cocos2d::Vec2& P);

	inline float distToSegment(
		cocos2d::Vec2 A,
		cocos2d::Vec2 B,
		cocos2d::Vec2 P);

	inline float distToSegmentSq(
		cocos2d::Vec2 A,
		cocos2d::Vec2 B,
		cocos2d::Vec2 P);

	inline bool pointOnSegment(
		const cocos2d::Vec2 P,
		const cocos2d::Vec2 A,
		const cocos2d::Vec2 B);

	inline bool segmentCircleIntersect(
		cocos2d::Vec2 A,
		cocos2d::Vec2 B,
		cocos2d::Vec2 P,
		float    radius);

	inline std::vector<cocos2d::Vec2> segmentCircleIntersectPoints(
		cocos2d::Vec2 A,
		cocos2d::Vec2 B,
		cocos2d::Vec2 P,
		float R);

	inline bool segmentCircleOverlap(
		cocos2d::Vec2 A,
		cocos2d::Vec2 B,
		cocos2d::Vec2 P,
		float    radius);

	inline bool segmentPolygonIntersect(
		cocos2d::Vec2 A,
		cocos2d::Vec2 B,
		const std::vector<cocos2d::Vec2>& poly);

	inline std::vector<cocos2d::Vec2> segmentPolygonIntersectPoints(
		cocos2d::Vec2 A,
		cocos2d::Vec2 B,
		const std::vector<cocos2d::Vec2>& poly);

	inline bool segmentPolygonOverlap(
		cocos2d::Vec2 A,
		cocos2d::Vec2 B,
		const std::vector<cocos2d::Vec2>& poly);

	inline bool circlesIntersect(
		cocos2d::Vec2 c1, float r1,
		cocos2d::Vec2 c2, float r2);

	inline int circlesIntersectPoints(
		float cx0, float cy0, float radius0,
		float cx1, float cy1, float radius1,
		cocos2d::Vec2& intersection1, cocos2d::Vec2& intersection2);

	inline std::vector<cocos2d::Vec2> circlesIntersectPoints(
		cocos2d::Vec2 p1, float r1,
		cocos2d::Vec2 p2, float r2);

	inline bool circlesOverlap(
		cocos2d::Vec2 c1, float r1,
		cocos2d::Vec2 c2, float r2);

	inline bool circlesEnforceAway(
		cocos2d::Vec2 pos1,
		float radius1,
		cocos2d::Vec2 pos2,
		float radius2,
		cocos2d::Vec2& force);

	inline bool circlePolygonIntersect(
		cocos2d::Vec2 pos,
		float radius,
		const std::vector<cocos2d::Vec2>& poly);

	inline std::vector<cocos2d::Vec2> circlePolygonIntersectPoints(
		cocos2d::Vec2 pos,
		float radius,
		const std::vector<cocos2d::Vec2>& poly);

	inline bool circlePolygonOverlap(
		cocos2d::Vec2 pos,
		float radius,
		const std::vector<cocos2d::Vec2>& poly);

	inline bool circlePolygonEnforceAway(
		cocos2d::Vec2 pos,
		float radius,
		const std::vector<cocos2d::Vec2>& poly,
		cocos2d::Vec2& force);

	inline bool pointOnPolygon(
		cocos2d::Vec2 pt,
		const std::vector<cocos2d::Vec2>& poly);

	inline bool pointInPolygon(
		cocos2d::Vec2 pt,
		const std::vector<cocos2d::Vec2>& poly);

	inline bool circleInPolygon(
		cocos2d::Vec2 pos,
		float radius,
		const std::vector<cocos2d::Vec2>& poly);

	inline bool polygonInCircle(
		cocos2d::Vec2 pos,
		float radius,
		const std::vector<cocos2d::Vec2>& poly);

	inline bool polygonsIntersect(
		const std::vector<cocos2d::Vec2>& poly1,
		const std::vector<cocos2d::Vec2>& poly2);

	inline std::vector<cocos2d::Vec2> polygonsIntersectPoints(
		const std::vector<cocos2d::Vec2>& poly1,
		const std::vector<cocos2d::Vec2>& poly2);

	inline bool polygonsOverlap(
		const std::vector<cocos2d::Vec2>& poly1,
		const std::vector<cocos2d::Vec2>& poly2);

	inline float polygonMaxDist(const std::vector<cocos2d::Vec2>& poly);

	inline float polygonMaxDist(
		const std::vector<cocos2d::Vec2>& poly,
		cocos2d::Vec2& A,
		cocos2d::Vec2& B);

	inline cocos2d::Vec2 polygonCentroid(const std::vector<cocos2d::Vec2>& poly);

	inline std::vector<cocos2d::Vec2> makeConvexHull(
		const std::vector<cocos2d::Vec2>& poly);

	inline Circle enclosingCircle(const std::vector<cocos2d::Vec2>& points);

	inline Circle makeCircleOnePoint(
		const std::vector<cocos2d::Vec2>& points,
		size_t size,
		cocos2d::Vec2 p);

	inline Circle makeCircleTwoPoints(
		const std::vector<cocos2d::Vec2>& points,
		size_t size,
		cocos2d::Vec2 p,
		cocos2d::Vec2 q);

	inline Circle makeDiameterCircle(cocos2d::Vec2 a, cocos2d::Vec2 b);

	inline Circle makeCircumcircle(cocos2d::Vec2 a, cocos2d::Vec2 b, cocos2d::Vec2 c);

	inline Circle enclosingCircleNaive(std::vector<cocos2d::Vec2> points);

	inline float distToRayPlaneIntersect(
		cocos2d::Vec2 RayOrigin,
		cocos2d::Vec2 RayHeading,
		cocos2d::Vec2 PlanePoint,  //any point on the plane
		cocos2d::Vec2 PlaneNormal);

	enum span_type { plane_backside, plane_front, on_plane };
	inline span_type whereIsPoint(
		cocos2d::Vec2 point,
		cocos2d::Vec2 PointOnPlane, //any point on the plane
		cocos2d::Vec2 PlaneNormal);

	inline float rayCircleIntersect(
		cocos2d::Vec2 RayOrigin,
		cocos2d::Vec2 RayHeading,
		cocos2d::Vec2 CircleOrigin,
		float  radius,
		float& dist);

	inline bool rayCircleIntersect(
		cocos2d::Vec2 RayOrigin,
		cocos2d::Vec2 RayHeading,
		cocos2d::Vec2 CircleOrigin,
		float     radius);

	inline bool getTangentPoints(
		cocos2d::Vec2 C,
		float R,
		cocos2d::Vec2 P,
		cocos2d::Vec2& T1,
		cocos2d::Vec2& T2);

	inline float circlesIntersectArea(
		float x1, float y1, float r1,
		float x2, float y2, float r2);

	inline float circleArea(float radius);

	inline bool getSegmentCircleClosestIntersectPoint(
		cocos2d::Vec2 A,
		cocos2d::Vec2 B,
		cocos2d::Vec2 pos,
		float    radius,
		cocos2d::Vec2& IntersectionPoint);

	inline bool findTangents(cocos2d::Vec2 center, float radius,
		cocos2d::Vec2 external_point, cocos2d::Vec2& pt1, cocos2d::Vec2& pt2);

	inline int findTangentsInTwoCircle(
		cocos2d::Vec2 c1, float radius1, cocos2d::Vec2 c2, float radius2,
		cocos2d::Vec2& outer1_p1, cocos2d::Vec2& outer1_p2,
		cocos2d::Vec2& outer2_p1, cocos2d::Vec2& outer2_p2,
		cocos2d::Vec2& inner1_p1, cocos2d::Vec2& inner1_p2,
		cocos2d::Vec2& inner2_p1, cocos2d::Vec2& inner2_p2);
}


#include "Geometry.inl"