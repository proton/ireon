/**
 * @file constants.h
 * Different constants
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
#ifndef _CONSTANTS_H
#define _CONSTANTS_H

const String BLANK_STRING = String("");

const uint MAX_INT = 0xFFFFFFFF;

const uint MOB_ID_FLAG = (uint) 1 << 31;

/// If cosinus of angle is lesser then this value,
/// landscape is impassable
const Real IMPASSABLE = 0.1;

const Real CHAR_SIZE = 2;

/// Distance in which near fight starts
const Real CLOSE_COMBAT_DISTANCE = 3;

///==== Some variables for network interaction
/// Max removable lag time (i.e. max lag that can be forgived by server)
const uint MAX_LAG_TIME = 3000; // (ms)
/// Max time interval for difference between sending char position for client
const uint MAX_SEND_POS_TIME = 500; // (ms)

const uint PING_TIME = 5; ///Time in seconds between ping signals

const uint LOST_CONNECTION_TIME = 30; ///Time in seconds when connection closes without pong signals

const byte SIGNALS_START = 1; ///First id of signals (0 - ping)

enum CreateCharacterError
{
	///No error
	CCE_NONE = 0,
	///Character with this name exists
	CCE_EXISTS,

	///Other error
	CCE_OTHER = 100
};

/// Chat commands
enum ChatCommand
{
	CHAT_MSG,		/// Message to channel
	CHAT_PRIVMSG,	/// Private message to character
	CHAT_JOIN,		/// Somebody join channel
	CHAT_PART,		/// Somebody part channel
	CHAT_NAMES,		/// Character's list on channel
	CHAT_CHAN_MODE,	/// Change mode of channel
	CHAT_LIST		/// Channels list
};

///Fight commands
enum FightCommand
{
	FIGHT_ATTACK, /// Attack somebody
	FIGHT_STOP,   /// Stop fight
	FIGHT_HIT,    /// Hit somebody
	FIGHT_DEATH,  /// Somebody is now dead
	FIGHT_MSG     /// Some message about fight
};

#endif
