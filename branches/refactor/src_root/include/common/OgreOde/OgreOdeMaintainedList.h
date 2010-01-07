#ifndef _OGREODEMAINTAINEDLIST_H_
#define _OGREODEMAINTAINEDLIST_H_

#include "OgreOdePreReqs.h"

namespace OgreOde
{
	class _OgreOdeExport MaintainedItem
	{
	public:
		MaintainedItem();
		virtual ~MaintainedItem();

		virtual unsigned long getID() = 0;
		virtual void setDebug(bool debug);
		virtual void sync();
		virtual void notify(Body* body);
	};

	class _OgreOdeExport MaintainedList
	{
		friend class Body;
		friend class World;

	public:
		MaintainedList();
		~MaintainedList();

		void registerItem(MaintainedItem* ptr);
		MaintainedItem* findItem(unsigned long id);
		void unregisterItem(unsigned long id);
		void setDebug(bool debug);
		void sync();
		void notify(Body* body);

	protected:
		std::map<unsigned long,MaintainedItem*> _map;
	};
}

#endif
