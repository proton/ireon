/**
 * @file common/world/mesh.h
 * Generic mesh class
 */

/* Copyright (C) 2005-2006 ireon.org developers council
 * $Id: mesh.h 864 2007-08-20 07:28:11Z mip $

 * See LICENSE for details
 */

#pragma once

#include "common/common_stdafx.h"
#include "common/log_mixin.h"
#include "exception"
#include "common/kd_tree.h"



namespace ireon
{
	namespace nav
	{

		using namespace ireon::kd_tree;

		const Coord COORD_EPSILON = 0.0001f;
		class EIndexOutRange: public std::exception {};

		
		struct Point
		{ 
			Point(): x(0),y(0),z(0) {};
			Point(const Coord& _x, const Coord& _y, const Coord& _z): x(_x),y(_y),z(_z) {};
			explicit Point(const Vector3& v): x(v.x),y(v.y),z(v.z) {};
			
			Point(const Point& rvl)
			{
				x = rvl.x; y = rvl.y; z = rvl.z;
			}
			Point& operator = (const Vector3& rvl)
			{
				x = rvl.x; y = rvl.y; z = rvl.z;
				return *this;
			}
			Point operator + (const Point& rvl)
			{
				Point res;
				res.x = x + rvl.x; res.y = y + rvl.y; res.z = z + rvl.z;
				return res;
			}
			Point operator + (const Point& rvl) const
			{
				Point res;
				res.x = x + rvl.x; res.y = y + rvl.y; res.z = z + rvl.z;
				return res;
			}
			Point operator - (const Point& rvl) const
			{
				Point res;
				res.x = x - rvl.x; res.y = y - rvl.y; res.z = z - rvl.z;
				return res;
			}
			inline Point& operator += ( const Point& rvl )
			{
				x += rvl.x;
				y += rvl.y;
				z += rvl.z;
				return *this;
			}

			inline Point& operator -= ( const Point& rvl )
			{
				x -= rvl.x;
				y -= rvl.y;
				z -= rvl.z;

				return *this;
			}

			Point operator * (float rvl)
			{
				Point res;
				res.x = x * rvl; res.y = y * rvl; res.z = z * rvl;
				return res;
			};

			inline Point operator * ( const Point& rhs)
			{
				return Point(
					x * rhs.x,
					y * rhs.y,
					z * rhs.z);
			}
			inline Point operator / ( const Point& rvl) const
			{
				return Point(
					x / rvl.x,
					y / rvl.y,
					z / rvl.z);
			}

			inline friend Point operator / ( const Coord fScalar, const Point& rkVector )
			{
				return Point(
					fScalar / rkVector.x,
					fScalar / rkVector.y,
					fScalar / rkVector.z);
			}

			inline bool operator < ( const Point& rvl ) const
			{
				if( x < rvl.x && y < rvl.y && z < rvl.z )
					return true;
				return false;
			}

			inline bool operator > ( const Point& rvl ) const
			{
				if( x > rvl.x && y > rvl.y && z > rvl.z )
					return true;
				return false;
			}

			inline bool operator < ( const Vector3& rvl ) const
			{
				if( x < rvl.x && y < rvl.y && z < rvl.z )
					return true;
				return false;
			}

			inline bool operator > ( const Vector3& rvl ) const
			{
				if( x > rvl.x && y > rvl.y && z > rvl.z )
					return true;
				return false;
			}

			inline bool operator == ( const Point& rvl ) const
			{
				return ( x.equals(rvl.x) && y.equals(rvl.y) && z.equals(rvl.z) );
			}

			inline Point operator - () const
			{
				return Point(-x, -y, -z);
			}

			inline Coord length () const
			{
				return (float)sqrt((float) (x * x + y * y + z * z ));;
			}

			inline Coord squaredLength () const
			{
				return x * x + y * y + z * z;
			}

