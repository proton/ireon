/**
 * @file common/world/mesh.cpp
 * Generic mesh class 
 */

/* Copyright (C) 2005-2006 ireon.org developers council
 * $Id: mesh.cpp 864 2007-08-20 07:28:11Z mip $

 * See LICENSE for details
 */

#include "common/common_stdafx.h"
#include "common/world/mesh.h"
#include "common/xsd/navigation_mesh_xsd.h"
#include <memory>
#include <map>
#include <vector>
#include "common/OgreTypes/OgreMath.h"
#include "common/OgreTypes/OgreMatrix4.h"
using namespace ireon::nav;

/// navigation mesh oriented in co-ordinates XYZ
/// XY - horizontal plane
/// Z -  vertical axis
//////////////////////////////////////////////////////////////////////////
///              ^  Z
///              |
///              |
///              |
///              |
///              |
///               -----------> X
///             /
///            /
///           /
///          /
///         Y

/// TODO: (CHECK THIS!)
// все коментарии об точности не явля.тся верными
//#include "math.h"





Vector3 vec3_proj_on_line(Vector3 q, Vector3 l1, Vector3 l2)
{
	Vector3  lq;

	lq = q - l1;
	return (l1 + l2 * lq.dotProduct(l2) );
}




/*
Calculate the line segment PaPb that is the shortest route between
two lines P1P2 and P3P4. Calculate also the values of mua and mub where
Pa = P1 + mua (P2 - P1)
Pb = P3 + mub (P4 - P3)
Return FALSE if no solution exists.

in e-1
*/
bool LineLineIntersect(const Point& p1,const Point& p2,const Point& p3,const Point& p4,Point *pa,Point *pb,
					  double *mua, double *mub)
{
	Point p13,p43,p21;
	Coord d1343,d4321,d1321,d4343,d2121;
	Coord numera,numerb,denom;
	// e-1
	p13 = p1 - p3;
	p43 = p4 - p3;
	
	if (p43.squaredLength()  < COORD_EPSILON )
		return false;
	
	p21 = p2 - p1;
	if (p21.squaredLength() < COORD_EPSILON )
		return false;
	
	// e-1
	d1343 = p13.dotProduct(p43); //e
	d4321 = p43.dotProduct(p21); //b
	d1321 = p13.dotProduct(p21); //d
	d4343 = p43.dotProduct(p43); //c
	d2121 = p21.dotProduct(p21); //a
	// e-2


	denom = d2121 * d4343 - d4321 * d4321;
	// e-4

	if (absx(denom) < COORD_EPSILON)
		return(false);
	numera = d1343 * d4321 - d1321 * d4343;
	// e-4

	*mua = (double)numera / (double)denom;
	// e-8
	//D = a*c - b*b;
	//sc = (b*e - c*d) / D;
	//tc = (a*e - b*d) / D;
	numerb = d2121 * d1343 - d4321 * d1321;
	// e-4
	*mub =  numerb / denom;    // old = (d1343 + d4321 * (*mua)) / d4343;
	// e-8

	
	pa->x = p1.x + Coord(*mua * (double)p21.x);
	pa->y = p1.y + Coord(*mua * (double)p21.y);
	pa->z = p1.z + Coord(*mua * (double)p21.z);
	pb->x = p3.x + Coord(*mub * (double)p43.x);
	pb->y = p3.y + Coord(*mub * (double)p43.y);
	pb->z = p3.z + Coord(*mub * (double)p43.z);
	*pa = pa->floor(1);
	*pb = pb->floor(1);

	// e-1
	return(true);
}

Vector3 vec3_closest_lines(Vector3 l1, Vector3 v1, Vector3 l2, Vector3 v2)
{
	Real d12 = v1.dotProduct(v2);
	Real t1;

	if (d12 == 1.0)
		return (l1);
	t1 = (l2 - l1).dotProduct(v1 - v2 * d12)
		/ (1.0f - d12 * d12);
	return (l1 + v1 * t1);
} 


Point ireon::nav::vec3_inter_lines(const DirectedLine& Line1, const DirectedLine& Line2)
{
	Vector3 l1 = Line1.point(0).toVector3();
	Vector3 v1 = Line1.point(1).toVector3();
	Vector3 l2 = Line2.point(0).toVector3();
	Vector3 v2 = Line2.point(1).toVector3();

	Vector3 vec1= vec3_closest_lines(l1, v1, l2, v2);
	Vector3 vec2= vec3_closest_lines(l2, v2, l1, v1);
	Vector3 res = vec1.midPoint(vec2);
	return Point(res);
} 

Vector3 ireon::nav::Line_to_Line(const DirectedLine& L1,const DirectedLine& L2)
{
	Vector3   u = L1.point(1).toVector3() - L1.point(0).toVector3();
	Vector3   v = L2.point(1).toVector3() - L2.point(0).toVector3();
	Vector3   w = L1.point(0).toVector3() - L2.point(0).toVector3();
	float    a = u.dotProduct(u);        // always >= 0
	float    b = u.dotProduct(v);
	float    c = v.dotProduct(v);        // always >= 0
	float    d = u.dotProduct(w);
	float    e = v.dotProduct(w);
	float    D = a*c - b*b;       // always >= 0
	float    sc, tc;

	// compute the line parameters of the two closest points
	if (D < 0.00000001) {         // the lines are almost parallel
		sc = 0.0;
		tc = (b>c ? d/b : e/c);   // use the largest denominator
	}
	else {
		sc = (b*e - c*d) / D;
		tc = (a*e - b*d) / D;
	}

	// get the difference of the two closest points
	//Vector3   dP = w + (sc * u) - (tc * v);  // = L1(sc) - L2(tc)

	Vector3 p1 = L1.point(0).toVector3() + sc * u;
	Vector3 p2 = L2.point(0).toVector3() + tc * v;
	
	return p1.midPoint(p2);
	//return norm(dP);   // return the closest distance
}


Point ireon::nav::getIntersectionPointLineWithSegment(const DirectedLine& Line1, const DirectedLine& Line2) 
{
	Point a,b;
	double ma,mb;
	if (!LineLineIntersect(Line1.point(0),Line1.point(1),Line2.point(0),Line2.point(1),&a,&b,&ma,&mb) )
		throw;
	Point res = a;// a.midPoint(b).floor(1);
	return res;
}

bool ireon::nav::isPointInsideSegment(const PointIntersection& point, const DirectedLine& segment)
{
	// dot = in e-1 out e-2
	return (segment.point(0)-point).dotProduct(segment.point(1)-point) < 0.01f;//доверяем двум цифрам;

}

