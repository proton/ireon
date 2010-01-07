/**
* @file navigation_mesh_test.cpp
* Tests for Mesh class
*/

/* Copyright (C) 2005 ireon.org developers council
* $Id$

*  See LICENSE for details
*/

#include "boost/test/unit_test.hpp"

#include <boost/test/execution_monitor.hpp>
using boost::unit_test::test_suite;
#pragma warning(push)
#pragma warning(disable:4251 4005)
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "common/world/mesh.h"
#include <log4cxx/logger.h>
#include <log4cxx/xml/domconfigurator.h>
#include <log4cxx/basicconfigurator.h>
#include <log4cxx/ndc.h>


void testUtilityFunction()
{

	
	//--------------------

	BOOST_CHECKPOINT("Test utility function navigation mesh");
	std::auto_ptr<ireon::nav::Mesh> mesh;
	mesh.reset(new ireon::nav::Mesh("../data/proto/zones/navigation_mesh.xml"));

	{	

		BOOST_CHECKPOINT("Test a");
		// p
		ireon::nav::Point p(0,0,0);
		ireon::nav::Triangle* res;
		try
		{
		res = mesh->getTriangleInKDTree(p);
		}
		catch (ireon::nav::Mesh::ETriangleNotFound&)
		{

		}
		
		//BOOST_CHECK(res == ireon::nav::Point(0,0,1));
		
	}

	{	

		BOOST_CHECKPOINT("Test 1");
		// get vector projection on plane
		ireon::nav::Point begin(0,0,0);
		ireon::nav::Point end(0,0,1);
		ireon::nav::Point res;
		ireon::nav::Point p1(0,0,0);
		ireon::nav::Point p2(5,0,5);
		ireon::nav::Point p3(5,0,-5);
		ireon::nav::ManualTriangle* curtri = new ireon::nav::ManualTriangle(p1,p2,p3);
		res = mesh->getVectorProjectionOnPlane(begin, end, curtri);
		BOOST_CHECK(res == ireon::nav::Point(0,0,1));
		delete curtri;
	}

	{	
		BOOST_CHECKPOINT("Test 2");
		// get vector projection on plane
		ireon::nav::Point begin(0,0,0);
		ireon::nav::Point end(5,5,0);
		ireon::nav::Point res;
		ireon::nav::Point p1(1,0,1);
		ireon::nav::Point p2(5,0,5);
		ireon::nav::Point p3(5,0,-5);
		ireon::nav::ManualTriangle* curtri = new ireon::nav::ManualTriangle(p1,p2,p3);
		res = mesh->getVectorProjectionOnPlane(begin, end, curtri);
		BOOST_CHECK(res == ireon::nav::Point(5,0,0));
		delete curtri;
	}
	
	{	
		BOOST_CHECKPOINT("Test 3");
		// get intersection point two of directed line
		ireon::nav::Point res;

		ireon::nav::DirectedLine l1(Vector3(0,0,0),Vector3(2,2,2));
		ireon::nav::DirectedLine l2(Vector3(1,1,0),Vector3(1,1,4));
		
		res = ireon::nav::getIntersectionPointLineWithSegment(l1,l2);
		BOOST_CHECK(res == ireon::nav::Point(1,1,1));
		
	}


	{	
		BOOST_CHECKPOINT("Test 4");
		// get vector projection on plane
		ireon::nav::Point res;

		ireon::nav::DirectedLine l1(Vector3(-2,-1,0),Vector3(4,2,0));
		ireon::nav::DirectedLine l2(Vector3(5,0,0),Vector3(-1,2,0));

		res = ireon::nav::getIntersectionPointLineWithSegment(l1,l2);
		BOOST_CHECK(res == ireon::nav::Point(2,1,0));

	}
	{	
		BOOST_CHECKPOINT("Test 4a");
		// 
		ireon::nav::DirectedLine L1(Vector3(1,0,1), Vector3(10,0,1));
		ireon::nav::DirectedLine L2(Vector3(2,0,0), Vector3(2,0,5));

		ireon::nav::Point res1,res2;

		res1 = ireon::nav::getIntersectionPointLineWithSegment(L1, L2);
		BOOST_CHECK(res1 == ireon::nav::Point(2,0,1));
		//res2 = ireon::nav::vec3_inter_lines(L1, L2);
		//BOOST_CHECK(res2 == Vector3(2,0,1));
	}

	{
		BOOST_CHECKPOINT("Test 4b");
		// 
		//ireon::nav::Point posOnMesh_(-21.3300,       5.7890,     -17.2800);
		//ireon::nav::Point curentPosition2D = mesh->GetPointOnMesh(posOnMesh_);
		//ireon::nav::Triangle* curentTriangle = mesh->getTriangleForPoint(curentPosition2D);

	}
	

	{	
		BOOST_CHECKPOINT("Test 4b");
		// 
		ireon::nav::Point posOnMesh_(-35, 6.90349 , -16);
		ireon::nav::Point curentPosition2D = mesh->GetPointOnMesh(posOnMesh_);
		ireon::nav::Triangle* curentTriangle = mesh->getTriangleForPoint(curentPosition2D);

		ireon::nav::DirectedLine L1 = *(curentTriangle->edge(2));

		ireon::nav::DirectedLine L2(curentPosition2D, curentPosition2D + ireon::nav::Point(0,0,-2));
		
		ireon::nav::Point res1,res2,res3;

		res1 = ireon::nav::getIntersectionPointLineWithSegment(L1, L2);
		res2 = ireon::nav::vec3_inter_lines(L1, L2);
		res3 = ireon::nav::Line_to_Line(L1,L2);
		//BOOST_CHECK(res2 == Vector3(2,0,1));
	}
	
	{	
		BOOST_CHECKPOINT("Test 5");
		// проверить случай с первой точкой в начале вектора
		ireon::nav::PointIntersection res;

		ireon::nav::DirectedLine l1(Vector3(0,0,0),Vector3(4,2,0));
		ireon::nav::PointIntersection p1(2,1,0);
		ireon::nav::PointIntersection p2(-2,-1,0);

		res = ireon::nav::getPointOnDirection(l1,p1,p2);
		// p1 находится по направлению движения
		BOOST_CHECK(res == p1);

	}


	{	
		BOOST_CHECKPOINT("Test 6");
		// 
		ireon::nav::DirectedLine L(Vector3(1,0,1), Vector3(3,0,3));
		ireon::nav::PointIntersection point;
		ireon::nav::LineSegmentTypeIntersection res;
		ireon::nav::Point p1(0,0,0);
		ireon::nav::Point p2(0,0,4);
		ireon::nav::Point p3(4,0,0);
		ireon::nav::ManualTriangle* T = new ireon::nav::ManualTriangle(p1,p2,p3);
		res = ireon::nav::IntersectionCoplonarLineSegmentWithTriangle( point, L, T);
		BOOST_CHECK(res == ireon::nav::lst_intersection_on_edge);
		BOOST_CHECK(point == ireon::nav::Point(2,0,2));
		delete T;
	}
	{	
		BOOST_CHECKPOINT("Test 7");
		// 
		ireon::nav::DirectedLine L(Vector3(0,0,1), Vector3(0,0,3));
		ireon::nav::PointIntersection point;
		ireon::nav::LineSegmentTypeIntersection res;
		
		ireon::nav::Point p1(0,0,0);
		ireon::nav::Point p2(0,0,4);
		ireon::nav::Point p3(4,0,0);
		ireon::nav::ManualTriangle* T = new ireon::nav::ManualTriangle(p1,p2,p3);
		res = ireon::nav::IntersectionCoplonarLineSegmentWithTriangle( point, L, T);
		BOOST_CHECK(res == ireon::nav::lst_intersection_on_edge);
		BOOST_CHECK(point == ireon::nav::Point(0,0,3));
		delete T;
	}
	{	
		BOOST_CHECKPOINT("Test 8");
		// 
		ireon::nav::DirectedLine L(Vector3(0,0,0), Vector3(0,0,7));
		ireon::nav::PointIntersection point;
		ireon::nav::LineSegmentTypeIntersection res;
		
		ireon::nav::Point p1(0,0,0);
		ireon::nav::Point p2(0,0,4);
		ireon::nav::Point p3(4,0,0);
		ireon::nav::ManualTriangle* T = new ireon::nav::ManualTriangle(p1,p2,p3);
		res = ireon::nav::IntersectionCoplonarLineSegmentWithTriangle( point, L, T);
		BOOST_CHECK(res == ireon::nav::lst_intersection_at_vertex);
		BOOST_CHECK(point == ireon::nav::Point(0,0,4));
		delete T;
	}

	{	
		BOOST_CHECKPOINT("Test 8");
		// 
		ireon::nav::DirectedLine L(Vector3(0,0,1), Vector3(7,0,1));
		ireon::nav::PointIntersection point;
		ireon::nav::LineSegmentTypeIntersection res;
		
		ireon::nav::Point p1(0,0,0);
		ireon::nav::Point p2(0,0,4);
		ireon::nav::Point p3(4,0,0);
		ireon::nav::ManualTriangle* T = new ireon::nav::ManualTriangle(p1,p2,p3);
		res = ireon::nav::IntersectionCoplonarLineSegmentWithTriangle( point, L, T);
		BOOST_CHECK(res == ireon::nav::lst_intersection_on_edge);
		BOOST_CHECK(point == ireon::nav::Point(3,0,1));
		delete T;
	}

	{	
		BOOST_CHECKPOINT("Test 9");
		// 
		ireon::nav::DirectedLine L(Vector3(0,0,2), Vector3(6,0,-1));
		ireon::nav::PointIntersection point;
		ireon::nav::LineSegmentTypeIntersection res;
		
		ireon::nav::Point p1(0,0,0);
		ireon::nav::Point p2(0,0,4);
		ireon::nav::Point p3(4,0,0);
		ireon::nav::ManualTriangle* T = new ireon::nav::ManualTriangle(p1,p2,p3);
		res = ireon::nav::IntersectionCoplonarLineSegmentWithTriangle( point, L, T);
		BOOST_CHECK(res == ireon::nav::lst_intersection_at_vertex);
		BOOST_CHECK(point == ireon::nav::Point(4,0,0));
		delete T;
	}

	{	
		BOOST_CHECKPOINT("Test 10");
		// 
		ireon::nav::DirectedLine L(Vector3(4,0,0), Vector3(2,0,-1));
		ireon::nav::PointIntersection point;
		ireon::nav::LineSegmentTypeIntersection res;
		
		ireon::nav::Point p1(0,0,0);
		ireon::nav::Point p2(0,0,4);
		ireon::nav::Point p3(4,0,0);
		ireon::nav::ManualTriangle* T = new ireon::nav::ManualTriangle(p1,p2,p3);
		res = ireon::nav::IntersectionCoplonarLineSegmentWithTriangle(point, L, T);
		BOOST_CHECK(res == ireon::nav::lst_intersection_at_vertex);
		BOOST_CHECK(point == ireon::nav::Point(4,0,0));
		delete T;
	}

	{	
		BOOST_CHECKPOINT("Test 11");
		// 
		ireon::nav::DirectedLine L(Vector3(1,0,1), Vector3(1.5,0,1.5));
		ireon::nav::PointIntersection point;
		ireon::nav::LineSegmentTypeIntersection res;
		
		ireon::nav::Point p1(0,0,0);
		ireon::nav::Point p2(0,0,4);
		ireon::nav::Point p3(4,0,0);
		ireon::nav::ManualTriangle* T = new ireon::nav::ManualTriangle(p1,p2,p3);
		res = ireon::nav::IntersectionCoplonarLineSegmentWithTriangle(point, L, T);
		BOOST_CHECK(res == ireon::nav::lst_triangle_contains_segment);
		BOOST_CHECK(point == ireon::nav::Point(1.5,0,1.5));
		delete T;
	}

	{
		BOOST_CHECKPOINT("Test 12");
		ireon::nav::PointIntersection p(0,0,0.99f);
		ireon::nav::Point p0(0,0,0);
		ireon::nav::Point p1(0,0,1);
		ireon::nav::DirectedLine L(p0,p1);
		bool resb;
		bool oldresb = true;
		for (int i=0; i<100000; ++i)
		{
			p = p+ireon::nav::Point(0,0,0.000001);
			resb = ireon::nav::isPointInsideSegment(p,L);
			if (resb != oldresb)
			{
				printf("changed - %15.7f, %15.7f, %15.7f \n",(float)p.x, (float)p.y, (float)p.z);
				oldresb = resb;
			}

		}
		//BOOST_CHECK(resb == true);
	}




}