			inline Point crossProduct( const Point& rvl ) const
			{
				return Point(
					y * rvl.z - z * rvl.y,
					z * rvl.x - x * rvl.z,
					x * rvl.y - y * rvl.x);
			}

			inline Point floor (int precision = 0) const
			{
				float p10 = pow(10.0f,precision);
				float floorx = floorf( (float)x * p10 + 0.5f)/p10 ;
				float floory = floorf( (float)y * p10 + 0.5f)/p10 ;
				float floorz = floorf( (float)z * p10 + 0.5f)/p10 ;

				return Point( Coord(floorx),Coord(floory),Coord(floorz) );
			}
			inline Coord dotProduct(const Point& rvl) const
			{
				return x * rvl.x + y * rvl.y + z * rvl.z;
			}

			inline Point midPoint( const Point& rvl ) const
			{
				return Point(
					( x + rvl.x ) * 0.5,
					( y + rvl.y ) * 0.5,
					( z + rvl.z ) * 0.5 );
			}
			inline Coord normalise()
			{
				Coord fLength = (float)sqrt((float) (x * x + y * y + z * z ));

				// Will also work for zero-sized vectors, but will change nothing
				if ( fLength > COORD_EPSILON )
				{
					Coord fInvLength = 1.0f / (float)fLength;
					x *= fInvLength;
					y *= fInvLength;
					z *= fInvLength;
					//floor(1);
				}

				return fLength;
			}
			inline Point normalisedCopy(void) const
			{
				Point ret = *this;
				ret.normalise();
				return ret;
			}
			inline bool directionEquals(const Point& rvl,
				const Radian& tolerance) const
			{
				Coord dot = dotProduct(rvl);
				Radian angle = Math::ACos(dot);

				return Math::Abs(angle.valueRadians()) <= tolerance.valueRadians();

			}
			inline Point abs() const
			{
				return Point(absx(x),absx(y),absx(z));
			}
			
			Vector3 toVector3() const {return Vector3((Real const)x,(float)y,(float)z);};
			Vector2 toVector2() const {return Vector2((float)x,(float)z);}
			String toText() const
			{
				char c[50];
				sprintf(c,"(%12.4f, %12.4f, %12.4f)",(float)x,(float)y,(float)z);
				String s(c);
				return s;
			}
				
			union
			{
				struct { Coord r, g, b; };
				struct { Coord cell[3]; };
				struct { Coord x,y,z; }; // iniatalise last
				
			};
			 
		};
		

		const Point POINT_EPSILON = Point(COORD_EPSILON,COORD_EPSILON,COORD_EPSILON);

		enum ElementType
		{
			ET_NONE,
			ET_VERTEX,
			ET_EDGE
		};

		struct PointIntersection: public Point
		{
			PointIntersection():Point(){};
			PointIntersection(Real _x, Real _y, Real _z):Point(_x,_y,_z), indexElement(255),elementType(ET_VERTEX){};
			explicit PointIntersection(const Vector3& v):Point(v), indexElement(255),elementType(ET_VERTEX){};
			explicit PointIntersection(const Point& v):Point(v), indexElement(255),elementType(ET_VERTEX){};

			PointIntersection& operator= (const Point& rvl)
			{
				x = rvl.x; y = rvl.y; z = rvl.z;
				return *this;
			}

			int indexElement;
			ElementType elementType;
		};


		class Line
		{
		public:
			Line():v0_(Vector3()),v1_(Vector3()) {};
			Line(const Vector3& v0, const Vector3& v1):v0_(v0),v1_(v1) {};
			Line(const Point& v0, const Point& v1):v0_(v0),v1_(v1) {};
			
			Point point(int n) const
			{
				if (n>=0 && n<2)
					if (n == 0)  return v0_;
					else return v1_;
				else throw EIndexOutRange();
			}
			String toText()const
			{
				char c[110];
				sprintf(c,"[ %s, %s ]",v0_.toText().c_str(), v1_.toText().c_str());
				String s(c);
				return s;
			}

