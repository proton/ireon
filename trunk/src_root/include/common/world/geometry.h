/**
  * @file geometry.h
  * World geometry
  */
 
 /* Copyright (C) 2005 ireon.org developers council
  * $Id: geometry.h 693 2006-08-31 08:02:35Z zak $
 
  *  See LICENSE for details
  */
 
 #ifndef _GEOMETRY_H
 #define _GEOMETRY_H
 
 class CSegment
 {
 public:
 	CSegment(Vector2 p1, Vector2 p2):m_p1(p1),m_p2(p2) {update();}
 	CSegment(const CSegment& s) {m_p1 = s.m_p1; m_p2 = s.m_p2; update();}
 
 	CSegment operator=(const CSegment& s) {m_p1 = s.m_p1; m_p2 = s.m_p2; update(); return *this;}
 
 	void setP1(Vector2 p) {m_p1 = p; update();}
 	void setP2(Vector2 p) {m_p2 = p; update();}
 	const Vector2& p1() {return m_p1;}
 	const Vector2& p2() {return m_p2;}
 
 	const Vector2& getNormal() {return m_normal;}
 
 	/* Squared distance from point to segment
 	 * p1 + (p2-p1)*t = projection of point to segment
 	 * if it isn't projection t = 0 or 1
 	*/
 	Real squareDist(const Vector2& point, Real* t);
 
 	/* Function finds if two segments are intersected
 	*/
 	bool intersects(const CSegment& seg);
 protected:
 
 	/// Update normal
 	void update() {m_normal = (m_p2-m_p1).perpendicular().normalisedCopy(); }
 
 protected:
 	Vector2 m_p1;
 	Vector2 m_p2;
 
 	Vector2 m_normal;
 };
 
 #endif