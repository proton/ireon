/**
* @file common/kd_tree.cpp
* A general purpose KdTree which
*		a) holds 3D objects of generic type
*		b) supports ray tracing
*/

/* Copyright (C) 2005-2006 ireon.org developers council
* $Id: mesh.h 863 2007-07-06 11:51:41Z mip $

* See LICENSE for details
*/

#include "common/common_stdafx.h"
#include "common/kd_tree.h"

namespace ireon
{
	namespace nav
	{
		struct Triangle;
	}
}

using namespace ireon::kd_tree;

template <class T>
MManager<T>* KdTree<T>::s_MManager = 0;

#define MAXTREEDEPTH	20


ireon::kd_tree::Ray::Ray( Vector3& a_Origin, Vector3& a_Dir ) : 
m_Origin( a_Origin ), 
m_Direction( a_Dir )
{
}

// -----------------------------------------------------------
// KdTree class implementation
// -----------------------------------------------------------
template <class T>
ireon::kd_tree::KdTree<T>::KdTree():
sList_(NULL)
{
	m_Modx = new int[64];
	m_Mod = (int*)((((unsigned long)m_Modx) + 32) & (0xffffffff - 31));
	m_Mod[0] = 0, m_Mod[1] = 1, m_Mod[2] = 2, m_Mod[3] = 0, m_Mod[4] = 1;

	root_ = new KdTreeNode<T>();
}
template <class T>
ireon::kd_tree::KdTree<T>::~KdTree()
{
	delete[] m_Modx;
	delete root_;
	delete s_MManager;
}

template <class T>
void ireon::kd_tree::KdTree<T>::build(T* bufferOfPrimitives, uint bufSize, const aabb* extendBox)
{
	
	for ( uint i = 0; i < bufSize; ++i )
		root_->add( bufferOfPrimitives + i );
	
	
	//aabb sbox =  ireon::nav::Mesh::GetExtends();
	
	SplitList* foo = sPool_ = new SplitList[bufSize * 2 + 8];
	
	// линкуем все примитивы в списке
	for ( uint i = 0; i < (bufSize * 2 + 6); ++i ) 
		sPool_[i].next = &sPool_[i + 1];
		
	
	subdivide( root_, *extendBox, 0, bufSize );
	delete[] foo;
}

template <class T>
void ireon::kd_tree::KdTree<T>::insertSplitPos( real a_SplitPos )
{
	// insert a split position candidate in the list if unique
	SplitList* entry = sPool_;
	sPool_ = sPool_->next;
	entry->next = 0;
	entry->splitpos = a_SplitPos;
	entry->n1count = 0;
	entry->n2count = 0;
	if (!sList_) sList_ = entry; else
	{
		if (a_SplitPos < sList_->splitpos)
		{
			entry->next = sList_;
			sList_ = entry;
		}
		else if (a_SplitPos == sList_->splitpos)
		{
			entry->next = sPool_; // redundant; recycle
			sPool_ = entry;
		}
		else
		{
			SplitList* list = sList_;
			while ((list->next) && (a_SplitPos >= list->next->splitpos)) 
			{
				if (a_SplitPos == list->next->splitpos)
				{
					entry->next = sPool_; // redundant; recycle
					sPool_ = entry;
					return;
				}
				list = list->next;
			}
			entry->next = list->next;
			list->next = entry;
		}
	}
}