		protected:

			Point v0_,v1_;
		};

		class DirectedLine: public Line
		{
		public:
			DirectedLine(const Vector3& v1, const Vector3& v2):
			  Line(v1,v2)
			  {
				  calculatePluckerCoeffs();
			  }
			DirectedLine(const Point& v1, const Point& v2):
			  Line(v1,v2)
			  {
				  calculatePluckerCoeffs();
			  }

			const Coord& operator[](int n) const { if (n>=0 && n<6) return L[n]; else throw EIndexOutRange();}
			
			// side-product   e-4 
			Coord side(const DirectedLine& p) const
			{
				return L[0]*p[4] + L[1]*p[5] + L[2]*p[3] + L[3]*p[2] + L[4]*p[0] + L[5]*p[1];
			}

		private:
			void calculatePluckerCoeffs()
			{
				L[0] = v0_.x *  v1_.y - v1_.x * v0_.y ;
				L[1] = v0_.x *  v1_.z - v1_.x * v0_.z ;
				L[2] = v0_.x - v1_.x ;
				L[3] = v0_.y * v1_.z  - v1_.y * v0_.z ;
				L[4] = v0_.z - v1_.z ;
				L[5] = v1_.y - v0_.y ;
			}
			Coord L[6]; // e-2

		};

		class Edge: public DirectedLine
		{
		public:
			Edge(const Vector3& v1, const Vector3& v2):	DirectedLine(v1,v2){};
			Edge(const Point& v1, const Point& v2):	DirectedLine(v1,v2){}
			
			int id(){}
		private:

		};
		
		
		class Mesh;
		struct Triangle 
		{ 
			friend Mesh;
			
			Triangle() 
			{
				cleanup();
			};
			
			Triangle(Point* vertex0, Point* vertex1, Point* vertex2)
			{
				cleanup(); vertex[0] = vertex0; vertex[1] = vertex1; vertex[2] = vertex2;  constructEdges(); 
			};
			
			Triangle(const Triangle& rvl)
			{
				cleanup(); vertex[0] = rvl.vertex[0]; vertex[1] = rvl.vertex[1]; vertex[2] = rvl.vertex[2]; constructEdges();
			}
			
			~Triangle()
			{ 
				destructEdges();
			}

			Triangle& operator= (const Triangle& rvl)
			{
				destructEdges();
				vertex[0] = rvl.vertex[0]; vertex[1] = rvl.vertex[1]; vertex[2] = rvl.vertex[2];
				neighbors[0] = rvl.neighbors[0]; neighbors[1] = rvl.neighbors[1]; neighbors[2] = rvl.neighbors[2];
				constructEdges();

				return *this;
			}
			void calculateRange( real& a_Pos1, real& a_Pos2, int a_Axis )
			{
					Point* pos1 = vertex[0];
					a_Pos1 = (float)pos1->cell[a_Axis], a_Pos2 = (float)pos1->cell[a_Axis];
					for ( int i = 1; i < 3; i++ )
					{
						Point* pos = vertex[i];
						if ((float)pos->cell[a_Axis] < a_Pos1) a_Pos1 = (float)pos->cell[a_Axis];
						if ((float)pos->cell[a_Axis] > a_Pos2) a_Pos2 = (float)pos->cell[a_Axis];
					}
			
			}

