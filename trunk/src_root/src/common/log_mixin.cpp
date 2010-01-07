/**
 * @file common/log_mixin.cpp
 * Mixin class with logging  functions
 */

/* Copyright (C) 2005-2006 ireon.org developers council
 * $Id: log_mixin.cpp 781 2006-10-07 19:52:08Z zak $

 * See LICENSE for details
 */

#include "common/log_mixin.h"
#include <log4cxx/ndc.h>

#ifdef __UNIX__
#include <stdarg.h>
#endif

const static byte MAX_LOG_STRING_SIZE = 128;
static char log_message_buffer[MAX_LOG_STRING_SIZE];

CLogMixin::CLogMixin()
{
	m_logger = log4cxx::Logger::getRootLogger();
}

CLogMixin::CLogMixin(const char* const loggerName)
{
	m_logger = log4cxx::Logger::getLogger(loggerName);
}

void CLogMixin::_log(const ::log4cxx::LevelPtr& lvl, const ::log4cxx::spi::LocationInfo& loc, const char *msg, ...)
{
	assert(msg);
	va_list args;
	va_start(args, msg);
	if (m_logger->isEnabledFor(lvl))
	{
		memset(log_message_buffer, 0, MAX_LOG_STRING_SIZE); 
		vsnprintf_s(log_message_buffer, sizeof(log_message_buffer)-1, _TRUNCATE, msg, args);
		m_logger->forcedLog(lvl, log_message_buffer, loc);
	}
	va_end(args);
}

void CLogMixin::_pushNDC(const String& context)
{
	log4cxx::NDC::push(context);
}

void CLogMixin::_popNDC()
{
	log4cxx::NDC::pop();
}

