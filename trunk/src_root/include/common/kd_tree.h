/**
* @file common/kd_tree.h
* A general purpose KdTree which
*		a) holds 3D objects of generic type
*		b) supports ray tracing
*
* later will be used http://math.ucsd.edu/~sbuss/MathCG 
*/

/* Copyright (C) 2005-2006 ireon.org developers council
* $Id: mesh.h 863 2007-07-06 11:51:41Z mip $

* See LICENSE for details
*/

#ifndef KDTREE_H
#define KDTREE_H

//#include "common/common_stdafx.h"
#include "common/log_mixin.h"
#define real double 

namespace ireon
{
	namespace kd_tree
	{


		// Intersection method return values
#define HIT		 1		// Ray hit primitive
#define MISS	 0		// Ray missed primitive
#define INPRIM	-1		// Ray started inside primitive


		class aabb
		{
		public:
			aabb() : pos_( Vector3( 0, 0, 0 ) ), size_( Vector3( 0, 0, 0 ) ) {};
			aabb( Vector3& a_Pos, Vector3& a_Size ) : pos_( a_Pos ), size_( a_Size ) {};
			
			const Vector3& getPos() const { return pos_; } 
			const Vector3& getSize() const { return size_; }
			void setPos(const Vector3& pos)  { pos_ = pos; } 
			void setSize(const Vector3& size){ size_ = size; }
			bool intersect( aabb& b2 )
			{
				Vector3 v1 = b2.getPos(), v2 = b2.getPos() + b2.getSize();
				Vector3 v3 = pos_, v4 = pos_ + size_;
				return ((v4.x >= v1.x) && (v3.x <= v2.x) && // x-axis overlap
					(v4.y >= v1.y) && (v3.y <= v2.y) && // y-axis overlap
					(v4.z >= v1.z) && (v3.z <= v2.z));   // z-axis overlap
			}

			bool contains( Vector3 pos )
			{
				Vector3 v1 = pos_, v2 = pos_ + size_;
				return ((pos.x >= v1.x) && (pos.x <= v2.x) &&
					(pos.y >= v1.y) && (pos.y <= v2.y) &&
					(pos.z >= v1.z) && (pos.z <= v2.z));
			}

			real w() const { return size_.x; }
			real h() const { return size_.y; }
			real d() const { return size_.z; }
			real x() const { return pos_.x; }
			real y() const { return pos_.y; }
			real z() const { return pos_.z; }
		private:
			Vector3 pos_, size_;
		};

		// -----------------------------------------------------------
		// Ray class definition
		// -----------------------------------------------------------
		class Ray
		{
		public:
			Ray() : m_Origin( Vector3( 0, 0, 0 ) ), m_Direction( Vector3( 0, 0, 0 ) ) {};
			Ray( Vector3& a_Origin, Vector3& a_Dir );
			void setOrigin( Vector3& a_Origin ) { m_Origin = a_Origin; }
			void setDirection( Vector3& a_Direction ) { m_Direction = a_Direction; }
			Vector3& getOrigin() { return m_Origin; }
			Vector3& getDirection() { return m_Direction; }
		private:
			Vector3 m_Origin;
			Vector3 m_Direction;
		};


		// -----------------------------------------------------------
		// Object list helper class
		// -----------------------------------------------------------

		template <class T>
		class ObjectList
		{
		public:
			ObjectList() : m_Primitive( 0 ), m_Next( 0 ) {}
			~ObjectList() { /*delete m_Next;*/ }
			void setPrimitive( T* a_Prim ) { m_Primitive = a_Prim; }
			T* getPrimitive() { return m_Primitive; }
			void setNext( ObjectList<T>* a_Next ) { m_Next = a_Next; }
			ObjectList* getNext() { return m_Next; }
		private:
			T* m_Primitive;
			ObjectList<T>* m_Next;
		};

		// -----------------------------------------------------------
		// Engine class definition
		// Raytracer core
		// -----------------------------------------------------------
		template <class T>
		class KdTreeNode
		{
		public:
			//typedef ObjectList std::list<T> 
			KdTreeNode() : data_( 6 ) {};
			
			void setAxis( int axis ) { data_ = (data_ & 0xfffffffc) + axis; }
			int getAxis() { return data_ & 3; }
			
			void setSplitPos( real pos ) { split_ = pos; }
			real getSplitPos() { return split_; }
			
			void setLeft( KdTreeNode* left ) { data_ = (unsigned long)left + (data_ & 7); }
			KdTreeNode* getLeft() { return (KdTreeNode*)(data_&0xfffffff8); }
			KdTreeNode* getRight() { return ((KdTreeNode*)(data_&0xfffffff8)) + 1; }
			
			void add(T* primitive );
			bool isLeaf() { return ((data_ & 4) > 0); }
			void setLeaf( bool leaf ) { data_ = (leaf)?(data_|4):(data_&0xfffffffb); }
			
			//ObjectList* getList() { return (ObjectList*)(data_&0xfffffff8); }
			ObjectList<T>* getList() { return (ObjectList<T>*)(data_&0xfffffff8); }
			
			void setList( ObjectList<T>* list ) { data_ = (unsigned long)list + (data_ & 7); }
		private:
			// int m_Flags;
			real split_;
			unsigned long data_;
		};

		template <class T>
		class MManager
		{
		public:
			MManager();
			~MManager();
			ObjectList<T>* NewObjectList();
			void FreeObjectList( ObjectList<T>* a_List );
			KdTreeNode<T>* NewKdTreeNodePair();
		private:
			ObjectList<T>* m_OList;
			char* m_KdArray, *m_ObjArray;
			KdTreeNode<T>* m_KdPtr;
			ObjectList<T>* m_ObjPtr;
			int m_KdUsed;
		};
		struct SplitList
		{
			SplitList():next(NULL){};
			real splitpos;
			int n1count, n2count;
			SplitList* next;
		};

		template <class T>
		struct kdstack
		{
			KdTreeNode<T>* node;
			real t;
			Vector3 pb;
			int prev, dummy1, dummy2;
		};


		template <class T>
		class KdTree
		{
		public:
			KdTree();
			~KdTree();

			// buld kd-tree ... generating tree from buffer of primitives
			void build(T* bufferOfPrimitives, uint bufSize, const aabb* extendBox);
			
			KdTreeNode<T>* getRoot() { return root_; }
			void setRoot( KdTreeNode<T>* a_Root ) { root_ = a_Root; }

			int FindNearest( Ray& a_Ray, real& a_Dist, T*& a_Prim,  kdstack<T> * const m_Stack, const aabb* extendBox );

		
			static void SetMemoryManager( MManager<T>* a_MM ) { s_MManager = a_MM; }
		private:
			// tree generation
			void insertSplitPos( real a_SplitPos );
			void subdivide( KdTreeNode<T>* node, const aabb& box, int depth, int a_Prims );
			
		private:
			// узлы дерева
			KdTreeNode<T>* root_;
			//
			SplitList* sList_, *sPool_;
			// fast %
			int* m_Mod, *m_Modx;

			// counters
			int m_RaysCast, m_Intersections;
		public:
			static MManager<T>* s_MManager;
		};


		

	}
}

#endif // KDTREE_H