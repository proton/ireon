# This file was created automatically by SWIG 1.3.29.
# Don't modify this file, modify the SWIG interface instead.
# This file is compatible with both classic and new-style classes.

import _IreonClient
import new
new_instancemethod = new.instancemethod
def _swig_setattr_nondynamic(self,class_type,name,value,static=1):
    if (name == "thisown"): return self.this.own(value)
    if (name == "this"):
        if type(value).__name__ == 'PySwigObject':
            self.__dict__[name] = value
            return
    method = class_type.__swig_setmethods__.get(name,None)
    if method: return method(self,value)
    if (not static) or hasattr(self,name):
        self.__dict__[name] = value
    else:
        raise AttributeError("You cannot add attributes to %s" % self)

def _swig_setattr(self,class_type,name,value):
    return _swig_setattr_nondynamic(self,class_type,name,value,0)

def _swig_getattr(self,class_type,name):
    if (name == "thisown"): return self.this.own()
    method = class_type.__swig_getmethods__.get(name,None)
    if method: return method(self)
    raise AttributeError,name

def _swig_repr(self):
    try: strthis = "proxy of " + self.this.__repr__()
    except: strthis = ""
    return "<%s.%s; %s >" % (self.__class__.__module__, self.__class__.__name__, strthis,)

import types
try:
    _object = types.ObjectType
    _newclass = 1
except AttributeError:
    class _object : pass
    _newclass = 0
del types


class PySwigIterator(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, PySwigIterator, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, PySwigIterator, name)
    def __init__(self): raise AttributeError, "No constructor defined"
    __repr__ = _swig_repr
    __swig_destroy__ = _IreonClient.delete_PySwigIterator
    __del__ = lambda self : None;
    def value(*args): return _IreonClient.PySwigIterator_value(*args)
    def incr(*args): return _IreonClient.PySwigIterator_incr(*args)
    def decr(*args): return _IreonClient.PySwigIterator_decr(*args)
    def distance(*args): return _IreonClient.PySwigIterator_distance(*args)
    def equal(*args): return _IreonClient.PySwigIterator_equal(*args)
    def copy(*args): return _IreonClient.PySwigIterator_copy(*args)
    def next(*args): return _IreonClient.PySwigIterator_next(*args)
    def previous(*args): return _IreonClient.PySwigIterator_previous(*args)
    def advance(*args): return _IreonClient.PySwigIterator_advance(*args)
    def __eq__(*args): return _IreonClient.PySwigIterator___eq__(*args)
    def __ne__(*args): return _IreonClient.PySwigIterator___ne__(*args)
    def __iadd__(*args): return _IreonClient.PySwigIterator___iadd__(*args)
    def __isub__(*args): return _IreonClient.PySwigIterator___isub__(*args)
    def __add__(*args): return _IreonClient.PySwigIterator___add__(*args)
    def __sub__(*args): return _IreonClient.PySwigIterator___sub__(*args)
    def __iter__(self): return self
PySwigIterator_swigregister = _IreonClient.PySwigIterator_swigregister
PySwigIterator_swigregister(PySwigIterator)

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

OGRE_PLATFORM_WIN32 = _IreonClient.OGRE_PLATFORM_WIN32
OGRE_PLATFORM_LINUX = _IreonClient.OGRE_PLATFORM_LINUX
OGRE_PLATFORM_APPLE = _IreonClient.OGRE_PLATFORM_APPLE
OGRE_COMPILER_MSVC = _IreonClient.OGRE_COMPILER_MSVC
OGRE_COMPILER_GNUC = _IreonClient.OGRE_COMPILER_GNUC
OGRE_COMPILER_BORL = _IreonClient.OGRE_COMPILER_BORL
OGRE_ENDIAN_LITTLE = _IreonClient.OGRE_ENDIAN_LITTLE
OGRE_ENDIAN_BIG = _IreonClient.OGRE_ENDIAN_BIG
OGRE_ARCHITECTURE_32 = _IreonClient.OGRE_ARCHITECTURE_32
OGRE_ARCHITECTURE_64 = _IreonClient.OGRE_ARCHITECTURE_64
OGRE_PLATFORM = _IreonClient.OGRE_PLATFORM
OGRE_ARCH_TYPE = _IreonClient.OGRE_ARCH_TYPE
OGRE_DEBUG_MODE = _IreonClient.OGRE_DEBUG_MODE
OGRE_PLATFORM_LIB = _IreonClient.OGRE_PLATFORM_LIB
OGRE_ENDIAN = _IreonClient.OGRE_ENDIAN
OGRE_VERSION_MAJOR = _IreonClient.OGRE_VERSION_MAJOR
OGRE_VERSION_MINOR = _IreonClient.OGRE_VERSION_MINOR
OGRE_VERSION_PATCH = _IreonClient.OGRE_VERSION_PATCH
OGRE_VERSION_NAME = _IreonClient.OGRE_VERSION_NAME
OGRE_VERSION = _IreonClient.OGRE_VERSION
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

CMPF_ALWAYS_FAIL = _IreonClient.CMPF_ALWAYS_FAIL
CMPF_ALWAYS_PASS = _IreonClient.CMPF_ALWAYS_PASS
CMPF_LESS = _IreonClient.CMPF_LESS
CMPF_LESS_EQUAL = _IreonClient.CMPF_LESS_EQUAL
CMPF_EQUAL = _IreonClient.CMPF_EQUAL
CMPF_NOT_EQUAL = _IreonClient.CMPF_NOT_EQUAL
CMPF_GREATER_EQUAL = _IreonClient.CMPF_GREATER_EQUAL
CMPF_GREATER = _IreonClient.CMPF_GREATER
TFO_NONE = _IreonClient.TFO_NONE
TFO_BILINEAR = _IreonClient.TFO_BILINEAR
TFO_TRILINEAR = _IreonClient.TFO_TRILINEAR
TFO_ANISOTROPIC = _IreonClient.TFO_ANISOTROPIC
FT_MIN = _IreonClient.FT_MIN
FT_MAG = _IreonClient.FT_MAG
FT_MIP = _IreonClient.FT_MIP
FO_NONE = _IreonClient.FO_NONE
FO_POINT = _IreonClient.FO_POINT
FO_LINEAR = _IreonClient.FO_LINEAR
FO_ANISOTROPIC = _IreonClient.FO_ANISOTROPIC
SO_FLAT = _IreonClient.SO_FLAT
SO_GOURAUD = _IreonClient.SO_GOURAUD
SO_PHONG = _IreonClient.SO_PHONG
FOG_NONE = _IreonClient.FOG_NONE
FOG_EXP = _IreonClient.FOG_EXP
FOG_EXP2 = _IreonClient.FOG_EXP2
FOG_LINEAR = _IreonClient.FOG_LINEAR
CULL_NONE = _IreonClient.CULL_NONE
CULL_CLOCKWISE = _IreonClient.CULL_CLOCKWISE
CULL_ANTICLOCKWISE = _IreonClient.CULL_ANTICLOCKWISE
MANUAL_CULL_NONE = _IreonClient.MANUAL_CULL_NONE
MANUAL_CULL_BACK = _IreonClient.MANUAL_CULL_BACK
MANUAL_CULL_FRONT = _IreonClient.MANUAL_CULL_FRONT
WFT_SINE = _IreonClient.WFT_SINE
WFT_TRIANGLE = _IreonClient.WFT_TRIANGLE
WFT_SQUARE = _IreonClient.WFT_SQUARE
WFT_SAWTOOTH = _IreonClient.WFT_SAWTOOTH
WFT_INVERSE_SAWTOOTH = _IreonClient.WFT_INVERSE_SAWTOOTH
WFT_PWM = _IreonClient.WFT_PWM
PM_POINTS = _IreonClient.PM_POINTS
PM_WIREFRAME = _IreonClient.PM_WIREFRAME
PM_SOLID = _IreonClient.PM_SOLID
SHADOWTYPE_NONE = _IreonClient.SHADOWTYPE_NONE
SHADOWDETAILTYPE_ADDITIVE = _IreonClient.SHADOWDETAILTYPE_ADDITIVE
SHADOWDETAILTYPE_MODULATIVE = _IreonClient.SHADOWDETAILTYPE_MODULATIVE
SHADOWDETAILTYPE_STENCIL = _IreonClient.SHADOWDETAILTYPE_STENCIL
SHADOWDETAILTYPE_TEXTURE = _IreonClient.SHADOWDETAILTYPE_TEXTURE
SHADOWTYPE_STENCIL_MODULATIVE = _IreonClient.SHADOWTYPE_STENCIL_MODULATIVE
SHADOWTYPE_STENCIL_ADDITIVE = _IreonClient.SHADOWTYPE_STENCIL_ADDITIVE
SHADOWTYPE_TEXTURE_MODULATIVE = _IreonClient.SHADOWTYPE_TEXTURE_MODULATIVE
SHADOWTYPE_TEXTURE_ADDITIVE = _IreonClient.SHADOWTYPE_TEXTURE_ADDITIVE
TVC_NONE = _IreonClient.TVC_NONE
TVC_AMBIENT = _IreonClient.TVC_AMBIENT
TVC_DIFFUSE = _IreonClient.TVC_DIFFUSE
TVC_SPECULAR = _IreonClient.TVC_SPECULAR
TVC_EMISSIVE = _IreonClient.TVC_EMISSIVE
SM_DIRECTION = _IreonClient.SM_DIRECTION
SM_DISTANCE = _IreonClient.SM_DISTANCE
FBT_COLOUR = _IreonClient.FBT_COLOUR
FBT_DEPTH = _IreonClient.FBT_DEPTH
FBT_STENCIL = _IreonClient.FBT_STENCIL
class Box(_object):
    """Proxy of C++ Box class"""
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, Box, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, Box, name)
    __repr__ = _swig_repr
    __swig_setmethods__["left"] = _IreonClient.Box_left_set
    __swig_getmethods__["left"] = _IreonClient.Box_left_get
    if _newclass:left = property(_IreonClient.Box_left_get, _IreonClient.Box_left_set)
    __swig_setmethods__["top"] = _IreonClient.Box_top_set
    __swig_getmethods__["top"] = _IreonClient.Box_top_get
    if _newclass:top = property(_IreonClient.Box_top_get, _IreonClient.Box_top_set)
    __swig_setmethods__["right"] = _IreonClient.Box_right_set
    __swig_getmethods__["right"] = _IreonClient.Box_right_get
    if _newclass:right = property(_IreonClient.Box_right_get, _IreonClient.Box_right_set)
    __swig_setmethods__["bottom"] = _IreonClient.Box_bottom_set
    __swig_getmethods__["bottom"] = _IreonClient.Box_bottom_get
    if _newclass:bottom = property(_IreonClient.Box_bottom_get, _IreonClient.Box_bottom_set)
    __swig_setmethods__["front"] = _IreonClient.Box_front_set
    __swig_getmethods__["front"] = _IreonClient.Box_front_get
    if _newclass:front = property(_IreonClient.Box_front_get, _IreonClient.Box_front_set)
    __swig_setmethods__["back"] = _IreonClient.Box_back_set
    __swig_getmethods__["back"] = _IreonClient.Box_back_get
    if _newclass:back = property(_IreonClient.Box_back_get, _IreonClient.Box_back_set)
    def __init__(self, *args): 
        """
        __init__(self) -> Box
        __init__(self, size_t l, size_t t, size_t r, size_t b) -> Box
        __init__(self, size_t l, size_t t, size_t ff, size_t r, size_t b, 
            size_t bb) -> Box
        """
        this = _IreonClient.new_Box(*args)
        try: self.this.append(this)
        except: self.this = this
    def contains(*args):
        """contains(self, Box def) -> bool"""
        return _IreonClient.Box_contains(*args)

    def getWidth(*args):
        """getWidth(self) -> size_t"""
        return _IreonClient.Box_getWidth(*args)

    def getHeight(*args):
        """getHeight(self) -> size_t"""
        return _IreonClient.Box_getHeight(*args)

    def getDepth(*args):
        """getDepth(self) -> size_t"""
        return _IreonClient.Box_getDepth(*args)

    __swig_destroy__ = _IreonClient.delete_Box
    __del__ = lambda self : None;
Box_swigregister = _IreonClient.Box_swigregister
Box_swigregister(Box)


def findCommandLineOpts(*args):
  """
    findCommandLineOpts(int numargs, char argv, UnaryOptionList unaryOptList, 
        BinaryOptionList binOptList) -> int
    """
  return _IreonClient.findCommandLineOpts(*args)
class Vector2(_object):
    """Proxy of C++ Vector2 class"""
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, Vector2, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, Vector2, name)
    __repr__ = _swig_repr
    def __eq__(*args):
        """__eq__(self, Vector2 rkVector) -> bool"""
        return _IreonClient.Vector2___eq__(*args)

    def __ne__(*args):
        """__ne__(self, Vector2 rkVector) -> bool"""
        return _IreonClient.Vector2___ne__(*args)

    def __add__(*args):
        """__add__(self, Vector2 rkVector) -> Vector2"""
        return _IreonClient.Vector2___add__(*args)

    def __sub__(*args):
        """__sub__(self, Vector2 rkVector) -> Vector2"""
        return _IreonClient.Vector2___sub__(*args)

    def __mul__(*args):
        """
        __mul__(self, Real fScalar) -> Vector2
        __mul__(self, Vector2 rhs) -> Vector2
        """
        return _IreonClient.Vector2___mul__(*args)

    def __div__(*args):
        """
        __div__(self, Real fScalar) -> Vector2
        __div__(self, Vector2 rhs) -> Vector2
        """
        return _IreonClient.Vector2___div__(*args)

    def __pos__(*args):
        """__pos__(self) -> Vector2"""
        return _IreonClient.Vector2___pos__(*args)

    def __neg__(*args):
        """__neg__(self) -> Vector2"""
        return _IreonClient.Vector2___neg__(*args)

    def dotProduct(*args):
        """dotProduct(self, Vector2 vec) -> Real"""
        return _IreonClient.Vector2_dotProduct(*args)

    def normalise(*args):
        """normalise(self) -> Real"""
        return _IreonClient.Vector2_normalise(*args)

    def midPoint(*args):
        """midPoint(self, Vector2 vec) -> Vector2"""
        return _IreonClient.Vector2_midPoint(*args)

    def __lt__(*args):
        """__lt__(self, Vector2 rhs) -> bool"""
        return _IreonClient.Vector2___lt__(*args)

    def __gt__(*args):
        """__gt__(self, Vector2 rhs) -> bool"""
        return _IreonClient.Vector2___gt__(*args)

    def makeFloor(*args):
        """makeFloor(self, Vector2 cmp)"""
        return _IreonClient.Vector2_makeFloor(*args)

    def makeCeil(*args):
        """makeCeil(self, Vector2 cmp)"""
        return _IreonClient.Vector2_makeCeil(*args)

    def perpendicular(*args):
        """perpendicular(self) -> Vector2"""
        return _IreonClient.Vector2_perpendicular(*args)

    def crossProduct(*args):
        """crossProduct(self, Vector2 rkVector) -> Real"""
        return _IreonClient.Vector2_crossProduct(*args)

    def randomDeviant(*args):
        """randomDeviant(self, Real angle) -> Vector2"""
        return _IreonClient.Vector2_randomDeviant(*args)

    def normalisedCopy(*args):
        """normalisedCopy(self) -> Vector2"""
        return _IreonClient.Vector2_normalisedCopy(*args)

    def reflect(*args):
        """reflect(self, Vector2 normal) -> Vector2"""
        return _IreonClient.Vector2_reflect(*args)

    length = property(_IreonClient.Vector2_length_get, None, None,
                    "Vector2.length -> Ogre::Real  (read-only)\n\nThis is equivalent to calling the C++ Ogre method:\n\tget: Ogre::Real Vector2::length()")

    squaredLength = property(_IreonClient.Vector2_squaredLength_get, None, None,
                    "Vector2.squaredLength -> Ogre::Real  (read-only)\n\nThis is equivalent to calling the C++ Ogre method:\n\tget: Ogre::Real Vector2::squaredLength()")

    def __nonzero__(*args):
        """__nonzero__(self, void ?) -> PyObject"""
        return _IreonClient.Vector2___nonzero__(*args)

    def __str__(*args):
        """__str__(self, void ?) -> PyObject"""
        return _IreonClient.Vector2___str__(*args)

    def __getitem__(*args):
        """__getitem__(self, size_t i) -> PyObject"""
        return _IreonClient.Vector2___getitem__(*args)

    def __iadd__(*args):
        """__iadd__(self, Vector2 v)"""
        return _IreonClient.Vector2___iadd__(*args)

    def __isub__(*args):
        """__isub__(self, Vector2 v)"""
        return _IreonClient.Vector2___isub__(*args)

    def __imul__(*args):
        """__imul__(self, Real f)"""
        return _IreonClient.Vector2___imul__(*args)

    def __idiv__(*args):
        """__idiv__(self, Real f)"""
        return _IreonClient.Vector2___idiv__(*args)

    def __init__(self, *args): 
        """
        __init__(self) -> Vector2
        __init__(self, Real fX, Real fY) -> Vector2
        __init__(self, Vector2 rkVector) -> Vector2
        """
        this = _IreonClient.new_Vector2(*args)
        try: self.this.append(this)
        except: self.this = this
    __swig_setmethods__["x"] = _IreonClient.Vector2_x_set
    __swig_getmethods__["x"] = _IreonClient.Vector2_x_get
    if _newclass:x = property(_IreonClient.Vector2_x_get, _IreonClient.Vector2_x_set)
    __swig_setmethods__["y"] = _IreonClient.Vector2_y_set
    __swig_getmethods__["y"] = _IreonClient.Vector2_y_get
    if _newclass:y = property(_IreonClient.Vector2_y_get, _IreonClient.Vector2_y_set)
    __swig_destroy__ = _IreonClient.delete_Vector2
    __del__ = lambda self : None;
Vector2_swigregister = _IreonClient.Vector2_swigregister
Vector2_swigregister(Vector2)
cvar = _IreonClient.cvar
Vector2.ZERO = _IreonClient.cvar.Vector2_ZERO
Vector2.UNIT_X = _IreonClient.cvar.Vector2_UNIT_X
Vector2.UNIT_Y = _IreonClient.cvar.Vector2_UNIT_Y
Vector2.NEGATIVE_UNIT_X = _IreonClient.cvar.Vector2_NEGATIVE_UNIT_X
Vector2.NEGATIVE_UNIT_Y = _IreonClient.cvar.Vector2_NEGATIVE_UNIT_Y
Vector2.UNIT_SCALE = _IreonClient.cvar.Vector2_UNIT_SCALE

Vector2.ZERO = Vector2(0, 0)
Vector2.UNIT_X = Vector2(1, 0)
Vector2.UNIT_Y = Vector2(0, 1)
Vector2.NEGATIVE_UNIT_X = Vector2(-1, 0)
Vector2.NEGATIVE_UNIT_Y = Vector2(0, -1)
Vector2.UNIT_SCALE = Vector2(1, 1)

class Vector3(_object):
    """Proxy of C++ Vector3 class"""
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, Vector3, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, Vector3, name)
    __repr__ = _swig_repr
    def __eq__(*args):
        """__eq__(self, Vector3 rkVector) -> bool"""
        return _IreonClient.Vector3___eq__(*args)

    def __ne__(*args):
        """__ne__(self, Vector3 rkVector) -> bool"""
        return _IreonClient.Vector3___ne__(*args)

    def __add__(*args):
        """__add__(self, Vector3 rkVector) -> Vector3"""
        return _IreonClient.Vector3___add__(*args)

    def __sub__(*args):
        """__sub__(self, Vector3 rkVector) -> Vector3"""
        return _IreonClient.Vector3___sub__(*args)

    def __mul__(*args):
        """
        __mul__(self, Real fScalar) -> Vector3
        __mul__(self, Vector3 rhs) -> Vector3
        """
        return _IreonClient.Vector3___mul__(*args)

    def __div__(*args):
        """
        __div__(self, Real fScalar) -> Vector3
        __div__(self, Vector3 rhs) -> Vector3
        """
        return _IreonClient.Vector3___div__(*args)

    def __pos__(*args):
        """__pos__(self) -> Vector3"""
        return _IreonClient.Vector3___pos__(*args)

    def __neg__(*args):
        """__neg__(self) -> Vector3"""
        return _IreonClient.Vector3___neg__(*args)

    def dotProduct(*args):
        """dotProduct(self, Vector3 vec) -> Real"""
        return _IreonClient.Vector3_dotProduct(*args)

    def normalise(*args):
        """normalise(self) -> Real"""
        return _IreonClient.Vector3_normalise(*args)

    def crossProduct(*args):
        """crossProduct(self, Vector3 rkVector) -> Vector3"""
        return _IreonClient.Vector3_crossProduct(*args)

    def midPoint(*args):
        """midPoint(self, Vector3 vec) -> Vector3"""
        return _IreonClient.Vector3_midPoint(*args)

    def __lt__(*args):
        """__lt__(self, Vector3 rhs) -> bool"""
        return _IreonClient.Vector3___lt__(*args)

    def __gt__(*args):
        """__gt__(self, Vector3 rhs) -> bool"""
        return _IreonClient.Vector3___gt__(*args)

    def makeFloor(*args):
        """makeFloor(self, Vector3 cmp)"""
        return _IreonClient.Vector3_makeFloor(*args)

    def makeCeil(*args):
        """makeCeil(self, Vector3 cmp)"""
        return _IreonClient.Vector3_makeCeil(*args)

    def perpendicular(*args):
        """perpendicular(self) -> Vector3"""
        return _IreonClient.Vector3_perpendicular(*args)

    def randomDeviant(*args):
        """
        randomDeviant(self, Radian angle, Vector3 up=ZERO) -> Vector3
        randomDeviant(self, Radian angle) -> Vector3
        """
        return _IreonClient.Vector3_randomDeviant(*args)

    def getRotationTo(*args):
        """
        getRotationTo(self, Vector3 dest, Vector3 fallbackAxis=ZERO) -> Quaternion
        getRotationTo(self, Vector3 dest) -> Quaternion
        """
        return _IreonClient.Vector3_getRotationTo(*args)

    def normalisedCopy(*args):
        """normalisedCopy(self) -> Vector3"""
        return _IreonClient.Vector3_normalisedCopy(*args)

    def reflect(*args):
        """reflect(self, Vector3 normal) -> Vector3"""
        return _IreonClient.Vector3_reflect(*args)

    def positionEquals(*args):
        """
        positionEquals(self, Vector3 rhs, Real tolerance=1e-03) -> bool
        positionEquals(self, Vector3 rhs) -> bool
        """
        return _IreonClient.Vector3_positionEquals(*args)

    def directionEquals(*args):
        """directionEquals(self, Vector3 rhs, Radian tolerance) -> bool"""
        return _IreonClient.Vector3_directionEquals(*args)

    length = property(_IreonClient.Vector3_length_get, None, None,
                    "Vector3.length -> Ogre::Real  (read-only)\n\nThis is equivalent to calling the C++ Ogre method:\n\tget: Ogre::Real Vector3::length()")

    squaredLength = property(_IreonClient.Vector3_squaredLength_get, None, None,
                    "Vector3.squaredLength -> Ogre::Real  (read-only)\n\nThis is equivalent to calling the C++ Ogre method:\n\tget: Ogre::Real Vector3::squaredLength()")

    def __nonzero__(*args):
        """__nonzero__(self, void ?) -> PyObject"""
        return _IreonClient.Vector3___nonzero__(*args)

    def __str__(*args):
        """__str__(self, void ?) -> PyObject"""
        return _IreonClient.Vector3___str__(*args)

    def __getitem__(*args):
        """__getitem__(self, size_t i) -> PyObject"""
        return _IreonClient.Vector3___getitem__(*args)

    def __iadd__(*args):
        """__iadd__(self, Vector3 v)"""
        return _IreonClient.Vector3___iadd__(*args)

    def __isub__(*args):
        """__isub__(self, Vector3 v)"""
        return _IreonClient.Vector3___isub__(*args)

    def __imul__(*args):
        """
        __imul__(self, Vector3 v)
        __imul__(self, Real f)
        """
        return _IreonClient.Vector3___imul__(*args)

    def __idiv__(*args):
        """
        __idiv__(self, Vector3 v)
        __idiv__(self, Real f)
        """
        return _IreonClient.Vector3___idiv__(*args)

    def __init__(self, *args): 
        """
        __init__(self) -> Vector3
        __init__(self, Real fX, Real fY, Real fZ) -> Vector3
        __init__(self, Vector3 rkVector) -> Vector3
        """
        this = _IreonClient.new_Vector3(*args)
        try: self.this.append(this)
        except: self.this = this
    __swig_setmethods__["x"] = _IreonClient.Vector3_x_set
    __swig_getmethods__["x"] = _IreonClient.Vector3_x_get
    if _newclass:x = property(_IreonClient.Vector3_x_get, _IreonClient.Vector3_x_set)
    __swig_setmethods__["y"] = _IreonClient.Vector3_y_set
    __swig_getmethods__["y"] = _IreonClient.Vector3_y_get
    if _newclass:y = property(_IreonClient.Vector3_y_get, _IreonClient.Vector3_y_set)
    __swig_setmethods__["z"] = _IreonClient.Vector3_z_set
    __swig_getmethods__["z"] = _IreonClient.Vector3_z_get
    if _newclass:z = property(_IreonClient.Vector3_z_get, _IreonClient.Vector3_z_set)
    __swig_destroy__ = _IreonClient.delete_Vector3
    __del__ = lambda self : None;
