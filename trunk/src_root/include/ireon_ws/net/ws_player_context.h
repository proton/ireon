/* Copyright (C) 2005 ireon.org developers council
  * $Id: ws_player_context.h 670 2006-07-31 18:41:05Z zak $
 
  *  See LICENSE for details
  */
 
 /**
  * @file ws_player_context.h
  * Player connection context on world server
  */
 
 #ifndef _WS_PLAYER_CONTEXT_H
 #define _WS_PLAYER_CONTEXT_H

/*
 #include "net/net_data_processor.h"
 
 struct ClientOwnCharData;
 
 class CWSPlayerContext;
 
 typedef void (CWSPlayerContext::*CWSSig)(CData&);
 
 class CWSPlayerContext : public CNetDataProcessor
 {
 public:
 	CWSPlayerContext(TCPsocket socket, SDLNet_SocketSet socketSet);
 
 	bool processCommand(CData& data, byte id);
 
 	static void initSignals();
 
 	void onClose();
 public:
 	///Commands
 
 	/// 0 - login
 	/// Send initial info to player when logged in
 	void sendInitialInfo(CData& data);
 
 	/// 1, Send characters positions to player
 	void sendCharactersPositions();
 
 	/// 2, Send chat command
 	void sendChatCommand(ChatCommand cmd, const String& arg1, const String& arg2, const String& arg3);
 
 	/// 3, Send messase about hit
 	void sendHit(uint chId, uint enemyId, byte percentHP, byte flags);
 	/// Message about death
 	void sendDeath(uint ch, uint enemy);
 
 	/// 4, Resurrect info
 	void sendResurrect();
 public:
 
 	///Command handlers
 
 	/// 0, Login player
 	void onLogin(CData& data);
 
 	/// 1, Player sends its position
 	void onSendPos(CData& data);
 
 	/// 2, Player sends chat command
 	void onChatCmd(CData& data);
 
 	/// 3, Player sends fight command
 	void onFightCmd(CData& data);
 
 	/// 4, Player sends resurrect command
 	void onResurrect(CData& data);
 public:
 
 	/// Enter game when logged in
 	void enterGame(AccPtr a);
 
 	AccPtr getAccount() {return m_account;}
 
 	CharPlayerPtr getCharacter() {return m_character;}
 protected:
 	///Account (if logged in)
 	AccPtr m_account;
 	CharPlayerPtr m_character;
 
 	///Login and password when waiting authentication
 	String m_login;
 	String m_password;
 
 	///Signals
 	static std::vector<CWSSig> m_signals;
 };
 */
 
 #endif
