/**
 * @file common/misc/mysqlpp_wrapper.h
 * disables certain warnings in mysql++
 */

/* Copyright (C) 2005-2006 ireon.org developers council
 * $Id: mysqlpp_wrapper.h 730 2006-09-14 17:33:44Z zak $

 * See LICENSE for details
 */

#pragma once

#pragma warning( push, 3 ) // disable does not work. why?
//#pragma warning( disable : 4244 4512 )
#include <mysql++.h>
#pragma warning( pop )
