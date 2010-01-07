/**
* @file client_static.h
* Static object
*/

/* Copyright (C) 2005 ireon.org developers council
* $Id: client_static.h 829 2006-11-30 18:39:37Z mip $

*  See LICENSE for details
*/
#ifndef _PROTOTYPE_H
#define _PROTOTYPE_H

class CSegment;

class CPrototype: protected CLogMixin
{
public:
	CPrototype();
	virtual ~CPrototype();
	/// load prototypes : load id, name ... need use autoId within @see CPrototypeManager. Name need used only for beaty... FIX ME!
	virtual bool load(const String& resourceName);
	
	const char* getMeshName() const {return meshName_.c_str();}

	void setMeshName(const char* meshName) { meshName_ = meshName;}


	const char* getName() const {return name_.c_str();}
	
	void setName(const char* name) { name_ = name;}

	const Vector3& getScale() const {return scale_;}
	void setScale(const Vector3& scale) { scale_ = scale;}

	const Vector3& getSize() const {return size_;}

protected:

	String name_;

	/// Name of object's mesh
	String meshName_;

	/* One of scale or size parameters can be used
	* if scale parameter is specified, it will be used
	* even if size paramter is specified.
	*/
	/// Scale by axes
	Vector3 scale_;
	/// Size of object
	Vector3 size_;
};


#endif
