//------------------------------------------------------------------------------
//
// File Name:	Intersection2D.cpp
// Author(s):	Davdi Cohen (david.cohen)
// Project:		Project 7
// Course:		WANIC VGP2 2018-2019
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "Intersection2D.h"

// Components
#include "Transform.h"
#include "Physics.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Function Declarations:
//------------------------------------------------------------------------------

// Check whether a point and circle intersect.
// Params:
//  point = The point in the intersection.
//  circle = The circle in the intersection.
// Returns:
//   True if intersection, false otherwise.
bool PointCircleIntersection(const Vector2D& point, const Circle& circle)
{
	return point.DistanceSquared(circle.center) <= circle.radius * circle.radius;
}

// Check whether a point and rectangle intersect.
// Params:
//  point = The point in the intersection.
//  rect  = The rectangle in the intersection.
// Returns:
//   True if intersection, false otherwise.
bool PointRectangleIntersection(const Vector2D& point, const BoundingRectangle& rect)
{
	return point.x >= rect.left && point.x <= rect.right && point.y >= rect.bottom && point.y <= rect.top;
}

// Check whether two circles intersect.
// Params:
//  circle1 = The first circle.
//  circle2 = The second circle.
// Returns:
//   True if intersection, false otherwise.
bool CircleCircleIntersection(const Circle& circle1, const Circle& circle2)
{
	// Calculate the sum of all radii.
	float radiusSum = circle1.radius + circle2.radius;

	// Check if the two circles are close enough to be intersecting.
	return circle1.center.DistanceSquared(circle2.center) <= radiusSum * radiusSum;
}

// Check whether two rectangles intersect.
// Params:
//  rect1 = The first rectangle.
//	rect2 = The second rectangle.
// Returns:
//   True if intersection, false otherwise.
bool RectangleRectangleIntersection(const BoundingRectangle& rect1, const BoundingRectangle& rect2)
{
	return rect1.right >= rect2.left && rect1.left <= rect2.right && rect1.top >= rect2.bottom && rect1.bottom <= rect2.top;
}

// Check whether a rectangle and circle intersect.
// Params:
//  rect = The rectangle.
//  circle  = The circle.
// Returns:
//   True if intersection, false otherwise.
bool RectangleCircleIntersection(const BoundingRectangle& rect, const Circle& circle)
{
	// Calculate the point on the rectangle closest to the circle.
	Vector2D point = circle.center;

	if (point.x < rect.left)
		point.x = rect.left;
	if (point.x > rect.right)
		point.x = rect.right;
	if (point.y < rect.bottom)
		point.y = rect.bottom;
	if (point.y > rect.top)
		point.y = rect.top;

	// Check if the point is close enough to be intersecting the circle.
	return point.DistanceSquared(circle.center) <= circle.radius * circle.radius;
}

//------------------------------------------------------------------------------
