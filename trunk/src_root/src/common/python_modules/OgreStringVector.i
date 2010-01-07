%{
#include "OgreStringVector.h"
%}

%include OgreStringVector.h

// Instantiate the two templates typedef'ed in OgreStringVector.h
%template(StringVector) std::vector<Ogre::String>;
%template(_StringVectorPtr) Ogre::SharedPtr<std::vector<Ogre::String> >;
