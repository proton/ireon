/**
 * @file py_editor_app.cpp
 * Export app class to python
 */

/* Copyright (C) 2005 ireon.org developers council
 * $Id$

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

#include "stdafx.h"
#include "db/client_char_data.h"

#define BOOST_PYTHON_STATIC_MODULE
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
using namespace boost::python;

class CPythonOutput
{
public:
	void write(String arg) {CLog::instance()->log(CLog::msgFlagPython,CLog::msgLvlInfo,arg.c_str());}
};

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(CApp_overloads1, getSetting, 1, 1)
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(CApp_overloads2, setSetting, 2, 2)

BOOST_PYTHON_MODULE(App)
{

	class_<StringVector>("StringVector")
	.def( vector_indexing_suite< std::vector<std::string>, true>());

	class_< CPythonOutput >
		("Output")
		.def("write",&CPythonOutput::write);

	class_< CData >
		("Data");
		
	class_< ClientCharData, ClientCharData* >
		("ClientCharData")
		.def_readwrite("velocity",&ClientCharData::m_velocity)
		.def("serialize",&ClientCharData::serialize);

	class_< CEditorApp >
		("Application",no_init)
		.def("shutdown",&CEditorApp::shutdown)
		.def("getSetting",&CEditorApp::getSetting,CApp_overloads1())
		.def("setSetting",&CEditorApp::setSetting,CApp_overloads2())
		.def("getFPS",&CEditorApp::getFPS)
		.def("getTriCount", &CEditorApp::getTriCount )
		.def("version", &CEditorApp::getVersion);

	def("get",&CEditorApp::instance,return_value_policy<reference_existing_object>());
};

void initAppModule()
{
	initApp();
};
