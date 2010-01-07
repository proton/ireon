/**
  * @file world_mob_prototype.h
  * Mob prototype for world server
  */
 
 /* Copyright (C) 2005 ireon.org developers council
  * $Id: world_mob_prototype.h 806 2006-11-08 14:02:44Z zak $
  
  *  See LICENSE for details
  */
 
 #ifndef _WORLD_MOB_PROTOTYPE_H
 #define _WORLD_MOB_PROTOTYPE_H
 
 class TiXmlNode;
 
 class CWorldMobPrototype
 {
 friend class WSWorld;
 protected:
 	CWorldMobPrototype();
 public:
 	~CWorldMobPrototype();
 
 	bool load(const String& type);
 
 	String getType() const {return m_type;}
 
 	const char* getName() const {return m_name.c_str();}
 
 	float getVelocity() {return m_velocity;}
 	byte getFightSpeed() {return m_fightSpeed;}
 
 	uint getMaxHP() {return m_maxHP;}
 	uint getMaxMana() {return m_maxMana;}
 
 	uint getStr() {return m_str;}
 	uint getSta() {return m_sta;}
 	uint getInt() {return m_int;}
 	uint getWis() {return m_wis;}
 	uint getCon() {return m_con;}
 	uint getLuck() {return m_luck;}
 protected:
 
 	bool processOption(TiXmlNode* option);
 protected:
 
 	// Identifier
	String m_type;
 	//uint m_id;
 
 	/// Mob's name
 	String m_name;
 
 	uint m_maxHP;
 	uint m_maxMana;
 	uint m_str;
 	uint m_sta;
 	uint m_int;
 	uint m_wis;
 	uint m_con;
 	uint m_luck;
 
 	/// Mob's velocity
 	float m_velocity;
 
 	/// Start position
 	Vector2 m_startPos;
 
 	/// Fight speed
 	byte m_fightSpeed;
 };
 
 #endif
