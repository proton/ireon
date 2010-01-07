/**
  * @file py_editor_app.cpp
  * Export app class to python
  */
 
 /* Copyright (C) 2005 ireon.org developers council
  * $Id: py_editor_app.cpp 752 2006-09-23 19:12:44Z zak $
 
  *  See LICENSE for details
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
 	void write(String arg) {CLog::instance()->log(CLog::msgFlagPython,CLog::msgLvlInfo,__FUNCTION__,arg.c_str());}
 };
 
 BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(CApp_overloads1, getSetting, 1, 1)
 BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(CApp_overloads2, setSetting, 2, 2)
 
 BOOST_PYTHON_MODULE(App)
 {
 
 	class_<StringVector>("StringVector")
 	.def( vector_indexing_suite< std::vector<std::string>, true>());
 
	typedef std::vector<byte> ByteVector;
 	class_<ByteVector>("ByteVector")
 	.def( vector_indexing_suite< std::vector<byte> >());

	typedef  std::vector<StaticPtr> StaticVector;
	class_<StaticVector>("StaticVector")
 		.def( vector_indexing_suite< std::vector<StaticPtr>, true >());
 
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
