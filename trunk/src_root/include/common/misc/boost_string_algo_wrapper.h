/**
 * @file common/misc/boost_string_algo_wrapper.h
 * Turns off vc7.1 warning 4512 for boost/algorithm/string.hpp
 */

/* Copyright (C) 2005-2006 ireon.org developers council
 * $Id: boost_string_algo_wrapper.h 672 2006-08-16 18:33:53Z zak $

 *  See LICENSE for details
 */


#ifdef _MSC_VER
	#pragma warning(push)
	#pragma warning(disable:4512)
#endif
#include "boost/algorithm/string.hpp"
#ifdef _MSC_VER
	#pragma warning(pop)
#endif 

