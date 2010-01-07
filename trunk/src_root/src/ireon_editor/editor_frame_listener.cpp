/* Copyright (C) 2005 ireon.org developers council
  * $Id: editor_frame_listener.cpp 693 2006-08-31 08:02:35Z zak $
 
  *  See LICENSE for details
  */
 
 /**
  * @file client_frame_listener.cpp
  * Frame listener
  */
 
 #include "stdafx.h"
 #include "editor_frame_listener.h"
 #include "editor_app.h"
 
 bool CEditorFrameListener::frameStarted(const Ogre::FrameEvent &evt)
 {
 	FrameEventArg arg;
 	arg.m_time = evt.timeSinceLastFrame;
 	CEventManager::instance()->execute(ON_FRAME,&arg);
 	return CEditorApp::instance()->pulse(evt.timeSinceLastFrame);
 };
