/* Copyright (C) 2005-2006 ireon.org developers council
 * $Id: client_frame_listener.cpp 610 2006-04-20 11:04:59Z zak $

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
 * @file client_frame_listener.cpp
 * Frame listener
 */

#include "stdafx.h"
#include "client_frame_listener.h"
#include "client_app.h"

bool CClientFrameListener::frameStarted(const Ogre::FrameEvent &evt)
{
	FrameEventArg arg;
	arg.m_time = evt.timeSinceLastFrame;
	CEventManager::instance()->execute(ON_FRAME,&arg);
	return CClientApp::instance()->pulse(evt.timeSinceLastFrame);
};
