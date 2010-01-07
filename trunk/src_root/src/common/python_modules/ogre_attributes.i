/* Macros used to generate the right set of accessor methods to transform
 * getters/setters into Python attributes.
 *
 * Class - the class name
 * type - the type of the variable
 * name - the name you wish to use in the python bindings
 * setmethod - the set method in Class
 * getmethod - the get method in Class
 */

/* USAGE: basic data types only and references (read-only or read-write) */

%define %ogre_attribute(Class, type, name, getmethod, ...)
#if #__VA_ARGS__ != ""
%feature("shadow") Ogre::Class::name##_set {
  name = property(_IreonClient.##Class##_##name##_get, _IreonClient.##Class##_##name##_set,
                  None, "Class.name -> type\n\nThis is equivalent to calling the C++ Ogre methods:\n\tget: type Class::getmethod()\n\tset: void Class::__VA_ARGS__(type)")};
%feature("shadow") Ogre::Class::name##_get {}
%extend Ogre::Class
{
    void name##_set(type _val) { self->__VA_ARGS__(_val); }
}
%ignore Ogre::Class::__VA_ARGS__;
#else
%feature("shadow") Ogre::Class::name##_get {
  name = property(_IreonClient.##Class##_##name##_get, None, None,
                  "Class.name -> type  (read-only)\n\nThis is equivalent to calling the C++ Ogre method:\n\tget: type Class::getmethod()")};
#endif
%extend Ogre::Class
{
    type name##_get() { return (type)(self->getmethod()); }
}
%ignore Ogre::Class::getmethod;
%enddef

%define %ogre_multi_attr(Class, name, getmethod, setmethod)

%feature("shadow") Ogre::Class::setmethod {
  name = property(_IreonClient.##Class##___##getmethod, fix_args(_IreonClient.##Class##___##setmethod), None,
                  "Class.name -> type\n\nThis is equivalent to calling the C++ Ogre methods:\n\tget: Class::getmethod()\n\tset: void Class::setmethod(...)")};
%feature("shadow") Ogre::Class::getmethod {}
%rename (__##setmethod) Ogre::Class::setmethod;
%rename (__##getmethod) Ogre::Class::getmethod;
%enddef

/* USAGE: read-only attribute that need to be returned as a new object */

%define %ogre_attribute_new(Class, type, name, getmethod, ...)
#if #__VA_ARGS__ != ""
%feature("shadow") Ogre::Class::name##_set {
  name = property(_IreonClient.##Class##_##name##_get, _IreonClient.##Class##_##name##_set, None,
                  "Class.name -> type\n\nThis is equivalent to calling the C++ Ogre methods:\n\tget: type Class::getmethod()\n\tset: void Class::__VA_ARGS__(type)")};
%feature("shadow") Ogre::Class::name##_get {}
%extend Ogre::Class
{
    void name##_set(type _val) { self->__VA_ARGS__(_val); }
}
%ignore Ogre::Class::__VA_ARGS__;
#else
%feature("shadow") Ogre::Class::name##_get {
  name = property(_IreonClient.##Class##_##name##_get, None, None, "Class.name -> type  (read-only)\n\nThis is equivalent to calling the C++ Ogre method:\n\tget: type Class::getmethod()")};
#endif
%newobject Ogre::Class::name;
%extend Ogre::Class
{
    type * name##_get() { return new type(self->getmethod()); }
}
%ignore Ogre::Class::getmethod;
%enddef

/* Macro used when SWIG does not properly wrap public variables because of
 * anonymous unions (for example ColourValue::r or Vector3::x).
 * 
 * Class - the class name
 * type - the type of the variable
 * name - the name you wish to use in the PYTHON bindings
 * attr - the attribute (member) in the C++ class
 */
%define %ogre_member(Class, type, name, attr)
%extend Class
{
    type name;
}
%{
    inline void SWIG_Mangle(Class) ##_## name ##_set(Class * _t, type _val)
    { _t->attr = _val; }
    inline type SWIG_Mangle(Class) ##_## name ##_get(const Class *_t)
    { return _t->attr; }
%}
%enddef