Vector3_swigregister = _IreonClient.Vector3_swigregister
Vector3_swigregister(Vector3)
Vector3.ZERO = _IreonClient.cvar.Vector3_ZERO
Vector3.UNIT_X = _IreonClient.cvar.Vector3_UNIT_X
Vector3.UNIT_Y = _IreonClient.cvar.Vector3_UNIT_Y
Vector3.UNIT_Z = _IreonClient.cvar.Vector3_UNIT_Z
Vector3.NEGATIVE_UNIT_X = _IreonClient.cvar.Vector3_NEGATIVE_UNIT_X
Vector3.NEGATIVE_UNIT_Y = _IreonClient.cvar.Vector3_NEGATIVE_UNIT_Y
Vector3.NEGATIVE_UNIT_Z = _IreonClient.cvar.Vector3_NEGATIVE_UNIT_Z
Vector3.UNIT_SCALE = _IreonClient.cvar.Vector3_UNIT_SCALE

Vector3.ZERO = Vector3(0, 0, 0)
Vector3.UNIT_X = Vector3(1, 0, 0)
Vector3.UNIT_Y = Vector3(0, 1, 0)
Vector3.UNIT_Z = Vector3(0, 0, 1)
Vector3.NEGATIVE_UNIT_X = Vector3(-1, 0, 0)
Vector3.NEGATIVE_UNIT_Y = Vector3(0, -1, 0)
Vector3.NEGATIVE_UNIT_Z = Vector3(0, 0, -1)
Vector3.UNIT_SCALE = Vector3(1, 1, 1)

class Vector4(_object):
    """Proxy of C++ Vector4 class"""
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, Vector4, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, Vector4, name)
    __repr__ = _swig_repr
    def __eq__(*args):
        """__eq__(self, Vector4 rkVector) -> bool"""
        return _IreonClient.Vector4___eq__(*args)

    def __ne__(*args):
        """__ne__(self, Vector4 rkVector) -> bool"""
        return _IreonClient.Vector4___ne__(*args)

    def __add__(*args):
        """__add__(self, Vector4 rkVector) -> Vector4"""
        return _IreonClient.Vector4___add__(*args)

    def __sub__(*args):
        """__sub__(self, Vector4 rkVector) -> Vector4"""
        return _IreonClient.Vector4___sub__(*args)

    def __mul__(*args):
        """
        __mul__(self, Real fScalar) -> Vector4
        __mul__(self, Vector4 rhs) -> Vector4
        """
        return _IreonClient.Vector4___mul__(*args)

    def __div__(*args):
        """
        __div__(self, Real fScalar) -> Vector4
        __div__(self, Vector4 rhs) -> Vector4
        """
        return _IreonClient.Vector4___div__(*args)

    def __pos__(*args):
        """__pos__(self) -> Vector4"""
        return _IreonClient.Vector4___pos__(*args)

    def __neg__(*args):
        """__neg__(self) -> Vector4"""
        return _IreonClient.Vector4___neg__(*args)

    def dotProduct(*args):
        """dotProduct(self, Vector4 vec) -> Real"""
        return _IreonClient.Vector4_dotProduct(*args)

    def __nonzero__(*args):
        """__nonzero__(self, void ?) -> PyObject"""
        return _IreonClient.Vector4___nonzero__(*args)

    def __str__(*args):
        """__str__(self, void ?) -> PyObject"""
        return _IreonClient.Vector4___str__(*args)

    def __getitem__(*args):
        """__getitem__(self, size_t i) -> PyObject"""
        return _IreonClient.Vector4___getitem__(*args)

    def __iadd__(*args):
        """__iadd__(self, Vector4 v)"""
        return _IreonClient.Vector4___iadd__(*args)

    def __isub__(*args):
        """__isub__(self, Vector4 v)"""
        return _IreonClient.Vector4___isub__(*args)

    def __imul__(*args):
        """
        __imul__(self, Vector4 v)
        __imul__(self, Real f)
        """
        return _IreonClient.Vector4___imul__(*args)

    def __idiv__(*args):
        """
        __idiv__(self, Vector4 v)
        __idiv__(self, Real f)
        """
        return _IreonClient.Vector4___idiv__(*args)

    def __init__(self, *args): 
        """
        __init__(self) -> Vector4
        __init__(self, Real fX, Real fY, Real fZ, Real fW) -> Vector4
        __init__(self, Vector4 rkVector) -> Vector4
        """
        this = _IreonClient.new_Vector4(*args)
        try: self.this.append(this)
        except: self.this = this
    __swig_setmethods__["x"] = _IreonClient.Vector4_x_set
    __swig_getmethods__["x"] = _IreonClient.Vector4_x_get
    if _newclass:x = property(_IreonClient.Vector4_x_get, _IreonClient.Vector4_x_set)
    __swig_setmethods__["y"] = _IreonClient.Vector4_y_set
    __swig_getmethods__["y"] = _IreonClient.Vector4_y_get
    if _newclass:y = property(_IreonClient.Vector4_y_get, _IreonClient.Vector4_y_set)
    __swig_setmethods__["z"] = _IreonClient.Vector4_z_set
    __swig_getmethods__["z"] = _IreonClient.Vector4_z_get
    if _newclass:z = property(_IreonClient.Vector4_z_get, _IreonClient.Vector4_z_set)
    __swig_setmethods__["w"] = _IreonClient.Vector4_w_set
    __swig_getmethods__["w"] = _IreonClient.Vector4_w_get
    if _newclass:w = property(_IreonClient.Vector4_w_get, _IreonClient.Vector4_w_set)
    __swig_destroy__ = _IreonClient.delete_Vector4
    __del__ = lambda self : None;
Vector4_swigregister = _IreonClient.Vector4_swigregister
Vector4_swigregister(Vector4)
Vector4.ZERO = _IreonClient.cvar.Vector4_ZERO

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

class Matrix3(_object):
    """Proxy of C++ Matrix3 class"""
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, Matrix3, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, Matrix3, name)
    __repr__ = _swig_repr
    def __init__(self, *args): 
        """
        __init__(self) -> Matrix3
        __init__(self, Real arr) -> Matrix3
        __init__(self, Matrix3 rkMatrix) -> Matrix3
        __init__(self, Real fEntry00, Real fEntry01, Real fEntry02, Real fEntry10, 
            Real fEntry11, Real fEntry12, Real fEntry20, 
            Real fEntry21, Real fEntry22) -> Matrix3
        """
        this = _IreonClient.new_Matrix3(*args)
        try: self.this.append(this)
        except: self.this = this
    def GetColumn(*args):
        """GetColumn(self, size_t iCol) -> Vector3"""
        return _IreonClient.Matrix3_GetColumn(*args)

    def SetColumn(*args):
        """SetColumn(self, size_t iCol, Vector3 vec)"""
        return _IreonClient.Matrix3_SetColumn(*args)

    def FromAxes(*args):
        """FromAxes(self, Vector3 xAxis, Vector3 yAxis, Vector3 zAxis)"""
        return _IreonClient.Matrix3_FromAxes(*args)

    def __eq__(*args):
        """__eq__(self, Matrix3 rkMatrix) -> bool"""
        return _IreonClient.Matrix3___eq__(*args)

    def __ne__(*args):
        """__ne__(self, Matrix3 rkMatrix) -> bool"""
        return _IreonClient.Matrix3___ne__(*args)

    def __add__(*args):
        """__add__(self, Matrix3 rkMatrix) -> Matrix3"""
        return _IreonClient.Matrix3___add__(*args)

    def __sub__(*args):
        """__sub__(self, Matrix3 rkMatrix) -> Matrix3"""
        return _IreonClient.Matrix3___sub__(*args)

    def __neg__(*args):
        """__neg__(self) -> Matrix3"""
        return _IreonClient.Matrix3___neg__(*args)

    def __mul__(*args):
        """
        __mul__(self, Matrix3 rkMatrix) -> Matrix3
        __mul__(self, Vector3 rkVector) -> Vector3
        __mul__(self, Real fScalar) -> Matrix3
        """
        return _IreonClient.Matrix3___mul__(*args)

    def Transpose(*args):
        """Transpose(self) -> Matrix3"""
        return _IreonClient.Matrix3_Transpose(*args)

    def Inverse(*args):
        """
        Inverse(self, Matrix3 rkInverse, Real fTolerance=1e-06) -> bool
        Inverse(self, Matrix3 rkInverse) -> bool
        Inverse(self, Real fTolerance=1e-06) -> Matrix3
        Inverse(self) -> Matrix3
        """
        return _IreonClient.Matrix3_Inverse(*args)

    def Determinant(*args):
        """Determinant(self) -> Real"""
        return _IreonClient.Matrix3_Determinant(*args)

    def SingularValueDecomposition(*args):
        """SingularValueDecomposition(self, Matrix3 rkL, Vector3 rkS, Matrix3 rkR)"""
        return _IreonClient.Matrix3_SingularValueDecomposition(*args)

    def SingularValueComposition(*args):
        """SingularValueComposition(self, Matrix3 rkL, Vector3 rkS, Matrix3 rkR)"""
        return _IreonClient.Matrix3_SingularValueComposition(*args)

    def Orthonormalize(*args):
        """Orthonormalize(self)"""
        return _IreonClient.Matrix3_Orthonormalize(*args)

    def QDUDecomposition(*args):
        """QDUDecomposition(self, Matrix3 rkQ, Vector3 rkD, Vector3 rkU)"""
        return _IreonClient.Matrix3_QDUDecomposition(*args)

    def SpectralNorm(*args):
        """SpectralNorm(self) -> Real"""
        return _IreonClient.Matrix3_SpectralNorm(*args)

    def ToAxisAngle(*args):
        """
        ToAxisAngle(self, Vector3 rkAxis, Radian rfAngle)
        ToAxisAngle(self, Vector3 rkAxis, Degree rfAngle)
        """
        return _IreonClient.Matrix3_ToAxisAngle(*args)

    def FromAxisAngle(*args):
        """FromAxisAngle(self, Vector3 rkAxis, Radian fRadians)"""
        return _IreonClient.Matrix3_FromAxisAngle(*args)

    def ToEulerAnglesXYZ(*args):
        """ToEulerAnglesXYZ(self, Radian rfYAngle, Radian rfPAngle, Radian rfRAngle) -> bool"""
        return _IreonClient.Matrix3_ToEulerAnglesXYZ(*args)

    def ToEulerAnglesXZY(*args):
        """ToEulerAnglesXZY(self, Radian rfYAngle, Radian rfPAngle, Radian rfRAngle) -> bool"""
        return _IreonClient.Matrix3_ToEulerAnglesXZY(*args)

    def ToEulerAnglesYXZ(*args):
        """ToEulerAnglesYXZ(self, Radian rfYAngle, Radian rfPAngle, Radian rfRAngle) -> bool"""
        return _IreonClient.Matrix3_ToEulerAnglesYXZ(*args)

    def ToEulerAnglesYZX(*args):
        """ToEulerAnglesYZX(self, Radian rfYAngle, Radian rfPAngle, Radian rfRAngle) -> bool"""
        return _IreonClient.Matrix3_ToEulerAnglesYZX(*args)

    def ToEulerAnglesZXY(*args):
        """ToEulerAnglesZXY(self, Radian rfYAngle, Radian rfPAngle, Radian rfRAngle) -> bool"""
        return _IreonClient.Matrix3_ToEulerAnglesZXY(*args)

    def ToEulerAnglesZYX(*args):
        """ToEulerAnglesZYX(self, Radian rfYAngle, Radian rfPAngle, Radian rfRAngle) -> bool"""
        return _IreonClient.Matrix3_ToEulerAnglesZYX(*args)

    def FromEulerAnglesXYZ(*args):
        """FromEulerAnglesXYZ(self, Radian fYAngle, Radian fPAngle, Radian fRAngle)"""
        return _IreonClient.Matrix3_FromEulerAnglesXYZ(*args)

    def FromEulerAnglesXZY(*args):
        """FromEulerAnglesXZY(self, Radian fYAngle, Radian fPAngle, Radian fRAngle)"""
        return _IreonClient.Matrix3_FromEulerAnglesXZY(*args)

    def FromEulerAnglesYXZ(*args):
        """FromEulerAnglesYXZ(self, Radian fYAngle, Radian fPAngle, Radian fRAngle)"""
        return _IreonClient.Matrix3_FromEulerAnglesYXZ(*args)

    def FromEulerAnglesYZX(*args):
        """FromEulerAnglesYZX(self, Radian fYAngle, Radian fPAngle, Radian fRAngle)"""
        return _IreonClient.Matrix3_FromEulerAnglesYZX(*args)

    def FromEulerAnglesZXY(*args):
        """FromEulerAnglesZXY(self, Radian fYAngle, Radian fPAngle, Radian fRAngle)"""
        return _IreonClient.Matrix3_FromEulerAnglesZXY(*args)

    def FromEulerAnglesZYX(*args):
        """FromEulerAnglesZYX(self, Radian fYAngle, Radian fPAngle, Radian fRAngle)"""
        return _IreonClient.Matrix3_FromEulerAnglesZYX(*args)

    def EigenSolveSymmetric(*args):
        """EigenSolveSymmetric(self, Real afEigenvalue, Vector3 akEigenvector)"""
        return _IreonClient.Matrix3_EigenSolveSymmetric(*args)

    def TensorProduct(*args):
        """TensorProduct(Vector3 rkU, Vector3 rkV, Matrix3 rkProduct)"""
        return _IreonClient.Matrix3_TensorProduct(*args)

    if _newclass:TensorProduct = staticmethod(TensorProduct)
    __swig_getmethods__["TensorProduct"] = lambda x: TensorProduct
    def __getitem__(*args):
        """__getitem__(self, size_t i) -> Vector3"""
        return _IreonClient.Matrix3___getitem__(*args)

    def Get(*args):
        """Get(self, size_t iRow, size_t iColumn) -> Real"""
        return _IreonClient.Matrix3_Get(*args)

    def Set(*args):
        """Set(self, size_t iRow, size_t iColumn, Real value)"""
        return _IreonClient.Matrix3_Set(*args)

    def GetRow(*args):
        """GetRow(self, size_t iRow) -> Vector3"""
        return _IreonClient.Matrix3_GetRow(*args)

    def SetRow(*args):
        """SetRow(self, size_t iRow, Vector3 vec)"""
        return _IreonClient.Matrix3_SetRow(*args)

    __swig_destroy__ = _IreonClient.delete_Matrix3
    __del__ = lambda self : None;
Matrix3_swigregister = _IreonClient.Matrix3_swigregister
Matrix3_swigregister(Matrix3)

def Matrix3_TensorProduct(*args):
  """Matrix3_TensorProduct(Vector3 rkU, Vector3 rkV, Matrix3 rkProduct)"""
  return _IreonClient.Matrix3_TensorProduct(*args)
Matrix3.EPSILON = _IreonClient.cvar.Matrix3_EPSILON
Matrix3.ZERO = _IreonClient.cvar.Matrix3_ZERO
Matrix3.IDENTITY = _IreonClient.cvar.Matrix3_IDENTITY

Matrix3.EPSILON = 1e-06
Matrix3.ZERO = Matrix3(0, 0, 0, 0, 0, 0, 0, 0, 0);
Matrix3.IDENTITY = Matrix3(1, 0, 0, 0, 1, 0, 0, 0, 1);

class Matrix4(_object):
    """Proxy of C++ Matrix4 class"""
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, Matrix4, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, Matrix4, name)
    __repr__ = _swig_repr
    def __init__(self, *args): 
        """
        __init__(self) -> Matrix4
        __init__(self, Real m00, Real m01, Real m02, Real m03, Real m10, Real m11, 
            Real m12, Real m13, Real m20, Real m21, 
            Real m22, Real m23, Real m30, Real m31, Real m32, 
            Real m33) -> Matrix4
        __init__(self, Matrix3 m3x3) -> Matrix4
        __init__(self, Quaternion rot) -> Matrix4
        """
        this = _IreonClient.new_Matrix4(*args)
        try: self.this.append(this)
        except: self.this = this
    def concatenate(*args):
        """concatenate(self, Matrix4 m2) -> Matrix4"""
        return _IreonClient.Matrix4_concatenate(*args)

    def __add__(*args):
        """__add__(self, Matrix4 m2) -> Matrix4"""
        return _IreonClient.Matrix4___add__(*args)

    def __sub__(*args):
        """__sub__(self, Matrix4 m2) -> Matrix4"""
        return _IreonClient.Matrix4___sub__(*args)

    def __eq__(*args):
        """__eq__(self, Matrix4 m2) -> bool"""
        return _IreonClient.Matrix4___eq__(*args)

    def __ne__(*args):
        """__ne__(self, Matrix4 m2) -> bool"""
        return _IreonClient.Matrix4___ne__(*args)

    def transpose(*args):
        """transpose(self) -> Matrix4"""
        return _IreonClient.Matrix4_transpose(*args)

    def setTrans(*args):
        """setTrans(self, Vector3 v)"""
        return _IreonClient.Matrix4_setTrans(*args)

    def makeTrans(*args):
        """
        makeTrans(self, Vector3 v)
        makeTrans(self, Real tx, Real ty, Real tz)
        """
        return _IreonClient.Matrix4_makeTrans(*args)

    def getTrans(*args):
        """
        getTrans() -> Vector3
        getTrans(Vector3 v) -> Matrix4
        getTrans(Real t_x, Real t_y, Real t_z) -> Matrix4
        """
        return _IreonClient.Matrix4_getTrans(*args)

    if _newclass:getTrans = staticmethod(getTrans)
    __swig_getmethods__["getTrans"] = lambda x: getTrans
    def setScale(*args):
        """setScale(self, Vector3 v)"""
        return _IreonClient.Matrix4_setScale(*args)

    def getScale(*args):
        """
        getScale(Vector3 v) -> Matrix4
        getScale(Real s_x, Real s_y, Real s_z) -> Matrix4
        """
        return _IreonClient.Matrix4_getScale(*args)

    if _newclass:getScale = staticmethod(getScale)
    __swig_getmethods__["getScale"] = lambda x: getScale
    def extract3x3Matrix(*args):
        """extract3x3Matrix(self, Matrix3 m3x3)"""
        return _IreonClient.Matrix4_extract3x3Matrix(*args)

    def extractQuaternion(*args):
        """extractQuaternion(self) -> Quaternion"""
        return _IreonClient.Matrix4_extractQuaternion(*args)

    def __mul__(*args):
        """
        __mul__(self, Matrix4 m2) -> Matrix4
        __mul__(self, Vector3 v) -> Vector3
        __mul__(self, Vector4 v) -> Vector4
        __mul__(self, Plane p) -> Plane
        __mul__(self, Real scalar) -> Matrix4
        """
        return _IreonClient.Matrix4___mul__(*args)

    def adjoint(*args):
        """adjoint(self) -> Matrix4"""
        return _IreonClient.Matrix4_adjoint(*args)

    def determinant(*args):
        """determinant(self) -> Real"""
        return _IreonClient.Matrix4_determinant(*args)

    def inverse(*args):
        """inverse(self) -> Matrix4"""
        return _IreonClient.Matrix4_inverse(*args)

    def makeTransform(*args):
        """makeTransform(self, Vector3 position, Vector3 scale, Quaternion orientation)"""
        return _IreonClient.Matrix4_makeTransform(*args)

    def makeInverseTransform(*args):
        """makeInverseTransform(self, Vector3 position, Vector3 scale, Quaternion orientation)"""
        return _IreonClient.Matrix4_makeInverseTransform(*args)

    def __getitem__(*args):
        """__getitem__(self, size_t i) -> Vector4"""
        return _IreonClient.Matrix4___getitem__(*args)

    __swig_destroy__ = _IreonClient.delete_Matrix4
    __del__ = lambda self : None;
Matrix4_swigregister = _IreonClient.Matrix4_swigregister
Matrix4_swigregister(Matrix4)

def Matrix4_getTrans(*args):
  """
    getTrans() -> Vector3
    getTrans(Vector3 v) -> Matrix4
    Matrix4_getTrans(Real t_x, Real t_y, Real t_z) -> Matrix4
    """
  return _IreonClient.Matrix4_getTrans(*args)

def Matrix4_getScale(*args):
  """
    getScale(Vector3 v) -> Matrix4
    Matrix4_getScale(Real s_x, Real s_y, Real s_z) -> Matrix4
    """
  return _IreonClient.Matrix4_getScale(*args)
Matrix4.ZERO = _IreonClient.cvar.Matrix4_ZERO
Matrix4.IDENTITY = _IreonClient.cvar.Matrix4_IDENTITY
Matrix4.CLIPSPACE2DTOIMAGESPACE = _IreonClient.cvar.Matrix4_CLIPSPACE2DTOIMAGESPACE


def __mul__(*args):
  """__mul__(Vector4 v, Matrix4 mat) -> Vector4"""
  return _IreonClient.__mul__(*args)
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

