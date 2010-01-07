#include "boost/test/unit_test.hpp"
using boost::unit_test::test_suite;


#include "common/common_stdafx.h"
#include "net/layers/sock_layer.h"
#include "common/boost/shared_ptr.hpp"

using namespace std;
typedef boost::shared_ptr<CDataSocketManager>  CDataSockMgrPtr;


#ifdef __WIN32__
   #define WAIT(msecs) Sleep(msecs);
#elif defined __UNIX__
   #define WAIT(msecs) usleep(msecs*1000);
#endif

void initTest()
{
   SDLNet_Init();
}

void simpleConnectionTest() 
{
	CIncomingConnectionListener server;
	BOOST_REQUIRE(server.start(500, 1));

	CDataSocketManager sockMgr("localhost", 500);

	CNetLayerBasePtr initSock = sockMgr.connect();
	BOOST_REQUIRE(initSock.get() != NULL);
	WAIT(1000);

	BOOST_CHECK(!((CDataSocket*) initSock.get())->isConnected());

	CNetLayerBasePtr acceptSock = CNetLayerBasePtr(server.checkIncomingConnections());
	BOOST_REQUIRE(acceptSock.get() != NULL);
	BOOST_CHECK(((CDataSocket*) acceptSock.get())->isConnected());
	BOOST_CHECK(((CDataSocket*) initSock.get())->isConnected());
}

void multipleConnectionTest()
{
   const nConnections = 3;
   const nExceed      = 2;

   CIncomingConnectionListener server;
   BOOST_REQUIRE(server.start(500, nConnections));

   CDataSockMgrPtr sockMgr[nConnections+nExceed];
   CNetLayerBasePtr initSock[nConnections+nExceed];

   for(int i = 0; i < nConnections; ++i)
   {
      sockMgr[i] = CDataSockMgrPtr(new CDataSocketManager("localhost", 500));
      initSock[i] = sockMgr[i]->connect();
      WAIT(400);
      BOOST_REQUIRE(initSock[i].get() != NULL);
   }

	CNetLayerBasePtr acceptSockNorm[nConnections];
   for(i = 0; i < nConnections; ++i)
   {
      acceptSockNorm[i] = CNetLayerBasePtr(server.checkIncomingConnections());
      BOOST_REQUIRE_MESSAGE(acceptSockNorm[i].get() != NULL, "i="<<i);
      if (acceptSockNorm[i].get() != NULL)
			BOOST_CHECK_MESSAGE(((CDataSocket*) acceptSockNorm[i].get())->isConnected(), "i="<<i);
			
		BOOST_CHECK_MESSAGE(((CDataSocket*) initSock[i].get())->isConnected(), "i="<<i);
   }

   CNetLayerBasePtr acceptSockAbnorm = CNetLayerBasePtr(server.checkIncomingConnections());
   BOOST_CHECK(acceptSockAbnorm.get() == NULL);

   for(i = 0; i < nExceed; ++i)
   {
      sockMgr[nConnections+i] = CDataSockMgrPtr(new CDataSocketManager("localhost", 500));
      initSock[nConnections+i] = sockMgr[nConnections+i]->connect();
      BOOST_CHECK(initSock[nConnections+i] != NULL);
   }

	CNetLayerBasePtr acceptSockExceed[nExceed];
   for(i = 0; i < nExceed; ++i)
   {
      CNetLayerBasePtr acceptSockExceed = CNetLayerBasePtr(server.checkIncomingConnections());
      BOOST_CHECK_MESSAGE(acceptSockExceed.get() == NULL, "i="<<(i+nConnections));
   }
   WAIT(1000);

   for(i = 0; i < nExceed; ++i)
      BOOST_CHECK_MESSAGE(!(((CDataSocket*) initSock[i+nConnections].get())->isConnected()), "i="<<(i+nConnections));
}

void simpleDataExchangeTest()
{
   int data = 1821;

   CIncomingConnectionListener server;
   BOOST_REQUIRE(server.start(500, 1));
   CDataSocketManager sockMgr("localhost", 500);
   
   CNetLayerBasePtr initSock = sockMgr.connect();
   BOOST_REQUIRE(initSock.get() != NULL);
   WAIT(1000);

	CNetLayerBasePtr acceptSock = server.checkIncomingConnections();
	BOOST_CHECK(acceptSock.get() != NULL);

   
   BOOST_CHECK(((CDataSocket*) initSock.get())->isConnected());
   CData output;
   output<<data;
   initSock->write(output);
   BOOST_CHECK(initSock->flush());

   WAIT(1000);

   vector<CData> input;
   server.checkSocketsInput();
	BOOST_CHECK(acceptSock->read(input));
   BOOST_CHECK(!input.empty());

   if (!input.empty())
   {
      int number;
	  CData d;
		d = input[0];
	  d.reset(false);
	  d >> number;
      BOOST_CHECK(number == data);
   }

	acceptSock->write(output); 
	WAIT(1000);
	sockMgr.checkSocketsInput();
	BOOST_CHECK(initSock->read(input)); 
	
	if (!input.empty())
	{
		int number;
		CData d;
		d = input[0];
		d.reset(false);
		d >> number;
		BOOST_CHECK(number == data);
}
}

