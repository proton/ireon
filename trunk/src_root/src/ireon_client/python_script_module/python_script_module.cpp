/*
--------------------------------------------------------------------------------
This source file is part of Xenocide
  by  Project Xenocide Team

For the latest info on Xenocide, see http://www.projectxenocide.com/

This work is licensed under the Creative Commons
Attribution-NonCommercial-ShareAlike 2.5 License.

To view a copy of this license, visit
http://creativecommons.org/licenses/by-nc-sa/2.5/
or send a letter to Creative Commons, 543 Howard Street, 5th Floor,
San Francisco, California, 94105, USA.
--------------------------------------------------------------------------------
*/

/**
 * @file ceguipython.cpp
 * @date Created: 
 * @author File creator: guyver6
 * @author Credits: guyver6
 */
#include "stdafx.h"
#include <Python.h>
#pragma warning(push, 1)
#include "CEGUI.h"
#pragma warning(pop)

#include <marshal.h>
#include <traceback.h>
#include <cStringIO.h>

#include <iostream>
#include "ireon_client/python_script_module/python_script_module.h"
#include "ireon_client/python_script_module/cegui_python_functor.h"

#include "ireon_client/interface/client_event_manager.h"

extern unsigned char cegui_py_wrapper_marshalled[]; // cegui.py file compiled and marshalled
extern size_t cegui_py_wrapper_marshalled_size;
extern "C" void init_cegui(void);

// Defined at end of cegui_wrap.cpp
bool CEGUI_runHandler(PyObject *handler, const CEGUI::EventArgs &args);
/* attribute recognised by some compilers to avoid 'unused' warnings */


extern "C" struct swig_type_info;
extern "C" PyObject * SWIG_Python_NewPointerObj(void *ptr, swig_type_info *type, int flags);
#define SWIG_NewPointerObj(ptr, type, flags) SWIG_Python_NewPointerObj(ptr, type, flags)

namespace
{
	

	PyObject* s_mainModule;
    PyObject* s_mainDict;

//def extract_stack(f=None, limit = None):
//    """Extract the raw traceback from the current stack frame.
//
//    The return value has the same format as for extract_tb().  The
//    optional 'f' and 'limit' arguments have the same meaning as for
//    print_stack().  Each item in the list is a quadruple (filename,
//    line number, function name, text), and the entries are in order
//    from oldest to newest stack frame.
//    """
//    if f is None:
//        try:
//            raise ZeroDivisionError
//        except ZeroDivisionError:
//            f = sys.exc_info()[2].tb_frame.f_back
//    if limit is None:
//        if hasattr(sys, 'tracebacklimit'):
//            limit = sys.tracebacklimit
//    list = []
//    n = 0
//    while f is not None and (limit is None or n < limit):
//        lineno = f.f_lineno
//        co = f.f_code
//        filename = co.co_filename
//        name = co.co_name
//        linecache.checkcache(filename)
//        line = linecache.getline(filename, lineno)
//        if line: line = line.strip()
//        else: line = None
//        list.append((filename, lineno, name, line))
//        f = f.f_back
//        n = n+1
//    list.reverse()
//    return list
    //PyObject *tb_exractStack(PyObject *stack, int limit = 0)
    //{
    //    PyObject *list = NULL;
    //    PyObject *sysmodule = NULL;
    //    PyObject *sysdict = NULL;
    //    PyObject *code = NULL;
    //    int lineno;
    //    
    //    try
    //    {
    //        list = PyList_New(0);
    //        if (!list) throw 1;

    //        sysmodule = PyImport_ImportModule("sys");
    //        if (!sysmodule) throw 1;

    //        sysdict = PyModule_GetDict(sysmodule);
    //        if (!sysdict) throw 1;



    //    }
    //    catch (const int&)
    //    {
    //        Py_XDECREF(sysdict);
    //        Py_XDECREF(sysmodule);
    //        Py_XDECREF(list);

    //        return NULL;
    //    }
    //}

}

// Start of CEGUI namespace section
namespace CEGUI
{
	/*typedef struct 
	{
		PyObject_HEAD
			void *ptr;
		swig_type_info *ty;
		int own;
		PyObject *next;
	} PySwigObject;*/

    /*************************************************************************
    Constructor (creates Python state)
    *************************************************************************/
    PythonScriptModule::PythonScriptModule()
        : d_ownsState(false)
    {
        if (!Py_IsInitialized())
        {
            d_ownsState = true;
            Py_InitializeEx(0);
        }

        PycString_IMPORT;

        setModuleIdentifierString();
    }

