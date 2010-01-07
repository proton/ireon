/**
 * @file cnetlayer_test.cpp
 * Tests for ireon.org net layer classes
 */

/* Copyright (C) 2005 ireon.org developers council
 * $Id: cnetlayer_test.cpp 693 2006-08-31 08:02:35Z zak $
  
 *  See LICENSE for details
 */

//#define BOOST_AUTO_TEST_MAIN
//#include <boost/test/auto_unit_test.hpp>

/*
#define _MIP_DEBUG
#include "stdafx.h"
#include <locale>
#include "boost/test/unit_test.hpp"
using boost::unit_test::test_suite;


#include "net/layers/sock_layer.h"
#include "net/layers/compressing_layer.h"
#include "net\data.h"

// ---------------------------------------------------- //
//---------- Keep alive establisment connection --------//
//---------- Send Data !( from server to client)--------//
void pingpong(CDataSocket* server,CDataSocket* client,SDLNet_SocketSet m_socketSetClient)
{
	std::vector<CData> input;
	char * pongPtr;
	pongPtr = new char [2];
	memset(pongPtr,0,2);
	CData pong(pongPtr,2,true);
	// write CData to Net
	BOOST_REQUIRE(server->isConnected());
	server->write(pong);
	BOOST_CHECK(server->flush());
	SDL_Delay(100);
	int numservready=SDLNet_CheckSockets(m_socketSetClient, 0);
	BOOST_REQUIRE_MESSAGE(numservready!=-1 , "SDLNet_CheckSockets: Error check");
	BOOST_REQUIRE_MESSAGE(numservready, "SDLNet_CheckSockets: no action");
	BOOST_CHECK(client->read(input));
	delete[] pongPtr;
	input.clear();
};
// ---------------------------------------------------- 

void testCDataSocketIO()
{
	BOOST_CHECKPOINT("Test CDataSocket Input-Output System");
	CScreenLogListener* loglistener=new CScreenLogListener("cnetlayer_d.exe");
	CLog::instance()->regListener(loglistener);
	//---------------------------
	std::vector<CData> output;
	std::vector<CData> input;
	SDLNet_SocketSet m_socketSet;
	SDLNet_SocketSet m_socketSetClient;
	TCPsocket m_socket = NULL;
	TCPsocket socketServer = NULL;
	IPaddress addr;
	ushort localPort = 10000;
	ushort maximumNumberOfPeers = 2;
	//---------------------------Init Net Subsystem SDL-------
	BOOST_REQUIRE_MESSAGE(SDL_Init(0) !=-1, "SDL_Init: Error");
	BOOST_REQUIRE_MESSAGE(SDLNet_Init() !=-1, "SDL_Init: Error");
	//---------------------------Start server-----------------
	m_socketSet	= SDLNet_AllocSocketSet(maximumNumberOfPeers); 
	m_socketSetClient	= SDLNet_AllocSocketSet(maximumNumberOfPeers); 
	BOOST_REQUIRE_MESSAGE(m_socketSet, "Can't start server. Error allocating socket set.");
	SDLNet_ResolveHost(&addr, NULL, localPort);
	m_socket = SDLNet_TCP_Open(&addr);
	BOOST_REQUIRE_MESSAGE(m_socket, "Can't start server. Error creating socket." );
	SDL_Delay(100);
	//---------------------Establishment connection------------
	CDataSocket client;
	client.connect("127.0.0.1",10000,m_socketSetClient);//use own socketset
	SDL_Delay(100);
	socketServer = SDLNet_TCP_Accept(m_socket);
	BOOST_REQUIRE(socketServer) ;
	{
	CDataSocket server(socketServer, m_socketSet);
	SDL_Delay(100);

	//------------------- write - read one CData -----------
	{	
		output.clear();
		input.clear();
		CData data("12345",6,true);
		// write CData to Net
		BOOST_REQUIRE(client.isConnected());
		client.write(data);
		BOOST_CHECK(client.flush());
		SDL_Delay(100);
		// read CData from Net
		while (input.size() != 1)  //One Data must be received
		{
			int numready=SDLNet_CheckSockets(m_socketSet, 10);
			BOOST_REQUIRE_MESSAGE(numready!=-1 , "SDLNet_CheckSockets: Error check");
			BOOST_REQUIRE_MESSAGE(numready, "SDLNet_CheckSockets: no action");
			BOOST_CHECK(server.read(input));
		}
		//------------------------Test---------------------
		BOOST_CHECK(input.size() == 1);
		BOOST_CHECK(!strncmp((*(input.begin())).data(),data.data(), (*(input.begin())).length()));
	}
	
	//------------------- write - read 0xFFFA - 0xFFFF CData -----------
	int k = 1;
	for (uint z = 0xfffa; z != 0xffff+1; z++)
	{	
		//printf ("packet size = %u\n",z);
		output.clear();
		input.clear();
		char * str;
		str = new char[z];
		memset(str, k, z);	// For compare packet in sniffer :)
		k++;
		CData data(str,z,true);
		// write CData to Net
		BOOST_REQUIRE(client.isConnected());
		client.write(data);
		BOOST_CHECK(client.flush());
		SDL_Delay(100);
		// read CData from Net
		while (input.size() != 1)  //One Data must be received
		{
			int numready=SDLNet_CheckSockets(m_socketSet, 10);
			BOOST_REQUIRE_MESSAGE(numready!=-1 , "SDLNet_CheckSockets: Error check");
			BOOST_REQUIRE_MESSAGE(numready, "SDLNet_CheckSockets: no action");
			BOOST_CHECK(server.read(input));
		}
		//------------------------Test---------------------
		BOOST_CHECK(input.size() == 1);
		BOOST_CHECK(!strncmp((*(input.begin())).data(),data.data(), (*(input.begin())).length()));//---------------------------
		pingpong( &server, &client, m_socketSetClient);
		delete[] str;
	}
	
	//----------------------------write - read one NULL CData ------
    {	
		output.clear();
		input.clear();
		CData data("12345",6,false); //no store
		// write CData to Net
		BOOST_REQUIRE(client.isConnected());
		client.write(data);
		BOOST_CHECK(!client.flush()); // we don't send NULL
		SDL_Delay(100);
		int numready=SDLNet_CheckSockets(m_socketSet, 10);
		BOOST_REQUIRE_MESSAGE(numready!=-1 , "SDLNet_CheckSockets: Error check");
		BOOST_REQUIRE_MESSAGE(numready, "SDLNet_CheckSockets: no action");
		BOOST_CHECK(server.read(input));
		// We don't received Null  
		BOOST_REQUIRE(input.size() == 0);		
	}
	//------------------- write - read one  CData > 0xFFFF-----------
	{	
		output.clear();
		input.clear();
		char * str;
		str = new char[0xFFFF+1];
		memset(str, 1, (0xFFFF+1));
		CData data(str, (0xFFFF+1),true);
		// write CData to Net
		BOOST_REQUIRE(client.isConnected());
		client.write(data);
		BOOST_CHECK(client.flush());
		SDL_Delay(100);
		// read CData from Net
		int numready=SDLNet_CheckSockets(m_socketSet, 10);
		BOOST_REQUIRE_MESSAGE(numready!=-1 , "SDLNet_CheckSockets: Error check");
		// Packet don't send, therefore we don't receive this
		BOOST_REQUIRE_MESSAGE(!numready, "SDLNet_CheckSockets: no action");
		delete[] str;
	}
	}
	

	//--------------------Close All----------------------------------
	
	if (m_socketSet && m_socket)
		SDLNet_TCP_DelSocket(m_socketSet,m_socket);
	if (m_socket)
	{
		SDLNet_TCP_Close(m_socket);
		m_socket = NULL;
	}
	if (m_socketSet)
	{
		SDLNet_FreeSocketSet(m_socketSet);
		m_socketSet=NULL; 
	}
	if (m_socketSetClient)
	{
		SDLNet_FreeSocketSet(m_socketSetClient);
		m_socketSetClient=NULL; 
	}
 	SDLNet_Quit();
	SDL_Quit();
	if( CLog::instance() )
	{
		CLog::instance()->close();
		// my destroing singleton log, boost warning destroy ;)  
		//CLog::destroyInstance();
	}

}
class CLoopbackLayer:public CUtilityLayer
{
public:
	virtual bool read(std::vector<CData> &inputBuffer)
	{
		inputBuffer.clear();
		copy(m_outputBuffer.begin(), m_outputBuffer.end(), std::back_inserter(inputBuffer));
		return true;
	};
	virtual void write(const CData& data)
	{
		m_outputBuffer.push_back(data);
	};
	virtual bool flush(){return true;};
private:
	std::vector<CData> m_outputBuffer;
};


void testCCompressingLayerIO()
{
		//------------------------ Pack - Unpack 123456'0x0' data ----------
	{
		BOOST_CHECKPOINT("Pack - Unpack 123456'0x0' data");
		CCompressingLayer *CompressingLayer = new CCompressingLayer;
		CLoopbackLayer *LoopbackLayer = new CLoopbackLayer;
		CompressingLayer->SetUnderlyingLayer(LoopbackLayer);
		CData data("12345",6,true);
		CompressingLayer->write(data);
		BOOST_CHECK(CompressingLayer->flush());
		std::vector<CData> input;
		BOOST_CHECK(CompressingLayer->read(input));
		BOOST_CHECK( (*(input.begin())).length() == data.length() );
		BOOST_CHECK(!strncmp((*(input.begin())).data(),data.data(), data.length()));//---------------------------
		delete CompressingLayer;
		delete LoopbackLayer;
	}
	
	//------------------------ Pack - Unpack 1 data size 0XFFFF filled 0xFF------
	{
		BOOST_CHECKPOINT("Pack - Unpack 1 data size 0xFFFF filled 0xFF");
		CCompressingLayer *CompressingLayer = new CCompressingLayer;
		CLoopbackLayer *LoopbackLayer = new CLoopbackLayer;
		CompressingLayer->SetUnderlyingLayer(LoopbackLayer);
		char * str;
		str = new char[0xFFFF];
		memset(str, 0xFF, (0xFFFF));
		CData data(str, (0xFFFF),true);
		CompressingLayer->write(data);
		BOOST_CHECK(CompressingLayer->flush());
		std::vector<CData> input;
		BOOST_CHECK(CompressingLayer->read(input));
		BOOST_CHECK( (*(input.begin())).length() == data.length() );
		BOOST_CHECK(!strncmp((*(input.begin())).data(),data.data(), data.length()));//---------------------------
		delete CompressingLayer;
		delete LoopbackLayer;
		delete[] str;
	}
		//------------------------ Pack - Unpack 0xFF data size 0XFFFF filled 0xFF------
	{
		BOOST_CHECKPOINT("Pack - Unpack 0xFF data size 0xFFFF filled 0xFF");
		CCompressingLayer *CompressingLayer = new CCompressingLayer;
		CLoopbackLayer *LoopbackLayer = new CLoopbackLayer;
		CompressingLayer->SetUnderlyingLayer(LoopbackLayer);
		std::vector<CData> output;
		for (int k = 1; k != 0xA+1; k++)
		{
			char * str;
			str = new char[0xFFFF];
			memset(str, 0xFF, (0xFFFF));
			CData data(str, (0xFFFF),true);
			CompressingLayer->write(data);
			output.push_back(data);
			delete[] str;
		}
		BOOST_CHECK(CompressingLayer->flush());
		
		std::vector<CData> input;
		BOOST_CHECK(CompressingLayer->read(input));
		std::vector<CData>::iterator i;
		std::vector<CData>::iterator j;
		for (i = input.begin(), j = output.begin();  j != output.end(); i++, j++ )
		{	
			BOOST_CHECK( (*i).length() == (*j).length());
			BOOST_CHECK(!strncmp((*i).data(),(*j).data(), (*j).length()));
		}
		delete CompressingLayer;
		delete LoopbackLayer;
		
	}
		//------------------------ Pack - Unpack 0xA data size 0XFFFF filled random data------
	{
		BOOST_CHECKPOINT("Pack - Unpack 0xFF data size 0xFFFF filled 0xFF");
		CCompressingLayer *CompressingLayer = new CCompressingLayer;
		CLoopbackLayer *LoopbackLayer = new CLoopbackLayer;
		CompressingLayer->SetUnderlyingLayer(LoopbackLayer);
		std::vector<CData> output;
		
		for (int k = 0; k != 0xA; k++)
		{
			char * str;
			srand(0xFF);
			str = new char[0xFFFF];
			for (int kk = 0 ; kk != 0xFFFF; kk++)
				str[kk] = (uchar)rand();
			
			CData data(str, (0xFFFF),true);
			CompressingLayer->write(data);
			output.push_back(data);
			delete[] str;
		}
		BOOST_CHECK(CompressingLayer->flush());
		
		std::vector<CData> input;
		BOOST_CHECK(CompressingLayer->read(input));
		std::vector<CData>::iterator i;
		std::vector<CData>::iterator j;
		if (output.size()&&input.size())
		{
			for (i = input.begin(), j = output.begin();  j != output.end(); i++, j++ )
			{	
				BOOST_CHECK( (*i).length() != (*j).length());
				BOOST_CHECK(!strncmp((*i).data(),(*j).data(), (*j).length()));
			}
		}
		delete CompressingLayer;
		delete LoopbackLayer;
		
	}
		//------------------------ Pack - Unpack 0xFF data size 0x1 - 0XFF00 filled random data------
	{
		BOOST_CHECKPOINT("Pack - Unpack 0xFF data size 0x1 - 0XFFFF filled random data");
		CCompressingLayer *CompressingLayer = new CCompressingLayer;
		CLoopbackLayer *LoopbackLayer = new CLoopbackLayer;
		CompressingLayer->SetUnderlyingLayer(LoopbackLayer);
		std::vector<CData> output;
		int size = 0xFF00;
		for (int k = 0; k != 0xFF-1; k++)
		{
			
			char * str;
			srand(0xFF);
			str = new char[size];
			for (int kk = 0 ; kk != size; kk++)
				str[kk] = (uchar)rand();
			CData data(str, (size),true);
			CompressingLayer->write(data);
			output.push_back(data);
			delete[] str;
			size -= 0xFF00/0xFF;
		}
		BOOST_CHECK(CompressingLayer->flush());
		
		std::vector<CData> input;
		BOOST_CHECK(CompressingLayer->read(input));
		std::vector<CData>::iterator i;
		std::vector<CData>::iterator j;
		if (output.size()&&input.size())
		{
			for (i = input.begin(), j = output.begin();  j != output.end(); i++, j++ )
			{	
				BOOST_CHECK( (*i).length() == (*j).length());
				BOOST_CHECK(!strncmp((*i).data(),(*j).data(), (*j).length()));
			}
		}
		delete CompressingLayer;
		delete LoopbackLayer;
		
	}

	//------------------------ Pack - Unpack 0xFF data size 0x1 - 0XFFFF filled random data, another order------
	{
		BOOST_CHECKPOINT("Pack - Unpack 0xFF data size 0x1 - 0XFFFF filled random data, another order");
		CCompressingLayer *CompressingLayer = new CCompressingLayer;
		CLoopbackLayer *LoopbackLayer = new CLoopbackLayer;
		CompressingLayer->SetUnderlyingLayer(LoopbackLayer);
		std::vector<CData> output;
		int size = 0x0;
		for (int k = 0; k != 0xFF; k++)
		{
			size = (k+3)%2*(k+1)*0xFFFF/(2*0xFF)+(k+4)%2*(0xFFFF-(k+1)*0xFFFF/(2*0xFF));
			char * str;
			srand(0xFF);
			str = new char[size];
			for (int kk = 0 ; kk != size; kk++)
				str[kk] = (uchar)rand();
			
			CData data(str, (size),true);
			CompressingLayer->write(data);
			output.push_back(data);
			delete[] str;
			
		}
		BOOST_CHECK(CompressingLayer->flush());
		
		std::vector<CData> input;
		BOOST_CHECK(CompressingLayer->read(input));
		std::vector<CData>::iterator i;
		std::vector<CData>::iterator j;
		if (output.size()&&input.size())
		{
			for (i = input.begin(), j = output.begin();  j != output.end(); i++, j++ )
			{	
				BOOST_CHECK( (*i).length() == (*j).length());
				BOOST_CHECK(!strncmp((*i).data(),(*j).data(), (*j).length()));
		
			}
		}
		delete CompressingLayer;
		delete LoopbackLayer;
		
	}

}
//-----------------------------------------------------------------------
*/
