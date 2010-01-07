/**
 * @file client_interface.h
 * Client interface class
 */

/* Copyright (C) 2005 ireon.org developers council
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

#ifndef _CLIENT_I_INTERFACE_H
#define _CLIENT_I_INTERFACE_H

#include "interface/interface.h"

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
protected:

	void mousePressedNoGUI(MouseEvent *e);

private:


	/** Difference between needed and real y rotation (if 
	     needed position is under ground
	*/
	Radian m_diffY;
	///Distance from camera to character
	Real m_camR;

	///Camera position in previous frame
	Vector3 m_prevCamPos;
	///First person camera on
	bool m_firstPerson;

	CharacterPtr m_target;
	CharacterPtr m_highlight;

	static CClientInterface* m_singleton;
};

#endif