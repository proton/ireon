/**
 * @file common/file/directory.cpp
 * CDirectory realization
 */

/* Copyright (C) 2005-2006 ireon.org developers council
 * portions (C) Radon Labs GmbH, www.nebuladevice.org
 * $Id: directory.cpp 565 2006-03-22 16:50:54Z zak $

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


#include "common/common_stdafx.h"
#include "common/file/directory.h"
#include "common/file/fs.h"
#include "common/file/file.h"
#include "common/misc/boost_string_algo_wrapper.h"
//#include "net/data.h"

CDirectory::CDirectory(const String& path) :
CResourceLocation(path),
#ifdef __WIN32__
	m_handle(0),
#elif defined(__UNIX__)
	m_dir(NULL),
	m_dp(NULL),
#endif
	m_empty(true)
{
};

CDirectory::~CDirectory()
{
	if (this->isOpen())
	{
		this->close();
	}
}

bool CDirectory::open(const String& dirName)
{
		assert(!this->isOpen());

		assert(strlen(dirName.c_str())>0);

		this->m_path	=	CFS::instance()->realPath(dirName.c_str());

#ifdef __WIN32__
		DWORD	attr;
		bool retval;
		this->m_handle	=	NULL;

		attr = GetFileAttributes(this->m_path.c_str());
		if ((attr	!= 0xffffffff) &&	(attr	&	FILE_ATTRIBUTE_DIRECTORY))
		{
				retval = true;
				this->m_empty = !(this->setToFirstEntry());
		}
		else
		{
				retval = false;
				this->m_path.clear();
		}

		return retval;
#elif defined(__UNIX__)
		m_dir = opendir(m_path.c_str());
		if( m_dir )
		{
			m_empty = !setToFirstEntry();
		} else
			m_path.clear();
		return false;
#endif
}

void CDirectory::close()
{
		assert(this->isOpen());

#ifdef __WIN32__
		if (this->m_handle) 
		{
				FindClose(this->m_handle);
				this->m_handle	=	NULL;
		}
#elif defined(__UNIX__)
		closedir(m_dir);
#endif

		this->m_path.clear();
}

bool CDirectory::isEmpty()
{
		assert(this->isOpen());
		return this->m_empty;
}

bool CDirectory::setToFirstEntry()
{
		assert(this->isOpen());

#ifdef __WIN32__
		if (this->m_handle)
		{
				FindClose(this->m_handle);
		}

		String tmpName = this->m_path;
		tmpName	+= "\\*.*";
		this->m_handle	=	FindFirstFile(tmpName.c_str(), &(this->m_findData));

		if (this->m_handle	== INVALID_HANDLE_VALUE)
		{
				return false;
		}

		while	((strcmp(this->m_findData.cFileName,	"..")	==0) ||	(strcmp(this->m_findData.cFileName, ".")	== 0))
		{
				if (!FindNextFile(this->m_handle, &this->m_findData))
				{
						return false;
				}
		}
		return true;
#elif defined(__UNIX__)
	rewinddir(m_dir);
	m_dp = readdir(m_dir);
	if( m_dp == NULL ) 
		return false;
	while((strcmp(m_dp->d_name,"..") == 0) ||	(strcmp(m_dp->d_name, ".") == 0))
	{
		if (!(m_dp = readdir(m_dir)))
		{
			return false;
		}
	}
	return true;
#endif
}

bool CDirectory::setToNextEntry()
{
		assert(this->isOpen());

#ifdef __WIN32__
		assert(this->m_handle);
		assert(this->m_handle !=	INVALID_HANDLE_VALUE);

		bool suc = (FindNextFile(this->m_handle,	&(this->m_findData))	!= 0)	?	true : false;

		return suc;
#elif defined(__UNIX__)
	m_dp = readdir(m_dir);
	if( m_dp == NULL ) 
		return false;
	while((strcmp(m_dp->d_name,"..") == 0) ||	(strcmp(m_dp->d_name, ".") == 0))
	{
		if (!(m_dp = readdir(m_dir)))
		{
			return false;
		}
	}
	return true;
#endif
}

const	char*	CDirectory::getEntryName()
{
		assert(this->isOpen());

#ifdef __WIN32__
		assert(this->m_handle);
		assert(this->m_handle !=	INVALID_HANDLE_VALUE);

		this->m_apath = this->m_path;
		this->m_apath +=	"/";
		this->m_apath +=	this->m_findData.cFileName;
		return this->m_apath.c_str();
#else
		assert(m_dp);
		m_apath = m_path;
		m_apath += "/";
		m_apath += m_dp->d_name;
		return m_apath.c_str();
		return 0;
#endif
}

CDirectory::EntryType CDirectory::getEntryType()
{
		assert(this->isOpen());

#ifdef __WIN32__
		assert(this->m_handle);
		assert(this->m_handle !=	INVALID_HANDLE_VALUE);

		if(this->m_findData.dwFileAttributes	&	FILE_ATTRIBUTE_DIRECTORY)
				return DIRECTORY;
		else
				return FILE;
#elif defined(__UNIX__)
		assert(m_dp);
		if( m_dp->d_type == DT_DIR )
			return DIRECTORY;
		else 
			return FILE;
#endif
}

void CDirectory::list(StringVector* res)
{
	if( !isOpen() )
		return;
	if (setToFirstEntry())
	{
		do{
			if( getEntryType() == CDirectory::FILE )
			{
				String name = getEntryName();
				boost::algorithm::erase_head(name,(unsigned int) name.find_last_of('/')+1);
				res->push_back(name);
			};
		} while (setToNextEntry());
	}
};

DataPtr CDirectory::load(const String& name)
{
	if( !isOpen() )
		return DataPtr();
	String fullName = getPathName();
	fullName += "/";
	fullName += name;
	CFile f;
	f.open(fullName.c_str(),"rb");
	if( !f.isOpen() )
	{
		CLog::instance()->log(CLog::msgLvlError,_("Can't open requested resource '%s'."),fullName.c_str());
		return DataPtr();
	}
	DataPtr d(new CData(&f));
	return d;
};
