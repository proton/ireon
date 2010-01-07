/**
 * @file py_editor.cpp
 * Export editor classes and constants to python
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

#define BOOST_PYTHON_STATIC_MODULE
#include "boost/python.hpp"
using namespace boost::python;

BOOST_PYTHON_MODULE(Editor)
{
	enum_< EditorEvent >
		("EventID");

	class_< CEditorInterface, bases<CInterface> >
		("CEditorInterface",no_init);

	class_<CEditorWorld, bases<CWorld> >
		("CEditorWorld",no_init);

	def("getInterface",&CEditorInterface::instance,return_value_policy<reference_existing_object>());
	def("getWorld",&CEditorWorld::instance,return_value_policy<reference_existing_object>());
};

void initEditorModule()
{
	initEditor();
};