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
 * @file CEGUIPython.h
 * @date Created: 
 * @author File creator: guyver6
 * @author Credits: guyver6
 */

#ifndef _CEGUIPython_h_
#define _CEGUIPython_h_
#pragma warning(push, 1)
#include "CEGUI.h"
#pragma warning(pop)
/*************************************************************************
Import / Export control macros
*************************************************************************/
//#if defined( __WIN32__ ) || defined( _WIN32 )
//#   ifdef CEGUIPYTHON_EXPORTS
//#       define CEGUIPYTHON_API __declspec(dllexport)
//#   else
//#       define CEGUIPYTHON_API __declspec(dllimport)
//#   endif
//#else
#   define CEGUIPYTHON_API
//#endif

// Forward declarations
typedef struct _object PyObject;
//typedef struct _typeobject PyTypeObject;
//struct PySwigClientData;
struct swigTypeInfo;
struct EventArg;

// Start of CEGUI namespace section
namespace CEGUI
{
	///* PySwigClientData */

	//typedef struct {
	//	PyObject *klass;
	//	PyObject *newraw;
	//	PyObject *newargs;
	//	PyObject *destroy;
	//	int delargs;
	//	int implicitconv;
	//} PySwigClientData;

	//typedef void *(*swig_converter_func)(void *);
	//typedef struct swig_type_info *(*swig_dycast_func)(void **);
	///* Structure to store inforomation on one type */
	//typedef struct swig_type_info {
	//	const char             *name;			/* mangled name of this type */
	//	const char             *str;			/* human readable name of this type */
	//	swig_dycast_func        dcast;		/* dynamic cast function down a hierarchy */
	//	struct swig_cast_info  *cast;			/* linked list of types that can cast into this type */
	//	void                   *clientdata;		/* language specific type data */
	//	int                    owndata;		/* flag if the structure owns the clientdata */
	//} swig_type_info;
	///* Structure to store a type and conversion function used for casting */
	//typedef struct swig_cast_info {
	//	swig_type_info         *type;			/* pointer to type that is equivalent to this type */
	//	swig_converter_func     converter;		/* function to cast the void pointers */
	//	struct swig_cast_info  *next;			/* pointer to next cast in linked list */
	//	struct swig_cast_info  *prev;			/* pointer to the previous cast */
	//} swig_cast_info;

	/*!
	\brief
	Interface for the LuaScriptModule class
	*/
	class CEGUIPYTHON_API PythonScriptModule : public CEGUI::ScriptModule
	{

	public:
		/*************************************************************************
		Construction and Destruction
		*************************************************************************/
		/*!
		\brief
		Constructor for PythonScriptModule class which create a lua_State
		*/
		PythonScriptModule();


		/*!
		\brief
		Destructor for PythonScriptModule class.
		*/
		~PythonScriptModule();


		/*************************************************************************
		Script Execution Functions
		*************************************************************************/
		/*!
		\brief
		Execute a script file.

		\param filename
		String object holding the filename of the script file that is to be executed

		\param resourceGroup
		Resource group idendifier to be passed to the ResourceProvider when loading the script file.
		*/
		void executeScriptFile(const String& filename, const String& resourceGroup);


		/*!
		\brief
		Execute a scripted global function.  The function should not take any parameters and should return an integer.

		\param function_name
		String object holding the name of the function, in the global script environment, that
		is to be executed.

		\return
		The integer value returned from the script function.
		*/
		int executeScriptGlobal(const String& function_name);


		/*!
		\brief
		Execute a scripted global function.  The function take pointer on EventArg  and don't should return any.

		\param function_name
		String object holding the name of the function, in the global script environment, that
		is to be executed.
		
		\param func_arg
		Pointer on EventArg based object holding argument function passed to the global scripted function.

		*/
		void executeScriptGlobalWithParams(const String& function_name, EventArg* func_arg);

