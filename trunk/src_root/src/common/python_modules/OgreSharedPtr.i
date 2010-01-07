%{
#include "OgreSharedPtr.h"
%}

/* We don't need all the fuss in OgreSharedPtr.h so we redefine a compatible
   class here; please note that binding pointers from Python (bind() method)
   is currently _not_ supported! */

namespace Ogre {
    template<class T> class SharedPtr {
     public:
        SharedPtr();
        SharedPtr(T* rep);
        SharedPtr(const SharedPtr& r);
        virtual ~SharedPtr(); 
        T* get() const;
    };
}

/* The macro here will ignore the real shared pointer (SWIG smart pointer)
   stuff and simply provide a little proxy magic to the _XXXSharedPtr class,
   generated from the instancing of SharedPtr<XXX>. This makes possible to
   access all the attribute and methods of XXX using the proxy class, just
   like real shared pointers do. 

   Class - the class to be wrapped in the SharedPtr
   ClassPtr - the class that uses SharedPtr<Class> as base class (optional)
*/

%define %ogre_shared_ptr(Class, ...)
#if #__VA_ARGS__ != ""
%template(_##Class##Pointer) Ogre::SharedPtr<Ogre::Class>;
%rename(Class##Pointer) Ogre::__VA_ARGS__;
%extend Ogre::__VA_ARGS__
#else
%template(Class##Pointer) Ogre::SharedPtr<Ogre::Class>;
%extend Ogre::SharedPtr<Ogre::Class>
#endif
{
    %pythoncode %{
    def __getattr__(self, name):
        try:
            return self.__dict__[name]
        except KeyError:
            return getattr(self.get(), name)
    %}

    PyObject *__nonzero__(void)
    {
        PyObject *toReturn = (self->isNull()) ? Py_False : Py_True;
        Py_INCREF(toReturn);
        return toReturn;
    }
 
    const bool unique;
    const unsigned int useCount;
}
#if #__VA_ARGS__ != ""
%{
    inline bool
    SWIG_Mangle(Ogre::__VA_ARGS__)##_unique_get(Ogre::__VA_ARGS__ *_t) {
        return _t->unique();
    }
    inline unsigned int
    SWIG_Mangle(Ogre::__VA_ARGS__)##_useCount_get(Ogre::__VA_ARGS__ *_t) {
        return _t->useCount();
    }
%}
#else
%{
    inline bool
    SWIG_Mangle(Ogre::SharedPtr<Ogre::Class>)##_unique_get(
             Ogre::SharedPtr<Ogre::Class> *_t) {
        return _t->unique();
    }
    inline unsigned int
    SWIG_Mangle(Ogre::SharedPtr<Ogre::Class>)##_useCount_get(
             Ogre::SharedPtr<Ogre::Class> *_t) {
        return _t->useCount();
    }
%}
#endif
%enddef

/* SharedPtr subclasses or instantiated templates need to be registered to
   allow the Python programmer to use the nice SharedPtr() function.
*/
%define %ogre_shared_ptr_registry(Class, ...)
#if #__VA_ARGS__ != ""
%pythoncode %{
_SharedPtrRegistry[Class] = __VA_ARGS__
%}
#else
%pythoncode %{
_SharedPtrRegistry[Class] = Class##Pointer
%}
#endif
%enddef