    /*************************************************************************
    Destructor
    *************************************************************************/
    PythonScriptModule::~PythonScriptModule()
    {
        if (d_ownsState)
        {
            Py_Finalize();
        }
    }


    /*************************************************************************
    Execute script file
    *************************************************************************/
    void PythonScriptModule::executeScriptFile(const String& filename, const String& resourceGroup)
    {
        RawDataContainer raw;
        System::getSingleton().getResourceProvider()->loadRawDataContainer( filename, raw,
            resourceGroup.empty() ? d_defaultResourceGroup : resourceGroup );

        // Get the file's content into String
        String content(raw.getDataPtr(), raw.getSize());

        System::getSingleton().getResourceProvider()->unloadRawDataContainer( raw );

        // Line-endings convertion
        String::size_type pos;
        while ((pos = content.find("\r\n")) != String::npos) // replace CRLF with LF
            content.replace(pos, 2, "\n", 1);
        while ((pos = content.find("\r")) != String::npos) // replace CR with LF
            content.replace(pos, 2, "\n", 1);
        content += "\n"; // make sure that passed file content is compilable
                         // (must end with a new line)

        PyObject* codeObject = Py_CompileString(content.c_str(), "scripts.py", Py_file_input);
        if (!codeObject)
            reportPythonError( "cannot compile file: " + filename );

        PyObject* result = PyEval_EvalCode(reinterpret_cast<PyCodeObject*>(codeObject), s_mainDict, s_mainDict);
        if (!result)
            reportPythonError( "cannot evaluate file: " + filename );

        Py_XDECREF(codeObject);
        Py_XDECREF(result);
        codeObject = NULL;
        result = NULL;
    }


    /*************************************************************************
    Execute global script function
    *************************************************************************/
    int	PythonScriptModule::executeScriptGlobal(const String& function_name)
    {
        const char* name = function_name.c_str();

        int retval = 0;

        PyObject* globalFunction = getObjectFromEval(function_name);
        if (!globalFunction)
        {
            reportPythonError("cannot execute script global, getObjectFromEval failed");
            return 0;
        }

        if (!PyCallable_Check(globalFunction))
        {
            Py_XDECREF(globalFunction);
            reportPythonError("Python object is not callable: " + function_name);
            return 0;
        }

        // Call object and check for errors
        PyObject* result = PyObject_CallObject(globalFunction, NULL);
        Py_DECREF(globalFunction); // not needed anymore
        if (!result)
        {
            Py_XDECREF(result);
            reportPythonError("cannot call global function: " + function_name);
            return 0;
        }

        // If function returns None, then we treat it as 0
        if (result == Py_None)
            retval = 0;

        // If result is integer then return it
        if (PyInt_Check(result))
            retval = (int)PyInt_AsLong(result);

        Py_DECREF(result);
        return retval;
    }

	swigTypeInfo* PythonScriptModule::getArgumentEventType(EventArg* funcArg)
	{
		return  funcArg->getSwigType() ;
	}

	/*************************************************************************
	Execute global script function with parameters
	*************************************************************************/

	void PythonScriptModule::executeScriptGlobalWithParams(const String& function_name, EventArg* func_arg)
	{
		try
		{
			PyObject* argument1 = NULL;
			PyObject* argsTuple = NULL;
			PyObject *pyFuncArg = NULL;
			if( func_arg )
			{	
				
				swigTypeInfo* argType = getArgumentEventType(func_arg);
				if (argType)
					pyFuncArg = SWIG_NewPointerObj((void *)func_arg, (swig_type_info*)argType, 0);
				else
				{
					Logger::getSingleton().logEvent( "Don't found argument event type", Errors );
					return;
				}
				if (pyFuncArg)
				{
					argsTuple = PyTuple_New(1);
					PyTuple_SET_ITEM(argsTuple, 0, pyFuncArg);
				}
					
			}
			else
			{
				//executeScriptGlobal(function_name);
				//return;
				argsTuple = PyTuple_New(1);
				PyTuple_SET_ITEM(argsTuple, 0, Py_None);
			}
			PyObject* globalFunction = getObjectFromEval(function_name);
			if (!globalFunction)
			{
				reportPythonError("cannot execute script global, getObjectFromEval failed");
				return;
			}

			PyObject* result = PyObject_CallObject(globalFunction, argsTuple);
			Py_DECREF(globalFunction); // not needed anymore
			Py_XDECREF(argument1); // not needed anymore
			Py_XDECREF(argsTuple); // ?
			if (!result)
			{
				Py_XDECREF(result);
				reportPythonError("cannot call global function: " + function_name);
				return;
			}

		} catch (...)
		{
			PyErr_Print();
		};
	}

