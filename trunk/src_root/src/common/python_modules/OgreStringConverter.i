%{
#include "OgreStringConverter.h"
%}

// We ignore these because these are not needed
%ignore Ogre::StringConverter::toString(size_t);
%ignore Ogre::StringConverter::toString(size_t,unsigned short);
%ignore Ogre::StringConverter::toString(size_t,unsigned short,char);
%ignore Ogre::StringConverter::toString(size_t,unsigned short,char,std::ios::fmtflags);

%include OgreStringConverter.h
