/* Copyright (C) 2005 ireon.org developers council
 * $Id: synchronizer.h 672 2006-08-16 18:33:53Z zak $

 *  See LICENSE for details
 */

/**
 * @file synchronizer.h
 * Objects synchronization
 */
#ifndef _SYNCHRONIZER_H
#define _SYNCHRONIZER_H

class CData;

class CSynchronizer
{
public:
	CSynchronizer():m_syncInt(1),m_needUpdateSync(false){}

	bool checkSync(int64 sync) {return m_syncInt > sync;}
	void needSync() {m_needUpdateSync = true;}
	void updSync() {if( m_needUpdateSync){ m_syncInt++; m_needUpdateSync = false;}}
	int64 getSync() {return m_syncInt;}

	void serialize(CData& data);

protected:
	int64 m_syncInt;
	bool m_needUpdateSync;
};

#endif

