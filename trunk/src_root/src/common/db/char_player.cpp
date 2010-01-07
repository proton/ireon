/* Copyright (C) 2005 ireon.org developers council
  * $Id: char_player.cpp 623 2006-05-05 20:19:22Z mip $
 
  *  See LICENSE for details
  */
 
 /**
  * @file char_player.cpp
  * Player's character
  */
 
 #include "stdafx.h"
 #include "db/char_player_data.h"
 #include "db/client_char_data.h"
 
 void CCharPlayerData::serialize(CData& data)
 {
 	CSynchronizer::serialize(data);
 	if( data.store() )
 	{
 		data << m_id;
 		data << m_accountId;
 		data << m_name;
 	} else
 	{
 		data >> m_id;
 		data >> m_accountId;
 		data >> m_name;
 	}
 };
 
 void CCharPlayerData::init()
 {
 	setStartPos(Vector2(0,0));
 };
 
 void CCharPlayerData::toClientOwnCharData(ClientOwnCharData& d)
 {
 	toClientCharPlayerData(d.m_charPlayerData);
 };
 
 void CCharPlayerData::toClientCharPlayerData(ClientCharPlayerData &data)
 {
 	data.m_id = m_id;
 	data.m_name = m_name;
 };
 
 void CCharPlayerData::fromClientOwnCharData(ClientOwnCharData &data)
 {
 	m_id = data.m_charPlayerData.m_id;
 	m_name = data.m_charPlayerData.m_name;
 };