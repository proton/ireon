#ifndef _OGREODESPACE_H_
#define _OGREODESPACE_H_

#include "OgreOdePreReqs.h"
#include "OgreOdeMaintainedList.h"

namespace OgreOde 
{
	class _OgreOdeExport Space:public MaintainedItem
	{
		friend class Geometry;
		friend class World;

	public:
		Space(const Space* space = 0);
		virtual ~Space();

		void registerSpace();

		void setAutoCleanup(bool on); 
		bool getAutoCleanup(); 

		void addGeometry(const Geometry& geometry); 
		void removeGeometry(const Geometry& geometry); 
		bool containsGeometry(const Geometry& geometry); 
		int getGeometryCount(); 
		Geometry* getGeometry(int index);

		void collide(void* data = 0);
		void collide(Space* space,void* data = 0);
		void collide(Geometry* geometry,void* data = 0);

		void setInternalCollisions(bool collide);
		bool getInternalCollisions();

		virtual const AxisAlignedBox& getAxisAlignedBox(); 

		virtual unsigned long getID();

	protected:
		dSpaceID getSpaceID() const;
		dSpaceID getSpaceID(const Space* space) const;

	protected:
		dSpaceID _space;
		bool _internal_collisions;
		AxisAlignedBox _bounding_box;
	};

	class _OgreOdeExport SimpleSpace:public Space
	{
	public:
		SimpleSpace(const Space* space = 0);
		~SimpleSpace();
	};

	class _OgreOdeExport HashTableSpace:public Space
	{
	public:
		HashTableSpace(const Space* space = 0);
		~HashTableSpace();

		void setLevels(int min_level,int max_level);
		int getMinimumLevel();
		int getMaximumLevel();
	};

	class _OgreOdeExport QuadTreeSpace:public Space
	{
	public:
		QuadTreeSpace(const Vector3& center,const Vector3& extents,int depth,const Space* space = 0);
		~QuadTreeSpace();
	};
}

#endif

