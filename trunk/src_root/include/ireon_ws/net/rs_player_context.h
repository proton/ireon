/* Copyright (C) 2005 ireon.org developers council
 
  *  See LICENSE for details
  */
 
 /**
  * @file ws_player_context.h
  * Player connection context on world server
  */
 
 #ifndef _RS_PLAYER_CONTEXT_H
 #define _RS_PLAYER_CONTEXT_H
 
 #include "db/account.h"
 
 class CRSPlayerContext : public CClientContext
 {
 public:
 
 	enum State
 	{
 		STATE_NONE = 0,
 		STATE_WAIT_WORLD
 	};
 
 	CRSPlayerContext(CNetServer* server, PlayerID id);
 
 	bool emit(byte id, CData& data);
 
 public:
 	///Commands
 
 	/// 0 = login answer
 
 	///Send destination world, 1
 	void sendWorld(const char* host, uint port);
 
 	/// 2 = create new character
 public:
 
 	///Command handlers
 
 	///Login attempt, 0
 	void onLogin(CData& data);
 
 	///Select char, 1
 	void onSelectChar(CData& data);
 
 	///Create new char, 2
 	void onCreateChar(CData& data);
 
 public:
 
 	AccPtr getAccount() { return m_account; }
 
 protected:
 
 	AccPtr m_account;
 
 	byte m_state;
 };
 
 
 #endif