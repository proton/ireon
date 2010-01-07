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

#ifndef _I_INTERFACE_H
#define _I_INTERFACE_H

#include "OgreFrameListener.h"
#include "OgreKeyEvent.h"
#include "OgreEventListeners.h"
#include "OgreInput.h"


class CCharacter;

namespace Ogre{
class Camera;
class RenderWindow;
class EventProcessor;
};

class CInterface : public Ogre::FrameListener,
	public KeyListener,
	public MouseMotionListener,
	public MouseListener
{
protected:
	CInterface();
public:

	enum WinType
	{
		BASIC,
		FRAME,
		STATIC_TEXT,
		STATIC_IMAGE,
		MULTI_LIST,
		CHECK_BOX,
		LIST,
		SCROLL_BAR,
		PROGRESS_BAR
	};

	enum MousePressed
	{
		LBUTTON,
		RBUTTON,
		MBUTTON
	};

	virtual ~CInterface();

	//static CInterface* instance() {{if( !m_singleton ) m_singleton = new CInterface; return m_singleton;}}

	virtual bool init(Ogre::RenderWindow* win);

	/* Update interface in one pulse (1 frame). This function
	 * is called after world update
	*/
	virtual void pulseUpdate(Real time) {};
protected:
	/// Init convertion from english to russian letters
	void initConvert();

	bool frameStarted(const Ogre::FrameEvent &evt);
	bool frameEnded(const Ogre::FrameEvent &evt);

    void mouseMoved (Ogre::MouseEvent *e);

	void mouseDragged (Ogre::MouseEvent *e);

	void keyReleased(Ogre::KeyEvent* e);

private:
    void mousePressed (Ogre::MouseEvent *e);

    void mouseReleased (Ogre::MouseEvent *e);

	void mouseClicked(Ogre::MouseEvent* e);

	void mouseEntered(Ogre::MouseEvent* e);

	void mouseExited(Ogre::MouseEvent* e);

    void keyPressed(Ogre::KeyEvent* e);

	void keyClicked(Ogre::KeyEvent* e);
protected:

	/** Function is called when user pressed mouse and it wasn't
	 *	handled by GUI
	*/
	virtual void mousePressedNoGUI( Ogre::MouseEvent *e ) {}

	/** Function is called when user pressed key and it wasn't
	 *	handled by GUI
	*/
	virtual void keyPressedNoGUI( Ogre::KeyEvent* e ) {}
public:
	///Window operations
	///Destroy window object
	void destroyWindow(const WndPtr& window, WndPtr root = NULL_WINDOW);
	///Create new window
	WndPtr createWindow(const String& type, const String& name, WinType wtype = BASIC);
	///Get existing window
	WndPtr getWindow(const String& name, WinType type = BASIC);
	///Find window in tree
	WndPtr findWindow(const String& name, WndPtr root = NULL_WINDOW);
	///Unlink window (remove from tree, but not destroy)
	WndPtr unlinkWindow(const WndPtr& window, WndPtr root = NULL_WINDOW );
	bool linkWindow(const WndPtr& win, WndPtr root = NULL_WINDOW );

	bool hasChildren(const CWindow* win, const CWindow* root);

	///Disable all windows, maybe except one
	void disableWindows(const CWindow* except, CWindow* root = NULL);
	void enableWindows(WndPtr root = NULL_WINDOW);

	bool isKeyPressed(KeyCode code) {return m_keysPressed.find(code) != m_keysPressed.end();}
	bool onlyOneKeyPressed() {return m_keysPressed.size() == 1;}

	void switchLayout() {m_altLayout = (m_altLayout ? false : true);}

	Camera* getPlayerCam() const {return m_playerCam;}
	RenderWindow* getRenderWindow() const {return m_window;}

	float getFontHeight(const String& font = BLANK_STRING);
	float getTextExtent(const String& text, const String& font = BLANK_STRING);

	bool isCursorShown();
	void setShowCursor(bool value);

	void setRotX(Real degree) {m_rotX = Radian(Degree(degree));}
	void setRotY(Real degree) {m_rotY = Radian(Degree(degree));};
private:
	CWindow* newWindowObject(WinType type, CEGUI::Window* win);

protected:

	RaySceneQuery* m_query;

	Degree m_rotScale;
	Degree m_rotateSpeed;

	///How much rotate the cam
	Radian m_rotX, m_rotY;

	std::set< KeyCode > m_keysPressed;
	std::set< MousePressed > m_mousePressed;

	/** Alternative layout enabled
	*/
	bool m_altLayout;

	EventProcessor* m_eventProcessor;

private:

	Camera* m_playerCam;
	
	RenderWindow* m_window;

	CEGUI::System* m_GUISystem;
    WndPtr m_mainWindow;

    CEGUI::Renderer* m_GUIRenderer;

	///Need shutdown
	bool m_shutdown;
};

#endif