    /*************************************************************************
    Execute scripted event handler
    *************************************************************************/
    bool PythonScriptModule::executeScriptedEventHandler(const String& handler_name, const EventArgs& e)
    {
        const char* name = handler_name.c_str();

        bool retval = false;

        PyObject* handler = getObjectFromEval(handler_name);

        if (handler)
        {
            // Running handler requires us to catch EventCallback exception
            try
            {
                retval = CEGUI_runHandler(handler, e);
            }
            catch (ScriptException& e)
            {
                Py_XDECREF(handler);
                reportPythonError("exception thrown when executing handler: " + e.getMessage());
            }
        }

        Py_XDECREF(handler);
        return retval;
    }


    /*************************************************************************
    Execute script code string
    *************************************************************************/
    void PythonScriptModule::executeString(const String& str)
    {
        PyObject* result =
            PyRun_String(str.c_str(), Py_file_input, s_mainDict, 0);

        if (!result)
            reportPythonError("Error executing string: " + str);

        Py_DECREF(result);
    }

    /*************************************************************************
    Subscribes the named Event to a scripted funtion
    *************************************************************************/
    Event::Connection PythonScriptModule::subscribeEvent(EventSet* target, const String& event_name, const String& subscriber_name)
    {
        PyObject* handler = getObjectFromEval(subscriber_name);
        if (!handler)
            throw CEGUI::ScriptException("Cannot find specified handler to subscribe to event");

        try
        {
            PythonFunctor functor(handler);
            return target->subscribeEvent(event_name, Event::Subscriber(functor));
        }
        catch (ScriptException& e)
        {
            reportPythonError("Unable to create PythonFunctor for '" + subscriber_name + "'");
            throw;
        }
    }

    /*************************************************************************
    Subscribes the specified group of the named Event to a scripted funtion
    *************************************************************************/
    Event::Connection PythonScriptModule::subscribeEvent(EventSet* target, const String& event_name, Event::Group group, const String& subscriber_name)
    {
        PyObject* handler = getObjectFromEval(subscriber_name);
        if (!handler)
            throw CEGUI::ScriptException("Cannot find specified handler to subscribe to event");

        PythonFunctor functor(handler);
        return target->subscribeEvent(event_name, group, Event::Subscriber(functor));
    }

    /*************************************************************************
    *************************************************************************/
    PyObject* PythonScriptModule::getObjectFromEval(const String& object_name)
    {
        const char* name = object_name.c_str();

        PyObject* codeObject = Py_CompileString(name, "<cegui script>", Py_eval_input);
        if (!codeObject)
            reportPythonError( "unable to compile string " + object_name );

        PyObject* function = PyEval_EvalCode(reinterpret_cast<PyCodeObject*>(codeObject), s_mainDict, s_mainDict);
        Py_DECREF(codeObject);
        if (!function)
            reportPythonError( "unable to evaluate object " + object_name );

        return function;
    }

    /*************************************************************************
    Throw an exception reporting python error
    *************************************************************************/
    void PythonScriptModule::reportPythonError(const String& str)
    {
        if (!PyErr_Occurred())
            return;

        if (Logger::getSingletonPtr())
        {
            Logger::getSingleton().logEvent( printPythonError(str), Errors );
        }
        else
        {
            std::cerr << "PythonScriptModule::reportPythonError (logger not available): " << str << std::endl;
        }
        PyErr_Print();
    }