		bool PlaneBoxOverlap( Vector3& a_Normal, Vector3& a_Vert, Vector3& a_MaxBox )
		{
			Vector3 vmin, vmax;
			for( int q = 0; q < 3; q++ )
			{
				float v = a_Vert.val[q];
				if (a_Normal.val[q] > 0.0f)
				{
					vmin.val[q] = -a_MaxBox.val[q] - v;
					vmax.val[q] =  a_MaxBox.val[q] - v;
				}
				else
				{
					vmin.val[q] =  a_MaxBox.val[q] - v;
					vmax.val[q] = -a_MaxBox.val[q] - v;
				}
			}
			if (a_Normal.dotProduct(vmin) > 0.0f) return false;
			if (a_Normal.dotProduct(vmax) >= 0.0f) return true;
			return false;
		}
#define FINDMINMAX( x0, x1, x2, min, max ) \
	min = max = x0; if(x1<min) min=x1; if(x1>max) max=x1; if(x2<min) min=x2; if(x2>max) max=x2;
		// X-tests			
#define AXISTEST_X01( a, b, fa, fb )											\
	p0 = a * v0.val[1] - b * v0.val[2], p2 = a * v2.val[1] - b * v2.val[2]; \
	if (p0 < p2) { min = p0; max = p2;} else { min = p2; max = p0; }			\
	rad = fa * a_BoxHalfsize.val[1] + fb * a_BoxHalfsize.val[2];				\
	if (min > rad || max < -rad) return 0;
#define AXISTEST_X2( a, b, fa, fb )												\
	p0 = a * v0.val[1] - b * v0.val[2], p1 = a * v1.val[1] - b * v1.val[2];	\
	if (p0 < p1) { min = p0; max = p1; } else { min = p1; max = p0;}			\
	rad = fa * a_BoxHalfsize.val[1] + fb * a_BoxHalfsize.val[2];				\
	if(min>rad || max<-rad) return 0;
		// Y-tests
#define AXISTEST_Y02( a, b, fa, fb )											\
	p0 = -a * v0.val[0] + b * v0.val[2], p2 = -a * v2.val[0] + b * v2.val[2]; \
	if(p0 < p2) { min = p0; max = p2; } else { min = p2; max = p0; }			\
	rad = fa * a_BoxHalfsize.val[0] + fb * a_BoxHalfsize.val[2];				\
	if (min > rad || max < -rad) return 0;
#define AXISTEST_Y1( a, b, fa, fb )												\
	p0 = -a * v0.val[0] + b * v0.val[2], p1 = -a * v1.val[0] + b * v1.val[2]; \
	if (p0 < p1) { min = p0; max = p1; } else { min = p1; max = p0; }			\
	rad = fa * a_BoxHalfsize.val[0] + fb * a_BoxHalfsize.val[2];				\
	if (min > rad || max < -rad) return 0;
		// Z-tests
#define AXISTEST_Z12( a, b, fa, fb )											\
	p1 = a * v1.val[0] - b * v1.val[1], p2 = a * v2.val[0] - b * v2.val[1]; \
	if(p2 < p1) { min = p2; max = p1; } else { min = p1; max = p2; }			\
	rad = fa * a_BoxHalfsize.val[0] + fb * a_BoxHalfsize.val[1];				\
	if (min > rad || max < -rad) return 0;
#define AXISTEST_Z0( a, b, fa, fb )												\
	p0 = a * v0.val[0] - b * v0.val[1], p1 = a * v1.val[0] - b * v1.val[1];	\
	if(p0 < p1) { min = p0; max = p1; } else { min = p1; max = p0; }			\
	rad = fa * a_BoxHalfsize.val[0] + fb * a_BoxHalfsize.val[1];				\
	if (min > rad || max < -rad) return 0;

