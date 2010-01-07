/**
 * @file common/net/tests/net_layer_tests.cpp
 * Tests for net layer classes
 */

/* Copyright (C) 2005-2006 ireon.org developers council
 * $Id: net_layer_tests.cpp 714 2006-09-07 16:32:40Z zak $

 *  See LICENSE for details
 */

#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>

#include "common/net/server_connections_manager.h"
#include "common/net/client_connections_manager.h"
#include "common/net/net_connection.h"
#include "common/log/log_listener.h"
#include "common/net/net_data.h"

#ifdef __WIN32__
#define WAIT(msecs) Sleep(msecs);
#elif defined __UNIX__
#include "unistd.h"
#define WAIT(msecs) usleep(msecs*1000);
#endif

//////////////////////////////////////////////////////////////////////////
//////////////// dummy connections and states ////////////////////////////
//////////////////////////////////////////////////////////////////////////

enum testMsgs
{
	testInitiateConversation = ID_USER_PACKET_ENUM,
	testConversation1,
	testConversation2
};

enum testConversation1Packet
{
	test
};

enum testConversation2Packet
{
	test1
};


class CTestClientState : public CGenericState 
{
public:
	CTestClientState (CNetConnection *ownerConnection):
	  CGenericState(ownerConnection)
	{
		setStateName("Client Test State");
		registerSlot(testConversation1, static_cast<CGenericSlot> (&CTestClientState::onTest1));
	}
private:
	void onTest1(String &packetData)
	{
		CNetData<testConversation1Packet> data;
		data.unserialize(packetData);
		int i;
		data[test] >> i;
#ifndef DISABLE_CLOG
		CLog::instance()->log(gtc_("Got test packet 1. Data = %d (has to be 123)."), i);
#endif
		CNetData<testConversation2Packet> outdata;
		outdata[test1] << "test string";
		conn().send(testConversation2, outdata.serialize());
	}
};

class CDummyClientConnection : public CNetConnection 
{
public:
	enum states {test};
	int i;
	CDummyClientConnection()
	{
//		registerState(CDummyClientConnection::test, new CTestClientState());
		// first call to registerState calls setNextState automatically
		// setNextState(CDummyClientConnection::test);
	}
};


// uses class derived from CNetConnection
class CClientConnectionBuilder
{
public:
	static CNetConnection *buildConnection()
	{
		CNetConnection *p = new CDummyClientConnection();
		p -> registerState(CDummyClientConnection::test, new CTestClientState(p));
		return p;
	}
};

//////////////////////////////////////////////////////////////////////////

class CTestServerState : public CGenericState 
{
public:
	CTestServerState (CNetConnection *ownerConnection):
	  CGenericState(ownerConnection)
	{
		setStateName("Server Test State");
		registerSlot(testInitiateConversation, static_cast<CGenericSlot> (&CTestServerState::onTestInit), ireon::net::netAddress::atInternalAddress);
		registerSlot(testConversation2, static_cast<CGenericSlot> (&CTestServerState::onTest2));
	}
private:
	void onTestInit(String &packetData)
	{
		CNetData<testConversation1Packet> outdata;
		outdata[test] << 123;
		conn().send(testConversation1, outdata.serialize());
#ifndef DISABLE_CLOG
		CLog::instance()->log(gtc_("Initiated connection."));
#endif
		packetData;
	}
	void onTest2(String &packetData)
	{
		CNetData<testConversation2Packet> data;
		data.unserialize(packetData);
		String s;
		data[test1] >> s;
#ifndef DISABLE_CLOG
		CLog::instance()->log(gtc_("Got test packet 2. Data = %s (has to be \"test string\")."), s.c_str());
#endif
	}
};

// uses base CNetConnection class
class CServerConnectionBuilder
{
public:
	static CNetConnection *buildConnection()
	{
		CNetConnection *p = new CNetConnection();
		p -> registerState(1, new CTestServerState(p));
		return p;
	}
};



//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
	

BOOST_AUTO_UNIT_TEST(testSimpleConnect)
{
#ifndef DISABLE_CLOG
	CLog::instance()->regListener(new CFileLogListener("log_debug.txt", CLog::msgFlagALL, CLog::msgLvlTrace));
#endif
	{
		CClientConnectionsManager<CClientConnectionBuilder> client;
		client.connect("localhost",29500);
		BOOST_CHECK(client.isConnected() == false);
		client.disconnect();
		WAIT(1000);
	}
	{
		CServerConnectionsManager<CServerConnectionBuilder> srv;
		CClientConnectionsManager<CClientConnectionBuilder> client1;
		srv.startListening(29500,10);
		client1.connect("localhost",29500);
		BOOST_CHECK(client1.isConnected() == false);
		WAIT(1000);
		BOOST_REQUIRE(client1.isConnected() == true);
		client1.disconnect();
		srv.stopListening();
	}
}

BOOST_AUTO_UNIT_TEST(testBasicMessages)
{
	CServerConnectionsManager<CServerConnectionBuilder> srv;
	CClientConnectionsManager<CClientConnectionBuilder> client1;
	srv.startListening(29500,10);
	client1.connect("localhost",29500);
	BOOST_CHECK(client1.isConnected() == false);
	WAIT(1000);
	BOOST_REQUIRE(client1.isConnected() == true);

	for (int i = 0; i < 10; ++i)
	{
		WAIT(100);
		srv.processInput();
		WAIT(100);
		client1.processInput();
	}

	client1.disconnect();
	WAIT(100);
	srv.processInput();

	CClientConnectionsManager<CClientConnectionBuilder> client2;
	CClientConnectionsManager<CClientConnectionBuilder> client3;
	CClientConnectionsManager<CClientConnectionBuilder> client4;
	client1.connect("localhost",29500);
	client2.connect("localhost",29500);
	client3.connect("localhost",29500);
	client4.connect("localhost",29500);
	WAIT(1000);
	srv.processInput();
	BOOST_REQUIRE(client2.isConnected() == true);
	srv.getConnectionById(0).processPaket(testInitiateConversation,"",0);
	srv.getConnectionById(1).processPaket(testInitiateConversation,"",0);
	srv.getConnectionById(2).processPaket(testInitiateConversation,"",0);
	srv.getConnectionById(3).processPaket(testInitiateConversation,"",0);

	for (int i = 0; i < 10; ++i)
	{
		WAIT(100);
		srv.processInput();
		WAIT(100);
		client1.processInput();
		client2.processInput();
		client3.processInput();
		client4.processInput();
	}

	srv.stopListening();
	WAIT(100);
	client1.processInput();
	client2.processInput();
	client3.processInput();
	client4.processInput();
	client1.disconnect();
	client2.disconnect();
	client3.disconnect();
	client4.disconnect();
	WAIT(1000);
}