    String PythonScriptModule::printPythonError(const String& desc, const String& filename)
    {
        if (!PyErr_Occurred())
            throw InvalidRequestException("No Python error occured");

        PyObject *exc = NULL;
        PyObject *excStr = NULL;
        PyObject *val = NULL;
        PyObject *valStr = NULL;
        PyObject *trb = NULL;
        PyObject *trbIO = NULL;
        PyObject *trbStr = NULL;

        String retval;

        PyErr_Fetch(&exc, &val, &trb);
        PyErr_NormalizeException(&exc, &val, &trb);
        try
        {
            std::ostringstream result;
            result << "(PythonScriptModule) Error";
            if (!filename.empty())
                result << " (" << filename << ")";
            if (!desc.empty())
                result << " - " << desc;
            result << std::endl << std::endl;

            // Printing traceback using cStringIO as a File-like object
            if (trb)
            {
                trbIO = PycStringIO->NewOutput(2048);
                if (!trbIO)
                    throw GenericException("Unable to create cStringIO object, PycStringIO->NewOutput failed");
                
                int i = PyTraceBack_Print(trb, trbIO);
                if (i < 0)
                    throw GenericException("Unable to print traceback, PyTraceBack_Print failed");
                
                trbStr = PycStringIO->cgetvalue(trbIO);
                if (!trbStr)
                    throw GenericException("Unable to get traceback string, PycStringIO->cgetvalue failed");

                Py_DECREF(trbIO); trbIO = NULL;

                result << PyString_AsString(trbStr) << std::endl;
            }

            // Printing exception type
            excStr = PyObject_Str(exc);
            if (!excStr)
                throw GenericException("Unable to stringize exception, PyObject_Str failed");

            result << PyString_AsString(excStr);
            Py_DECREF(excStr); excStr = NULL;

            // Printing exception value right next to the exception type
            if (val)
            {
                valStr = PyObject_Str(val);
                if (!valStr)
                    throw GenericException("Unable to stringize value, PyObject_Str failed");
                result << ": " << PyString_AsString(valStr);
                Py_DECREF(valStr); valStr = NULL;
            }

            retval = result.str();

            PyErr_Restore(exc, val, trb);
        }
        catch (GenericException&)
        {
            Py_XDECREF(exc);
            Py_XDECREF(excStr); 
            Py_XDECREF(val); 
            Py_XDECREF(valStr); 
            Py_XDECREF(trb);
            Py_XDECREF(trbIO);
            Py_XDECREF(trbStr);

            PyErr_Print();

            throw;
        }

        return retval;
    }

    /*************************************************************************
    Create Python bindings
    *************************************************************************/
    void PythonScriptModule::createBindings(void)
    {
        CEGUI::Logger::getSingleton().logEvent( "---- Creating Python bindings ----" );

        s_mainModule = PyImport_AddModule("__main__");
        if (!s_mainModule)
            reportPythonError("Couldn't get __main__ module");

        s_mainDict = PyModule_GetDict(s_mainModule);
        if (!s_mainDict)
            reportPythonError("Couldn't get __dict__ from __main__ module");

        // TODO: do I need these?
        Py_INCREF(s_mainModule);
        Py_INCREF(s_mainDict);

        // Initialize _cegui module
        init_cegui();
        
        PyObject* ceguiModuleCode = 
			PyMarshal_ReadObjectFromString(
				(char*)cegui_py_wrapper_marshalled,
				(int)cegui_py_wrapper_marshalled_size );
        if (!ceguiModuleCode)
            reportPythonError("Error in embedded cegui.py code");

        PyObject* ceguiModule = PyImport_ExecCodeModule("cegui", ceguiModuleCode);
        Py_XDECREF(ceguiModuleCode); // don't need this at this point
        if (!ceguiModule)
            reportPythonError("Error compiling embedded cegui.py code");
        
        // Add CEGUI module as "cegui" to main namespace
        if (PyModule_AddObject(s_mainModule, "cegui", ceguiModule))
            reportPythonError("Error adding cegui module to __main__"); // steals ceguiModule reference
   		
	}


    /*************************************************************************
    Destroy Python bindings
    *************************************************************************/
    void PythonScriptModule::destroyBindings(void)
    {
        CEGUI::Logger::getSingleton().logEvent( "---- Destroing Python bindings ----" );

        // Remove "cegui" from main scope
        if (PyObject_HasAttrString(s_mainModule, "cegui"))
            PyObject_DelAttrString(s_mainModule, "cegui");

        Py_CLEAR(s_mainDict);
        Py_CLEAR(s_mainModule);

        PyErr_Clear();
    }

    /*************************************************************************
    Set the ID string for the module
    *************************************************************************/
    void PythonScriptModule::setModuleIdentifierString()
    {
        // set ID string
        d_identifierString = "CEGUI::PythonScriptModule - Python based scripting module for CEGUI";
    }


} // namespace CEGUI

