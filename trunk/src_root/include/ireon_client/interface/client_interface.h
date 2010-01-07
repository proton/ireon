/**
 * @file client_interface.h
 * Client interface class
 */

/* Copyright (C) 2005 ireon.org developers council
 * $Id: client_interface.h 855 2007-04-27 09:33:36Z mip $

 *  See LICENSE for details
 */

#ifndef _CLIENT_I_INTERFACE_H
#define _CLIENT_I_INTERFACE_H
#include "common/interface/interface.h"
#include "boost/shared_ptr.hpp"
#pragma warning(push,1)
#include <CEGUI/CEGUISystem.h>
#pragma warning(pop)

class CDynamicObject;
typedef boost::shared_ptr<CDynamicObject> CharacterPtr;
#define I_INTERFACE (CClientInterface::instance())

class CClientInterface : public CInterface
{
private:
	CClientInterface();
public:
	~CClientInterface();

	static CClientInterface* instance() {{if( !m_singleton ) m_singleton = new CClientInterface; return m_singleton;}}

	bool init(RenderWindow* win);

	void pulseUpdate(Real time);

	bool frameStarted(const FrameEvent &evt);

	bool isFirstPerson() {return m_firstPerson;}
	void setFirstPerson(bool value) {m_firstPerson = value;}

	CharacterPtr getTarget();
	void setTarget(CharacterPtr target) {m_target = target;}
	void nextTarget();

	CharacterPtr getHighlight() {return m_highlight;}

	void setCamR(Real radius) {m_camR = radius;}

	void mouseMoved(MouseEvent *e);
	
	void enableControl(bool enable){m_enableControl = enable;};

	Real getTimeLastUpdate(){ return time_last_update_; }

	/// Control for actions camera
	Real getCameraSpeed()const {return camSpeed_;}
	void setCameraSpeed(Real speed) {camSpeed_ = speed;}

protected:

	void mousePressedNoGUI(MouseEvent *e);
	
private:
	/// changed in update()
	Real time_last_update_;

	/** Difference between needed and real y rotation (if 
	     needed position is under ground
	*/
	Radian m_diffY;
	///Distance from camera to character
	Real m_camR;
	/// Speed moving camera
	Real camSpeed_;
	///Camera position in previous frame
	Vector3 m_prevCamPos;
	///First person camera on
	bool m_firstPerson;
	/// Enable/Disable control key , etc KC_UP, KC_DOWN
	bool m_enableControl;

	CharacterPtr m_target;
	CharacterPtr m_highlight;

	static CClientInterface* m_singleton;
	
	
	
};

#endif
