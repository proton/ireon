%{
#include "OgreVector4.h"

#define new_Ogre_Vector4 new Ogre::Vector4
%}

#ifndef PYOGRE_ALLOW_WARNINGS
%warnfilter(503) Ogre::Vector4;
#endif

%ignore Ogre::Vector4::operator=;
%ignore Ogre::Vector4::operator[];
%extend Ogre::Vector4
{
    PyObject *__nonzero__(void)
    {
        Ogre::Real x = self->x, y = self->y, z = self->z, w = self->w;
        Ogre::Real sqlen = (x * x) + (y * y) + (z * z) + (w * w);

        PyObject *toReturn = (sqlen < (1e-06 * 1e-06)) ? Py_False : Py_True;
        Py_INCREF(toReturn);
        return toReturn;
    }
    
    PyObject *__str__(void)
    {
        char buffer[128];
        PyOS_snprintf(buffer, 127, "Vector4(%f, %f, %f, %f)",
                self->x, self->y, self->z, self->w);
        buffer[127] = '\0';
        return PyString_FromString(buffer);
    }
    
    PyObject* __getitem__(size_t i)
    {
        if (i < 0 || i > 3)
        {
            PyErr_SetString(PyExc_IndexError, "vector index out of range");
            return NULL;
        }
        return PyFloat_FromDouble((double)self->operator[](i));
    }

	%typemap(out) void "$result = obj0; Py_INCREF(obj0);";
	void __iadd__(Ogre::Vector4 &v) { (*self) += v; }
	void __isub__(Ogre::Vector4 &v) { (*self) -= v; }
	void __imul__(Ogre::Vector4 &v) { (*self) *= v; }
	void __idiv__(Ogre::Vector4 &v) { (*self) /= v; }
	void __imul__(Ogre::Real f) { (*self) *= f; }
	void __idiv__(Ogre::Real f) { (*self) /= f; }
	%typemap(out) void "Py_INCREF(Py_None); $result = Py_None;";
	
    Vector4();
    Vector4(Real fX, Real fY, Real fZ, Real fW);
    Vector4(const Vector4& rkVector);
};
// ignore the default constructor, we'll use our owns defined above
%ignore Ogre::Vector4::Vector4;

// ignore some operators to allow typemaps on them (see %extend above)
%ignore Ogre::Vector4::operator+=;
%ignore Ogre::Vector4::operator-=;
%ignore Ogre::Vector4::operator*=;
%ignore Ogre::Vector4::operator/=;

%ogre_member(Ogre::Vector4, Ogre::Real, x, x);
%ogre_member(Ogre::Vector4, Ogre::Real, y, y);
%ogre_member(Ogre::Vector4, Ogre::Real, z, z);
%ogre_member(Ogre::Vector4, Ogre::Real, w, w);

%include OgreVector4.h

// static class members are exported into their original class here
%shadow 
%{
    Vector4.ZERO = Vector4(0, 0, 0, 0)
    Vector4.UNIT_X = Vector4(1, 0, 0, 0)
    Vector4.UNIT_Y = Vector4(0, 1, 0, 0)
    Vector4.UNIT_Z = Vector4(0, 0, 1, 0)
    Vector4.UNIT_W = Vector4(0, 0, 0, 1)
    Vector4.NEGATIVE_UNIT_X = Vector4(-1, 0, 0, 0)
    Vector4.NEGATIVE_UNIT_Y = Vector4(0, -1, 0, 0)
    Vector4.NEGATIVE_UNIT_Z = Vector4(0, 0, -1, 0)
    Vector4.NEGATIVE_UNIT_W = Vector4(0, 0, 0, -1)
    Vector4.UNIT_SCALE = Vector4(1, 1, 1, 1)
%}
