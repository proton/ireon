#ifndef _KEY_MAP_
#define _KEY_MAP_

#include "common/world/managers/command.h"
#include <map>
#pragma warning(push, 1)
#include "OIS/OIS.h"
#pragma warning(pop)
#include "boost/shared_ptr.hpp"
namespace OIS
{
	enum InputSystemCode
	{
		MC_UP = MB_Button7 + 100,
		MC_DOWN,
		MC_LEFT,
		MC_RIGHT,
		MOUSE_MOVE_END
	};
}

typedef boost::shared_ptr<CCommand> CommandPtr;


class ActionMap
{
public:
	typedef std::map< int ,CommandPtr> actionMapType;

	virtual ~ActionMap(){};

	virtual void setDefaultMap()=0;
	
	virtual void loadMap()=0;

	virtual void saveMap()=0;
	
	// TODO : refactor this: cast enum in int. 

	virtual void setAction(int key,CommandPtr action );

	ActionMap::actionMapType::iterator ActionMap::getAction(int key);


	actionMapType::iterator end(){ return actionMap_.end();}

private:
	actionMapType actionMap_;
};

class KeyboardActionMap: public ActionMap
{
public:
	void setDefaultMap();

	void loadMap();

	void saveMap();
 
};

class MouseActionMap: public ActionMap
{
public:

	void setDefaultMap();

	void loadMap();

	void saveMap();

};


#endif 