class Quaternion(_object):
    """Proxy of C++ Quaternion class"""
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, Quaternion, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, Quaternion, name)
    __repr__ = _swig_repr
    def __init__(self, *args): 
        """
        __init__(self, Real fW=1.0, Real fX=0.0, Real fY=0.0, Real fZ=0.0) -> Quaternion
        __init__(self, Real fW=1.0, Real fX=0.0, Real fY=0.0) -> Quaternion
        __init__(self, Real fW=1.0, Real fX=0.0) -> Quaternion
        __init__(self, Real fW=1.0) -> Quaternion
        __init__(self) -> Quaternion
        __init__(self, Quaternion rkQ) -> Quaternion
        __init__(self, Matrix3 rot) -> Quaternion
        __init__(self, Radian rfAngle, Vector3 rkAxis) -> Quaternion
        __init__(self, Vector3 xaxis, Vector3 yaxis, Vector3 zaxis) -> Quaternion
        __init__(self, Vector3 akAxis) -> Quaternion
        __init__(self, Real valptr) -> Quaternion
        """
        this = _IreonClient.new_Quaternion(*args)
        try: self.this.append(this)
        except: self.this = this
    def FromRotationMatrix(*args):
        """FromRotationMatrix(self, Matrix3 kRot)"""
        return _IreonClient.Quaternion_FromRotationMatrix(*args)

    def ToRotationMatrix(*args):
        """ToRotationMatrix(self, Matrix3 kRot)"""
        return _IreonClient.Quaternion_ToRotationMatrix(*args)

    def FromAngleAxis(*args):
        """FromAngleAxis(self, Radian rfAngle, Vector3 rkAxis)"""
        return _IreonClient.Quaternion_FromAngleAxis(*args)

    def ToAngleAxis(*args):
        """
        ToAngleAxis(self, Radian rfAngle, Vector3 rkAxis)
        ToAngleAxis(self, Degree dAngle, Vector3 rkAxis)
        """
        return _IreonClient.Quaternion_ToAngleAxis(*args)

    def FromAxes(*args):
        """
        FromAxes(self, Vector3 akAxis)
        FromAxes(self, Vector3 xAxis, Vector3 yAxis, Vector3 zAxis)
        """
        return _IreonClient.Quaternion_FromAxes(*args)

    def ToAxes(*args):
        """
        ToAxes(self, Vector3 akAxis)
        ToAxes(self, Vector3 xAxis, Vector3 yAxis, Vector3 zAxis)
        """
        return _IreonClient.Quaternion_ToAxes(*args)

    def xAxis(*args):
        """xAxis(self) -> Vector3"""
        return _IreonClient.Quaternion_xAxis(*args)

    def yAxis(*args):
        """yAxis(self) -> Vector3"""
        return _IreonClient.Quaternion_yAxis(*args)

    def zAxis(*args):
        """zAxis(self) -> Vector3"""
        return _IreonClient.Quaternion_zAxis(*args)

    def __add__(*args):
        """__add__(self, Quaternion rkQ) -> Quaternion"""
        return _IreonClient.Quaternion___add__(*args)

    def __sub__(*args):
        """__sub__(self, Quaternion rkQ) -> Quaternion"""
        return _IreonClient.Quaternion___sub__(*args)

    def __neg__(*args):
        """__neg__(self) -> Quaternion"""
        return _IreonClient.Quaternion___neg__(*args)

    def __eq__(*args):
        """__eq__(self, Quaternion rhs) -> bool"""
        return _IreonClient.Quaternion___eq__(*args)

    def __ne__(*args):
        """__ne__(self, Quaternion rhs) -> bool"""
        return _IreonClient.Quaternion___ne__(*args)

    def Dot(*args):
        """Dot(self, Quaternion rkQ) -> Real"""
        return _IreonClient.Quaternion_Dot(*args)

    def Norm(*args):
        """Norm(self) -> Real"""
        return _IreonClient.Quaternion_Norm(*args)

    def normalise(*args):
        """normalise(self) -> Real"""
        return _IreonClient.Quaternion_normalise(*args)

    def Inverse(*args):
        """Inverse(self) -> Quaternion"""
        return _IreonClient.Quaternion_Inverse(*args)

    def UnitInverse(*args):
        """UnitInverse(self) -> Quaternion"""
        return _IreonClient.Quaternion_UnitInverse(*args)

    def Exp(*args):
        """Exp(self) -> Quaternion"""
        return _IreonClient.Quaternion_Exp(*args)

    def Log(*args):
        """Log(self) -> Quaternion"""
        return _IreonClient.Quaternion_Log(*args)

    def __mul__(*args):
        """
        __mul__(self, Quaternion rkQ) -> Quaternion
        __mul__(self, Real fScalar) -> Quaternion
        __mul__(self, Vector3 rkVector) -> Vector3
        """
        return _IreonClient.Quaternion___mul__(*args)

    def getRoll(*args):
        """getRoll(self) -> Radian"""
        return _IreonClient.Quaternion_getRoll(*args)

    def getPitch(*args):
        """getPitch(self) -> Radian"""
        return _IreonClient.Quaternion_getPitch(*args)

    def getYaw(*args):
        """getYaw(self) -> Radian"""
        return _IreonClient.Quaternion_getYaw(*args)

    def equals(*args):
        """equals(self, Quaternion rhs, Radian tolerance) -> bool"""
        return _IreonClient.Quaternion_equals(*args)

    def Slerp(*args):
        """
        Slerp(Real fT, Quaternion rkP, Quaternion rkQ, bool shortestPath=False) -> Quaternion
        Slerp(Real fT, Quaternion rkP, Quaternion rkQ) -> Quaternion
        """
        return _IreonClient.Quaternion_Slerp(*args)

    if _newclass:Slerp = staticmethod(Slerp)
    __swig_getmethods__["Slerp"] = lambda x: Slerp
    def SlerpExtraSpins(*args):
        """SlerpExtraSpins(Real fT, Quaternion rkP, Quaternion rkQ, int iExtraSpins) -> Quaternion"""
        return _IreonClient.Quaternion_SlerpExtraSpins(*args)

    if _newclass:SlerpExtraSpins = staticmethod(SlerpExtraSpins)
    __swig_getmethods__["SlerpExtraSpins"] = lambda x: SlerpExtraSpins
    def Intermediate(*args):
        """
        Intermediate(Quaternion rkQ0, Quaternion rkQ1, Quaternion rkQ2, 
            Quaternion rka, Quaternion rkB)
        """
        return _IreonClient.Quaternion_Intermediate(*args)

    if _newclass:Intermediate = staticmethod(Intermediate)
    __swig_getmethods__["Intermediate"] = lambda x: Intermediate
    def Squad(*args):
        """
        Squad(Real fT, Quaternion rkP, Quaternion rkA, Quaternion rkB, 
            Quaternion rkQ, bool shortestPath=False) -> Quaternion
        Squad(Real fT, Quaternion rkP, Quaternion rkA, Quaternion rkB, 
            Quaternion rkQ) -> Quaternion
        """
        return _IreonClient.Quaternion_Squad(*args)

    if _newclass:Squad = staticmethod(Squad)
    __swig_getmethods__["Squad"] = lambda x: Squad
    def nlerp(*args):
        """
        nlerp(Real fT, Quaternion rkP, Quaternion rkQ, bool shortestPath=False) -> Quaternion
        nlerp(Real fT, Quaternion rkP, Quaternion rkQ) -> Quaternion
        """
        return _IreonClient.Quaternion_nlerp(*args)

    if _newclass:nlerp = staticmethod(nlerp)
    __swig_getmethods__["nlerp"] = lambda x: nlerp
    __swig_destroy__ = _IreonClient.delete_Quaternion
    __del__ = lambda self : None;
Quaternion_swigregister = _IreonClient.Quaternion_swigregister
Quaternion_swigregister(Quaternion)

def Quaternion_Slerp(*args):
  """
    Slerp(Real fT, Quaternion rkP, Quaternion rkQ, bool shortestPath=False) -> Quaternion
    Quaternion_Slerp(Real fT, Quaternion rkP, Quaternion rkQ) -> Quaternion
    """
  return _IreonClient.Quaternion_Slerp(*args)

def Quaternion_SlerpExtraSpins(*args):
  """Quaternion_SlerpExtraSpins(Real fT, Quaternion rkP, Quaternion rkQ, int iExtraSpins) -> Quaternion"""
  return _IreonClient.Quaternion_SlerpExtraSpins(*args)

def Quaternion_Intermediate(*args):
  """
    Quaternion_Intermediate(Quaternion rkQ0, Quaternion rkQ1, Quaternion rkQ2, 
        Quaternion rka, Quaternion rkB)
    """
  return _IreonClient.Quaternion_Intermediate(*args)

def Quaternion_Squad(*args):
  """
    Squad(Real fT, Quaternion rkP, Quaternion rkA, Quaternion rkB, 
        Quaternion rkQ, bool shortestPath=False) -> Quaternion
    Quaternion_Squad(Real fT, Quaternion rkP, Quaternion rkA, Quaternion rkB, 
        Quaternion rkQ) -> Quaternion
    """
  return _IreonClient.Quaternion_Squad(*args)

def Quaternion_nlerp(*args):
  """
    nlerp(Real fT, Quaternion rkP, Quaternion rkQ, bool shortestPath=False) -> Quaternion
    Quaternion_nlerp(Real fT, Quaternion rkP, Quaternion rkQ) -> Quaternion
    """
  return _IreonClient.Quaternion_nlerp(*args)
Quaternion.ms_fEpsilon = _IreonClient.cvar.Quaternion_ms_fEpsilon
Quaternion.ZERO = _IreonClient.cvar.Quaternion_ZERO
Quaternion.IDENTITY = _IreonClient.cvar.Quaternion_IDENTITY

Quaternion.ZERO = Quaternion(0, 0, 0, 0)
Quaternion.IDENTITY = Quaternion(1, 0, 0, 0)

class String(_object):
    """Proxy of C++ String class"""
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, String, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, String, name)
    def __init__(self): raise AttributeError, "No constructor defined"
    __repr__ = _swig_repr
    __swig_getmethods__["pyString"] = _IreonClient.String_pyString_get
    if _newclass:pyString = property(_IreonClient.String_pyString_get)
    def __str__(*args):
        """__str__(self) -> PyObject"""
        return _IreonClient.String___str__(*args)

String_swigregister = _IreonClient.String_swigregister
String_swigregister(String)

class StringVector(_object):
    """Proxy of C++ StringVector class"""
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, StringVector, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, StringVector, name)
    __repr__ = _swig_repr
    def iterator(*args):
        """iterator(self, PyObject PYTHON_SELF) -> PySwigIterator"""
        return _IreonClient.StringVector_iterator(*args)

    def __iter__(self): return self.iterator()
    def __nonzero__(*args):
        """__nonzero__(self) -> bool"""
        return _IreonClient.StringVector___nonzero__(*args)

    def __len__(*args):
        """__len__(self) -> std::vector<(std::string)>::size_type"""
        return _IreonClient.StringVector___len__(*args)

    def pop(*args):
        """pop(self) -> std::vector<(std::string)>::value_type"""
        return _IreonClient.StringVector_pop(*args)

    def __getslice__(*args):
        """__getslice__(self, std::vector<(std::string)>::difference_type i, std::vector<(std::string)>::difference_type j) -> StringVector"""
        return _IreonClient.StringVector___getslice__(*args)

    def __setslice__(*args):
        """
        __setslice__(self, std::vector<(std::string)>::difference_type i, std::vector<(std::string)>::difference_type j, 
            StringVector v)
        """
        return _IreonClient.StringVector___setslice__(*args)

    def __delslice__(*args):
        """__delslice__(self, std::vector<(std::string)>::difference_type i, std::vector<(std::string)>::difference_type j)"""
        return _IreonClient.StringVector___delslice__(*args)

    def __delitem__(*args):
        """__delitem__(self, std::vector<(std::string)>::difference_type i)"""
        return _IreonClient.StringVector___delitem__(*args)

    def __getitem__(*args):
        """__getitem__(self, std::vector<(std::string)>::difference_type i) -> std::vector<(std::string)>::value_type"""
        return _IreonClient.StringVector___getitem__(*args)

    def __setitem__(*args):
        """__setitem__(self, std::vector<(std::string)>::difference_type i, std::vector<(std::string)>::value_type x)"""
        return _IreonClient.StringVector___setitem__(*args)

    def append(*args):
        """append(self, std::vector<(std::string)>::value_type x)"""
        return _IreonClient.StringVector_append(*args)

    def empty(*args):
        """empty(self) -> bool"""
        return _IreonClient.StringVector_empty(*args)

    def size(*args):
        """size(self) -> std::vector<(std::string)>::size_type"""
        return _IreonClient.StringVector_size(*args)

    def clear(*args):
        """clear(self)"""
        return _IreonClient.StringVector_clear(*args)

    def swap(*args):
        """swap(self, StringVector v)"""
        return _IreonClient.StringVector_swap(*args)

    def get_allocator(*args):
        """get_allocator(self) -> std::vector<(std::string)>::allocator_type"""
        return _IreonClient.StringVector_get_allocator(*args)

    def begin(*args):
        """
        begin(self) -> std::vector<(std::string)>::iterator
        begin(self) -> std::vector<(std::string)>::const_iterator
        """
        return _IreonClient.StringVector_begin(*args)

    def end(*args):
        """
        end(self) -> std::vector<(std::string)>::iterator
        end(self) -> std::vector<(std::string)>::const_iterator
        """
        return _IreonClient.StringVector_end(*args)

    def rbegin(*args):
        """
        rbegin(self) -> std::vector<(std::string)>::reverse_iterator
        rbegin(self) -> std::vector<(std::string)>::const_reverse_iterator
        """
        return _IreonClient.StringVector_rbegin(*args)

    def rend(*args):
        """
        rend(self) -> std::vector<(std::string)>::reverse_iterator
        rend(self) -> std::vector<(std::string)>::const_reverse_iterator
        """
        return _IreonClient.StringVector_rend(*args)

    def pop_back(*args):
        """pop_back(self)"""
        return _IreonClient.StringVector_pop_back(*args)

    def erase(*args):
        """
        erase(self, std::vector<(std::string)>::iterator pos) -> std::vector<(std::string)>::iterator
        erase(self, std::vector<(std::string)>::iterator first, std::vector<(std::string)>::iterator last) -> std::vector<(std::string)>::iterator
        """
        return _IreonClient.StringVector_erase(*args)

    def __init__(self, *args): 
        """
        __init__(self) -> StringVector
        __init__(self, StringVector ?) -> StringVector
        __init__(self, std::vector<(std::string)>::size_type size) -> StringVector
        __init__(self, std::vector<(std::string)>::size_type size, std::vector<(std::string)>::value_type value) -> StringVector
        """
        this = _IreonClient.new_StringVector(*args)
        try: self.this.append(this)
        except: self.this = this
    def push_back(*args):
        """push_back(self, std::vector<(std::string)>::value_type x)"""
        return _IreonClient.StringVector_push_back(*args)

    def front(*args):
        """front(self) -> std::vector<(std::string)>::value_type"""
        return _IreonClient.StringVector_front(*args)

    def back(*args):
        """back(self) -> std::vector<(std::string)>::value_type"""
        return _IreonClient.StringVector_back(*args)

    def assign(*args):
        """assign(self, std::vector<(std::string)>::size_type n, std::vector<(std::string)>::value_type x)"""
        return _IreonClient.StringVector_assign(*args)

    def resize(*args):
        """
        resize(self, std::vector<(std::string)>::size_type new_size)
        resize(self, std::vector<(std::string)>::size_type new_size, std::vector<(std::string)>::value_type x)
        """
        return _IreonClient.StringVector_resize(*args)

    def insert(*args):
        """
        insert(self, std::vector<(std::string)>::iterator pos, std::vector<(std::string)>::value_type x) -> std::vector<(std::string)>::iterator
        insert(self, std::vector<(std::string)>::iterator pos, std::vector<(std::string)>::size_type n, 
            std::vector<(std::string)>::value_type x)
        """
        return _IreonClient.StringVector_insert(*args)

    def reserve(*args):
        """reserve(self, std::vector<(std::string)>::size_type n)"""
        return _IreonClient.StringVector_reserve(*args)

    def capacity(*args):
        """capacity(self) -> std::vector<(std::string)>::size_type"""
        return _IreonClient.StringVector_capacity(*args)

    __swig_destroy__ = _IreonClient.delete_StringVector
    __del__ = lambda self : None;
StringVector_swigregister = _IreonClient.StringVector_swigregister
StringVector_swigregister(StringVector)

class _StringVectorPtr(_object):
    """Proxy of C++ _StringVectorPtr class"""
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, _StringVectorPtr, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, _StringVectorPtr, name)
    __repr__ = _swig_repr
    def __init__(self, *args): 
        """
        __init__(self) -> _StringVectorPtr
        __init__(self, StringVector rep) -> _StringVectorPtr
        __init__(self, _StringVectorPtr r) -> _StringVectorPtr
        """
        this = _IreonClient.new__StringVectorPtr(*args)
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _IreonClient.delete__StringVectorPtr
    __del__ = lambda self : None;
    def get(*args):
        """get(self) -> StringVector"""
        return _IreonClient._StringVectorPtr_get(*args)

_StringVectorPtr_swigregister = _IreonClient._StringVectorPtr_swigregister
_StringVectorPtr_swigregister(_StringVectorPtr)

class StringConverter(_object):
    """Proxy of C++ StringConverter class"""
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, StringConverter, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, StringConverter, name)
    __repr__ = _swig_repr
    def toString(*args):
        """
        toString(Real val, unsigned short precision=6, unsigned short width=0, 
            char fill=' ', std::ios::fmtflags flags=std::ios::fmtflags(0)) -> String
        toString(Real val, unsigned short precision=6, unsigned short width=0, 
            char fill=' ') -> String
        toString(Real val, unsigned short precision=6, unsigned short width=0) -> String
        toString(Real val, unsigned short precision=6) -> String
        toString(Real val) -> String
        toString(Radian val, unsigned short precision=6, unsigned short width=0, 
            char fill=' ', std::ios::fmtflags flags=std::ios::fmtflags(0)) -> String
        toString(Radian val, unsigned short precision=6, unsigned short width=0, 
            char fill=' ') -> String
        toString(Radian val, unsigned short precision=6, unsigned short width=0) -> String
        toString(Radian val, unsigned short precision=6) -> String
        toString(Radian val) -> String
        toString(Degree val, unsigned short precision=6, unsigned short width=0, 
            char fill=' ', std::ios::fmtflags flags=std::ios::fmtflags(0)) -> String
        toString(Degree val, unsigned short precision=6, unsigned short width=0, 
            char fill=' ') -> String
        toString(Degree val, unsigned short precision=6, unsigned short width=0) -> String
        toString(Degree val, unsigned short precision=6) -> String
        toString(Degree val) -> String
        toString(int val, unsigned short width=0, char fill=' ', std::ios::fmtflags flags=std::ios::fmtflags(0)) -> String
        toString(int val, unsigned short width=0, char fill=' ') -> String
        toString(int val, unsigned short width=0) -> String
        toString(int val) -> String
        toString(unsigned long val, unsigned short width=0, char fill=' ', 
            std::ios::fmtflags flags=std::ios::fmtflags(0)) -> String
        toString(unsigned long val, unsigned short width=0, char fill=' ') -> String
        toString(unsigned long val, unsigned short width=0) -> String
        toString(unsigned long val) -> String
        toString(long val, unsigned short width=0, char fill=' ', std::ios::fmtflags flags=std::ios::fmtflags(0)) -> String
        toString(long val, unsigned short width=0, char fill=' ') -> String
        toString(long val, unsigned short width=0) -> String
        toString(long val) -> String
        toString(bool val, bool yesNo=False) -> String
        toString(bool val) -> String
        toString(Vector3 val) -> String
        toString(Matrix3 val) -> String
        toString(Matrix4 val) -> String
        toString(Quaternion val) -> String
        toString(ColourValue val) -> String
        toString(StringVector val) -> String
        """
        return _IreonClient.StringConverter_toString(*args)

    if _newclass:toString = staticmethod(toString)
    __swig_getmethods__["toString"] = lambda x: toString
    def parseReal(*args):
        """parseReal(String val) -> Real"""
        return _IreonClient.StringConverter_parseReal(*args)

    if _newclass:parseReal = staticmethod(parseReal)
    __swig_getmethods__["parseReal"] = lambda x: parseReal
    def parseAngle(*args):
        """parseAngle(String val) -> Radian"""
        return _IreonClient.StringConverter_parseAngle(*args)

    if _newclass:parseAngle = staticmethod(parseAngle)
    __swig_getmethods__["parseAngle"] = lambda x: parseAngle
    def parseInt(*args):
        """parseInt(String val) -> int"""
        return _IreonClient.StringConverter_parseInt(*args)

    if _newclass:parseInt = staticmethod(parseInt)
    __swig_getmethods__["parseInt"] = lambda x: parseInt
    def parseUnsignedInt(*args):
        """parseUnsignedInt(String val) -> unsigned int"""
        return _IreonClient.StringConverter_parseUnsignedInt(*args)

    if _newclass:parseUnsignedInt = staticmethod(parseUnsignedInt)
    __swig_getmethods__["parseUnsignedInt"] = lambda x: parseUnsignedInt
    def parseLong(*args):
        """parseLong(String val) -> long"""
        return _IreonClient.StringConverter_parseLong(*args)

    if _newclass:parseLong = staticmethod(parseLong)
    __swig_getmethods__["parseLong"] = lambda x: parseLong
    def parseUnsignedLong(*args):
        """parseUnsignedLong(String val) -> unsigned long"""
        return _IreonClient.StringConverter_parseUnsignedLong(*args)

    if _newclass:parseUnsignedLong = staticmethod(parseUnsignedLong)
    __swig_getmethods__["parseUnsignedLong"] = lambda x: parseUnsignedLong
    def parseBool(*args):
        """parseBool(String val) -> bool"""
        return _IreonClient.StringConverter_parseBool(*args)

    if _newclass:parseBool = staticmethod(parseBool)
    __swig_getmethods__["parseBool"] = lambda x: parseBool
    def parseVector3(*args):
        """parseVector3(String val) -> Vector3"""
        return _IreonClient.StringConverter_parseVector3(*args)

    if _newclass:parseVector3 = staticmethod(parseVector3)
    __swig_getmethods__["parseVector3"] = lambda x: parseVector3
    def parseMatrix3(*args):
        """parseMatrix3(String val) -> Matrix3"""
        return _IreonClient.StringConverter_parseMatrix3(*args)

    if _newclass:parseMatrix3 = staticmethod(parseMatrix3)
    __swig_getmethods__["parseMatrix3"] = lambda x: parseMatrix3
    def parseMatrix4(*args):
        """parseMatrix4(String val) -> Matrix4"""
        return _IreonClient.StringConverter_parseMatrix4(*args)

    if _newclass:parseMatrix4 = staticmethod(parseMatrix4)
    __swig_getmethods__["parseMatrix4"] = lambda x: parseMatrix4
    def parseQuaternion(*args):
        """parseQuaternion(String val) -> Quaternion"""
        return _IreonClient.StringConverter_parseQuaternion(*args)

    if _newclass:parseQuaternion = staticmethod(parseQuaternion)
    __swig_getmethods__["parseQuaternion"] = lambda x: parseQuaternion
    def parseColourValue(*args):
        """parseColourValue(String val) -> ColourValue"""
        return _IreonClient.StringConverter_parseColourValue(*args)

    if _newclass:parseColourValue = staticmethod(parseColourValue)
    __swig_getmethods__["parseColourValue"] = lambda x: parseColourValue
    def parseStringVector(*args):
        """parseStringVector(String val) -> StringVector"""
        return _IreonClient.StringConverter_parseStringVector(*args)

    if _newclass:parseStringVector = staticmethod(parseStringVector)
    __swig_getmethods__["parseStringVector"] = lambda x: parseStringVector
    def isNumber(*args):
        """isNumber(String val) -> bool"""
        return _IreonClient.StringConverter_isNumber(*args)

    if _newclass:isNumber = staticmethod(isNumber)
    __swig_getmethods__["isNumber"] = lambda x: isNumber
    def __init__(self, *args): 
        """__init__(self) -> StringConverter"""
        this = _IreonClient.new_StringConverter(*args)
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _IreonClient.delete_StringConverter
    __del__ = lambda self : None;
StringConverter_swigregister = _IreonClient.StringConverter_swigregister
StringConverter_swigregister(StringConverter)

