/**
  * @file py_editor.cpp
  * Export editor classes and constants to python
  */
 
 /* Copyright (C) 2005 ireon.org developers council
  * $Id: py_editor.cpp 693 2006-08-31 08:02:35Z zak $
 
  *  See LICENSE for details
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
 		("CEditorInterface",no_init)
		.def("enableControl", &CEditorInterface::enableControl );
 
 	class_<CEditorWorld, bases<CWorld> >
 		("CEditorWorld",no_init);
 
 	def("getInterface",&CEditorInterface::instance,return_value_policy<reference_existing_object>());
 	def("getWorld",&CEditorWorld::instance,return_value_policy<reference_existing_object>());
 };
 
 void initEditorModule()
 {
 	initEditor();
 };