/* Copyright (C) 2005 ireon.org developers council
 * $Id: common_stdafx.h 417 2005-12-18 15:10:44Z zak $

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
 * @file stdafx.h
 * Precompiled headers
 */
#ifndef _COMMON_STDAFX_H
#define _COMMON_STDAFX_H


#include <stdio.h>
#include <stdlib.h>

#include <assert.h>
//Gettext headers
#include "gettext/libintl.h"
#include <locale.h>

//STL headers
#include <numeric>

#include <vector>
#include <map>
#include <string>
#include <set>
#include <list>
#include <deque>
#include <queue>
#include <stack>

//Boost headers
#include "boost/algorithm/string.hpp"
#include "boost/shared_ptr.hpp"
#include "boost/assign.hpp"

/// Classes
class CAccount;
class CFile;
class CDirectory;
class CData;
class CAccount;

typedef boost::shared_ptr<CAccount> AccPtr;
typedef boost::shared_ptr<CFile> FilePtr;
typedef boost::shared_ptr<CDirectory> DirPtr;
typedef boost::shared_ptr<CData> DataPtr;

//ireon.org headers
#include "system.h"
#include "define.h"
#include "types.h"
#include "constants.h"
#include "net/net_types.h"
#include "log/log_listener.h"
#include "log/log.h"

#endif