template <class T>
void ireon::kd_tree::KdTree<T>::subdivide( KdTreeNode<T>* node, const aabb& box, int depth, int a_Prims )
{
	// recycle used split list nodes
	//add sPool_ at end sList_
	if (sList_)
	{
		SplitList* list = sList_;
		while (list->next) 
			list = list->next;
		list->next = sPool_;
		sPool_ = sList_, sList_ = 0;
	}

	// determine split axis
	Vector3 s = box.getSize();
	if ((s.x >= s.y) && (s.x >= s.z)) 
		node->setAxis( 0 );
	else if ((s.y >= s.x) && (s.y >= s.z))
		node->setAxis( 1 );
	
	int axis = node->getAxis();
	
	// make a list of the split position candidates
	ObjectList<T>* l = node->getList();
	real p1, p2;
	real pos1 = box.getPos().val[axis];
	real pos2 = box.getPos().val[axis] + box.getSize().val[axis];
	bool* pright = new bool[a_Prims];
	float* eleft = new float[a_Prims], *eright = new float[a_Prims];
	T** parray = new T*[a_Prims];
	
	real etleft, etright;
	int aidx = 0;
	while (l)
	{
		T* p = parray[aidx] = l->getPrimitive();
		pright[aidx] = true;

		etleft =  eleft[aidx];
		etright = eright[aidx];
		p->calculateRange( etleft, etright, axis );
		eleft[aidx] = (float)etleft;
		eright[aidx] = (float)etright;
		aidx++;
		for ( int i = 0; i < 3; i++ )
		{
			p1 = (float)p->vertice( i )->cell[axis];
			if ((p1 >= pos1) && (p1 <= pos2)) 
				insertSplitPos( p1 );
		}
		
		l = l->getNext();
	}
	// determine n1count / n2count for each split position
	aabb b1, b2, b3 = box, b4 = box;
	SplitList* splist = sList_;
	float b3p1 = b3.getPos().val[axis];
	float b4p2 = b4.getPos().val[axis] + b4.getSize().val[axis];
	Vector3 foo;
	while (splist)
	{
		foo = b4.getPos();
		foo.val[axis] = splist->splitpos;
		b4.setPos(foo);
		foo = b4.getSize();
		foo.val[axis] = pos2 - splist->splitpos;
		b4.setSize(foo);
		foo = b3.getSize();
		foo.val[axis] = splist->splitpos - pos1;
		b3.setSize(foo);
		
		float b3p2 = b3.getPos().val[axis] + b3.getSize().val[axis];
		float b4p1 = b4.getPos().val[axis];
		for ( int i = 0; i < a_Prims; i++ ) if (pright[i])
		{
			T* p = parray[i];
			if ((eleft[i] <= b3p2) && (eright[i] >= b3p1))
				if (p->intersectBox( b3 )) splist->n1count++;
			if ((eleft[i] <= b4p2) && (eright[i] >= b4p1))
				if (p->intersectBox( b4 )) splist->n2count++; else pright[i] = false;
		}
		else splist->n1count++;
		splist = splist->next;
	}
	delete[] pright;
	// calculate surface area for current node
	real SAV = 0.5f / (box.w() * box.d() + box.w() * box.h() + box.d() * box.h());
	// calculate cost for not splitting
	real Cleaf = a_Prims * 1.0f;
	// determine optimal split plane position
	splist = sList_;
	real lowcost = 10000;
	real bestpos = 0;
	while (splist)
	{
		// calculate child node extends
		foo = b4.getPos();
		foo.val[axis] = splist->splitpos;
		b4.setPos(foo);
		foo = b4.getSize();
		foo.val[axis] = pos2 - splist->splitpos;
		b4.setSize(foo);
		foo = b3.getSize();
		foo.val[axis] = splist->splitpos - pos1;
		b3.setSize(foo);

		// calculate child node cost
		real SA1 = 2 * (b3.w() * b3.d() + b3.w() * b3.h() + b3.d() * b3.h());
		real SA2 = 2 * (b4.w() * b4.d() + b4.w() * b4.h() + b4.d() * b4.h());
		real splitcost = 0.3f + 1.0f * (SA1 * SAV * splist->n1count + SA2 * SAV * splist->n2count);
		// update best cost tracking variables
		if (splitcost < lowcost)
		{
			lowcost = splitcost;
			bestpos = splist->splitpos;
			b1 = b3, b2 = b4;
		}
		splist = splist->next;
	}
	if (lowcost > Cleaf) 
	{
		delete[] eleft;
		delete[] eright;
		delete[] parray;
		return;
	}
		node->setSplitPos( bestpos );
	// construct child nodes
	KdTreeNode<T>* left = s_MManager->NewKdTreeNodePair();
	int n1count = 0, n2count = 0, total = 0;
	// assign primitives to both sides
	float b1p1 = b1.getPos().val[axis];
	float b2p2 = b2.getPos().val[axis] + b2.getSize().val[axis];
	float b1p2 = b1.getPos().val[axis] + b1.getSize().val[axis];
	float b2p1 = b2.getPos().val[axis];
	for ( int i = 0; i < a_Prims; i++ )
	{
		T* p = parray[i];
		total++;
		if ((eleft[i] <= b1p2) && (eright[i] >= b1p1)) if (p->intersectBox( b1 )) 
		{
			left->add( p );
			n1count++;
		}
		if ((eleft[i] <= b2p2) && (eright[i] >= b2p1)) if (p->intersectBox( b2 )) 
		{
			(left + 1)->add( p );
			n2count++;
		}
	}
	delete[] eleft;
	delete[] eright;
	delete[] parray;
	
	s_MManager->FreeObjectList( node->getList() );
	
	node->setLeft( left );
	node->setLeaf( false );
	if (depth < MAXTREEDEPTH)
	{
		if (n1count > 2) subdivide( left, b1, depth + 1, n1count );
		if (n2count > 2) subdivide( left + 1, b2, depth + 1, n2count );
	}
}