//bool dirEquals(const Vector3& lhs,const Vector3& rhs,
//							const Radian& tolerance)
//{
//	Real dot = lhs.dotProduct(rhs);
//	Radian angle = Math::ACos(dot);
//
//	return Math::Abs(angle.valueRadians()) <= tolerance.valueRadians();
//
//}

// предполагается что (p1,p2) и segment коллинеарны
// вероятна ошибка если точки пересечения распологаются до обеих концов отрезка
// это может произойти если произошло неправильное определение треугольника изза погрешности
// временное решение - не давать приблизиться к границам треугольника на некотором расстоянии
PointIntersection ireon::nav::getPointOnDirection(const DirectedLine& segment, const PointIntersection& p1, const PointIntersection& p2)
{
	PointIntersection res;
	Point segmentA = segment.point(0);
	Point segmentB = segment.point(1);

	//проверка крайних случаев
	// вырожденый отрезок
	if ( segmentB == segmentA)
		throw;
	// нельзя выбрать из двух одинаковых точек - возвразаем первую попавшуюся
	if (p1 == p2)
		return p1;
	// e-1
	Point vecSegment = segmentB-segmentA;
	Point vecP = p1 - p2;
	// нормализуем вектора - иначе происходило неправильное определение их соноправленности - т.к. скалярное произведение их не было равно 1 
	vecSegment.normalise();
	vecP.normalise();
	//выбираем точку которая является концом соноправленого отрезка, построенным на основе двух точек
	if (vecSegment.directionEquals(vecP,Radian(1)))
		res = p1;
	else if (vecSegment.directionEquals(-vecP,Radian(1)))
		res = p2;
	else 
		// отрезки не коллинеарны - что вы хотите от функции ?
		throw;
	//усекаем точку до ближайшего конца отрезка
	if (!isPointInsideSegment(res,segment))
	{
		//if (vecSegment.directionEquals(segmentB - res,Radian(1)))
		//	throw;
		if ( (res - segmentA).squaredLength() < (res - segmentB).squaredLength())
			res = segmentA;
		else
		{
			res = segmentB;
			
		}
		res.elementType = ET_NONE;
		res.indexElement = 255;
	}

	return res;

};

// находит и возвращает тип пересечения и точку пересечения направленого вектора с треугольником
// точка пересечения валидна только если вектор и треугольник лежат на одной плоскости
LineSegmentTypeIntersection ireon::nav::IntersectionCoplonarLineSegmentWithTriangle(PointIntersection& intersectedPoint, const DirectedLine& L,const Triangle* T)
{

	PointIntersection p1,p2;
	LineTypeIntersection res = TestIntersectionCoplonarLineWithTriangle(p1, p2, L, T);
	
	if (res == lt_unknown_intersection)
		return lst_unknown_intersection;

	if (res == lt_coplanar_and_no_intersection)
		return lst_no_intersection;



	intersectedPoint = getPointOnDirection(L, p1 , p2);
	
			
	if (res == lt_coplanar_and_intersection_at_vertex)
		return lst_intersection_at_vertex;

	if (res == lt_intersection_contains_edge)
	{
		// если не касаемся вершин
		// предпологается что p1, p2 идут в clockwise порядке
		if (intersectedPoint.toVector3() != p1.toVector3() && intersectedPoint.toVector3() != p2.toVector3())
		{
			intersectedPoint.elementType = ET_EDGE;
			if (p1.indexElement == 0 && p2.indexElement == 2)
				intersectedPoint.indexElement = p2.indexElement; //ребро следующее за вершиной
			else if (p2.indexElement == 0 && p1.indexElement == 2)
				intersectedPoint.indexElement = p1.indexElement; //ребро следующее за вершиной
			else if (p1.indexElement > p2.indexElement )
				intersectedPoint.indexElement = p2.indexElement; //ребро следующее за вершиной
			else			
				intersectedPoint.indexElement = p1.indexElement; //ребро следующее за вершиной

			return lst_intersection_on_edge;
		}
	
		// иначе пересечение в вершине , не меняем ничего
		return lst_intersection_at_vertex;
		
	}

	if (res == lt_coplanar_and_intersection_at_vertex || res == lt_intersection_through_two_edges  || res == lt_intersection_through_one_vertex_and_edge)
	{
		if (intersectedPoint.elementType == ET_NONE)
			return lst_triangle_contains_segment;
		if (intersectedPoint.elementType == ET_EDGE)
			return lst_intersection_on_edge;
		if (intersectedPoint.elementType == ET_VERTEX)
			return lst_intersection_at_vertex;
	}

	return lst_unknown_intersection;
}


// http://www.loria.fr/~lazard//ARC-Visi3D/Pant-project/files/Line_Triangle.html