void outputBufferTest()
{
   const dataSize = 10;
   int data[dataSize] =  {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

   CIncomingConnectionListener server;
   BOOST_REQUIRE(server.start(500, 1));
   CDataSocketManager sockMgr("localhost", 500);

   CNetLayerBasePtr initSock = sockMgr.connect();
   BOOST_REQUIRE(initSock.get() != NULL);
   WAIT(1000);

   CNetLayerBasePtr acceptSock = server.checkIncomingConnections();
   BOOST_REQUIRE(acceptSock.get() != NULL);

   vector<CData> output(dataSize);
   BOOST_CHECK(((CDataSocket*) initSock.get())->isConnected());
   for(unsigned i = 0; i < dataSize; ++i)
   {
      output[i]<<data[i];
      initSock->write(output[i]);
   }
   BOOST_CHECK(initSock->flush());

   WAIT(200*dataSize);

   vector<CData> input;
   server.checkSocketsInput();
   BOOST_CHECK(acceptSock->read(input));
   BOOST_CHECK_MESSAGE(input.size() == dataSize, "received size is "<<input.size());
   for(i = 0; i < input.size(); ++i)
   {
      int number;
	   CData d;
      d = input[i];//>>number;
	   d.reset(false);
	   d >> number;
      BOOST_CHECK(number == data[i]);
   }
}

void inputBufferTest()
{
   const dataSize = 10;
   int data[dataSize] =  {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

   CIncomingConnectionListener server;
   BOOST_REQUIRE(server.start(500, 1));
   CDataSocketManager sockMgr("localhost", 500);

   CNetLayerBasePtr initSock = sockMgr.connect();
   BOOST_REQUIRE(initSock.get() != NULL);
   WAIT(1000);

   CNetLayerBasePtr acceptSock = server.checkIncomingConnections();
   BOOST_REQUIRE(acceptSock.get() != NULL);

   vector<CData> output(dataSize);
   BOOST_CHECK(((CDataSocket*) initSock.get())->isConnected());
   for(unsigned i = 0; i < dataSize; ++i)
   {
      output[i]<<data[i];
      initSock->write(output[i]);
      BOOST_CHECK(initSock->flush());
      WAIT(1000);
   }

   vector<CData> input;
   server.checkSocketsInput();
   BOOST_CHECK(acceptSock->read(input));
   BOOST_CHECK_MESSAGE(input.size() == dataSize, "received size is "<<input.size());
   for(i = 0; i < input.size(); ++i)
   {
      int number;
	   CData d;
      d = input[i];//>>number;
	   d.reset(false);
	   d >> number;
      BOOST_CHECK(number == data[i]);
   }
}

void socketInformationTest()
{
   CIncomingConnectionListener server;
   BOOST_REQUIRE(server.start(500, 1));

   CDataSocketManager sockMgr("localhost", 500);
   CNetLayerBasePtr initSock = sockMgr.connect();
   BOOST_REQUIRE(initSock.get() != NULL);
   WAIT(1000);

   BOOST_CHECK(((CDataSocket*) initSock.get())->isConnected());

   CNetLayerBasePtr acceptSock = server.checkIncomingConnections();
   BOOST_REQUIRE(acceptSock.get() != NULL);
   BOOST_CHECK(((CDataSocket*) acceptSock.get())->isConnected());
   BOOST_CHECK_MESSAGE(initSock->getInfo().compare("localhost"), initSock->getInfo().c_str());
   BOOST_CHECK_MESSAGE(acceptSock->getInfo().compare("localhost"), acceptSock->getInfo().c_str());
   //TODO: Add getPort() test
}

void basicTests()
{
	{
	   CIncomingConnectionListener server;
	}
	{
	   CDataSocketManager sockMgr("localhost", 500);
	}
}

test_suite* init_unit_test_suite( int /*argc*/, char* /*argv*/[] ) 
{
	test_suite* test = BOOST_TEST_SUITE("CDataManager test");

	initTest();

	test->add( BOOST_TEST_CASE( &basicTests ) ); 
	test->add( BOOST_TEST_CASE( &simpleConnectionTest ) ); 
	test->add( BOOST_TEST_CASE( &multipleConnectionTest ) );
	test->add( BOOST_TEST_CASE( &simpleDataExchangeTest ) );
	test->add( BOOST_TEST_CASE( &outputBufferTest ) );
	test->add( BOOST_TEST_CASE( &inputBufferTest ) );
	test->add( BOOST_TEST_CASE( &socketInformationTest ) );

	return test;
}
