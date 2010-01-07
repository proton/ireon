/**
* @file object_manager.h
* Command
* 
*/

/* Copyright (C) 2005 ireon.org developers council
* $Id: command.h 825 2006-11-29 14:39:37Z mip $

*  See LICENSE for details
*/

#ifndef _COMMAND_H
#define _COMMAND_H
#include "boost/function.hpp"

/*
			CCommand
		/				  \
		/				   \
CObjectCommand				   CProgramCommand
	 /           \					/          \
	/             \					/            \
CStaticObjCom  CDynamicObjCom   CSystemCommand  CManagerCommand  
	 /
	/
CMoveAbsStaticObj
*/

class CStaticObject;

class CCommand: protected CLogMixin
{
public:
	virtual void execute()=0;
	virtual String description() const =0;
	virtual ~CCommand(){};
};

class CProgramCommand: public CCommand
{
public:
	virtual void execute()=0;
	virtual String description() const =0;
	virtual ~CProgramCommand(){};
};

class CSystemCommand: public CProgramCommand
{
public:
	virtual void execute()=0;
	virtual String description() const =0;
	virtual ~CSystemCommand(){};
};


class CManagerCommand: public CProgramCommand
{
public:
	virtual void execute()=0;
	virtual String description() const =0;
	virtual ~CManagerCommand(){};
};


class CCreateOwnPlayerCommand: public CManagerCommand
{
public:
	CCreateOwnPlayerCommand(const String& protypeName):
		protypeName_(protypeName)
		{};
	virtual void execute();
	virtual String description() const;
	virtual ~CCreateOwnPlayerCommand(){};
protected:
	String protypeName_;
};

class CCreateStaticObjectCommand: public CManagerCommand
{
public:
	CCreateStaticObjectCommand(const String& protypeName,const String& objectName):
		protypeName_(protypeName),
		objectName_(objectName) {};
	virtual void execute();
	virtual String description() const;
	virtual ~CCreateStaticObjectCommand(){};
protected:
	String protypeName_;
	String objectName_;
};

class CLoadOwnPlayerPrototypeCommand: public CManagerCommand
{
public:
	virtual void execute();
	virtual String description() const;
	virtual ~CLoadOwnPlayerPrototypeCommand(){};
};

class CLoadStaticPrototypesCommand: public CManagerCommand
{
public:
	virtual void execute();
	virtual String description() const;
	virtual ~CLoadStaticPrototypesCommand(){};
};


class CObjectCommand: public CCommand
{
public:
	virtual void execute()=0;
	virtual String description()const=0 ;
	virtual ~CObjectCommand(){};
};

class CStaticObjectCommand: public CObjectCommand
{
public:
	virtual void execute() = 0;
	virtual String description()const=0 ;
	virtual ~CStaticObjectCommand(){};
};

class CDynamicObjectCommand: public CObjectCommand
{
public:
	virtual void execute() = 0;
	virtual String description()const=0 ;
	virtual ~CDynamicObjectCommand(){};
};

class CMoveAbsoluteStaticObjectCommand: public CStaticObjectCommand
{
public:
	CMoveAbsoluteStaticObjectCommand(const String& objectName, Vector3 pos): m_objectName(objectName), m_pos(pos) {};
	virtual void execute();
	virtual String description() const;
	virtual ~CMoveAbsoluteStaticObjectCommand(){};
protected:
	Vector3 m_pos;
	String m_objectName;
};

class CRotateAbsoluteStaticObjectCommand: public CStaticObjectCommand
{
public:
	CRotateAbsoluteStaticObjectCommand(const String& objectName, Quaternion q): objectName_(objectName), q_(q) {};
	virtual void execute();
	virtual String description() const;
	virtual ~CRotateAbsoluteStaticObjectCommand(){};
protected:
	Quaternion q_;
	String objectName_;
};


class CLoadStaticObjectCommand: public CStaticObjectCommand
{
public:
	CLoadStaticObjectCommand(const String& objectName): m_objectName(objectName){};
	virtual void execute();
	virtual String description() const;
	virtual ~CLoadStaticObjectCommand(){};
protected:
	String m_objectName;
};

class CLoadAllStaticObjectCommand: public CStaticObjectCommand
{
public:
	CLoadAllStaticObjectCommand(boost::function<bool (CStaticObject)> statement):m_statement(statement){};
	virtual void execute();
	virtual String description() const;
	virtual ~CLoadAllStaticObjectCommand(){};
protected:
	boost::function<bool (CStaticObject)> m_statement;
};

