/* Copyright (C) 2005 ireon.org developers council
* $Id: synchronizer.cpp 690 2006-08-29 18:43:35Z zak $

*  See LICENSE for details
*/

/**
* @file synchronizer.cpp
* Synchronizer
*/
#include "common/common_stdafx.h"
#include "common/synchronizer.h"
#include "common/data.h"

void CSynchronizer::serialize(CData& data)
{
	if( data.store() )
	{
		data << m_syncInt;
		data << m_needUpdateSync;
	} else
	{
		data >> m_syncInt;
		data >> m_needUpdateSync;
	};
};
