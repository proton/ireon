/* Vector2 in/typecheck typemap: accept a 2-tuple as it was a Vector2 */

%typemap(in) Ogre::Vector2 & (Ogre::Vector2 vec) {
    if (PyTuple_Check($input)) {
        if (!PyArg_ParseTuple($input, "ff:$symname",
                              &(vec.x), &(vec.y))) {
            return NULL;
        }
        $1 = &vec;
    }
    else {
        if ((SWIG_ConvertPtr($input, (void **)&$1, $1_descriptor, 0)) == -1)
            return NULL;
    }
}

%typecheck(SWIG_TYPECHECK_POINTER) Ogre::Vector2 & {
    void *ptr;
    if (!PyTuple_Check($input)
        && SWIG_ConvertPtr($input, &ptr, $1_descriptor, 0) == -1) {
        $1 = 0;
        PyErr_Clear();
    } else {
        $1 = 1;
    }
}


/* Vector3 in/typecheck typemap: accept a 3-tuple as it was a Vector3 */

%typemap(in) Ogre::Vector3 & (Ogre::Vector3 vec) {
    if (PyTuple_Check($input)) {
        if (!PyArg_ParseTuple($input, "fff:$symname",
                              &(vec.x), &(vec.y), &(vec.z))) {
            return NULL;
        }
        $1 = &vec;
    }
    else {
        if ((SWIG_ConvertPtr($input, (void **)&$1, $1_descriptor, 0)) == -1)
            return NULL;
    }
}

%typecheck(SWIG_TYPECHECK_POINTER) Ogre::Vector3 & {
    void *ptr;
    if (!PyTuple_Check($input)
        && SWIG_ConvertPtr($input, &ptr, $1_descriptor, 0) == -1) {
        $1 = 0;
        PyErr_Clear();
    } else {
        $1 = 1;
    }
}

%typemap(in) Ogre::Vector3 *VECTOR3PTRIN (Ogre::Vector3 vec) {
    if (PyTuple_Check($input)) {
        if (!PyArg_ParseTuple($input, "fff:$symname",
                              &(vec.x), &(vec.y), &(vec.z))) {
            return NULL;
        }
        $1 = &vec;
    }
    else {
        if ((SWIG_ConvertPtr($input, (void **)&$1, $1_descriptor, 0)) == -1)
            return NULL;
    }
}

%typecheck(SWIG_TYPECHECK_POINTER) Ogre::Vector3 *VECTOR3PTRIN {
    void *ptr;
    if (!PyTuple_Check($input)
        && SWIG_ConvertPtr($input, &ptr, $1_descriptor, 0) == -1) {
        $1 = 0;
        PyErr_Clear();
    } else {
        $1 = 1;
    }
}

/* Vector4 in/typecheck typemap: accept a 4-tuple as it was a Vector4 */

%typemap(in) Ogre::Vector4 & (Ogre::Vector4 vec) {
    if (PyTuple_Check($input)) {
        if (!PyArg_ParseTuple($input, "ffff:$symname",
                              &(vec.x), &(vec.y), &(vec.z), &(vec.w))) {
            return NULL;
        }
        $1 = &vec;
    }
    else {
        if ((SWIG_ConvertPtr($input, (void **)&$1, $1_descriptor, 0)) == -1)
            return NULL;
    }
}

%typecheck(SWIG_TYPECHECK_POINTER) Ogre::Vector4 & {
    void *ptr;
    if (!PyTuple_Check($input)
        && SWIG_ConvertPtr($input, &ptr, $1_descriptor, 0) == -1) {
        $1 = 0;
        PyErr_Clear();
    } else {
        $1 = 1;
    }
}

/* ColourValue in/typecheck typemap: accept a 3,4-tuple as it was a colour */

%typemap(in) Ogre::ColourValue & (Ogre::ColourValue col) {
    if (PyTuple_Check($input)) {
        col.a = 1.0;
        if (!PyArg_ParseTuple($input, "fff|f:$symname",
                              &(col.r), &(col.g), &(col.b), &(col.a))) {
            return NULL;
        }
        $1 = &col;
    }
    else {
        if ((SWIG_ConvertPtr($input, (void **)&$1, $1_descriptor, 0)) == -1)
            return NULL;
    }
}

%typecheck(SWIG_TYPECHECK_POINTER) Ogre::ColourValue & {
    void *ptr;
    if (!PyTuple_Check($input)
        && SWIG_ConvertPtr($input, &ptr, $1_descriptor, 0) == -1) {
        $1 = 0;
        PyErr_Clear();
    } else {
        $1 = 1;
    }
}

/* Force Radian to accept a Degree instead. */
%typecheck(SWIG_TYPECHECK_POINTER) Ogre::Radian &
{
    void *ptr;
    if ((SWIG_ConvertPtr($input, &ptr, $1_descriptor, 0) == -1) &&
        (SWIG_ConvertPtr($input, &ptr, SWIGTYPE_p_Ogre__Degree, 0) == -1))
    {
        $1 = 0;
    }
    else
    {
        $1 = 1;
    }
}

%typemap(in) Ogre::Radian & (Ogre::Degree *dTmp, Ogre::Radian rTmp)
{
    if ((SWIG_ConvertPtr($input, (void **)&dTmp,
                         SWIGTYPE_p_Ogre__Degree, 0)) != -1)
    {
        rTmp = *dTmp;
        $1 = &rTmp;
    }
    else if ((SWIG_ConvertPtr($input, (void **)&$1, 
                              $1_descriptor, 0)) == -1)
    {
        return NULL;
    }
}


/* DataStreamPtr in typemap: accept a Python string as it was a DataStreamPtr */
%typemap(in) Ogre::DataStreamPtr & (SwigValueWrapper<Ogre::DataStreamPtr> dsp)
{
    if (!PyString_Check($input))
    {
	PyErr_SetString(PyExc_TypeError, 
	                "You must use a string for DataStream objects.");
	return NULL;
    }
    
    int len;
    char *buff;
    
    PyString_AsStringAndSize($input, &buff, &len);
    char *buffcopy = new char[len];  // This is cleaned up by MemoryDataStream
    memcpy(buffcopy, buff, len);
    
    dsp = Ogre::DataStreamPtr(new Ogre::MemoryDataStream(buffcopy, len, true));
    $1 = &dsp;
}

%pythoncode
%{
def isFloatTuple(args):
    for x in args:
        t = type(x)
        if not t == int and not t == long and not t == float:
            return False
    return True

def fix_args(funct):
    def _inner(self, args):
        if type(args) == tuple:
            if isFloatTuple(args):
                args = (self, args)
            else:
                args = (self,) + args
        else:
            args = (self, args)
        return funct(*args)
    return _inner
%}
