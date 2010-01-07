/**
  * @file object_manager.cpp
  * Editor world manager
  */
 
 /* Copyright (C) 2005 ireon.org developers council
  * $Id: client_static_collection.cpp 715 2006-09-10 20:12:50Z mip $
 
  *  See LICENSE for details
  */
#include "stdafx.h"
#include "common/world/managers/static_collection.h"
//#include "common/world/static_object/client_static.h"

CStaticCollection::CStaticCollection():
CGenericCollection<CStaticCollection, CStaticObject>("StaticCollection")
{

}

///*
//bool CClientStaticCollection::cloneStaticObject(const String& objectName)
//{
//	std::map< uint, CClientStaticObject >::iterator it = m_staticObjects.find( objectName );
//	if it == m_staticObjects.end()
//		return false;
//};*/
//
//
//bool CClientStaticCollection::create(StaticPrototypePtr& prot)
//{
//	StaticPtr st;
//	if( !prot )
//		return false;
//    st.reset( new CClientStaticObject(prot) );
//	m_staticObjects.insert(std::map< uint, CClientStaticObject >::value_type(st->getId(), st));
//	return true;
//};*
//
//
//
//CClientStaticCollection::serialize(vector<CData>& d)
//{
//	/// обработать все обьекты (статики) в сцене и в мире
//	/// сериализовать их
//	/// 
//
//};

//----------------------------------------------------------

