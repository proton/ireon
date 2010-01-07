/* Copyright (C) 2005 ireon.org developers council
 * $Id: client_frame_listener.cpp 750 2006-09-21 20:07:20Z mip $

 *  See LICENSE for details
 */

/**
 * @file client_frame_listener.cpp
 * Frame listener
 */

#include "ireon_client/stdafx.h"
#include "ireon_client/client_frame_listener.h"
#include "ireon_client/client_app.h"
#include "ireon_client/interface/client_event_manager.h"
bool CClientFrameListener::frameStarted(const Ogre::FrameEvent &evt)
{
	FrameEventArg arg;
	arg.m_time = evt.timeSinceLastFrame;
	CEventManager::instance()->execute(ON_FRAME,&arg);
	CClientApp::instance()->pulse(evt.timeSinceLastFrame);
	return true;
};
