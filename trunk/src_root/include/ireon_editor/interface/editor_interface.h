/* Copyright (C) 2005 ireon.org developers council
  * $Id: editor_interface.h 693 2006-08-31 08:02:35Z zak $
 
  *  See LICENSE for details
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
	void enableControl(bool enable){m_enableControl = enable;};
 protected:
 
 	Real m_moveSpeed;
 	Real m_moveScale;

	/// Enable/Disable control key , etc KC_UP, KC_DOWN
	bool m_enableControl;

 	/// Detail solid/wireframe/points
 	int m_sceneDetailIndex;
 private:
	
 	static CEditorInterface* m_singleton;
 };
 
 #endif