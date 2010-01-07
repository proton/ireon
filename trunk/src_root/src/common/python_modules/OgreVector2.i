%{
#include "OgreVector2.h"

#define new_Ogre_Vector2 new Ogre::Vector2
%}

#ifndef PYOGRE_ALLOW_WARNINGS
%warnfilter(503) Ogre::Vector2;
#endif

%ogre_attribute(Vector2, Ogre::Real, length, length);
%ogre_attribute(Vector2, Ogre::Real, squaredLength, squaredLength);

%ignore Ogre::Vector2::isZeroLength;
%ignore Ogre::Vector2::operator=;
%ignore Ogre::Vector2::operator[];
%extend Ogre::Vector2
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
        PyOS_snprintf(buffer, 127, "Vector2(%f, %f)",
                self->x, self->y);
        buffer[127] = '\0';
        return PyString_FromString(buffer);
    }
    
    PyObject* __getitem__(size_t i)
    {
        if (i < 0 || i > 1)
        {
            PyErr_SetString(PyExc_IndexError, "vector index out of range");
            return NULL;
        }
        return PyFloat_FromDouble((double)self->operator[](i));
    }
    
	%typemap(out) void "$result = obj0; Py_INCREF(obj0);";
	void __iadd__(Ogre::Vector2 &v) { (*self) += v; }
	void __isub__(Ogre::Vector2 &v) { (*self) -= v; }
	void __imul__(Ogre::Real f) { (*self) *= f; }
	void __idiv__(Ogre::Real f) { (*self) /= f; }
	%typemap(out) void "Py_INCREF(Py_None); $result = Py_None;";
    
    Vector2();
    Vector2(Real fX, Real fY);
    Vector2(const Vector2& rkVector);
};

// ignore the default constructor, we'll use our owns defined above
%ignore Ogre::Vector2::Vector2;

// ignore some operators to allow typemaps on them (see %extend above)
%ignore Ogre::Vector2::operator+=;
%ignore Ogre::Vector2::operator-=;
%ignore Ogre::Vector2::operator*=;
%ignore Ogre::Vector2::operator/=;

%ogre_member(Ogre::Vector2, Ogre::Real, x, x);
%ogre_member(Ogre::Vector2, Ogre::Real, y, y);

// a list of immutable static (class) members
%immutable Ogre::Vector2::ZERO;
%immutable Ogre::Vector2::UNIT_X;
%immutable Ogre::Vector2::UNIT_Y;
%immutable Ogre::Vector2::NEGATIVE_UNIT_X;
%immutable Ogre::Vector2::NEGATIVE_UNIT_Y;
%immutable Ogre::Vector2::UNIT_SCALE;

%include OgreVector2.h

// static class members are exported into their original class here
%shadow 
%{
    Vector2.ZERO = Vector2(0, 0)
    Vector2.UNIT_X = Vector2(1, 0)
    Vector2.UNIT_Y = Vector2(0, 1)
    Vector2.NEGATIVE_UNIT_X = Vector2(-1, 0)
    Vector2.NEGATIVE_UNIT_Y = Vector2(0, -1)
    Vector2.UNIT_SCALE = Vector2(1, 1)
%}
