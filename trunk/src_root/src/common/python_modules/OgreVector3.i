%{
#include "OgreVector3.h"

#define new_Ogre_Vector3 new Ogre::Vector3
%}

#ifndef PYOGRE_ALLOW_WARNINGS
%warnfilter(503) Ogre::Vector3;
#endif

%ogre_attribute(Vector3, Ogre::Real, length, length)
%ogre_attribute(Vector3, Ogre::Real, squaredLength, squaredLength)

%ignore Ogre::Vector3::isZeroLength;
%ignore Ogre::Vector3::operator=;
%ignore Ogre::Vector3::operator[];
%extend Ogre::Vector3
{
    PyObject *__nonzero__(void)
    {
        PyObject *toReturn = (self->isZeroLength()) ? Py_False : Py_True;
        Py_INCREF(toReturn);
        return toReturn;
    }
    
    PyObject *__str__(void)
    {
        char buffer[128];
        PyOS_snprintf(buffer, 127, "Vector3(%f, %f, %f)",
                self->x, self->y, self->z);
        buffer[127] = '\0';
        return PyString_FromString(buffer);
    }
    
    PyObject *__getitem__(size_t i)
    {
        if (i < 0 || i > 2)
        {
            PyErr_SetString(PyExc_IndexError, "vector index out of range");
            return NULL;
        }
        return PyFloat_FromDouble((double)self->operator[](i));
    }

	%typemap(out) void "$result = obj0; Py_INCREF(obj0);";
	void __iadd__(Ogre::Vector3 &v) { (*self) += v; }
	void __isub__(Ogre::Vector3 &v) { (*self) -= v; }
	void __imul__(Ogre::Vector3 &v) { (*self) *= v; }
	void __idiv__(Ogre::Vector3 &v) { (*self) /= v; }
	void __imul__(Ogre::Real f) { (*self) *= f; }
	void __idiv__(Ogre::Real f) { (*self) /= f; }
	%typemap(out) void "Py_INCREF(Py_None); $result = Py_None;";

    Vector3();
    Vector3(Real fX, Real fY, Real fZ);
    Vector3(const Vector3& rkVector);
    
}

// ignore the default constructor, we'll use our owns defined above
%ignore Ogre::Vector3::Vector3;

// ignore some operators to allow typemaps on them (see %extend above)
%ignore Ogre::Vector3::operator+=;
%ignore Ogre::Vector3::operator-=;
%ignore Ogre::Vector3::operator*=;
%ignore Ogre::Vector3::operator/=;

%ogre_member(Ogre::Vector3, Ogre::Real, x, x);
%ogre_member(Ogre::Vector3, Ogre::Real, y, y);
%ogre_member(Ogre::Vector3, Ogre::Real, z, z);

%include OgreVector3.h

// static class members are exported into their original class here
%shadow 
%{
    Vector3.ZERO = Vector3(0, 0, 0)
    Vector3.UNIT_X = Vector3(1, 0, 0)
    Vector3.UNIT_Y = Vector3(0, 1, 0)
    Vector3.UNIT_Z = Vector3(0, 0, 1)
    Vector3.NEGATIVE_UNIT_X = Vector3(-1, 0, 0)
    Vector3.NEGATIVE_UNIT_Y = Vector3(0, -1, 0)
    Vector3.NEGATIVE_UNIT_Z = Vector3(0, 0, -1)
    Vector3.UNIT_SCALE = Vector3(1, 1, 1)
%}
