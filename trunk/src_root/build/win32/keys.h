
#include "Ogre/main/OgreInput.h" 
#include <map>
#include "boost/shared_ptr.hpp"
typedef boost::shared_ptr<CCommand> CommandPtr;


class CKeys
{
	std::map<Ogre::KeyCode,CommandPtr> actionMap;

	public:

		void load () {} //load map
   
		void save() {} //save map
  
		void change (Ogre::KeyCode key,CommandPtr action ) //change map
  {
   actionMap.insert(key);
   actionMap[key]= action;
  }
  
};