//находит и возвращает тип пересечения линии с треугольником лежащими в одной плоскости
LineTypeIntersection ireon::nav::TestIntersectionCoplonarLineWithTriangle(PointIntersection& p1, PointIntersection& p2, const DirectedLine& L,const Triangle* T)
{
	// Four trial points
	Point p[4];
	p[0] = Point(0,0,0);
	p[1] = Point(0,1,0);
	p[2] = Point(0,0,1);
	p[3] = Point(1,0,0);
	Point x;	// The selected point : This point does not lie in the same plane
	ManualTriangle T1;	// Trial triangle constructed to test coplanarity
	int intersectedEdge[3] = {0,0,0};
	int intersectedVertex[3] = {0,0,0};

	int i,intersection=0;
	Coord s1,s2,s3;
	
	for(i=0;i<4;i++)
	{
		T1 = Triangle(T->vertice(0),T->vertice(1),&p[i]);
		// in e-1  out e-4
		s1 = L.side(*(T1.edge(0)));
		s2 = L.side(*(T1.edge(1)));
		s3 = L.side(*(T1.edge(2)));

		if(!(s1.equals(0) && s2.equals(0) && s3.equals(0)))
			break;
	}
	
	Point outP= (*(T->vertice(2)) - *(T->vertice(0))).crossProduct(*(T->vertice(1)) - *(T->vertice(0)));
	
	outP = outP.normalisedCopy() * 10;
	//x = *(T->vertice(0)) + outP;
	//x = p[i]; // This plane is not in plane containing the Triangle T  and the Line L

	for(i=0;i<3;i++)
	{
		x = (T->edge(i)->point(0).midPoint(T->edge(i)->point(1)) ) + outP;
		T1 = ManualTriangle(T->edge(i)->point(0),T->edge(i)->point(1),x);
		// in e-1  out e-4
		s2 = L.side(*(T1.edge(1)));
		s3 = L.side(*(T1.edge(2)));

		//http://www.loria.fr/~lazard//ARC-Visi3D/Pant-project/files/Line_Segment_Line.html

		// line does not intersect the line segment 
		if (s2*s3 < -COORD_EPSILON ) {};
		// line passes through the line segment 
		if (s2*s3 > COORD_EPSILON) 
		{ 
			intersectedEdge[i]++;
			intersection++;
		}
		// line contains the line segment 
		if (absx(s2) <= COORD_EPSILON && absx(s3) <= COORD_EPSILON) 
		{  
			intersectedEdge[i] +=2;
			intersection += 2;
			break;
		}
		
		// line passes through an end point of the segment 
		
		// through end point
		if( absx(s2) <= COORD_EPSILON )
		{  
			//vertex[(i+1) -3*((i+1)/3)]++; //????? vertex[0] ?
			intersectedVertex[(i+1)%3]++;
			intersection++;
		}
		// through start point
		if( absx(s3) <= COORD_EPSILON )
		{
			intersectedVertex[i]++;
			intersection++;
		}
	} 

	if (intersection==0)
		return lt_coplanar_and_no_intersection;


	for (i = 0; i<3; i++)
		if (intersectedEdge[i]==2)
		{
			p1 = getIntersectionPointLineWithSegment(L,*(T->edge((i+2)%3)) );
			p1.indexElement =  (i)%3;
			p1.elementType = ET_VERTEX;
			p2 = getIntersectionPointLineWithSegment(L,*(T->edge((i+1)%3)) );
			p2.indexElement = (i+1)%3;
			p2.elementType = ET_VERTEX;
			return lt_intersection_contains_edge;
		}
		
	for (i = 0; i<3; i++)
		if (intersectedEdge[i]==0 && intersectedEdge[(i+1)%3] && intersectedEdge[(i+2)%3])
		{
			p1 = getIntersectionPointLineWithSegment(L,*(T->edge((i+1)%3)));
			p1.indexElement =  (i+1)%3;
			p1.elementType = ET_EDGE;
			p2 = getIntersectionPointLineWithSegment(L,*(T->edge((i+2)%3)));
			p2.indexElement =  (i+2)%3;
			p2.elementType = ET_EDGE;
			return lt_intersection_through_two_edges;
		}
			 


	for (i = 0; i<3; i++)
		if (intersectedEdge[i]==1 && intersectedVertex[(i+2)%3])
		{
			p1 = getIntersectionPointLineWithSegment(L,*(T->edge(i)));
			p1.indexElement =  i;
			p1.elementType = ET_EDGE;
			p2 = * T->vertice((i+2)%3);
			p2.indexElement =  (i+2)%3;
			p2.elementType = ET_VERTEX;
			return lt_intersection_through_one_vertex_and_edge;
		}

	for (i = 0; i<3; i++)
		if (intersectedEdge[i]==0 && intersectedVertex[(i+2)%3])
		{
			p1 = p2 = PointIntersection(* T->vertice((i+2)%3));
			p1.indexElement =  (i+2)%3;
			p1.elementType = ET_VERTEX;
			return lt_coplanar_and_intersection_at_vertex;
		}	

	return lt_unknown_intersection;
}

// находит и возвращает тип пересечения линии и треугольника в 3-х мерном пространстве
LineTypeIntersection ireon::nav::TestIntersectionLineWithTriangle(PointIntersection& p1, PointIntersection& p2, const DirectedLine& L,const Triangle* T)
{
	
	DirectedLine e1(T->vertice(0)->toVector3(), T->vertice(1)->toVector3());
	DirectedLine e2(T->vertice(1)->toVector3(), T->vertice(2)->toVector3());
	DirectedLine e3(T->vertice(2)->toVector3(), T->vertice(0)->toVector3());

	Real S1 = L.side(e1);
	Real S2 = L.side(e2);
	Real S3 = L.side(e3);
	
	//The triangle and the lines is not coplanar
	if ((S1>0 && S2>0 && S3>0) || (S1<0 && S2<0 && S3<0))
		return lt_proper_intersection;
	//S1, S2 and S3 don't all have the same sign
	if (!( (S1>0 && S2>0 && S3>0) || (S1<0 && S2<0 && S3<0) ))
		return lt_no_intersection;
	//Two out of S1, S2, S3 are equal to zero
	if ( (!S1 && !S2 && S3) || (S1 && !S2 && !S3) || (!S1 && S2 && !S3) )
		return lt_intersection_at_vertex;
	//One of the three S1,S2,S3 is zero and the other two have same sign (>0 or <0)
	if ( (!S1 && (S2 * S3 > 0) ) || (!S2 && (S1 * S3 > 0)) || ( (S1 * S2 > 0) && !S3) )
		return lt_intersection_on_edge;
	
	//The triangle and the lines are coplanar, then S1 = S2 = S3 = 0
	if ( S1 == 0 && S2 == 0 && S3 == 0 )
		return TestIntersectionCoplonarLineWithTriangle( p1, p2, L, T );
		 
	return lt_unknown_intersection;

}

// TODO: неверный переход из одной СК в другую
PointIntersection chooseEdge(const PointIntersection& p, const DirectedLine& L,const Triangle* T)
{
	PointIntersection res;
	if (p.elementType == ET_EDGE)
	{
		return p;
		//_log(_error, _loc, gtc_("Don't choosed edge. Input point has type ET_EDGE, but must be ET_VERTEX. Point = %s LineSegment = %s Triangle = %s "),p.toText().c_str(), L.toText().c_str(), T->toText().c_str());
	}
	// найти нормаль к плоскости треугольника
	Point e0 = T->edge(0)->point(1) - T->edge(0)->point(0);
	Point e1 = T->edge(1)->point(1) - T->edge(1)->point(0);
	// нормаль
	Point vn = e0.crossProduct(e1).normalisedCopy();
	// находим вектор высоты треугольника к стороне противоположной вершине p
	//сторона
	Point ve = T->edge((p.indexElement + 2)%3)->point(1) - T->edge((p.indexElement + 2)%3)->point(0);
	ve = -ve.normalisedCopy(); // переводим в правосторонюю систему координат из левосторонней
	//высота
	Point vh = vn.crossProduct( ve ).normalisedCopy();

	//Vector3 t,u,v; //новые оси x,y,z
	
	//вектор перехода из одной СК в другую
	Point f =  vh - Point(1,0,0);//Vector3::UNIT_X; 

		
	// вектор направленого отрезка
	Point vL = (L.point(1) - L.point(0)).normalisedCopy();
	//в новой СК
	vL = vL + f;
	if (vL.z < 0)
	{
		res = p;
		res.elementType = ET_EDGE;
		res.indexElement = (p.indexElement - 1) % 3; // сторона до вершины
		return res;
	}
	if (vL.z > 0)
	{
		res = p;
		res.elementType = ET_EDGE;
		res.indexElement = p.indexElement ; // сторона после вершины
		return res;
	}
	// мы уперлись в вершину - никуда не идем
	res = p;
	return res;
}