def StringConverter_toString(*args):
  """
    toString(Real val, unsigned short precision=6, unsigned short width=0, 
        char fill=' ', std::ios::fmtflags flags=std::ios::fmtflags(0)) -> String
    toString(Real val, unsigned short precision=6, unsigned short width=0, 
        char fill=' ') -> String
    toString(Real val, unsigned short precision=6, unsigned short width=0) -> String
    toString(Real val, unsigned short precision=6) -> String
    toString(Real val) -> String
    toString(Radian val, unsigned short precision=6, unsigned short width=0, 
        char fill=' ', std::ios::fmtflags flags=std::ios::fmtflags(0)) -> String
    toString(Radian val, unsigned short precision=6, unsigned short width=0, 
        char fill=' ') -> String
    toString(Radian val, unsigned short precision=6, unsigned short width=0) -> String
    toString(Radian val, unsigned short precision=6) -> String
    toString(Radian val) -> String
    toString(Degree val, unsigned short precision=6, unsigned short width=0, 
        char fill=' ', std::ios::fmtflags flags=std::ios::fmtflags(0)) -> String
    toString(Degree val, unsigned short precision=6, unsigned short width=0, 
        char fill=' ') -> String
    toString(Degree val, unsigned short precision=6, unsigned short width=0) -> String
    toString(Degree val, unsigned short precision=6) -> String
    toString(Degree val) -> String
    toString(int val, unsigned short width=0, char fill=' ', std::ios::fmtflags flags=std::ios::fmtflags(0)) -> String
    toString(int val, unsigned short width=0, char fill=' ') -> String
    toString(int val, unsigned short width=0) -> String
    toString(int val) -> String
    toString(unsigned long val, unsigned short width=0, char fill=' ', 
        std::ios::fmtflags flags=std::ios::fmtflags(0)) -> String
    toString(unsigned long val, unsigned short width=0, char fill=' ') -> String
    toString(unsigned long val, unsigned short width=0) -> String
    toString(unsigned long val) -> String
    toString(long val, unsigned short width=0, char fill=' ', std::ios::fmtflags flags=std::ios::fmtflags(0)) -> String
    toString(long val, unsigned short width=0, char fill=' ') -> String
    toString(long val, unsigned short width=0) -> String
    toString(long val) -> String
    toString(bool val, bool yesNo=False) -> String
    toString(bool val) -> String
    toString(Vector3 val) -> String
    toString(Matrix3 val) -> String
    toString(Matrix4 val) -> String
    toString(Quaternion val) -> String
    toString(ColourValue val) -> String
    StringConverter_toString(StringVector val) -> String
    """
  return _IreonClient.StringConverter_toString(*args)

def StringConverter_parseReal(*args):
  """StringConverter_parseReal(String val) -> Real"""
  return _IreonClient.StringConverter_parseReal(*args)

def StringConverter_parseAngle(*args):
  """StringConverter_parseAngle(String val) -> Radian"""
  return _IreonClient.StringConverter_parseAngle(*args)

def StringConverter_parseInt(*args):
  """StringConverter_parseInt(String val) -> int"""
  return _IreonClient.StringConverter_parseInt(*args)

def StringConverter_parseUnsignedInt(*args):
  """StringConverter_parseUnsignedInt(String val) -> unsigned int"""
  return _IreonClient.StringConverter_parseUnsignedInt(*args)

def StringConverter_parseLong(*args):
  """StringConverter_parseLong(String val) -> long"""
  return _IreonClient.StringConverter_parseLong(*args)

def StringConverter_parseUnsignedLong(*args):
  """StringConverter_parseUnsignedLong(String val) -> unsigned long"""
  return _IreonClient.StringConverter_parseUnsignedLong(*args)

def StringConverter_parseBool(*args):
  """StringConverter_parseBool(String val) -> bool"""
  return _IreonClient.StringConverter_parseBool(*args)

def StringConverter_parseVector3(*args):
  """StringConverter_parseVector3(String val) -> Vector3"""
  return _IreonClient.StringConverter_parseVector3(*args)

def StringConverter_parseMatrix3(*args):
  """StringConverter_parseMatrix3(String val) -> Matrix3"""
  return _IreonClient.StringConverter_parseMatrix3(*args)

def StringConverter_parseMatrix4(*args):
  """StringConverter_parseMatrix4(String val) -> Matrix4"""
  return _IreonClient.StringConverter_parseMatrix4(*args)

def StringConverter_parseQuaternion(*args):
  """StringConverter_parseQuaternion(String val) -> Quaternion"""
  return _IreonClient.StringConverter_parseQuaternion(*args)

def StringConverter_parseColourValue(*args):
  """StringConverter_parseColourValue(String val) -> ColourValue"""
  return _IreonClient.StringConverter_parseColourValue(*args)

def StringConverter_parseStringVector(*args):
  """StringConverter_parseStringVector(String val) -> StringVector"""
  return _IreonClient.StringConverter_parseStringVector(*args)

def StringConverter_isNumber(*args):
  """StringConverter_isNumber(String val) -> bool"""
  return _IreonClient.StringConverter_isNumber(*args)

KC_UNASSIGNED = _IreonClient.KC_UNASSIGNED
KC_ESCAPE = _IreonClient.KC_ESCAPE
KC_1 = _IreonClient.KC_1
KC_2 = _IreonClient.KC_2
KC_3 = _IreonClient.KC_3
KC_4 = _IreonClient.KC_4
KC_5 = _IreonClient.KC_5
KC_6 = _IreonClient.KC_6
KC_7 = _IreonClient.KC_7
KC_8 = _IreonClient.KC_8
KC_9 = _IreonClient.KC_9
KC_0 = _IreonClient.KC_0
KC_MINUS = _IreonClient.KC_MINUS
KC_EQUALS = _IreonClient.KC_EQUALS
KC_BACK = _IreonClient.KC_BACK
KC_TAB = _IreonClient.KC_TAB
KC_Q = _IreonClient.KC_Q
KC_W = _IreonClient.KC_W
KC_E = _IreonClient.KC_E
KC_R = _IreonClient.KC_R
KC_T = _IreonClient.KC_T
KC_Y = _IreonClient.KC_Y
KC_U = _IreonClient.KC_U
KC_I = _IreonClient.KC_I
KC_O = _IreonClient.KC_O
KC_P = _IreonClient.KC_P
KC_LBRACKET = _IreonClient.KC_LBRACKET
KC_RBRACKET = _IreonClient.KC_RBRACKET
KC_RETURN = _IreonClient.KC_RETURN
KC_LCONTROL = _IreonClient.KC_LCONTROL
KC_A = _IreonClient.KC_A
KC_S = _IreonClient.KC_S
KC_D = _IreonClient.KC_D
KC_F = _IreonClient.KC_F
KC_G = _IreonClient.KC_G
KC_H = _IreonClient.KC_H
KC_J = _IreonClient.KC_J
KC_K = _IreonClient.KC_K
KC_L = _IreonClient.KC_L
KC_SEMICOLON = _IreonClient.KC_SEMICOLON
KC_APOSTROPHE = _IreonClient.KC_APOSTROPHE
KC_GRAVE = _IreonClient.KC_GRAVE
KC_LSHIFT = _IreonClient.KC_LSHIFT
KC_BACKSLASH = _IreonClient.KC_BACKSLASH
KC_Z = _IreonClient.KC_Z
KC_X = _IreonClient.KC_X
KC_C = _IreonClient.KC_C
KC_V = _IreonClient.KC_V
KC_B = _IreonClient.KC_B
KC_N = _IreonClient.KC_N
KC_M = _IreonClient.KC_M
KC_COMMA = _IreonClient.KC_COMMA
KC_PERIOD = _IreonClient.KC_PERIOD
KC_SLASH = _IreonClient.KC_SLASH
KC_RSHIFT = _IreonClient.KC_RSHIFT
KC_MULTIPLY = _IreonClient.KC_MULTIPLY
KC_LMENU = _IreonClient.KC_LMENU
KC_SPACE = _IreonClient.KC_SPACE
KC_CAPITAL = _IreonClient.KC_CAPITAL
KC_F1 = _IreonClient.KC_F1
KC_F2 = _IreonClient.KC_F2
KC_F3 = _IreonClient.KC_F3
KC_F4 = _IreonClient.KC_F4
KC_F5 = _IreonClient.KC_F5
KC_F6 = _IreonClient.KC_F6
KC_F7 = _IreonClient.KC_F7
KC_F8 = _IreonClient.KC_F8
KC_F9 = _IreonClient.KC_F9
KC_F10 = _IreonClient.KC_F10
KC_NUMLOCK = _IreonClient.KC_NUMLOCK
KC_SCROLL = _IreonClient.KC_SCROLL
KC_NUMPAD7 = _IreonClient.KC_NUMPAD7
KC_NUMPAD8 = _IreonClient.KC_NUMPAD8
KC_NUMPAD9 = _IreonClient.KC_NUMPAD9
KC_SUBTRACT = _IreonClient.KC_SUBTRACT
KC_NUMPAD4 = _IreonClient.KC_NUMPAD4
KC_NUMPAD5 = _IreonClient.KC_NUMPAD5
KC_NUMPAD6 = _IreonClient.KC_NUMPAD6
KC_ADD = _IreonClient.KC_ADD
KC_NUMPAD1 = _IreonClient.KC_NUMPAD1
KC_NUMPAD2 = _IreonClient.KC_NUMPAD2
KC_NUMPAD3 = _IreonClient.KC_NUMPAD3
KC_NUMPAD0 = _IreonClient.KC_NUMPAD0
KC_DECIMAL = _IreonClient.KC_DECIMAL
KC_OEM_102 = _IreonClient.KC_OEM_102
KC_F11 = _IreonClient.KC_F11
KC_F12 = _IreonClient.KC_F12
KC_F13 = _IreonClient.KC_F13
KC_F14 = _IreonClient.KC_F14
KC_F15 = _IreonClient.KC_F15
KC_KANA = _IreonClient.KC_KANA
KC_ABNT_C1 = _IreonClient.KC_ABNT_C1
KC_CONVERT = _IreonClient.KC_CONVERT
KC_NOCONVERT = _IreonClient.KC_NOCONVERT
KC_YEN = _IreonClient.KC_YEN
KC_ABNT_C2 = _IreonClient.KC_ABNT_C2
KC_NUMPADEQUALS = _IreonClient.KC_NUMPADEQUALS
KC_PREVTRACK = _IreonClient.KC_PREVTRACK
KC_AT = _IreonClient.KC_AT
KC_COLON = _IreonClient.KC_COLON
KC_UNDERLINE = _IreonClient.KC_UNDERLINE
KC_KANJI = _IreonClient.KC_KANJI
KC_STOP = _IreonClient.KC_STOP
KC_AX = _IreonClient.KC_AX
KC_UNLABELED = _IreonClient.KC_UNLABELED
KC_NEXTTRACK = _IreonClient.KC_NEXTTRACK
KC_NUMPADENTER = _IreonClient.KC_NUMPADENTER
KC_RCONTROL = _IreonClient.KC_RCONTROL
KC_MUTE = _IreonClient.KC_MUTE
KC_CALCULATOR = _IreonClient.KC_CALCULATOR
KC_PLAYPAUSE = _IreonClient.KC_PLAYPAUSE
KC_MEDIASTOP = _IreonClient.KC_MEDIASTOP
KC_VOLUMEDOWN = _IreonClient.KC_VOLUMEDOWN
KC_VOLUMEUP = _IreonClient.KC_VOLUMEUP
KC_WEBHOME = _IreonClient.KC_WEBHOME
KC_NUMPADCOMMA = _IreonClient.KC_NUMPADCOMMA
KC_DIVIDE = _IreonClient.KC_DIVIDE
KC_SYSRQ = _IreonClient.KC_SYSRQ
KC_RMENU = _IreonClient.KC_RMENU
KC_PAUSE = _IreonClient.KC_PAUSE
KC_HOME = _IreonClient.KC_HOME
KC_UP = _IreonClient.KC_UP
KC_PGUP = _IreonClient.KC_PGUP
KC_LEFT = _IreonClient.KC_LEFT
KC_RIGHT = _IreonClient.KC_RIGHT
KC_END = _IreonClient.KC_END
KC_DOWN = _IreonClient.KC_DOWN
KC_PGDOWN = _IreonClient.KC_PGDOWN
KC_INSERT = _IreonClient.KC_INSERT
KC_DELETE = _IreonClient.KC_DELETE
KC_LWIN = _IreonClient.KC_LWIN
KC_RWIN = _IreonClient.KC_RWIN
KC_APPS = _IreonClient.KC_APPS
KC_POWER = _IreonClient.KC_POWER
KC_SLEEP = _IreonClient.KC_SLEEP
KC_WAKE = _IreonClient.KC_WAKE
KC_WEBSEARCH = _IreonClient.KC_WEBSEARCH
KC_WEBFAVORITES = _IreonClient.KC_WEBFAVORITES
KC_WEBREFRESH = _IreonClient.KC_WEBREFRESH
KC_WEBSTOP = _IreonClient.KC_WEBSTOP
KC_WEBFORWARD = _IreonClient.KC_WEBFORWARD
KC_WEBBACK = _IreonClient.KC_WEBBACK
KC_MYCOMPUTER = _IreonClient.KC_MYCOMPUTER
KC_MAIL = _IreonClient.KC_MAIL
KC_MEDIASELECT = _IreonClient.KC_MEDIASELECT
class KeyEvent(_object):
    """Proxy of C++ KeyEvent class"""
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, KeyEvent, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, KeyEvent, name)
    __repr__ = _swig_repr
    def __init__(self, *args): 
        """__init__(self, Object obj, unsigned int ts, KeyCode kc, unsigned int txt) -> KeyEvent"""
        this = _IreonClient.new_KeyEvent(*args)
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _IreonClient.delete_KeyEvent
    __del__ = lambda self : None;
    __swig_getmethods__["key"] = _IreonClient.KeyEvent_key_get
    if _newclass:key = property(_IreonClient.KeyEvent_key_get)
    __swig_setmethods__["text"] = _IreonClient.KeyEvent_text_set
    __swig_getmethods__["text"] = _IreonClient.KeyEvent_text_get
    if _newclass:text = property(_IreonClient.KeyEvent_text_get, _IreonClient.KeyEvent_text_set)
KeyEvent_swigregister = _IreonClient.KeyEvent_swigregister
KeyEvent_swigregister(KeyEvent)

class KeyListener(_object):
    """Proxy of C++ KeyListener class"""
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, KeyListener, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, KeyListener, name)
    def __init__(self): raise AttributeError, "No constructor defined"
    __repr__ = _swig_repr
    __swig_destroy__ = _IreonClient.delete_KeyListener
    __del__ = lambda self : None;
    def keyPressed(*args):
        """keyPressed(self, KeyEvent arg) -> bool"""
        return _IreonClient.KeyListener_keyPressed(*args)

    def keyReleased(*args):
        """keyReleased(self, KeyEvent arg) -> bool"""
        return _IreonClient.KeyListener_keyReleased(*args)

KeyListener_swigregister = _IreonClient.KeyListener_swigregister
KeyListener_swigregister(KeyListener)

class Keyboard(_object):
    """Proxy of C++ Keyboard class"""
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, Keyboard, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, Keyboard, name)
    def __init__(self): raise AttributeError, "No constructor defined"
    __repr__ = _swig_repr
    __swig_destroy__ = _IreonClient.delete_Keyboard
    __del__ = lambda self : None;
    def isKeyDown(*args):
        """isKeyDown(self, KeyCode key) -> bool"""
        return _IreonClient.Keyboard_isKeyDown(*args)

    def setEventCallback(*args):
        """setEventCallback(self, KeyListener keyListener)"""
        return _IreonClient.Keyboard_setEventCallback(*args)

    def getEventCallback(*args):
        """getEventCallback(self) -> KeyListener"""
        return _IreonClient.Keyboard_getEventCallback(*args)

    Off = _IreonClient.Keyboard_Off
    Unicode = _IreonClient.Keyboard_Unicode
    Ascii = _IreonClient.Keyboard_Ascii
    def setTextTranslation(*args):
        """setTextTranslation(self, TextTranslationMode mode)"""
        return _IreonClient.Keyboard_setTextTranslation(*args)

    def getTextTranslation(*args):
        """getTextTranslation(self) -> int"""
        return _IreonClient.Keyboard_getTextTranslation(*args)

    def getAsString(*args):
        """getAsString(self, KeyCode kc) -> string"""
        return _IreonClient.Keyboard_getAsString(*args)

    Shift = _IreonClient.Keyboard_Shift
    Ctrl = _IreonClient.Keyboard_Ctrl
    Alt = _IreonClient.Keyboard_Alt
    def isModifierDown(*args):
        """isModifierDown(self, Modifier mod) -> bool"""
        return _IreonClient.Keyboard_isModifierDown(*args)

    def copyKeyStates(*args):
        """copyKeyStates(self, char keys)"""
        return _IreonClient.Keyboard_copyKeyStates(*args)

Keyboard_swigregister = _IreonClient.Keyboard_swigregister
Keyboard_swigregister(Keyboard)

NONE = _IreonClient.NONE
ON_KEY_PRESSED = _IreonClient.ON_KEY_PRESSED
ON_KEY_RELEASED = _IreonClient.ON_KEY_RELEASED
ON_KEY_CLICKED = _IreonClient.ON_KEY_CLICKED
ON_FRAME = _IreonClient.ON_FRAME
ON_CHAR_TO_WORLD = _IreonClient.ON_CHAR_TO_WORLD
ON_CHAR_FROM_WORLD = _IreonClient.ON_CHAR_FROM_WORLD
ON_CHAR_UPDATE = _IreonClient.ON_CHAR_UPDATE
ON_CHAR_DIE = _IreonClient.ON_CHAR_DIE
EVENT_COUNT = _IreonClient.EVENT_COUNT
class StringLess(_object):
    """Proxy of C++ StringLess class"""
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, StringLess, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, StringLess, name)
    __repr__ = _swig_repr
    def __call__(*args):
        """__call__(self, String arg1, String arg2) -> bool"""
        return _IreonClient.StringLess___call__(*args)

    def __init__(self, *args): 
        """__init__(self) -> StringLess"""
        this = _IreonClient.new_StringLess(*args)
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _IreonClient.delete_StringLess
    __del__ = lambda self : None;
StringLess_swigregister = _IreonClient.StringLess_swigregister
StringLess_swigregister(StringLess)

class CEventManager(_object):
    """Proxy of C++ CEventManager class"""
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, CEventManager, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, CEventManager, name)
    def __init__(self): raise AttributeError, "No constructor defined"
    __repr__ = _swig_repr
    def instance(*args):
        """instance() -> CEventManager"""
        return _IreonClient.CEventManager_instance(*args)

    if _newclass:instance = staticmethod(instance)
    __swig_getmethods__["instance"] = lambda x: instance
    def registerSwigType(*args):
        """registerSwigType(self, swigTypeInfo arg, String eventName)"""
        return _IreonClient.CEventManager_registerSwigType(*args)

    def getSwigType(*args):
        """getSwigType(self, String eventName) -> swigTypeInfo"""
        return _IreonClient.CEventManager_getSwigType(*args)

    def execute(*args):
        """
        execute(self, uint id, EventArg arg) -> bool
        execute(self, String name, EventArg arg) -> bool
        """
        return _IreonClient.CEventManager_execute(*args)

    def keyPressed(*args):
        """keyPressed(self, KeyCode code) -> bool"""
        return _IreonClient.CEventManager_keyPressed(*args)

    def keyReleased(*args):
        """keyReleased(self, KeyCode code) -> bool"""
        return _IreonClient.CEventManager_keyReleased(*args)

    def keyClicked(*args):
        """keyClicked(self, KeyCode code) -> bool"""
        return _IreonClient.CEventManager_keyClicked(*args)

    def addEvent(*args):
        """addEvent(self, uint id, String handler)"""
        return _IreonClient.CEventManager_addEvent(*args)

    def removeEvent(*args):
        """removeEvent(self, uint id, String handler)"""
        return _IreonClient.CEventManager_removeEvent(*args)

    def addKeyPressedEvent(*args):
        """addKeyPressedEvent(self, KeyCode code, String handler)"""
        return _IreonClient.CEventManager_addKeyPressedEvent(*args)

    def addKeyReleasedEvent(*args):
        """addKeyReleasedEvent(self, KeyCode code, String handler)"""
        return _IreonClient.CEventManager_addKeyReleasedEvent(*args)

    def addKeyClickedEvent(*args):
        """addKeyClickedEvent(self, KeyCode code, String handler)"""
        return _IreonClient.CEventManager_addKeyClickedEvent(*args)

    def removeKeyPressedEvent(*args):
        """removeKeyPressedEvent(self, KeyCode code, String handler)"""
        return _IreonClient.CEventManager_removeKeyPressedEvent(*args)

    def removeKeyReleasedEvent(*args):
        """removeKeyReleasedEvent(self, KeyCode code, String handler)"""
        return _IreonClient.CEventManager_removeKeyReleasedEvent(*args)

    def removeKeyClickedEvent(*args):
        """removeKeyClickedEvent(self, KeyCode code, String handler)"""
        return _IreonClient.CEventManager_removeKeyClickedEvent(*args)

    __swig_destroy__ = _IreonClient.delete_CEventManager
    __del__ = lambda self : None;
CEventManager_swigregister = _IreonClient.CEventManager_swigregister
CEventManager_swigregister(CEventManager)

def CEventManager_instance(*args):
  """CEventManager_instance() -> CEventManager"""
  return _IreonClient.CEventManager_instance(*args)

class EventArg(_object):
    """Proxy of C++ EventArg class"""
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, EventArg, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, EventArg, name)
    def __init__(self): raise AttributeError, "No constructor defined"
    __repr__ = _swig_repr
    def getSwigType(*args):
        """getSwigType(self) -> swigTypeInfo"""
        return _IreonClient.EventArg_getSwigType(*args)

    __swig_destroy__ = _IreonClient.delete_EventArg
    __del__ = lambda self : None;
EventArg_swigregister = _IreonClient.EventArg_swigregister
EventArg_swigregister(EventArg)

class FrameEventArg(EventArg):
    """Proxy of C++ FrameEventArg class"""
    __swig_setmethods__ = {}
    for _s in [EventArg]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, FrameEventArg, name, value)
    __swig_getmethods__ = {}
    for _s in [EventArg]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, FrameEventArg, name)
    __repr__ = _swig_repr
    def registerSwigType(*args):
        """registerSwigType(swigTypeInfo typeInfo)"""
        return _IreonClient.FrameEventArg_registerSwigType(*args)

    if _newclass:registerSwigType = staticmethod(registerSwigType)
    __swig_getmethods__["registerSwigType"] = lambda x: registerSwigType
    def getSwigType(*args):
        """getSwigType(self) -> swigTypeInfo"""
        return _IreonClient.FrameEventArg_getSwigType(*args)

    __swig_setmethods__["m_time"] = _IreonClient.FrameEventArg_m_time_set
    __swig_getmethods__["m_time"] = _IreonClient.FrameEventArg_m_time_get
    if _newclass:m_time = property(_IreonClient.FrameEventArg_m_time_get, _IreonClient.FrameEventArg_m_time_set)
    def __init__(self, *args): 
        """__init__(self) -> FrameEventArg"""
        this = _IreonClient.new_FrameEventArg(*args)
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _IreonClient.delete_FrameEventArg
    __del__ = lambda self : None;
FrameEventArg_swigregister = _IreonClient.FrameEventArg_swigregister
FrameEventArg_swigregister(FrameEventArg)

