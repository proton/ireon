/* Copyright (C) 2005 ireon.org developers council
* $Id: common_stdafx.h 829 2006-11-30 18:39:37Z mip $

*  See LICENSE for details
*/

/**
* @file stdafx.h
* Precompiled headers
*/
#ifndef _COMMON_STDAFX_H
#define _COMMON_STDAFX_H

#include "common/system.h"
#include "common/define.h"

#pragma warning(push)
#pragma warning(disable:4201 4510 4610) // disable 'nonstandard extension used : nameless struct/union' warning
#define	_OGRE_TYPES
#include "common/types.h"
#pragma warning(pop)

#include "common/constants.h"
#include "common/log_mixin.h"

#endif