Mesh::Mesh(const char *filename)
{

	
	std::auto_ptr<ireon::ws::navmesh_cfg::Mesh> meshcfg = ireon::ws::navmesh_cfg::mesh(filename);	
	maxX_ = 0;
	minX_ = 0;
	maxY_ = 0;
	minY_ = 0;
	vertexBufSize_= 0;

	bool skipFirstPoint = true; // octopus exporter added point

	// use shared buffer for all submeshes
	for ( ireon::ws::navmesh_cfg::Submeshes::submesh::iterator 
		it = meshcfg->submeshes().submesh().begin();
		it != meshcfg->submeshes().submesh().end();
		++it )
			vertexBufSize_ += (*it).geometry().vertexcount();
		
	vertexBuf_ = new Point[vertexBufSize_];


	uint subcount = 0;
	// load vertices
	Point * curv;
	uint count = 0;

	for ( ireon::ws::navmesh_cfg::Submeshes::submesh::iterator 
		it = meshcfg->submeshes().submesh().begin();
		it != meshcfg->submeshes().submesh().end();
		++it )
	{

		// use array pointers of submesh (section of vertices)
	vertexSubmeshBuf_[subcount] = vertexBuf_+count;
	
	ireon::ws::navmesh_cfg::VertexBuffer::vertex::container vcont = (*it).geometry().vertexbuffer().vertex();
	for (ireon::ws::navmesh_cfg::VertexBuffer::vertex::container::iterator it = vcont.begin(); it != vcont.end(); ++it )
	{
		curv = vertexBuf_+count;
		curv->x = it->position().x();
		if ( curv->x < minX_ ) 
			minX_ = curv->x;
		else if ( curv->x > maxX_ )
			maxX_ = curv->x;

		// not make z axis point upwards instead of y
		curv->y = it->position().y(); // be -z
		if ( curv->y < minY_ ) 
			minY_ = curv->y;
		else if ( curv->y > maxY_ )
			maxY_ = curv->y;

		
		curv->z = it->position().z();
		if ( curv->z < minZ_ ) 
			minZ_ = curv->y;
		else if ( curv->z > maxZ_ )
			maxZ_ = curv->z;

		// set precision to e-1
		curv->x = curv->floor(1).x;
		curv->y = curv->floor(1).y;
		curv->z = curv->floor(1).z;

		++count;
	}

	++subcount;

	}
	
	submeshCount_ = subcount;
	
	trianglesBufSize_ = 0;
	for ( ireon::ws::navmesh_cfg::Submeshes::submesh::iterator 
		it = meshcfg->submeshes().submesh().begin();
		it != meshcfg->submeshes().submesh().end();
		++it )
			trianglesBufSize_ += (*it).faces().count();

	trianglesBuf_ = new Triangle[trianglesBufSize_];

	typedef std::pair<uint,uint> edgeVertices;
	// maps vertex id pairs to face
	std::map<edgeVertices, Triangle *> edges;
	typedef std::map<edgeVertices, Triangle *>::iterator eiter;

	// load triangles
	uint vtmp[4];
	count = 0;
	subcount = 0;
	Triangle * curtri;
	uint tempSum; //  use for keep all vertice section number
	for ( ireon::ws::navmesh_cfg::Submeshes::submesh::iterator 
		it = meshcfg->submeshes().submesh().begin();
		it != meshcfg->submeshes().submesh().end();
		++it )
		{

		trianglesSubmeshBuf_[subcount] =  trianglesBuf_ + count;
		tempSum = count;
	ireon::ws::navmesh_cfg::Faces::face::container tcont = (*it).faces().face();
	for (ireon::ws::navmesh_cfg::Faces::face::container::iterator it = tcont.begin(); it != tcont.end(); ++it )
	{
		curtri = trianglesBuf_ + count;
		// check on clockwise order
		Vector3 p1,p2,p3;
		
		// skip first [0] vertex... octopus added him
		p1 = (vertexSubmeshBuf_[subcount] + it->v2())->toVector3() - (vertexSubmeshBuf_[subcount] + it->v1() )->toVector3();
		p2 = (vertexSubmeshBuf_[subcount] + it->v3())->toVector3() - (vertexSubmeshBuf_[subcount] + it->v2() )->toVector3();
		// нормаль
		p3 = p1.crossProduct(p2).normalisedCopy();
		
		Matrix3 matrix( p1.x, p1.y, p1.z,
						p2.x, p2.y, p2.z,
						p3.x, p3.y, p3.z
						);
		Real det = matrix.Determinant();
		if (det < 0)
			printf("submesh: %u triangle: %u NOT CONTRCLOCKWISE det = %f \n",subcount,count, det);
		
		(*curtri) = Triangle(vertexSubmeshBuf_[subcount] + it->v1() ,vertexSubmeshBuf_[subcount] + it->v2() ,vertexSubmeshBuf_[subcount] + it->v3() );
		
		//for create mesh
		//curtri->vertexIndex[0] = it->v1();
		//curtri->vertexIndex[1] = it->v2();
		//curtri->vertexIndex[2] = it->v3();
		//number
		curtri->number = count;
		// check on 
		for (int i = 0; i < 3 ; i++)
		{
			Vector3 E1 = curtri->edge(i)->point(1).toVector3() - curtri->edge(i)->point(0).toVector3();
			Vector3 E2 = curtri->edge((i+1)%3)->point(1).toVector3() - curtri->edge((i+1)%3 )->point(0).toVector3();
			Vector3 E3 = curtri->edge((i+2)%3)->point(1).toVector3() - curtri->edge((i+2)%3 )->point(0).toVector3();
			if (E1.length() == E2.length() + E3.length())
			{
				printf("%u Triangle is not normal %s \n", count, curtri->toText().c_str() );
				break;
			}
		}
		
		

		/*curtri->setVertice(0, vertexBuf_ + it->v1());
		curtri->setVertice(1, vertexBuf_ + it->v2());
		curtri->setVertice(2, vertexBuf_ + it->v3());*/

		
		vtmp[0] = tempSum + it->v1() ;
		vtmp[1] = tempSum + it->v2() ;
		vtmp[2] = tempSum + it->v3() ;
		vtmp[3] = tempSum + it->v1() ;

		// calculate neighbors
		for ( byte i = 0; i < 3; ++i)
		{
			uint max = std::max(vtmp[i], vtmp[i+1]);
			uint min = std::min(vtmp[i], vtmp[i+1]);

			eiter eit = edges.find(edgeVertices(max,min));
			// if there is already one neighbor for this edge
			if ( eit != edges.end() )
			{
				Triangle *neib = eit->second;
				curtri->neighbors[i] = neib;
				// set this triangle as neighbor in neighbor's list

				for (byte j=0; j<3; j++)
					for (byte k = 0;  k<3; k++)
					{
						//сравниваем учитывая что порядок обхода counterclock-wise
						if ( neib->vertex[j] == curtri->vertex[k] && neib->vertex[(j+1)%3] == curtri->vertex[(k+2)%3] )
						{
							neib->neighbors[j] = curtri;
							break;
						}
						
					}
				edges.erase(eit);
			}
			else
				edges[edgeVertices(max, min)] = curtri;
		}
		++count;
	}

		++subcount;
		}
	Triangle* sectri = NULL;
	//search not unicaled vertex == TODO: replace in utility
	for (int k = 0; k < trianglesBufSize_; ++k)
	{
		curtri = trianglesBuf_ + k;		
		for (int r = 0; r < trianglesBufSize_; ++r)
		{
			sectri = trianglesBuf_ + r;		
			for (byte l=0; l<3; l++)
				for (byte m = 0;  m<3; m++)
				{
					if ( *(curtri->vertice(l)) == *(sectri->vertice(m)) &&
						*(curtri->vertice((l+1)%3)) == *(sectri->vertice((m+2)%3)) &&
						(curtri->getNeighbor(l) != sectri || sectri->getNeighbor((m+2)%3) != curtri ) 
					)
					{
						//printf("[%u] Triangle must be neigbour [%u] Triangle \n -- %s \n -- %s \n"
						//	,curtri->number, sectri->number, curtri->toText().c_str(), sectri->toText().c_str() );
						curtri->neighbors[l] = sectri;
						sectri->neighbors[(m+2)%3] = curtri;
					}

				}
			

		}

	}

	KdTree<Triangle>::SetMemoryManager( new MManager<Triangle>() );

	
	tkdTree = new KdTree<Triangle>;
	
	aabb extendBox(Vector3(minX_, minY_, minZ_), Vector3(maxX_, maxY_, maxZ_) - Vector3(minX_, minY_, minZ_));
	tkdTree->build(trianglesBuf_, trianglesBufSize_, &extendBox);

}

