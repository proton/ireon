/* Copyright (C) 2005-2006 ireon.org developers council
 * $Id: synchronizer.h 522 2006-03-06 16:49:30Z zak $

 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 */

/**
 * @file synchronizer.h
 * Objects synchronization
 */
#ifndef _SYNCHRONIZER_H
#define _SYNCHRONIZER_H

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

