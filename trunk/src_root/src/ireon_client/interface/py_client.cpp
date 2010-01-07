/**
  * @file py_client.cpp
  * Export client classes and constants to python
  */
 
 /* Copyright (C) 2005 ireon.org developers council
  * $Id: py_client.cpp 844 2007-01-03 13:28:38Z mip $
 
  *  See LICENSE for details
  */
#ifdef _ENABLE_EXPORT_TO_PYTHON
#include "stdafx.h"
#include "ireon_client/interface/client_interface.h"
#include "ireon_client/interface/client_event_manager.h"
#include "ireon_client/world/client_world.h"
#include "ireon_client/world/char_player.h"
 
#define BOOST_PYTHON_STATIC_MODULE
#include "boost/python.hpp"
using namespace boost::python;
 
 BOOST_PYTHON_MODULE(Client)
 {
 	class_< AppChangeStateEventArg, bases<EventArg> >
 		("AppChangeStateEventArg")
 		.def_readwrite("old",&AppChangeStateEventArg::m_oldState)
 		.def_readwrite("new",&AppChangeStateEventArg::m_newState);
 
 	class_< CharCreatedEventArg, bases<EventArg> >
 		("CharCreatedEventArg")
 		.def_readwrite("res",&CharCreatedEventArg::m_res)
 		.def_readwrite("d",&CharCreatedEventArg::m_d);
 
 	class_< CharRemovedEventArg, bases<EventArg> >
 		("CharRemovedEventArg")
 		.def_readwrite("res",&CharRemovedEventArg::m_res)
 		.def_readwrite("id",&CharRemovedEventArg::m_id);
 
 	class_< ConnectFailedEventArg, bases<EventArg> >
 		("ConnectFailedEventArg")
 		.def_readwrite("error",&ConnectFailedEventArg::m_error)
 		.def_readwrite("host",&ConnectFailedEventArg::m_host)
 		.def_readwrite("port",&ConnectFailedEventArg::m_port);
 
 	enum_< ChatCommand >
 		("ChatCommand")
 		.value("MSG",CHAT_MSG)
 		.value("PRIVMSG",CHAT_PRIVMSG)
 		.value("JOIN",CHAT_JOIN)
 		.value("PART",CHAT_PART)
 		.value("NAMES",CHAT_NAMES)
 		.value("CHAN_MODE",CHAT_CHAN_MODE)
 		.value("LIST",CHAT_LIST);
 
 	class_< ChatEventArg, bases<EventArg> >
 		("ChatEventArg")
 		.def_readwrite("cmd",&ChatEventArg::m_command)
 		.def_readwrite("arg1",&ChatEventArg::m_arg1)
 		.def_readwrite("arg2",&ChatEventArg::m_arg2)
 		.def_readwrite("arg3",&ChatEventArg::m_arg3);
 
 	enum_< ClientEvent >
 		("EventID")
 		.value("APP_CHANGES_STATE",ON_APP_CHANGES_STATE)
 		.value("CHAR_LIST_CHANGED",ON_CHAR_LIST_CHANGED)
 		.value("ROOT_MENU_UPDATED",ON_ROOT_MENU_UPDATED)
 		.value("CHAR_CREATED",ON_CHAR_CREATED)
 		.value("CHAR_REMOVED",ON_CHAR_REMOVED)
 		.value("LOGIN_FAILED",ON_LOGIN_FAILED)
 		.value("CONNECT_FAILED",ON_CONN_FAILED)
 		.value("CHAT",ON_CHAT)
 		.value("CHAR_INFO_UPDATE",ON_CHAR_INFO_UPDATE);
 
 	class_< CClientInterface, bases<CInterface> >
 		("CClientInterface",no_init)
 		.add_property("firstPerson",&CClientInterface::isFirstPerson, &CClientInterface::setFirstPerson )
 		.add_property("target", &CClientInterface::getTarget, &CClientInterface::setTarget )
 		.def("nextTarget", &CClientInterface::nextTarget)		
		.def("enableControl", &CClientInterface::enableControl )
 		.add_property("highlight", &CClientInterface::getHighlight );
 
 	class_<CClientWorld, bases<CWorld> >
 		("CClientWorld",no_init)
 		.add_property("ownChar",&CClientWorld::getOwnChar);
 
 	class_< CCharacter, CharPlayerPtr, bases<CDynamicObject> >
 		("CharPlayer", no_init );
 
 	class_< COwnCharPlayer, OwnCharPlayerPtr, bases<CDynamicObject> >
 		("OwnCharPlayer", no_init );
 
 	def("getInterface",&CClientInterface::instance,return_value_policy<reference_existing_object>());
 	def("getWorld",&CClientWorld::instance,return_value_policy<reference_existing_object>());
 };
 
 void initClientModule()
 {
 	initClient();
 };
#endif