//////////////////////////////////////////////////////////////////////////

Mesh::~Mesh()
{
	delete [] trianglesBuf_;
	delete [] vertexBuf_;
	delete tkdTree;
}

//////////////////////////////////////////////////////////////////////////
//http://jgt.akpeters.com/papers/MollerTrumbore97/

#define TEST_CULL

int intersect_triangle(const Point& orig, const Point& dir,
				   const Point& vert0, const Point& vert1, const Point& vert2,
				   double *t, double *u, double *v)
{
	Point edge1, edge2, tvec, pvec, qvec;
	double det,inv_det;

	/* find vectors for two edges sharing vert0 */
	// находим вектора сторон включающих vert0
	// e-1
	edge1 = vert1 - vert0;
	edge2 = vert2 - vert0;
	// e-1
	/* begin calculating determinant - also used to calculate U parameter */
	pvec = dir.crossProduct( edge2 );
	// e-2
	/* if determinant is near zero, ray lies in plane of triangle */
	det = edge1.dotProduct( pvec );
	// e-4

#ifdef TEST_CULL           /* define TEST_CULL if culling is desired */
	if (det < COORD_EPSILON)
		return 0;

	/* calculate distance from vert0 to ray origin */
	// e-1
	tvec = orig - vert0;
	// e-1
	/* calculate U parameter and test bounds */
	*u = tvec.dotProduct( pvec );
	// e-3
	
	if (*u <= - (0.001f) || *u >= det * (1.001f) )
		return 0;

	/* prepare to test V parameter */
	qvec = tvec.crossProduct( edge1 );
	// e-2
	
	/* calculate V parameter and test bounds */
	*v = dir.dotProduct( qvec );
	// e-3

	if (*v <= - (0.001f) || *u + *v >= det * (1.001f))
		return 0;
	/* calculate t, scale parameters, ray intersects triangle */

	//*t = edge2.dotProduct( qvec );
	//inv_det = Coord(1.0f) / det;
	//*t *= inv_det;

	*u /= det;
	*v /= det;
	//*u *= inv_det;
	//*v *= inv_det;
	*t = 1.0f - *u - *v;  // ed

	// e-4 or e-3 ?

#else                    /* the non-culling branch */
	if (-COORD_EPSILON < det  && COORD_EPSILON > det )
		return 0;
	//inv_det = 1.0f / det;

	/* calculate distance from vert0 to ray origin */
	tvec = orig - vert0;

	/* calculate U parameter and test bounds */
	*u = tvec.dotProduct( pvec ) /det ;//* inv_det;
	if (*u < 0.0f || *u > 1.0f)
		return 0;

	/* prepare to test V parameter */
	qvec = tvec.crossProduct( edge1 );

	/* calculate V parameter and test bounds */
	*v = dir.dotProduct( qvec ) / det ;//* inv_det;
	if (*v < 0.0f || *u + *v > 1.0f)
		return 0;

	/* calculate t, ray intersects triangle */
	*t = edge2.dotProduct( qvec ) /det;//* inv_det;
#endif
	return 1;
}


