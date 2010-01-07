/**
  * @file py_init.cpp
  * Init python scripts
  */
 
 /* Copyright (C) 2005 ireon.org developers council
  * $Id: py_init.cpp 752 2006-09-23 19:12:44Z zak $
 
  *  See LICENSE for details
  */
 
 #include "stdafx.h"
 #include "editor_app.h"
 #include "file/file.h"
 #include "file/directory.h"
 
 #define BOOST_PYTHON_STATIC_MODULE
 #include "boost/python.hpp"
 using namespace boost::python;
 
 extern void initEventModule();
 extern void initAppModule();
 extern void initWorldModule();
 extern void initInterfaceModule();
 extern void initEditorModule();
 
 void exportAllToPython()
 {
 	CLog::instance()->log(CLog::msgLvlInfo,__FUNCTION__,"Export to python... ");
 	try{
 		Py_Initialize();
 		initInterfaceModule();
 		initEventModule();
 		initAppModule();
 		initWorldModule();
 		initEditorModule();
 
 	} catch (error_already_set)
 	{
 		CLog::instance()->log(CLog::msgLvlError,__FUNCTION__,_("Can't initialize python."));
 		CEditorApp::instance()->shutdown();
 	};
 	CLog::instance()->log(CLog::msgLvlInfo,__FUNCTION__,"completed.\n");
 };
 
 bool loadScript(CFile* f)
 {
 	if( !f->isOpen() )
 		return false;
 	CData d;
 	d.fromFile(f);
 	d.write("\x0",1);
 	String s = d.data();
 	try{
 		object main_module((
 		handle<>(borrowed
 		(PyImport_AddModule("__main__")))));
 
 		object main_namespace =
 			main_module.attr("__dict__");
 
 		boost::algorithm::replace_all(s,"\r\n","\n");
 		handle<> ignored(PyRun_String(
 			s.c_str()
 			, Py_file_input
 			, main_namespace.ptr()
 			, main_namespace.ptr()
 			));
 	} catch(error_already_set)
 	{
 		PyErr_Print();
 		return false;
 	};
 	return true;
 };
 
 void loadScripts()
 {
 	String root = "../data/interface/"; 
 	String output = root; 
 	output.append("scripts/output.py");
 	String scripts = root; 
 	scripts.append("scripts/editor.py");
 
 	CLog::instance()->log(CLog::msgLvlInfo,__FUNCTION__,"Loading scripts.\n");
 	CData d;
 	DirPtr dir(new CDirectory);
 	FilePtr f(new CFile);
 	if(CEditorApp::instance()->getSetting("ScriptDir") == "" )
 		CEditorApp::instance()->setSetting("ScriptDir",".");
 	f->open(output.c_str(),"rb");
 	if( !(f->isOpen() && loadScript(f.get())) )
 		CLog::instance()->log(CLog::msgFlagPython | CLog::msgFlagNone,CLog::msgLvlError,__FUNCTION__,"Error loading 'output.py'.\n");
 	else
 		CLog::instance()->log(CLog::msgFlagPython | CLog::msgFlagNone,CLog::msgLvlInfo,__FUNCTION__,"output.py\n");
 	f->close();
 	f->open(scripts.c_str(),"rb");
 	if( !(f->isOpen() && loadScript(f.get())) )
 		CLog::instance()->log(CLog::msgFlagPython,CLog::msgLvlError,__FUNCTION__,"Error loading 'editor.py'.\n");
 	else
 		CLog::instance()->log(CLog::msgFlagPython | CLog::msgFlagNone,CLog::msgLvlInfo,__FUNCTION__,"editor.py\n");
 	f->close();
 /*	dir->open(CClientApp::instance()->getSetting("ScriptDir").c_str());
 	if( dir->isOpen() )
 	{
 		bool res = dir->setToFirstEntry();
 		while( res )
 		{
 			if( dir->getEntryType() == CDirectory::FILE )
 				if(boost::algorithm::ends_with(dir->getEntryName(),".py") && !boost::algorithm::ends_with(dir->getEntryName(),"output.py"))
 				{
 					String path = dir->getEntryName();
 					CLog::instance()->log(CLog::msgLvlInfo,__FUNCTION__,_("Loading script '%s'.\n"),path.c_str());
 //					boost::algorithm::erase_head(path,path.find_last_of('/')+1);
 					f->open(path.c_str(),"r");
 					if( !loadScript(f.get()) )
 						CLog::instance()->log(CLog::msgLvlError,_("Error in script file '%s'.\n"),path.c_str());
 					f->close();
 				};
 			res = dir->setToNextEntry();
 		};
 	};*/
 	CLog::instance()->log(CLog::msgLvlInfo,__FUNCTION__,"Loading scripts completed.\n");
 };