def FrameEventArg_registerSwigType(*args):
  """FrameEventArg_registerSwigType(swigTypeInfo typeInfo)"""
  return _IreonClient.FrameEventArg_registerSwigType(*args)

class CharacterEventArg(EventArg):
    """Proxy of C++ CharacterEventArg class"""
    __swig_setmethods__ = {}
    for _s in [EventArg]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, CharacterEventArg, name, value)
    __swig_getmethods__ = {}
    for _s in [EventArg]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, CharacterEventArg, name)
    __repr__ = _swig_repr
    def registerSwigType(*args):
        """registerSwigType(swigTypeInfo typeInfo)"""
        return _IreonClient.CharacterEventArg_registerSwigType(*args)

    if _newclass:registerSwigType = staticmethod(registerSwigType)
    __swig_getmethods__["registerSwigType"] = lambda x: registerSwigType
    def getSwigType(*args):
        """getSwigType(self) -> swigTypeInfo"""
        return _IreonClient.CharacterEventArg_getSwigType(*args)

    __swig_setmethods__["m_ch"] = _IreonClient.CharacterEventArg_m_ch_set
    __swig_getmethods__["m_ch"] = _IreonClient.CharacterEventArg_m_ch_get
    if _newclass:m_ch = property(_IreonClient.CharacterEventArg_m_ch_get, _IreonClient.CharacterEventArg_m_ch_set)
    def __init__(self, *args): 
        """__init__(self) -> CharacterEventArg"""
        this = _IreonClient.new_CharacterEventArg(*args)
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _IreonClient.delete_CharacterEventArg
    __del__ = lambda self : None;
CharacterEventArg_swigregister = _IreonClient.CharacterEventArg_swigregister
CharacterEventArg_swigregister(CharacterEventArg)

def CharacterEventArg_registerSwigType(*args):
  """CharacterEventArg_registerSwigType(swigTypeInfo typeInfo)"""
  return _IreonClient.CharacterEventArg_registerSwigType(*args)

class CharUpdateEventArg(EventArg):
    """Proxy of C++ CharUpdateEventArg class"""
    __swig_setmethods__ = {}
    for _s in [EventArg]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, CharUpdateEventArg, name, value)
    __swig_getmethods__ = {}
    for _s in [EventArg]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, CharUpdateEventArg, name)
    __repr__ = _swig_repr
    def registerSwigType(*args):
        """registerSwigType(swigTypeInfo typeInfo)"""
        return _IreonClient.CharUpdateEventArg_registerSwigType(*args)

    if _newclass:registerSwigType = staticmethod(registerSwigType)
    __swig_getmethods__["registerSwigType"] = lambda x: registerSwigType
    def getSwigType(*args):
        """getSwigType(self) -> swigTypeInfo"""
        return _IreonClient.CharUpdateEventArg_getSwigType(*args)

    __swig_setmethods__["m_ch"] = _IreonClient.CharUpdateEventArg_m_ch_set
    __swig_getmethods__["m_ch"] = _IreonClient.CharUpdateEventArg_m_ch_get
    if _newclass:m_ch = property(_IreonClient.CharUpdateEventArg_m_ch_get, _IreonClient.CharUpdateEventArg_m_ch_set)
    __swig_setmethods__["m_time"] = _IreonClient.CharUpdateEventArg_m_time_set
    __swig_getmethods__["m_time"] = _IreonClient.CharUpdateEventArg_m_time_get
    if _newclass:m_time = property(_IreonClient.CharUpdateEventArg_m_time_get, _IreonClient.CharUpdateEventArg_m_time_set)
    def __init__(self, *args): 
        """__init__(self) -> CharUpdateEventArg"""
        this = _IreonClient.new_CharUpdateEventArg(*args)
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _IreonClient.delete_CharUpdateEventArg
    __del__ = lambda self : None;
CharUpdateEventArg_swigregister = _IreonClient.CharUpdateEventArg_swigregister
CharUpdateEventArg_swigregister(CharUpdateEventArg)

def CharUpdateEventArg_registerSwigType(*args):
  """CharUpdateEventArg_registerSwigType(swigTypeInfo typeInfo)"""
  return _IreonClient.CharUpdateEventArg_registerSwigType(*args)

class CInterface(KeyListener):
    """Proxy of C++ CInterface class"""
    __swig_setmethods__ = {}
    for _s in [KeyListener]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, CInterface, name, value)
    __swig_getmethods__ = {}
    for _s in [KeyListener]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, CInterface, name)
    def __init__(self): raise AttributeError, "No constructor defined"
    __repr__ = _swig_repr
    LBUTTON = _IreonClient.CInterface_LBUTTON
    RBUTTON = _IreonClient.CInterface_RBUTTON
    MBUTTON = _IreonClient.CInterface_MBUTTON
    __swig_destroy__ = _IreonClient.delete_CInterface
    __del__ = lambda self : None;
    def getGUISystemPtr(*args):
        """getGUISystemPtr(self) -> System"""
        return _IreonClient.CInterface_getGUISystemPtr(*args)

    def getGUIWinManagerPtr(*args):
        """getGUIWinManagerPtr(self) -> WindowManager"""
        return _IreonClient.CInterface_getGUIWinManagerPtr(*args)

    def init(*args):
        """init(self, RenderWindow win) -> bool"""
        return _IreonClient.CInterface_init(*args)

    def initOIS(*args):
        """initOIS(self)"""
        return _IreonClient.CInterface_initOIS(*args)

    def loadCeguiSchemes(*args):
        """loadCeguiSchemes(self)"""
        return _IreonClient.CInterface_loadCeguiSchemes(*args)

    def pulseUpdate(*args):
        """pulseUpdate(self, Real time)"""
        return _IreonClient.CInterface_pulseUpdate(*args)

    def getMouse(*args):
        """getMouse(self) -> OIS::Mouse"""
        return _IreonClient.CInterface_getMouse(*args)

    def getKeyboard(*args):
        """getKeyboard(self) -> Keyboard"""
        return _IreonClient.CInterface_getKeyboard(*args)

    def getRotScale(*args):
        """getRotScale(self) -> Degree"""
        return _IreonClient.CInterface_getRotScale(*args)

    def getPlayerCam(*args):
        """getPlayerCam(self) -> Camera"""
        return _IreonClient.CInterface_getPlayerCam(*args)

    def getRenderWindow(*args):
        """getRenderWindow(self) -> RenderWindow"""
        return _IreonClient.CInterface_getRenderWindow(*args)

    def setRotX(*args):
        """setRotX(self, Real degree)"""
        return _IreonClient.CInterface_setRotX(*args)

    def setRotY(*args):
        """setRotY(self, Real degree)"""
        return _IreonClient.CInterface_setRotY(*args)

    def getKeyboardMap(*args):
        """getKeyboardMap() -> KeyboardMap"""
        return _IreonClient.CInterface_getKeyboardMap(*args)

    if _newclass:getKeyboardMap = staticmethod(getKeyboardMap)
    __swig_getmethods__["getKeyboardMap"] = lambda x: getKeyboardMap
    def setRepeatKeyTime(*args):
        """setRepeatKeyTime(ulong firstRepeatTime, ulong secondRepeatTime)"""
        return _IreonClient.CInterface_setRepeatKeyTime(*args)

    if _newclass:setRepeatKeyTime = staticmethod(setRepeatKeyTime)
    __swig_getmethods__["setRepeatKeyTime"] = lambda x: setRepeatKeyTime
    def getRepeatKeyTime(*args):
        """getRepeatKeyTime(ulong firstRepeatTime, ulong secondRepeatTime)"""
        return _IreonClient.CInterface_getRepeatKeyTime(*args)

    if _newclass:getRepeatKeyTime = staticmethod(getRepeatKeyTime)
    __swig_getmethods__["getRepeatKeyTime"] = lambda x: getRepeatKeyTime
    def getRotateScale(*args):
        """getRotateScale(self) -> Degree"""
        return _IreonClient.CInterface_getRotateScale(*args)

    def getRotateSpeed(*args):
        """getRotateSpeed(self) -> Degree"""
        return _IreonClient.CInterface_getRotateSpeed(*args)

    def setRotateScale(*args):
        """setRotateScale(self, Degree rotateScale)"""
        return _IreonClient.CInterface_setRotateScale(*args)

    def setRotateSpeed(*args):
        """setRotateSpeed(self, Degree rotateSpeed)"""
        return _IreonClient.CInterface_setRotateSpeed(*args)

    def getRotateX(*args):
        """getRotateX(self) -> Radian"""
        return _IreonClient.CInterface_getRotateX(*args)

    def getRotateY(*args):
        """getRotateY(self) -> Radian"""
        return _IreonClient.CInterface_getRotateY(*args)

    def setRotateX(*args):
        """setRotateX(self, Radian rotX)"""
        return _IreonClient.CInterface_setRotateX(*args)

    def setRotateY(*args):
        """setRotateY(self, Radian rotY)"""
        return _IreonClient.CInterface_setRotateY(*args)

    def getRootWindow(*args):
        """getRootWindow(self) -> CEGUI::Window"""
        return _IreonClient.CInterface_getRootWindow(*args)

CInterface_swigregister = _IreonClient.CInterface_swigregister
CInterface_swigregister(CInterface)

def CInterface_getKeyboardMap(*args):
  """CInterface_getKeyboardMap() -> KeyboardMap"""
  return _IreonClient.CInterface_getKeyboardMap(*args)

def CInterface_setRepeatKeyTime(*args):
  """CInterface_setRepeatKeyTime(ulong firstRepeatTime, ulong secondRepeatTime)"""
  return _IreonClient.CInterface_setRepeatKeyTime(*args)

def CInterface_getRepeatKeyTime(*args):
  """CInterface_getRepeatKeyTime(ulong firstRepeatTime, ulong secondRepeatTime)"""
  return _IreonClient.CInterface_getRepeatKeyTime(*args)

class CWorld(_object):
    """Proxy of C++ CWorld class"""
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, CWorld, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, CWorld, name)
    def __init__(self): raise AttributeError, "No constructor defined"
    __repr__ = _swig_repr
    __swig_destroy__ = _IreonClient.delete_CWorld
    __del__ = lambda self : None;
    def heightAt(*args):
        """heightAt(self, Vector3 ?) -> Real"""
        return _IreonClient.CWorld_heightAt(*args)

    def getNormalAt(*args):
        """getNormalAt(self, Real ?, Real ?) -> Vector3"""
        return _IreonClient.CWorld_getNormalAt(*args)

    def getPageIndices(*args):
        """getPageIndices(self, Real ?, Real ?, uint ?, uint ?, bool ?)"""
        return _IreonClient.CWorld_getPageIndices(*args)

    def init(*args):
        """init(self) -> bool"""
        return _IreonClient.CWorld_init(*args)

    def createScene(*args):
        """createScene(self) -> bool"""
        return _IreonClient.CWorld_createScene(*args)

    def update(*args):
        """update(self, Real time)"""
        return _IreonClient.CWorld_update(*args)

    def clear(*args):
        """clear(self)"""
        return _IreonClient.CWorld_clear(*args)

    def sceneManager(*args):
        """sceneManager(self) -> SceneManager"""
        return _IreonClient.CWorld_sceneManager(*args)

    def addMob(*args):
        """addMob(self, ClientCharMobData data) -> CharMobPtr"""
        return _IreonClient.CWorld_addMob(*args)

    def addDead(*args):
        """addDead(self, CharacterPtr ch)"""
        return _IreonClient.CWorld_addDead(*args)

    def removeDead(*args):
        """removeDead(self, uint id)"""
        return _IreonClient.CWorld_removeDead(*args)

    def removeCharacter(*args):
        """removeCharacter(self, uint id)"""
        return _IreonClient.CWorld_removeCharacter(*args)

    def findCharacter(*args):
        """
        findCharacter(self, uint id) -> CharacterPtr
        findCharacter(self, SceneNode node) -> CharacterPtr
        """
        return _IreonClient.CWorld_findCharacter(*args)

    def loaded(*args):
        """loaded(self) -> bool"""
        return _IreonClient.CWorld_loaded(*args)

    def translateTo2d(*args):
        """translateTo2d(self, Vector3 coords) -> Vector2"""
        return _IreonClient.CWorld_translateTo2d(*args)

    def pointIsVisible(*args):
        """pointIsVisible(self, Vector3 coords) -> bool"""
        return _IreonClient.CWorld_pointIsVisible(*args)

    def getFirstChar(*args):
        """getFirstChar(self) -> CharMapIterator"""
        return _IreonClient.CWorld_getFirstChar(*args)

    def getNextChar(*args):
        """getNextChar(self) -> CharMapIterator"""
        return _IreonClient.CWorld_getNextChar(*args)

    def lastChar(*args):
        """lastChar(self, CharMapIterator it) -> bool"""
        return _IreonClient.CWorld_lastChar(*args)

CWorld_swigregister = _IreonClient.CWorld_swigregister
CWorld_swigregister(CWorld)

class CObject(_object):
    """Proxy of C++ CObject class"""
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, CObject, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, CObject, name)
    __repr__ = _swig_repr
    def __init__(self, *args): 
        """__init__(self, String name) -> CObject"""
        this = _IreonClient.new_CObject(*args)
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _IreonClient.delete_CObject
    __del__ = lambda self : None;
    NONE = _IreonClient.CObject_NONE
    PLAYER = _IreonClient.CObject_PLAYER
    OWN_PLAYER = _IreonClient.CObject_OWN_PLAYER
    MOB = _IreonClient.CObject_MOB
    STATIC = _IreonClient.CObject_STATIC
    def getPosition3D(*args):
        """getPosition3D(self) -> Vector3"""
        return _IreonClient.CObject_getPosition3D(*args)

    def setPosition3D(*args):
        """setPosition3D(self, Vector3 pos)"""
        return _IreonClient.CObject_setPosition3D(*args)

    def getPosition2D(*args):
        """getPosition2D(self) -> Vector2"""
        return _IreonClient.CObject_getPosition2D(*args)

    def setPosition2D(*args):
        """setPosition2D(self, Vector2 pos)"""
        return _IreonClient.CObject_setPosition2D(*args)

    def getOrientation(*args):
        """getOrientation(self) -> Quaternion"""
        return _IreonClient.CObject_getOrientation(*args)

    def setOrientation(*args):
        """setOrientation(self, Quaternion q)"""
        return _IreonClient.CObject_setOrientation(*args)

    def pitch(*args):
        """pitch(self, Radian angle)"""
        return _IreonClient.CObject_pitch(*args)

    def yaw(*args):
        """yaw(self, Radian angle)"""
        return _IreonClient.CObject_yaw(*args)

    def roll(*args):
        """roll(self, Radian angle)"""
        return _IreonClient.CObject_roll(*args)

    def rotateAA(*args):
        """rotateAA(self, Vector3 axis, Radian angle)"""
        return _IreonClient.CObject_rotateAA(*args)

    def rotateQ(*args):
        """rotateQ(self, Quaternion q)"""
        return _IreonClient.CObject_rotateQ(*args)

    def getUniqueName(*args):
        """getUniqueName(char base) -> String"""
        return _IreonClient.CObject_getUniqueName(*args)

    if _newclass:getUniqueName = staticmethod(getUniqueName)
    __swig_getmethods__["getUniqueName"] = lambda x: getUniqueName
    def getName(*args):
        """getName(self) -> char"""
        return _IreonClient.CObject_getName(*args)

    def setName(*args):
        """setName(self, char name)"""
        return _IreonClient.CObject_setName(*args)

    def getType(*args):
        """getType(self) -> int"""
        return _IreonClient.CObject_getType(*args)

    def getSceneNode(*args):
        """getSceneNode(self) -> SceneNode"""
        return _IreonClient.CObject_getSceneNode(*args)

    def getEntity(*args):
        """getEntity(self) -> Entity"""
        return _IreonClient.CObject_getEntity(*args)

    def update(*args):
        """update(self, Real ?)"""
        return _IreonClient.CObject_update(*args)

    def visible(*args):
        """visible(self) -> bool"""
        return _IreonClient.CObject_visible(*args)

CObject_swigregister = _IreonClient.CObject_swigregister
CObject_swigregister(CObject)

def CObject_getUniqueName(*args):
  """CObject_getUniqueName(char base) -> String"""
  return _IreonClient.CObject_getUniqueName(*args)

class WayPoint(_object):
    """Proxy of C++ WayPoint class"""
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, WayPoint, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, WayPoint, name)
    __repr__ = _swig_repr
    def __init__(self, *args): 
        """__init__(self, int64 pulse, Vector2 pos, Radian rotation) -> WayPoint"""
        this = _IreonClient.new_WayPoint(*args)
        try: self.this.append(this)
        except: self.this = this
    __swig_setmethods__["m_pulse"] = _IreonClient.WayPoint_m_pulse_set
    __swig_getmethods__["m_pulse"] = _IreonClient.WayPoint_m_pulse_get
    if _newclass:m_pulse = property(_IreonClient.WayPoint_m_pulse_get, _IreonClient.WayPoint_m_pulse_set)
    __swig_setmethods__["m_pos"] = _IreonClient.WayPoint_m_pos_set
    __swig_getmethods__["m_pos"] = _IreonClient.WayPoint_m_pos_get
    if _newclass:m_pos = property(_IreonClient.WayPoint_m_pos_get, _IreonClient.WayPoint_m_pos_set)
    __swig_setmethods__["m_rotation"] = _IreonClient.WayPoint_m_rotation_set
    __swig_getmethods__["m_rotation"] = _IreonClient.WayPoint_m_rotation_get
    if _newclass:m_rotation = property(_IreonClient.WayPoint_m_rotation_get, _IreonClient.WayPoint_m_rotation_set)
    __swig_destroy__ = _IreonClient.delete_WayPoint
    __del__ = lambda self : None;
WayPoint_swigregister = _IreonClient.WayPoint_swigregister
WayPoint_swigregister(WayPoint)

class CDynamicObject(CObject):
    """Proxy of C++ CDynamicObject class"""
    __swig_setmethods__ = {}
    for _s in [CObject]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, CDynamicObject, name, value)
    __swig_getmethods__ = {}
    for _s in [CObject]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, CDynamicObject, name)
    __repr__ = _swig_repr
    def __init__(self, *args): 
        """__init__(self, CPrototype proto) -> CDynamicObject"""
        this = _IreonClient.new_CDynamicObject(*args)
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _IreonClient.delete_CDynamicObject
    __del__ = lambda self : None;
    def productCallback(*args):
        """productCallback(CPrototype proto) -> CDynamicObject"""
        return _IreonClient.CDynamicObject_productCallback(*args)

    if _newclass:productCallback = staticmethod(productCallback)
    __swig_getmethods__["productCallback"] = lambda x: productCallback
    def getId(*args):
        """getId(self) -> uint"""
        return _IreonClient.CDynamicObject_getId(*args)

    def update(*args):
        """update(self, Real time)"""
        return _IreonClient.CDynamicObject_update(*args)

    def visible(*args):
        """visible(self) -> bool"""
        return _IreonClient.CDynamicObject_visible(*args)

    def lookAt(*args):
        """lookAt(self, Vector2 to)"""
        return _IreonClient.CDynamicObject_lookAt(*args)

    def jump(*args):
        """jump(self)"""
        return _IreonClient.CDynamicObject_jump(*args)

    def canWalk(*args):
        """canWalk(self, Vector3 to, Vector2 norm) -> bool"""
        return _IreonClient.CDynamicObject_canWalk(*args)

    def walk(*args):
        """
        walk(self, Real time, Vector2 direction=None)
        walk(self, Real time)
        """
        return _IreonClient.CDynamicObject_walk(*args)

    def isWalking(*args):
        """isWalking(self) -> bool"""
        return _IreonClient.CDynamicObject_isWalking(*args)

    def walkAnimStep(*args):
        """
        walkAnimStep(self, Real time, bool toStand=False)
        walkAnimStep(self, Real time)
        """
        return _IreonClient.CDynamicObject_walkAnimStep(*args)

    def getWalkSpeed(*args):
        """getWalkSpeed(self) -> Real"""
        return _IreonClient.CDynamicObject_getWalkSpeed(*args)

    def setWalkSpeed(*args):
        """setWalkSpeed(self, Real speed)"""
        return _IreonClient.CDynamicObject_setWalkSpeed(*args)

    def startFight(*args):
        """startFight(self, uint enemy, bool closeCombat)"""
        return _IreonClient.CDynamicObject_startFight(*args)

    def stopFight(*args):
        """stopFight(self)"""
        return _IreonClient.CDynamicObject_stopFight(*args)

    def getFightSpeed(*args):
        """getFightSpeed(self) -> byte"""
        return _IreonClient.CDynamicObject_getFightSpeed(*args)

    def die(*args):
        """die(self)"""
        return _IreonClient.CDynamicObject_die(*args)

    def isDead(*args):
        """isDead(self) -> bool"""
        return _IreonClient.CDynamicObject_isDead(*args)

    def getAnimStandPos(*args):
        """getAnimStandPos(self) -> Real"""
        return _IreonClient.CDynamicObject_getAnimStandPos(*args)

    def addWayPoint(*args):
        """addWayPoint(self, int64 pulse, Vector2 p, Radian rotation)"""
        return _IreonClient.CDynamicObject_addWayPoint(*args)

    def clearWayPoints(*args):
        """clearWayPoints(self)"""
        return _IreonClient.CDynamicObject_clearWayPoints(*args)

    def setRotation(*args):
        """setRotation(self, Radian rot)"""
        return _IreonClient.CDynamicObject_setRotation(*args)

    def getRotation(*args):
        """getRotation(self) -> Radian"""
        return _IreonClient.CDynamicObject_getRotation(*args)

    def getPercentHP(*args):
        """getPercentHP(self) -> byte"""
        return _IreonClient.CDynamicObject_getPercentHP(*args)

    def setPercentHP(*args):
        """setPercentHP(self, byte percent)"""
        return _IreonClient.CDynamicObject_setPercentHP(*args)

    def getEnemy(*args):
        """getEnemy(self) -> CDynamicObject"""
        return _IreonClient.CDynamicObject_getEnemy(*args)

CDynamicObject_swigregister = _IreonClient.CDynamicObject_swigregister
CDynamicObject_swigregister(CDynamicObject)

def CDynamicObject_productCallback(*args):
  """CDynamicObject_productCallback(CPrototype proto) -> CDynamicObject"""
  return _IreonClient.CDynamicObject_productCallback(*args)

class CCharacter(CDynamicObject):
    """Proxy of C++ CCharacter class"""
    __swig_setmethods__ = {}
    for _s in [CDynamicObject]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, CCharacter, name, value)
    __swig_getmethods__ = {}
    for _s in [CDynamicObject]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, CCharacter, name)
    __repr__ = _swig_repr
    def __init__(self, *args): 
        """__init__(self, CPrototype proto) -> CCharacter"""
        this = _IreonClient.new_CCharacter(*args)
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _IreonClient.delete_CCharacter
    __del__ = lambda self : None;
    def productCallback(*args):
        """productCallback(CPrototype proto) -> CCharacter"""
        return _IreonClient.CCharacter_productCallback(*args)

    if _newclass:productCallback = staticmethod(productCallback)
    __swig_getmethods__["productCallback"] = lambda x: productCallback
    def update(*args):
        """update(self, Real time)"""
        return _IreonClient.CCharacter_update(*args)

CCharacter_swigregister = _IreonClient.CCharacter_swigregister
CCharacter_swigregister(CCharacter)

def CCharacter_productCallback(*args):
  """CCharacter_productCallback(CPrototype proto) -> CCharacter"""
  return _IreonClient.CCharacter_productCallback(*args)