bool IntersectTriangle2(const Vector3&  v1, const Vector3&  v2, const Vector3&  v3,
						const Vector3&  n,const  Vector3&  p1, const Vector3&  p2,
						Vector3& pc)
{    
	Vector3  t1 = p1 - v1;
	Vector3  t2 = p2 - v1;

	double r1 = n.dotProduct(t1);
	double r2 = n.dotProduct(t2);


	if ((r1 > 0 && r2 > 0) || (r1 < 0 && r2 < 0))
		return false;

	Vector3 ip = (p1 + ((p2 - p1) * (-r1 / (r2 - r1))));

	if( (((v2 - v1).crossProduct(ip - v1)).dotProduct(n)) > 0.0001f )
	{
		if( (((v3 - v2).crossProduct(ip - v2)).dotProduct(n)) < - 0.0001f) return false;
		if( (((v1 - v3).crossProduct(ip - v3)).dotProduct(n)) < - 0.0001f) return false;
	}
	else
	{
		if( (((v3 - v2).crossProduct(ip - v2)).dotProduct(n)) > 0.0001f) return false;
		if( (((v1 - v3).crossProduct(ip - v3)).dotProduct(n)) > 0.0001f) return false;
	}

	pc = ip; 
	return true; 
}
//x,y  =  precision <= e-3 
//точность результата = e-3, ошибка E= 1 * e-4
Triangle * Mesh::getTriangleForPoint(const Point& p )
{
	Point intersectedPoint;
	Point pf = p;//.floor(3);
	std::multimap<Triangle*, Point> lt;

	Point dir(0, -1, 0);
	Point orig( pf.x, 100, pf.z );
	Point vert1, vert2, vert3;
	Vector3 pc;

	double r,u,v;
	for (uint t = 0; t < trianglesBufSize_; ++t)
	{
		Triangle* T = trianglesBuf_ + t;
				
		
		vert1 = * T->vertex[0];
		vert2 = * T->vertex[1];
		vert3 = * T->vertex[2];
		
		if (IntersectTriangle2(vert1.toVector3(), vert2.toVector3(), vert3.toVector3(),
				(vert3-vert1).crossProduct(vert2-vert1).toVector3(),orig.toVector3(), Vector3( (float)pf.x, -10, (float)pf.z),pc ))
		//if ( intersect_triangle( orig, dir, vert3, vert2, vert1, &r,&u,&v) )
		{
			
			//intersectedPoint = (* T->vertice(0)) * (1-u-v) + (* T->vertice(1)) * u + (* T->vertice(2)) * v;
			intersectedPoint = pc;
			//if ( intersectedPoint - p < POINT_EPSILON)
			//{
			//	// возвращаем первый попавшийся, если на границе между треугольниками.
			//	return T;	
			//}

			lt.insert(std::pair<Triangle*, Point>(T,intersectedPoint));	
			break;
		}
		
	}
	if (lt.empty())
		throw Mesh::ETriangleNotFound();
	//refactor
	std::multimap<Triangle*, Point>::iterator closest;
	closest = lt.begin();
	for (std::multimap<Triangle*, Point>::iterator it = lt.begin();
		it != lt.end();
		++it)
	{
		if ( (it->second - p).squaredLength() < (closest->second - p).squaredLength())
			closest = it;

	}
	return closest->first;
	

}

Triangle * Mesh::getTriangleInKDTree(const Point& p)
{

	Point pf = p;//.floor(3);


	Point dir(0, -1, 0);
	Point orig( pf.x, 100, pf.z );

	real st;
	Ray ray(orig.toVector3(), dir.toVector3());
	Triangle* T;
	//m_Stack = new kdstack[64];
	//m_Stack = (kdstack*)((((unsigned long)m_Stack) + 32) & (0xffffffff - 31));

	kdstack<Triangle>* kds = new kdstack<Triangle>[64];
	
	Vector3 pos,size;
	aabb extendBox(Vector3(minX_, minY_, minZ_), Vector3(maxX_, maxY_, maxZ_) - Vector3(minX_, minY_, minZ_));

	int res = tkdTree->FindNearest(ray, st, T, kds, &extendBox);
	delete[] kds;
	if (!res) 
		throw Mesh::ETriangleNotFound();
	

	return T;
}

//x,y  =  precision <= e-3 
//точность результата = e-3, ошибка E= 1 * e-4
Point Mesh::GetPointOnMesh(const Point& p )
{
	
	Point intersectedPoint;
	Point pf = p.floor(1);
	Triangle* T;
	try
	{
	T = getTriangleForPoint(pf);
	}
	catch (Mesh::EIntersectionNotFound &)
	{
		return Point(0,0,0);
	}
	
	
			Coord x1 = T->vertice(0)->x;
			Coord y1 = T->vertice(0)->y;
			Coord z1 = T->vertice(0)->z;

			Coord x2 = T->vertice(1)->x;
			Coord y2 = T->vertice(1)->y;
			Coord z2 = T->vertice(1)->z;

			Coord x3 = T->vertice(2)->x;
			Coord y3 = T->vertice(2)->y;
			Coord z3 = T->vertice(2)->z;

			Coord A = y1 * (z2 - z3) + y2 * (z3 - z1) + y3 * (z1 - z2);
			Coord B = z1 * (x2 - x3) + z2 * (x3 - x1) + z3 * (x1 - x2);
			Coord C = x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2);
			Coord D = -( x1 * (y2 * z3 - y3 * z2) + x2 * (y3 * z1 - y1 * z3) + x3 * (y1 * z2 - y2 * z1) );

			intersectedPoint.x = pf.x;
			intersectedPoint.y =  -(A*pf.x + C*pf.z + D)/B;
			intersectedPoint.z =  pf.z;
		
			return intersectedPoint;
}
//  может сделать проверку на то что нач точка принадлежит плоскости треугольника ?
Point Mesh::getVectorProjectionOnPlane(const Point& begin, const Point& end,const Triangle* T )
{
	//начальный вектор в локальных координатах
	// e-1
	Point vec = end - begin;
	// e-1
	// вектора ребер треугольника
	Point v1 = * T->vertex[1] - * T->vertex[0];
	Point v2 = * T->vertex[2] - * T->vertex[0];
	//e-1
	// находим нормаль к треугольнику
	Point norm = (v1.crossProduct(v2)).normalisedCopy();
	// e-4
	// находим проекцию вектора на нормаль
	Point projOnNorm = norm * (vec.dotProduct(norm)) ;
	// e-4
	// находим искомую проекцию на плоскость
	Point projOnPlane = vec - projOnNorm ;
	
	// возвращаем локальный точку конца проекции
	return projOnPlane;
};