// -----------------------------------------------------------
// KdTreeNode class implementation
// -----------------------------------------------------------
template <class T>
void ireon::kd_tree::KdTreeNode<T>::add(T* primitive )
{
	// add at begin object list  primitive
	ObjectList<T>* lnode = KdTree<T>::s_MManager->NewObjectList();
	lnode->setPrimitive( primitive );
	lnode->setNext( getList() );
	setList( lnode );
}

////----------------------------------------------

template <class T>
ireon::kd_tree::MManager<T>::MManager() : m_OList( 0 )
{
	// build a 32-byte aligned array of KdTreeNodes
	m_KdArray = (char*)(new KdTreeNode<T>[1000000]);
	m_ObjArray = (char*)(new ObjectList<T>[100000]);
	unsigned long addr = (unsigned long)m_KdArray;
	m_KdPtr = (KdTreeNode<T>*)((addr + 32) & (0xffffffff - 31));
	addr = (unsigned long)m_ObjArray;
	m_ObjPtr = (ObjectList<T>*)((addr + 32) & (0xffffffff - 31));
	ObjectList<T>* ptr = m_ObjPtr;
	for ( int i = 0; i < 99995; i++ ) 
	{
		ptr->setNext( ptr + 1 );
		ptr++;
	}
	ptr->setNext( 0 );
	m_OList = m_ObjPtr;
}
template <class T>
ireon::kd_tree::MManager<T>::~MManager() 
{
	delete[] (KdTreeNode<T>*)(m_KdArray);
	delete[] (ObjectList<T>*)(m_ObjArray);
}


template <class T>
ObjectList<T>* ireon::kd_tree::MManager<T>::NewObjectList()
{
	ObjectList<T>* retval;
	retval = m_OList;
	m_OList = m_OList->getNext();
	retval->setNext( 0 );
	retval->setPrimitive( 0 );
	return retval;
}

template <class T>
void ireon::kd_tree::MManager<T>::FreeObjectList( ObjectList<T>* a_List )
{
	ObjectList<T>* list = a_List;
	while (list->getNext()) list = list->getNext();
	list->setNext( m_OList );
	m_OList = a_List;
}

template <class T>
KdTreeNode<T>* ireon::kd_tree::MManager<T>::NewKdTreeNodePair()
{ 
	unsigned long* tmp = (unsigned long*)m_KdPtr;
	tmp[1] = tmp[3] = 6;
	KdTreeNode<T>* node = m_KdPtr;
	m_KdPtr += 2;
	return node;
}


template <class T>
int intersect(T* pr, Ray& a_Ray, real& a_Dist )
{

	unsigned int modulo[] = { 0, 1, 2, 0, 1 };
#define ku modulo[pr->k + 1]
#define kv modulo[pr->k + 2]
		Vector3 O = a_Ray.getOrigin(), D = a_Ray.getDirection(), A = pr->vertice(0)->toVector3();
		const real lnd = 1.0f / (D.val[pr->k] + pr->nu * D.val[ku] + pr->nv * D.val[kv]);
		const real t = (pr->nd - O.val[pr->k] - pr->nu * O.val[ku] - pr->nv * O.val[kv]) * lnd;
		if (!(a_Dist > t && t > 0)) return MISS;
		real hu = O.val[ku] + t * D.val[ku] - A.val[ku];
		real hv = O.val[kv] + t * D.val[kv] - A.val[kv];
		real beta = pr->m_U = hv * pr->bnu + hu * pr->bnv;
		if (beta < 0) return MISS;
		real gamma = pr->m_V = hu * pr->cnu + hv * pr->cnv;
		if (gamma < 0) return MISS;
		if ((pr->m_U + pr->m_V) > 1) return MISS;
		a_Dist = t;
		return ( D.dotProduct( pr->m_N)  > 0)?INPRIM:HIT;

}

