/* Copyright (C) 2005 ireon.org developers council
  * $Id: client_frame_listener.h 727 2006-09-14 14:22:14Z mip $
 
  *  See LICENSE for details
  */
 
 /**
  * @file client_frame_listener.h
  * Frame listener for client application pulses
  */
#ifndef _CLIENT_FRAME_LISTENER_H
#define _CLIENT_FRAME_LISTENER_H
#pragma warning(push,1)
#include "Ogre/main/OgreFrameListener.h"
#pragma warning(pop)
#include "ireon_client/interface/client_event_manager.h"
 
 
 class CClientFrameListener:public Ogre::FrameListener
 {
 public:
 	bool frameStarted(const Ogre::FrameEvent& evt);
 };
 
 #endif