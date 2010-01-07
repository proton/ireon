%{
#include "OgreMatrix3.h"
%}

#ifndef PYOGRE_ALLOW_WARNINGS
%warnfilter(503) Ogre::Matrix3;
#endif

%ignore Ogre::Matrix3::operator=;
		
%ignore Ogre::Matrix3::operator[];
%newobject Ogre::Matrix3::__getitem__;
%extend Ogre::Matrix3 {
    // slicing a matrix automatically return a temporary vector: this is a
    // little bit of overhead but very Pythonic. Gheez :)
    inline Ogre::Vector3* __getitem__(size_t i) {
        return new Ogre::Vector3(self->operator[](i));
    }

    // Get and Set are necessary because we can't assign to [] results
    inline Ogre::Real Get(size_t iRow, size_t iColumn) {
        return (self->operator[](iRow))[iColumn];
    }
    inline void Set(size_t iRow, size_t iColumn, Ogre::Real value) {
        (self->operator[](iRow))[iColumn] = value;
    }

    // in Python we provide the symmetric of getColum/setColum
    inline const Vector3* GetRow(size_t iRow) {
        return new Ogre::Vector3(self->operator[](iRow));
    }
    inline void SetRow(size_t iRow, const Vector3 &vec) {
        *(self->operator[](iRow))   = vec.x;
        *(self->operator[](iRow)+1) = vec.y;
        *(self->operator[](iRow)+2) = vec.z;
    }
};

%include OgreMatrix3.h

%shadow
%{
    Matrix3.EPSILON = 1e-06
    Matrix3.ZERO = Matrix3(0, 0, 0, 0, 0, 0, 0, 0, 0);
    Matrix3.IDENTITY = Matrix3(1, 0, 0, 0, 1, 0, 0, 0, 1);
%}