		/*!
		\brief
		Execute a scripted global 'event handler' function.  The function should take some kind of EventArgs like parameter
		that the concrete implementation of this function can create from the passed EventArgs based object.  The function
		should not return anything.

		\param handler_name
		String object holding the name of the scripted handler function.

		\param e
		EventArgs based object that should be passed, by any appropriate means, to the scripted function.

		\return
		- true if the event was handled.
		- false if the event was not handled.
		*/
		bool executeScriptedEventHandler(const String& handler_name, const EventArgs& e);


		/*!
		\brief
		Execute script code contained in the given CEGUI::String object.

		\param str
		String object holding the valid script code that should be executed.

		\return
		Nothing.
		*/
		void executeString(const String& str);


		/*************************************************************************
		Bindings creation / destruction
		*************************************************************************/
		/*!
		\brief
		Method called during system initialisation, prior to running any scripts via the ScriptModule, to enable the ScriptModule
		to perform any operations required to complete initialisation or binding of the script language to the gui system objects.

		\return
		Nothing.
		*/
		void createBindings(void);


		/*!
		\brief
		Method called during system destruction, after all scripts have been run via the ScriptModule, to enable the ScriptModule
		to perform any operations required to cleanup bindings of the script language to the gui system objects, as set-up in the
		earlier createBindings call.

		\return
		Nothing.
		*/
		void destroyBindings(void);


        /*!
        \brief
        Subscribes the named Event to a scripted funtion

        \param target
        The target EventSet for the subscription.

        \param event_name
        String object containing the name of the Event to subscribe to.

        \param subscriber_name
        String object containing the name of the script funtion that is to be subscribed to the Event.

        \return
        Connection object that can be used to check the status of the Event connection and to disconnect (unsubscribe) from the Event.

        \exception UnknownObjectException	Thrown if an Event named \a name is not in the EventSet
        */
        virtual Event::Connection	subscribeEvent(EventSet* target, const String& event_name, const String& subscriber_name);


        /*!
        \brief
        Subscribes the specified group of the named Event to a scripted funtion.

        \param target
        The target EventSet for the subscription.

        \param event_name
        String object containing the name of the Event to subscribe to.

        \param group
        Group which is to be subscribed to.  Subscription groups are called in ascending order.

        \param subscriber_name
        String object containing the name of the script funtion that is to be subscribed to the Event.

        \return
        Connection object that can be used to check the status of the Event connection and to disconnect (unsubscribe) from the Event.

        \exception UnknownObjectException	Thrown if an Event named \a name is not in the EventSet
        */
        virtual Event::Connection	subscribeEvent(EventSet* target, const String& event_name, Event::Group group, const String& subscriber_name);

        String printPythonError(const String& desc = "", const String& filename = "");

	private:
		/*************************************************************************
		Implementation Functions
		*************************************************************************/
		void setModuleIdentifierString();

        void reportPythonError(const String& str = "");
        PyObject* getObjectFromEval(const String& object_name);

		//------------------------- Create PyObject from C++ object -----------------------------------------
		//PyObject* SWIG_NewPointerObj(void *ptr, swig_type_info *type, int flags);
		//PyObject* PySwigObject_New(void *ptr, swig_type_info *ty, int own);
		//PyObject* PythonScriptModule::SWIG_Py_Void(void);
		//PyTypeObject* PySwigObject_type(void);
		//PyTypeObject* _PySwigObject_type(void);
		//PyObject* SWIG_Python_NewShadowInstance(PySwigClientData *data, PyObject *swig_this);
		//PyObject* SWIG_This(void);
		//PyObject* _SWIG_This(void);
		swigTypeInfo* getArgumentEventType(EventArg* funcArg);
		/*************************************************************************
		Implementation Data
		*************************************************************************/
		bool d_ownsState;		//!< true when the attached lua_State was created by this script module


};

} // namespace CEGUI

#endif // end of guard _CEGUIPython_h_
