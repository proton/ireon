/* Copyright (C) 2005 ireon.org developers council
  * $Id: char_player.h 623 2006-05-05 20:19:22Z mip $
  
  *  See LICENSE for details
  */
 
 /**
  * @file char_player.h
  * Player's character
  */
 
 #ifndef _CHAR_PLAYER_H
 #define _CHAR_PLAYER_H
 
 struct ClientOwnCharData;
 struct ClientCharData;
 struct ClientCharPlayerData;
 
 class CCharPlayerData :public CSynchronizer
 {
 	friend class CRootDB;
 public:
 	CCharPlayerData() {};
 	~CCharPlayerData() {};
 
 	///Initialize new character
 	void init();
 
 	void serialize(CData& d);
 
 	void toClientOwnCharData(ClientOwnCharData& data);
 	void toClientCharPlayerData(ClientCharPlayerData& data);
 	void fromClientOwnCharData(ClientOwnCharData& data);
 
 	uint getAccountId() {return m_accountId;}
 	void setAccountId(uint id) {m_accountId = id; updSync();}
 
 	uint getId() {return m_id;}
 
 	///Name must be associated with id to avoid errors
 	///void setName(const char* str) {m_name = str; updSync();}
 	const char* getName() {return m_name.c_str();}
 
 	Vector2 getStartPos() {return m_startPos;}
 	void setStartPos(const Vector2& pos) {m_startPos = pos; updSync();}
 protected:
 
 	uint m_id;
 
 	uint m_accountId;
 
 	AccPtr m_account;
 
 	String m_name;
 
 	Vector2 m_startPos;
 };
 
 #endif