class COwnPlayer(CCharacter):
    """Proxy of C++ COwnPlayer class"""
    __swig_setmethods__ = {}
    for _s in [CCharacter]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, COwnPlayer, name, value)
    __swig_getmethods__ = {}
    for _s in [CCharacter]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, COwnPlayer, name)
    __repr__ = _swig_repr
    def __init__(self, *args): 
        """__init__(self, CPrototype proto) -> COwnPlayer"""
        this = _IreonClient.new_COwnPlayer(*args)
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _IreonClient.delete_COwnPlayer
    __del__ = lambda self : None;
    def setDifference(*args):
        """setDifference(self, Vector2 dif)"""
        return _IreonClient.COwnPlayer_setDifference(*args)

    def getDifference(*args):
        """getDifference(self) -> Vector2"""
        return _IreonClient.COwnPlayer_getDifference(*args)

    def update(*args):
        """update(self, Real time)"""
        return _IreonClient.COwnPlayer_update(*args)

    def setPosition3D(*args):
        """setPosition3D(self, Vector3 position)"""
        return _IreonClient.COwnPlayer_setPosition3D(*args)

    def setPosition2D(*args):
        """setPosition2D(self, Vector2 position)"""
        return _IreonClient.COwnPlayer_setPosition2D(*args)

    def walk(*args):
        """
        walk(self, Real time, Vector2 direction=0)
        walk(self, Real time)
        """
        return _IreonClient.COwnPlayer_walk(*args)

    def rotate(*args):
        """rotate(self, Degree degree)"""
        return _IreonClient.COwnPlayer_rotate(*args)

COwnPlayer_swigregister = _IreonClient.COwnPlayer_swigregister
COwnPlayer_swigregister(COwnPlayer)

class CMob(CDynamicObject):
    """Proxy of C++ CMob class"""
    __swig_setmethods__ = {}
    for _s in [CDynamicObject]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, CMob, name, value)
    __swig_getmethods__ = {}
    for _s in [CDynamicObject]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, CMob, name)
    __repr__ = _swig_repr
    def __init__(self, *args): 
        """__init__(self, CMobPrototype proto) -> CMob"""
        this = _IreonClient.new_CMob(*args)
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _IreonClient.delete_CMob
    __del__ = lambda self : None;
    def productCallback(*args):
        """productCallback(CMobPrototype proto) -> CMob"""
        return _IreonClient.CMob_productCallback(*args)

    if _newclass:productCallback = staticmethod(productCallback)
    __swig_getmethods__["productCallback"] = lambda x: productCallback
CMob_swigregister = _IreonClient.CMob_swigregister
CMob_swigregister(CMob)

def CMob_productCallback(*args):
  """CMob_productCallback(CMobPrototype proto) -> CMob"""
  return _IreonClient.CMob_productCallback(*args)

class CStaticObject(CObject):
    """Proxy of C++ CStaticObject class"""
    __swig_setmethods__ = {}
    for _s in [CObject]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, CStaticObject, name, value)
    __swig_getmethods__ = {}
    for _s in [CObject]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, CStaticObject, name)
    __repr__ = _swig_repr
    def __init__(self, *args): 
        """__init__(self, CStaticPrototype proto) -> CStaticObject"""
        this = _IreonClient.new_CStaticObject(*args)
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _IreonClient.delete_CStaticObject
    __del__ = lambda self : None;
    def productCallback(*args):
        """productCallback(CStaticPrototype proto) -> CStaticObject"""
        return _IreonClient.CStaticObject_productCallback(*args)

    if _newclass:productCallback = staticmethod(productCallback)
    __swig_getmethods__["productCallback"] = lambda x: productCallback
    def load(*args):
        """load(self) -> bool"""
        return _IreonClient.CStaticObject_load(*args)

    def unload(*args):
        """unload(self) -> bool"""
        return _IreonClient.CStaticObject_unload(*args)

    def setRotation(*args):
        """setRotation(self, Radian q)"""
        return _IreonClient.CStaticObject_setRotation(*args)

    def getRotation(*args):
        """getRotation(self) -> Radian"""
        return _IreonClient.CStaticObject_getRotation(*args)

    def pitch(*args):
        """pitch(self, Radian rot)"""
        return _IreonClient.CStaticObject_pitch(*args)

    def getPitch(*args):
        """getPitch(self) -> Radian"""
        return _IreonClient.CStaticObject_getPitch(*args)

    def roll(*args):
        """roll(self, Radian rot)"""
        return _IreonClient.CStaticObject_roll(*args)

    def getRoll(*args):
        """getRoll(self) -> Radian"""
        return _IreonClient.CStaticObject_getRoll(*args)

    def yaw(*args):
        """yaw(self, Radian rot)"""
        return _IreonClient.CStaticObject_yaw(*args)

    def setPosition3D(*args):
        """setPosition3D(self, Vector3 pos)"""
        return _IreonClient.CStaticObject_setPosition3D(*args)

    def getPosition3D(*args):
        """getPosition3D(self) -> Vector3"""
        return _IreonClient.CStaticObject_getPosition3D(*args)

    def setPosition2D(*args):
        """setPosition2D(self, Vector2 pos)"""
        return _IreonClient.CStaticObject_setPosition2D(*args)

    def isLoaded(*args):
        """isLoaded(self) -> bool"""
        return _IreonClient.CStaticObject_isLoaded(*args)

    def getPage(*args):
        """getPage(self) -> std::pair<(uint,uint)>"""
        return _IreonClient.CStaticObject_getPage(*args)

    def getSegments(*args):
        """getSegments(self) -> std::vector<(CSegment,std::allocator<(CSegment)>)>"""
        return _IreonClient.CStaticObject_getSegments(*args)

    def canWalk(*args):
        """canWalk(self, Vector2 from, Vector2 to, Vector2 norm) -> bool"""
        return _IreonClient.CStaticObject_canWalk(*args)

CStaticObject_swigregister = _IreonClient.CStaticObject_swigregister
CStaticObject_swigregister(CStaticObject)

def CStaticObject_productCallback(*args):
  """CStaticObject_productCallback(CStaticPrototype proto) -> CStaticObject"""
  return _IreonClient.CStaticObject_productCallback(*args)

class CStaticPrototype(_object):
    """Proxy of C++ CStaticPrototype class"""
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, CStaticPrototype, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, CStaticPrototype, name)
    __repr__ = _swig_repr
    def __init__(self, *args): 
        """__init__(self) -> CStaticPrototype"""
        this = _IreonClient.new_CStaticPrototype(*args)
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _IreonClient.delete_CStaticPrototype
    __del__ = lambda self : None;
    def setMeshFileFullPath(*args):
        """setMeshFileFullPath(self, String meshFileFullPath)"""
        return _IreonClient.CStaticPrototype_setMeshFileFullPath(*args)

    def getMeshFileFullPath(*args):
        """getMeshFileFullPath(self) -> String"""
        return _IreonClient.CStaticPrototype_getMeshFileFullPath(*args)

    def setMaterialFileFullPath(*args):
        """setMaterialFileFullPath(self, String meshFileFullPath)"""
        return _IreonClient.CStaticPrototype_setMaterialFileFullPath(*args)

    def getMaterialFileFullPath(*args):
        """getMaterialFileFullPath(self) -> String"""
        return _IreonClient.CStaticPrototype_getMaterialFileFullPath(*args)

CStaticPrototype_swigregister = _IreonClient.CStaticPrototype_swigregister
CStaticPrototype_swigregister(CStaticPrototype)

class CPrototypeManager(_object):
    """Proxy of C++ CPrototypeManager class"""
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, CPrototypeManager, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, CPrototypeManager, name)
    __repr__ = _swig_repr
    def loadStaticPrototypes(*args):
        """loadStaticPrototypes(self) -> bool"""
        return _IreonClient.CPrototypeManager_loadStaticPrototypes(*args)

    def loadOwnPlayerPrototypes(*args):
        """loadOwnPlayerPrototypes(self) -> bool"""
        return _IreonClient.CPrototypeManager_loadOwnPlayerPrototypes(*args)

    def loadCharacterPrototypes(*args):
        """loadCharacterPrototypes(self) -> bool"""
        return _IreonClient.CPrototypeManager_loadCharacterPrototypes(*args)

    def loadMobPrototypes(*args):
        """loadMobPrototypes(self) -> bool"""
        return _IreonClient.CPrototypeManager_loadMobPrototypes(*args)

    def __init__(self, *args): 
        """__init__(self) -> CPrototypeManager"""
        this = _IreonClient.new_CPrototypeManager(*args)
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _IreonClient.delete_CPrototypeManager
    __del__ = lambda self : None;
CPrototypeManager_swigregister = _IreonClient.CPrototypeManager_swigregister
CPrototypeManager_swigregister(CPrototypeManager)

class IntVector(_object):
    """Proxy of C++ IntVector class"""
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, IntVector, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, IntVector, name)
    __repr__ = _swig_repr
    def iterator(*args):
        """iterator(self, PyObject PYTHON_SELF) -> PySwigIterator"""
        return _IreonClient.IntVector_iterator(*args)

    def __iter__(self): return self.iterator()
    def __nonzero__(*args):
        """__nonzero__(self) -> bool"""
        return _IreonClient.IntVector___nonzero__(*args)

    def __len__(*args):
        """__len__(self) -> size_type"""
        return _IreonClient.IntVector___len__(*args)

    def pop(*args):
        """pop(self) -> value_type"""
        return _IreonClient.IntVector_pop(*args)

    def __getslice__(*args):
        """__getslice__(self, difference_type i, difference_type j) -> IntVector"""
        return _IreonClient.IntVector___getslice__(*args)

    def __setslice__(*args):
        """__setslice__(self, difference_type i, difference_type j, IntVector v)"""
        return _IreonClient.IntVector___setslice__(*args)

    def __delslice__(*args):
        """__delslice__(self, difference_type i, difference_type j)"""
        return _IreonClient.IntVector___delslice__(*args)

    def __delitem__(*args):
        """__delitem__(self, difference_type i)"""
        return _IreonClient.IntVector___delitem__(*args)

    def __getitem__(*args):
        """__getitem__(self, difference_type i) -> value_type"""
        return _IreonClient.IntVector___getitem__(*args)

    def __setitem__(*args):
        """__setitem__(self, difference_type i, value_type x)"""
        return _IreonClient.IntVector___setitem__(*args)

    def append(*args):
        """append(self, value_type x)"""
        return _IreonClient.IntVector_append(*args)

    def empty(*args):
        """empty(self) -> bool"""
        return _IreonClient.IntVector_empty(*args)

    def size(*args):
        """size(self) -> size_type"""
        return _IreonClient.IntVector_size(*args)

    def clear(*args):
        """clear(self)"""
        return _IreonClient.IntVector_clear(*args)

    def swap(*args):
        """swap(self, IntVector v)"""
        return _IreonClient.IntVector_swap(*args)

    def get_allocator(*args):
        """get_allocator(self) -> allocator_type"""
        return _IreonClient.IntVector_get_allocator(*args)

    def begin(*args):
        """
        begin(self) -> iterator
        begin(self) -> const_iterator
        """
        return _IreonClient.IntVector_begin(*args)

    def end(*args):
        """
        end(self) -> iterator
        end(self) -> const_iterator
        """
        return _IreonClient.IntVector_end(*args)

    def rbegin(*args):
        """
        rbegin(self) -> reverse_iterator
        rbegin(self) -> const_reverse_iterator
        """
        return _IreonClient.IntVector_rbegin(*args)

    def rend(*args):
        """
        rend(self) -> reverse_iterator
        rend(self) -> const_reverse_iterator
        """
        return _IreonClient.IntVector_rend(*args)

    def pop_back(*args):
        """pop_back(self)"""
        return _IreonClient.IntVector_pop_back(*args)

    def erase(*args):
        """
        erase(self, iterator pos) -> iterator
        erase(self, iterator first, iterator last) -> iterator
        """
        return _IreonClient.IntVector_erase(*args)

    def __init__(self, *args): 
        """
        __init__(self) -> IntVector
        __init__(self, IntVector ?) -> IntVector
        __init__(self, size_type size) -> IntVector
        __init__(self, size_type size, value_type value) -> IntVector
        """
        this = _IreonClient.new_IntVector(*args)
        try: self.this.append(this)
        except: self.this = this
    def push_back(*args):
        """push_back(self, value_type x)"""
        return _IreonClient.IntVector_push_back(*args)

    def front(*args):
        """front(self) -> value_type"""
        return _IreonClient.IntVector_front(*args)

    def back(*args):
        """back(self) -> value_type"""
        return _IreonClient.IntVector_back(*args)

    def assign(*args):
        """assign(self, size_type n, value_type x)"""
        return _IreonClient.IntVector_assign(*args)

    def resize(*args):
        """
        resize(self, size_type new_size)
        resize(self, size_type new_size, value_type x)
        """
        return _IreonClient.IntVector_resize(*args)

    def insert(*args):
        """
        insert(self, iterator pos, value_type x) -> iterator
        insert(self, iterator pos, size_type n, value_type x)
        """
        return _IreonClient.IntVector_insert(*args)

    def reserve(*args):
        """reserve(self, size_type n)"""
        return _IreonClient.IntVector_reserve(*args)

    def capacity(*args):
        """capacity(self) -> size_type"""
        return _IreonClient.IntVector_capacity(*args)

    __swig_destroy__ = _IreonClient.delete_IntVector
    __del__ = lambda self : None;
IntVector_swigregister = _IreonClient.IntVector_swigregister
IntVector_swigregister(IntVector)

class DoubleVector(_object):
    """Proxy of C++ DoubleVector class"""
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, DoubleVector, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, DoubleVector, name)
    __repr__ = _swig_repr
    def iterator(*args):
        """iterator(self, PyObject PYTHON_SELF) -> PySwigIterator"""
        return _IreonClient.DoubleVector_iterator(*args)

    def __iter__(self): return self.iterator()
    def __nonzero__(*args):
        """__nonzero__(self) -> bool"""
        return _IreonClient.DoubleVector___nonzero__(*args)

    def __len__(*args):
        """__len__(self) -> size_type"""
        return _IreonClient.DoubleVector___len__(*args)

    def pop(*args):
        """pop(self) -> value_type"""
        return _IreonClient.DoubleVector_pop(*args)

    def __getslice__(*args):
        """__getslice__(self, difference_type i, difference_type j) -> DoubleVector"""
        return _IreonClient.DoubleVector___getslice__(*args)

    def __setslice__(*args):
        """__setslice__(self, difference_type i, difference_type j, DoubleVector v)"""
        return _IreonClient.DoubleVector___setslice__(*args)

    def __delslice__(*args):
        """__delslice__(self, difference_type i, difference_type j)"""
        return _IreonClient.DoubleVector___delslice__(*args)

    def __delitem__(*args):
        """__delitem__(self, difference_type i)"""
        return _IreonClient.DoubleVector___delitem__(*args)

    def __getitem__(*args):
        """__getitem__(self, difference_type i) -> value_type"""
        return _IreonClient.DoubleVector___getitem__(*args)

    def __setitem__(*args):
        """__setitem__(self, difference_type i, value_type x)"""
        return _IreonClient.DoubleVector___setitem__(*args)

    def append(*args):
        """append(self, value_type x)"""
        return _IreonClient.DoubleVector_append(*args)

    def empty(*args):
        """empty(self) -> bool"""
        return _IreonClient.DoubleVector_empty(*args)

    def size(*args):
        """size(self) -> size_type"""
        return _IreonClient.DoubleVector_size(*args)

    def clear(*args):
        """clear(self)"""
        return _IreonClient.DoubleVector_clear(*args)

    def swap(*args):
        """swap(self, DoubleVector v)"""
        return _IreonClient.DoubleVector_swap(*args)

    def get_allocator(*args):
        """get_allocator(self) -> allocator_type"""
        return _IreonClient.DoubleVector_get_allocator(*args)

    def begin(*args):
        """
        begin(self) -> iterator
        begin(self) -> const_iterator
        """
        return _IreonClient.DoubleVector_begin(*args)

    def end(*args):
        """
        end(self) -> iterator
        end(self) -> const_iterator
        """
        return _IreonClient.DoubleVector_end(*args)

    def rbegin(*args):
        """
        rbegin(self) -> reverse_iterator
        rbegin(self) -> const_reverse_iterator
        """
        return _IreonClient.DoubleVector_rbegin(*args)

    def rend(*args):
        """
        rend(self) -> reverse_iterator
        rend(self) -> const_reverse_iterator
        """
        return _IreonClient.DoubleVector_rend(*args)

    def pop_back(*args):
        """pop_back(self)"""
        return _IreonClient.DoubleVector_pop_back(*args)

    def erase(*args):
        """
        erase(self, iterator pos) -> iterator
        erase(self, iterator first, iterator last) -> iterator
        """
        return _IreonClient.DoubleVector_erase(*args)

    def __init__(self, *args): 
        """
        __init__(self) -> DoubleVector
        __init__(self, DoubleVector ?) -> DoubleVector
        __init__(self, size_type size) -> DoubleVector
        __init__(self, size_type size, value_type value) -> DoubleVector
        """
        this = _IreonClient.new_DoubleVector(*args)
        try: self.this.append(this)
        except: self.this = this
    def push_back(*args):
        """push_back(self, value_type x)"""
        return _IreonClient.DoubleVector_push_back(*args)

    def front(*args):
        """front(self) -> value_type"""
        return _IreonClient.DoubleVector_front(*args)

    def back(*args):
        """back(self) -> value_type"""
        return _IreonClient.DoubleVector_back(*args)

    def assign(*args):
        """assign(self, size_type n, value_type x)"""
        return _IreonClient.DoubleVector_assign(*args)

    def resize(*args):
        """
        resize(self, size_type new_size)
        resize(self, size_type new_size, value_type x)
        """
        return _IreonClient.DoubleVector_resize(*args)

    def insert(*args):
        """
        insert(self, iterator pos, value_type x) -> iterator
        insert(self, iterator pos, size_type n, value_type x)
        """
        return _IreonClient.DoubleVector_insert(*args)

    def reserve(*args):
        """reserve(self, size_type n)"""
        return _IreonClient.DoubleVector_reserve(*args)

    def capacity(*args):
        """capacity(self) -> size_type"""
        return _IreonClient.DoubleVector_capacity(*args)

    __swig_destroy__ = _IreonClient.delete_DoubleVector
    __del__ = lambda self : None;
DoubleVector_swigregister = _IreonClient.DoubleVector_swigregister
DoubleVector_swigregister(DoubleVector)

class ByteVector(_object):
    """Proxy of C++ ByteVector class"""
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, ByteVector, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, ByteVector, name)
    __repr__ = _swig_repr
    def iterator(*args):
        """iterator(self, PyObject PYTHON_SELF) -> PySwigIterator"""
        return _IreonClient.ByteVector_iterator(*args)

    def __iter__(self): return self.iterator()
    def __nonzero__(*args):
        """__nonzero__(self) -> bool"""
        return _IreonClient.ByteVector___nonzero__(*args)

    def __len__(*args):
        """__len__(self) -> std::vector<(unsigned char)>::size_type"""
        return _IreonClient.ByteVector___len__(*args)

    def pop(*args):
        """pop(self) -> std::vector<(unsigned char)>::value_type"""
        return _IreonClient.ByteVector_pop(*args)

    def __getslice__(*args):
        """__getslice__(self, std::vector<(unsigned char)>::difference_type i, std::vector<(unsigned char)>::difference_type j) -> ByteVector"""
        return _IreonClient.ByteVector___getslice__(*args)

    def __setslice__(*args):
        """
        __setslice__(self, std::vector<(unsigned char)>::difference_type i, std::vector<(unsigned char)>::difference_type j, 
            ByteVector v)
        """
        return _IreonClient.ByteVector___setslice__(*args)

    def __delslice__(*args):
        """__delslice__(self, std::vector<(unsigned char)>::difference_type i, std::vector<(unsigned char)>::difference_type j)"""
        return _IreonClient.ByteVector___delslice__(*args)

    def __delitem__(*args):
        """__delitem__(self, std::vector<(unsigned char)>::difference_type i)"""
        return _IreonClient.ByteVector___delitem__(*args)

    def __getitem__(*args):
        """__getitem__(self, std::vector<(unsigned char)>::difference_type i) -> std::vector<(unsigned char)>::value_type"""
        return _IreonClient.ByteVector___getitem__(*args)

    def __setitem__(*args):
        """__setitem__(self, std::vector<(unsigned char)>::difference_type i, std::vector<(unsigned char)>::value_type x)"""
        return _IreonClient.ByteVector___setitem__(*args)

    def append(*args):
        """append(self, std::vector<(unsigned char)>::value_type x)"""
        return _IreonClient.ByteVector_append(*args)

    def empty(*args):
        """empty(self) -> bool"""
        return _IreonClient.ByteVector_empty(*args)

    def size(*args):
        """size(self) -> std::vector<(unsigned char)>::size_type"""
        return _IreonClient.ByteVector_size(*args)

    def clear(*args):
        """clear(self)"""
        return _IreonClient.ByteVector_clear(*args)

    def swap(*args):
        """swap(self, ByteVector v)"""
        return _IreonClient.ByteVector_swap(*args)

    def get_allocator(*args):
        """get_allocator(self) -> std::vector<(unsigned char)>::allocator_type"""
        return _IreonClient.ByteVector_get_allocator(*args)

    def begin(*args):
        """
        begin(self) -> std::vector<(unsigned char)>::iterator
        begin(self) -> std::vector<(unsigned char)>::const_iterator
        """
        return _IreonClient.ByteVector_begin(*args)

    def end(*args):
        """
        end(self) -> std::vector<(unsigned char)>::iterator
        end(self) -> std::vector<(unsigned char)>::const_iterator
        """
        return _IreonClient.ByteVector_end(*args)

    def rbegin(*args):
        """
        rbegin(self) -> std::vector<(unsigned char)>::reverse_iterator
        rbegin(self) -> std::vector<(unsigned char)>::const_reverse_iterator
        """
        return _IreonClient.ByteVector_rbegin(*args)

    def rend(*args):
        """
        rend(self) -> std::vector<(unsigned char)>::reverse_iterator
        rend(self) -> std::vector<(unsigned char)>::const_reverse_iterator
        """
        return _IreonClient.ByteVector_rend(*args)

    def pop_back(*args):
        """pop_back(self)"""
        return _IreonClient.ByteVector_pop_back(*args)

    def erase(*args):
        """
        erase(self, std::vector<(unsigned char)>::iterator pos) -> std::vector<(unsigned char)>::iterator
        erase(self, std::vector<(unsigned char)>::iterator first, std::vector<(unsigned char)>::iterator last) -> std::vector<(unsigned char)>::iterator
        """
        return _IreonClient.ByteVector_erase(*args)

    def __init__(self, *args): 
        """
        __init__(self) -> ByteVector
        __init__(self, ByteVector ?) -> ByteVector
        __init__(self, std::vector<(unsigned char)>::size_type size) -> ByteVector
        __init__(self, std::vector<(unsigned char)>::size_type size, std::vector<(unsigned char)>::value_type value) -> ByteVector
        """
        this = _IreonClient.new_ByteVector(*args)
        try: self.this.append(this)
        except: self.this = this
    def push_back(*args):
        """push_back(self, std::vector<(unsigned char)>::value_type x)"""
        return _IreonClient.ByteVector_push_back(*args)

    def front(*args):
        """front(self) -> std::vector<(unsigned char)>::value_type"""
        return _IreonClient.ByteVector_front(*args)

    def back(*args):
        """back(self) -> std::vector<(unsigned char)>::value_type"""
        return _IreonClient.ByteVector_back(*args)

    def assign(*args):
        """assign(self, std::vector<(unsigned char)>::size_type n, std::vector<(unsigned char)>::value_type x)"""
        return _IreonClient.ByteVector_assign(*args)

    def resize(*args):
        """
        resize(self, std::vector<(unsigned char)>::size_type new_size)
        resize(self, std::vector<(unsigned char)>::size_type new_size, std::vector<(unsigned char)>::value_type x)
        """
        return _IreonClient.ByteVector_resize(*args)

    def insert(*args):
        """
        insert(self, std::vector<(unsigned char)>::iterator pos, std::vector<(unsigned char)>::value_type x) -> std::vector<(unsigned char)>::iterator
        insert(self, std::vector<(unsigned char)>::iterator pos, std::vector<(unsigned char)>::size_type n, 
            std::vector<(unsigned char)>::value_type x)
        """
        return _IreonClient.ByteVector_insert(*args)

    def reserve(*args):
        """reserve(self, std::vector<(unsigned char)>::size_type n)"""
        return _IreonClient.ByteVector_reserve(*args)

    def capacity(*args):
        """capacity(self) -> std::vector<(unsigned char)>::size_type"""
        return _IreonClient.ByteVector_capacity(*args)

    __swig_destroy__ = _IreonClient.delete_ByteVector
    __del__ = lambda self : None;
