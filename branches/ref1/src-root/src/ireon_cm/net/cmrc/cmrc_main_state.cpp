/**
 * @file ireon_cm/net/cmrc/cmrc_main_state.cpp
 * cm root server connection main state
 */

/* Copyright (C) 2005-2006 ireon.org developers council
 * $Id$

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

#include "ireon_cm/net/cmrc/cmrc_main_state.h"
#include "common/net/signal_codes/cmrc_codes.h"
#include "ireon_cm/stdafx.h"
/*
#include "common/net/wscc_codes.h"

#include "db/client_char_data.h"
#include "db/account.h"
#include "db/cm_db.h"
//#include "ireon_rs/accounts/user_account.h"
#include "ireon_cm/cm_app.h"
//#include "ireon_rs/net/root_eserver.h"
*/


CCMRCMainState::CCMRCMainState(CCMRSConnection *ownerConnection):
m_ownerConnection(ownerConnection)
{
	setStateName("Hello State");
/*
	/// register external signals
	registerSlot(ireon::net::cmwcCodes::scMainGetAccount, (CGenericSlot) &CCMWCMainState::onGetAccount);
	registerSlot(ireon::net::cmwcCodes::scMainSendPlayerChar, (CGenericSlot) &CCMWCMainState::onSendPlayerChar);
	registerSlot(ireon::net::cmwcCodes::scMainNewChar, (CGenericSlot) &CCMWCMainState::onNewChar);
	registerSlot(ireon::net::cmwcCodes::scMainRemoveChar, (CGenericSlot) &CCMWCMainState::onRemoveChar);
	/// register internal signals, etc network command
	registerSlot(ireon::net::cmwcCodes::iscMainSendNewChar, (CGenericSlot) &CCMWCMainState::sendNewChar, ireon::net::netAddress::atInternalAddress );
	registerSlot(ireon::net::cmwcCodes::iscMainRemoveChar, (CGenericSlot) &CCMWCMainState::removeChar, ireon::net::netAddress::atInternalAddress);
	registerSlot(ireon::net::cmwcCodes::iscMainSendAccount, (CGenericSlot) &CCMWCMainState::sendAccount, ireon::net::netAddress::atInternalAddress);
*/
}



/*

void CCMWCMainState::onGetAccount(CData& data)
{
	String name;
	int64 sync;
	data >> name;
	data >> sync;
	WaitAccountStruct s;
	s.name = name;
	s.context = m_ownerConnection;
	s.sync = sync;
	CCMDB::instance()->removeWaitAccount(m_ownerConnection);
	CCMDB::instance()->m_waitAccount.push_back(s);
	AccPtr a = CCMDB::instance()->findAccount(name);
//	CCMApp::instance()->getRSConnection()->getConnection()->getAccount(name.c_str(),a.get() ? a->getSync() : 0);
};

void CCMWCMainState::onSendPlayerChar(CData &data)
{
	data;
};

void CCMWCMainState::onNewChar(CData& data)
{
	byte res;
	data >> res;
	if( res != CCE_NONE )
	{
		///It was error, delete char from database
		uint id;
		data >> id;
		CCMDB::instance()->removeCharacter(id);
//		CCMApp::instance()->getRSConnection()->getConnection()->sendNewChar(res,id,0);
	} else
	{
		ClientOwnCharData d;
		d.serialize(data);
//		CCMApp::instance()->getRSConnection()->getConnection()->sendNewChar(res,0,&d);
	};
};

void CCMWCMainState::onRemoveChar(CData& data)
{
	uint id;
	byte res;
	data >> res >> id;
//	CCMApp::instance()->getRSConnection()->getConnection()->removeChar(res,id);
	if( res == 1 )
 		CCMDB::instance()->removeCharacter(id);
};

void CCMWCMainState::sendAccount(CData& data)
{
	CLog::instance()->log(CLog::msgLvlInfo,_("Sending account data to %s.\n"),m_ownerConnection->getHost());
	m_ownerConnection->write(data);
};

void CCMWCMainState::sendNewChar(CData& data)
{
	CData request;
	request.wrt((ireon::net::commandId) ireon::net::wsccCodes::scMainSendNewChar);
	//ch->serialize(data);
	//data << acc;
	request.write(data.data(), data.length());
	
	m_ownerConnection->write(data);
};

void CCMWCMainState::removeChar(CData& data)
{
	CData request;
	request.wrt((byte)((ireon::net::commandId) ireon::net::wsccCodes::scMainRemoveChar));
	request.write(data.data(), data.length());
	m_ownerConnection->write(data);
};

 */