template <class T>
int ireon::kd_tree::KdTree<T>::FindNearest( Ray& a_Ray, real& a_Dist, T*& a_Prim ,  kdstack<T>  * const m_Stack, const aabb* extendBox)
{
	real tnear = 0, tfar = a_Dist, t;
	int retval = 0;
	Vector3 p1 = extendBox->getPos();
	Vector3 p2 = p1 + extendBox->getSize();
	Vector3 D = a_Ray.getDirection(), O = a_Ray.getOrigin();
	for ( int i = 0; i < 3; i++ ) if (D.val[i] < 0) 
	{
		if (O.val[i] < p1.val[i]) return 0;
	}
	else if (O.val[i] > p2.val[i]) return 0;
	// clip ray segment to box
	for ( int i = 0; i < 3; i++ )
	{
		real pos = O.val[i] + tfar * D.val[i];
		if (D.val[i] < 0)
		{
			// clip end point
			if (pos < p1.val[i]) tfar = tnear + (tfar - tnear) * ((O.val[i] - p1.val[i]) / (O.val[i] - pos));
			// clip start point
			if (O.val[i] > p2.val[i]) tnear += (tfar - tnear) * ((O.val[i] - p2.val[i]) / (tfar * D.val[i]));
		}
		else
		{
			// clip end point
			if (pos > p2.val[i]) tfar = tnear + (tfar - tnear) * ((p2.val[i] - O.val[i]) / (pos - O.val[i]));
			// clip start point
			if (O.val[i] < p1.val[i]) tnear += (tfar - tnear) * ((p1.val[i] - O.val[i]) / (tfar * D.val[i]));
		}
		if (tnear > tfar) return 0;
	}
	// init stack
	int entrypoint = 0, exitpoint = 1;
	// init traversal
	KdTreeNode<T>* farchild, *currnode;
	currnode = getRoot();
	m_Stack[entrypoint].t = tnear;
	if (tnear > 0.0f) m_Stack[entrypoint].pb = O + D * tnear;
	else m_Stack[entrypoint].pb = O;
	m_Stack[exitpoint].t = tfar;
	m_Stack[exitpoint].pb = O + D * tfar;
	m_Stack[exitpoint].node = 0;
	// traverse kd-tree
	while (currnode)
	{
		while (!currnode->isLeaf())
		{
			real splitpos = currnode->getSplitPos();
			int axis = currnode->getAxis();
			if (m_Stack[entrypoint].pb.val[axis] <= splitpos)
			{
				if (m_Stack[exitpoint].pb.val[axis] <= splitpos)
				{
					currnode = currnode->getLeft();
					continue;
				}
				if (m_Stack[exitpoint].pb.val[axis] == splitpos)
				{
					currnode = currnode->getRight();
					continue;
				}
				currnode = currnode->getLeft();
				farchild = currnode + 1; // GetRight();
			}
			else
			{
				if (m_Stack[exitpoint].pb.val[axis] > splitpos)
				{
					currnode = currnode->getRight();
					continue;
				}
				farchild = currnode->getLeft();
				currnode = farchild + 1; // GetRight();
			}
			t = (splitpos - O.val[axis]) / D.val[axis];
			int tmp = exitpoint++;
			if (exitpoint == entrypoint) exitpoint++;
			m_Stack[exitpoint].prev = tmp;
			m_Stack[exitpoint].t = t;
			m_Stack[exitpoint].node = farchild;
			m_Stack[exitpoint].pb.val[axis] = splitpos;
			int nextaxis = m_Mod[axis + 1];
			int prevaxis = m_Mod[axis + 2];
			m_Stack[exitpoint].pb.val[nextaxis] = O.val[nextaxis] + t * D.val[nextaxis];
			m_Stack[exitpoint].pb.val[prevaxis] = O.val[prevaxis] + t * D.val[prevaxis];
		}
		ObjectList<T>* list = currnode->getList();
		real dist = m_Stack[exitpoint].t;
		while (list)
		{
			T* pr = list->getPrimitive();
			int result;
			m_Intersections++;
			if (result = intersect( pr, a_Ray, dist ))
			{
				retval = result;
				a_Dist = dist;
				a_Prim = pr;
			}
			list = list->getNext();
		}
		if (retval) return retval;
		entrypoint = exitpoint;
		currnode = m_Stack[exitpoint].node;
		exitpoint = m_Stack[entrypoint].prev;
	}
	return 0;
}