Point Mesh::getHeightAt(const Point& p ,const Triangle* T)
{

	Coord x1 = T->vertice(0)->x;
	Coord y1 = T->vertice(0)->y;
	Coord z1 = T->vertice(0)->z;

	Coord x2 = T->vertice(1)->x;
	Coord y2 = T->vertice(1)->y;
	Coord z2 = T->vertice(1)->z;

	Coord x3 = T->vertice(2)->x;
	Coord y3 = T->vertice(2)->y;
	Coord z3 = T->vertice(2)->z;

	Coord A = y1 * (z2 - z3) + y2 * (z3 - z1) + y3 * (z1 - z2);
	Coord B = z1 * (x2 - x3) + z2 * (x3 - x1) + z3 * (x1 - x2);
	Coord C = x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2);
	Coord D = -( x1 * (y2 * z3 - y3 * z2) + x2 * (y3 * z1 - y1 * z3) + x3 * (y1 * z2 - y2 * z1) );

	Point res;
	res.x = p.x;
	res.y =  -(A*p.x + C*p.z + D)/B;
	res.z = p.z;

	return res;
	
}
Vector3 toVector3(const Vector4& v)
{
	return Vector3(v.x,v.y,v.z);
}
//http://www.gamedev.ru/proj/?id=1&doc=54
ManualTriangle* getTriangleInsideTriangle(const Triangle* T)
{
	Point *vertexA, *vertexB, *vertexC;
	Point newVertexA, newVertexB, newVertexC;

	vertexA = T->vertice(0);
	vertexB = T->vertice(1);
	vertexC = T->vertice(2);

	Matrix3 scaleMatrix
		(0.9995f, 0, 0, 
		 0, 0.9995f, 0, 
		 0, 0, 0.9995f);
	
	newVertexA = scaleMatrix.Transpose() * vertexA->toVector3();
	newVertexB = scaleMatrix.Transpose() * vertexB->toVector3();
	newVertexC = scaleMatrix.Transpose() * vertexC->toVector3();

	Coord mid1X =(vertexA->x+((vertexB->x + vertexC->x)))/(1+2);
	Coord mid1Y =(vertexA->y+((vertexB->y + vertexC->y)))/(1+2);
	Coord mid1Z =(vertexA->z+((vertexB->z + vertexC->z)))/(1+2);

	Coord mid2X =(newVertexA.x+((newVertexB.x + newVertexC.x)))/(1+2);
	Coord mid2Y =(newVertexA.y+((newVertexB.y + newVertexC.y)))/(1+2);
	Coord mid2Z =(newVertexA.z+((newVertexB.z + newVertexC.z)))/(1+2);

	Matrix4 translateMatrix
		(1, 0, 0, (float) (mid2X-mid1X),
		0, 1, 0, (float) (mid2X-mid1X),
		0, 0, 1, (float) (mid2X-mid1X),
		0, 0, 0, 1);

	newVertexA = toVector3(translateMatrix * Vector4( (float)newVertexA.x, (float)newVertexA.y, (float)newVertexA.z, 1)) ;
	newVertexB = toVector3(translateMatrix * Vector4( (float)newVertexB.x, (float)newVertexB.y, (float)newVertexB.z, 1)) ;
	newVertexC = toVector3(translateMatrix * Vector4( (float)newVertexC.x, (float)newVertexC.y, (float)newVertexC.z, 1)) ;

		
		
	return new ManualTriangle(newVertexA,newVertexB, newVertexC);
	

}
void Mesh::traceWalk( const Point & startPoint3D,const Point & movementVector2D, Triangle *& T , Point& endPoint3D)
{
	Point closestDist;
	
	//треугольник в плоскости XZ
	const ManualTriangle xzT(Point(1,0,0),Point(0,0,0), Point(0,0,1));
	
	if (movementVector2D.abs() < POINT_EPSILON)
	{
		endPoint3D = startPoint3D;
		return;
	}
	// точка назначения лежит в тойже горизонтальной плоскости
	// увеличиваем вектор 
	Point destPoint3D = startPoint3D + movementVector2D;
	// находим точку назначения на плоскости текущего треугольника
//	destPoint3D = startPoint3D + getVectorProjectionOnPlane(startPoint3D, destPoint3D, T );
	destPoint3D = getHeightAt(destPoint3D, T);
	
	//ManualTriangle* tInT = getTriangleInsideTriangle(T); 

	DirectedLine L(startPoint3D,destPoint3D);

	PointIntersection intersectedPoint;
	LineSegmentTypeIntersection res = IntersectionCoplonarLineSegmentWithTriangle(intersectedPoint, L, T);
	//delete tInT;
	Point speed3D;

	intersectedPoint = intersectedPoint.floor(1);
	switch(res)
	{
	case lst_intersection_at_vertex:
		//мы находимся в углу треугольника
		endPoint3D = intersectedPoint;
		// нужно выбрать одного соседа из двух прилежащих к сторонам, сходящимся в вершине
		intersectedPoint = chooseEdge(intersectedPoint,L,T);
		endPoint3D = intersectedPoint;
		if (intersectedPoint.elementType == ET_VERTEX)
			_log(_info, _loc, gtc_("trace walk. Vertex. Stop. Point = % LineSegment = %s Triangle = %s "), intersectedPoint.toText().c_str(), L.toText().c_str(), T->toText().c_str());
		else
			if (T->neighbors[intersectedPoint.indexElement])
			{
				T = T->neighbors[intersectedPoint.indexElement];
				//проецируем остаток скорости обратно на плоскость XZ
				speed3D = getVectorProjectionOnPlane(endPoint3D, destPoint3D,&xzT).floor(3);
				if (speed3D.squaredLength() < Vector3(0.1f,0.1f,0.1f).squaredLength())
					speed3D = Vector3(0.1f,0.1f,0.1f);
				traceWalk(intersectedPoint, speed3D ,T,endPoint3D);
			}
			
		break;
	case lst_intersection_on_edge:
		//пересекаем сторону
		endPoint3D = intersectedPoint;
		speed3D = getVectorProjectionOnPlane(endPoint3D, destPoint3D,&xzT).floor(1);

		// не даем приблизиться к границам на некторую величину
		//closestDist = speed3D.normalisedCopy() * 0.01f;
		//endPoint3D -= closestDist; // откатываемся немного назад
		//speed3D += closestDist; // увеличиваем скорость нанемного
		 
		//если есть сосед продолжаем двигаться
		if (T->neighbors[intersectedPoint.indexElement])
		{
			T = T->neighbors[intersectedPoint.indexElement];
			traceWalk(intersectedPoint,speed3D,T,endPoint3D);
		}	
		else 
		{
			endPoint3D = endPoint3D - speed3D.normalisedCopy()*0.1f; 
		}
		break;
	case lst_triangle_contains_segment:
		// мы не покинули треугольник
		endPoint3D = intersectedPoint;


		break;
	case lst_no_intersection:
		_log(_fatal, _loc, gtc_("coplonar_and_no_intersection: error in input data."));
		_log(_fatal, _loc, gtc_("LineSegment = %s Triangle = %s "), L.toText().c_str(), T->toText().c_str());
		//пересечения между линией и треугольником нет
		//линия лежит вне треугольника - ошибка в входных данных
		break;
	case lst_unknown_intersection:
		//произошла ошибка внутри функции определения пересечения линии и треугольника
		_log(_fatal, _loc, gtc_("unknown_intersection: error in function IntersectionCoplonarLineSegmentWithTriangle."));
		_log(_fatal, _loc, gtc_("LineSegment = %s Triangle = %s "), L.toText().c_str(), T->toText().c_str());

		break;
	default:
		_log(_fatal, _loc, gtc_("unknown result: IntersectionCoplonarLineSegmentWithTriangle return unknown result."));
		_log(_fatal, _loc, gtc_("LineSegment = %s Triangle = %s "), L.toText().c_str(), T->toText().c_str());
		//неизвестный результат - функция вернула что то неизвестное
		break;
	}

	

}

