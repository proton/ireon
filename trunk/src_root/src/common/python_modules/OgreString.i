namespace Ogre
{
    class String
    {
        String();
        String(const String &);
        
        virtual ~String();
    };
}

%{
    inline PyObject * const Ogre_String_pyString_get(Ogre::String *self)
    { return PyString_FromString(self->c_str()); }
%}


%extend Ogre::String
{
    PyObject * const pyString;
}

%extend Ogre::String
{
    PyObject *__str__()
    {return PyString_FromString(self->c_str());}
}