//-----------------------------------------------------------------------

void testWalk()
{
	{	
		std::auto_ptr<ireon::nav::Mesh> ptr;
		ptr.reset(new ireon::nav::Mesh("../data/proto/zones/navigation_mesh.xml"));

		//{	
		//	// верт вниз
		//	ireon::nav::Point speed(0,0,1);
		//	short timet = 1;
		//	
		//	ireon::nav::Point curent = ireon::nav::Point(-35,100,20);
		//	ireon::nav::Point curent2 = ptr->GetPointOnMesh(ireon::nav::Point(curent));
		//	ireon::nav::Position pos(curent2, ptr->getTriangleForPoint(ireon::nav::Point(curent2)));
		//	
		//	ptr->walk(pos,speed, timet);
		//	
		//	

		//}


		//{	
		//	float m_walkSpeed = 1.0f;
		//	ireon::nav::Point posOnMesh_(-23.959984, 5.283846, -21.217026);
		//	ireon::nav::Point curentPosition2D = ptr->GetPointOnMesh(posOnMesh_);
		//	// -23960000 , 5283698, -21217000
		//	ireon::nav::Point	direction2D( -0.033742, 0, -0.999431 );
		//	
		//	ireon::nav::Triangle* curentTriangle = ptr->getTriangleForPoint(curentPosition2D);
		//	ireon::nav::Position curentPositionOnMesh2D(curentPosition2D, curentTriangle);

		//	ireon::nav::Position newPositionOnMesh2D( ptr->walk(curentPositionOnMesh2D, direction2D * m_walkSpeed, 1) );
		//	//-23.959984, 5.283846, -21.217026
		//	ireon::nav::Point newPosition3D = newPositionOnMesh2D.getPoint3D();
		//	
		//	ireon::nav::Point x (newPosition3D.toVector3());
		//	


		//}
		{
			BOOST_CHECKPOINT("Test WALK");
			float m_walkSpeed = 1.0f;
			ireon::nav::Point posOnMesh_( -1.5000,       6.4954,      10.7000);
			ireon::nav::Point curentPosition2D = ptr->GetPointOnMesh(posOnMesh_);
			//ireon::nav::Point curentPosition2D = posOnMesh_;

			ireon::nav::Point	direction2D( 0, 0, -1 );

			ireon::nav::Triangle* curentTriangle = ptr->getTriangleForPoint(curentPosition2D);
			ireon::nav::Position curentPositionOnMesh2D(curentPosition2D, curentTriangle);

			ireon::nav::Position newPositionOnMesh2D( ptr->walk(curentPositionOnMesh2D, direction2D * m_walkSpeed, 1) );
			ireon::nav::Point newPosition3D = newPositionOnMesh2D.getPoint3D();

			ireon::nav::Point x (newPosition3D.toVector3());

			//BOOST_CHECK(newPosition3D == ireon::nav::Point(-1.5, 6.6, 9.7));

		}

		{
			BOOST_CHECKPOINT("Test WALK a");
			float m_walkSpeed = 1.0f;
			ireon::nav::Point posOnMesh_( 29.7,       5.8 ,      17.9);
			//ireon::nav::Point curentPosition2D = ptr->GetPointOnMesh(posOnMesh_);
			ireon::nav::Point curentPosition2D = posOnMesh_;

			ireon::nav::Point	direction2D( 0, 0, -1 );

			ireon::nav::Triangle* curentTriangle = ptr->getTriangleForPoint(curentPosition2D);
			ireon::nav::Position curentPositionOnMesh2D(curentPosition2D, curentTriangle);

			//ireon::nav::Position newPositionOnMesh2D( ptr->walk(curentPositionOnMesh2D, direction2D * m_walkSpeed, 1) );
			//ireon::nav::Point newPosition3D = newPositionOnMesh2D.getPoint3D();

			//ireon::nav::Point x (newPosition3D.toVector3());

			//BOOST_CHECK(newPosition3D == ireon::nav::Point(29.7, 5.9, 16.9));

		}

		{
			BOOST_CHECKPOINT("Test WALKx - edge");
			float m_walkSpeed = 1.0f;
			ireon::nav::Point posOnMesh_( -22.2000,       0.0000,     -42.4000);
			// T[90]		
			ireon::nav::Point curentPosition2D = ptr->GetPointOnMesh(posOnMesh_);
			//ireon::nav::Point curentPosition2D = posOnMesh_;

			ireon::nav::Point	direction2D( 0, 0, -1 );

			ireon::nav::Triangle* curentTriangle = ptr->getTriangleForPoint(curentPosition2D);
			ireon::nav::Position curentPositionOnMesh2D(curentPosition2D, curentTriangle);

			ireon::nav::Position newPositionOnMesh2D( ptr->walk(curentPositionOnMesh2D, direction2D * m_walkSpeed, 1) );
			ireon::nav::Point newPosition3D = newPositionOnMesh2D.getPoint3D();

			ireon::nav::Point x (newPosition3D.toVector3());

			//BOOST_CHECK(newPosition3D == ireon::nav::Point(-22.2, 0, -42.3));

		}
		{
			BOOST_CHECKPOINT("Test WALKz - edge vert");
			float m_walkSpeed = 1.0f;
			ireon::nav::Point posOnMesh_( 42.7000,       0.0000,     -32.0000);
					
			//ireon::nav::Point curentPosition2D = ptr->GetPointOnMesh(posOnMesh_);
			ireon::nav::Point curentPosition2D = posOnMesh_;

			ireon::nav::Point	direction2D( 0, 0, 1 );

			ireon::nav::Triangle* curentTriangle = ptr->getTriangleForPoint(curentPosition2D);
			ireon::nav::Position curentPositionOnMesh2D(curentPosition2D, curentTriangle);

			//ireon::nav::Position newPositionOnMesh2D( ptr->walk(curentPositionOnMesh2D, direction2D * m_walkSpeed, 1) );
			//ireon::nav::Point newPosition3D = newPositionOnMesh2D.getPoint3D();

			//ireon::nav::Point x (newPosition3D.toVector3());

			//BOOST_CHECK(newPosition3D == ireon::nav::Point(-1.5, 6.6, 9.7));

		}
		
		 
		 {
			 BOOST_CHECKPOINT("Test WALKsd - edge");
			 float m_walkSpeed = 1.0f;
			 ireon::nav::Point posOnMesh_( 42.8000,       0.0000,      10.0000);
			 // T[90]		
			 ireon::nav::Point curentPosition2D = ptr->GetPointOnMesh(posOnMesh_);
			 //ireon::nav::Point curentPosition2D = posOnMesh_;

			 ireon::nav::Point	direction2D( 0.0000,       0.0000,       1.0000 );

			 ireon::nav::Triangle* curentTriangle = ptr->getTriangleForPoint(curentPosition2D);
			 ireon::nav::Position curentPositionOnMesh2D(curentPosition2D, curentTriangle);

			 ireon::nav::Position newPositionOnMesh2D( ptr->walk(curentPositionOnMesh2D, direction2D * m_walkSpeed, 1) );
			 ireon::nav::Point newPosition3D = newPositionOnMesh2D.getPoint3D();

			 ireon::nav::Point x (newPosition3D.toVector3());

			 //BOOST_CHECK(newPosition3D == ireon::nav::Point(-22.2, 0, -42.3));

		 }
		 
		 {
			 BOOST_CHECKPOINT("Test WALK - recursion depth error");
			 float m_walkSpeed = 1.0f;
			 ireon::nav::Point posOnMesh_( 38.7000,       2.1074,      15.3000);
				
			 ireon::nav::Point curentPosition2D = ptr->GetPointOnMesh(posOnMesh_);
			 //ireon::nav::Point curentPosition2D = posOnMesh_;

			 ireon::nav::Point	direction2D( 0.0000,       0.0000,       -1.0000 );

			 ireon::nav::Triangle* curentTriangle = ptr->getTriangleForPoint(curentPosition2D);
			 ireon::nav::Position curentPositionOnMesh2D(curentPosition2D, curentTriangle);

			 ireon::nav::Position newPositionOnMesh2D( ptr->walk(curentPositionOnMesh2D, direction2D * m_walkSpeed, 1) );
			 ireon::nav::Point newPosition3D = newPositionOnMesh2D.getPoint3D();

			 ireon::nav::Point x (newPosition3D.toVector3());

			 //BOOST_CHECK(newPosition3D == ireon::nav::Point(-22.2, 0, -42.3));

		 }
		 
		 {
			 BOOST_CHECKPOINT("Test WALK - recursion depth error");
			 float m_walkSpeed = 1.0f;
			 ireon::nav::Point posOnMesh_( 10.8000,       7.1877,      10.7000);

			 ireon::nav::Point curentPosition2D = ptr->GetPointOnMesh(posOnMesh_);
			 //ireon::nav::Point curentPosition2D = posOnMesh_;

			 ireon::nav::Point	direction2D( -1.0000,       0.0000,       0.0000 );

			 ireon::nav::Triangle* curentTriangle = ptr->getTriangleForPoint(curentPosition2D);
			 ireon::nav::Position curentPositionOnMesh2D(curentPosition2D, curentTriangle);

			 ireon::nav::Position newPositionOnMesh2D( ptr->walk(curentPositionOnMesh2D, direction2D * m_walkSpeed, 1) );
			 ireon::nav::Point newPosition3D = newPositionOnMesh2D.getPoint3D();

			 ireon::nav::Point x (newPosition3D.toVector3());

			 //BOOST_CHECK(newPosition3D == ireon::nav::Point(-22.2, 0, -42.3));

		 }
		{
			
			BOOST_CHECKPOINT("Test WALK 2");
			ireon::nav::Point p(-21,       5.8,     -17);
			//ireon::nav::Point k = ptr->GetPointOnMesh(p);
			ireon::nav::Point k = p;
			ireon::nav::Triangle* oldT = NULL;
			ireon::nav::Triangle* T, *R = NULL;
			for (int i=0;i<10; i++)
			{
				k = k + ireon::nav::Point(-0.1f,0,0);
				try
				{
					R = ptr->getTriangleForPoint(k);
					T = R;
				}
				catch (ireon::nav::Mesh::ETriangleNotFound&)
				{
					printf("except - %15.6f, %15.6f, %15.6f \n",(float)k.x, (float)k.y, (float)k.z);
					T = R;
				}
				
				if (T != oldT)
				{
					printf("changed - %15.6f, %15.6f, %15.6f \n",(float)k.x, (float)k.y, (float)k.z);
					oldT = T;
				}
			}
			printf("end - %15.6f, %15.6f, %15.6f \n",(float)k.x, (float)k.y, (float)k.z);

		
		}
		



	}
}
//-----------------------------------------------------------------------

