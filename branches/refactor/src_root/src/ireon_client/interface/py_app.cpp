/* Copyright (C) 2005 ireon.org developers council
 * $Id: py_app.cpp 433 2005-12-20 20:19:15Z zak $

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
 * @file py_app.cpp
 * Export app class to python
 */
#include "stdafx.h"
#include "db/client_char_data.h"
#include "ireon_client/net/ws_player_client.h"

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

	typedef std::vector<ClientOwnCharData> CharVector;
	class_<CharVector>("CharVector")
	.def( vector_indexing_suite< std::vector<ClientOwnCharData> >());

	typedef std::vector<byte> ByteVector;
	class_<ByteVector>("ByteVector")
	.def( vector_indexing_suite< std::vector<byte> >());

	class_<StringVector>("StringVector")
	.def( vector_indexing_suite< std::vector<std::string>, true>());

	class_< CPythonOutput >
		("Output")
		.def("write",&CPythonOutput::write);

	class_< CData >
		("Data");
		
	class_< CNetClient, CNetClient*, boost::noncopyable >
		("NetClient", no_init );

	class_< CPlayerClient, CPlayerClient*, bases<CNetClient> >
		("PlayerClient", no_init );

	class_< CWSPlayerClient, CWSPlayerClient*, bases<CPlayerClient> >
		("WSPlayerClient", no_init)
		.def("disconnect",&CWSPlayerClient::disconnect)
		.def("reconnect",&CWSPlayerClient::reconnect)
		.def("connect",&CWSPlayerClient::connect)
		.add_property("port",&CWSPlayerClient::getPort)
		.add_property("host",&CWSPlayerClient::getHost)
		/// Commands
		.def("sendChatCmd",&CWSPlayerClient::sendChatCmd);

	class_< ClientCharData, ClientCharData* >
		("ClientCharData")
		.def_readwrite("velocity",&ClientCharData::m_velocity)
		.def("serialize",&ClientCharData::serialize);

	class_< ClientCharPlayerData, ClientCharPlayerData* >
		("ClientCharPlayerData")
		.def_readwrite("id",&ClientCharPlayerData::m_id)
		.def_readwrite("name",&ClientCharPlayerData::m_name)
		.def("serialize",&ClientCharPlayerData::serialize);

	class_< ClientOwnCharData, ClientOwnCharData*, bases<ClientCharPlayerData> >
		("ClientOwnCharData")
		.def_readwrite("id",&ClientOwnCharData::m_id)
		.def_readwrite("name",&ClientOwnCharData::m_name)
		.def_readwrite("velocity",&ClientOwnCharData::m_velocity);

	class_< CClientApp >
		("Application",no_init)
//		.def("rootConn",&CClientApp::getRootConn,return_value_policy<reference_existing_object>())
//		.def("worldConn",&CClientApp::getWorldConn,return_value_policy<reference_existing_object>())
		.def("shutdown",&CClientApp::shutdown)
		.add_property("state",&CClientApp::getState,&CClientApp::setState)
		.def("getSetting",&CClientApp::getSetting,CApp_overloads1())
		.def("setSetting",&CClientApp::setSetting,CApp_overloads2())
//		.def("connect",&CClientApp::connect)
//		.def("cancelConnect",&CClientApp::cancelConnect)
//		.def("createChar",&CClientApp::createChar)
//		.def("selectChar",&CClientApp::selectChar)
//		.def("removeChar",&CClientApp::removeChar)
		.def_readwrite("characters",&CClientApp::m_characters);


	enum_< CClientApp::State >
		("State")
		.value("MENU",CClientApp::AS_MENU)
//		.value("CONNECTING_ROOT",CClientApp::AS_CONNECTING_ROOT)
//		.value("LOGGING_ROOT",CClientApp::AS_LOGGING_ROOT)
//		.value("CONNECTING_WORLD",CClientApp::AS_CONNECTING_WORLD)
//		.value("LOGGING_WORLD",CClientApp::AS_LOGGING_WORLD)
		.value("ROOT",CClientApp::AS_ROOT)
		.value("PLAY",CClientApp::AS_PLAY);


	def("get",&CClientApp::instance,return_value_policy<reference_existing_object>());
};

void initAppModule()
{
	initApp();
};