		bool intersectTriBox( Vector3& a_BoxCentre, Vector3& a_BoxHalfsize, Vector3& a_V0, Vector3& a_V1, Vector3& a_V2 )
		{
			Vector3 v0, v1, v2, normal, e0, e1, e2;
			float min, max, p0, p1, p2, rad, fex, fey, fez;
			v0 = a_V0 - a_BoxCentre;
			v1 = a_V1 - a_BoxCentre;
			v2 = a_V2 - a_BoxCentre;
			e0 = v1 - v0, e1 = v2 - v1, e2 = v0 - v2;
			fex = fabsf( e0.val[0] );
			fey = fabsf( e0.val[1] );
			fez = fabsf( e0.val[2] );
			AXISTEST_X01( e0.val[2], e0.val[1], fez, fey );
			AXISTEST_Y02( e0.val[2], e0.val[0], fez, fex );
			AXISTEST_Z12( e0.val[1], e0.val[0], fey, fex );
			fex = fabsf( e1.val[0] );
			fey = fabsf( e1.val[1] );
			fez = fabsf( e1.val[2] );
			AXISTEST_X01( e1.val[2], e1.val[1], fez, fey );
			AXISTEST_Y02( e1.val[2], e1.val[0], fez, fex );
			AXISTEST_Z0 ( e1.val[1], e1.val[0], fey, fex );
			fex = fabsf( e2.val[0] );
			fey = fabsf( e2.val[1] );
			fez = fabsf( e2.val[2] );
			AXISTEST_X2 ( e2.val[2], e2.val[1], fez, fey );
			AXISTEST_Y1 ( e2.val[2], e2.val[0], fez, fex );
			AXISTEST_Z12( e2.val[1], e2.val[0], fey, fex );
			FINDMINMAX( v0.val[0], v1.val[0], v2.val[0], min, max );
			if (min > a_BoxHalfsize.val[0] || max < -a_BoxHalfsize.val[0]) return false;
			FINDMINMAX( v0.val[1], v1.val[1], v2.val[1], min, max );
			if (min > a_BoxHalfsize.val[1] || max < -a_BoxHalfsize.val[1]) return false;
			FINDMINMAX( v0.val[2], v1.val[2], v2.val[2], min, max );
			if (min > a_BoxHalfsize.val[2] || max < -a_BoxHalfsize.val[2]) return false;
			normal = e0.crossProduct( e1 );
			if (!PlaneBoxOverlap( normal, v0, a_BoxHalfsize )) return false;
			return true;
		}

			bool intersectBox( aabb& a_Box )
			{
				return intersectTriBox( a_Box.getPos() + a_Box.getSize() * 0.5f, a_Box.getSize() * 0.5f, 
					vertex[0]->toVector3(), vertex[1]->toVector3(), vertex[2]->toVector3() );
			}
			

			//Returns the vertex with index n
			Point* vertice(int n) const { if (n>=0 && n<3) return vertex[n]; else throw EIndexOutRange();}
			void setVertice(int n, Point* v) {if (n>=0 && n<3) vertex[n] = v; else throw EIndexOutRange();}
			//Returns the vertex between edge m and n
			//Vector3* vertex(int n, int m){ ( (n>=0 && n<3) && (m>=0 && m<3) )? return vertex[n]: throw EIndexOutRange;}
			
			Edge* edge(int n) const { if (n>=0 && n<3) return edges[n]; throw EIndexOutRange();} 
		
			void setNeighbor(int n, Triangle* neighbor){ (n>=0 && n<3)? neighbors[n] = neighbor: throw EIndexOutRange(); };
			Triangle* getNeighbor(int n) const  { if (n>=0 && n<3) return neighbors[n]; else throw EIndexOutRange();};

			String toText() const
			{
				char c[340];
				sprintf(c,"< %s, %s, %s >",vertex[0]->toText().c_str(), vertex[1]->toText().c_str(),vertex[2]->toText().c_str());
				String s(c);
				return s;
			}
			//uint vertexIndex[3];
			uint number;

			// triangle
			struct
			{
				Vector3 m_N;							// 12
				Point* m_Vertex[3];					// 12
				real m_U, m_V;							// 8
				real nu, nv, nd;						// 12
				int k;									// 4
				real bnu, bnv;							// 8
				real cnu, cnv;							// 8, total: 64
			};

