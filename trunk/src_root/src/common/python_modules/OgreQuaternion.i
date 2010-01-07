%{
#include "OgreQuaternion.h"
%}

#ifndef PYOGRE_ALLOW_WARNINGS
%warnfilter(503) Ogre::Quaternion;
#endif

%ignore Ogre::Quaternion::operator=;
%include OgreQuaternion.h

%shadow
%{
    Quaternion.ZERO = Quaternion(0, 0, 0, 0)
    Quaternion.IDENTITY = Quaternion(1, 0, 0, 0)
%}
