/**
 * @file constants.h
 * Different constants
 */
 
/* Copyright (C) 2005 ireon.org developers council
 * $Id: constants.h 693 2006-08-31 08:02:35Z zak $
 
 *  See LICENSE for details
 */
#ifndef _CONSTANTS_H
#define _CONSTANTS_H
 
const String BLANK_STRING = String("");

const uint MAX_INT = 0xFFFFFFFF;

const uint MOB_ID_FLAG = static_cast<uint> (1 << 31);

/// If cosinus of angle is lesser then this value,
/// landscape is impassable
const Real IMPASSABLE = static_cast<Real> (0.1);

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
