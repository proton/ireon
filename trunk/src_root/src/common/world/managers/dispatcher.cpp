/**
  * @file client_zone.cpp
  * Client-side zone class
  */
 
 /* Copyright (C) 2005 ireon.org developers council
  * $Id: dispatcher.cpp 715 2006-09-10 20:12:50Z mip $
 
  *  See LICENSE for details
  */
#include "stdafx.h"
#include "common\world\managers\dispatcher.h"

CDispatcher* CDispatcher::m_singleton = NULL;

CDispatcher* CDispatcher::getSingletonPtr()
{
	if( !m_singleton )
		m_singleton = new CDispatcher();
	return m_singleton;
}

CDispatcher& CDispatcher::getSingleton()
{
	if( !m_singleton )
		m_singleton = new CDispatcher();
	return *m_singleton;
}