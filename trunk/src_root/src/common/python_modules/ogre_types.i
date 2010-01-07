%module OgreTypes
%{
#include "OgrePrerequisites.h" 
#include "OgreColourValue.h"
#include "OgreIteratorWrappers.h"
#include "OgreString.h" 
using namespace Ogre; 
%}
// else String be unknown type 

typedef std::string String;
%feature("autodoc", "1"); 
//%include OgreCommon.i 
// std library typemaps and ogre templates
%include std_string.i
%include std_vector.i
%include std_pair.i
%include std_map.i
%include std_multimap.i

%include ogre_attributes.i
%include ogre_typemaps.i 

// remove some declarations from ogre headers
#define _OgreExport
#define OGRE_AUTO_SHARED_MUTEX 

// we know that "Nested union not currently supported (ignored)."
#pragma SWIG nowarn=312 
// we *include* OgrePlatform.h and OgrePrerequisites.h to have some #defined
// values available as constants in pyogre.ogre
%include OgrePlatform.h
%include OgrePrerequisites.h 

// Python implementation of SharedPtr as a super-overloaded function
%pythoncode
%{
_SharedPtrRegistry = {}

def SharedPtr(obj, sp=None):
    if sp is None:
        objtype = type(obj)
        try:
            sp = _SharedPtrRegistry[objtype]
        except:
            for objinfo, spinfo in _SharedPtrRegistry.items():
                if issubclass(objtype, objinfo):
                    sp = spinfo ; break
        if sp is None:
            raise RuntimeError(
                "missing SharedPtr implementation for type '%s'" %
                obj.__class__.__name__)
    obj_sp = sp(obj)
    obj.__disown__()
    return obj_sp

def SharedPtrRegister(objinfo, spinfo):
    _SharedPtrRegistry[objinfo] = spinfo
%} 

// templates and macro definitions
//%include OgreIteratorWrappers.i
//%include OgreSingleton.i
%include OgreSharedPtr.i
//%include OgreController.i 



%include OgreCommon.i
 
// geometrical stuff and math
%include OgreVector2.i
%include OgreVector3.i
%include OgreVector4.i
%include OgreMatrix3.i
%include OgreMatrix4.i
%include OgreQuaternion.i 

// miscellaneous ogre classes that better go before others
%include OgreString.i
%include OgreStringVector.i
%include OgreStringConverter.i
 
//%include "common/types.h"
