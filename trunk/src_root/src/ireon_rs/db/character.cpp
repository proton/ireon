/**
 * @file ireon_rs/db/character.cpp
 * root server character info class
 */

/* Copyright (C) 2005-2006 ireon.org developers council
 * $Id: character.cpp 775 2006-09-29 20:11:59Z zak $

 * See LICENSE for details
 */

#include "ireon_rs/db/character.h"

#include "common/db/mysql.h"
#include "common/misc/mysqlpp_wrapper.h"

CRSCharacterInfo::CRSCharacterInfo():
m_id(0),
m_owner(0),
m_ws(nullWSId),
m_name("")
{
}

// --------------------------------------------------------

CRSCharacterInfo::CRSCharacterInfo(String &name, bool loadFromDB):
m_id(0),
m_owner(0),
m_ws(nullWSId),
m_name(name)
{
	if (loadFromDB)
	{
		try
		{
			mysqlpp::Query	query = CMySQLConnection::instance()->getQuery();

			query << "select * from `characters` where `name`=" << mysqlpp::quote << m_name ;

			mysqlpp::Result result = query.store();

			if (result.size())	{
				mysqlpp::Row row;
				row	= result.at(0);
				m_id = row["id"];
				m_owner = row["owner"];
				m_ws = row["ws"];
			} else
				throw ECharacterNotFound();
		}
		catch (mysqlpp::Exception&)
		{
			throw EDBError();
		}

	}
}

// --------------------------------------------------------

CRSCharacterInfo::CRSCharacterInfo(ushort id, bool loadFromDB):
m_id(id),
m_owner(0),
m_ws(nullWSId)
{
	if (loadFromDB)
	{
		try
		{
			mysqlpp::Query	query = CMySQLConnection::instance()->getQuery();

			query << "select * from `characters` where `id`=" << static_cast<int> (id) ;

			mysqlpp::Result result = query.store();

			if (result.size())	{
				mysqlpp::Row row;
				row	= result.at(0);
				m_id = row["id"];
				m_owner = row["owner"];
				m_ws = row["ws"];
				m_name = row["name"];
			} else
				throw ECharacterNotFound();
		}
		catch (mysqlpp::Exception&)
		{
			throw EDBError();
		}

	}
}

// --------------------------------------------------------

void CRSCharacterInfo::saveToDB()
{
	try
	{
		mysqlpp::Query	query = CMySQLConnection::instance()->getQuery();
		if (!m_id)
		{
			if (!m_owner)
				throw EOwnerNotSet();

			query << "insert into `characters` (`id`,`name`,`owner`,`ws`) values (default, " << 
						mysqlpp::quote << m_name << ", " << m_owner << ", " << static_cast<int> (m_ws) << ")";
			query.execute();
			mysqlpp::Query	idquery = CMySQLConnection::instance()->getQuery();
			idquery << "select `id` from `characters` where `name`=" << mysqlpp::quote << m_name ;
			mysqlpp::Result result = idquery.store();
			if (result.size())	{
				mysqlpp::Row row;
				row	= result.at(0);
				m_id = row["id"];
			} else
				throw EDBError();

		} else	{
			query << "replace into `characters` (`id`,`name`,`owner`,`ws`) values(" << static_cast<int> (m_id) << ", " << 
			mysqlpp::quote << m_name << ", " << m_owner << ", " << static_cast<int> (m_ws) << ")";
			query.execute();
		}
	}
	catch (mysqlpp::Exception&)
	{
		throw EDBError();
	}
}


// --------------------------------------------------------------------

byte CRSCharacterInfo::getCharactersCount (uint accountId)
{
	assert(accountId);

	try
	{
 		mysqlpp::Query		query = CMySQLConnection::instance()->getQuery();
		query << "select * from `characters` where `owner`=" << accountId;
		mysqlpp::Result result = query.store();
		return static_cast<byte> (result.size());
	}
	catch (mysqlpp::Exception&)
	{
		throw EDBError();
	}
}

// --------------------------------------------------------------------

void CRSCharacterInfo::getCharacters (uint accountId, std::map<ushort, boost::shared_ptr<CRSCharacterInfo>> &charlist)
{
	assert(accountId);

	mysqlpp::Result result;
	try
	{
		mysqlpp::Query		query = CMySQLConnection::instance()->getQuery();
		query << "select * from `characters` where `owner`=" << accountId;
		result = query.store();
	}
	catch (mysqlpp::Exception&)
	{
		throw EDBError();
	}	

	for (mysqlpp::Result::iterator i = result.begin(); i != result.end(); ++i)
	{
		boost::shared_ptr<CRSCharacterInfo> p(new CRSCharacterInfo());
		p->m_id = (*i)["id"];
		p->m_name = (*i)["name"];
		p->m_owner = (*i)["owner"];
		p->m_ws = (*i)["ws"];
		charlist[p->m_id] = p;
	}

}
/*
bool CGeneralCharacterInfo::removeFromDB()
{
	assert(m_id);

 	mysqlpp::Query		query = CMySQLConnection::instance()->getQuery();

	query << "delete from `" << CMySQLConnection::instance()->getTableNamesPrefix() << 
				"characters` where `id`=" << m_id;

	return query.exec(query.str());

}

*/
