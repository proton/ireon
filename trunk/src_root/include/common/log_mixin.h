/**
* @file common/log_mixin.h
* Mixin class with logging  functions
*/

/* Copyright (C) 2005-2006 ireon.org developers council
* $Id: log_mixin.h 781 2006-10-07 19:52:08Z zak $

* See LICENSE for details
*/

#pragma once

#include "common/common_stdafx.h"

#pragma warning(push)
#pragma warning(disable:4250)
#include <log4cxx/logger.h>
#pragma warning(pop)

#define _loc LOG4CXX_LOCATION
#define _fatal ::log4cxx::Level::FATAL
#define _error ::log4cxx::Level::ERROR
#define _warn ::log4cxx::Level::WARN
#define _info ::log4cxx::Level::INFO
#define _debug ::log4cxx::Level::DEBUG

class CLogMixin
{
protected:
	CLogMixin();
	explicit CLogMixin(const char* const loggerName);

	void _log(const ::log4cxx::LevelPtr& lvl, const ::log4cxx::spi::LocationInfo& loc, const char *msg, ...);
	void _pushNDC(const String& context);
	void _popNDC();

private:
	log4cxx::LoggerPtr m_logger;
};