ByteVector_swigregister = _IreonClient.ByteVector_swigregister
ByteVector_swigregister(ByteVector)

class ClientCharData(_object):
    """Proxy of C++ ClientCharData class"""
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, ClientCharData, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, ClientCharData, name)
    __repr__ = _swig_repr
    __swig_setmethods__["m_velocity"] = _IreonClient.ClientCharData_m_velocity_set
    __swig_getmethods__["m_velocity"] = _IreonClient.ClientCharData_m_velocity_get
    if _newclass:m_velocity = property(_IreonClient.ClientCharData_m_velocity_get, _IreonClient.ClientCharData_m_velocity_set)
    __swig_setmethods__["m_fightSpeed"] = _IreonClient.ClientCharData_m_fightSpeed_set
    __swig_getmethods__["m_fightSpeed"] = _IreonClient.ClientCharData_m_fightSpeed_get
    if _newclass:m_fightSpeed = property(_IreonClient.ClientCharData_m_fightSpeed_get, _IreonClient.ClientCharData_m_fightSpeed_set)
    __swig_setmethods__["m_id"] = _IreonClient.ClientCharData_m_id_set
    __swig_getmethods__["m_id"] = _IreonClient.ClientCharData_m_id_get
    if _newclass:m_id = property(_IreonClient.ClientCharData_m_id_get, _IreonClient.ClientCharData_m_id_set)
    __swig_setmethods__["m_percentHP"] = _IreonClient.ClientCharData_m_percentHP_set
    __swig_getmethods__["m_percentHP"] = _IreonClient.ClientCharData_m_percentHP_get
    if _newclass:m_percentHP = property(_IreonClient.ClientCharData_m_percentHP_get, _IreonClient.ClientCharData_m_percentHP_set)
    def __init__(self, *args): 
        """__init__(self) -> ClientCharData"""
        this = _IreonClient.new_ClientCharData(*args)
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _IreonClient.delete_ClientCharData
    __del__ = lambda self : None;
ClientCharData_swigregister = _IreonClient.ClientCharData_swigregister
ClientCharData_swigregister(ClientCharData)

class ClientCharPlayerData(ClientCharData):
    """Proxy of C++ ClientCharPlayerData class"""
    __swig_setmethods__ = {}
    for _s in [ClientCharData]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, ClientCharPlayerData, name, value)
    __swig_getmethods__ = {}
    for _s in [ClientCharData]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, ClientCharPlayerData, name)
    __repr__ = _swig_repr
    __swig_setmethods__["m_name"] = _IreonClient.ClientCharPlayerData_m_name_set
    __swig_getmethods__["m_name"] = _IreonClient.ClientCharPlayerData_m_name_get
    if _newclass:m_name = property(_IreonClient.ClientCharPlayerData_m_name_get, _IreonClient.ClientCharPlayerData_m_name_set)
    def __init__(self, *args): 
        """__init__(self) -> ClientCharPlayerData"""
        this = _IreonClient.new_ClientCharPlayerData(*args)
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _IreonClient.delete_ClientCharPlayerData
    __del__ = lambda self : None;
ClientCharPlayerData_swigregister = _IreonClient.ClientCharPlayerData_swigregister
ClientCharPlayerData_swigregister(ClientCharPlayerData)

class ClientOwnCharData(ClientCharPlayerData):
    """Proxy of C++ ClientOwnCharData class"""
    __swig_setmethods__ = {}
    for _s in [ClientCharPlayerData]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, ClientOwnCharData, name, value)
    __swig_getmethods__ = {}
    for _s in [ClientCharPlayerData]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, ClientOwnCharData, name)
    __repr__ = _swig_repr
    def __eq__(*args):
        """__eq__(self, ClientOwnCharData d) -> bool"""
        return _IreonClient.ClientOwnCharData___eq__(*args)

    __swig_setmethods__["m_HP"] = _IreonClient.ClientOwnCharData_m_HP_set
    __swig_getmethods__["m_HP"] = _IreonClient.ClientOwnCharData_m_HP_get
    if _newclass:m_HP = property(_IreonClient.ClientOwnCharData_m_HP_get, _IreonClient.ClientOwnCharData_m_HP_set)
    __swig_setmethods__["m_maxHP"] = _IreonClient.ClientOwnCharData_m_maxHP_set
    __swig_getmethods__["m_maxHP"] = _IreonClient.ClientOwnCharData_m_maxHP_get
    if _newclass:m_maxHP = property(_IreonClient.ClientOwnCharData_m_maxHP_get, _IreonClient.ClientOwnCharData_m_maxHP_set)
    __swig_setmethods__["m_mana"] = _IreonClient.ClientOwnCharData_m_mana_set
    __swig_getmethods__["m_mana"] = _IreonClient.ClientOwnCharData_m_mana_get
    if _newclass:m_mana = property(_IreonClient.ClientOwnCharData_m_mana_get, _IreonClient.ClientOwnCharData_m_mana_set)
    __swig_setmethods__["m_maxMana"] = _IreonClient.ClientOwnCharData_m_maxMana_set
    __swig_getmethods__["m_maxMana"] = _IreonClient.ClientOwnCharData_m_maxMana_get
    if _newclass:m_maxMana = property(_IreonClient.ClientOwnCharData_m_maxMana_get, _IreonClient.ClientOwnCharData_m_maxMana_set)
    __swig_setmethods__["m_str"] = _IreonClient.ClientOwnCharData_m_str_set
    __swig_getmethods__["m_str"] = _IreonClient.ClientOwnCharData_m_str_get
    if _newclass:m_str = property(_IreonClient.ClientOwnCharData_m_str_get, _IreonClient.ClientOwnCharData_m_str_set)
    __swig_setmethods__["m_sta"] = _IreonClient.ClientOwnCharData_m_sta_set
    __swig_getmethods__["m_sta"] = _IreonClient.ClientOwnCharData_m_sta_get
    if _newclass:m_sta = property(_IreonClient.ClientOwnCharData_m_sta_get, _IreonClient.ClientOwnCharData_m_sta_set)
    __swig_setmethods__["m_int"] = _IreonClient.ClientOwnCharData_m_int_set
    __swig_getmethods__["m_int"] = _IreonClient.ClientOwnCharData_m_int_get
    if _newclass:m_int = property(_IreonClient.ClientOwnCharData_m_int_get, _IreonClient.ClientOwnCharData_m_int_set)
    __swig_setmethods__["m_wis"] = _IreonClient.ClientOwnCharData_m_wis_set
    __swig_getmethods__["m_wis"] = _IreonClient.ClientOwnCharData_m_wis_get
    if _newclass:m_wis = property(_IreonClient.ClientOwnCharData_m_wis_get, _IreonClient.ClientOwnCharData_m_wis_set)
    __swig_setmethods__["m_con"] = _IreonClient.ClientOwnCharData_m_con_set
    __swig_getmethods__["m_con"] = _IreonClient.ClientOwnCharData_m_con_get
    if _newclass:m_con = property(_IreonClient.ClientOwnCharData_m_con_get, _IreonClient.ClientOwnCharData_m_con_set)
    __swig_setmethods__["m_luck"] = _IreonClient.ClientOwnCharData_m_luck_set
    __swig_getmethods__["m_luck"] = _IreonClient.ClientOwnCharData_m_luck_get
    if _newclass:m_luck = property(_IreonClient.ClientOwnCharData_m_luck_get, _IreonClient.ClientOwnCharData_m_luck_set)
    def __init__(self, *args): 
        """__init__(self) -> ClientOwnCharData"""
        this = _IreonClient.new_ClientOwnCharData(*args)
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _IreonClient.delete_ClientOwnCharData
    __del__ = lambda self : None;
ClientOwnCharData_swigregister = _IreonClient.ClientOwnCharData_swigregister
ClientOwnCharData_swigregister(ClientOwnCharData)

class ClientCharRegistrationData(_object):
    """Proxy of C++ ClientCharRegistrationData class"""
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, ClientCharRegistrationData, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, ClientCharRegistrationData, name)
    __repr__ = _swig_repr
    def __init__(self, *args): 
        """__init__(self) -> ClientCharRegistrationData"""
        this = _IreonClient.new_ClientCharRegistrationData(*args)
        try: self.this.append(this)
        except: self.this = this
    def __eq__(*args):
        """__eq__(self, ClientCharRegistrationData d) -> bool"""
        return _IreonClient.ClientCharRegistrationData___eq__(*args)

    __swig_setmethods__["m_id"] = _IreonClient.ClientCharRegistrationData_m_id_set
    __swig_getmethods__["m_id"] = _IreonClient.ClientCharRegistrationData_m_id_get
    if _newclass:m_id = property(_IreonClient.ClientCharRegistrationData_m_id_get, _IreonClient.ClientCharRegistrationData_m_id_set)
    __swig_setmethods__["m_name"] = _IreonClient.ClientCharRegistrationData_m_name_set
    __swig_getmethods__["m_name"] = _IreonClient.ClientCharRegistrationData_m_name_get
    if _newclass:m_name = property(_IreonClient.ClientCharRegistrationData_m_name_get, _IreonClient.ClientCharRegistrationData_m_name_set)
    __swig_destroy__ = _IreonClient.delete_ClientCharRegistrationData
    __del__ = lambda self : None;
ClientCharRegistrationData_swigregister = _IreonClient.ClientCharRegistrationData_swigregister
ClientCharRegistrationData_swigregister(ClientCharRegistrationData)

class ClientCharMobData(ClientCharData):
    """Proxy of C++ ClientCharMobData class"""
    __swig_setmethods__ = {}
    for _s in [ClientCharData]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, ClientCharMobData, name, value)
    __swig_getmethods__ = {}
    for _s in [ClientCharData]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, ClientCharMobData, name)
    __repr__ = _swig_repr
    __swig_setmethods__["m_prototype"] = _IreonClient.ClientCharMobData_m_prototype_set
    __swig_getmethods__["m_prototype"] = _IreonClient.ClientCharMobData_m_prototype_get
    if _newclass:m_prototype = property(_IreonClient.ClientCharMobData_m_prototype_get, _IreonClient.ClientCharMobData_m_prototype_set)
    def __init__(self, *args): 
        """__init__(self) -> ClientCharMobData"""
        this = _IreonClient.new_ClientCharMobData(*args)
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _IreonClient.delete_ClientCharMobData
    __del__ = lambda self : None;
ClientCharMobData_swigregister = _IreonClient.ClientCharMobData_swigregister
ClientCharMobData_swigregister(ClientCharMobData)

ON_APP_CHANGES_STATE = _IreonClient.ON_APP_CHANGES_STATE
ON_CHAR_LIST_CHANGED = _IreonClient.ON_CHAR_LIST_CHANGED
ON_CHAR_ADD_LIST = _IreonClient.ON_CHAR_ADD_LIST
ON_ROOT_MENU_UPDATED = _IreonClient.ON_ROOT_MENU_UPDATED
ON_CHAR_CREATED = _IreonClient.ON_CHAR_CREATED
ON_CHAR_REMOVED = _IreonClient.ON_CHAR_REMOVED
ON_CHAR_SELECT_ACK = _IreonClient.ON_CHAR_SELECT_ACK
ON_CHAR_SELECT_ACK_FAILED = _IreonClient.ON_CHAR_SELECT_ACK_FAILED
ON_CHAR_SELECT = _IreonClient.ON_CHAR_SELECT
ON_CHAR_SELECT_FAILED = _IreonClient.ON_CHAR_SELECT_FAILED
ON_CONN_FAILED = _IreonClient.ON_CONN_FAILED
ON_CHECK_VERSION_FAILED = _IreonClient.ON_CHECK_VERSION_FAILED
ON_LOGIN_FAILED = _IreonClient.ON_LOGIN_FAILED
ON_CHAT = _IreonClient.ON_CHAT
ON_CHAR_INFO_UPDATE = _IreonClient.ON_CHAR_INFO_UPDATE
class AppChangeStateEventArg(EventArg):
    """Proxy of C++ AppChangeStateEventArg class"""
    __swig_setmethods__ = {}
    for _s in [EventArg]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, AppChangeStateEventArg, name, value)
    __swig_getmethods__ = {}
    for _s in [EventArg]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, AppChangeStateEventArg, name)
    __repr__ = _swig_repr
    def registerSwigType(*args):
        """registerSwigType(swigTypeInfo typeInfo)"""
        return _IreonClient.AppChangeStateEventArg_registerSwigType(*args)

    if _newclass:registerSwigType = staticmethod(registerSwigType)
    __swig_getmethods__["registerSwigType"] = lambda x: registerSwigType
    def getSwigType(*args):
        """getSwigType(self) -> swigTypeInfo"""
        return _IreonClient.AppChangeStateEventArg_getSwigType(*args)

    __swig_setmethods__["m_oldState"] = _IreonClient.AppChangeStateEventArg_m_oldState_set
    __swig_getmethods__["m_oldState"] = _IreonClient.AppChangeStateEventArg_m_oldState_get
    if _newclass:m_oldState = property(_IreonClient.AppChangeStateEventArg_m_oldState_get, _IreonClient.AppChangeStateEventArg_m_oldState_set)
    __swig_setmethods__["m_newState"] = _IreonClient.AppChangeStateEventArg_m_newState_set
    __swig_getmethods__["m_newState"] = _IreonClient.AppChangeStateEventArg_m_newState_get
    if _newclass:m_newState = property(_IreonClient.AppChangeStateEventArg_m_newState_get, _IreonClient.AppChangeStateEventArg_m_newState_set)
    def __init__(self, *args): 
        """__init__(self) -> AppChangeStateEventArg"""
        this = _IreonClient.new_AppChangeStateEventArg(*args)
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _IreonClient.delete_AppChangeStateEventArg
    __del__ = lambda self : None;
AppChangeStateEventArg_swigregister = _IreonClient.AppChangeStateEventArg_swigregister
AppChangeStateEventArg_swigregister(AppChangeStateEventArg)

def AppChangeStateEventArg_registerSwigType(*args):
  """AppChangeStateEventArg_registerSwigType(swigTypeInfo typeInfo)"""
  return _IreonClient.AppChangeStateEventArg_registerSwigType(*args)

class CharCreatedEventArg(EventArg):
    """Proxy of C++ CharCreatedEventArg class"""
    __swig_setmethods__ = {}
    for _s in [EventArg]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, CharCreatedEventArg, name, value)
    __swig_getmethods__ = {}
    for _s in [EventArg]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, CharCreatedEventArg, name)
    __repr__ = _swig_repr
    def registerSwigType(*args):
        """registerSwigType(swigTypeInfo typeInfo)"""
        return _IreonClient.CharCreatedEventArg_registerSwigType(*args)

    if _newclass:registerSwigType = staticmethod(registerSwigType)
    __swig_getmethods__["registerSwigType"] = lambda x: registerSwigType
    def getSwigType(*args):
        """getSwigType(self) -> swigTypeInfo"""
        return _IreonClient.CharCreatedEventArg_getSwigType(*args)

    __swig_setmethods__["m_res"] = _IreonClient.CharCreatedEventArg_m_res_set
    __swig_getmethods__["m_res"] = _IreonClient.CharCreatedEventArg_m_res_get
    if _newclass:m_res = property(_IreonClient.CharCreatedEventArg_m_res_get, _IreonClient.CharCreatedEventArg_m_res_set)
    def __init__(self, *args): 
        """__init__(self) -> CharCreatedEventArg"""
        this = _IreonClient.new_CharCreatedEventArg(*args)
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _IreonClient.delete_CharCreatedEventArg
    __del__ = lambda self : None;
CharCreatedEventArg_swigregister = _IreonClient.CharCreatedEventArg_swigregister
CharCreatedEventArg_swigregister(CharCreatedEventArg)

def CharCreatedEventArg_registerSwigType(*args):
  """CharCreatedEventArg_registerSwigType(swigTypeInfo typeInfo)"""
  return _IreonClient.CharCreatedEventArg_registerSwigType(*args)

class CharRemovedEventArg(EventArg):
    """Proxy of C++ CharRemovedEventArg class"""
    __swig_setmethods__ = {}
    for _s in [EventArg]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, CharRemovedEventArg, name, value)
    __swig_getmethods__ = {}
    for _s in [EventArg]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, CharRemovedEventArg, name)
    __repr__ = _swig_repr
    def registerSwigType(*args):
        """registerSwigType(swigTypeInfo typeInfo)"""
        return _IreonClient.CharRemovedEventArg_registerSwigType(*args)

    if _newclass:registerSwigType = staticmethod(registerSwigType)
    __swig_getmethods__["registerSwigType"] = lambda x: registerSwigType
    def getSwigType(*args):
        """getSwigType(self) -> swigTypeInfo"""
        return _IreonClient.CharRemovedEventArg_getSwigType(*args)

    __swig_setmethods__["m_res"] = _IreonClient.CharRemovedEventArg_m_res_set
    __swig_getmethods__["m_res"] = _IreonClient.CharRemovedEventArg_m_res_get
    if _newclass:m_res = property(_IreonClient.CharRemovedEventArg_m_res_get, _IreonClient.CharRemovedEventArg_m_res_set)
    __swig_setmethods__["m_id"] = _IreonClient.CharRemovedEventArg_m_id_set
    __swig_getmethods__["m_id"] = _IreonClient.CharRemovedEventArg_m_id_get
    if _newclass:m_id = property(_IreonClient.CharRemovedEventArg_m_id_get, _IreonClient.CharRemovedEventArg_m_id_set)
    def __init__(self, *args): 
        """__init__(self) -> CharRemovedEventArg"""
        this = _IreonClient.new_CharRemovedEventArg(*args)
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _IreonClient.delete_CharRemovedEventArg
    __del__ = lambda self : None;
CharRemovedEventArg_swigregister = _IreonClient.CharRemovedEventArg_swigregister
CharRemovedEventArg_swigregister(CharRemovedEventArg)

def CharRemovedEventArg_registerSwigType(*args):
  """CharRemovedEventArg_registerSwigType(swigTypeInfo typeInfo)"""
  return _IreonClient.CharRemovedEventArg_registerSwigType(*args)

class CharAddListArg(EventArg):
    """Proxy of C++ CharAddListArg class"""
    __swig_setmethods__ = {}
    for _s in [EventArg]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, CharAddListArg, name, value)
    __swig_getmethods__ = {}
    for _s in [EventArg]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, CharAddListArg, name)
    __repr__ = _swig_repr
    def registerSwigType(*args):
        """registerSwigType(swigTypeInfo typeInfo)"""
        return _IreonClient.CharAddListArg_registerSwigType(*args)

    if _newclass:registerSwigType = staticmethod(registerSwigType)
    __swig_getmethods__["registerSwigType"] = lambda x: registerSwigType
    def getSwigType(*args):
        """getSwigType(self) -> swigTypeInfo"""
        return _IreonClient.CharAddListArg_getSwigType(*args)

    __swig_setmethods__["m_id"] = _IreonClient.CharAddListArg_m_id_set
    __swig_getmethods__["m_id"] = _IreonClient.CharAddListArg_m_id_get
    if _newclass:m_id = property(_IreonClient.CharAddListArg_m_id_get, _IreonClient.CharAddListArg_m_id_set)
    __swig_setmethods__["m_name"] = _IreonClient.CharAddListArg_m_name_set
    __swig_getmethods__["m_name"] = _IreonClient.CharAddListArg_m_name_get
    if _newclass:m_name = property(_IreonClient.CharAddListArg_m_name_get, _IreonClient.CharAddListArg_m_name_set)
    def __init__(self, *args): 
        """__init__(self) -> CharAddListArg"""
        this = _IreonClient.new_CharAddListArg(*args)
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _IreonClient.delete_CharAddListArg
    __del__ = lambda self : None;
CharAddListArg_swigregister = _IreonClient.CharAddListArg_swigregister
CharAddListArg_swigregister(CharAddListArg)

def CharAddListArg_registerSwigType(*args):
  """CharAddListArg_registerSwigType(swigTypeInfo typeInfo)"""
  return _IreonClient.CharAddListArg_registerSwigType(*args)

class CharSelectAckArg(EventArg):
    """Proxy of C++ CharSelectAckArg class"""
    __swig_setmethods__ = {}
    for _s in [EventArg]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, CharSelectAckArg, name, value)
    __swig_getmethods__ = {}
    for _s in [EventArg]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, CharSelectAckArg, name)
    __repr__ = _swig_repr
    def registerSwigType(*args):
        """registerSwigType(swigTypeInfo typeInfo)"""
        return _IreonClient.CharSelectAckArg_registerSwigType(*args)

    if _newclass:registerSwigType = staticmethod(registerSwigType)
    __swig_getmethods__["registerSwigType"] = lambda x: registerSwigType
    def getSwigType(*args):
        """getSwigType(self) -> swigTypeInfo"""
        return _IreonClient.CharSelectAckArg_getSwigType(*args)

    __swig_setmethods__["m_res"] = _IreonClient.CharSelectAckArg_m_res_set
    __swig_getmethods__["m_res"] = _IreonClient.CharSelectAckArg_m_res_get
    if _newclass:m_res = property(_IreonClient.CharSelectAckArg_m_res_get, _IreonClient.CharSelectAckArg_m_res_set)
    def __init__(self, *args): 
        """__init__(self) -> CharSelectAckArg"""
        this = _IreonClient.new_CharSelectAckArg(*args)
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _IreonClient.delete_CharSelectAckArg
    __del__ = lambda self : None;
CharSelectAckArg_swigregister = _IreonClient.CharSelectAckArg_swigregister
CharSelectAckArg_swigregister(CharSelectAckArg)

def CharSelectAckArg_registerSwigType(*args):
  """CharSelectAckArg_registerSwigType(swigTypeInfo typeInfo)"""
  return _IreonClient.CharSelectAckArg_registerSwigType(*args)

class CharSelectAckFailedArg(EventArg):
    """Proxy of C++ CharSelectAckFailedArg class"""
    __swig_setmethods__ = {}
    for _s in [EventArg]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, CharSelectAckFailedArg, name, value)
    __swig_getmethods__ = {}
    for _s in [EventArg]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, CharSelectAckFailedArg, name)
    __repr__ = _swig_repr
    def registerSwigType(*args):
        """registerSwigType(swigTypeInfo typeInfo)"""
        return _IreonClient.CharSelectAckFailedArg_registerSwigType(*args)

    if _newclass:registerSwigType = staticmethod(registerSwigType)
    __swig_getmethods__["registerSwigType"] = lambda x: registerSwigType
    def getSwigType(*args):
        """getSwigType(self) -> swigTypeInfo"""
        return _IreonClient.CharSelectAckFailedArg_getSwigType(*args)

    __swig_setmethods__["m_res"] = _IreonClient.CharSelectAckFailedArg_m_res_set
    __swig_getmethods__["m_res"] = _IreonClient.CharSelectAckFailedArg_m_res_get
    if _newclass:m_res = property(_IreonClient.CharSelectAckFailedArg_m_res_get, _IreonClient.CharSelectAckFailedArg_m_res_set)
    def __init__(self, *args): 
        """__init__(self) -> CharSelectAckFailedArg"""
        this = _IreonClient.new_CharSelectAckFailedArg(*args)
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _IreonClient.delete_CharSelectAckFailedArg
    __del__ = lambda self : None;
