/**
  * @file dispatcher.h
  * dispatcher for object (yet object manager) commands
  */
 
 /* Copyright (C) 2005 ireon.org developers council
  * $Id: dispatcher.h 822 2006-11-22 13:34:49Z mip $
 
  *  See LICENSE for details
  */

#include "stdafx.h"
#include "boost/shared_ptr.hpp"
#include "common/world/managers/command.h"
typedef boost::shared_ptr<CCommand> CommandPtr;
class CDispatcher: protected CLogMixin
{
public:
	/// get pointer on singleton
	static CDispatcher* getSingletonPtr();
	/// get reference on singleton
	static CDispatcher& getSingleton();

	virtual void addCommand(CommandPtr& command)
	{
		m_commands.push_back(command);
	};
	virtual void executeAllCommand()
	{
		std::vector<CommandPtr> m_commands_reserve = m_commands;
		m_commands.clear();
		String desc;
		std::vector<CommandPtr>::iterator i;
		for(i = m_commands_reserve.begin(); i != m_commands_reserve.end(); i++)
		{
			desc = (*i)->description();
			_log(_info,_loc,_("Command: %s"), desc.c_str() );
			(*i)->execute();
		};

	};
	virtual void executeNow(const CommandPtr& command)
	{
		String desc = command->description();
		_log(_info,_loc,_("Command: %s"), desc.c_str() );
		command->execute();
	};
private:    
	std::vector<CommandPtr> m_commands;

	/// private constructor, therefore use getSingletonPtr() or getSingleton()
	CDispatcher(void){};
	~CDispatcher(void){};
	/// instance of singleton
	static CDispatcher* m_singleton;
};