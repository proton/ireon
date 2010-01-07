/**
 * @file ireon_rs/net/rspc_connection.h
 * root server to player client connection class
 */

/* Copyright (C) 2005-2006 ireon.org developers council
 * $Id: rspc_connection.h 721 2006-09-12 12:54:00Z zak $

 * See LICENSE for details
 */

#ifndef _RSPC_CONNECTION_H
#define _RSPC_CONNECTION_H

#include "common/net/net_connection.h"
#include "ireon_rs/db/user_account.h"
#include <memory>

class CRSPCConnection : public CNetConnection
{
public:
	enum RSPCStates
	{
		versionCheck,
		hello,
		main
	};
	void setAccount(std::auto_ptr<CUserAccount> acc) { m_acc = acc; }
	std::auto_ptr<CUserAccount>& getAccount() { return m_acc; }

private:
	std::auto_ptr<CUserAccount>	m_acc;
};

#endif