void testMeshConstructor()
{
	std::auto_ptr<ireon::nav::Mesh> ptr;
	ptr.reset(new ireon::nav::Mesh("../data/proto/zones/navigation_mesh.xml"));
	//const ireon::nav::Triangle* T = ptr->getTriangleBuffer();
	//BOOST_CHECK(T[1].edge(2)->point(1).toVector3() == T[2].edge(2)->point(0).toVector3() && T[1].edge(2)->point(0).toVector3() == T[2].edge(2)->point(1).toVector3());

}

//-----------------------------------------------------------------------

class AA
{
public:
	explicit  AA(int A, int B = 10):B_(B){}
	AA& operator* (const AA& rvl){return *this;}
	int B_;
};
test_suite* init_unit_test_suite( int /*argc*/, char* /*argv*/[] ) {
	log4cxx::BasicConfigurator::configure();
	log4cxx::LoggerPtr logger = log4cxx::Logger::getRootLogger();
	log4cxx::NDC::push("");

	test_suite* test = BOOST_TEST_SUITE("Mesh test");
	std::cout << "sizeof(int) = "<<sizeof(int)<<std::endl;
	std::cout << "sizeof(long) = "<<sizeof(long)<<std::endl;
	std::cout << "sizeof(long long) = "<<sizeof(long long)<<std::endl;
	std::cout << "sizeof(double) = "<<sizeof(double)<<std::endl;
	std::cout << "sizeof(int64) = "<<sizeof(int64)<<std::endl;
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetReportMode( _CRT_ERROR, _CRTDBG_MODE_DEBUG );
	test->add( BOOST_TEST_CASE( &testMeshConstructor ) );
	test->add( BOOST_TEST_CASE( &testUtilityFunction ) );
	test->add( BOOST_TEST_CASE( &testWalk ) );
	// real N  = numer_block - 1 
	// because boost generate yet 'new'
	//boost::detect_memory_leak(449872);
	AA xx(23);
	AA r = xx * 25;

	return test;
}
