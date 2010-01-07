/**
  * @file editor_world.h
  * Editor world manager
  */
 
 /* Copyright (C) 2005 ireon.org developers council
  * $Id: editor_world.h 693 2006-08-31 08:02:35Z zak $
 
  *  See LICENSE for details
  */
 
 #ifndef _EDITOR_I_WORLD_H
 #define _EDITOR_I_WORLD_H
 
 #include "world/world.h"
 
 class CClientZone;
 
 class CEditorWorld : public CWorld
 {
 private:
 	CEditorWorld();
 
 public:
 	~CEditorWorld();
 
 	static CEditorWorld* instance() {if( !m_singleton ) m_singleton = new CEditorWorld; return m_singleton;}
 
 	bool init();
 
 	bool createScene();
 
 public:
 
 	/// Update world
 	void update(Real time);
 
 	/// Clear world
 	void clear();
 
 	CClientZone* getZone() {return m_zone;}
 public:
 
 	/* List of all available zones
 	*/
 	StringVector m_zoneList;
 protected:
 
 	/* Current zone
 	*/
 	CClientZone* m_zone;
 
 private:
 
 	static CEditorWorld* m_singleton;
 };
 
 #endif