/**
* @file client_interface.h
* Client interface class
*/

/* Copyright (C) 2005 ireon.org developers council
* $Id: client_interface.h 844 2007-01-03 13:28:38Z mip $

*  See LICENSE for details
*/

#ifndef _CLIENT_CAMERA_H
#define _CLIENT_CAMERA_H
//#include "extlib/Ogre/main/OgreCamera.h"

class CObject;

class ClientCamera:  protected CLogMixin
{
public:
	//TODO: refactoring. virtual function
	enum ObserveMode
	{
		IN_EYE, // like 3d shooter
		THIRD_PERSON,  // follow for object camera
		FREE_MOVE
	};

	// accept 'target' for observe him
	void watch(CObject* target);

	void setObserveMode(ObserveMode mode);

	void update();

private:

	ObserveMode mode_;
	CObject* target_;

};

#endif
