/* Copyright (C) 2005 ireon.org developers council
 * $Id: interface.h 510 2006-02-26 21:09:40Z zak $

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
 * @file interface.h
 * Interface class
 */

#ifndef _EDITOR_I_INTERFACE_H
#define _EDITOR_I_INTERFACE_H

#include "interface/interface.h"

class CEditorInterface : public CInterface
{
protected:

	CEditorInterface();

public:
	~CEditorInterface();

	static CEditorInterface* instance() {{if( !m_singleton ) m_singleton = new CEditorInterface; return m_singleton;}}

	bool init(RenderWindow* win);

	bool frameStarted(const FrameEvent &evt);

	void mouseMoved(MouseEvent *e);

	void mousePressedNoGUI(MouseEvent *e);

	void keyPressedNoGUI(KeyEvent *e);

	void pulseUpdate(Real time);
protected:

	Real m_moveSpeed;
	Real m_moveScale;

	/// Detail solid/wireframe/points
	int m_sceneDetailIndex;
private:

	static CEditorInterface* m_singleton;
};

#endif