		protected:
			inline void constructEdges()
			{
				edges[0] = new Edge(*vertex[0], *vertex[1]);
				edges[1] = new Edge(*vertex[1], *vertex[2]);
				edges[2] = new Edge(*vertex[2], *vertex[0]);
			}
			inline void destructEdges()
			{
				for (int i = 0; i<3; i++) delete edges[i];
			}
			
			inline void cleanup()
			{
				memset(vertex,NULL,3*sizeof(vertex[0]));memset(neighbors,NULL,3*sizeof(neighbors[0]));memset(edges,NULL,3*sizeof(edges[0]));
			}
			Point* vertex[3];
			Edge* edges[3];
			Triangle * neighbors[3];

			
			
			
		};

		class ManualTriangle: public Triangle
		{
		public:
			ManualTriangle(): Triangle() {};

			ManualTriangle(const Vector3& vertex0, const Vector3& vertex1, const Vector3& vertex2) :Triangle() 
			{
				vertex[0] = new Point(vertex0); vertex[1] = new Point(vertex1); vertex[2] = new Point(vertex2);
				constructEdges();
			};
			ManualTriangle(const Point& vertex0, const Point& vertex1, const Point& vertex2): Triangle()
			{
				vertex[0] = new Point(vertex0); vertex[1] = new Point(vertex1); vertex[2] = new Point(vertex2);
				constructEdges();
			};
			ManualTriangle(const ManualTriangle& rvl) :Triangle()
			{
				vertex[0] = new Point(*(rvl.vertex[0])); vertex[1] = new Point(*(rvl.vertex[1])); vertex[2] = new Point(*(rvl.vertex[2]));
				constructEdges();
			}
			
			~ManualTriangle()
			{ 
				for (int i = 0; i<3; i++) delete vertex[i];
			}

			ManualTriangle& operator= (const Triangle& rvl)
			{
				for (int i = 0; i<3; i++) delete vertex[i];
				destructEdges();
				vertex[0] = new Point(*(rvl.vertice(0))); vertex[1] = new Point(*(rvl.vertice(1))); vertex[2] = new Point(*(rvl.vertice(2))); 
				neighbors[0] = rvl.getNeighbor(0); neighbors[1] = rvl.getNeighbor(1); neighbors[2] = rvl.getNeighbor(2);
				constructEdges();
				
				return *this;
			}
			
			ManualTriangle& operator= (const ManualTriangle& rvl)
			{
				for (int i = 0; i<3; i++) delete vertex[i];
				destructEdges();
				vertex[0] = new Point(*(rvl.vertice(0))); vertex[1] = new Point(*(rvl.vertice(1))); vertex[2] = new Point(*(rvl.vertice(2))); 
				neighbors[0] = rvl.getNeighbor(0); neighbors[1] = rvl.getNeighbor(1); neighbors[2] = rvl.getNeighbor(2);
				constructEdges();

				return *this;
			}

		};

		enum LineTypeIntersection
		{
			lt_unknown_intersection,
			lt_proper_intersection,
			lt_no_intersection,
			lt_intersection_at_vertex,
			lt_intersection_on_edge,
			lt_coplanar_and_no_intersection,
			lt_coplanar_and_intersection_at_vertex,
			lt_intersection_through_two_edges,
			lt_intersection_contains_edge,
			lt_intersection_through_one_vertex_and_edge
		};
		enum LineSegmentTypeIntersection
		{
			lst_unknown_intersection,
			lst_no_intersection,
			lst_intersection_at_vertex,
			lst_intersection_on_edge,
			lst_triangle_contains_segment
		};
		Point vec3_inter_lines(const DirectedLine& Line1, const DirectedLine& Line2);

		Point getIntersectionPointLineWithSegment(const DirectedLine& Line1, const DirectedLine& Line2);
		
		PointIntersection getPointOnDirection(const DirectedLine& segment, const PointIntersection& p1, const PointIntersection& p2);

		LineSegmentTypeIntersection IntersectionCoplonarLineSegmentWithTriangle(PointIntersection& intersectedPoint, const DirectedLine& L,const Triangle* T);

