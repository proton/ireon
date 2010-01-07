#include "OgreOdeMaintainedList.h"

using namespace OgreOde;

MaintainedItem::MaintainedItem()
{
}

MaintainedItem::~MaintainedItem()
{
}

void MaintainedItem::setDebug(bool debug)
{
}

void MaintainedItem::sync()
{
}

void MaintainedItem::notify(Body* body)
{
}

MaintainedList::MaintainedList()
{
}

MaintainedList::~MaintainedList()
{
}

void MaintainedList::registerItem(MaintainedItem* ptr)
{
	assert(ptr->getID());
	_map[ptr->getID()] = ptr;
}

MaintainedItem* MaintainedList::findItem(unsigned long id)
{
	std::map<unsigned long,MaintainedItem*>::iterator i = _map.find(id);
	if(i == _map.end()) return 0;

	return i->second;
}

void MaintainedList::unregisterItem(unsigned long id)
{
	std::map<unsigned long,MaintainedItem*>::iterator i = _map.find(id);
	if(i != _map.end()) _map.erase(i);	
}

void MaintainedList::setDebug(bool debug)
{
	std::map<unsigned long,MaintainedItem*>::iterator i = _map.begin();
	std::map<unsigned long,MaintainedItem*>::iterator end = _map.end();
	for(;i != end;++i)
	{
		i->second->setDebug(debug);
	}
}

void MaintainedList::sync()
{
	std::map<unsigned long,MaintainedItem*>::iterator i = _map.begin();
	std::map<unsigned long,MaintainedItem*>::iterator end = _map.end();
	for(;i != end;++i)
	{
		i->second->sync();
	}
}

void MaintainedList::notify(Body* body)
{
	std::map<unsigned long,MaintainedItem*>::iterator i = _map.begin();
	std::map<unsigned long,MaintainedItem*>::iterator end = _map.end();
	for(;i != end;++i)
	{
		i->second->notify(body);
	}
}
