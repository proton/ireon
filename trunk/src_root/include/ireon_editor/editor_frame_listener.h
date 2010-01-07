/* Copyright (C) 2005 ireon.org developers council
  * $Id: editor_frame_listener.h 693 2006-08-31 08:02:35Z zak $
 
  *  See LICENSE for details
  */
 
 /**
  * @file client_frame_listener.h
  * Frame listener for client application pulses
  */
 #ifndef _CLIENT_FRAME_LISTENER_H
 #define _CLIENT_FRAME_LISTENER_H
 
 #include "OgreFrameListener.h"
 
 
 
 class CEditorFrameListener:public Ogre::FrameListener
 {
 public:
 	bool frameStarted(const Ogre::FrameEvent& evt);
 };
 
 #endif