class CUnloadAllStaticObjectCommand: public CStaticObjectCommand
{
public:
	CUnloadAllStaticObjectCommand(boost::function<bool (CStaticObject)> statement):m_statement(statement){};
	virtual void execute();
	virtual String description() const;
	virtual ~CUnloadAllStaticObjectCommand(){};
protected:
	boost::function<bool (CStaticObject)> m_statement;
};


class CQuitCommand: public CSystemCommand
{
public:
	CQuitCommand(){};
	virtual void execute();
	virtual String description() const;
	virtual ~CQuitCommand(){};
};

class CRightRotateCameraCommand: public CDynamicObjectCommand 
{
public:
	virtual void execute();
	virtual String description()const;
	virtual ~CRightRotateCameraCommand(){};
};

class CLeftRotateCameraCommand: public CDynamicObjectCommand 
{
public:
	virtual void execute();
	virtual String description()const;
	virtual ~CLeftRotateCameraCommand(){};
};

class CUpRotateCameraCommand: public CDynamicObjectCommand 
{
public:
	virtual void execute();
	virtual String description()const;
	virtual ~CUpRotateCameraCommand(){};
};
class CDownRotateCameraCommand: public CDynamicObjectCommand 
{
public:
	virtual void execute();
	virtual String description()const;
	virtual ~CDownRotateCameraCommand(){};
};
class CForwardMoveObjectCommand: public CDynamicObjectCommand 
{
public:
	virtual void execute();
	virtual String description()const;
	virtual ~CForwardMoveObjectCommand(){};
};
class CBackMoveObjectCommand: public CDynamicObjectCommand 
{
public:
	virtual void execute();
	virtual String description()const;
	virtual ~CBackMoveObjectCommand(){};
};
class CLeftMoveObjectCommand: public CDynamicObjectCommand 
{
public:
	virtual void execute();
	virtual String description()const;
	virtual ~CLeftMoveObjectCommand(){};
};
class CRightMoveObjectCommand: public CDynamicObjectCommand 
{
public:
	virtual void execute();
	virtual String description()const;
	virtual ~CRightMoveObjectCommand(){};
};
//------------------------------------------------------------------
class CLeftRotateObjectCommand: public CDynamicObjectCommand 
{
public:
	virtual void execute();
	virtual String description()const;
	virtual ~CLeftRotateObjectCommand(){};
};
class CRightRotateObjectCommand: public CDynamicObjectCommand 
{
public:
	virtual void execute();
	virtual String description()const;
	virtual ~CRightRotateObjectCommand(){};
};

class CLookCenterCameraCommand: public CDynamicObjectCommand 
{
public:
	virtual void execute();
	virtual String description()const;
	virtual ~CLookCenterCameraCommand(){};
};

class CForwardMoveCameraCommand: public CDynamicObjectCommand 
{
public:
	virtual void execute();
	virtual String description()const;
	virtual ~CForwardMoveCameraCommand(){};
};

class CBackMoveCameraCommand: public CDynamicObjectCommand 
{
public:
	virtual void execute();
	virtual String description()const;
	virtual ~CBackMoveCameraCommand(){};
};

class CDecreaseSpeedCameraCommand: public CDynamicObjectCommand 
{
public:
	virtual void execute();
	virtual String description()const;
	virtual ~CDecreaseSpeedCameraCommand(){};
};
class CIncreaseSpeedCameraCommand: public CDynamicObjectCommand 
{
public:
	virtual void execute();
	virtual String description()const;
	virtual ~CIncreaseSpeedCameraCommand(){};
};

//-------------------------- Follow object ( 3D - out eye view ) --------------

class CLeftRotateFollowObjectCommand: public CDynamicObjectCommand 
{
public:
	virtual void execute();
	virtual String description()const;
	virtual ~CLeftRotateFollowObjectCommand(){};
};
class CRightRotateFollowObjectCommand: public CDynamicObjectCommand 
{
public:
	virtual void execute();
	virtual String description()const;
	virtual ~CRightRotateFollowObjectCommand(){};
};

class CForwardMoveFollowObjectCommand: public CDynamicObjectCommand 
{
public:
	virtual void execute();
	virtual String description()const;
	virtual ~CForwardMoveFollowObjectCommand(){};
};

class CBackMoveFollowObjectCommand: public CDynamicObjectCommand 
{
public:
	virtual void execute();
	virtual String description()const;
	virtual ~CBackMoveFollowObjectCommand(){};
};

class CShowCurrentTriangleNavMeshInfoCommand: public CSystemCommand 
{
public:
	virtual void execute();
	virtual String description()const;
	virtual ~CShowCurrentTriangleNavMeshInfoCommand(){};
};
#endif