//все входные и выходные координаты в e-4
//все вычисления происходят в float(e-38) изредка переходя в double


//////////////////////////////////////////////////////////////////////////
// startPos - start position ( ( x,z ) + Triangle) ... precision E-4
// speed - vector speed ( normalized vector direction (x,z) * speed ( pt/s ) ) ... precision E-4
// time - walking time in ms ... precision E-4
Position Mesh::walk(Position startPos, Point speed, short time)
{

	//round to input precision 
	// соответсвует координатам в плоскости XZ
	Point startPoint3D;
	/*startPoint3D.x = startPos.x;
	startPoint3D.y = startPos.y;
	startPoint3D.z = startPos.z;*/
	startPoint3D = startPos.getPoint3D().floor(1);
	// вектор движения
	Point movementVector = speed * time;
	movementVector.y = 0;
	movementVector = movementVector.floor(1);
	// result point
	Point endPoint3D;

	Triangle * startTriangle = startPos.tri;
	
	try
	{
		// compute allowed max destination point
		traceWalk(startPoint3D, movementVector, startTriangle, endPoint3D);
	} 
	catch (Mesh::EIntersectionNotFound &) {
		_log(_error, _loc, gtc_("Mesh::EIntersectionNotFound. Some strange error occurred. Call mip asap."));
		endPoint3D = startPoint3D;
	}
	catch (EFixedPointMathError) {
		_log(_error, _loc, gtc_("EFixedPointMathError. Fixed point ariphmetic operation error. Small size fixed point "));
		endPoint3D = startPoint3D;
	}
	endPoint3D = endPoint3D.floor(1);
	//return changed end point
	return Position(endPoint3D, startTriangle);
}



// http://algolist.manual.ru/maths/geom/equation/plane.php

// return precision E-5
Point Position::getPoint3D() const
{	
	Coord x1 = tri->vertice(0)->x;
	Coord y1 = tri->vertice(0)->y;
	Coord z1 = tri->vertice(0)->z;

	Coord x2 = tri->vertice(1)->x;
	Coord y2 = tri->vertice(1)->y;
	Coord z2 = tri->vertice(1)->z;

	Coord x3 = tri->vertice(2)->x;
	Coord y3 = tri->vertice(2)->y;
	Coord z3 = tri->vertice(2)->z;

	Coord A = y1 * (z2 - z3) + y2 * (z3 - z1) + y3 * (z1 - z2);
	Coord B = z1 * (x2 - x3) + z2 * (x3 - x1) + z3 * (x1 - x2);
	Coord C = x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2);
	Coord D = -( x1 * (y2 * z3 - y3 * z2) + x2 * (y3 * z1 - y1 * z3) + x3 * (y1 * z2 - y2 * z1) );

	Point res;
	res.x = x;
	/*res.y = coordRound<Coord>( -(A*x + C*y + D)/B, WORK_PRECISION);
	*/
	res.y = y;

	/*res.z = coordRound<Coord>( y, WORK_PRECISION);*/
	res.z = z;
	return res;
}
//////////////////////////////////////////////////////////////////////////

//void Mesh::getLinesIntersection(Coord x11, Coord y11, Coord x12, Coord y12,
//						  Coord x21, Coord y21, Coord x22, Coord y22,
//						  Coord &x, Coord &y)
//{
//	float a1 = y12-y11;
//	float b1 = x11-x12;
//	float c1 = x12 * y11 - x11 * y12;
//
//	float a2 = y22-y21;
//	float b2 = x21-x22;
//	float c2 = x22*y21 - x21*y22;
//
//	float d = a1*b2 - a2*b1;
//	if (!d)
//		throw "lines are parallel";
//
//	x = (b1*c2 - b2*c1)/d;
//	y = (a2*c1 - a1*c2)/d;
//}

//////////////////////////////////////////////////////////////////////////

/// see http://www.ecse.rpi.edu/Homepages/wrf/Research/Short_Notes/pnpoly.html
//bool Mesh::pointIsInTriangle( Vector2 & pt, Triangle * curtri )
//{
//	double r,u,v;
//	double orig[3]={pt.x,10000,pt.y};
//	double dir[3]= {0,-1,0};
//	double vert1[3]={curtri->vertex[0]->x, curtri->vertex[0]->y, curtri->vertex[0]->z};
//	double vert2[3]={curtri->vertex[1]->x, curtri->vertex[1]->y, curtri->vertex[1]->z};
//	double vert3[3]={curtri->vertex[2]->x, curtri->vertex[2]->y, curtri->vertex[2]->z};
//
//	if (intersect_triangle( orig, dir, vert1, vert2, vert3,	&r,&u,&v))
//		return true;
//	return false;
//
//
//}

//////////////////////////////////////////////////////////////////////////
//Real Mesh::getHeightAt(const Vector2& point2D)
//{
//	Triangle* T;
//	Point point;
//	try
//	{
//		T = getTriangleForPoint(point2D.x, point2D.y);
//		Position posit(point2D, T);
//		point = posit.getPoint3D();
//	}
//	catch (const ETriangleNotFound&)
//	{
//		throw ETriangleNotFound();
//	}
//	return point.y;
//	
//}
