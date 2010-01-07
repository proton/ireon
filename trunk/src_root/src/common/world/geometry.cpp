/**
  * @file geometry.cpp
  * World geometry
  */
 
 /* Copyright (C) 2005 ireon.org developers council
  * $Id: geometry.cpp 715 2006-09-10 20:12:50Z mip $
 
  *  See LICENSE for details
  */
#include "stdafx.h"
#include "common/world/geometry.h"
 
 Real CSegment::squareDist(const Vector2& point, Real* t)
 {
 	Vector2 diff = point - m_p1;
 	Vector2 dir = m_p2 - m_p1;
 	Real rT  = diff.dotProduct(dir);
 
 	if(rT<=0.0f)
 	{
 		rT = 0.0f;
 	}
 	else
 	{
 		Real sqrLen= dir.squaredLength();
 		if(rT >= sqrLen)
 		{
 			rT = 1.0f;
 			diff -= dir;
 		}
 		else
 		{
 			rT /= sqrLen;
 			diff -= rT * dir;
 		}
 	}
 
 	if( t )
 		*t = rT;
 
 	return diff.squaredLength();
 };
 
 bool CSegment::intersects(const CSegment &seg)
 {
 	Real xb1 = m_p1.x;
 	Real xb2 = m_p2.x;
 	Real yb1 = m_p1.y;
 	Real yb2 = m_p2.y;
 	Real xb3 = seg.m_p1.x;
 	Real xb4 = seg.m_p2.x;
 	Real yb3 = seg.m_p1.y;
 	Real yb4 = seg.m_p2.y;
 	Real b;
 	if( xb1 > xb2 )
 	{
 		b = xb1;
 		xb1 = xb2;
 		xb2 = b;
 	}
 	if( yb1 > yb2 )
 	{
 		b = yb1;
 		yb1 = yb2;
 		yb2 = b;
 	}
 	if( xb3 > xb4 )
 	{
 		b = xb3;
 		xb3 = xb4;
 		xb4 = b;
 	}
 	if( yb3 > yb4 )
 	{
 		b = yb3;
 		yb3 = yb4;
 		yb4 = b;
 	}
 	if( xb2 >= xb3 && xb4 >= xb1 && yb2 >= yb3 && yb4 >= yb1 )
 	{
 		xb1 = m_p2.x - m_p1.x;
 		yb1 = m_p2.y - m_p1.y;
 		xb2 = seg.m_p2.x - m_p1.x;
 		yb2 = seg.m_p2.y - m_p1.y;
 		xb3 = seg.m_p1.x - m_p1.x;
 		yb3 = seg.m_p1.y - m_p1.y;
 		if( (xb1*yb2-xb2*yb1)*(xb1*yb3-xb3*yb1) > 0 )
 			return false;
 		xb1 = seg.m_p1.x - seg.m_p2.x;
 		yb1 = seg.m_p1.y - seg.m_p2.y;
 		xb2 = m_p2.x - seg.m_p2.x;
 		yb2 = m_p2.y - seg.m_p2.y;
 		xb3 = m_p1.x - seg.m_p2.x;
 		yb3 = m_p1.y - seg.m_p2.y;
 		if( (xb1*yb2-xb2*yb1)*(xb1*yb3-xb3*yb1) > 0 )
 			return false;
 		return true;
 	}
 	return false;
 };