/* Copyright (C) 2005-2006 ireon.org developers council
 * $Id: client_frame_listener.h 522 2006-03-06 16:49:30Z zak $

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

/**
 * @file client_frame_listener.h
 * Frame listener for client application pulses
 */
#ifndef _CLIENT_FRAME_LISTENER_H
#define _CLIENT_FRAME_LISTENER_H

#include "OgreFrameListener.h"



class CClientFrameListener:public Ogre::FrameListener
{
public:
	bool frameStarted(const Ogre::FrameEvent& evt);
};

#endif