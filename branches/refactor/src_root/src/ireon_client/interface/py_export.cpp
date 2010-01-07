/* Copyright (C) 2005 ireon.org developers council
 * $Id: py_export.cpp 350 2005-12-05 22:03:56Z llyeli $

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

/**
 * @file py_export.cpp
 * Export to python
 */

#include "stdafx.h"
#include "file/file.h"
#include "file/directory.h"

#define BOOST_PYTHON_STATIC_MODULE
#include "boost/python.hpp"
using namespace boost::python;

extern void initEventModule();
extern void initAppModule();
extern void initWorldModule();
extern void initInterfaceModule();

void exportAllToPython()
{
	CLog::instance()->log(CLog::msgLvlInfo,"Export to python... ");
	try{
		Py_Initialize();
		initInterfaceModule();
		initEventModule();
		initAppModule();
		initWorldModule();

/*		object module(handle<>(borrowed(PyImport_AddModule("__main__"))));
		object dictionary = module.attr("__dict__");

		PyRun_SimpleString("import Interface");
		PyRun_SimpleString("I = Interface.get()");
		PyRun_SimpleString("res = I.getWindow('DefaultPopupWindow')");
		object result = dictionary["res"];
		WndPtr result_value = extract<WndPtr>(result);
		int a = 0;
*/
	} catch (error_already_set)
	{
		CLog::instance()->log(CLog::msgLvlError,_("Can't initialize python."));
		CClientApp::instance()->shutdown();
	};
	CLog::instance()->log(CLog::msgLvlInfo,"completed.\n");
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
	CLog::instance()->log(CLog::msgLvlInfo,"Loading scripts.\n");
	CData d;
	DirPtr dir(new CDirectory);
	FilePtr f(new CFile);
	if(CClientApp::instance()->getSetting("ScriptDir") == "" )
		CClientApp::instance()->setSetting("ScriptDir",".");
	f->open("scripts/output.py","rb");
	if( !(f->isOpen() && loadScript(f.get())) )
		CLog::instance()->log(CLog::msgFlagPython | CLog::msgFlagNone,CLog::msgLvlError,"Error loading 'output.py'.\n");
	else
		CLog::instance()->log(CLog::msgFlagPython | CLog::msgFlagNone,CLog::msgLvlInfo,"output.py\n");
	f->close();
	f->open("scripts/scripts.py","rb");
	if( !(f->isOpen() && loadScript(f.get())) )
		CLog::instance()->log(CLog::msgFlagPython,CLog::msgLvlError,"Error loading 'scripts.py'.\n");
	else
		CLog::instance()->log(CLog::msgFlagPython | CLog::msgFlagNone,CLog::msgLvlInfo,"scripts.py\n");
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
					CLog::instance()->log(CLog::msgLvlInfo,_("Loading script '%s'.\n"),path.c_str());
//					boost::algorithm::erase_head(path,path.find_last_of('/')+1);
					f->open(path.c_str(),"r");
					if( !loadScript(f.get()) )
						CLog::instance()->log(CLog::msgLvlError,_("Error in script file '%s'.\n"),path.c_str());
					f->close();
				};
			res = dir->setToNextEntry();
		};
	};*/
	CLog::instance()->log(CLog::msgLvlInfo,"Loading scripts completed.\n");
};
