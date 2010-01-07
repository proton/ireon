%{
#include "OgreMatrix4.h"
%}

#ifndef PYOGRE_ALLOW_WARNINGS
%warnfilter(503) Ogre::Matrix4;
#endif

%ignore Ogre::Matrix4::operator=;

// slicing a matrix automatically return a temporary vector: this is a
// little bit of overhead but very Pythonic. Geez :)
%ignore Ogre::Matrix4::operator[];
%newobject Ogre::Matrix4::__getitem__;
%extend Ogre::Matrix4 {

    inline Ogre::Vector4* __getitem__(size_t i) {
        return new Ogre::Vector4(self->operator[](i));
    }
};

%include OgreMatrix4.h

%shadow
%{
    Matrix4.ZERO = Matrix4(
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0)

    Matrix4.IDENTITY = Matrix4(
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1)

    Matrix4.CLIPSPACE2DTOIMAGESPACE = Matrix4(
        0.5,    0,  0, 0.5, 
        0,   -0.5,  0, 0.5, 
        0,      0,  1,   0,
        0,      0,  0,   1)
%}