		LineTypeIntersection TestIntersectionCoplonarLineWithTriangle(PointIntersection& p1, PointIntersection& p2, const DirectedLine& L,const Triangle* T);

		LineTypeIntersection TestIntersectionLineWithTriangle(PointIntersection& p1, PointIntersection& p2, const DirectedLine& L,const Triangle* T);

		bool isPointInsideSegment(const PointIntersection& point, const DirectedLine& segment);
		
		Vector3 Line_to_Line(const DirectedLine& L1,const DirectedLine& L2);

		
struct Position;

class Mesh: private CLogMixin
{
public:
	class ETriangleNotFound {};
	class EIntersectionNotFound {};

	explicit Mesh(const char *filename);
	~Mesh();

	

	Coord getMinX() { return minX_; }
	Coord getMaxX() { return maxX_; }
	Coord getMinY() { return minY_; }
	Coord getMaxY() { return maxY_; }

	/// returns pointer to first found triangle for coordinates or throws exception if no triangle was found
	/// warning: slow
	Triangle * getTriangleForPoint(const Point& p );
	Triangle * getTriangleInKDTree(const Point& p);

	Point GetPointOnMesh(const Point& p );
	Point getHeightAt(const Point& p ,const Triangle* T);
	// compute height of 2d point
	/*Real getHeightAt(const Vector2& pos);*/
	
	Point getVectorProjectionOnPlane(const Point& begin, const Point& end,const Triangle* curtri );
	

	
	/// returns destination point for straight walk on mesh with given params
	/// if destination point is not directly reachable returns farthest reachable point of walk
	Position walk(Position startPos, Point speed, short time);


	// get array of vertices
	const Point* getVertexBuffer(){return vertexBuf_;};
	uint getVertexBufferSize(){return vertexBufSize_;};
	
	const Triangle* getTriangleBuffer(){return trianglesBuf_;};
	uint getTriangleBufferSize(){return trianglesBufSize_;};
private:
	// no copy or assignment
	Mesh(const Mesh & rhs);
	Mesh & operator = (const Mesh & rhs);

	/// head of array of vertices
	Point * vertexBuf_;
	/// vertex count
	uint vertexBufSize_;
	/// head of array of vertice submesh
	Point * vertexSubmeshBuf_[16];
	/// count submesh
	uint submeshCount_;

	/// head of array of triangles (faces)
	Triangle * trianglesBuf_;
	/// head of array of triangles submesh
	Triangle * trianglesSubmeshBuf_[16];
	/// faces count
	uint trianglesBufSize_;

	/// mesh bounds
	Coord minX_, minY_, maxX_, maxY_, minZ_, maxZ_;

	KdTree<Triangle>* tkdTree;

	/// recursively walks on mesh until destination point is reached or destination could not be reached 
	/// farthest reachable point is set as destination
	/// throws EIntersectionNotFound in case of internal error. if caught - call zak :)
	void traceWalk(const Point & startPoint3D, const Point & movementVector2D, Triangle *& T , Point& endPos );
	/*bool pointIsInTriangle( Vector2 & pt, Triangle * curtri );*/
	//void getLinesIntersection(Coord x11, Coord y11, Coord x12, Coord y12, Coord x21, Coord y21, Coord x22, Coord y22, Coord &x, Coord &y);
};

struct Position
{ 
	Position(Coord _x, Coord _y, Coord _z, Triangle *_tri) : x(_x), y(_y), z(_z), tri(_tri) {};
	Position(const Vector3& vec,  Triangle *_tri) : x(vec.x), y(vec.y), z(vec.z), tri(_tri) {};
	Position(const Point& p,  Triangle *_tri) : x(p.x), y(p.y), z(p.z), tri(_tri) {};

	Point getPoint3D() const;

	Coord x,y,z; Triangle *tri;

};

	}
}