CharSelectAckFailedArg_swigregister = _IreonClient.CharSelectAckFailedArg_swigregister
CharSelectAckFailedArg_swigregister(CharSelectAckFailedArg)

def CharSelectAckFailedArg_registerSwigType(*args):
  """CharSelectAckFailedArg_registerSwigType(swigTypeInfo typeInfo)"""
  return _IreonClient.CharSelectAckFailedArg_registerSwigType(*args)

class CharSelectArg(EventArg):
    """Proxy of C++ CharSelectArg class"""
    __swig_setmethods__ = {}
    for _s in [EventArg]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, CharSelectArg, name, value)
    __swig_getmethods__ = {}
    for _s in [EventArg]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, CharSelectArg, name)
    __repr__ = _swig_repr
    def registerSwigType(*args):
        """registerSwigType(swigTypeInfo typeInfo)"""
        return _IreonClient.CharSelectArg_registerSwigType(*args)

    if _newclass:registerSwigType = staticmethod(registerSwigType)
    __swig_getmethods__["registerSwigType"] = lambda x: registerSwigType
    def getSwigType(*args):
        """getSwigType(self) -> swigTypeInfo"""
        return _IreonClient.CharSelectArg_getSwigType(*args)

    __swig_setmethods__["m_WSaddress"] = _IreonClient.CharSelectArg_m_WSaddress_set
    __swig_getmethods__["m_WSaddress"] = _IreonClient.CharSelectArg_m_WSaddress_get
    if _newclass:m_WSaddress = property(_IreonClient.CharSelectArg_m_WSaddress_get, _IreonClient.CharSelectArg_m_WSaddress_set)
    __swig_setmethods__["m_token"] = _IreonClient.CharSelectArg_m_token_set
    __swig_getmethods__["m_token"] = _IreonClient.CharSelectArg_m_token_get
    if _newclass:m_token = property(_IreonClient.CharSelectArg_m_token_get, _IreonClient.CharSelectArg_m_token_set)
    def __init__(self, *args): 
        """__init__(self) -> CharSelectArg"""
        this = _IreonClient.new_CharSelectArg(*args)
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _IreonClient.delete_CharSelectArg
    __del__ = lambda self : None;
CharSelectArg_swigregister = _IreonClient.CharSelectArg_swigregister
CharSelectArg_swigregister(CharSelectArg)

def CharSelectArg_registerSwigType(*args):
  """CharSelectArg_registerSwigType(swigTypeInfo typeInfo)"""
  return _IreonClient.CharSelectArg_registerSwigType(*args)

class CharSelectFailedArg(EventArg):
    """Proxy of C++ CharSelectFailedArg class"""
    __swig_setmethods__ = {}
    for _s in [EventArg]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, CharSelectFailedArg, name, value)
    __swig_getmethods__ = {}
    for _s in [EventArg]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, CharSelectFailedArg, name)
    __repr__ = _swig_repr
    def registerSwigType(*args):
        """registerSwigType(swigTypeInfo typeInfo)"""
        return _IreonClient.CharSelectFailedArg_registerSwigType(*args)

    if _newclass:registerSwigType = staticmethod(registerSwigType)
    __swig_getmethods__["registerSwigType"] = lambda x: registerSwigType
    def getSwigType(*args):
        """getSwigType(self) -> swigTypeInfo"""
        return _IreonClient.CharSelectFailedArg_getSwigType(*args)

    __swig_setmethods__["m_res"] = _IreonClient.CharSelectFailedArg_m_res_set
    __swig_getmethods__["m_res"] = _IreonClient.CharSelectFailedArg_m_res_get
    if _newclass:m_res = property(_IreonClient.CharSelectFailedArg_m_res_get, _IreonClient.CharSelectFailedArg_m_res_set)
    __swig_setmethods__["m_charInGame"] = _IreonClient.CharSelectFailedArg_m_charInGame_set
    __swig_getmethods__["m_charInGame"] = _IreonClient.CharSelectFailedArg_m_charInGame_get
    if _newclass:m_charInGame = property(_IreonClient.CharSelectFailedArg_m_charInGame_get, _IreonClient.CharSelectFailedArg_m_charInGame_set)
    def __init__(self, *args): 
        """__init__(self) -> CharSelectFailedArg"""
        this = _IreonClient.new_CharSelectFailedArg(*args)
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _IreonClient.delete_CharSelectFailedArg
    __del__ = lambda self : None;
CharSelectFailedArg_swigregister = _IreonClient.CharSelectFailedArg_swigregister
CharSelectFailedArg_swigregister(CharSelectFailedArg)

def CharSelectFailedArg_registerSwigType(*args):
  """CharSelectFailedArg_registerSwigType(swigTypeInfo typeInfo)"""
  return _IreonClient.CharSelectFailedArg_registerSwigType(*args)

class ConnectFailedEventArg(EventArg):
    """Proxy of C++ ConnectFailedEventArg class"""
    __swig_setmethods__ = {}
    for _s in [EventArg]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, ConnectFailedEventArg, name, value)
    __swig_getmethods__ = {}
    for _s in [EventArg]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, ConnectFailedEventArg, name)
    __repr__ = _swig_repr
    def registerSwigType(*args):
        """registerSwigType(swigTypeInfo typeInfo)"""
        return _IreonClient.ConnectFailedEventArg_registerSwigType(*args)

    if _newclass:registerSwigType = staticmethod(registerSwigType)
    __swig_getmethods__["registerSwigType"] = lambda x: registerSwigType
    def getSwigType(*args):
        """getSwigType(self) -> swigTypeInfo"""
        return _IreonClient.ConnectFailedEventArg_getSwigType(*args)

    __swig_setmethods__["m_host"] = _IreonClient.ConnectFailedEventArg_m_host_set
    __swig_getmethods__["m_host"] = _IreonClient.ConnectFailedEventArg_m_host_get
    if _newclass:m_host = property(_IreonClient.ConnectFailedEventArg_m_host_get, _IreonClient.ConnectFailedEventArg_m_host_set)
    __swig_setmethods__["m_port"] = _IreonClient.ConnectFailedEventArg_m_port_set
    __swig_getmethods__["m_port"] = _IreonClient.ConnectFailedEventArg_m_port_get
    if _newclass:m_port = property(_IreonClient.ConnectFailedEventArg_m_port_get, _IreonClient.ConnectFailedEventArg_m_port_set)
    __swig_setmethods__["m_error"] = _IreonClient.ConnectFailedEventArg_m_error_set
    __swig_getmethods__["m_error"] = _IreonClient.ConnectFailedEventArg_m_error_get
    if _newclass:m_error = property(_IreonClient.ConnectFailedEventArg_m_error_get, _IreonClient.ConnectFailedEventArg_m_error_set)
    def __init__(self, *args): 
        """__init__(self) -> ConnectFailedEventArg"""
        this = _IreonClient.new_ConnectFailedEventArg(*args)
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _IreonClient.delete_ConnectFailedEventArg
    __del__ = lambda self : None;
ConnectFailedEventArg_swigregister = _IreonClient.ConnectFailedEventArg_swigregister
ConnectFailedEventArg_swigregister(ConnectFailedEventArg)

def ConnectFailedEventArg_registerSwigType(*args):
  """ConnectFailedEventArg_registerSwigType(swigTypeInfo typeInfo)"""
  return _IreonClient.ConnectFailedEventArg_registerSwigType(*args)

class CheckVersionFailedEventArg(EventArg):
    """Proxy of C++ CheckVersionFailedEventArg class"""
    __swig_setmethods__ = {}
    for _s in [EventArg]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, CheckVersionFailedEventArg, name, value)
    __swig_getmethods__ = {}
    for _s in [EventArg]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, CheckVersionFailedEventArg, name)
    __repr__ = _swig_repr
    def registerSwigType(*args):
        """registerSwigType(swigTypeInfo typeInfo)"""
        return _IreonClient.CheckVersionFailedEventArg_registerSwigType(*args)

    if _newclass:registerSwigType = staticmethod(registerSwigType)
    __swig_getmethods__["registerSwigType"] = lambda x: registerSwigType
    def getSwigType(*args):
        """getSwigType(self) -> swigTypeInfo"""
        return _IreonClient.CheckVersionFailedEventArg_getSwigType(*args)

    __swig_setmethods__["m_host"] = _IreonClient.CheckVersionFailedEventArg_m_host_set
    __swig_getmethods__["m_host"] = _IreonClient.CheckVersionFailedEventArg_m_host_get
    if _newclass:m_host = property(_IreonClient.CheckVersionFailedEventArg_m_host_get, _IreonClient.CheckVersionFailedEventArg_m_host_set)
    __swig_setmethods__["m_port"] = _IreonClient.CheckVersionFailedEventArg_m_port_set
    __swig_getmethods__["m_port"] = _IreonClient.CheckVersionFailedEventArg_m_port_get
    if _newclass:m_port = property(_IreonClient.CheckVersionFailedEventArg_m_port_get, _IreonClient.CheckVersionFailedEventArg_m_port_set)
    __swig_setmethods__["m_serverVersion"] = _IreonClient.CheckVersionFailedEventArg_m_serverVersion_set
    __swig_getmethods__["m_serverVersion"] = _IreonClient.CheckVersionFailedEventArg_m_serverVersion_get
    if _newclass:m_serverVersion = property(_IreonClient.CheckVersionFailedEventArg_m_serverVersion_get, _IreonClient.CheckVersionFailedEventArg_m_serverVersion_set)
    def __init__(self, *args): 
        """__init__(self) -> CheckVersionFailedEventArg"""
        this = _IreonClient.new_CheckVersionFailedEventArg(*args)
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _IreonClient.delete_CheckVersionFailedEventArg
    __del__ = lambda self : None;
CheckVersionFailedEventArg_swigregister = _IreonClient.CheckVersionFailedEventArg_swigregister
CheckVersionFailedEventArg_swigregister(CheckVersionFailedEventArg)

def CheckVersionFailedEventArg_registerSwigType(*args):
  """CheckVersionFailedEventArg_registerSwigType(swigTypeInfo typeInfo)"""
  return _IreonClient.CheckVersionFailedEventArg_registerSwigType(*args)

class ChatEventArg(EventArg):
    """Proxy of C++ ChatEventArg class"""
    __swig_setmethods__ = {}
    for _s in [EventArg]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, ChatEventArg, name, value)
    __swig_getmethods__ = {}
    for _s in [EventArg]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, ChatEventArg, name)
    __repr__ = _swig_repr
    def registerSwigType(*args):
        """registerSwigType(swigTypeInfo typeInfo)"""
        return _IreonClient.ChatEventArg_registerSwigType(*args)

    if _newclass:registerSwigType = staticmethod(registerSwigType)
    __swig_getmethods__["registerSwigType"] = lambda x: registerSwigType
    def getSwigType(*args):
        """getSwigType(self) -> swigTypeInfo"""
        return _IreonClient.ChatEventArg_getSwigType(*args)

    __swig_setmethods__["m_command"] = _IreonClient.ChatEventArg_m_command_set
    __swig_getmethods__["m_command"] = _IreonClient.ChatEventArg_m_command_get
    if _newclass:m_command = property(_IreonClient.ChatEventArg_m_command_get, _IreonClient.ChatEventArg_m_command_set)
    __swig_setmethods__["m_arg1"] = _IreonClient.ChatEventArg_m_arg1_set
    __swig_getmethods__["m_arg1"] = _IreonClient.ChatEventArg_m_arg1_get
    if _newclass:m_arg1 = property(_IreonClient.ChatEventArg_m_arg1_get, _IreonClient.ChatEventArg_m_arg1_set)
    __swig_setmethods__["m_arg2"] = _IreonClient.ChatEventArg_m_arg2_set
    __swig_getmethods__["m_arg2"] = _IreonClient.ChatEventArg_m_arg2_get
    if _newclass:m_arg2 = property(_IreonClient.ChatEventArg_m_arg2_get, _IreonClient.ChatEventArg_m_arg2_set)
    __swig_setmethods__["m_arg3"] = _IreonClient.ChatEventArg_m_arg3_set
    __swig_getmethods__["m_arg3"] = _IreonClient.ChatEventArg_m_arg3_get
    if _newclass:m_arg3 = property(_IreonClient.ChatEventArg_m_arg3_get, _IreonClient.ChatEventArg_m_arg3_set)
    def __init__(self, *args): 
        """__init__(self) -> ChatEventArg"""
        this = _IreonClient.new_ChatEventArg(*args)
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _IreonClient.delete_ChatEventArg
    __del__ = lambda self : None;
ChatEventArg_swigregister = _IreonClient.ChatEventArg_swigregister
ChatEventArg_swigregister(ChatEventArg)

def ChatEventArg_registerSwigType(*args):
  """ChatEventArg_registerSwigType(swigTypeInfo typeInfo)"""
  return _IreonClient.ChatEventArg_registerSwigType(*args)

class CClientInterface(CInterface):
    """Proxy of C++ CClientInterface class"""
    __swig_setmethods__ = {}
    for _s in [CInterface]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, CClientInterface, name, value)
    __swig_getmethods__ = {}
    for _s in [CInterface]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, CClientInterface, name)
    def __init__(self): raise AttributeError, "No constructor defined"
    __repr__ = _swig_repr
    __swig_destroy__ = _IreonClient.delete_CClientInterface
    __del__ = lambda self : None;
    def instance(*args):
        """instance() -> CClientInterface"""
        return _IreonClient.CClientInterface_instance(*args)

    if _newclass:instance = staticmethod(instance)
    __swig_getmethods__["instance"] = lambda x: instance
    def init(*args):
        """init(self, RenderWindow win) -> bool"""
        return _IreonClient.CClientInterface_init(*args)

    def pulseUpdate(*args):
        """pulseUpdate(self, Real time)"""
        return _IreonClient.CClientInterface_pulseUpdate(*args)

    def frameStarted(*args):
        """frameStarted(self, FrameEvent evt) -> bool"""
        return _IreonClient.CClientInterface_frameStarted(*args)

    def isFirstPerson(*args):
        """isFirstPerson(self) -> bool"""
        return _IreonClient.CClientInterface_isFirstPerson(*args)

    def setFirstPerson(*args):
        """setFirstPerson(self, bool value)"""
        return _IreonClient.CClientInterface_setFirstPerson(*args)

    def getTarget(*args):
        """getTarget(self) -> CharacterPtr"""
        return _IreonClient.CClientInterface_getTarget(*args)

    def setTarget(*args):
        """setTarget(self, CharacterPtr target)"""
        return _IreonClient.CClientInterface_setTarget(*args)

    def nextTarget(*args):
        """nextTarget(self)"""
        return _IreonClient.CClientInterface_nextTarget(*args)

    def getHighlight(*args):
        """getHighlight(self) -> CharacterPtr"""
        return _IreonClient.CClientInterface_getHighlight(*args)

    def setCamR(*args):
        """setCamR(self, Real radius)"""
        return _IreonClient.CClientInterface_setCamR(*args)

    def mouseMoved(*args):
        """mouseMoved(self, MouseEvent e)"""
        return _IreonClient.CClientInterface_mouseMoved(*args)

    def enableControl(*args):
        """enableControl(self, bool enable)"""
        return _IreonClient.CClientInterface_enableControl(*args)

    def getTimeLastUpdate(*args):
        """getTimeLastUpdate(self) -> Real"""
        return _IreonClient.CClientInterface_getTimeLastUpdate(*args)

CClientInterface_swigregister = _IreonClient.CClientInterface_swigregister
CClientInterface_swigregister(CClientInterface)

def CClientInterface_instance(*args):
  """CClientInterface_instance() -> CClientInterface"""
  return _IreonClient.CClientInterface_instance(*args)

class CClientWorld(CWorld):
    """Proxy of C++ CClientWorld class"""
    __swig_setmethods__ = {}
    for _s in [CWorld]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, CClientWorld, name, value)
    __swig_getmethods__ = {}
    for _s in [CWorld]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, CClientWorld, name)
    def __init__(self): raise AttributeError, "No constructor defined"
    __repr__ = _swig_repr
    __swig_destroy__ = _IreonClient.delete_CClientWorld
    __del__ = lambda self : None;
    def instance(*args):
        """instance() -> CClientWorld"""
        return _IreonClient.CClientWorld_instance(*args)

    if _newclass:instance = staticmethod(instance)
    __swig_getmethods__["instance"] = lambda x: instance
    def init(*args):
        """init(self) -> bool"""
        return _IreonClient.CClientWorld_init(*args)

    def createScene(*args):
        """createScene(self) -> bool"""
        return _IreonClient.CClientWorld_createScene(*args)

    def load(*args):
        """load(self) -> bool"""
        return _IreonClient.CClientWorld_load(*args)

    def loadZones(*args):
        """loadZones(self) -> bool"""
        return _IreonClient.CClientWorld_loadZones(*args)

    def updateCharactersInfo(*args):
        """updateCharactersInfo(self)"""
        return _IreonClient.CClientWorld_updateCharactersInfo(*args)

    def makeVisibleList(*args):
        """
        makeVisibleList(self, std::vector<(CharacterPtr,std::allocator<(CharacterPtr)>)> list, 
            bool sorted=False)
        makeVisibleList(self, std::vector<(CharacterPtr,std::allocator<(CharacterPtr)>)> list)
        """
        return _IreonClient.CClientWorld_makeVisibleList(*args)

    def update(*args):
        """update(self, Real time)"""
        return _IreonClient.CClientWorld_update(*args)

    def clear(*args):
        """clear(self)"""
        return _IreonClient.CClientWorld_clear(*args)

CClientWorld_swigregister = _IreonClient.CClientWorld_swigregister
CClientWorld_swigregister(CClientWorld)

def CClientWorld_instance(*args):
  """CClientWorld_instance() -> CClientWorld"""
  return _IreonClient.CClientWorld_instance(*args)

class PythonOutput(_object):
    """Proxy of C++ PythonOutput class"""
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, PythonOutput, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, PythonOutput, name)
    __repr__ = _swig_repr
    def write(*args):
        """write(self, String arg)"""
        return _IreonClient.PythonOutput_write(*args)

    def __init__(self, *args): 
        """__init__(self) -> PythonOutput"""
        this = _IreonClient.new_PythonOutput(*args)
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _IreonClient.delete_PythonOutput
    __del__ = lambda self : None;
PythonOutput_swigregister = _IreonClient.PythonOutput_swigregister
PythonOutput_swigregister(PythonOutput)

class CClientApp(_object):
    """Proxy of C++ CClientApp class"""
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, CClientApp, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, CClientApp, name)
    __repr__ = _swig_repr
    AS_MENU = _IreonClient.CClientApp_AS_MENU
    AS_CONNECTING_ROOT = _IreonClient.CClientApp_AS_CONNECTING_ROOT
    AS_CHECKING_VERSION = _IreonClient.CClientApp_AS_CHECKING_VERSION
    AS_LOGING_ROOT = _IreonClient.CClientApp_AS_LOGING_ROOT
    AS_ROOT = _IreonClient.CClientApp_AS_ROOT
    AS_CONNECTING_WORLD = _IreonClient.CClientApp_AS_CONNECTING_WORLD
    AS_LOGING_WORLD = _IreonClient.CClientApp_AS_LOGING_WORLD
    AS_PLAY = _IreonClient.CClientApp_AS_PLAY
    AS_DEAD = _IreonClient.CClientApp_AS_DEAD
    AS_EMPTY = _IreonClient.CClientApp_AS_EMPTY
    AS_COUNT = _IreonClient.CClientApp_AS_COUNT
    def __init__(self, *args): 
        """__init__(self, String configPath) -> CClientApp"""
        this = _IreonClient.new_CClientApp(*args)
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _IreonClient.delete_CClientApp
    __del__ = lambda self : None;
    def init(*args):
        """init(self) -> bool"""
        return _IreonClient.CClientApp_init(*args)

    def go(*args):
        """go(self)"""
        return _IreonClient.CClientApp_go(*args)

    def shutdown(*args):
        """shutdown(self)"""
        return _IreonClient.CClientApp_shutdown(*args)

    def instance(*args):
        """instance() -> CClientApp"""
        return _IreonClient.CClientApp_instance(*args)

    if _newclass:instance = staticmethod(instance)
    __swig_getmethods__["instance"] = lambda x: instance
    def getState(*args):
        """getState(self) -> int"""
        return _IreonClient.CClientApp_getState(*args)

    def setState(*args):
        """setState(self, State s)"""
        return _IreonClient.CClientApp_setState(*args)

    def getConfig(*args):
        """getConfig(self) -> ireon::client::CPCConfig"""
        return _IreonClient.CClientApp_getConfig(*args)

    def inGame(*args):
        """inGame(self) -> bool"""
        return _IreonClient.CClientApp_inGame(*args)

    def createChar(*args):
        """createChar(self, String name)"""
        return _IreonClient.CClientApp_createChar(*args)

    def selectChar(*args):
        """selectChar(self, ushort id)"""
        return _IreonClient.CClientApp_selectChar(*args)

    def removeChar(*args):
        """removeChar(self, uint id)"""
        return _IreonClient.CClientApp_removeChar(*args)

    def refreshListChar(*args):
        """refreshListChar(self)"""
        return _IreonClient.CClientApp_refreshListChar(*args)

    def getFPS(*args):
        """getFPS(self) -> Real"""
        return _IreonClient.CClientApp_getFPS(*args)

    def getTriCount(*args):
        """getTriCount(self) -> uint"""
        return _IreonClient.CClientApp_getTriCount(*args)

    def getVersion(*args):
        """getVersion(self) -> ByteVector"""
        return _IreonClient.CClientApp_getVersion(*args)

    def getRenderWindow(*args):
        """getRenderWindow(self) -> RenderWindow"""
        return _IreonClient.CClientApp_getRenderWindow(*args)

    def getScriptModule(*args):
        """getScriptModule(self) -> PythonScriptModule"""
        return _IreonClient.CClientApp_getScriptModule(*args)

    def startRSConnection(*args):
        """startRSConnection(self)"""
        return _IreonClient.CClientApp_startRSConnection(*args)

    def startWSConnection(*args):
        """startWSConnection(self)"""
        return _IreonClient.CClientApp_startWSConnection(*args)

    def setWorldConnectionParams(*args):
        """setWorldConnectionParams(self, String worldHost, String token)"""
        return _IreonClient.CClientApp_setWorldConnectionParams(*args)

    def disconnect(*args):
        """disconnect(self)"""
        return _IreonClient.CClientApp_disconnect(*args)

    def pulse(*args):
        """pulse(self, Real time)"""
        return _IreonClient.CClientApp_pulse(*args)

    def enterWorld(*args):
        """enterWorld(self)"""
        return _IreonClient.CClientApp_enterWorld(*args)

CClientApp_swigregister = _IreonClient.CClientApp_swigregister
CClientApp_swigregister(CClientApp)

def CClientApp_instance(*args):
  """CClientApp_instance() -> CClientApp"""
  return _IreonClient.CClientApp_instance(*args)



