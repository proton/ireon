/* Copyright (C) 2005 ireon.org developers council
  * $Id: interface.h 855 2007-04-27 09:33:36Z mip $
 
  *  See LICENSE for details
  */
 
 /**
  * @file interface.h
  * Interface class
  */
 
#ifndef _I_INTERFACE_H
#define _I_INTERFACE_H

#pragma warning(push, 1)
#include "CEGUI/CEGUIString.h"
#include "Ogre/main/OgreFrameListener.h"
#include "OIS/OIS.h"
#pragma warning(pop)

class CDynamicObject;

class KeyboardActionMap;
class MouseActionMap;

struct KeyEventPL;
struct MouseEventPL;

class ClientCamera;

namespace Ogre
{
	class Camera;
	class RenderWindow;
	class EventProcessor;
};
namespace CEGUI
{
	class System;
	class WindowManager;
	class SchemeManager;
	class ImagesetManager;
}


//------------------------------------------


class CInterface : public Ogre::FrameListener,
	public OIS::KeyListener,
	public OIS::MouseListener,
	protected CLogMixin
{
public:
	enum MousePressed
	{
 		LBUTTON,
 		RBUTTON,
 		MBUTTON
	};
 
	virtual ~CInterface();
	
	CEGUI::System* getGUISystemPtr();
	CEGUI::WindowManager* getGUIWinManagerPtr();
	
	virtual bool init(Ogre::RenderWindow* win);
 	void initOIS();
	void loadCeguiSchemes();

 	/* Update interface in one pulse (1 frame). This function
 	 * is called after world update
 	*/
 	virtual void pulseUpdate(Real time) {time;};

	OIS::Mouse* getMouse() { return m_mouse;}
	OIS::Keyboard* getKeyboard() { return m_keyboard;}

	Degree getRotScale() { return rotateScale_; }

	Camera* getPlayerCam() const {return m_playerCam;}
	ClientCamera* getCamera() const {return camera_;}
	RenderWindow* getRenderWindow() const {return m_window;}
	/*
	float getFontHeight(const String& font = BLANK_STRING);
	float getTextExtent(const String& text, const String& font = BLANK_STRING);

	bool isCursorShown();
	void setShowCursor(bool value);
	*/
	void setRotX(Real degree) {rotX_ = Radian(Degree(degree));}
	void setRotY(Real degree) {rotY_ = Radian(Degree(degree));};

	static KeyboardActionMap* getKeyboardMap() { return keyboardMap_; }

	static MouseActionMap* getMouseMap() { return mouseMap_; }

protected:

	CInterface();
	/** Function is called when user pressed mouse and it wasn't
	*	handled by GUI
	*/
	virtual void mousePressedNoGUI( Ogre::MouseEvent *e ) {e;}

	/** Function is called when user pressed key and it wasn't
	*	handled by GUI
	*/
	virtual void keyPressedNoGUI( Ogre::KeyEvent* e ) {e;}

 	bool frameStarted(const Ogre::FrameEvent &evt);
 	bool frameEnded(const Ogre::FrameEvent &evt);
 
	bool mouseMoved( const OIS::MouseEvent &arg );
	bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
	bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
	
	bool keyPressed( const OIS::KeyEvent &arg );
	bool keyReleased( const OIS::KeyEvent &arg );


public:

	static void setRepeatKeyTime(ulong firstRepeatTime, ulong secondRepeatTime)
	{
		m_firstRepeatTime = firstRepeatTime;
		m_secondRepeatTime = secondRepeatTime;
	};
	
	static void getRepeatKeyTime(ulong& firstRepeatTime, ulong& secondRepeatTime)
	{
		firstRepeatTime = m_firstRepeatTime;
		secondRepeatTime = m_secondRepeatTime;
	}
	/// TODO : move method in ownChar ?
	Degree getRotateScale(){ return rotateScale_;}
	Degree getRotateSpeed(){ return rotateSpeed_;}
	void setRotateScale(Degree rotateScale){ rotateScale_ = rotateScale;}
	void setRotateSpeed(Degree rotateSpeed){ rotateSpeed_ = rotateSpeed;}
	
	Radian getRotateX(){ return rotX_;}
	Radian getRotateY(){ return rotY_;}
	void setRotateX(Radian rotX){ rotX_ = rotX;}
	void setRotateY(Radian rotY){ rotY_ = rotY;}

	CEGUI::Window* getRootWindow();
protected:
 
 	RaySceneQuery* m_query;
 
 	Degree rotateScale_;
 	Degree rotateSpeed_;
 
 	///How much rotate the cam
 	Radian rotX_, rotY_;

	EventProcessor* m_eventProcessor;
	/// time  between press and token time
	static ulong m_firstRepeatTime;
	/// time between token time and send char
	/// etc time send char = time_press( +) + m_first_repeat_time + m_second_repeat_time
	static ulong m_secondRepeatTime;

private:
 
 	Camera* m_playerCam;
	ClientCamera* camera_;
 	RenderWindow* m_window;
 	CEGUI::System* m_GUISystem;
	CEGUI::WindowManager* winMgr_;
	CEGUI::ImagesetManager* imgMgr_;
	CEGUI::SchemeManager* scmMgr_;
	CEGUI::Window* m_mainWindow;
    CEGUI::Renderer* m_GUIRenderer;
 	///Need shutdown
 	bool m_shutdown;

	/// ---------- Input System Variable -----------

	/// layers for processing input system event OIS
	std::auto_ptr<KeyEventPL> keyLayer_;
	std::auto_ptr<MouseEventPL> mouseLayer_;
	/// OIS devices
	OIS::Mouse* m_mouse;
	OIS::Keyboard* m_keyboard;
	/// Keyboard layout 
	static KeyboardActionMap* keyboardMap_;
	static MouseActionMap* mouseMap_;
};
#endif //_I_INTERFACE_H
