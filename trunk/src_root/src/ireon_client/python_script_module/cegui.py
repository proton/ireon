# This file was created automatically by SWIG 1.3.29.
# Don't modify this file, modify the SWIG interface instead.

import _cegui
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


def _swig_setattr_nondynamic_method(set):
    def set_attr(self,name,value):
        if (name == "thisown"): return self.this.own(value)
        if hasattr(self,name) or (name == "this"):
            set(self,name,value)
        else:
            raise AttributeError("You cannot add attributes to %s" % self)
    return set_attr


try:
    import weakref
    weakref_proxy = weakref.proxy
except:
    weakref_proxy = lambda x: x


SWIG_BUILD_VERSION = 0x010329

CEGUI_ALIGN_ELEMENTS_TO_PIXELS = _cegui.CEGUI_ALIGN_ELEMENTS_TO_PIXELS
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

class _FloatFloatPair(object):
    thisown = property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, *args): 
        _cegui._FloatFloatPair_swiginit(self,_cegui.new__FloatFloatPair(*args))
    first = property(_cegui._FloatFloatPair_first_get, _cegui._FloatFloatPair_first_set)
    second = property(_cegui._FloatFloatPair_second_get, _cegui._FloatFloatPair_second_set)
    def __len__(self): return 2
    def __repr__(self): return str((self.first, self.second))
    def __getitem__(self, index): 
      if not (index % 2): 
        return self.first
      else:
        return self.second
    def __setitem__(self, index, val):
      if not (index % 2): 
        self.first = val
      else:
        self.second = val
    __swig_destroy__ = _cegui.delete__FloatFloatPair
_FloatFloatPair_swigregister = _cegui._FloatFloatPair_swigregister
_FloatFloatPair_swigregister(_FloatFloatPair)
cvar = _cegui.cvar
DefaultNativeHorzRes = cvar.DefaultNativeHorzRes
DefaultNativeVertRes = cvar.DefaultNativeVertRes

class Scheme(object):
    thisown = property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    def __init__(self): raise AttributeError, "No constructor defined"
    __repr__ = _swig_repr
    getDefaultResourceGroup = staticmethod(_cegui.Scheme_getDefaultResourceGroup)
    setDefaultResourceGroup = staticmethod(_cegui.Scheme_setDefaultResourceGroup)
    __swig_destroy__ = _cegui.delete_Scheme
    Name = property(_cegui.Scheme_Name_get, None, None,
                    "Scheme.Name -> const CEGUI::String &  (read-only)\n\nThis is equivalent to calling the C++ CEGUI method:\n\tget: const CEGUI::String & Scheme::getName()")

    ResourcesLoaded = property(_cegui.Scheme_ResourcesLoaded_get, None, None,
                    "Scheme.ResourcesLoaded -> bool  (read-only)\n\nThis is equivalent to calling the C++ CEGUI method:\n\tget: bool Scheme::resourcesLoaded()")

Scheme.loadResources = new_instancemethod(_cegui.Scheme_loadResources,None,Scheme)
Scheme.unloadResources = new_instancemethod(_cegui.Scheme_unloadResources,None,Scheme)
Scheme_swigregister = _cegui.Scheme_swigregister
Scheme_swigregister(Scheme)
Scheme_getDefaultResourceGroup = _cegui.Scheme_getDefaultResourceGroup
Scheme_setDefaultResourceGroup = _cegui.Scheme_setDefaultResourceGroup

class Rect(object):
    thisown = property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, *args): 
        _cegui.Rect_swiginit(self,_cegui.new_Rect(*args))
    d_top = property(_cegui.Rect_d_top_get, _cegui.Rect_d_top_set)
    d_bottom = property(_cegui.Rect_d_bottom_get, _cegui.Rect_d_bottom_set)
    d_left = property(_cegui.Rect_d_left_get, _cegui.Rect_d_left_set)
    d_right = property(_cegui.Rect_d_right_get, _cegui.Rect_d_right_set)
    Position = property(_cegui.Rect_Position_get, _cegui.Rect_Position_set, None,
                    "Rect.Position -> CEGUI::Vector2\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: CEGUI::Vector2 Rect::getPosition()\n\tset: void Rect::setPosition(CEGUI::Vector2)")


    Width = property(_cegui.Rect_Width_get, _cegui.Rect_Width_set, None,
                    "Rect.Width -> float\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: float Rect::getWidth()\n\tset: void Rect::setWidth(float)")


    Height = property(_cegui.Rect_Height_get, _cegui.Rect_Height_set, None,
                    "Rect.Height -> float\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: float Rect::getHeight()\n\tset: void Rect::setHeight(float)")


    Size = property(_cegui.Rect_Size_get, _cegui.Rect_Size_set, None,
                    "Rect.Size -> CEGUI::Size\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: CEGUI::Size Rect::getSize()\n\tset: void Rect::setSize(CEGUI::Size)")


    __swig_destroy__ = _cegui.delete_Rect
Rect.getIntersection = new_instancemethod(_cegui.Rect_getIntersection,None,Rect)
Rect.offset = new_instancemethod(_cegui.Rect_offset,None,Rect)
Rect.isPointInRect = new_instancemethod(_cegui.Rect_isPointInRect,None,Rect)
Rect.constrainSizeMax = new_instancemethod(_cegui.Rect_constrainSizeMax,None,Rect)
Rect.constrainSizeMin = new_instancemethod(_cegui.Rect_constrainSizeMin,None,Rect)
Rect.constrainSize = new_instancemethod(_cegui.Rect_constrainSize,None,Rect)
Rect.__eq__ = new_instancemethod(_cegui.Rect___eq__,None,Rect)
Rect.__ne__ = new_instancemethod(_cegui.Rect___ne__,None,Rect)
Rect.__mul__ = new_instancemethod(_cegui.Rect___mul__,None,Rect)
Rect.__imul__ = new_instancemethod(_cegui.Rect___imul__,None,Rect)
Rect_swigregister = _cegui.Rect_swigregister
Rect_swigregister(Rect)

class Vector2(object):
    thisown = property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, *args): 
        _cegui.Vector2_swiginit(self,_cegui.new_Vector2(*args))
    X = property(_cegui.Vector2_X_get, _cegui.Vector2_X_set)
    Y = property(_cegui.Vector2_Y_get, _cegui.Vector2_Y_set)
    __swig_destroy__ = _cegui.delete_Vector2
Vector2.__imul__ = new_instancemethod(_cegui.Vector2___imul__,None,Vector2)
Vector2.__idiv__ = new_instancemethod(_cegui.Vector2___idiv__,None,Vector2)
Vector2.__iadd__ = new_instancemethod(_cegui.Vector2___iadd__,None,Vector2)
Vector2.__isub__ = new_instancemethod(_cegui.Vector2___isub__,None,Vector2)
Vector2.__add__ = new_instancemethod(_cegui.Vector2___add__,None,Vector2)
Vector2.__sub__ = new_instancemethod(_cegui.Vector2___sub__,None,Vector2)
Vector2.__mul__ = new_instancemethod(_cegui.Vector2___mul__,None,Vector2)
Vector2.__eq__ = new_instancemethod(_cegui.Vector2___eq__,None,Vector2)
Vector2.__ne__ = new_instancemethod(_cegui.Vector2___ne__,None,Vector2)
Vector2.asSize = new_instancemethod(_cegui.Vector2_asSize,None,Vector2)
Vector2_swigregister = _cegui.Vector2_swigregister
Vector2_swigregister(Vector2)

class Vector3(object):
    thisown = property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, *args): 
        _cegui.Vector3_swiginit(self,_cegui.new_Vector3(*args))
    X = property(_cegui.Vector3_X_get, _cegui.Vector3_X_set)
    Y = property(_cegui.Vector3_Y_get, _cegui.Vector3_Y_set)
    Z = property(_cegui.Vector3_Z_get, _cegui.Vector3_Z_set)
    __swig_destroy__ = _cegui.delete_Vector3
Vector3_swigregister = _cegui.Vector3_swigregister
Vector3_swigregister(Vector3)

class String(object):
    thisown = property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    def __init__(self): raise AttributeError, "No constructor defined"
    __repr__ = _swig_repr
String.__str__ = new_instancemethod(_cegui.String___str__,None,String)
String_swigregister = _cegui.String_swigregister
String_swigregister(String)

class Size(object):
    thisown = property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, *args): 
        _cegui.Size_swiginit(self,_cegui.new_Size(*args))
    Width = property(_cegui.Size_Width_get, _cegui.Size_Width_set)
    Height = property(_cegui.Size_Height_get, _cegui.Size_Height_set)
    __swig_destroy__ = _cegui.delete_Size
Size.__eq__ = new_instancemethod(_cegui.Size___eq__,None,Size)
Size.__ne__ = new_instancemethod(_cegui.Size___ne__,None,Size)
Size_swigregister = _cegui.Size_swigregister
Size_swigregister(Size)

class Image(object):
    thisown = property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, *args): 
        _cegui.Image_swiginit(self,_cegui.new_Image(*args))
    __swig_destroy__ = _cegui.delete_Image
    Width = property(_cegui.Image_Width_get, None, None,
                    "Image.Width -> float  (read-only)\n\nThis is equivalent to calling the C++ CEGUI method:\n\tget: float Image::getWidth()")

    Height = property(_cegui.Image_Height_get, None, None,
                    "Image.Height -> float  (read-only)\n\nThis is equivalent to calling the C++ CEGUI method:\n\tget: float Image::getHeight()")

    ImagesetName = property(_cegui.Image_ImagesetName_get, None, None,
                    "Image.ImagesetName -> const CEGUI::String &  (read-only)\n\nThis is equivalent to calling the C++ CEGUI method:\n\tget: const CEGUI::String & Image::getImagesetName()")

    Size = property(_cegui.Image_Size_get, None, None,
                    "Image.Size -> CEGUI::Size  (read-only)\n\nThis is equivalent to calling the C++ CEGUI method:\n\tget: CEGUI::Size Image::getSize()")

    Offsets = property(_cegui.Image_Offsets_get, None, None,
                    "Image.Offsets -> CEGUI::Point  (read-only)\n\nThis is equivalent to calling the C++ CEGUI method:\n\tget: CEGUI::Point Image::getOffsets()")

    OffsetX = property(_cegui.Image_OffsetX_get, None, None,
                    "Image.OffsetX -> float  (read-only)\n\nThis is equivalent to calling the C++ CEGUI method:\n\tget: float Image::getOffsetX()")

    OffsetY = property(_cegui.Image_OffsetY_get, None, None,
                    "Image.OffsetY -> float  (read-only)\n\nThis is equivalent to calling the C++ CEGUI method:\n\tget: float Image::getOffsetY()")

    Mame = property(_cegui.Image_Mame_get, None, None,
                    "Image.Mame -> const CEGUI::String &  (read-only)\n\nThis is equivalent to calling the C++ CEGUI method:\n\tget: const CEGUI::String & Image::getName()")

    SourceTextureArea = property(_cegui.Image_SourceTextureArea_get, None, None,
                    "Image.SourceTextureArea -> const CEGUI::Rect &  (read-only)\n\nThis is equivalent to calling the C++ CEGUI method:\n\tget: const CEGUI::Rect & Image::getSourceTextureArea()")

Image.getImageset = new_instancemethod(_cegui.Image_getImageset,None,Image)
Image.draw = new_instancemethod(_cegui.Image_draw,None,Image)
Image.writeXMLToStream = new_instancemethod(_cegui.Image_writeXMLToStream,None,Image)
Image_swigregister = _cegui.Image_swigregister
Image_swigregister(Image)

class Imageset(object):
    thisown = property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    def __init__(self): raise AttributeError, "No constructor defined"
    __repr__ = _swig_repr
    __swig_destroy__ = _cegui.delete_Imageset
    setDefaultResourceGroup = staticmethod(_cegui.Imageset_setDefaultResourceGroup)
    getDefaultResourceGroup = staticmethod(_cegui.Imageset_getDefaultResourceGroup)
    Texture = property(_cegui.Imageset_Texture_get, None, None,
                    "Imageset.Texture -> CEGUI::Texture *  (read-only)\n\nThis is equivalent to calling the C++ CEGUI method:\n\tget: CEGUI::Texture * Imageset::getTexture()")

    Name = property(_cegui.Imageset_Name_get, None, None,
                    "Imageset.Name -> const CEGUI::String &  (read-only)\n\nThis is equivalent to calling the C++ CEGUI method:\n\tget: const CEGUI::String & Imageset::getName()")

    ImageCount = property(_cegui.Imageset_ImageCount_get, None, None,
                    "Imageset.ImageCount -> CEGUI::uint  (read-only)\n\nThis is equivalent to calling the C++ CEGUI method:\n\tget: CEGUI::uint Imageset::getImageCount()")

    AutoScaled = property(_cegui.Imageset_AutoScaled_get, _cegui.Imageset_AutoScaled_set, None,
                    "Imageset.AutoScaled -> bool\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: bool Imageset::isAutoScaled()\n\tset: void Imageset::setAutoScalingEnabled(bool)")


    NativeResolution = property(_cegui.Imageset_NativeResolution_get, _cegui.Imageset_NativeResolution_set, None,
                    "Imageset.NativeResolution -> CEGUI::Size\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: CEGUI::Size Imageset::getNativeResolution()\n\tset: void Imageset::setNativeResolution(CEGUI::Size)")


Imageset.isImageDefined = new_instancemethod(_cegui.Imageset_isImageDefined,None,Imageset)
Imageset.getImage = new_instancemethod(_cegui.Imageset_getImage,None,Imageset)
Imageset.undefineImage = new_instancemethod(_cegui.Imageset_undefineImage,None,Imageset)
Imageset.undefineAllImages = new_instancemethod(_cegui.Imageset_undefineAllImages,None,Imageset)
Imageset.getImageSize = new_instancemethod(_cegui.Imageset_getImageSize,None,Imageset)
Imageset.getImageWidth = new_instancemethod(_cegui.Imageset_getImageWidth,None,Imageset)
Imageset.getImageHeight = new_instancemethod(_cegui.Imageset_getImageHeight,None,Imageset)
Imageset.getImageOffset = new_instancemethod(_cegui.Imageset_getImageOffset,None,Imageset)
Imageset.getImageOffsetX = new_instancemethod(_cegui.Imageset_getImageOffsetX,None,Imageset)
Imageset.getImageOffsetY = new_instancemethod(_cegui.Imageset_getImageOffsetY,None,Imageset)
Imageset.defineImage = new_instancemethod(_cegui.Imageset_defineImage,None,Imageset)
Imageset.draw = new_instancemethod(_cegui.Imageset_draw,None,Imageset)
Imageset.notifyScreenResolution = new_instancemethod(_cegui.Imageset_notifyScreenResolution,None,Imageset)
Imageset.writeXMLToStream = new_instancemethod(_cegui.Imageset_writeXMLToStream,None,Imageset)
Imageset_swigregister = _cegui.Imageset_swigregister
Imageset_swigregister(Imageset)
Imageset_setDefaultResourceGroup = _cegui.Imageset_setDefaultResourceGroup
Imageset_getDefaultResourceGroup = _cegui.Imageset_getDefaultResourceGroup

class colour(object):
    thisown = property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, *args): 
        _cegui.colour_swiginit(self,_cegui.new_colour(*args))
    A = property(_cegui.colour_A_get, _cegui.colour_A_set, None,
                    "colour.A -> float\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: float colour::getAlpha()\n\tset: void colour::setAlpha(float)")


    B = property(_cegui.colour_B_get, _cegui.colour_B_set, None,
                    "colour.B -> float\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: float colour::getBlue()\n\tset: void colour::setBlue(float)")


    R = property(_cegui.colour_R_get, _cegui.colour_R_set, None,
                    "colour.R -> float\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: float colour::getRed()\n\tset: void colour::setRed(float)")


    G = property(_cegui.colour_G_get, _cegui.colour_G_set, None,
                    "colour.G -> float\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: float colour::getGreen()\n\tset: void colour::setGreen(float)")


    ARGB = property(_cegui.colour_ARGB_get, _cegui.colour_ARGB_set, None,
                    "colour.ARGB -> CEGUI::ulong\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: CEGUI::ulong colour::getARGB()\n\tset: void colour::setARGB(CEGUI::ulong)")


    __swig_destroy__ = _cegui.delete_colour
colour.getHue = new_instancemethod(_cegui.colour_getHue,None,colour)
colour.getSaturation = new_instancemethod(_cegui.colour_getSaturation,None,colour)
colour.getLumination = new_instancemethod(_cegui.colour_getLumination,None,colour)
colour.set = new_instancemethod(_cegui.colour_set,None,colour)
colour.setRGB = new_instancemethod(_cegui.colour_setRGB,None,colour)
colour.setHSL = new_instancemethod(_cegui.colour_setHSL,None,colour)
colour.invertColour = new_instancemethod(_cegui.colour_invertColour,None,colour)
colour.invertColourWithAlpha = new_instancemethod(_cegui.colour_invertColourWithAlpha,None,colour)
colour.__iand__ = new_instancemethod(_cegui.colour___iand__,None,colour)
colour.__ior__ = new_instancemethod(_cegui.colour___ior__,None,colour)
colour.__ilshift__ = new_instancemethod(_cegui.colour___ilshift__,None,colour)
colour.__irshift__ = new_instancemethod(_cegui.colour___irshift__,None,colour)
colour.__add__ = new_instancemethod(_cegui.colour___add__,None,colour)
colour.__sub__ = new_instancemethod(_cegui.colour___sub__,None,colour)
colour.__mul__ = new_instancemethod(_cegui.colour___mul__,None,colour)
colour.__imul__ = new_instancemethod(_cegui.colour___imul__,None,colour)
colour.__eq__ = new_instancemethod(_cegui.colour___eq__,None,colour)
colour.__ne__ = new_instancemethod(_cegui.colour___ne__,None,colour)
colour_swigregister = _cegui.colour_swigregister
colour_swigregister(colour)

class ColourRect(object):
    thisown = property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, *args): 
        _cegui.ColourRect_swiginit(self,_cegui.new_ColourRect(*args))
    TopLeft = property(_cegui.ColourRect_TopLeft_get, _cegui.ColourRect_TopLeft_set)
    TopRight = property(_cegui.ColourRect_TopRight_get, _cegui.ColourRect_TopRight_set)
    BottomLeft = property(_cegui.ColourRect_BottomLeft_get, _cegui.ColourRect_BottomLeft_set)
    BottomRight = property(_cegui.ColourRect_BottomRight_get, _cegui.ColourRect_BottomRight_set)
    __swig_destroy__ = _cegui.delete_ColourRect
ColourRect.setAlpha = new_instancemethod(_cegui.ColourRect_setAlpha,None,ColourRect)
ColourRect.setTopAlpha = new_instancemethod(_cegui.ColourRect_setTopAlpha,None,ColourRect)
ColourRect.setBottomAlpha = new_instancemethod(_cegui.ColourRect_setBottomAlpha,None,ColourRect)
ColourRect.setLeftAlpha = new_instancemethod(_cegui.ColourRect_setLeftAlpha,None,ColourRect)
ColourRect.setRightAlpha = new_instancemethod(_cegui.ColourRect_setRightAlpha,None,ColourRect)
ColourRect.isMonochromatic = new_instancemethod(_cegui.ColourRect_isMonochromatic,None,ColourRect)
ColourRect.getSubRectangle = new_instancemethod(_cegui.ColourRect_getSubRectangle,None,ColourRect)
ColourRect.getColourAtPoint = new_instancemethod(_cegui.ColourRect_getColourAtPoint,None,ColourRect)
ColourRect.setColours = new_instancemethod(_cegui.ColourRect_setColours,None,ColourRect)
ColourRect.modulateAlpha = new_instancemethod(_cegui.ColourRect_modulateAlpha,None,ColourRect)
ColourRect.__imul__ = new_instancemethod(_cegui.ColourRect___imul__,None,ColourRect)
ColourRect_swigregister = _cegui.ColourRect_swigregister
ColourRect_swigregister(ColourRect)

class UDim(object):
    thisown = property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, *args): 
        _cegui.UDim_swiginit(self,_cegui.new_UDim(*args))
    __swig_destroy__ = _cegui.delete_UDim
    Scale = property(_cegui.UDim_Scale_get, _cegui.UDim_Scale_set)
    Offset = property(_cegui.UDim_Offset_get, _cegui.UDim_Offset_set)
UDim.asAbsolute = new_instancemethod(_cegui.UDim_asAbsolute,None,UDim)
UDim.asRelative = new_instancemethod(_cegui.UDim_asRelative,None,UDim)
UDim.__add__ = new_instancemethod(_cegui.UDim___add__,None,UDim)
UDim.__sub__ = new_instancemethod(_cegui.UDim___sub__,None,UDim)
UDim.__div__ = new_instancemethod(_cegui.UDim___div__,None,UDim)
UDim.__mul__ = new_instancemethod(_cegui.UDim___mul__,None,UDim)
UDim.__iadd__ = new_instancemethod(_cegui.UDim___iadd__,None,UDim)
UDim.__isub__ = new_instancemethod(_cegui.UDim___isub__,None,UDim)
UDim.__idiv__ = new_instancemethod(_cegui.UDim___idiv__,None,UDim)
UDim.__imul__ = new_instancemethod(_cegui.UDim___imul__,None,UDim)
UDim.__eq__ = new_instancemethod(_cegui.UDim___eq__,None,UDim)
UDim.__ne__ = new_instancemethod(_cegui.UDim___ne__,None,UDim)
UDim_swigregister = _cegui.UDim_swigregister
UDim_swigregister(UDim)

class UVector2(object):
    thisown = property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, *args): 
        _cegui.UVector2_swiginit(self,_cegui.new_UVector2(*args))
    __swig_destroy__ = _cegui.delete_UVector2
    X = property(_cegui.UVector2_X_get, _cegui.UVector2_X_set)
    Y = property(_cegui.UVector2_Y_get, _cegui.UVector2_Y_set)
UVector2.asAbsolute = new_instancemethod(_cegui.UVector2_asAbsolute,None,UVector2)
UVector2.asRelative = new_instancemethod(_cegui.UVector2_asRelative,None,UVector2)
UVector2.__add__ = new_instancemethod(_cegui.UVector2___add__,None,UVector2)
UVector2.__sub__ = new_instancemethod(_cegui.UVector2___sub__,None,UVector2)
UVector2.__div__ = new_instancemethod(_cegui.UVector2___div__,None,UVector2)
UVector2.__mul__ = new_instancemethod(_cegui.UVector2___mul__,None,UVector2)
UVector2.__iadd__ = new_instancemethod(_cegui.UVector2___iadd__,None,UVector2)
UVector2.__isub__ = new_instancemethod(_cegui.UVector2___isub__,None,UVector2)
UVector2.__idiv__ = new_instancemethod(_cegui.UVector2___idiv__,None,UVector2)
UVector2.__imul__ = new_instancemethod(_cegui.UVector2___imul__,None,UVector2)
UVector2.__eq__ = new_instancemethod(_cegui.UVector2___eq__,None,UVector2)
UVector2.__ne__ = new_instancemethod(_cegui.UVector2___ne__,None,UVector2)
UVector2_swigregister = _cegui.UVector2_swigregister
UVector2_swigregister(UVector2)

class URect(object):
    thisown = property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, *args): 
        _cegui.URect_swiginit(self,_cegui.new_URect(*args))
    __swig_destroy__ = _cegui.delete_URect
    Min = property(_cegui.URect_Min_get, _cegui.URect_Min_set)
    Max = property(_cegui.URect_Max_get, _cegui.URect_Max_set)
    Width = property(_cegui.URect_Width_get, _cegui.URect_Width_set, None,
                    "URect.Width -> CEGUI::UDim\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: CEGUI::UDim URect::getWidth()\n\tset: void URect::setWidth(CEGUI::UDim)")


    Height = property(_cegui.URect_Height_get, _cegui.URect_Height_set, None,
                    "URect.Height -> CEGUI::UDim\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: CEGUI::UDim URect::getHeight()\n\tset: void URect::setHeight(CEGUI::UDim)")


    Size = property(_cegui.URect_Size_get, _cegui.URect_Size_set, None,
                    "URect.Size -> CEGUI::UVector2\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: CEGUI::UVector2 URect::getSize()\n\tset: void URect::setSize(CEGUI::UVector2)")


    Position = property(_cegui.URect_Position_get, _cegui.URect_Position_set, None,
                    "URect.Position -> const CEGUI::UVector2 &\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: const CEGUI::UVector2 & URect::getPosition()\n\tset: void URect::setPosition(const CEGUI::UVector2 &)")


URect.asAbsolute = new_instancemethod(_cegui.URect_asAbsolute,None,URect)
URect.asRelative = new_instancemethod(_cegui.URect_asRelative,None,URect)
URect.offset = new_instancemethod(_cegui.URect_offset,None,URect)
URect_swigregister = _cegui.URect_swigregister
URect_swigregister(URect)

class Exception:
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, Exception, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, Exception, name)
    def __init__(self): raise AttributeError, "No constructor defined"
    __repr__ = _swig_repr
    __swig_destroy__ = _cegui.delete_Exception
Exception_swigregister = _cegui.Exception_swigregister
Exception_swigregister(Exception)

CEGUIException = Exception

class BoundSlot(object):
    thisown = property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, *args): 
        _cegui.BoundSlot_swiginit(self,_cegui.new_BoundSlot(*args))
    __swig_destroy__ = _cegui.delete_BoundSlot
BoundSlot.connected = new_instancemethod(_cegui.BoundSlot_connected,None,BoundSlot)
BoundSlot.disconnect = new_instancemethod(_cegui.BoundSlot_disconnect,None,BoundSlot)
BoundSlot_swigregister = _cegui.BoundSlot_swigregister
BoundSlot_swigregister(BoundSlot)

class _EventConnection(object):
    thisown = property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, *args): 
        _cegui._EventConnection_swiginit(self,_cegui.new__EventConnection(*args))
    __swig_destroy__ = _cegui.delete__EventConnection
_EventConnection.__ref__ = new_instancemethod(_cegui._EventConnection___ref__,None,_EventConnection)
_EventConnection.isValid = new_instancemethod(_cegui._EventConnection_isValid,None,_EventConnection)
_EventConnection_swigregister = _cegui._EventConnection_swigregister
_EventConnection_swigregister(_EventConnection)

class Event(object):
    thisown = property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, *args): 
        _cegui.Event_swiginit(self,_cegui.new_Event(*args))
    __swig_destroy__ = _cegui.delete_Event
    Name = property(_cegui.Event_Name_get, None, None,
                    "Event.Name -> const CEGUI::String &  (read-only)\n\nThis is equivalent to calling the C++ CEGUI method:\n\tget: const CEGUI::String & Event::getName()")

Event.__call__ = new_instancemethod(_cegui.Event___call__,None,Event)
Event_swigregister = _cegui.Event_swigregister
Event_swigregister(Event)

class EventSet(object):
    thisown = property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, *args): 
        _cegui.EventSet_swiginit(self,_cegui.new_EventSet(*args))
    __swig_destroy__ = _cegui.delete_EventSet
    Muted = property(_cegui.EventSet_Muted_get, _cegui.EventSet_Muted_set, None,
                    "EventSet.Muted -> bool\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: bool EventSet::isMuted()\n\tset: void EventSet::setMutedState(bool)")


EventSet.addEvent = new_instancemethod(_cegui.EventSet_addEvent,None,EventSet)
EventSet.removeEvent = new_instancemethod(_cegui.EventSet_removeEvent,None,EventSet)
EventSet.removeAllEvents = new_instancemethod(_cegui.EventSet_removeAllEvents,None,EventSet)
EventSet.isEventPresent = new_instancemethod(_cegui.EventSet_isEventPresent,None,EventSet)
EventSet.subscribeScriptedEvent = new_instancemethod(_cegui.EventSet_subscribeScriptedEvent,None,EventSet)
EventSet.subscribeEvent = new_instancemethod(_cegui.EventSet_subscribeEvent,None,EventSet)
EventSet.fireEvent = new_instancemethod(_cegui.EventSet_fireEvent,None,EventSet)
EventSet_swigregister = _cegui.EventSet_swigregister
EventSet_swigregister(EventSet)

FlipHorizontal = _cegui.FlipHorizontal
FlipVertical = _cegui.FlipVertical
RotateRightAngle = _cegui.RotateRightAngle
TopLeftToBottomRight = _cegui.TopLeftToBottomRight
BottomLeftToTopRight = _cegui.BottomLeftToTopRight
class Renderer(EventSet):
    thisown = property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    def __init__(self): raise AttributeError, "No constructor defined"
    __repr__ = _swig_repr
    __swig_destroy__ = _cegui.delete_Renderer
    EventNamespace = "Renderer" 
    EventDisplaySizeChanged = "DisplayModeChanged" 
    QueueingEnabled = property(_cegui.Renderer_QueueingEnabled_get, _cegui.Renderer_QueueingEnabled_set, None,
                    "Renderer.QueueingEnabled -> bool\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: bool Renderer::isQueueingEnabled()\n\tset: void Renderer::setQueueingEnabled(bool)")


    Width = property(_cegui.Renderer_Width_get, None, None,
                    "Renderer.Width -> float  (read-only)\n\nThis is equivalent to calling the C++ CEGUI method:\n\tget: float Renderer::getWidth()")

    Height = property(_cegui.Renderer_Height_get, None, None,
                    "Renderer.Height -> float  (read-only)\n\nThis is equivalent to calling the C++ CEGUI method:\n\tget: float Renderer::getHeight()")

    Size = property(_cegui.Renderer_Size_get, None, None,
                    "Renderer.Size -> CEGUI::Size  (read-only)\n\nThis is equivalent to calling the C++ CEGUI method:\n\tget: CEGUI::Size Renderer::getSize()")

    Rect = property(_cegui.Renderer_Rect_get, None, None,
                    "Renderer.Rect -> CEGUI::Rect  (read-only)\n\nThis is equivalent to calling the C++ CEGUI method:\n\tget: CEGUI::Rect Renderer::getRect()")

    MaxTextureSize = property(_cegui.Renderer_MaxTextureSize_get, None, None,
                    "Renderer.MaxTextureSize -> CEGUI::uint  (read-only)\n\nThis is equivalent to calling the C++ CEGUI method:\n\tget: CEGUI::uint Renderer::getMaxTextureSize()")

    HorzScreenDPI = property(_cegui.Renderer_HorzScreenDPI_get, None, None,
                    "Renderer.HorzScreenDPI -> CEGUI::uint  (read-only)\n\nThis is equivalent to calling the C++ CEGUI method:\n\tget: CEGUI::uint Renderer::getHorzScreenDPI()")

    VertScreenDPI = property(_cegui.Renderer_VertScreenDPI_get, None, None,
                    "Renderer.VertScreenDPI -> CEGUI::uint  (read-only)\n\nThis is equivalent to calling the C++ CEGUI method:\n\tget: CEGUI::uint Renderer::getVertScreenDPI()")

    ZValue = property(_cegui.Renderer_ZValue_get, None, None,
                    "Renderer.ZValue -> float  (read-only)\n\nThis is equivalent to calling the C++ CEGUI method:\n\tget: float Renderer::getCurrentZ()")

    IdentifierString = property(_cegui.Renderer_IdentifierString_get, None, None,
                    "Renderer.IdentifierString -> const CEGUI::String &  (read-only)\n\nThis is equivalent to calling the C++ CEGUI method:\n\tget: const CEGUI::String & Renderer::getIdentifierString()")

Renderer.addQuad = new_instancemethod(_cegui.Renderer_addQuad,None,Renderer)
Renderer.doRender = new_instancemethod(_cegui.Renderer_doRender,None,Renderer)
Renderer.clearRenderList = new_instancemethod(_cegui.Renderer_clearRenderList,None,Renderer)
Renderer.createTexture = new_instancemethod(_cegui.Renderer_createTexture,None,Renderer)
Renderer.destroyTexture = new_instancemethod(_cegui.Renderer_destroyTexture,None,Renderer)
Renderer.destroyAllTextures = new_instancemethod(_cegui.Renderer_destroyAllTextures,None,Renderer)
Renderer.resetZValue = new_instancemethod(_cegui.Renderer_resetZValue,None,Renderer)
Renderer.advanceZValue = new_instancemethod(_cegui.Renderer_advanceZValue,None,Renderer)
Renderer.getZLayer = new_instancemethod(_cegui.Renderer_getZLayer,None,Renderer)
Renderer.createResourceProvider = new_instancemethod(_cegui.Renderer_createResourceProvider,None,Renderer)
Renderer_swigregister = _cegui.Renderer_swigregister
Renderer_swigregister(Renderer)

class ResourceProvider(object):
    thisown = property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    def __init__(self): raise AttributeError, "No constructor defined"
    __repr__ = _swig_repr
    __swig_destroy__ = _cegui.delete_ResourceProvider
    DefaultResourceGroup = property(_cegui.ResourceProvider_DefaultResourceGroup_get, _cegui.ResourceProvider_DefaultResourceGroup_set, None,
                    "ResourceProvider.DefaultResourceGroup -> const CEGUI::String &\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: const CEGUI::String & ResourceProvider::getDefaultResourceGroup()\n\tset: void ResourceProvider::setDefaultResourceGroup(const CEGUI::String &)")


ResourceProvider.loadRawDataContainer = new_instancemethod(_cegui.ResourceProvider_loadRawDataContainer,None,ResourceProvider)
ResourceProvider.unloadRawDataContainer = new_instancemethod(_cegui.ResourceProvider_unloadRawDataContainer,None,ResourceProvider)
ResourceProvider_swigregister = _cegui.ResourceProvider_swigregister
ResourceProvider_swigregister(ResourceProvider)

class PropertyReceiver(object):
    thisown = property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, *args): 
        _cegui.PropertyReceiver_swiginit(self,_cegui.new_PropertyReceiver(*args))
    __swig_destroy__ = _cegui.delete_PropertyReceiver
PropertyReceiver_swigregister = _cegui.PropertyReceiver_swigregister
PropertyReceiver_swigregister(PropertyReceiver)

class Property(object):
    thisown = property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    def __init__(self): raise AttributeError, "No constructor defined"
    __repr__ = _swig_repr
    __swig_destroy__ = _cegui.delete_Property
    Help = property(_cegui.Property_Help_get, None, None,
                    "Property.Help -> const CEGUI::String &  (read-only)\n\nThis is equivalent to calling the C++ CEGUI method:\n\tget: const CEGUI::String & Property::getHelp()")

    Name = property(_cegui.Property_Name_get, None, None,
                    "Property.Name -> const CEGUI::String &  (read-only)\n\nThis is equivalent to calling the C++ CEGUI method:\n\tget: const CEGUI::String & Property::getName()")

Property.get = new_instancemethod(_cegui.Property_get,None,Property)
Property.set = new_instancemethod(_cegui.Property_set,None,Property)
Property.isDefault = new_instancemethod(_cegui.Property_isDefault,None,Property)
Property.getDefault = new_instancemethod(_cegui.Property_getDefault,None,Property)
Property.writeXMLToStream = new_instancemethod(_cegui.Property_writeXMLToStream,None,Property)
Property_swigregister = _cegui.Property_swigregister
Property_swigregister(Property)

class PropertySet(PropertyReceiver):
    thisown = property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, *args): 
        _cegui.PropertySet_swiginit(self,_cegui.new_PropertySet(*args))
    __swig_destroy__ = _cegui.delete_PropertySet
PropertySet.addProperty = new_instancemethod(_cegui.PropertySet_addProperty,None,PropertySet)
PropertySet.removeProperty = new_instancemethod(_cegui.PropertySet_removeProperty,None,PropertySet)
PropertySet.clearProperties = new_instancemethod(_cegui.PropertySet_clearProperties,None,PropertySet)
PropertySet.isPropertyPresent = new_instancemethod(_cegui.PropertySet_isPropertyPresent,None,PropertySet)
PropertySet.getPropertyHelp = new_instancemethod(_cegui.PropertySet_getPropertyHelp,None,PropertySet)
PropertySet.getProperty = new_instancemethod(_cegui.PropertySet_getProperty,None,PropertySet)
PropertySet.setProperty = new_instancemethod(_cegui.PropertySet_setProperty,None,PropertySet)
PropertySet.isPropertyDefault = new_instancemethod(_cegui.PropertySet_isPropertyDefault,None,PropertySet)
PropertySet.getPropertyDefault = new_instancemethod(_cegui.PropertySet_getPropertyDefault,None,PropertySet)
PropertySet_swigregister = _cegui.PropertySet_swigregister
PropertySet_swigregister(PropertySet)

LeftAligned = _cegui.LeftAligned
RightAligned = _cegui.RightAligned
Centred = _cegui.Centred
Justified = _cegui.Justified
WordWrapLeftAligned = _cegui.WordWrapLeftAligned
WordWrapRightAligned = _cegui.WordWrapRightAligned
WordWrapCentred = _cegui.WordWrapCentred
WordWrapJustified = _cegui.WordWrapJustified
class FontGlyph(object):
    thisown = property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, *args): 
        _cegui.FontGlyph_swiginit(self,_cegui.new_FontGlyph(*args))
    __swig_destroy__ = _cegui.delete_FontGlyph
FontGlyph.getImage = new_instancemethod(_cegui.FontGlyph_getImage,None,FontGlyph)
FontGlyph.getImageset = new_instancemethod(_cegui.FontGlyph_getImageset,None,FontGlyph)
FontGlyph.getSize = new_instancemethod(_cegui.FontGlyph_getSize,None,FontGlyph)
FontGlyph.getWidth = new_instancemethod(_cegui.FontGlyph_getWidth,None,FontGlyph)
FontGlyph.getHeight = new_instancemethod(_cegui.FontGlyph_getHeight,None,FontGlyph)
FontGlyph.getRenderedAdvance = new_instancemethod(_cegui.FontGlyph_getRenderedAdvance,None,FontGlyph)
FontGlyph.getAdvance = new_instancemethod(_cegui.FontGlyph_getAdvance,None,FontGlyph)
FontGlyph.setAdvance = new_instancemethod(_cegui.FontGlyph_setAdvance,None,FontGlyph)
FontGlyph.setImage = new_instancemethod(_cegui.FontGlyph_setImage,None,FontGlyph)
FontGlyph_swigregister = _cegui.FontGlyph_swigregister
FontGlyph_swigregister(FontGlyph)

class Font(PropertySet):
    thisown = property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    def __init__(self): raise AttributeError, "No constructor defined"
    __repr__ = _swig_repr
    setDefaultResourceGroup = staticmethod(_cegui.Font_setDefaultResourceGroup)
    getDefaultResourceGroup = staticmethod(_cegui.Font_getDefaultResourceGroup)
Font.load = new_instancemethod(_cegui.Font_load,None,Font)
Font.isCodepointAvailable = new_instancemethod(_cegui.Font_isCodepointAvailable,None,Font)
Font.drawText = new_instancemethod(_cegui.Font_drawText,None,Font)
Font.setNativeResolution = new_instancemethod(_cegui.Font_setNativeResolution,None,Font)
Font.notifyScreenResolution = new_instancemethod(_cegui.Font_notifyScreenResolution,None,Font)
Font.getLineSpacing = new_instancemethod(_cegui.Font_getLineSpacing,None,Font)
Font.getFontHeight = new_instancemethod(_cegui.Font_getFontHeight,None,Font)
Font.getBaseline = new_instancemethod(_cegui.Font_getBaseline,None,Font)
Font.getTextExtent = new_instancemethod(_cegui.Font_getTextExtent,None,Font)
Font.getCharAtPixel = new_instancemethod(_cegui.Font_getCharAtPixel,None,Font)
Font.getFormattedLineCount = new_instancemethod(_cegui.Font_getFormattedLineCount,None,Font)
Font.getFormattedTextExtent = new_instancemethod(_cegui.Font_getFormattedTextExtent,None,Font)
Font_swigregister = _cegui.Font_swigregister
Font_swigregister(Font)
Font.DefaultColour = _cegui.cvar.Font_DefaultColour
Font_setDefaultResourceGroup = _cegui.Font_setDefaultResourceGroup
Font_getDefaultResourceGroup = _cegui.Font_getDefaultResourceGroup

class ListboxItem(object):
    thisown = property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    def __init__(self): raise AttributeError, "No constructor defined"
    __repr__ = _swig_repr
    __swig_destroy__ = _cegui.delete_ListboxItem


    SelectionColours = property(_cegui.ListboxItem___getSelectionColours, fix_args(_cegui.ListboxItem___setSelectionColours), None,
                    "ListboxItem.SelectionColours -> type\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: ListboxItem::getSelectionColours()\n\tset: void ListboxItem::setSelectionColours(...)")

    SelectionBrushImage = property(_cegui.ListboxItem___getSelectionBrushImage, fix_args(_cegui.ListboxItem___setSelectionBrushImage), None,
                    "ListboxItem.SelectionBrushImage -> type\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: ListboxItem::getSelectionBrushImage()\n\tset: void ListboxItem::setSelectionBrushImage(...)")

    Text = property(_cegui.ListboxItem_Text_get, _cegui.ListboxItem_Text_set, None,
                    "ListboxItem.Text -> const CEGUI::String &\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: const CEGUI::String & ListboxItem::getText()\n\tset: void ListboxItem::setText(const CEGUI::String &)")


    ID = property(_cegui.ListboxItem_ID_get, _cegui.ListboxItem_ID_set, None,
                    "ListboxItem.ID -> CEGUI::uint\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: CEGUI::uint ListboxItem::getID()\n\tset: void ListboxItem::setID(CEGUI::uint)")


    Selected = property(_cegui.ListboxItem_Selected_get, _cegui.ListboxItem_Selected_set, None,
                    "ListboxItem.Selected -> bool\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: bool ListboxItem::isSelected()\n\tset: void ListboxItem::setSelected(bool)")


    Disabled = property(_cegui.ListboxItem_Disabled_get, _cegui.ListboxItem_Disabled_set, None,
                    "ListboxItem.Disabled -> bool\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: bool ListboxItem::isDisabled()\n\tset: void ListboxItem::setDisabled(bool)")


    AutoDeleted = property(_cegui.ListboxItem_AutoDeleted_get, _cegui.ListboxItem_AutoDeleted_set, None,
                    "ListboxItem.AutoDeleted -> bool\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: bool ListboxItem::isAutoDeleted()\n\tset: void ListboxItem::setAutoDeleted(bool)")


    PixelSize = property(_cegui.ListboxItem_PixelSize_get, None, None,
                    "ListboxItem.PixelSize -> CEGUI::Size  (read-only)\n\nThis is equivalent to calling the C++ CEGUI method:\n\tget: CEGUI::Size ListboxItem::getPixelSize()")

    TooltipText = property(_cegui.ListboxItem_TooltipText_get, _cegui.ListboxItem_TooltipText_set, None,
                    "ListboxItem.TooltipText -> const CEGUI::String &\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: const CEGUI::String & ListboxItem::getTooltipText()\n\tset: void ListboxItem::setTooltipText(const CEGUI::String &)")


    OwnerWindow = property(_cegui.ListboxItem_OwnerWindow_get, _cegui.ListboxItem_OwnerWindow_set, None,
                    "ListboxItem.OwnerWindow -> const CEGUI::Window *\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: const CEGUI::Window * ListboxItem::getOwnerWindow()\n\tset: void ListboxItem::setOwnerWindow(const CEGUI::Window *)")


ListboxItem.draw = new_instancemethod(_cegui.ListboxItem_draw,None,ListboxItem)
ListboxItem.__lt__ = new_instancemethod(_cegui.ListboxItem___lt__,None,ListboxItem)
ListboxItem.__gt__ = new_instancemethod(_cegui.ListboxItem___gt__,None,ListboxItem)
ListboxItem_swigregister = _cegui.ListboxItem_swigregister
ListboxItem_swigregister(ListboxItem)
ListboxItem.DefaultSelectionColour = _cegui.cvar.ListboxItem_DefaultSelectionColour

class ListboxTextItem(ListboxItem):
    thisown = property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    __swig_destroy__ = _cegui.delete_ListboxTextItem


    Font = property(_cegui.ListboxTextItem___getFont, fix_args(_cegui.ListboxTextItem___setFont), None,
                    "ListboxTextItem.Font -> type\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: ListboxTextItem::getFont()\n\tset: void ListboxTextItem::setFont(...)")

    TextColours = property(_cegui.ListboxTextItem___getTextColours, fix_args(_cegui.ListboxTextItem___setTextColours), None,
                    "ListboxTextItem.TextColours -> type\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: ListboxTextItem::getTextColours()\n\tset: void ListboxTextItem::setTextColours(...)")

    def __init__(self, *args): 
        _cegui.ListboxTextItem_swiginit(self,_cegui.new_ListboxTextItem(*args))
ListboxTextItem_swigregister = _cegui.ListboxTextItem_swigregister
ListboxTextItem_swigregister(ListboxTextItem)
ListboxTextItem.DefaultTextColour = _cegui.cvar.ListboxTextItem_DefaultTextColour

class WindowFactory(object):
    thisown = property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    def __init__(self): raise AttributeError, "No constructor defined"
    __repr__ = _swig_repr
    __swig_destroy__ = _cegui.delete_WindowFactory
    typeName = property(_cegui.WindowFactory_typeName_get, None, None,
                    "WindowFactory.typeName -> const CEGUI::String &  (read-only)\n\nThis is equivalent to calling the C++ CEGUI method:\n\tget: const CEGUI::String & WindowFactory::getTypeName()")

WindowFactory.createWindow = new_instancemethod(_cegui.WindowFactory_createWindow,None,WindowFactory)
WindowFactory.destroyWindow = new_instancemethod(_cegui.WindowFactory_destroyWindow,None,WindowFactory)
WindowFactory_swigregister = _cegui.WindowFactory_swigregister
WindowFactory_swigregister(WindowFactory)

VA_TOP = _cegui.VA_TOP
VA_CENTRE = _cegui.VA_CENTRE
VA_BOTTOM = _cegui.VA_BOTTOM
HA_LEFT = _cegui.HA_LEFT
HA_CENTRE = _cegui.HA_CENTRE
HA_RIGHT = _cegui.HA_RIGHT
class Window(PropertySet,EventSet):
    thisown = property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, *args): 
        _cegui.Window_swiginit(self,_cegui.new_Window(*args))
    __swig_destroy__ = _cegui.delete_Window
    getCaptureWindow = staticmethod(_cegui.Window_getCaptureWindow)


    MouseCursor = property(_cegui.Window___getMouseCursor, fix_args(_cegui.Window___setMouseCursor), None,
                    "Window.MouseCursor -> type\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: Window::getMouseCursor()\n\tset: void Window::setMouseCursor(...)")

    LookNFeel = property(_cegui.Window___getLookNFeel, fix_args(_cegui.Window___setLookNFeel), None,
                    "Window.LookNFeel -> type\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: Window::getLookNFeel()\n\tset: void Window::setLookNFeel(...)")

    WindowArea = property(_cegui.Window___getArea, fix_args(_cegui.Window___setArea), None,
                    "Window.WindowArea -> type\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: Window::getArea()\n\tset: void Window::setArea(...)")


    EventNamespace = "Window" 
    EventActivated = "Activated" 
    EventAlphaChanged = "AlphaChanged" 
    EventAlwaysOnTopChanged = "AlwaysOnTopChanged" 
    EventCharacterKey = "CharacterKey" 
    EventChildAdded = "AddedChild" 
    EventChildRemoved = "RemovedChild" 
    EventClippedByParentChanged = "ClippingChanged" 
    EventDeactivated = "Deactivated" 
    EventDestroyedByParentChanged = "DestroyedByParentChanged" 
    EventDestructionStarted = "DestructStart" 
    EventDisabled = "Disabled" 
    EventDragDropItemDropped = "DragDropItemDropped" 
    EventDragDropItemEnters = "DragDropItemEnters" 
    EventDragDropItemLeaves = "DragDropItemLeaves" 
    EventEnabled = "Enabled" 
    EventFontChanged = "FontChanged" 
    EventHidden = "Hidden" 
    EventHorizontalAlignmentChanged = "HorizontalAlignmentChanged" 
    EventIDChanged = "IDChanged" 
    EventInheritsAlphaChanged = "InheritAlphaChanged" 
    EventInputCaptureGained = "CaptureGained" 
    EventInputCaptureLost = "CaptureLost" 
    EventKeyDown = "KeyDown" 
    EventKeyUp = "KeyUp" 
    EventMouseButtonDown = "MouseButtonDown" 
    EventMouseButtonUp = "MouseButtonUp" 
    EventMouseClick = "MouseClick" 
    EventMouseDoubleClick = "MouseDoubleClick" 
    EventMouseEnters = "MouseEnter" 
    EventMouseLeaves = "MouseLeave" 
    EventMouseMove = "MouseMove" 
    EventMouseTripleClick = "MouseTripleClick" 
    EventMouseWheel = "MouseWheel" 
    EventMoved = "Moved" 
    EventParentSized = "ParentSized" 
    EventRenderingEnded = "EndRender" 
    EventRenderingStarted = "StartRender" 
    EventShown = "Shown" 
    EventSized = "Sized" 
    EventTextChanged = "TextChanged" 
    EventVerticalAlignmentChanged = "VerticalAlignmentChanged" 
    EventWindowRendererAttached = "WindowRendererAttached" 
    EventWindowRendererDetached = "WindowRendererDetached" 
    EventZOrderChanged = "ZChanged" 
    Enabled = property(_cegui.Window_Enabled_get, _cegui.Window_Enabled_set)
    Type = property(_cegui.Window_Type_get, None, None,
                    "Window.Type -> const String &  (read-only)\n\nThis is equivalent to calling the C++ CEGUI method:\n\tget: const String & Window::getType()")

    Name = property(_cegui.Window_Name_get, None, None,
                    "Window.Name -> const String &  (read-only)\n\nThis is equivalent to calling the C++ CEGUI method:\n\tget: const String & Window::getName()")

    ID = property(_cegui.Window_ID_get, _cegui.Window_ID_set, None,
                    "Window.ID -> CEGUI::uint\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: CEGUI::uint Window::getID()\n\tset: void Window::setID(CEGUI::uint)")


    Alpha = property(_cegui.Window_Alpha_get, _cegui.Window_Alpha_set, None,
                    "Window.Alpha -> float\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: float Window::getAlpha()\n\tset: void Window::setAlpha(float)")


    Font = property(_cegui.Window_Font_get, _cegui.Window_Font_set, None,
                    "Window.Font -> CEGUI::Font *\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: CEGUI::Font * Window::getFont()\n\tset: void Window::setFont(CEGUI::Font *)")


    Text = property(_cegui.Window_Text_get, _cegui.Window_Text_set, None,
                    "Window.Text -> const CEGUI::String &\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: const CEGUI::String & Window::getText()\n\tset: void Window::setText(const CEGUI::String &)")


    DestroyedByParent = property(_cegui.Window_DestroyedByParent_get, _cegui.Window_DestroyedByParent_set, None,
                    "Window.DestroyedByParent -> bool\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: bool Window::isDestroyedByParent()\n\tset: void Window::setDestroyedByParent(bool)")


    AlwaysOnTop = property(_cegui.Window_AlwaysOnTop_get, _cegui.Window_AlwaysOnTop_set, None,
                    "Window.AlwaysOnTop -> bool\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: bool Window::isAlwaysOnTop()\n\tset: void Window::setAlwaysOnTop(bool)")


    Visible = property(_cegui.Window_Visible_get, _cegui.Window_Visible_set, None,
                    "Window.Visible -> bool\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: bool Window::isVisible()\n\tset: void Window::setVisible(bool)")


    Active = property(_cegui.Window_Active_get, None, None,
                    "Window.Active -> bool  (read-only)\n\nThis is equivalent to calling the C++ CEGUI method:\n\tget: bool Window::isActive()")

    AutoWindow = property(_cegui.Window_AutoWindow_get, None, None,
                    "Window.AutoWindow -> bool  (read-only)\n\nThis is equivalent to calling the C++ CEGUI method:\n\tget: bool Window::isAutoWindow()")

    WritingXMLAllowed = property(_cegui.Window_WritingXMLAllowed_get, _cegui.Window_WritingXMLAllowed_set, None,
                    "Window.WritingXMLAllowed -> bool\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: bool Window::isWritingXMLAllowed()\n\tset: void Window::setWritingXMLAllowed(bool)")


    ClippedByParent = property(_cegui.Window_ClippedByParent_get, _cegui.Window_ClippedByParent_set, None,
                    "Window.ClippedByParent -> bool\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: bool Window::isClippedByParent()\n\tset: void Window::setClippedByParent(bool)")


    ChildCount = property(_cegui.Window_ChildCount_get, None, None,
                    "Window.ChildCount -> CEGUI::uint  (read-only)\n\nThis is equivalent to calling the C++ CEGUI method:\n\tget: CEGUI::uint Window::getChildCount()")

    InheritsAlpha = property(_cegui.Window_InheritsAlpha_get, _cegui.Window_InheritsAlpha_set, None,
                    "Window.InheritsAlpha -> bool\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: bool Window::inheritsAlpha()\n\tset: void Window::setInheritsAlpha(bool)")


    EffectiveAlpha = property(_cegui.Window_EffectiveAlpha_get, None, None,
                    "Window.EffectiveAlpha -> float  (read-only)\n\nThis is equivalent to calling the C++ CEGUI method:\n\tget: float Window::getEffectiveAlpha()")

    PixelRect = property(_cegui.Window_PixelRect_get, None, None,
                    "Window.PixelRect -> CEGUI::Rect  (read-only)\n\nThis is equivalent to calling the C++ CEGUI method:\n\tget: CEGUI::Rect Window::getPixelRect()")

    InnerRect = property(_cegui.Window_InnerRect_get, None, None,
                    "Window.InnerRect -> CEGUI::Rect  (read-only)\n\nThis is equivalent to calling the C++ CEGUI method:\n\tget: CEGUI::Rect Window::getInnerRect()")

    UnclippedPixelRect = property(_cegui.Window_UnclippedPixelRect_get, None, None,
                    "Window.UnclippedPixelRect -> CEGUI::Rect  (read-only)\n\nThis is equivalent to calling the C++ CEGUI method:\n\tget: CEGUI::Rect Window::getUnclippedPixelRect()")

    UnclippedInnerRect = property(_cegui.Window_UnclippedInnerRect_get, None, None,
                    "Window.UnclippedInnerRect -> CEGUI::Rect  (read-only)\n\nThis is equivalent to calling the C++ CEGUI method:\n\tget: CEGUI::Rect Window::getUnclippedInnerRect()")

    CapturedByThis = property(_cegui.Window_CapturedByThis_get, None, None,
                    "Window.CapturedByThis -> bool  (read-only)\n\nThis is equivalent to calling the C++ CEGUI method:\n\tget: bool Window::isCapturedByThis()")

    CapturedByAncestor = property(_cegui.Window_CapturedByAncestor_get, None, None,
                    "Window.CapturedByAncestor -> bool  (read-only)\n\nThis is equivalent to calling the C++ CEGUI method:\n\tget: bool Window::isCapturedByAncestor()")

    CapturedByChild = property(_cegui.Window_CapturedByChild_get, None, None,
                    "Window.CapturedByChild -> bool  (read-only)\n\nThis is equivalent to calling the C++ CEGUI method:\n\tget: bool Window::isCapturedByChild()")

    Parent = property(_cegui.Window_Parent_get, None, None,
                    "Window.Parent -> CEGUI::Window *  (read-only)\n\nThis is equivalent to calling the C++ CEGUI method:\n\tget: CEGUI::Window * Window::getParent()")

    ZOrderingEnabled = property(_cegui.Window_ZOrderingEnabled_get, _cegui.Window_ZOrderingEnabled_set, None,
                    "Window.ZOrderingEnabled -> bool\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: bool Window::isZOrderingEnabled()\n\tset: void Window::setZOrderingEnabled(bool)")


    AutoRepeatDelay = property(_cegui.Window_AutoRepeatDelay_get, _cegui.Window_AutoRepeatDelay_set, None,
                    "Window.AutoRepeatDelay -> float\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: float Window::getAutoRepeatDelay()\n\tset: void Window::setAutoRepeatDelay(float)")


    AutoRepeatRate = property(_cegui.Window_AutoRepeatRate_get, _cegui.Window_AutoRepeatRate_set, None,
                    "Window.AutoRepeatRate -> float\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: float Window::getAutoRepeatRate()\n\tset: void Window::setAutoRepeatRate(float)")


    MouseAutoRepeatEnabled = property(_cegui.Window_MouseAutoRepeatEnabled_get, _cegui.Window_MouseAutoRepeatEnabled_set, None,
                    "Window.MouseAutoRepeatEnabled -> bool\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: bool Window::isMouseAutoRepeatEnabled()\n\tset: void Window::setMouseAutoRepeatEnabled(bool)")


    WantsMultiClickEvents = property(_cegui.Window_WantsMultiClickEvents_get, _cegui.Window_WantsMultiClickEvents_set, None,
                    "Window.WantsMultiClickEvents -> bool\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: bool Window::wantsMultiClickEvents()\n\tset: void Window::setWantsMultiClickEvents(bool)")


    RestoreOldCapture = property(_cegui.Window_RestoreOldCapture_get, _cegui.Window_RestoreOldCapture_set, None,
                    "Window.RestoreOldCapture -> bool\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: bool Window::restoresOldCapture()\n\tset: void Window::setRestoreCapture(bool)")


    MousePassThroughEnabled = property(_cegui.Window_MousePassThroughEnabled_get, _cegui.Window_MousePassThroughEnabled_set, None,
                    "Window.MousePassThroughEnabled -> bool\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: bool Window::isMousePassThroughEnabled()\n\tset: void Window::setMousePassThroughEnabled(bool)")


    Tooltip = property(_cegui.Window_Tooltip_get, _cegui.Window_Tooltip_set, None,
                    "Window.Tooltip -> CEGUI::Tooltip *\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: CEGUI::Tooltip * Window::getTooltip()\n\tset: void Window::setTooltip(CEGUI::Tooltip *)")


    TooltipType = property(_cegui.Window_TooltipType_get, _cegui.Window_TooltipType_set, None,
                    "Window.TooltipType -> const CEGUI::String &\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: const CEGUI::String & Window::getTooltipType()\n\tset: void Window::setTooltipType(const CEGUI::String &)")


    TooltipText = property(_cegui.Window_TooltipText_get, _cegui.Window_TooltipText_set, None,
                    "Window.TooltipText -> const CEGUI::String &\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: const CEGUI::String & Window::getTooltipText()\n\tset: void Window::setTooltipText(const CEGUI::String &)")


    InheritsTooltipText = property(_cegui.Window_InheritsTooltipText_get, _cegui.Window_InheritsTooltipText_set, None,
                    "Window.InheritsTooltipText -> bool\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: bool Window::inheritsTooltipText()\n\tset: void Window::setInheritsTooltipText(bool)")


    HorizontalAlignment = property(_cegui.Window_HorizontalAlignment_get, _cegui.Window_HorizontalAlignment_set, None,
                    "Window.HorizontalAlignment -> CEGUI::HorizontalAlignment\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: CEGUI::HorizontalAlignment Window::getHorizontalAlignment()\n\tset: void Window::setHorizontalAlignment(CEGUI::HorizontalAlignment)")


    VerticalAlignment = property(_cegui.Window_VerticalAlignment_get, _cegui.Window_VerticalAlignment_set, None,
                    "Window.VerticalAlignment -> CEGUI::VerticalAlignment\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: CEGUI::VerticalAlignment Window::getVerticalAlignment()\n\tset: void Window::setVerticalAlignment(CEGUI::VerticalAlignment)")


    Position = property(_cegui.Window_Position_get, _cegui.Window_Position_set, None,
                    "Window.Position -> const CEGUI::UVector2 &\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: const CEGUI::UVector2 & Window::getPosition()\n\tset: void Window::setPosition(const CEGUI::UVector2 &)")


    Left = property(_cegui.Window_Left_get, _cegui.Window_Left_set, None,
                    "Window.Left -> const CEGUI::UDim &\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: const CEGUI::UDim & Window::getXPosition()\n\tset: void Window::setXPosition(const CEGUI::UDim &)")


    Top = property(_cegui.Window_Top_get, _cegui.Window_Top_set, None,
                    "Window.Top -> const CEGUI::UDim &\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: const CEGUI::UDim & Window::getYPosition()\n\tset: void Window::setYPosition(const CEGUI::UDim &)")


    Size = property(_cegui.Window_Size_get, _cegui.Window_Size_set, None,
                    "Window.Size -> CEGUI::UVector2\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: CEGUI::UVector2 Window::getSize()\n\tset: void Window::setSize(CEGUI::UVector2)")


    Height = property(_cegui.Window_Height_get, _cegui.Window_Height_set, None,
                    "Window.Height -> CEGUI::UDim\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: CEGUI::UDim Window::getHeight()\n\tset: void Window::setHeight(CEGUI::UDim)")


    Width = property(_cegui.Window_Width_get, _cegui.Window_Width_set, None,
                    "Window.Width -> CEGUI::UDim\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: CEGUI::UDim Window::getWidth()\n\tset: void Window::setWidth(CEGUI::UDim)")


    MaxSize = property(_cegui.Window_MaxSize_get, _cegui.Window_MaxSize_set, None,
                    "Window.MaxSize -> const CEGUI::UVector2 &\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: const CEGUI::UVector2 & Window::getMaxSize()\n\tset: void Window::setMaxSize(const CEGUI::UVector2 &)")


    MinSize = property(_cegui.Window_MinSize_get, _cegui.Window_MinSize_set, None,
                    "Window.MinSize -> const CEGUI::UVector2 &\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: const CEGUI::UVector2 & Window::getMinSize()\n\tset: void Window::setMinSize(const CEGUI::UVector2 &)")


    ParentPixelSize = property(_cegui.Window_ParentPixelSize_get, None, None,
                    "Window.ParentPixelSize -> CEGUI::Size  (read-only)\n\nThis is equivalent to calling the C++ CEGUI method:\n\tget: CEGUI::Size Window::getParentPixelSize()")

    ParentPixelWidth = property(_cegui.Window_ParentPixelWidth_get, None, None,
                    "Window.ParentPixelWidth -> float  (read-only)\n\nThis is equivalent to calling the C++ CEGUI method:\n\tget: float Window::getParentPixelWidth()")

    ParentPixelHeight = property(_cegui.Window_ParentPixelHeight_get, None, None,
                    "Window.ParentPixelHeight -> float  (read-only)\n\nThis is equivalent to calling the C++ CEGUI method:\n\tget: float Window::getParentPixelHeight()")

    ActiveChild = property(_cegui.Window_ActiveChild_get, None, None,
                    "Window.ActiveChild -> Window *  (read-only)\n\nThis is equivalent to calling the C++ CEGUI method:\n\tget: Window * Window::getActiveChild()")

    ActiveSibling = property(_cegui.Window_ActiveSibling_get, None, None,
                    "Window.ActiveSibling -> Window *  (read-only)\n\nThis is equivalent to calling the C++ CEGUI method:\n\tget: Window * Window::getActiveSibling()")

    ModalState = property(_cegui.Window_ModalState_get, _cegui.Window_ModalState_set, None,
                    "Window.ModalState -> bool\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: bool Window::getModalState()\n\tset: void Window::setModalState(bool)")


    RenderCache = property(_cegui.Window_RenderCache_get, None, None,
                    "Window.RenderCache -> CEGUI::RenderCache  (read-only)\n\nThis is equivalent to calling the C++ CEGUI method:\n\tget: CEGUI::RenderCache Window::getRenderCache()")

    RiseOnClickEnabled = property(_cegui.Window_RiseOnClickEnabled_get, _cegui.Window_RiseOnClickEnabled_set, None,
                    "Window.RiseOnClickEnabled -> bool\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: bool Window::isRiseOnClickEnabled()\n\tset: void Window::setRiseOnClickEnabled(bool)")


    DistributesCapturedInputs = property(_cegui.Window_DistributesCapturedInputs_get, _cegui.Window_DistributesCapturedInputs_set, None,
                    "Window.DistributesCapturedInputs -> bool\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: bool Window::distributesCapturedInputs()\n\tset: void Window::setDistributesCapturedInputs(bool)")


    UsingDefaultTooltip = property(_cegui.Window_UsingDefaultTooltip_get, None, None,
                    "Window.UsingDefaultTooltip -> bool  (read-only)\n\nThis is equivalent to calling the C++ CEGUI method:\n\tget: bool Window::isUsingDefaultTooltip()")

Window.isChildRecursive = new_instancemethod(_cegui.Window_isChildRecursive,None,Window)
Window.isChild = new_instancemethod(_cegui.Window_isChild,None,Window)
Window.getChild = new_instancemethod(_cegui.Window_getChild,None,Window)
Window.getChildRecursive = new_instancemethod(_cegui.Window_getChildRecursive,None,Window)
Window.getChildAtIdx = new_instancemethod(_cegui.Window_getChildAtIdx,None,Window)
Window.isAncestor = new_instancemethod(_cegui.Window_isAncestor,None,Window)
Window.getPixelRect_impl = new_instancemethod(_cegui.Window_getPixelRect_impl,None,Window)
Window.getUnclippedInnerRect_impl = new_instancemethod(_cegui.Window_getUnclippedInnerRect_impl,None,Window)
Window.isHit = new_instancemethod(_cegui.Window_isHit,None,Window)
Window.getChildAtPosition = new_instancemethod(_cegui.Window_getChildAtPosition,None,Window)
Window.getTargetChildAtPosition = new_instancemethod(_cegui.Window_getTargetChildAtPosition,None,Window)
Window.getPixelSize = new_instancemethod(_cegui.Window_getPixelSize,None,Window)
Window.testClassName = new_instancemethod(_cegui.Window_testClassName,None,Window)
Window.getUserString = new_instancemethod(_cegui.Window_getUserString,None,Window)
Window.isUserStringDefined = new_instancemethod(_cegui.Window_isUserStringDefined,None,Window)
Window.rename = new_instancemethod(_cegui.Window_rename,None,Window)
Window.initialiseComponents = new_instancemethod(_cegui.Window_initialiseComponents,None,Window)
Window.enable = new_instancemethod(_cegui.Window_enable,None,Window)
Window.disable = new_instancemethod(_cegui.Window_disable,None,Window)
Window.show = new_instancemethod(_cegui.Window_show,None,Window)
Window.hide = new_instancemethod(_cegui.Window_hide,None,Window)
Window.activate = new_instancemethod(_cegui.Window_activate,None,Window)
Window.deactivate = new_instancemethod(_cegui.Window_deactivate,None,Window)
Window.addChildWindow = new_instancemethod(_cegui.Window_addChildWindow,None,Window)
Window.removeChildWindow = new_instancemethod(_cegui.Window_removeChildWindow,None,Window)
Window.moveToFront = new_instancemethod(_cegui.Window_moveToFront,None,Window)
Window.moveToBack = new_instancemethod(_cegui.Window_moveToBack,None,Window)
Window.captureInput = new_instancemethod(_cegui.Window_captureInput,None,Window)
Window.releaseInput = new_instancemethod(_cegui.Window_releaseInput,None,Window)
Window.requestRedraw = new_instancemethod(_cegui.Window_requestRedraw,None,Window)
Window.notifyDragDropItemEnters = new_instancemethod(_cegui.Window_notifyDragDropItemEnters,None,Window)
Window.notifyDragDropItemLeaves = new_instancemethod(_cegui.Window_notifyDragDropItemLeaves,None,Window)
Window.notifyDragDropItemDropped = new_instancemethod(_cegui.Window_notifyDragDropItemDropped,None,Window)
Window.destroy = new_instancemethod(_cegui.Window_destroy,None,Window)
Window.performChildWindowLayout = new_instancemethod(_cegui.Window_performChildWindowLayout,None,Window)
Window.setUserString = new_instancemethod(_cegui.Window_setUserString,None,Window)
Window.render = new_instancemethod(_cegui.Window_render,None,Window)
Window.update = new_instancemethod(_cegui.Window_update,None,Window)
Window.writeXMLToStream = new_instancemethod(_cegui.Window_writeXMLToStream,None,Window)
Window.beginInitialisation = new_instancemethod(_cegui.Window_beginInitialisation,None,Window)
Window.endInitialisation = new_instancemethod(_cegui.Window_endInitialisation,None,Window)
Window.setWindowRenderer = new_instancemethod(_cegui.Window_setWindowRenderer,None,Window)
Window.getWindowRenderer = new_instancemethod(_cegui.Window_getWindowRenderer,None,Window)
Window.getWindowRendererName = new_instancemethod(_cegui.Window_getWindowRendererName,None,Window)
Window.notifyScreenAreaChanged = new_instancemethod(_cegui.Window_notifyScreenAreaChanged,None,Window)
Window_swigregister = _cegui.Window_swigregister
Window_swigregister(Window)
Window.TooltipNameSuffix = _cegui.cvar.Window_TooltipNameSuffix
Window.AutoWidgetNameSuffix = _cegui.cvar.Window_AutoWidgetNameSuffix
Window_getCaptureWindow = _cegui.Window_getCaptureWindow

class WindowRenderer(object):
    thisown = property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    def __init__(self): raise AttributeError, "No constructor defined"
    __repr__ = _swig_repr
    __swig_destroy__ = _cegui.delete_WindowRenderer
    name = property(_cegui.WindowRenderer_name_get, None, None,
                    "WindowRenderer.name -> const CEGUI::String &  (read-only)\n\nThis is equivalent to calling the C++ CEGUI method:\n\tget: const CEGUI::String & WindowRenderer::getName()")

    className = property(_cegui.WindowRenderer_className_get, None, None,
                    "WindowRenderer.className -> const CEGUI::String &  (read-only)\n\nThis is equivalent to calling the C++ CEGUI method:\n\tget: const CEGUI::String & WindowRenderer::getClass()")

    window = property(_cegui.WindowRenderer_window_get, None, None,
                    "WindowRenderer.window -> CEGUI::Window *  (read-only)\n\nThis is equivalent to calling the C++ CEGUI method:\n\tget: CEGUI::Window * WindowRenderer::getWindow()")

    lookNFeel = property(_cegui.WindowRenderer_lookNFeel_get, None, None,
                    "WindowRenderer.lookNFeel -> const CEGUI::WidgetLookFeel &  (read-only)\n\nThis is equivalent to calling the C++ CEGUI method:\n\tget: const CEGUI::WidgetLookFeel & WindowRenderer::getLookNFeel()")

    unclippedInnerRect = property(_cegui.WindowRenderer_unclippedInnerRect_get, None, None,
                    "WindowRenderer.unclippedInnerRect -> CEGUI::Rect  (read-only)\n\nThis is equivalent to calling the C++ CEGUI method:\n\tget: CEGUI::Rect WindowRenderer::getUnclippedInnerRect()")

    pixelRect = property(_cegui.WindowRenderer_pixelRect_get, None, None,
                    "WindowRenderer.pixelRect -> CEGUI::Rect  (read-only)\n\nThis is equivalent to calling the C++ CEGUI method:\n\tget: CEGUI::Rect WindowRenderer::getPixelRect()")

WindowRenderer.render = new_instancemethod(_cegui.WindowRenderer_render,None,WindowRenderer)
WindowRenderer.performChildWindowLayout = new_instancemethod(_cegui.WindowRenderer_performChildWindowLayout,None,WindowRenderer)
WindowRenderer_swigregister = _cegui.WindowRenderer_swigregister
WindowRenderer_swigregister(WindowRenderer)

class WindowRendererFactory(object):
    thisown = property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    def __init__(self): raise AttributeError, "No constructor defined"
    __repr__ = _swig_repr
    __swig_destroy__ = _cegui.delete_WindowRendererFactory
WindowRendererFactory.getName = new_instancemethod(_cegui.WindowRendererFactory_getName,None,WindowRendererFactory)
WindowRendererFactory.create = new_instancemethod(_cegui.WindowRendererFactory_create,None,WindowRendererFactory)
WindowRendererFactory.destroy = new_instancemethod(_cegui.WindowRendererFactory_destroy,None,WindowRendererFactory)
WindowRendererFactory_swigregister = _cegui.WindowRendererFactory_swigregister
WindowRendererFactory_swigregister(WindowRendererFactory)

class TooltipWindowRenderer(WindowRenderer):
    thisown = property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    def __init__(self): raise AttributeError, "No constructor defined"
    __repr__ = _swig_repr
    __swig_destroy__ = _cegui.delete_TooltipWindowRenderer
TooltipWindowRenderer.getTextSize = new_instancemethod(_cegui.TooltipWindowRenderer_getTextSize,None,TooltipWindowRenderer)
TooltipWindowRenderer_swigregister = _cegui.TooltipWindowRenderer_swigregister
TooltipWindowRenderer_swigregister(TooltipWindowRenderer)

class Tooltip(Window):
    thisown = property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, *args): 
        _cegui.Tooltip_swiginit(self,_cegui.new_Tooltip(*args))
    __swig_destroy__ = _cegui.delete_Tooltip
    EventNamespace = "Tooltip" 
    EventDisplayTimeChanged = "DisplayTimeChanged" 
    EventFadeTimeChanged = "FadeTimeChanged" 
    EventHoverTimeChanged = "HoverTimeChanged" 
    EventTooltipActive = "TooltipActive" 
    EventTooltipInactive = "TooltipInactive" 
    TargetWindow = property(_cegui.Tooltip_TargetWindow_get, _cegui.Tooltip_TargetWindow_set, None,
                    "Tooltip.TargetWindow -> CEGUI::Window *\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: CEGUI::Window * Tooltip::getTargetWindow()\n\tset: void Tooltip::setTargetWindow(CEGUI::Window *)")


    HoverTime = property(_cegui.Tooltip_HoverTime_get, _cegui.Tooltip_HoverTime_set, None,
                    "Tooltip.HoverTime -> float\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: float Tooltip::getHoverTime()\n\tset: void Tooltip::setHoverTime(float)")


    DisplayTime = property(_cegui.Tooltip_DisplayTime_get, _cegui.Tooltip_DisplayTime_set, None,
                    "Tooltip.DisplayTime -> float\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: float Tooltip::getDisplayTime()\n\tset: void Tooltip::setDisplayTime(float)")


    FadeTime = property(_cegui.Tooltip_FadeTime_get, _cegui.Tooltip_FadeTime_set, None,
                    "Tooltip.FadeTime -> float\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: float Tooltip::getFadeTime()\n\tset: void Tooltip::setFadeTime(float)")


    TextSize = property(_cegui.Tooltip_TextSize_get, None, None,
                    "Tooltip.TextSize -> CEGUI::Size  (read-only)\n\nThis is equivalent to calling the C++ CEGUI method:\n\tget: CEGUI::Size Tooltip::getTextSize()")

Tooltip.resetTimer = new_instancemethod(_cegui.Tooltip_resetTimer,None,Tooltip)
Tooltip.positionSelf = new_instancemethod(_cegui.Tooltip_positionSelf,None,Tooltip)
Tooltip.sizeSelf = new_instancemethod(_cegui.Tooltip_sizeSelf,None,Tooltip)
Tooltip.getTextSize_impl = new_instancemethod(_cegui.Tooltip_getTextSize_impl,None,Tooltip)
Tooltip_swigregister = _cegui.Tooltip_swigregister
Tooltip_swigregister(Tooltip)
Tooltip.WidgetTypeName = _cegui.cvar.Tooltip_WidgetTypeName

class DragContainer(Window):
    thisown = property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, *args): 
        _cegui.DragContainer_swiginit(self,_cegui.new_DragContainer(*args))
    __swig_destroy__ = _cegui.delete_DragContainer

    DragCursorImage = property(_cegui.DragContainer___getDragCursorImage, fix_args(_cegui.DragContainer___setDragCursorImage), None,
                    "DragContainer.DragCursorImage -> type\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: DragContainer::getDragCursorImage()\n\tset: void DragContainer::setDragCursorImage(...)")

    EventNamespace = "DragContainer" 
    EventDragAlphaChanged = "DragAlphaChanged" 
    EventDragDropTargetChanged = "DragDropTargetChanged" 
    EventDragEnabledChanged = "DragEnabledChanged" 
    EventDragEnded = "DragEnded" 
    EventDragMouseCursorChanged = "DragMouseCursorChanged" 
    EventDragPositionChanged = "DragPositionChanged" 
    EventDragStarted = "DragStarted" 
    EventDragThresholdChanged = "DragThresholdChanged" 
    DraggingEnabled = property(_cegui.DragContainer_DraggingEnabled_get, _cegui.DragContainer_DraggingEnabled_set, None,
                    "DragContainer.DraggingEnabled -> bool\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: bool DragContainer::isDraggingEnabled()\n\tset: void DragContainer::setDraggingEnabled(bool)")


    IsDragged = property(_cegui.DragContainer_IsDragged_get, None, None,
                    "DragContainer.IsDragged -> bool  (read-only)\n\nThis is equivalent to calling the C++ CEGUI method:\n\tget: bool DragContainer::isBeingDragged()")

    PixelDragThreshold = property(_cegui.DragContainer_PixelDragThreshold_get, _cegui.DragContainer_PixelDragThreshold_set, None,
                    "DragContainer.PixelDragThreshold -> float\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: float DragContainer::getPixelDragThreshold()\n\tset: void DragContainer::setPixelDragThreshold(float)")


    DragAlpha = property(_cegui.DragContainer_DragAlpha_get, _cegui.DragContainer_DragAlpha_set, None,
                    "DragContainer.DragAlpha -> float\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: float DragContainer::getDragAlpha()\n\tset: void DragContainer::setDragAlpha(float)")


    CurrentDropTarget = property(_cegui.DragContainer_CurrentDropTarget_get, None, None,
                    "DragContainer.CurrentDropTarget -> CEGUI::Window *  (read-only)\n\nThis is equivalent to calling the C++ CEGUI method:\n\tget: CEGUI::Window * DragContainer::getCurrentDropTarget()")

DragContainer_swigregister = _cegui.DragContainer_swigregister
DragContainer_swigregister(DragContainer)
DragContainer.WidgetTypeName = _cegui.cvar.DragContainer_WidgetTypeName

class ItemEntryWindowRenderer(WindowRenderer):
    thisown = property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    def __init__(self): raise AttributeError, "No constructor defined"
    __repr__ = _swig_repr
    __swig_destroy__ = _cegui.delete_ItemEntryWindowRenderer
ItemEntryWindowRenderer.getItemPixelSize = new_instancemethod(_cegui.ItemEntryWindowRenderer_getItemPixelSize,None,ItemEntryWindowRenderer)
ItemEntryWindowRenderer_swigregister = _cegui.ItemEntryWindowRenderer_swigregister
ItemEntryWindowRenderer_swigregister(ItemEntryWindowRenderer)

class ItemEntry(Window):
    thisown = property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, *args): 
        _cegui.ItemEntry_swiginit(self,_cegui.new_ItemEntry(*args))
    __swig_destroy__ = _cegui.delete_ItemEntry
    EventNamespace = "Window" 
    EventSelectionChanged = "SelectionChanged" 
    ItemPixelSize = property(_cegui.ItemEntry_ItemPixelSize_get, None, None,
                    "ItemEntry.ItemPixelSize -> CEGUI::Size  (read-only)\n\nThis is equivalent to calling the C++ CEGUI method:\n\tget: CEGUI::Size ItemEntry::getItemPixelSize()")

    OwnerList = property(_cegui.ItemEntry_OwnerList_get, None, None,
                    "ItemEntry.OwnerList -> CEGUI::ItemListBase *  (read-only)\n\nThis is equivalent to calling the C++ CEGUI method:\n\tget: CEGUI::ItemListBase * ItemEntry::getOwnerList()")

    Selected = property(_cegui.ItemEntry_Selected_get, _cegui.ItemEntry_Selected_set, None,
                    "ItemEntry.Selected -> bool\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: bool ItemEntry::isSelected()\n\tset: void ItemEntry::setSelected(bool)")


    Selectable = property(_cegui.ItemEntry_Selectable_get, _cegui.ItemEntry_Selectable_set, None,
                    "ItemEntry.Selectable -> bool\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: bool ItemEntry::isSelectable()\n\tset: void ItemEntry::setSelectable(bool)")


ItemEntry.select = new_instancemethod(_cegui.ItemEntry_select,None,ItemEntry)
ItemEntry.deselect = new_instancemethod(_cegui.ItemEntry_deselect,None,ItemEntry)
ItemEntry.setSelected_impl = new_instancemethod(_cegui.ItemEntry_setSelected_impl,None,ItemEntry)
ItemEntry_swigregister = _cegui.ItemEntry_swigregister
ItemEntry_swigregister(ItemEntry)
ItemEntry.WidgetTypeName = _cegui.cvar.ItemEntry_WidgetTypeName

class ItemListBaseWindowRenderer(WindowRenderer):
    thisown = property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    def __init__(self): raise AttributeError, "No constructor defined"
    __repr__ = _swig_repr
    __swig_destroy__ = _cegui.delete_ItemListBaseWindowRenderer
ItemListBaseWindowRenderer.getItemRenderArea = new_instancemethod(_cegui.ItemListBaseWindowRenderer_getItemRenderArea,None,ItemListBaseWindowRenderer)
ItemListBaseWindowRenderer_swigregister = _cegui.ItemListBaseWindowRenderer_swigregister
ItemListBaseWindowRenderer_swigregister(ItemListBaseWindowRenderer)

class ItemListBase(Window):
    thisown = property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    def __init__(self): raise AttributeError, "No constructor defined"
    __repr__ = _swig_repr
    Ascending = _cegui.ItemListBase_Ascending
    Descending = _cegui.ItemListBase_Descending
    UserSort = _cegui.ItemListBase_UserSort
    __swig_destroy__ = _cegui.delete_ItemListBase
    EventNamespace = "ItemListBase" 
    EventListContentsChanged = "ListItemsChanged" 
    EventSortEnabledChanged = "SortEnabledChanged" 
    EventSortModeChanged = "SortModeChanged" 
    ItemCount = property(_cegui.ItemListBase_ItemCount_get, None, None,
                    "ItemListBase.ItemCount -> size_t  (read-only)\n\nThis is equivalent to calling the C++ CEGUI method:\n\tget: size_t ItemListBase::getItemCount()")

    AutoResizeEnabled = property(_cegui.ItemListBase_AutoResizeEnabled_get, _cegui.ItemListBase_AutoResizeEnabled_set, None,
                    "ItemListBase.AutoResizeEnabled -> bool\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: bool ItemListBase::isAutoResizeEnabled()\n\tset: void ItemListBase::setAutoResizeEnabled(bool)")


    SortEnabled = property(_cegui.ItemListBase_SortEnabled_get, _cegui.ItemListBase_SortEnabled_set, None,
                    "ItemListBase.SortEnabled -> bool\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: bool ItemListBase::isSortEnabled()\n\tset: void ItemListBase::setSortEnabled(bool)")


    SortMode = property(_cegui.ItemListBase_SortMode_get, _cegui.ItemListBase_SortMode_set, None,
                    "ItemListBase.SortMode -> CEGUI::ItemListBase::SortMode\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: CEGUI::ItemListBase::SortMode ItemListBase::getSortMode()\n\tset: void ItemListBase::setSortMode(CEGUI::ItemListBase::SortMode)")


ItemListBase.getItemFromIndex = new_instancemethod(_cegui.ItemListBase_getItemFromIndex,None,ItemListBase)
ItemListBase.getItemIndex = new_instancemethod(_cegui.ItemListBase_getItemIndex,None,ItemListBase)
ItemListBase.findItemWithText = new_instancemethod(_cegui.ItemListBase_findItemWithText,None,ItemListBase)
ItemListBase.isItemInList = new_instancemethod(_cegui.ItemListBase_isItemInList,None,ItemListBase)
ItemListBase.getSortCallback = new_instancemethod(_cegui.ItemListBase_getSortCallback,None,ItemListBase)
ItemListBase.resetList = new_instancemethod(_cegui.ItemListBase_resetList,None,ItemListBase)
ItemListBase.addItem = new_instancemethod(_cegui.ItemListBase_addItem,None,ItemListBase)
ItemListBase.insertItem = new_instancemethod(_cegui.ItemListBase_insertItem,None,ItemListBase)
ItemListBase.removeItem = new_instancemethod(_cegui.ItemListBase_removeItem,None,ItemListBase)
ItemListBase.handleUpdatedItemData = new_instancemethod(_cegui.ItemListBase_handleUpdatedItemData,None,ItemListBase)
ItemListBase.sizeToContent = new_instancemethod(_cegui.ItemListBase_sizeToContent,None,ItemListBase)
ItemListBase.getItemRenderArea = new_instancemethod(_cegui.ItemListBase_getItemRenderArea,None,ItemListBase)
ItemListBase.getContentPane = new_instancemethod(_cegui.ItemListBase_getContentPane,None,ItemListBase)
ItemListBase.notifyItemClicked = new_instancemethod(_cegui.ItemListBase_notifyItemClicked,None,ItemListBase)
ItemListBase.notifyItemSelectState = new_instancemethod(_cegui.ItemListBase_notifyItemSelectState,None,ItemListBase)
ItemListBase.setSortCallback = new_instancemethod(_cegui.ItemListBase_setSortCallback,None,ItemListBase)
ItemListBase.sortList = new_instancemethod(_cegui.ItemListBase_sortList,None,ItemListBase)
ItemListBase_swigregister = _cegui.ItemListBase_swigregister
ItemListBase_swigregister(ItemListBase)

class MenuBase(ItemListBase):
    thisown = property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    def __init__(self): raise AttributeError, "No constructor defined"
    __repr__ = _swig_repr
    __swig_destroy__ = _cegui.delete_MenuBase
    EventNamespace = "MenuBase" 
    EventListContentsChanged = "ListItemsChanged" 
    EventPopupClosed = "PopupClosed" 
    EventPopupOpened = "PopupOpened" 
    EventSortEnabledChanged = "SortEnabledChanged" 
    EventSortModeChanged = "SortModeChanged" 
    MultiplePopupsAllowed = property(_cegui.MenuBase_MultiplePopupsAllowed_get, _cegui.MenuBase_MultiplePopupsAllowed_set, None,
                    "MenuBase.MultiplePopupsAllowed -> bool\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: bool MenuBase::isMultiplePopupsAllowed()\n\tset: void MenuBase::setAllowMultiplePopups(bool)")


    ItemSpacing = property(_cegui.MenuBase_ItemSpacing_get, _cegui.MenuBase_ItemSpacing_set, None,
                    "MenuBase.ItemSpacing -> float\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: float MenuBase::getItemSpacing()\n\tset: void MenuBase::setItemSpacing(float)")


    PopupMenuItem = property(_cegui.MenuBase_PopupMenuItem_get, _cegui.MenuBase_PopupMenuItem_set, None,
                    "MenuBase.PopupMenuItem -> MenuItem *\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: MenuItem * MenuBase::getPopupMenuItem()\n\tset: void MenuBase::changePopupMenuItem(MenuItem *)")


MenuBase_swigregister = _cegui.MenuBase_swigregister
MenuBase_swigregister(MenuBase)

class Menubar(MenuBase):
    thisown = property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, *args): 
        _cegui.Menubar_swiginit(self,_cegui.new_Menubar(*args))
    __swig_destroy__ = _cegui.delete_Menubar
    EventNamespace = "Menubar" 
    EventListContentsChanged = "ListItemsChanged" 
    EventPopupClosed = "PopupClosed" 
    EventPopupOpened = "PopupOpened" 
    EventSortEnabledChanged = "SortEnabledChanged" 
    EventSortModeChanged = "SortModeChanged" 
Menubar_swigregister = _cegui.Menubar_swigregister
Menubar_swigregister(Menubar)
Menubar.WidgetTypeName = _cegui.cvar.Menubar_WidgetTypeName

class PopupMenu(MenuBase):
    thisown = property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, *args): 
        _cegui.PopupMenu_swiginit(self,_cegui.new_PopupMenu(*args))
    __swig_destroy__ = _cegui.delete_PopupMenu
    EventNamespace = "PopupMenu" 
    EventListContentsChanged = "ListItemsChanged" 
    EventPopupClosed = "PopupClosed" 
    EventPopupOpened = "PopupOpened" 
    EventSortEnabledChanged = "SortEnabledChanged" 
    EventSortModeChanged = "SortModeChanged" 
    FadeInTime = property(_cegui.PopupMenu_FadeInTime_get, _cegui.PopupMenu_FadeInTime_set, None,
                    "PopupMenu.FadeInTime -> float\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: float PopupMenu::getFadeInTime()\n\tset: void PopupMenu::setFadeInTime(float)")


    FadeOutTime = property(_cegui.PopupMenu_FadeOutTime_get, _cegui.PopupMenu_FadeOutTime_set, None,
                    "PopupMenu.FadeOutTime -> float\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: float PopupMenu::getFadeOutTime()\n\tset: void PopupMenu::setFadeOutTime(float)")


PopupMenu.isPopupMenuOpen = new_instancemethod(_cegui.PopupMenu_isPopupMenuOpen,None,PopupMenu)
PopupMenu.openPopupMenu = new_instancemethod(_cegui.PopupMenu_openPopupMenu,None,PopupMenu)
PopupMenu.closePopupMenu = new_instancemethod(_cegui.PopupMenu_closePopupMenu,None,PopupMenu)
PopupMenu_swigregister = _cegui.PopupMenu_swigregister
PopupMenu_swigregister(PopupMenu)
PopupMenu.WidgetTypeName = _cegui.cvar.PopupMenu_WidgetTypeName

class MenuItem(ItemEntry):
    thisown = property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, *args): 
        _cegui.MenuItem_swiginit(self,_cegui.new_MenuItem(*args))
    __swig_destroy__ = _cegui.delete_MenuItem
    EventNamespace = "MenuItem" 
    EventClicked = "Clicked" 
    EventSelectionChanged = "SelectionChanged" 
    Hovering = property(_cegui.MenuItem_Hovering_get, None, None,
                    "MenuItem.Hovering -> bool  (read-only)\n\nThis is equivalent to calling the C++ CEGUI method:\n\tget: bool MenuItem::isHovering()")

    Pushed = property(_cegui.MenuItem_Pushed_get, None, None,
                    "MenuItem.Pushed -> bool  (read-only)\n\nThis is equivalent to calling the C++ CEGUI method:\n\tget: bool MenuItem::isPushed()")

    Opened = property(_cegui.MenuItem_Opened_get, None, None,
                    "MenuItem.Opened -> bool  (read-only)\n\nThis is equivalent to calling the C++ CEGUI method:\n\tget: bool MenuItem::isOpened()")

    PopupMenu = property(_cegui.MenuItem_PopupMenu_get, _cegui.MenuItem_PopupMenu_set, None,
                    "MenuItem.PopupMenu -> CEGUI::PopupMenu *\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: CEGUI::PopupMenu * MenuItem::getPopupMenu()\n\tset: void MenuItem::setPopupMenu(CEGUI::PopupMenu *)")


MenuItem.openPopupMenu = new_instancemethod(_cegui.MenuItem_openPopupMenu,None,MenuItem)
MenuItem.closePopupMenu = new_instancemethod(_cegui.MenuItem_closePopupMenu,None,MenuItem)
MenuItem.togglePopupMenu = new_instancemethod(_cegui.MenuItem_togglePopupMenu,None,MenuItem)
MenuItem_swigregister = _cegui.MenuItem_swigregister
MenuItem_swigregister(MenuItem)
MenuItem.WidgetTypeName = _cegui.cvar.MenuItem_WidgetTypeName

class Combobox(Window):
    thisown = property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, *args): 
        _cegui.Combobox_swiginit(self,_cegui.new_Combobox(*args))
    __swig_destroy__ = _cegui.delete_Combobox
    EventNamespace = "Combobox" 
    EventCaratMoved = "CaratMoved" 
    EventDropListDisplayed = "DropListDisplayed" 
    EventDropListRemoved = "DropListRemoved" 
    EventEditboxFull = "EditboxFullEvent" 
    EventHorzScrollbarModeChanged = "HorzScrollbarModeChanged" 
    EventInvalidEntryAttempted = "InvalidEntryAttempted" 
    EventListContentsChanged = "ListContentsChanged" 
    EventListSelectionAccepted = "ListSelectionAccepted" 
    EventListSelectionChanged = "ListSelectionChanged" 
    EventMaximumTextLengthChanged = "MaximumTextLengthChanged" 
    EventReadOnlyModeChanged = "ReadOnlyChanged" 
    EventSortModeChanged = "SortModeChanged" 
    EventTextAccepted = "TextAcceptedEvent" 
    EventTextInvalidated = "TextInvalidatedEvent" 
    EventTextSelectionChanged = "TextSelectionChanged" 
    EventValidationStringChanged = "ValidationStringChanged" 
    EventVertScrollbarModeChanged = "VertScrollbarModeChanged" 
    SelectionStartIndex = property(_cegui.Combobox_SelectionStartIndex_get, _cegui.Combobox_SelectionStartIndex_set)
    SelectionEndIndex = property(_cegui.Combobox_SelectionEndIndex_get, _cegui.Combobox_SelectionEndIndex_set)
    SelectedIndex = property(_cegui.Combobox_SelectedIndex_get)
    SingleClickEnabled = property(_cegui.Combobox_SingleClickEnabled_get, _cegui.Combobox_SingleClickEnabled_set, None,
                    "Combobox.SingleClickEnabled -> bool\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: bool Combobox::getSingleClickEnabled()\n\tset: void Combobox::setSingleClickEnabled(bool)")


    DropDownListVisible = property(_cegui.Combobox_DropDownListVisible_get, None, None,
                    "Combobox.DropDownListVisible -> bool  (read-only)\n\nThis is equivalent to calling the C++ CEGUI method:\n\tget: bool Combobox::isDropDownListVisible()")

    ReadOnly = property(_cegui.Combobox_ReadOnly_get, _cegui.Combobox_ReadOnly_set, None,
                    "Combobox.ReadOnly -> bool\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: bool Combobox::isReadOnly()\n\tset: void Combobox::setReadOnly(bool)")


    HasInputFocus = property(_cegui.Combobox_HasInputFocus_get, None, None,
                    "Combobox.HasInputFocus -> bool  (read-only)\n\nThis is equivalent to calling the C++ CEGUI method:\n\tget: bool Combobox::hasInputFocus()")

    TextValid = property(_cegui.Combobox_TextValid_get, None, None,
                    "Combobox.TextValid -> bool  (read-only)\n\nThis is equivalent to calling the C++ CEGUI method:\n\tget: bool Combobox::isTextValid()")

    SortEnabled = property(_cegui.Combobox_SortEnabled_get, _cegui.Combobox_SortEnabled_set, None,
                    "Combobox.SortEnabled -> bool\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: bool Combobox::isSortEnabled()\n\tset: void Combobox::setSortingEnabled(bool)")


    ShowVertScrollbar = property(_cegui.Combobox_ShowVertScrollbar_get, _cegui.Combobox_ShowVertScrollbar_set, None,
                    "Combobox.ShowVertScrollbar -> bool\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: bool Combobox::isVertScrollbarAlwaysShown()\n\tset: void Combobox::setShowVertScrollbar(bool)")


    ShowHorzScrollbar = property(_cegui.Combobox_ShowHorzScrollbar_get, _cegui.Combobox_ShowHorzScrollbar_set, None,
                    "Combobox.ShowHorzScrollbar -> bool\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: bool Combobox::isHorzScrollbarAlwaysShown()\n\tset: void Combobox::setShowHorzScrollbar(bool)")


    ValidationString = property(_cegui.Combobox_ValidationString_get, _cegui.Combobox_ValidationString_set, None,
                    "Combobox.ValidationString -> const String &\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: const String & Combobox::getValidationString()\n\tset: void Combobox::setValidationString(const String &)")


    CaratIndex = property(_cegui.Combobox_CaratIndex_get, _cegui.Combobox_CaratIndex_set, None,
                    "Combobox.CaratIndex -> size_t\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: size_t Combobox::getCaratIndex()\n\tset: void Combobox::setCaratIndex(size_t)")


    SelectionLength = property(_cegui.Combobox_SelectionLength_get, None, None,
                    "Combobox.SelectionLength -> size_t  (read-only)\n\nThis is equivalent to calling the C++ CEGUI method:\n\tget: size_t Combobox::getSelectionLength()")

    MaxTextLength = property(_cegui.Combobox_MaxTextLength_get, _cegui.Combobox_MaxTextLength_set, None,
                    "Combobox.MaxTextLength -> size_t\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: size_t Combobox::getMaxTextLength()\n\tset: void Combobox::setMaxTextLength(size_t)")


    ItemCount = property(_cegui.Combobox_ItemCount_get, None, None,
                    "Combobox.ItemCount -> size_t  (read-only)\n\nThis is equivalent to calling the C++ CEGUI method:\n\tget: size_t Combobox::getItemCount()")

    Editbox = property(_cegui.Combobox_Editbox_get, None, None,
                    "Combobox.Editbox -> CEGUI::Editbox *  (read-only)\n\nThis is equivalent to calling the C++ CEGUI method:\n\tget: CEGUI::Editbox * Combobox::getEditbox()")

    PushButton = property(_cegui.Combobox_PushButton_get, None, None,
                    "Combobox.PushButton -> CEGUI::PushButton *  (read-only)\n\nThis is equivalent to calling the C++ CEGUI method:\n\tget: CEGUI::PushButton * Combobox::getPushButton()")

    DropList = property(_cegui.Combobox_DropList_get, None, None,
                    "Combobox.DropList -> CEGUI::ComboDropList *  (read-only)\n\nThis is equivalent to calling the C++ CEGUI method:\n\tget: CEGUI::ComboDropList * Combobox::getDropList()")

    SelectedItem = property(_cegui.Combobox_SelectedItem_get, None, None,
                    "Combobox.SelectedItem -> CEGUI::ListboxItem *  (read-only)\n\nThis is equivalent to calling the C++ CEGUI method:\n\tget: CEGUI::ListboxItem * Combobox::getSelectedItem()")

Combobox.getListboxItemFromIndex = new_instancemethod(_cegui.Combobox_getListboxItemFromIndex,None,Combobox)
Combobox.getItemIndex = new_instancemethod(_cegui.Combobox_getItemIndex,None,Combobox)
Combobox.isItemSelected = new_instancemethod(_cegui.Combobox_isItemSelected,None,Combobox)
Combobox.findItemWithText = new_instancemethod(_cegui.Combobox_findItemWithText,None,Combobox)
Combobox.isListboxItemInList = new_instancemethod(_cegui.Combobox_isListboxItemInList,None,Combobox)
Combobox.showDropList = new_instancemethod(_cegui.Combobox_showDropList,None,Combobox)
Combobox.hideDropList = new_instancemethod(_cegui.Combobox_hideDropList,None,Combobox)
Combobox.setSelection = new_instancemethod(_cegui.Combobox_setSelection,None,Combobox)
Combobox.activateEditbox = new_instancemethod(_cegui.Combobox_activateEditbox,None,Combobox)
Combobox.resetList = new_instancemethod(_cegui.Combobox_resetList,None,Combobox)
Combobox.addItem = new_instancemethod(_cegui.Combobox_addItem,None,Combobox)
Combobox.insertItem = new_instancemethod(_cegui.Combobox_insertItem,None,Combobox)
Combobox.removeItem = new_instancemethod(_cegui.Combobox_removeItem,None,Combobox)
Combobox.clearAllSelections = new_instancemethod(_cegui.Combobox_clearAllSelections,None,Combobox)
Combobox.setItemSelectState = new_instancemethod(_cegui.Combobox_setItemSelectState,None,Combobox)
Combobox.handleUpdatedListItemData = new_instancemethod(_cegui.Combobox_handleUpdatedListItemData,None,Combobox)
Combobox_swigregister = _cegui.Combobox_swigregister
Combobox_swigregister(Combobox)
Combobox.WidgetTypeName = _cegui.cvar.Combobox_WidgetTypeName
Combobox.EditboxNameSuffix = _cegui.cvar.Combobox_EditboxNameSuffix
Combobox.DropListNameSuffix = _cegui.cvar.Combobox_DropListNameSuffix
Combobox.ButtonNameSuffix = _cegui.cvar.Combobox_ButtonNameSuffix

class ButtonBase(Window):
    thisown = property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, *args): 
        _cegui.ButtonBase_swiginit(self,_cegui.new_ButtonBase(*args))
    __swig_destroy__ = _cegui.delete_ButtonBase
    Hovering = property(_cegui.ButtonBase_Hovering_get, None, None,
                    "ButtonBase.Hovering -> bool  (read-only)\n\nThis is equivalent to calling the C++ CEGUI method:\n\tget: bool ButtonBase::isHovering()")

    Pushed = property(_cegui.ButtonBase_Pushed_get, None, None,
                    "ButtonBase.Pushed -> bool  (read-only)\n\nThis is equivalent to calling the C++ CEGUI method:\n\tget: bool ButtonBase::isPushed()")

ButtonBase_swigregister = _cegui.ButtonBase_swigregister
ButtonBase_swigregister(ButtonBase)

class Checkbox(ButtonBase):
    thisown = property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, *args): 
        _cegui.Checkbox_swiginit(self,_cegui.new_Checkbox(*args))
    __swig_destroy__ = _cegui.delete_Checkbox
    EventNamespace = "Checkbox" 
    EventCheckStateChanged = "CheckStateChanged" 
    Selected = property(_cegui.Checkbox_Selected_get, _cegui.Checkbox_Selected_set, None,
                    "Checkbox.Selected -> bool\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: bool Checkbox::isSelected()\n\tset: void Checkbox::setSelected(bool)")


Checkbox_swigregister = _cegui.Checkbox_swigregister
Checkbox_swigregister(Checkbox)
Checkbox.WidgetTypeName = _cegui.cvar.Checkbox_WidgetTypeName

class EditboxWindowRenderer(WindowRenderer):
    thisown = property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    def __init__(self): raise AttributeError, "No constructor defined"
    __repr__ = _swig_repr
    __swig_destroy__ = _cegui.delete_EditboxWindowRenderer
EditboxWindowRenderer.getTextIndexFromPosition = new_instancemethod(_cegui.EditboxWindowRenderer_getTextIndexFromPosition,None,EditboxWindowRenderer)
EditboxWindowRenderer_swigregister = _cegui.EditboxWindowRenderer_swigregister
EditboxWindowRenderer_swigregister(EditboxWindowRenderer)

class Editbox(Window):
    thisown = property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, *args): 
        _cegui.Editbox_swiginit(self,_cegui.new_Editbox(*args))
    __swig_destroy__ = _cegui.delete_Editbox
    EventNamespace = "Editbox" 
    EventCaratMoved = "TextCaratMoved" 
    EventEditboxFull = "EditboxFull" 
    EventInvalidEntryAttempted = "InvalidInputAttempt" 
    EventMaskCodePointChanged = "MaskCPChanged" 
    EventMaskedRenderingModeChanged = "MaskRenderChanged" 
    EventMaximumTextLengthChanged = "MaxTextLenChanged" 
    EventReadOnlyModeChanged = "ReadOnlyChanged" 
    EventTextAccepted = "TextAccepted" 
    EventTextInvalidated = "TextInvalidated" 
    EventTextSelectionChanged = "TextSelectChanged" 
    EventValidationStringChanged = "ValidatorChanged" 
    SelectionStartIndex = property(_cegui.Editbox_SelectionStartIndex_get, _cegui.Editbox_SelectionStartIndex_set)
    SelectionEndIndex = property(_cegui.Editbox_SelectionEndIndex_get, _cegui.Editbox_SelectionEndIndex_set)
    ReadOnly = property(_cegui.Editbox_ReadOnly_get, _cegui.Editbox_ReadOnly_set, None,
                    "Editbox.ReadOnly -> bool\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: bool Editbox::isReadOnly()\n\tset: void Editbox::setReadOnly(bool)")


    HasInputFocus = property(_cegui.Editbox_HasInputFocus_get, None, None,
                    "Editbox.HasInputFocus -> bool  (read-only)\n\nThis is equivalent to calling the C++ CEGUI method:\n\tget: bool Editbox::hasInputFocus()")

    TextMasked = property(_cegui.Editbox_TextMasked_get, _cegui.Editbox_TextMasked_set, None,
                    "Editbox.TextMasked -> bool\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: bool Editbox::isTextMasked()\n\tset: void Editbox::setTextMasked(bool)")


    TextValid = property(_cegui.Editbox_TextValid_get, None, None,
                    "Editbox.TextValid -> bool  (read-only)\n\nThis is equivalent to calling the C++ CEGUI method:\n\tget: bool Editbox::isTextValid()")

    ValidationString = property(_cegui.Editbox_ValidationString_get, _cegui.Editbox_ValidationString_set, None,
                    "Editbox.ValidationString -> const CEGUI::String &\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: const CEGUI::String & Editbox::getValidationString()\n\tset: void Editbox::setValidationString(const CEGUI::String &)")


    CaratIndex = property(_cegui.Editbox_CaratIndex_get, _cegui.Editbox_CaratIndex_set, None,
                    "Editbox.CaratIndex -> size_t\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: size_t Editbox::getCaratIndex()\n\tset: void Editbox::setCaratIndex(size_t)")


    SelectionLength = property(_cegui.Editbox_SelectionLength_get, None, None,
                    "Editbox.SelectionLength -> size_t  (read-only)\n\nThis is equivalent to calling the C++ CEGUI method:\n\tget: size_t Editbox::getSelectionLength()")

    MaxTextLength = property(_cegui.Editbox_MaxTextLength_get, _cegui.Editbox_MaxTextLength_set, None,
                    "Editbox.MaxTextLength -> size_t\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: size_t Editbox::getMaxTextLength()\n\tset: void Editbox::setMaxTextLength(size_t)")


    MaskCodePoint = property(_cegui.Editbox_MaskCodePoint_get, _cegui.Editbox_MaskCodePoint_set, None,
                    "Editbox.MaskCodePoint -> uint32\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: uint32 Editbox::getMaskCodePoint()\n\tset: void Editbox::setMaskCodePoint(uint32)")


Editbox.setSelection = new_instancemethod(_cegui.Editbox_setSelection,None,Editbox)
Editbox_swigregister = _cegui.Editbox_swigregister
Editbox_swigregister(Editbox)
Editbox.WidgetTypeName = _cegui.cvar.Editbox_WidgetTypeName

class FrameWindow(Window):
    thisown = property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    SizingNone = _cegui.FrameWindow_SizingNone
    SizingTopLeft = _cegui.FrameWindow_SizingTopLeft
    SizingTopRight = _cegui.FrameWindow_SizingTopRight
    SizingBottomLeft = _cegui.FrameWindow_SizingBottomLeft
    SizingBottomRight = _cegui.FrameWindow_SizingBottomRight
    SizingTop = _cegui.FrameWindow_SizingTop
    SizingLeft = _cegui.FrameWindow_SizingLeft
    SizingBottom = _cegui.FrameWindow_SizingBottom
    SizingRight = _cegui.FrameWindow_SizingRight




    NSSizingCursorImage = property(_cegui.FrameWindow___getNSSizingCursorImage, fix_args(_cegui.FrameWindow___setNSSizingCursorImage), None,
                    "FrameWindow.NSSizingCursorImage -> type\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: FrameWindow::getNSSizingCursorImage()\n\tset: void FrameWindow::setNSSizingCursorImage(...)")

    EWSizingCursorImage = property(_cegui.FrameWindow___getEWSizingCursorImage, fix_args(_cegui.FrameWindow___setEWSizingCursorImage), None,
                    "FrameWindow.EWSizingCursorImage -> type\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: FrameWindow::getEWSizingCursorImage()\n\tset: void FrameWindow::setEWSizingCursorImage(...)")

    NWSESizingCursorImage = property(_cegui.FrameWindow___getNWSESizingCursorImage, fix_args(_cegui.FrameWindow___setNWSESizingCursorImage), None,
                    "FrameWindow.NWSESizingCursorImage -> type\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: FrameWindow::getNWSESizingCursorImage()\n\tset: void FrameWindow::setNWSESizingCursorImage(...)")

    NESWSizingCursorImage = property(_cegui.FrameWindow___getNESWSizingCursorImage, fix_args(_cegui.FrameWindow___setNESWSizingCursorImage), None,
                    "FrameWindow.NESWSizingCursorImage -> type\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: FrameWindow::getNESWSizingCursorImage()\n\tset: void FrameWindow::setNESWSizingCursorImage(...)")

    def __init__(self, *args): 
        _cegui.FrameWindow_swiginit(self,_cegui.new_FrameWindow(*args))
    __swig_destroy__ = _cegui.delete_FrameWindow
    EventNamespace = "FrameWindow" 
    EventCloseClicked = "CloseClicked" 
    EventRollupToggled = "RollupToggled" 
    SizingEnabled = property(_cegui.FrameWindow_SizingEnabled_get, _cegui.FrameWindow_SizingEnabled_set, None,
                    "FrameWindow.SizingEnabled -> bool\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: bool FrameWindow::isSizingEnabled()\n\tset: void FrameWindow::setSizingEnabled(bool)")


    TitleBarEnabled = property(_cegui.FrameWindow_TitleBarEnabled_get, _cegui.FrameWindow_TitleBarEnabled_set, None,
                    "FrameWindow.TitleBarEnabled -> bool\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: bool FrameWindow::isTitleBarEnabled()\n\tset: void FrameWindow::setTitleBarEnabled(bool)")


    CloseButtonEnabled = property(_cegui.FrameWindow_CloseButtonEnabled_get, _cegui.FrameWindow_CloseButtonEnabled_set, None,
                    "FrameWindow.CloseButtonEnabled -> bool\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: bool FrameWindow::isCloseButtonEnabled()\n\tset: void FrameWindow::setCloseButtonEnabled(bool)")


    RollupEnabled = property(_cegui.FrameWindow_RollupEnabled_get, _cegui.FrameWindow_RollupEnabled_set, None,
                    "FrameWindow.RollupEnabled -> bool\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: bool FrameWindow::isRollupEnabled()\n\tset: void FrameWindow::setRollupEnabled(bool)")


    FrameEnabled = property(_cegui.FrameWindow_FrameEnabled_get, _cegui.FrameWindow_FrameEnabled_set, None,
                    "FrameWindow.FrameEnabled -> bool\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: bool FrameWindow::isFrameEnabled()\n\tset: void FrameWindow::setFrameEnabled(bool)")


    DragMovingEnabled = property(_cegui.FrameWindow_DragMovingEnabled_get, _cegui.FrameWindow_DragMovingEnabled_set, None,
                    "FrameWindow.DragMovingEnabled -> bool\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: bool FrameWindow::isDragMovingEnabled()\n\tset: void FrameWindow::setDragMovingEnabled(bool)")


    SizingBorderThickness = property(_cegui.FrameWindow_SizingBorderThickness_get, _cegui.FrameWindow_SizingBorderThickness_set, None,
                    "FrameWindow.SizingBorderThickness -> float\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: float FrameWindow::getSizingBorderThickness()\n\tset: void FrameWindow::setSizingBorderThickness(float)")


    Rolledup = property(_cegui.FrameWindow_Rolledup_get, _cegui.FrameWindow_Rolledup_set)
FrameWindow.toggleRollup = new_instancemethod(_cegui.FrameWindow_toggleRollup,None,FrameWindow)
FrameWindow.offsetPixelPosition = new_instancemethod(_cegui.FrameWindow_offsetPixelPosition,None,FrameWindow)
FrameWindow.getTitlebar = new_instancemethod(_cegui.FrameWindow_getTitlebar,None,FrameWindow)
FrameWindow.getCloseButton = new_instancemethod(_cegui.FrameWindow_getCloseButton,None,FrameWindow)
FrameWindow_swigregister = _cegui.FrameWindow_swigregister
FrameWindow_swigregister(FrameWindow)
FrameWindow.WidgetTypeName = _cegui.cvar.FrameWindow_WidgetTypeName
FrameWindow.DefaultSizingBorderSize = _cegui.cvar.FrameWindow_DefaultSizingBorderSize
FrameWindow.TitlebarNameSuffix = _cegui.cvar.FrameWindow_TitlebarNameSuffix
FrameWindow.CloseButtonNameSuffix = _cegui.cvar.FrameWindow_CloseButtonNameSuffix

class ScrolledContainer(Window):
    thisown = property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, *args): 
        _cegui.ScrolledContainer_swiginit(self,_cegui.new_ScrolledContainer(*args))
    __swig_destroy__ = _cegui.delete_ScrolledContainer
    EventNamespace = "ScrolledContainer" 
    EventAutoSizeSettingChanged = "AutoSizeSettingChanged" 
    EventContentChanged = "ContentChanged" 
    AutoSize = property(_cegui.ScrolledContainer_AutoSize_get, _cegui.ScrolledContainer_AutoSize_set, None,
                    "ScrolledContainer.AutoSize -> bool\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: bool ScrolledContainer::isContentPaneAutoSized()\n\tset: void ScrolledContainer::setContentPaneAutoSized(bool)")


    ContentArea = property(_cegui.ScrolledContainer_ContentArea_get, _cegui.ScrolledContainer_ContentArea_set, None,
                    "ScrolledContainer.ContentArea -> const CEGUI::Rect &\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: const CEGUI::Rect & ScrolledContainer::getContentArea()\n\tset: void ScrolledContainer::setContentArea(const CEGUI::Rect &)")


    ChildExtentsArea = property(_cegui.ScrolledContainer_ChildExtentsArea_get, None, None,
                    "ScrolledContainer.ChildExtentsArea -> CEGUI::Rect  (read-only)\n\nThis is equivalent to calling the C++ CEGUI method:\n\tget: CEGUI::Rect ScrolledContainer::getChildExtentsArea()")

ScrolledContainer_swigregister = _cegui.ScrolledContainer_swigregister
ScrolledContainer_swigregister(ScrolledContainer)
ScrolledContainer.WidgetTypeName = _cegui.cvar.ScrolledContainer_WidgetTypeName

class ScrollablePaneWindowRenderer(WindowRenderer):
    thisown = property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    def __init__(self): raise AttributeError, "No constructor defined"
    __repr__ = _swig_repr
    __swig_destroy__ = _cegui.delete_ScrollablePaneWindowRenderer
ScrollablePaneWindowRenderer.getViewableArea = new_instancemethod(_cegui.ScrollablePaneWindowRenderer_getViewableArea,None,ScrollablePaneWindowRenderer)
ScrollablePaneWindowRenderer_swigregister = _cegui.ScrollablePaneWindowRenderer_swigregister
ScrollablePaneWindowRenderer_swigregister(ScrollablePaneWindowRenderer)

class ScrollablePane(Window):
    thisown = property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, *args): 
        _cegui.ScrollablePane_swiginit(self,_cegui.new_ScrollablePane(*args))
    __swig_destroy__ = _cegui.delete_ScrollablePane
    EventNamespace = "ScrollablePane" 
    EventAutoSizeSettingChanged = "AutoSizeSettingChanged" 
    EventContentPaneChanged = "ContentPaneChanged" 
    EventContentPaneScrolled = "ContentPaneScrolled" 
    EventHorzScrollbarModeChanged = "HorzScrollbarModeChanged" 
    EventVertScrollbarModeChanged = "VertScrollbarModeChanged" 
    ContentPane = property(_cegui.ScrollablePane_ContentPane_get, None, None,
                    "ScrollablePane.ContentPane -> const ScrolledContainer *  (read-only)\n\nThis is equivalent to calling the C++ CEGUI method:\n\tget: const ScrolledContainer * ScrollablePane::getContentPane()")

    ShowVertScrollbar = property(_cegui.ScrollablePane_ShowVertScrollbar_get, _cegui.ScrollablePane_ShowVertScrollbar_set, None,
                    "ScrollablePane.ShowVertScrollbar -> bool\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: bool ScrollablePane::isVertScrollbarAlwaysShown()\n\tset: void ScrollablePane::setShowVertScrollbar(bool)")


    ShowHorzScrollbar = property(_cegui.ScrollablePane_ShowHorzScrollbar_get, _cegui.ScrollablePane_ShowHorzScrollbar_set, None,
                    "ScrollablePane.ShowHorzScrollbar -> bool\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: bool ScrollablePane::isHorzScrollbarAlwaysShown()\n\tset: void ScrollablePane::setShowHorzScrollbar(bool)")


    AutoSize = property(_cegui.ScrollablePane_AutoSize_get, _cegui.ScrollablePane_AutoSize_set, None,
                    "ScrollablePane.AutoSize -> bool\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: bool ScrollablePane::isContentPaneAutoSized()\n\tset: void ScrollablePane::setContentPaneAutoSized(bool)")


    PaneArea = property(_cegui.ScrollablePane_PaneArea_get, _cegui.ScrollablePane_PaneArea_set, None,
                    "ScrollablePane.PaneArea -> const Rect &\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: const Rect & ScrollablePane::getContentPaneArea()\n\tset: void ScrollablePane::setContentPaneArea(const Rect &)")


    ViewableArea = property(_cegui.ScrollablePane_ViewableArea_get, None, None,
                    "ScrollablePane.ViewableArea -> const Rect &  (read-only)\n\nThis is equivalent to calling the C++ CEGUI method:\n\tget: const Rect & ScrollablePane::getViewableArea()")

    HorzStepSize = property(_cegui.ScrollablePane_HorzStepSize_get, _cegui.ScrollablePane_HorzStepSize_set, None,
                    "ScrollablePane.HorzStepSize -> float\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: float ScrollablePane::getHorizontalStepSize()\n\tset: void ScrollablePane::setHorizontalStepSize(float)")


    HorzOverlapSize = property(_cegui.ScrollablePane_HorzOverlapSize_get, _cegui.ScrollablePane_HorzOverlapSize_set, None,
                    "ScrollablePane.HorzOverlapSize -> float\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: float ScrollablePane::getHorizontalOverlapSize()\n\tset: void ScrollablePane::setHorizontalOverlapSize(float)")


    HorzScrollPosition = property(_cegui.ScrollablePane_HorzScrollPosition_get, _cegui.ScrollablePane_HorzScrollPosition_set, None,
                    "ScrollablePane.HorzScrollPosition -> float\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: float ScrollablePane::getHorizontalScrollPosition()\n\tset: void ScrollablePane::setHorizontalScrollPosition(float)")


    VertStepSize = property(_cegui.ScrollablePane_VertStepSize_get, _cegui.ScrollablePane_VertStepSize_set, None,
                    "ScrollablePane.VertStepSize -> float\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: float ScrollablePane::getVerticalStepSize()\n\tset: void ScrollablePane::setVerticalStepSize(float)")


    VertOverlapSize = property(_cegui.ScrollablePane_VertOverlapSize_get, _cegui.ScrollablePane_VertOverlapSize_set, None,
                    "ScrollablePane.VertOverlapSize -> float\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: float ScrollablePane::getVerticalOverlapSize()\n\tset: void ScrollablePane::setVerticalOverlapSize(float)")


    VertScrollPosition = property(_cegui.ScrollablePane_VertScrollPosition_get, _cegui.ScrollablePane_VertScrollPosition_set, None,
                    "ScrollablePane.VertScrollPosition -> float\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: float ScrollablePane::getVerticalScrollPosition()\n\tset: void ScrollablePane::setVerticalScrollPosition(float)")


ScrollablePane.getVertScrollbar = new_instancemethod(_cegui.ScrollablePane_getVertScrollbar,None,ScrollablePane)
ScrollablePane.getHorzScrollbar = new_instancemethod(_cegui.ScrollablePane_getHorzScrollbar,None,ScrollablePane)
ScrollablePane_swigregister = _cegui.ScrollablePane_swigregister
ScrollablePane_swigregister(ScrollablePane)
ScrollablePane.WidgetTypeName = _cegui.cvar.ScrollablePane_WidgetTypeName
ScrollablePane.VertScrollbarNameSuffix = _cegui.cvar.ScrollablePane_VertScrollbarNameSuffix
ScrollablePane.HorzScrollbarNameSuffix = _cegui.cvar.ScrollablePane_HorzScrollbarNameSuffix
ScrollablePane.ScrolledContainerNameSuffix = _cegui.cvar.ScrollablePane_ScrolledContainerNameSuffix

class GUISheet(Window):
    thisown = property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, *args): 
        _cegui.GUISheet_swiginit(self,_cegui.new_GUISheet(*args))
    __swig_destroy__ = _cegui.delete_GUISheet
GUISheet_swigregister = _cegui.GUISheet_swigregister
GUISheet_swigregister(GUISheet)
GUISheet.WidgetTypeName = _cegui.cvar.GUISheet_WidgetTypeName

class ListboxWindowRenderer(WindowRenderer):
    thisown = property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    def __init__(self): raise AttributeError, "No constructor defined"
    __repr__ = _swig_repr
    __swig_destroy__ = _cegui.delete_ListboxWindowRenderer
ListboxWindowRenderer.getListRenderArea = new_instancemethod(_cegui.ListboxWindowRenderer_getListRenderArea,None,ListboxWindowRenderer)
ListboxWindowRenderer_swigregister = _cegui.ListboxWindowRenderer_swigregister
ListboxWindowRenderer_swigregister(ListboxWindowRenderer)

class Listbox(Window):
    thisown = property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def addItem(self, item):
        retval = _cegui.Listbox_addItem(self, item)
        
        item.AutoDeleted = True
        item.thisown = 0
        
        return retval


    def insertItem(self, item, position):
        retval = _cegui.Listbox_insertItem(self, item, position)
        
        item.AutoDeleted = True
        item.thisown = 0
        
        return retval


    def removeItem(self, item):
        if not isinstance(item, ListboxItem):
            raise TypeError, 'item must be of type ListboxItem'
            
        item.AutoDeleted = False
        
        return _cegui.Listbox_removeItem(self, item)


    def __init__(self, *args): 
        _cegui.Listbox_swiginit(self,_cegui.new_Listbox(*args))
    __swig_destroy__ = _cegui.delete_Listbox
    EventNamespace = "Listbox" 
    EventHorzScrollbarModeChanged = "HorzScrollModeChanged" 
    EventListContentsChanged = "ListItemsChanged" 
    EventMultiselectModeChanged = "MuliselectModeChanged" 
    EventSelectionChanged = "ItemSelectionChanged" 
    EventSortModeChanged = "SortModeChanged" 
    EventVertScrollbarModeChanged = "VertScrollModeChanged" 
    ItemCount = property(_cegui.Listbox_ItemCount_get, None, None,
                    "Listbox.ItemCount -> size_t  (read-only)\n\nThis is equivalent to calling the C++ CEGUI method:\n\tget: size_t Listbox::getItemCount()")

    SelectedCount = property(_cegui.Listbox_SelectedCount_get, None, None,
                    "Listbox.SelectedCount -> size_t  (read-only)\n\nThis is equivalent to calling the C++ CEGUI method:\n\tget: size_t Listbox::getSelectedCount()")

    TotalItemsHeight = property(_cegui.Listbox_TotalItemsHeight_get, None, None,
                    "Listbox.TotalItemsHeight -> float  (read-only)\n\nThis is equivalent to calling the C++ CEGUI method:\n\tget: float Listbox::getTotalItemsHeight()")

    WidestItemWidth = property(_cegui.Listbox_WidestItemWidth_get, None, None,
                    "Listbox.WidestItemWidth -> float  (read-only)\n\nThis is equivalent to calling the C++ CEGUI method:\n\tget: float Listbox::getWidestItemWidth()")

    SortEnabled = property(_cegui.Listbox_SortEnabled_get, _cegui.Listbox_SortEnabled_set, None,
                    "Listbox.SortEnabled -> bool\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: bool Listbox::isSortEnabled()\n\tset: void Listbox::setSortingEnabled(bool)")


    MultiselectEnabled = property(_cegui.Listbox_MultiselectEnabled_get, _cegui.Listbox_MultiselectEnabled_set, None,
                    "Listbox.MultiselectEnabled -> bool\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: bool Listbox::isMultiselectEnabled()\n\tset: void Listbox::setMultiselectEnabled(bool)")


    ShowVertScrollbar = property(_cegui.Listbox_ShowVertScrollbar_get, _cegui.Listbox_ShowVertScrollbar_set, None,
                    "Listbox.ShowVertScrollbar -> bool\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: bool Listbox::isVertScrollbarAlwaysShown()\n\tset: void Listbox::setShowVertScrollbar(bool)")


    ShowHorzScrollbar = property(_cegui.Listbox_ShowHorzScrollbar_get, _cegui.Listbox_ShowHorzScrollbar_set, None,
                    "Listbox.ShowHorzScrollbar -> bool\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: bool Listbox::isHorzScrollbarAlwaysShown()\n\tset: void Listbox::setShowHorzScrollbar(bool)")


    ItemTooltipsEnabled = property(_cegui.Listbox_ItemTooltipsEnabled_get, _cegui.Listbox_ItemTooltipsEnabled_set, None,
                    "Listbox.ItemTooltipsEnabled -> bool\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: bool Listbox::isItemTooltipsEnabled()\n\tset: void Listbox::setItemTooltipsEnabled(bool)")


Listbox.getFirstSelectedItem = new_instancemethod(_cegui.Listbox_getFirstSelectedItem,None,Listbox)
Listbox.getNextSelected = new_instancemethod(_cegui.Listbox_getNextSelected,None,Listbox)
Listbox.getListboxItemFromIndex = new_instancemethod(_cegui.Listbox_getListboxItemFromIndex,None,Listbox)
Listbox.getItemIndex = new_instancemethod(_cegui.Listbox_getItemIndex,None,Listbox)
Listbox.isItemSelected = new_instancemethod(_cegui.Listbox_isItemSelected,None,Listbox)
Listbox.findItemWithText = new_instancemethod(_cegui.Listbox_findItemWithText,None,Listbox)
Listbox.isListboxItemInList = new_instancemethod(_cegui.Listbox_isListboxItemInList,None,Listbox)
Listbox.resetList = new_instancemethod(_cegui.Listbox_resetList,None,Listbox)
Listbox.clearAllSelections = new_instancemethod(_cegui.Listbox_clearAllSelections,None,Listbox)
Listbox.setItemSelectState = new_instancemethod(_cegui.Listbox_setItemSelectState,None,Listbox)
Listbox.handleUpdatedItemData = new_instancemethod(_cegui.Listbox_handleUpdatedItemData,None,Listbox)
Listbox.ensureItemIsVisible = new_instancemethod(_cegui.Listbox_ensureItemIsVisible,None,Listbox)
Listbox.getListRenderArea = new_instancemethod(_cegui.Listbox_getListRenderArea,None,Listbox)
Listbox.getVertScrollbar = new_instancemethod(_cegui.Listbox_getVertScrollbar,None,Listbox)
Listbox.getHorzScrollbar = new_instancemethod(_cegui.Listbox_getHorzScrollbar,None,Listbox)
Listbox_swigregister = _cegui.Listbox_swigregister
Listbox_swigregister(Listbox)
Listbox.WidgetTypeName = _cegui.cvar.Listbox_WidgetTypeName
Listbox.VertScrollbarNameSuffix = _cegui.cvar.Listbox_VertScrollbarNameSuffix
Listbox.HorzScrollbarNameSuffix = _cegui.cvar.Listbox_HorzScrollbarNameSuffix

class ComboDropList(Listbox):
    thisown = property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, *args): 
        _cegui.ComboDropList_swiginit(self,_cegui.new_ComboDropList(*args))
    __swig_destroy__ = _cegui.delete_ComboDropList
    EventNamespace = "ComboDropList" 
    EventHorzScrollbarModeChanged = "HorzScrollModeChanged" 
    EventListContentsChanged = "ListItemsChanged" 
    EventListSelectionAccepted = "ListSelectionAccepted" 
    EventMultiselectModeChanged = "MuliselectModeChanged" 
    EventSelectionChanged = "ItemSelectionChanged" 
    EventSortModeChanged = "SortModeChanged" 
    EventVertScrollbarModeChanged = "VertScrollModeChanged" 
    Armed = property(_cegui.ComboDropList_Armed_get, _cegui.ComboDropList_Armed_set, None,
                    "ComboDropList.Armed -> bool\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: bool ComboDropList::isArmed()\n\tset: void ComboDropList::setArmed(bool)")


    AutoArmEnabled = property(_cegui.ComboDropList_AutoArmEnabled_get, _cegui.ComboDropList_AutoArmEnabled_set, None,
                    "ComboDropList.AutoArmEnabled -> bool\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: bool ComboDropList::isAutoArmEnabled()\n\tset: void ComboDropList::setAutoArmEnabled(bool)")


ComboDropList_swigregister = _cegui.ComboDropList_swigregister
ComboDropList_swigregister(ComboDropList)
ComboDropList.WidgetTypeName = _cegui.cvar.ComboDropList_WidgetTypeName

class ListHeaderSegment(Window):
    thisown = property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    NoSort = _cegui.ListHeaderSegment_NoSort
    Ascending = _cegui.ListHeaderSegment_Ascending
    Descending = _cegui.ListHeaderSegment_Descending


    SizingCursorImage = property(_cegui.ListHeaderSegment___getSizingCursorImage, fix_args(_cegui.ListHeaderSegment___setSizingCursorImage), None,
                    "ListHeaderSegment.SizingCursorImage -> type\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: ListHeaderSegment::getSizingCursorImage()\n\tset: void ListHeaderSegment::setSizingCursorImage(...)")

    MovingCursorImage = property(_cegui.ListHeaderSegment___getMovingCursorImage, fix_args(_cegui.ListHeaderSegment___setMovingCursorImage), None,
                    "ListHeaderSegment.MovingCursorImage -> type\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: ListHeaderSegment::getMovingCursorImage()\n\tset: void ListHeaderSegment::setMovingCursorImage(...)")

    def __init__(self, *args): 
        _cegui.ListHeaderSegment_swiginit(self,_cegui.new_ListHeaderSegment(*args))
    __swig_destroy__ = _cegui.delete_ListHeaderSegment
    EventNamespace = "ListHeaderSegment" 
    EventClickableSettingChanged = "ClickableSettingChanged" 
    EventMovableSettingChanged = "MovableSettingChanged" 
    EventSegmentClicked = "SegmentClicked" 
    EventSegmentDragPositionChanged = "SegmentDragPositionChanged" 
    EventSegmentDragStart = "SegmentDragStart" 
    EventSegmentDragStop = "SegmentDragStop" 
    EventSegmentSized = "SegmentSized" 
    EventSizingSettingChanged = "SizingSettingChanged" 
    EventSortDirectionChanged = "SortDirectionChanged" 
    EventSplitterDoubleClicked = "SplitterDoubleClicked" 
    Clickable = property(_cegui.ListHeaderSegment_Clickable_get, _cegui.ListHeaderSegment_Clickable_set, None,
                    "ListHeaderSegment.Clickable -> bool\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: bool ListHeaderSegment::isClickable()\n\tset: void ListHeaderSegment::setClickable(bool)")


    DragMovingEnabled = property(_cegui.ListHeaderSegment_DragMovingEnabled_get, _cegui.ListHeaderSegment_DragMovingEnabled_set, None,
                    "ListHeaderSegment.DragMovingEnabled -> bool\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: bool ListHeaderSegment::isDragMovingEnabled()\n\tset: void ListHeaderSegment::setDragMovingEnabled(bool)")


    SizingEnabled = property(_cegui.ListHeaderSegment_SizingEnabled_get, _cegui.ListHeaderSegment_SizingEnabled_set, None,
                    "ListHeaderSegment.SizingEnabled -> bool\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: bool ListHeaderSegment::isSizingEnabled()\n\tset: void ListHeaderSegment::setSizingEnabled(bool)")


    IsDragMoving = property(_cegui.ListHeaderSegment_IsDragMoving_get, None, None,
                    "ListHeaderSegment.IsDragMoving -> bool  (read-only)\n\nThis is equivalent to calling the C++ CEGUI method:\n\tget: bool ListHeaderSegment::isBeingDragMoved()")

    IsDragSizing = property(_cegui.ListHeaderSegment_IsDragSizing_get, None, None,
                    "ListHeaderSegment.IsDragSizing -> bool  (read-only)\n\nThis is equivalent to calling the C++ CEGUI method:\n\tget: bool ListHeaderSegment::isBeingDragSized()")

    SegmentHovering = property(_cegui.ListHeaderSegment_SegmentHovering_get, None, None,
                    "ListHeaderSegment.SegmentHovering -> bool  (read-only)\n\nThis is equivalent to calling the C++ CEGUI method:\n\tget: bool ListHeaderSegment::isSegmentHovering()")

    SegmentPushed = property(_cegui.ListHeaderSegment_SegmentPushed_get, None, None,
                    "ListHeaderSegment.SegmentPushed -> bool  (read-only)\n\nThis is equivalent to calling the C++ CEGUI method:\n\tget: bool ListHeaderSegment::isSegmentPushed()")

    SplitterHovering = property(_cegui.ListHeaderSegment_SplitterHovering_get, None, None,
                    "ListHeaderSegment.SplitterHovering -> bool  (read-only)\n\nThis is equivalent to calling the C++ CEGUI method:\n\tget: bool ListHeaderSegment::isSplitterHovering()")

    SortDirection = property(_cegui.ListHeaderSegment_SortDirection_get, _cegui.ListHeaderSegment_SortDirection_set, None,
                    "ListHeaderSegment.SortDirection -> CEGUI::ListHeaderSegment::SortDirection\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: CEGUI::ListHeaderSegment::SortDirection ListHeaderSegment::getSortDirection()\n\tset: void ListHeaderSegment::setSortDirection(CEGUI::ListHeaderSegment::SortDirection)")


    DragMoveOffset = property(_cegui.ListHeaderSegment_DragMoveOffset_get, None, None,
                    "ListHeaderSegment.DragMoveOffset -> CEGUI::Point  (read-only)\n\nThis is equivalent to calling the C++ CEGUI method:\n\tget: CEGUI::Point ListHeaderSegment::getDragMoveOffset()")

ListHeaderSegment_swigregister = _cegui.ListHeaderSegment_swigregister
ListHeaderSegment_swigregister(ListHeaderSegment)
ListHeaderSegment.WidgetTypeName = _cegui.cvar.ListHeaderSegment_WidgetTypeName
ListHeaderSegment.DefaultSizingArea = _cegui.cvar.ListHeaderSegment_DefaultSizingArea
ListHeaderSegment.SegmentMoveThreshold = _cegui.cvar.ListHeaderSegment_SegmentMoveThreshold

class MCLGridRef(object):
    thisown = property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, *args): 
        _cegui.MCLGridRef_swiginit(self,_cegui.new_MCLGridRef(*args))
    row = property(_cegui.MCLGridRef_row_get, _cegui.MCLGridRef_row_set)
    column = property(_cegui.MCLGridRef_column_get, _cegui.MCLGridRef_column_set)
    __swig_destroy__ = _cegui.delete_MCLGridRef
MCLGridRef.__lt__ = new_instancemethod(_cegui.MCLGridRef___lt__,None,MCLGridRef)
MCLGridRef.__le__ = new_instancemethod(_cegui.MCLGridRef___le__,None,MCLGridRef)
MCLGridRef.__gt__ = new_instancemethod(_cegui.MCLGridRef___gt__,None,MCLGridRef)
MCLGridRef.__ge__ = new_instancemethod(_cegui.MCLGridRef___ge__,None,MCLGridRef)
MCLGridRef.__eq__ = new_instancemethod(_cegui.MCLGridRef___eq__,None,MCLGridRef)
MCLGridRef.__ne__ = new_instancemethod(_cegui.MCLGridRef___ne__,None,MCLGridRef)
MCLGridRef_swigregister = _cegui.MCLGridRef_swigregister
MCLGridRef_swigregister(MCLGridRef)

class MultiColumnListWindowRenderer(WindowRenderer):
    thisown = property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    def __init__(self): raise AttributeError, "No constructor defined"
    __repr__ = _swig_repr
    __swig_destroy__ = _cegui.delete_MultiColumnListWindowRenderer
MultiColumnListWindowRenderer.getListRenderArea = new_instancemethod(_cegui.MultiColumnListWindowRenderer_getListRenderArea,None,MultiColumnListWindowRenderer)
MultiColumnListWindowRenderer_swigregister = _cegui.MultiColumnListWindowRenderer_swigregister
MultiColumnListWindowRenderer_swigregister(MultiColumnListWindowRenderer)

class MultiColumnList(Window):
    thisown = property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    RowSingle = _cegui.MultiColumnList_RowSingle
    RowMultiple = _cegui.MultiColumnList_RowMultiple
    CellSingle = _cegui.MultiColumnList_CellSingle
    CellMultiple = _cegui.MultiColumnList_CellMultiple
    NominatedColumnSingle = _cegui.MultiColumnList_NominatedColumnSingle
    NominatedColumnMultiple = _cegui.MultiColumnList_NominatedColumnMultiple
    ColumnSingle = _cegui.MultiColumnList_ColumnSingle
    ColumnMultiple = _cegui.MultiColumnList_ColumnMultiple
    NominatedRowSingle = _cegui.MultiColumnList_NominatedRowSingle
    NominatedRowMultiple = _cegui.MultiColumnList_NominatedRowMultiple
    def addRow(self, item, *args):
        retval = _cegui.MultiColumnList_addRow(self, item, *args)

        item.AutoDeleted = True
        item.thisown = 0

        return retval


    def insertRow(self, item, *args):
        retval = _cegui.MultiColumnList_insertRow(self, item, *args)

        item.AutoDeleted = True
        item.thisown = 0

        return retval


    def setItem(self, item, col_id, row_idx):
        retval = _cegui.MultiColumnList_setItem(self, item, col_id, row_idx)
        
        item.AutoDeleted = True
        item.thisown = 0

        return retval
        


    def __init__(self, *args): 
        _cegui.MultiColumnList_swiginit(self,_cegui.new_MultiColumnList(*args))
    __swig_destroy__ = _cegui.delete_MultiColumnList
    EventNamespace = "MultiColumnList" 
    EventHorzScrollbarModeChanged = "HorzBarModeChanged" 
    EventListColumnMoved = "ColMoved" 
    EventListColumnSized = "ColSized" 
    EventListContentsChanged = "ContentsChanged" 
    EventNominatedSelectColumnChanged = "NomSelColChanged" 
    EventNominatedSelectRowChanged = "NomSelRowChanged" 
    EventSelectionChanged = "SelectionChanged" 
    EventSelectionModeChanged = "SelectModeChanged" 
    EventSortColumnChanged = "SortColChanged" 
    EventSortDirectionChanged = "SortDirChanged" 
    EventVertScrollbarModeChanged = "VertBarModeChanged" 
    UserSortControlEnabled = property(_cegui.MultiColumnList_UserSortControlEnabled_get, _cegui.MultiColumnList_UserSortControlEnabled_set, None,
                    "MultiColumnList.UserSortControlEnabled -> bool\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: bool MultiColumnList::isUserSortControlEnabled()\n\tset: void MultiColumnList::setUserSortControlEnabled(bool)")


    UserColumnSizingEnabled = property(_cegui.MultiColumnList_UserColumnSizingEnabled_get, _cegui.MultiColumnList_UserColumnSizingEnabled_set, None,
                    "MultiColumnList.UserColumnSizingEnabled -> bool\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: bool MultiColumnList::isUserColumnSizingEnabled()\n\tset: void MultiColumnList::setUserColumnSizingEnabled(bool)")


    UserColumnDraggingEnabled = property(_cegui.MultiColumnList_UserColumnDraggingEnabled_get, _cegui.MultiColumnList_UserColumnDraggingEnabled_set, None,
                    "MultiColumnList.UserColumnDraggingEnabled -> bool\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: bool MultiColumnList::isUserColumnDraggingEnabled()\n\tset: void MultiColumnList::setUserColumnDraggingEnabled(bool)")


    ShowVertScrollbar = property(_cegui.MultiColumnList_ShowVertScrollbar_get, _cegui.MultiColumnList_ShowVertScrollbar_set, None,
                    "MultiColumnList.ShowVertScrollbar -> bool\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: bool MultiColumnList::isVertScrollbarAlwaysShown()\n\tset: void MultiColumnList::setShowVertScrollbar(bool)")


    ShowHorzScrollbar = property(_cegui.MultiColumnList_ShowHorzScrollbar_get, _cegui.MultiColumnList_ShowHorzScrollbar_set, None,
                    "MultiColumnList.ShowHorzScrollbar -> bool\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: bool MultiColumnList::isHorzScrollbarAlwaysShown()\n\tset: void MultiColumnList::setShowHorzScrollbar(bool)")


    ColumnCount = property(_cegui.MultiColumnList_ColumnCount_get, None, None,
                    "MultiColumnList.ColumnCount -> CEGUI::uint  (read-only)\n\nThis is equivalent to calling the C++ CEGUI method:\n\tget: CEGUI::uint MultiColumnList::getColumnCount()")

    RowCount = property(_cegui.MultiColumnList_RowCount_get, None, None,
                    "MultiColumnList.RowCount -> CEGUI::uint  (read-only)\n\nThis is equivalent to calling the C++ CEGUI method:\n\tget: CEGUI::uint MultiColumnList::getRowCount()")

    SortColumn = property(_cegui.MultiColumnList_SortColumn_get, _cegui.MultiColumnList_SortColumn_set, None,
                    "MultiColumnList.SortColumn -> CEGUI::uint\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: CEGUI::uint MultiColumnList::getSortColumn()\n\tset: void MultiColumnList::setSortColumn(CEGUI::uint)")


    SelectedCount = property(_cegui.MultiColumnList_SelectedCount_get, None, None,
                    "MultiColumnList.SelectedCount -> CEGUI::uint  (read-only)\n\nThis is equivalent to calling the C++ CEGUI method:\n\tget: CEGUI::uint MultiColumnList::getSelectedCount()")

    NominatedSelectionColumnID = property(_cegui.MultiColumnList_NominatedSelectionColumnID_get, _cegui.MultiColumnList_NominatedSelectionColumnID_set, None,
                    "MultiColumnList.NominatedSelectionColumnID -> CEGUI::uint\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: CEGUI::uint MultiColumnList::getNominatedSelectionColumnID()\n\tset: void MultiColumnList::setNominatedSelectionColumnID(CEGUI::uint)")


    NominatedSelectionColumn = property(_cegui.MultiColumnList_NominatedSelectionColumn_get, _cegui.MultiColumnList_NominatedSelectionColumn_set, None,
                    "MultiColumnList.NominatedSelectionColumn -> CEGUI::uint\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: CEGUI::uint MultiColumnList::getNominatedSelectionColumn()\n\tset: void MultiColumnList::setNominatedSelectionColumn(CEGUI::uint)")


    NominatedSelectionRow = property(_cegui.MultiColumnList_NominatedSelectionRow_get, _cegui.MultiColumnList_NominatedSelectionRow_set, None,
                    "MultiColumnList.NominatedSelectionRow -> CEGUI::uint\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: CEGUI::uint MultiColumnList::getNominatedSelectionRow()\n\tset: void MultiColumnList::setNominatedSelectionRow(CEGUI::uint)")


    TotalRowsHeight = property(_cegui.MultiColumnList_TotalRowsHeight_get, None, None,
                    "MultiColumnList.TotalRowsHeight -> CEGUI::uint  (read-only)\n\nThis is equivalent to calling the C++ CEGUI method:\n\tget: CEGUI::uint MultiColumnList::getTotalRowsHeight()")

    TotalColumnHeadersWidth = property(_cegui.MultiColumnList_TotalColumnHeadersWidth_get, None, None,
                    "MultiColumnList.TotalColumnHeadersWidth -> CEGUI::UDim  (read-only)\n\nThis is equivalent to calling the C++ CEGUI method:\n\tget: CEGUI::UDim MultiColumnList::getTotalColumnHeadersWidth()")

    SortDirection = property(_cegui.MultiColumnList_SortDirection_get, _cegui.MultiColumnList_SortDirection_set, None,
                    "MultiColumnList.SortDirection -> CEGUI::ListHeaderSegment::SortDirection\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: CEGUI::ListHeaderSegment::SortDirection MultiColumnList::getSortDirection()\n\tset: void MultiColumnList::setSortDirection(CEGUI::ListHeaderSegment::SortDirection)")


    SelectionMode = property(_cegui.MultiColumnList_SelectionMode_get, _cegui.MultiColumnList_SelectionMode_set, None,
                    "MultiColumnList.SelectionMode -> CEGUI::MultiColumnList::SelectionMode\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: CEGUI::MultiColumnList::SelectionMode MultiColumnList::getSelectionMode()\n\tset: void MultiColumnList::setSelectionMode(CEGUI::MultiColumnList::SelectionMode)")


MultiColumnList.getColumnWithID = new_instancemethod(_cegui.MultiColumnList_getColumnWithID,None,MultiColumnList)
MultiColumnList.getColumnWithHeaderText = new_instancemethod(_cegui.MultiColumnList_getColumnWithHeaderText,None,MultiColumnList)
MultiColumnList.getColumnHeaderWidth = new_instancemethod(_cegui.MultiColumnList_getColumnHeaderWidth,None,MultiColumnList)
MultiColumnList.getHeaderSegmentForColumn = new_instancemethod(_cegui.MultiColumnList_getHeaderSegmentForColumn,None,MultiColumnList)
MultiColumnList.getItemRowIndex = new_instancemethod(_cegui.MultiColumnList_getItemRowIndex,None,MultiColumnList)
MultiColumnList.getItemColumnIndex = new_instancemethod(_cegui.MultiColumnList_getItemColumnIndex,None,MultiColumnList)
MultiColumnList.getItemGridReference = new_instancemethod(_cegui.MultiColumnList_getItemGridReference,None,MultiColumnList)
MultiColumnList.getItemAtGridReference = new_instancemethod(_cegui.MultiColumnList_getItemAtGridReference,None,MultiColumnList)
MultiColumnList.isListboxItemInColumn = new_instancemethod(_cegui.MultiColumnList_isListboxItemInColumn,None,MultiColumnList)
MultiColumnList.isListboxItemInRow = new_instancemethod(_cegui.MultiColumnList_isListboxItemInRow,None,MultiColumnList)
MultiColumnList.isListboxItemInList = new_instancemethod(_cegui.MultiColumnList_isListboxItemInList,None,MultiColumnList)
MultiColumnList.findColumnItemWithText = new_instancemethod(_cegui.MultiColumnList_findColumnItemWithText,None,MultiColumnList)
MultiColumnList.findRowItemWithText = new_instancemethod(_cegui.MultiColumnList_findRowItemWithText,None,MultiColumnList)
MultiColumnList.findListItemWithText = new_instancemethod(_cegui.MultiColumnList_findListItemWithText,None,MultiColumnList)
MultiColumnList.getFirstSelectedItem = new_instancemethod(_cegui.MultiColumnList_getFirstSelectedItem,None,MultiColumnList)
MultiColumnList.getNextSelected = new_instancemethod(_cegui.MultiColumnList_getNextSelected,None,MultiColumnList)
MultiColumnList.isItemSelected = new_instancemethod(_cegui.MultiColumnList_isItemSelected,None,MultiColumnList)
MultiColumnList.getColumnID = new_instancemethod(_cegui.MultiColumnList_getColumnID,None,MultiColumnList)
MultiColumnList.getRowID = new_instancemethod(_cegui.MultiColumnList_getRowID,None,MultiColumnList)
MultiColumnList.getRowWithID = new_instancemethod(_cegui.MultiColumnList_getRowWithID,None,MultiColumnList)
MultiColumnList.getListRenderArea = new_instancemethod(_cegui.MultiColumnList_getListRenderArea,None,MultiColumnList)
MultiColumnList.getVertScrollbar = new_instancemethod(_cegui.MultiColumnList_getVertScrollbar,None,MultiColumnList)
MultiColumnList.getHorzScrollbar = new_instancemethod(_cegui.MultiColumnList_getHorzScrollbar,None,MultiColumnList)
MultiColumnList.getListHeader = new_instancemethod(_cegui.MultiColumnList_getListHeader,None,MultiColumnList)
MultiColumnList.getWidestColumnItemWidth = new_instancemethod(_cegui.MultiColumnList_getWidestColumnItemWidth,None,MultiColumnList)
MultiColumnList.getHighestRowItemHeight = new_instancemethod(_cegui.MultiColumnList_getHighestRowItemHeight,None,MultiColumnList)
MultiColumnList.resetList = new_instancemethod(_cegui.MultiColumnList_resetList,None,MultiColumnList)
MultiColumnList.addColumn = new_instancemethod(_cegui.MultiColumnList_addColumn,None,MultiColumnList)
MultiColumnList.insertColumn = new_instancemethod(_cegui.MultiColumnList_insertColumn,None,MultiColumnList)
MultiColumnList.removeColumn = new_instancemethod(_cegui.MultiColumnList_removeColumn,None,MultiColumnList)
MultiColumnList.removeColumnWithID = new_instancemethod(_cegui.MultiColumnList_removeColumnWithID,None,MultiColumnList)
MultiColumnList.moveColumn = new_instancemethod(_cegui.MultiColumnList_moveColumn,None,MultiColumnList)
MultiColumnList.moveColumnWithID = new_instancemethod(_cegui.MultiColumnList_moveColumnWithID,None,MultiColumnList)
MultiColumnList.removeRow = new_instancemethod(_cegui.MultiColumnList_removeRow,None,MultiColumnList)
MultiColumnList.setSortColumnByID = new_instancemethod(_cegui.MultiColumnList_setSortColumnByID,None,MultiColumnList)
MultiColumnList.clearAllSelections = new_instancemethod(_cegui.MultiColumnList_clearAllSelections,None,MultiColumnList)
MultiColumnList.setItemSelectState = new_instancemethod(_cegui.MultiColumnList_setItemSelectState,None,MultiColumnList)
MultiColumnList.handleUpdatedItemData = new_instancemethod(_cegui.MultiColumnList_handleUpdatedItemData,None,MultiColumnList)
MultiColumnList.setColumnHeaderWidth = new_instancemethod(_cegui.MultiColumnList_setColumnHeaderWidth,None,MultiColumnList)
MultiColumnList.autoSizeColumnHeader = new_instancemethod(_cegui.MultiColumnList_autoSizeColumnHeader,None,MultiColumnList)
MultiColumnList.setRowID = new_instancemethod(_cegui.MultiColumnList_setRowID,None,MultiColumnList)
MultiColumnList_swigregister = _cegui.MultiColumnList_swigregister
MultiColumnList_swigregister(MultiColumnList)
MultiColumnList.WidgetTypeName = _cegui.cvar.MultiColumnList_WidgetTypeName
MultiColumnList.VertScrollbarNameSuffix = _cegui.cvar.MultiColumnList_VertScrollbarNameSuffix
MultiColumnList.HorzScrollbarNameSuffix = _cegui.cvar.MultiColumnList_HorzScrollbarNameSuffix
MultiColumnList.ListHeaderNameSuffix = _cegui.cvar.MultiColumnList_ListHeaderNameSuffix

class MultiLineEditboxWindowRenderer(WindowRenderer):
    thisown = property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    def __init__(self): raise AttributeError, "No constructor defined"
    __repr__ = _swig_repr
    __swig_destroy__ = _cegui.delete_MultiLineEditboxWindowRenderer
MultiLineEditboxWindowRenderer.getTextRenderArea = new_instancemethod(_cegui.MultiLineEditboxWindowRenderer_getTextRenderArea,None,MultiLineEditboxWindowRenderer)
MultiLineEditboxWindowRenderer_swigregister = _cegui.MultiLineEditboxWindowRenderer_swigregister
MultiLineEditboxWindowRenderer_swigregister(MultiLineEditboxWindowRenderer)

class MultiLineEditbox(Window):
    thisown = property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, *args): 
        _cegui.MultiLineEditbox_swiginit(self,_cegui.new_MultiLineEditbox(*args))
    __swig_destroy__ = _cegui.delete_MultiLineEditbox
    EventNamespace = "MultiLineEditbox" 
    EventCaratMoved = "CaratMoved" 
    EventEditboxFull = "EditboxFullEvent" 
    EventHorzScrollbarModeChanged = "HorzScrollbarModeChanged" 
    EventMaximumTextLengthChanged = "MaximumTextLengthChanged" 
    EventReadOnlyModeChanged = "ReadOnlyChanged" 
    EventTextSelectionChanged = "TextSelectionChanged" 
    EventVertScrollbarModeChanged = "VertScrollbarModeChanged" 
    EventWordWrapModeChanged = "WordWrapModeChanged" 
    SelectionStartIndex = property(_cegui.MultiLineEditbox_SelectionStartIndex_get, _cegui.MultiLineEditbox_SelectionStartIndex_set)
    SelectionEndIndex = property(_cegui.MultiLineEditbox_SelectionEndIndex_get, _cegui.MultiLineEditbox_SelectionEndIndex_set)
    HasInputFocus = property(_cegui.MultiLineEditbox_HasInputFocus_get, None, None,
                    "MultiLineEditbox.HasInputFocus -> bool  (read-only)\n\nThis is equivalent to calling the C++ CEGUI method:\n\tget: bool MultiLineEditbox::hasInputFocus()")

    ReadOnly = property(_cegui.MultiLineEditbox_ReadOnly_get, _cegui.MultiLineEditbox_ReadOnly_set, None,
                    "MultiLineEditbox.ReadOnly -> bool\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: bool MultiLineEditbox::isReadOnly()\n\tset: void MultiLineEditbox::setReadOnly(bool)")


    WordWrapping = property(_cegui.MultiLineEditbox_WordWrapping_get, _cegui.MultiLineEditbox_WordWrapping_set, None,
                    "MultiLineEditbox.WordWrapping -> bool\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: bool MultiLineEditbox::isWordWrapped()\n\tset: void MultiLineEditbox::setWordWrapping(bool)")


    CaratIndex = property(_cegui.MultiLineEditbox_CaratIndex_get, _cegui.MultiLineEditbox_CaratIndex_set, None,
                    "MultiLineEditbox.CaratIndex -> size_t\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: size_t MultiLineEditbox::getCaratIndex()\n\tset: void MultiLineEditbox::setCaratIndex(size_t)")


    SelectionLength = property(_cegui.MultiLineEditbox_SelectionLength_get, None, None,
                    "MultiLineEditbox.SelectionLength -> size_t  (read-only)\n\nThis is equivalent to calling the C++ CEGUI method:\n\tget: size_t MultiLineEditbox::getSelectionLength()")

    MaxTextLength = property(_cegui.MultiLineEditbox_MaxTextLength_get, _cegui.MultiLineEditbox_MaxTextLength_set, None,
                    "MultiLineEditbox.MaxTextLength -> size_t\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: size_t MultiLineEditbox::getMaxTextLength()\n\tset: void MultiLineEditbox::setMaxTextLength(size_t)")


MultiLineEditbox.getVertScrollbar = new_instancemethod(_cegui.MultiLineEditbox_getVertScrollbar,None,MultiLineEditbox)
MultiLineEditbox.getHorzScrollbar = new_instancemethod(_cegui.MultiLineEditbox_getHorzScrollbar,None,MultiLineEditbox)
MultiLineEditbox.getTextRenderArea = new_instancemethod(_cegui.MultiLineEditbox_getTextRenderArea,None,MultiLineEditbox)
MultiLineEditbox.getFormattedLines = new_instancemethod(_cegui.MultiLineEditbox_getFormattedLines,None,MultiLineEditbox)
MultiLineEditbox.getLineNumberFromIndex = new_instancemethod(_cegui.MultiLineEditbox_getLineNumberFromIndex,None,MultiLineEditbox)
MultiLineEditbox.setSelection = new_instancemethod(_cegui.MultiLineEditbox_setSelection,None,MultiLineEditbox)
MultiLineEditbox.ensureCaratIsVisible = new_instancemethod(_cegui.MultiLineEditbox_ensureCaratIsVisible,None,MultiLineEditbox)
MultiLineEditbox.setSelectionBrushImage = new_instancemethod(_cegui.MultiLineEditbox_setSelectionBrushImage,None,MultiLineEditbox)
MultiLineEditbox.getSelectionBrushImage = new_instancemethod(_cegui.MultiLineEditbox_getSelectionBrushImage,None,MultiLineEditbox)
MultiLineEditbox_swigregister = _cegui.MultiLineEditbox_swigregister
MultiLineEditbox_swigregister(MultiLineEditbox)
MultiLineEditbox.WidgetTypeName = _cegui.cvar.MultiLineEditbox_WidgetTypeName
MultiLineEditbox.VertScrollbarNameSuffix = _cegui.cvar.MultiLineEditbox_VertScrollbarNameSuffix
MultiLineEditbox.HorzScrollbarNameSuffix = _cegui.cvar.MultiLineEditbox_HorzScrollbarNameSuffix

class ProgressBar(Window):
    thisown = property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, *args): 
        _cegui.ProgressBar_swiginit(self,_cegui.new_ProgressBar(*args))
    __swig_destroy__ = _cegui.delete_ProgressBar
    EventNamespace = "ProgressBar" 
    EventProgressChanged = "ProgressChanged" 
    EventProgressDone = "ProgressDone" 
    Progress = property(_cegui.ProgressBar_Progress_get, _cegui.ProgressBar_Progress_set, None,
                    "ProgressBar.Progress -> float\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: float ProgressBar::getProgress()\n\tset: void ProgressBar::setProgress(float)")


    Step = property(_cegui.ProgressBar_Step_get, _cegui.ProgressBar_Step_set, None,
                    "ProgressBar.Step -> float\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: float ProgressBar::getStep()\n\tset: void ProgressBar::setStepSize(float)")


ProgressBar.step = new_instancemethod(_cegui.ProgressBar_step,None,ProgressBar)
ProgressBar.adjustProgress = new_instancemethod(_cegui.ProgressBar_adjustProgress,None,ProgressBar)
ProgressBar_swigregister = _cegui.ProgressBar_swigregister
ProgressBar_swigregister(ProgressBar)
ProgressBar.WidgetTypeName = _cegui.cvar.ProgressBar_WidgetTypeName

class PushButton(ButtonBase):
    thisown = property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, *args): 
        _cegui.PushButton_swiginit(self,_cegui.new_PushButton(*args))
    __swig_destroy__ = _cegui.delete_PushButton
    EventNamespace = "PushButton" 
    EventClicked = "Clicked" 
PushButton_swigregister = _cegui.PushButton_swigregister
PushButton_swigregister(PushButton)
PushButton.WidgetTypeName = _cegui.cvar.PushButton_WidgetTypeName

class RadioButton(ButtonBase):
    thisown = property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, *args): 
        _cegui.RadioButton_swiginit(self,_cegui.new_RadioButton(*args))
    __swig_destroy__ = _cegui.delete_RadioButton
    EventNamespace = "RadioButton" 
    EventSelectStateChanged = "SelectStateChanged" 
    Selected = property(_cegui.RadioButton_Selected_get, _cegui.RadioButton_Selected_set, None,
                    "RadioButton.Selected -> bool\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: bool RadioButton::isSelected()\n\tset: void RadioButton::setSelected(bool)")


    GroupID = property(_cegui.RadioButton_GroupID_get, _cegui.RadioButton_GroupID_set, None,
                    "RadioButton.GroupID -> CEGUI::uint\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: CEGUI::uint RadioButton::getGroupID()\n\tset: void RadioButton::setGroupID(CEGUI::uint)")


    SelectedButtonInGroup = property(_cegui.RadioButton_SelectedButtonInGroup_get, None, None,
                    "RadioButton.SelectedButtonInGroup -> CEGUI::RadioButton *  (read-only)\n\nThis is equivalent to calling the C++ CEGUI method:\n\tget: CEGUI::RadioButton * RadioButton::getSelectedButtonInGroup()")

RadioButton_swigregister = _cegui.RadioButton_swigregister
RadioButton_swigregister(RadioButton)
RadioButton.WidgetTypeName = _cegui.cvar.RadioButton_WidgetTypeName

class ScrollbarWindowRenderer(WindowRenderer):
    thisown = property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    def __init__(self): raise AttributeError, "No constructor defined"
    __repr__ = _swig_repr
    __swig_destroy__ = _cegui.delete_ScrollbarWindowRenderer
ScrollbarWindowRenderer.updateThumb = new_instancemethod(_cegui.ScrollbarWindowRenderer_updateThumb,None,ScrollbarWindowRenderer)
ScrollbarWindowRenderer.getValueFromThumb = new_instancemethod(_cegui.ScrollbarWindowRenderer_getValueFromThumb,None,ScrollbarWindowRenderer)
ScrollbarWindowRenderer.getAdjustDirectionFromPoint = new_instancemethod(_cegui.ScrollbarWindowRenderer_getAdjustDirectionFromPoint,None,ScrollbarWindowRenderer)
ScrollbarWindowRenderer_swigregister = _cegui.ScrollbarWindowRenderer_swigregister
ScrollbarWindowRenderer_swigregister(ScrollbarWindowRenderer)

class Scrollbar(Window):
    thisown = property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, *args): 
        _cegui.Scrollbar_swiginit(self,_cegui.new_Scrollbar(*args))
    __swig_destroy__ = _cegui.delete_Scrollbar
    EventNamespace = "Scrollbar" 
    EventScrollConfigChanged = "ScrollConfigChanged" 
    EventScrollPositionChanged = "ScrollPosChanged" 
    EventThumbTrackEnded = "ThumbTrackEnded" 
    EventThumbTrackStarted = "ThumbTrackStarted" 
    DocumentSize = property(_cegui.Scrollbar_DocumentSize_get, _cegui.Scrollbar_DocumentSize_set, None,
                    "Scrollbar.DocumentSize -> float\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: float Scrollbar::getDocumentSize()\n\tset: void Scrollbar::setDocumentSize(float)")


    PageSize = property(_cegui.Scrollbar_PageSize_get, _cegui.Scrollbar_PageSize_set, None,
                    "Scrollbar.PageSize -> float\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: float Scrollbar::getPageSize()\n\tset: void Scrollbar::setPageSize(float)")


    StepSize = property(_cegui.Scrollbar_StepSize_get, _cegui.Scrollbar_StepSize_set, None,
                    "Scrollbar.StepSize -> float\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: float Scrollbar::getStepSize()\n\tset: void Scrollbar::setStepSize(float)")


    OverlapSize = property(_cegui.Scrollbar_OverlapSize_get, _cegui.Scrollbar_OverlapSize_set, None,
                    "Scrollbar.OverlapSize -> float\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: float Scrollbar::getOverlapSize()\n\tset: void Scrollbar::setOverlapSize(float)")


    ScrollPosition = property(_cegui.Scrollbar_ScrollPosition_get, _cegui.Scrollbar_ScrollPosition_set, None,
                    "Scrollbar.ScrollPosition -> float\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: float Scrollbar::getScrollPosition()\n\tset: void Scrollbar::setScrollPosition(float)")


    IncreaseButton = property(_cegui.Scrollbar_IncreaseButton_get, None, None,
                    "Scrollbar.IncreaseButton -> CEGUI::PushButton *  (read-only)\n\nThis is equivalent to calling the C++ CEGUI method:\n\tget: CEGUI::PushButton * Scrollbar::getIncreaseButton()")

    DecreaseButton = property(_cegui.Scrollbar_DecreaseButton_get, None, None,
                    "Scrollbar.DecreaseButton -> CEGUI::PushButton *  (read-only)\n\nThis is equivalent to calling the C++ CEGUI method:\n\tget: CEGUI::PushButton * Scrollbar::getDecreaseButton()")

    Thumb = property(_cegui.Scrollbar_Thumb_get, None, None,
                    "Scrollbar.Thumb -> CEGUI::Thumb *  (read-only)\n\nThis is equivalent to calling the C++ CEGUI method:\n\tget: CEGUI::Thumb * Scrollbar::getThumb()")

Scrollbar_swigregister = _cegui.Scrollbar_swigregister
Scrollbar_swigregister(Scrollbar)
Scrollbar.WidgetTypeName = _cegui.cvar.Scrollbar_WidgetTypeName
Scrollbar.ThumbNameSuffix = _cegui.cvar.Scrollbar_ThumbNameSuffix
Scrollbar.IncreaseButtonNameSuffix = _cegui.cvar.Scrollbar_IncreaseButtonNameSuffix
Scrollbar.DecreaseButtonNameSuffix = _cegui.cvar.Scrollbar_DecreaseButtonNameSuffix

class ScrolledItemListBase(ItemListBase):
    thisown = property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    def __init__(self): raise AttributeError, "No constructor defined"
    __repr__ = _swig_repr
    __swig_destroy__ = _cegui.delete_ScrolledItemListBase
    EventNamespace = "ScrolledItemListBase" 
    EventHorzScrollbarModeChanged = "HorzScrollbarModeChanged" 
    EventListContentsChanged = "ListItemsChanged" 
    EventSortEnabledChanged = "SortEnabledChanged" 
    EventSortModeChanged = "SortModeChanged" 
    EventVertScrollbarModeChanged = "VertScrollbarModeChanged" 
    ShowVertScrollbar = property(_cegui.ScrolledItemListBase_ShowVertScrollbar_get, _cegui.ScrolledItemListBase_ShowVertScrollbar_set, None,
                    "ScrolledItemListBase.ShowVertScrollbar -> bool\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: bool ScrolledItemListBase::isVertScrollbarAlwaysShown()\n\tset: void ScrolledItemListBase::setShowVertScrollbar(bool)")


    ShowHorzScrollbar = property(_cegui.ScrolledItemListBase_ShowHorzScrollbar_get, _cegui.ScrolledItemListBase_ShowHorzScrollbar_set, None,
                    "ScrolledItemListBase.ShowHorzScrollbar -> bool\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: bool ScrolledItemListBase::isHorzScrollbarAlwaysShown()\n\tset: void ScrolledItemListBase::setShowHorzScrollbar(bool)")


    VertScrollbar = property(_cegui.ScrolledItemListBase_VertScrollbar_get, None, None,
                    "ScrolledItemListBase.VertScrollbar -> CEGUI::Scrollbar *  (read-only)\n\nThis is equivalent to calling the C++ CEGUI method:\n\tget: CEGUI::Scrollbar * ScrolledItemListBase::getVertScrollbar()")

    HorzScrollbar = property(_cegui.ScrolledItemListBase_HorzScrollbar_get, None, None,
                    "ScrolledItemListBase.HorzScrollbar -> CEGUI::Scrollbar *  (read-only)\n\nThis is equivalent to calling the C++ CEGUI method:\n\tget: CEGUI::Scrollbar * ScrolledItemListBase::getHorzScrollbar()")

ScrolledItemListBase_swigregister = _cegui.ScrolledItemListBase_swigregister
ScrolledItemListBase_swigregister(ScrolledItemListBase)
ScrolledItemListBase.VertScrollbarNameSuffix = _cegui.cvar.ScrolledItemListBase_VertScrollbarNameSuffix
ScrolledItemListBase.HorzScrollbarNameSuffix = _cegui.cvar.ScrolledItemListBase_HorzScrollbarNameSuffix
ScrolledItemListBase.ContentPaneNameSuffix = _cegui.cvar.ScrolledItemListBase_ContentPaneNameSuffix

class ItemListbox(ScrolledItemListBase):
    thisown = property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, *args): 
        _cegui.ItemListbox_swiginit(self,_cegui.new_ItemListbox(*args))
    __swig_destroy__ = _cegui.delete_ItemListbox
    EventNamespace = "ItemListbox" 
    EventHorzScrollbarModeChanged = "HorzScrollbarModeChanged" 
    EventListContentsChanged = "ListItemsChanged" 
    EventMultiSelectModeChanged = "MultiSelectModeChanged" 
    EventSelectionChanged = "SelectionChanged" 
    EventSortEnabledChanged = "SortEnabledChanged" 
    EventSortModeChanged = "SortModeChanged" 
    EventVertScrollbarModeChanged = "VertScrollbarModeChanged" 
    SelectedCount = property(_cegui.ItemListbox_SelectedCount_get, None, None,
                    "ItemListbox.SelectedCount -> size_t  (read-only)\n\nThis is equivalent to calling the C++ CEGUI method:\n\tget: size_t ItemListbox::getSelectedCount()")

ItemListbox.getLastSelectedItem = new_instancemethod(_cegui.ItemListbox_getLastSelectedItem,None,ItemListbox)
ItemListbox.getFirstSelectedItem = new_instancemethod(_cegui.ItemListbox_getFirstSelectedItem,None,ItemListbox)
ItemListbox.getNextSelectedItem = new_instancemethod(_cegui.ItemListbox_getNextSelectedItem,None,ItemListbox)
ItemListbox.getNextSelectedItemAfter = new_instancemethod(_cegui.ItemListbox_getNextSelectedItemAfter,None,ItemListbox)
ItemListbox.isMultiSelectEnabled = new_instancemethod(_cegui.ItemListbox_isMultiSelectEnabled,None,ItemListbox)
ItemListbox.isItemSelected = new_instancemethod(_cegui.ItemListbox_isItemSelected,None,ItemListbox)
ItemListbox.setMultiSelectEnabled = new_instancemethod(_cegui.ItemListbox_setMultiSelectEnabled,None,ItemListbox)
ItemListbox.clearAllSelections = new_instancemethod(_cegui.ItemListbox_clearAllSelections,None,ItemListbox)
ItemListbox.selectRange = new_instancemethod(_cegui.ItemListbox_selectRange,None,ItemListbox)
ItemListbox.selectAllItems = new_instancemethod(_cegui.ItemListbox_selectAllItems,None,ItemListbox)
ItemListbox.layoutItemWidgets = new_instancemethod(_cegui.ItemListbox_layoutItemWidgets,None,ItemListbox)
ItemListbox.getContentSize = new_instancemethod(_cegui.ItemListbox_getContentSize,None,ItemListbox)
ItemListbox.testClassName_impl = new_instancemethod(_cegui.ItemListbox_testClassName_impl,None,ItemListbox)
ItemListbox_swigregister = _cegui.ItemListbox_swigregister
ItemListbox_swigregister(ItemListbox)
ItemListbox.WidgetTypeName = _cegui.cvar.ItemListbox_WidgetTypeName

class SliderWindowRenderer(WindowRenderer):
    thisown = property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    def __init__(self): raise AttributeError, "No constructor defined"
    __repr__ = _swig_repr
    __swig_destroy__ = _cegui.delete_SliderWindowRenderer
SliderWindowRenderer.updateThumb = new_instancemethod(_cegui.SliderWindowRenderer_updateThumb,None,SliderWindowRenderer)
SliderWindowRenderer.getValueFromThumb = new_instancemethod(_cegui.SliderWindowRenderer_getValueFromThumb,None,SliderWindowRenderer)
SliderWindowRenderer.getAdjustDirectionFromPoint = new_instancemethod(_cegui.SliderWindowRenderer_getAdjustDirectionFromPoint,None,SliderWindowRenderer)
SliderWindowRenderer_swigregister = _cegui.SliderWindowRenderer_swigregister
SliderWindowRenderer_swigregister(SliderWindowRenderer)

class Slider(Window):
    thisown = property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, *args): 
        _cegui.Slider_swiginit(self,_cegui.new_Slider(*args))
    __swig_destroy__ = _cegui.delete_Slider
    EventNamespace = "Slider" 
    EventThumbTrackEnded = "ThumbTrackEnded" 
    EventThumbTrackStarted = "ThumbTrackStarted" 
    EventValueChanged = "ValueChanged" 
    CurrentValue = property(_cegui.Slider_CurrentValue_get, _cegui.Slider_CurrentValue_set, None,
                    "Slider.CurrentValue -> float\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: float Slider::getCurrentValue()\n\tset: void Slider::setCurrentValue(float)")


    MaxValue = property(_cegui.Slider_MaxValue_get, _cegui.Slider_MaxValue_set, None,
                    "Slider.MaxValue -> float\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: float Slider::getMaxValue()\n\tset: void Slider::setMaxValue(float)")


    ClickStep = property(_cegui.Slider_ClickStep_get, _cegui.Slider_ClickStep_set, None,
                    "Slider.ClickStep -> float\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: float Slider::getClickStep()\n\tset: void Slider::setClickStep(float)")


    Thumb = property(_cegui.Slider_Thumb_get, None, None,
                    "Slider.Thumb -> CEGUI::Thumb *  (read-only)\n\nThis is equivalent to calling the C++ CEGUI method:\n\tget: CEGUI::Thumb * Slider::getThumb()")

Slider_swigregister = _cegui.Slider_swigregister
Slider_swigregister(Slider)
Slider.WidgetTypeName = _cegui.cvar.Slider_WidgetTypeName
Slider.ThumbNameSuffix = _cegui.cvar.Slider_ThumbNameSuffix

class TabButton(ButtonBase):
    thisown = property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, *args): 
        _cegui.TabButton_swiginit(self,_cegui.new_TabButton(*args))
    __swig_destroy__ = _cegui.delete_TabButton
    EventNamespace = "TabButton" 
    EventClicked = "Clicked" 
    Selected = property(_cegui.TabButton_Selected_get, _cegui.TabButton_Selected_set, None,
                    "TabButton.Selected -> bool\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: bool TabButton::isSelected()\n\tset: void TabButton::setSelected(bool)")


    TargetWindow = property(_cegui.TabButton_TargetWindow_get, _cegui.TabButton_TargetWindow_set, None,
                    "TabButton.TargetWindow -> CEGUI::Window *\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: CEGUI::Window * TabButton::getTargetWindow()\n\tset: void TabButton::setTargetWindow(CEGUI::Window *)")


TabButton_swigregister = _cegui.TabButton_swigregister
TabButton_swigregister(TabButton)
TabButton.WidgetTypeName = _cegui.cvar.TabButton_WidgetTypeName
TabButton.EventDragged = _cegui.cvar.TabButton_EventDragged
TabButton.EventScrolled = _cegui.cvar.TabButton_EventScrolled

class TabControlWindowRenderer(WindowRenderer):
    thisown = property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    def __init__(self): raise AttributeError, "No constructor defined"
    __repr__ = _swig_repr
    __swig_destroy__ = _cegui.delete_TabControlWindowRenderer
TabControlWindowRenderer.createTabButton = new_instancemethod(_cegui.TabControlWindowRenderer_createTabButton,None,TabControlWindowRenderer)
TabControlWindowRenderer_swigregister = _cegui.TabControlWindowRenderer_swigregister
TabControlWindowRenderer_swigregister(TabControlWindowRenderer)

class TabControl(Window):
    thisown = property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    Top = _cegui.TabControl_Top
    Bottom = _cegui.TabControl_Bottom
    def __init__(self, *args): 
        _cegui.TabControl_swiginit(self,_cegui.new_TabControl(*args))
    __swig_destroy__ = _cegui.delete_TabControl
    EventNamespace = "TabControl" 
    EventSelectionChanged = "TabSelectionChanged" 
    TabCount = property(_cegui.TabControl_TabCount_get, None, None,
                    "TabControl.TabCount -> size_t  (read-only)\n\nThis is equivalent to calling the C++ CEGUI method:\n\tget: size_t TabControl::getTabCount()")

    TabTextPadding = property(_cegui.TabControl_TabTextPadding_get, _cegui.TabControl_TabTextPadding_set, None,
                    "TabControl.TabTextPadding -> const CEGUI::UDim &\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: const CEGUI::UDim & TabControl::getTabTextPadding()\n\tset: void TabControl::setTabTextPadding(const CEGUI::UDim &)")


    TabHeight = property(_cegui.TabControl_TabHeight_get, _cegui.TabControl_TabHeight_set, None,
                    "TabControl.TabHeight -> const CEGUI::UDim &\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: const CEGUI::UDim & TabControl::getTabHeight()\n\tset: void TabControl::setTabHeight(const CEGUI::UDim &)")


    TabPanePosition = property(_cegui.TabControl_TabPanePosition_get, _cegui.TabControl_TabPanePosition_set, None,
                    "TabControl.TabPanePosition -> TabControl::TabPanePosition\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: TabControl::TabPanePosition TabControl::getTabPanePosition()\n\tset: void TabControl::setTabPanePosition(TabControl::TabPanePosition)")


    SelectedTabIndex = property(_cegui.TabControl_SelectedTabIndex_get, _cegui.TabControl_SelectedTabIndex_set, None,
                    "TabControl.SelectedTabIndex -> size_t\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: size_t TabControl::getSelectedTabIndex()\n\tset: void TabControl::setSelectedTabAtIndex(size_t)")


TabControl.setSelectedTab = new_instancemethod(_cegui.TabControl_setSelectedTab,None,TabControl)
TabControl.makeTabVisible = new_instancemethod(_cegui.TabControl_makeTabVisible,None,TabControl)
TabControl.makeTabVisibleAtIndex = new_instancemethod(_cegui.TabControl_makeTabVisibleAtIndex,None,TabControl)
TabControl.getTabContentsAtIndex = new_instancemethod(_cegui.TabControl_getTabContentsAtIndex,None,TabControl)
TabControl.getTabContents = new_instancemethod(_cegui.TabControl_getTabContents,None,TabControl)
TabControl.isTabContentsSelected = new_instancemethod(_cegui.TabControl_isTabContentsSelected,None,TabControl)
TabControl.addTab = new_instancemethod(_cegui.TabControl_addTab,None,TabControl)
TabControl.removeTab = new_instancemethod(_cegui.TabControl_removeTab,None,TabControl)
TabControl_swigregister = _cegui.TabControl_swigregister
TabControl_swigregister(TabControl)
TabControl.WidgetTypeName = _cegui.cvar.TabControl_WidgetTypeName
TabControl.ContentPaneNameSuffix = _cegui.cvar.TabControl_ContentPaneNameSuffix
TabControl.TabButtonNameSuffix = _cegui.cvar.TabControl_TabButtonNameSuffix
TabControl.TabButtonPaneNameSuffix = _cegui.cvar.TabControl_TabButtonPaneNameSuffix
TabControl.ButtonScrollLeftSuffix = _cegui.cvar.TabControl_ButtonScrollLeftSuffix
TabControl.ButtonScrollRightSuffix = _cegui.cvar.TabControl_ButtonScrollRightSuffix

class Thumb(PushButton):
    thisown = property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr


    VerticalRange = property(_cegui.Thumb___getVertRange, fix_args(_cegui.Thumb___setVertRange), None,
                    "Thumb.VerticalRange -> type\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: Thumb::getVertRange()\n\tset: void Thumb::setVertRange(...)")

    HorizontalRange = property(_cegui.Thumb___getHorzRange, fix_args(_cegui.Thumb___setHorzRange), None,
                    "Thumb.HorizontalRange -> type\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: Thumb::getHorzRange()\n\tset: void Thumb::setHorzRange(...)")

    def __init__(self, *args): 
        _cegui.Thumb_swiginit(self,_cegui.new_Thumb(*args))
    __swig_destroy__ = _cegui.delete_Thumb
    EventNamespace = "Thumb" 
    EventClicked = "Clicked" 
    EventThumbPositionChanged = "ThumbPosChanged" 
    EventThumbTrackEnded = "ThumbTrackEnded" 
    EventThumbTrackStarted = "ThumbTrackStarted" 
    HotTracked = property(_cegui.Thumb_HotTracked_get, _cegui.Thumb_HotTracked_set, None,
                    "Thumb.HotTracked -> bool\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: bool Thumb::isHotTracked()\n\tset: void Thumb::setHotTracked(bool)")


    VerticalFree = property(_cegui.Thumb_VerticalFree_get, _cegui.Thumb_VerticalFree_set, None,
                    "Thumb.VerticalFree -> bool\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: bool Thumb::isVertFree()\n\tset: void Thumb::setVertFree(bool)")


    HorizontalFree = property(_cegui.Thumb_HorizontalFree_get, _cegui.Thumb_HorizontalFree_set, None,
                    "Thumb.HorizontalFree -> bool\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: bool Thumb::isHorzFree()\n\tset: void Thumb::setHorzFree(bool)")


Thumb_swigregister = _cegui.Thumb_swigregister
Thumb_swigregister(Thumb)
Thumb.WidgetTypeName = _cegui.cvar.Thumb_WidgetTypeName

class Titlebar(Window):
    thisown = property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, *args): 
        _cegui.Titlebar_swiginit(self,_cegui.new_Titlebar(*args))
    __swig_destroy__ = _cegui.delete_Titlebar
    DraggingEnabled = property(_cegui.Titlebar_DraggingEnabled_get, _cegui.Titlebar_DraggingEnabled_set, None,
                    "Titlebar.DraggingEnabled -> bool\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: bool Titlebar::isDraggingEnabled()\n\tset: void Titlebar::setDraggingEnabled(bool)")


Titlebar_swigregister = _cegui.Titlebar_swigregister
Titlebar_swigregister(Titlebar)
Titlebar.WidgetTypeName = _cegui.cvar.Titlebar_WidgetTypeName

class Spinner(Window):
    thisown = property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    FloatingPoint = _cegui.Spinner_FloatingPoint
    Integer = _cegui.Spinner_Integer
    Hexadecimal = _cegui.Spinner_Hexadecimal
    Octal = _cegui.Spinner_Octal
    def __init__(self, *args): 
        _cegui.Spinner_swiginit(self,_cegui.new_Spinner(*args))
    __swig_destroy__ = _cegui.delete_Spinner
    EventNamespace = "Spinner" 
    EventMaximumValueChanged = "MaximumValueChanged" 
    EventMinimumValueChanged = "MinimumValueChanged" 
    EventStepChanged = "StepChanged" 
    EventTextInputModeChanged = "TextInputModeChanged" 
    EventValueChanged = "ValueChanged" 
    CurrentValue = property(_cegui.Spinner_CurrentValue_get, _cegui.Spinner_CurrentValue_set, None,
                    "Spinner.CurrentValue -> float\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: float Spinner::getCurrentValue()\n\tset: void Spinner::setCurrentValue(float)")


    StepSize = property(_cegui.Spinner_StepSize_get, _cegui.Spinner_StepSize_set, None,
                    "Spinner.StepSize -> float\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: float Spinner::getStepSize()\n\tset: void Spinner::setStepSize(float)")


    MaxValue = property(_cegui.Spinner_MaxValue_get, _cegui.Spinner_MaxValue_set, None,
                    "Spinner.MaxValue -> float\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: float Spinner::getMaximumValue()\n\tset: void Spinner::setMaximumValue(float)")


    MinValue = property(_cegui.Spinner_MinValue_get, _cegui.Spinner_MinValue_set, None,
                    "Spinner.MinValue -> float\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: float Spinner::getMinimumValue()\n\tset: void Spinner::setMinimumValue(float)")


    TextInputMode = property(_cegui.Spinner_TextInputMode_get, _cegui.Spinner_TextInputMode_set, None,
                    "Spinner.TextInputMode -> CEGUI::Spinner::TextInputMode\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: CEGUI::Spinner::TextInputMode Spinner::getTextInputMode()\n\tset: void Spinner::setTextInputMode(CEGUI::Spinner::TextInputMode)")


Spinner_swigregister = _cegui.Spinner_swigregister
Spinner_swigregister(Spinner)
Spinner.WidgetTypeName = _cegui.cvar.Spinner_WidgetTypeName
Spinner.EditboxNameSuffix = _cegui.cvar.Spinner_EditboxNameSuffix
Spinner.IncreaseButtonNameSuffix = _cegui.cvar.Spinner_IncreaseButtonNameSuffix
Spinner.DecreaseButtonNameSuffix = _cegui.cvar.Spinner_DecreaseButtonNameSuffix

class ClippedContainer(Window):
    thisown = property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, *args): 
        _cegui.ClippedContainer_swiginit(self,_cegui.new_ClippedContainer(*args))
    __swig_destroy__ = _cegui.delete_ClippedContainer
    ClipArea = property(_cegui.ClippedContainer_ClipArea_get, _cegui.ClippedContainer_ClipArea_set, None,
                    "ClippedContainer.ClipArea -> const CEGUI::Rect &\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: const CEGUI::Rect & ClippedContainer::getClipArea()\n\tset: void ClippedContainer::setClipArea(const CEGUI::Rect &)")


    ClipperWindow = property(_cegui.ClippedContainer_ClipperWindow_get, _cegui.ClippedContainer_ClipperWindow_set, None,
                    "ClippedContainer.ClipperWindow -> CEGUI::Window *\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: CEGUI::Window * ClippedContainer::getClipperWindow()\n\tset: void ClippedContainer::setClipperWindow(CEGUI::Window *)")


ClippedContainer_swigregister = _cegui.ClippedContainer_swigregister
ClippedContainer_swigregister(ClippedContainer)
ClippedContainer.WidgetTypeName = _cegui.cvar.ClippedContainer_WidgetTypeName

class EventArgs(object):
    thisown = property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, *args): 
        _cegui.EventArgs_swiginit(self,_cegui.new_EventArgs(*args))
    __swig_destroy__ = _cegui.delete_EventArgs
    Handled = property(_cegui.EventArgs_Handled_get, _cegui.EventArgs_Handled_set)
EventArgs_swigregister = _cegui.EventArgs_swigregister
EventArgs_swigregister(EventArgs)

class Key(object):
    thisown = property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    Escape = _cegui.Key_Escape
    One = _cegui.Key_One
    Two = _cegui.Key_Two
    Three = _cegui.Key_Three
    Four = _cegui.Key_Four
    Five = _cegui.Key_Five
    Six = _cegui.Key_Six
    Seven = _cegui.Key_Seven
    Eight = _cegui.Key_Eight
    Nine = _cegui.Key_Nine
    Zero = _cegui.Key_Zero
    Minus = _cegui.Key_Minus
    Equals = _cegui.Key_Equals
    Backspace = _cegui.Key_Backspace
    Tab = _cegui.Key_Tab
    Q = _cegui.Key_Q
    W = _cegui.Key_W
    E = _cegui.Key_E
    R = _cegui.Key_R
    T = _cegui.Key_T
    Y = _cegui.Key_Y
    U = _cegui.Key_U
    I = _cegui.Key_I
    O = _cegui.Key_O
    P = _cegui.Key_P
    LeftBracket = _cegui.Key_LeftBracket
    RightBracket = _cegui.Key_RightBracket
    Return = _cegui.Key_Return
    LeftControl = _cegui.Key_LeftControl
    A = _cegui.Key_A
    S = _cegui.Key_S
    D = _cegui.Key_D
    F = _cegui.Key_F
    G = _cegui.Key_G
    H = _cegui.Key_H
    J = _cegui.Key_J
    K = _cegui.Key_K
    L = _cegui.Key_L
    Semicolon = _cegui.Key_Semicolon
    Apostrophe = _cegui.Key_Apostrophe
    Grave = _cegui.Key_Grave
    LeftShift = _cegui.Key_LeftShift
    Backslash = _cegui.Key_Backslash
    Z = _cegui.Key_Z
    X = _cegui.Key_X
    C = _cegui.Key_C
    V = _cegui.Key_V
    B = _cegui.Key_B
    N = _cegui.Key_N
    M = _cegui.Key_M
    Comma = _cegui.Key_Comma
    Period = _cegui.Key_Period
    Slash = _cegui.Key_Slash
    RightShift = _cegui.Key_RightShift
    Multiply = _cegui.Key_Multiply
    LeftAlt = _cegui.Key_LeftAlt
    Space = _cegui.Key_Space
    Capital = _cegui.Key_Capital
    F1 = _cegui.Key_F1
    F2 = _cegui.Key_F2
    F3 = _cegui.Key_F3
    F4 = _cegui.Key_F4
    F5 = _cegui.Key_F5
    F6 = _cegui.Key_F6
    F7 = _cegui.Key_F7
    F8 = _cegui.Key_F8
    F9 = _cegui.Key_F9
    F10 = _cegui.Key_F10
    NumLock = _cegui.Key_NumLock
    ScrollLock = _cegui.Key_ScrollLock
    Numpad7 = _cegui.Key_Numpad7
    Numpad8 = _cegui.Key_Numpad8
    Numpad9 = _cegui.Key_Numpad9
    Subtract = _cegui.Key_Subtract
    Numpad4 = _cegui.Key_Numpad4
    Numpad5 = _cegui.Key_Numpad5
    Numpad6 = _cegui.Key_Numpad6
    Add = _cegui.Key_Add
    Numpad1 = _cegui.Key_Numpad1
    Numpad2 = _cegui.Key_Numpad2
    Numpad3 = _cegui.Key_Numpad3
    Numpad0 = _cegui.Key_Numpad0
    Decimal = _cegui.Key_Decimal
    OEM_102 = _cegui.Key_OEM_102
    F11 = _cegui.Key_F11
    F12 = _cegui.Key_F12
    F13 = _cegui.Key_F13
    F14 = _cegui.Key_F14
    F15 = _cegui.Key_F15
    Kana = _cegui.Key_Kana
    ABNT_C1 = _cegui.Key_ABNT_C1
    Convert = _cegui.Key_Convert
    NoConvert = _cegui.Key_NoConvert
    Yen = _cegui.Key_Yen
    ABNT_C2 = _cegui.Key_ABNT_C2
    NumpadEquals = _cegui.Key_NumpadEquals
    PrevTrack = _cegui.Key_PrevTrack
    At = _cegui.Key_At
    Colon = _cegui.Key_Colon
    Underline = _cegui.Key_Underline
    Kanji = _cegui.Key_Kanji
    Stop = _cegui.Key_Stop
    AX = _cegui.Key_AX
    Unlabeled = _cegui.Key_Unlabeled
    NextTrack = _cegui.Key_NextTrack
    NumpadEnter = _cegui.Key_NumpadEnter
    RightControl = _cegui.Key_RightControl
    Mute = _cegui.Key_Mute
    Calculator = _cegui.Key_Calculator
    PlayPause = _cegui.Key_PlayPause
    MediaStop = _cegui.Key_MediaStop
    VolumeDown = _cegui.Key_VolumeDown
    VolumeUp = _cegui.Key_VolumeUp
    WebHome = _cegui.Key_WebHome
    NumpadComma = _cegui.Key_NumpadComma
    Divide = _cegui.Key_Divide
    SysRq = _cegui.Key_SysRq
    RightAlt = _cegui.Key_RightAlt
    Pause = _cegui.Key_Pause
    Home = _cegui.Key_Home
    ArrowUp = _cegui.Key_ArrowUp
    PageUp = _cegui.Key_PageUp
    ArrowLeft = _cegui.Key_ArrowLeft
    ArrowRight = _cegui.Key_ArrowRight
    End = _cegui.Key_End
    ArrowDown = _cegui.Key_ArrowDown
    PageDown = _cegui.Key_PageDown
    Insert = _cegui.Key_Insert
    Delete = _cegui.Key_Delete
    LeftWindows = _cegui.Key_LeftWindows
    RightWindow = _cegui.Key_RightWindow
    RightWindows = _cegui.Key_RightWindows
    AppMenu = _cegui.Key_AppMenu
    Power = _cegui.Key_Power
    Sleep = _cegui.Key_Sleep
    Wake = _cegui.Key_Wake
    WebSearch = _cegui.Key_WebSearch
    WebFavorites = _cegui.Key_WebFavorites
    WebRefresh = _cegui.Key_WebRefresh
    WebStop = _cegui.Key_WebStop
    WebForward = _cegui.Key_WebForward
    WebBack = _cegui.Key_WebBack
    MyComputer = _cegui.Key_MyComputer
    Mail = _cegui.Key_Mail
    MediaSelect = _cegui.Key_MediaSelect
    def __init__(self, *args): 
        _cegui.Key_swiginit(self,_cegui.new_Key(*args))
    __swig_destroy__ = _cegui.delete_Key
Key_swigregister = _cegui.Key_swigregister
Key_swigregister(Key)

LeftButton = _cegui.LeftButton
RightButton = _cegui.RightButton
MiddleButton = _cegui.MiddleButton
X1Button = _cegui.X1Button
X2Button = _cegui.X2Button
MouseButtonCount = _cegui.MouseButtonCount
NoButton = _cegui.NoButton
LeftMouse = _cegui.LeftMouse
RightMouse = _cegui.RightMouse
Shift = _cegui.Shift
Control = _cegui.Control
MiddleMouse = _cegui.MiddleMouse
X1Mouse = _cegui.X1Mouse
X2Mouse = _cegui.X2Mouse
Alt = _cegui.Alt
class WindowEventArgs(EventArgs):
    thisown = property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, *args): 
        _cegui.WindowEventArgs_swiginit(self,_cegui.new_WindowEventArgs(*args))
    Window = property(_cegui.WindowEventArgs_Window_get, _cegui.WindowEventArgs_Window_set)
    __swig_destroy__ = _cegui.delete_WindowEventArgs
WindowEventArgs_swigregister = _cegui.WindowEventArgs_swigregister
WindowEventArgs_swigregister(WindowEventArgs)

class MouseEventArgs(WindowEventArgs):
    thisown = property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, *args): 
        _cegui.MouseEventArgs_swiginit(self,_cegui.new_MouseEventArgs(*args))
    Position = property(_cegui.MouseEventArgs_Position_get, _cegui.MouseEventArgs_Position_set)
    MoveDelta = property(_cegui.MouseEventArgs_MoveDelta_get, _cegui.MouseEventArgs_MoveDelta_set)
    Button = property(_cegui.MouseEventArgs_Button_get, _cegui.MouseEventArgs_Button_set)
    SysKeys = property(_cegui.MouseEventArgs_SysKeys_get, _cegui.MouseEventArgs_SysKeys_set)
    WheelChange = property(_cegui.MouseEventArgs_WheelChange_get, _cegui.MouseEventArgs_WheelChange_set)
    ClickCount = property(_cegui.MouseEventArgs_ClickCount_get, _cegui.MouseEventArgs_ClickCount_set)
    __swig_destroy__ = _cegui.delete_MouseEventArgs
MouseEventArgs_swigregister = _cegui.MouseEventArgs_swigregister
MouseEventArgs_swigregister(MouseEventArgs)

class MouseCursorEventArgs(EventArgs):
    thisown = property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, *args): 
        _cegui.MouseCursorEventArgs_swiginit(self,_cegui.new_MouseCursorEventArgs(*args))
    MouseCursor = property(_cegui.MouseCursorEventArgs_MouseCursor_get, _cegui.MouseCursorEventArgs_MouseCursor_set)
    Image = property(_cegui.MouseCursorEventArgs_Image_get, _cegui.MouseCursorEventArgs_Image_set)
    __swig_destroy__ = _cegui.delete_MouseCursorEventArgs
MouseCursorEventArgs_swigregister = _cegui.MouseCursorEventArgs_swigregister
MouseCursorEventArgs_swigregister(MouseCursorEventArgs)

class KeyEventArgs(WindowEventArgs):
    thisown = property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, *args): 
        _cegui.KeyEventArgs_swiginit(self,_cegui.new_KeyEventArgs(*args))
    Codepoint = property(_cegui.KeyEventArgs_Codepoint_get, _cegui.KeyEventArgs_Codepoint_set)
    Scancode = property(_cegui.KeyEventArgs_Scancode_get, _cegui.KeyEventArgs_Scancode_set)
    SysKeys = property(_cegui.KeyEventArgs_SysKeys_get, _cegui.KeyEventArgs_SysKeys_set)
    __swig_destroy__ = _cegui.delete_KeyEventArgs
KeyEventArgs_swigregister = _cegui.KeyEventArgs_swigregister
KeyEventArgs_swigregister(KeyEventArgs)

class ActivationEventArgs(WindowEventArgs):
    thisown = property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, *args): 
        _cegui.ActivationEventArgs_swiginit(self,_cegui.new_ActivationEventArgs(*args))
    OtherWindow = property(_cegui.ActivationEventArgs_OtherWindow_get, _cegui.ActivationEventArgs_OtherWindow_set)
    __swig_destroy__ = _cegui.delete_ActivationEventArgs
ActivationEventArgs_swigregister = _cegui.ActivationEventArgs_swigregister
ActivationEventArgs_swigregister(ActivationEventArgs)

class DragDropEventArgs(WindowEventArgs):
    thisown = property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, *args): 
        _cegui.DragDropEventArgs_swiginit(self,_cegui.new_DragDropEventArgs(*args))
    DragDropItem = property(_cegui.DragDropEventArgs_DragDropItem_get, _cegui.DragDropEventArgs_DragDropItem_set)
    __swig_destroy__ = _cegui.delete_DragDropEventArgs
DragDropEventArgs_swigregister = _cegui.DragDropEventArgs_swigregister
DragDropEventArgs_swigregister(DragDropEventArgs)

class HeaderSequenceEventArgs(WindowEventArgs):
    thisown = property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, *args): 
        _cegui.HeaderSequenceEventArgs_swiginit(self,_cegui.new_HeaderSequenceEventArgs(*args))
    OldIndex = property(_cegui.HeaderSequenceEventArgs_OldIndex_get, _cegui.HeaderSequenceEventArgs_OldIndex_set)
    NewIndex = property(_cegui.HeaderSequenceEventArgs_NewIndex_get, _cegui.HeaderSequenceEventArgs_NewIndex_set)
    __swig_destroy__ = _cegui.delete_HeaderSequenceEventArgs
HeaderSequenceEventArgs_swigregister = _cegui.HeaderSequenceEventArgs_swigregister
HeaderSequenceEventArgs_swigregister(HeaderSequenceEventArgs)

class ListHeaderWindowRenderer(WindowRenderer):
    thisown = property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    def __init__(self): raise AttributeError, "No constructor defined"
    __repr__ = _swig_repr
    __swig_destroy__ = _cegui.delete_ListHeaderWindowRenderer
ListHeaderWindowRenderer.createNewSegment = new_instancemethod(_cegui.ListHeaderWindowRenderer_createNewSegment,None,ListHeaderWindowRenderer)
ListHeaderWindowRenderer.destroyListSegment = new_instancemethod(_cegui.ListHeaderWindowRenderer_destroyListSegment,None,ListHeaderWindowRenderer)
ListHeaderWindowRenderer_swigregister = _cegui.ListHeaderWindowRenderer_swigregister
ListHeaderWindowRenderer_swigregister(ListHeaderWindowRenderer)

class ListHeader(Window):
    thisown = property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, *args): 
        _cegui.ListHeader_swiginit(self,_cegui.new_ListHeader(*args))
    __swig_destroy__ = _cegui.delete_ListHeader
    EventNamespace = "ListHeader" 
    EventDragMoveSettingChanged = "DragMoveSettingChanged" 
    EventDragSizeSettingChanged = "DragSizeSettingChanged" 
    EventSegmentAdded = "SegmentAdded" 
    EventSegmentClicked = "SegmentClicked" 
    EventSegmentRemoved = "SegmentRemoved" 
    EventSegmentRenderOffsetChanged = "SegmentOffsetChanged" 
    EventSegmentSequenceChanged = "SegmentSequenceChanged" 
    EventSegmentSized = "SegmentSized" 
    EventSortColumnChanged = "SortColumnChanged" 
    EventSortDirectionChanged = "SortDirectionChanged" 
    EventSortSettingChanged = "SortSettingChanged" 
    EventSplitterDoubleClicked = "SplitterDoubleClicked" 
    ColumnCount = property(_cegui.ListHeader_ColumnCount_get, None, None,
                    "ListHeader.ColumnCount -> CEGUI::uint  (read-only)\n\nThis is equivalent to calling the C++ CEGUI method:\n\tget: CEGUI::uint ListHeader::getColumnCount()")

    SortColumn = property(_cegui.ListHeader_SortColumn_get, _cegui.ListHeader_SortColumn_set, None,
                    "ListHeader.SortColumn -> CEGUI::uint\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: CEGUI::uint ListHeader::getSortColumn()\n\tset: void ListHeader::setSortColumn(CEGUI::uint)")


    TotalSegmentsPixelExtent = property(_cegui.ListHeader_TotalSegmentsPixelExtent_get, None, None,
                    "ListHeader.TotalSegmentsPixelExtent -> float  (read-only)\n\nThis is equivalent to calling the C++ CEGUI method:\n\tget: float ListHeader::getTotalSegmentsPixelExtent()")

    SegmentOffset = property(_cegui.ListHeader_SegmentOffset_get, _cegui.ListHeader_SegmentOffset_set, None,
                    "ListHeader.SegmentOffset -> float\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: float ListHeader::getSegmentOffset()\n\tset: void ListHeader::setSegmentOffset(float)")


    SortDirection = property(_cegui.ListHeader_SortDirection_get, _cegui.ListHeader_SortDirection_set, None,
                    "ListHeader.SortDirection -> CEGUI::ListHeaderSegment::SortDirection\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: CEGUI::ListHeaderSegment::SortDirection ListHeader::getSortDirection()\n\tset: void ListHeader::setSortDirection(CEGUI::ListHeaderSegment::SortDirection)")


    SortSegment = property(_cegui.ListHeader_SortSegment_get, _cegui.ListHeader_SortSegment_set, None,
                    "ListHeader.SortSegment -> CEGUI::ListHeaderSegment &\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: CEGUI::ListHeaderSegment & ListHeader::getSortSegment()\n\tset: void ListHeader::setSortSegment(CEGUI::ListHeaderSegment &)")


    SortEnabled = property(_cegui.ListHeader_SortEnabled_get, _cegui.ListHeader_SortEnabled_set, None,
                    "ListHeader.SortEnabled -> bool\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: bool ListHeader::isSortingEnabled()\n\tset: void ListHeader::setSortingEnabled(bool)")


    ColumnSizingEnabled = property(_cegui.ListHeader_ColumnSizingEnabled_get, _cegui.ListHeader_ColumnSizingEnabled_set, None,
                    "ListHeader.ColumnSizingEnabled -> bool\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: bool ListHeader::isColumnSizingEnabled()\n\tset: void ListHeader::setColumnSizingEnabled(bool)")


    ColumnDraggingEnabled = property(_cegui.ListHeader_ColumnDraggingEnabled_get, _cegui.ListHeader_ColumnDraggingEnabled_set, None,
                    "ListHeader.ColumnDraggingEnabled -> bool\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: bool ListHeader::isColumnDraggingEnabled()\n\tset: void ListHeader::setColumnDraggingEnabled(bool)")


ListHeader.getSegmentFromColumn = new_instancemethod(_cegui.ListHeader_getSegmentFromColumn,None,ListHeader)
ListHeader.getSegmentFromID = new_instancemethod(_cegui.ListHeader_getSegmentFromID,None,ListHeader)
ListHeader.getColumnFromSegment = new_instancemethod(_cegui.ListHeader_getColumnFromSegment,None,ListHeader)
ListHeader.getColumnFromID = new_instancemethod(_cegui.ListHeader_getColumnFromID,None,ListHeader)
ListHeader.getColumnWithText = new_instancemethod(_cegui.ListHeader_getColumnWithText,None,ListHeader)
ListHeader.getPixelOffsetToSegment = new_instancemethod(_cegui.ListHeader_getPixelOffsetToSegment,None,ListHeader)
ListHeader.getPixelOffsetToColumn = new_instancemethod(_cegui.ListHeader_getPixelOffsetToColumn,None,ListHeader)
ListHeader.getColumnWidth = new_instancemethod(_cegui.ListHeader_getColumnWidth,None,ListHeader)
ListHeader.setSortColumnFromID = new_instancemethod(_cegui.ListHeader_setSortColumnFromID,None,ListHeader)
ListHeader.addColumn = new_instancemethod(_cegui.ListHeader_addColumn,None,ListHeader)
ListHeader.insertColumn = new_instancemethod(_cegui.ListHeader_insertColumn,None,ListHeader)
ListHeader.removeColumn = new_instancemethod(_cegui.ListHeader_removeColumn,None,ListHeader)
ListHeader.removeSegment = new_instancemethod(_cegui.ListHeader_removeSegment,None,ListHeader)
ListHeader.moveColumn = new_instancemethod(_cegui.ListHeader_moveColumn,None,ListHeader)
ListHeader.moveSegment = new_instancemethod(_cegui.ListHeader_moveSegment,None,ListHeader)
ListHeader.setColumnWidth = new_instancemethod(_cegui.ListHeader_setColumnWidth,None,ListHeader)
ListHeader_swigregister = _cegui.ListHeader_swigregister
ListHeader_swigregister(ListHeader)
ListHeader.WidgetTypeName = _cegui.cvar.ListHeader_WidgetTypeName
ListHeader.ScrollSpeed = _cegui.cvar.ListHeader_ScrollSpeed
ListHeader.MinimumSegmentPixelWidth = _cegui.cvar.ListHeader_MinimumSegmentPixelWidth
ListHeader.SegmentNameSuffix = _cegui.cvar.ListHeader_SegmentNameSuffix

class _FontManagerSingleton(object):
    thisown = property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, *args): 
        _cegui._FontManagerSingleton_swiginit(self,_cegui.new__FontManagerSingleton(*args))
    __swig_destroy__ = _cegui.delete__FontManagerSingleton
    getSingleton = staticmethod(_cegui._FontManagerSingleton_getSingleton)
_FontManagerSingleton_swigregister = _cegui._FontManagerSingleton_swigregister
_FontManagerSingleton_swigregister(_FontManagerSingleton)
_FontManagerSingleton_getSingleton = _cegui._FontManagerSingleton_getSingleton

class FontManager(_FontManagerSingleton):
    thisown = property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, *args): 
        _cegui.FontManager_swiginit(self,_cegui.new_FontManager(*args))
    __swig_destroy__ = _cegui.delete_FontManager
FontManager.createFont = new_instancemethod(_cegui.FontManager_createFont,None,FontManager)
FontManager.destroyFont = new_instancemethod(_cegui.FontManager_destroyFont,None,FontManager)
FontManager.destroyAllFonts = new_instancemethod(_cegui.FontManager_destroyAllFonts,None,FontManager)
FontManager.isFontPresent = new_instancemethod(_cegui.FontManager_isFontPresent,None,FontManager)
FontManager.getFont = new_instancemethod(_cegui.FontManager_getFont,None,FontManager)
FontManager.notifyScreenResolution = new_instancemethod(_cegui.FontManager_notifyScreenResolution,None,FontManager)
FontManager.writeFontToStream = new_instancemethod(_cegui.FontManager_writeFontToStream,None,FontManager)
FontManager_swigregister = _cegui.FontManager_swigregister
FontManager_swigregister(FontManager)

class _ImagesetManagerSingleton(object):
    thisown = property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, *args): 
        _cegui._ImagesetManagerSingleton_swiginit(self,_cegui.new__ImagesetManagerSingleton(*args))
    __swig_destroy__ = _cegui.delete__ImagesetManagerSingleton
    getSingleton = staticmethod(_cegui._ImagesetManagerSingleton_getSingleton)
_ImagesetManagerSingleton_swigregister = _cegui._ImagesetManagerSingleton_swigregister
_ImagesetManagerSingleton_swigregister(_ImagesetManagerSingleton)
_ImagesetManagerSingleton_getSingleton = _cegui._ImagesetManagerSingleton_getSingleton

class ImagesetManager(_ImagesetManagerSingleton):
    thisown = property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, *args): 
        _cegui.ImagesetManager_swiginit(self,_cegui.new_ImagesetManager(*args))
    __swig_destroy__ = _cegui.delete_ImagesetManager
ImagesetManager.createImageset = new_instancemethod(_cegui.ImagesetManager_createImageset,None,ImagesetManager)
ImagesetManager.createImagesetFromImageFile = new_instancemethod(_cegui.ImagesetManager_createImagesetFromImageFile,None,ImagesetManager)
ImagesetManager.destroyImageset = new_instancemethod(_cegui.ImagesetManager_destroyImageset,None,ImagesetManager)
ImagesetManager.destroyAllImagesets = new_instancemethod(_cegui.ImagesetManager_destroyAllImagesets,None,ImagesetManager)
ImagesetManager.getImageset = new_instancemethod(_cegui.ImagesetManager_getImageset,None,ImagesetManager)
ImagesetManager.isImagesetPresent = new_instancemethod(_cegui.ImagesetManager_isImagesetPresent,None,ImagesetManager)
ImagesetManager.notifyScreenResolution = new_instancemethod(_cegui.ImagesetManager_notifyScreenResolution,None,ImagesetManager)
ImagesetManager.writeImagesetToStream = new_instancemethod(_cegui.ImagesetManager_writeImagesetToStream,None,ImagesetManager)
ImagesetManager_swigregister = _cegui.ImagesetManager_swigregister
ImagesetManager_swigregister(ImagesetManager)

class _LoggerSingleton(object):
    thisown = property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, *args): 
        _cegui._LoggerSingleton_swiginit(self,_cegui.new__LoggerSingleton(*args))
    __swig_destroy__ = _cegui.delete__LoggerSingleton
    getSingleton = staticmethod(_cegui._LoggerSingleton_getSingleton)
_LoggerSingleton_swigregister = _cegui._LoggerSingleton_swigregister
_LoggerSingleton_swigregister(_LoggerSingleton)
_LoggerSingleton_getSingleton = _cegui._LoggerSingleton_getSingleton

Errors = _cegui.Errors
Standard = _cegui.Standard
Informative = _cegui.Informative
Insane = _cegui.Insane
class Logger(_LoggerSingleton):
    thisown = property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    def __init__(self): raise AttributeError, "No constructor defined"
    __repr__ = _swig_repr
    __swig_destroy__ = _cegui.delete_Logger
    LoggingLevel = property(_cegui.Logger_LoggingLevel_get, _cegui.Logger_LoggingLevel_set, None,
                    "Logger.LoggingLevel -> CEGUI::LoggingLevel\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: CEGUI::LoggingLevel Logger::getLoggingLevel()\n\tset: void Logger::setLoggingLevel(CEGUI::LoggingLevel)")


Logger.logEvent = new_instancemethod(_cegui.Logger_logEvent,None,Logger)
Logger.setLogFilename = new_instancemethod(_cegui.Logger_setLogFilename,None,Logger)
Logger_swigregister = _cegui.Logger_swigregister
Logger_swigregister(Logger)

class _MouseCursorSingleton(object):
    thisown = property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, *args): 
        _cegui._MouseCursorSingleton_swiginit(self,_cegui.new__MouseCursorSingleton(*args))
    __swig_destroy__ = _cegui.delete__MouseCursorSingleton
    getSingleton = staticmethod(_cegui._MouseCursorSingleton_getSingleton)
_MouseCursorSingleton_swigregister = _cegui._MouseCursorSingleton_swigregister
_MouseCursorSingleton_swigregister(_MouseCursorSingleton)
_MouseCursorSingleton_getSingleton = _cegui._MouseCursorSingleton_getSingleton

BlankMouseCursor = _cegui.BlankMouseCursor
DefaultMouseCursor = _cegui.DefaultMouseCursor
class MouseCursor(EventSet,_MouseCursorSingleton):
    thisown = property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, *args): 
        _cegui.MouseCursor_swiginit(self,_cegui.new_MouseCursor(*args))
    __swig_destroy__ = _cegui.delete_MouseCursor
    getSingleton = staticmethod(_cegui.MouseCursor_getSingleton)
    getSingletonPtr = staticmethod(_cegui.MouseCursor_getSingletonPtr)
    Image = property(_cegui.MouseCursor___getImage, fix_args(_cegui.MouseCursor___setImage), None,
                    "MouseCursor.Image -> type\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: MouseCursor::getImage()\n\tset: void MouseCursor::setImage(...)")


    ConstraintArea = property(_cegui.MouseCursor___getConstraintArea, fix_args(_cegui.MouseCursor___setConstraintArea), None,
                    "MouseCursor.ConstraintArea -> type\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: MouseCursor::getConstraintArea()\n\tset: void MouseCursor::setConstraintArea(...)")

    UnifiedConstraintArea = property(_cegui.MouseCursor___getUnifiedConstraintArea, fix_args(_cegui.MouseCursor___setUnifiedConstraintArea), None,
                    "MouseCursor.UnifiedConstraintArea -> type\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: MouseCursor::getUnifiedConstraintArea()\n\tset: void MouseCursor::setUnifiedConstraintArea(...)")



    EventNamespace = "MouseCursor" 
    EventImageChanged = "ImageChanged" 
    Position = property(_cegui.MouseCursor_Position_get, _cegui.MouseCursor_Position_set, None,
                    "MouseCursor.Position -> CEGUI::Point\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: CEGUI::Point MouseCursor::getPosition()\n\tset: void MouseCursor::setPosition(CEGUI::Point)")


    DisplayIndependantPosition = property(_cegui.MouseCursor_DisplayIndependantPosition_get, None, None,
                    "MouseCursor.DisplayIndependantPosition -> CEGUI::Point  (read-only)\n\nThis is equivalent to calling the C++ CEGUI method:\n\tget: CEGUI::Point MouseCursor::getDisplayIndependantPosition()")

    Visible = property(_cegui.MouseCursor_Visible_get, _cegui.MouseCursor_Visible_set, None,
                    "MouseCursor.Visible -> bool\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: bool MouseCursor::isVisible()\n\tset: void MouseCursor::setVisible(bool)")


MouseCursor.draw = new_instancemethod(_cegui.MouseCursor_draw,None,MouseCursor)
MouseCursor.offsetPosition = new_instancemethod(_cegui.MouseCursor_offsetPosition,None,MouseCursor)
MouseCursor.hide = new_instancemethod(_cegui.MouseCursor_hide,None,MouseCursor)
MouseCursor.show = new_instancemethod(_cegui.MouseCursor_show,None,MouseCursor)
MouseCursor_swigregister = _cegui.MouseCursor_swigregister
MouseCursor_swigregister(MouseCursor)
MouseCursor_getSingleton = _cegui.MouseCursor_getSingleton
MouseCursor_getSingletonPtr = _cegui.MouseCursor_getSingletonPtr

class _SchemeManagerSingleton(object):
    thisown = property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, *args): 
        _cegui._SchemeManagerSingleton_swiginit(self,_cegui.new__SchemeManagerSingleton(*args))
    __swig_destroy__ = _cegui.delete__SchemeManagerSingleton
    getSingleton = staticmethod(_cegui._SchemeManagerSingleton_getSingleton)
_SchemeManagerSingleton_swigregister = _cegui._SchemeManagerSingleton_swigregister
_SchemeManagerSingleton_swigregister(_SchemeManagerSingleton)
_SchemeManagerSingleton_getSingleton = _cegui._SchemeManagerSingleton_getSingleton

class SchemeManager(_SchemeManagerSingleton):
    thisown = property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, *args): 
        _cegui.SchemeManager_swiginit(self,_cegui.new_SchemeManager(*args))
    __swig_destroy__ = _cegui.delete_SchemeManager
SchemeManager.loadScheme = new_instancemethod(_cegui.SchemeManager_loadScheme,None,SchemeManager)
SchemeManager.unloadScheme = new_instancemethod(_cegui.SchemeManager_unloadScheme,None,SchemeManager)
SchemeManager.isSchemePresent = new_instancemethod(_cegui.SchemeManager_isSchemePresent,None,SchemeManager)
SchemeManager.getScheme = new_instancemethod(_cegui.SchemeManager_getScheme,None,SchemeManager)
SchemeManager.unloadAllSchemes = new_instancemethod(_cegui.SchemeManager_unloadAllSchemes,None,SchemeManager)
SchemeManager_swigregister = _cegui.SchemeManager_swigregister
SchemeManager_swigregister(SchemeManager)

class _WindowManagerSingleton(object):
    thisown = property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, *args): 
        _cegui._WindowManagerSingleton_swiginit(self,_cegui.new__WindowManagerSingleton(*args))
    __swig_destroy__ = _cegui.delete__WindowManagerSingleton
    getSingleton = staticmethod(_cegui._WindowManagerSingleton_getSingleton)
_WindowManagerSingleton_swigregister = _cegui._WindowManagerSingleton_swigregister
_WindowManagerSingleton_swigregister(_WindowManagerSingleton)
_WindowManagerSingleton_getSingleton = _cegui._WindowManagerSingleton_getSingleton

class WindowManagerKeyIterator(object):
    thisown = property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    def __init__(self): raise AttributeError, "No constructor defined"
    __repr__ = _swig_repr
    __swig_destroy__ = _cegui.delete_WindowManagerKeyIterator
WindowManagerKeyIterator.next = new_instancemethod(_cegui.WindowManagerKeyIterator_next,None,WindowManagerKeyIterator)
WindowManagerKeyIterator.__iter__ = new_instancemethod(_cegui.WindowManagerKeyIterator___iter__,None,WindowManagerKeyIterator)
WindowManagerKeyIterator_swigregister = _cegui.WindowManagerKeyIterator_swigregister
WindowManagerKeyIterator_swigregister(WindowManagerKeyIterator)

class WindowManagerValueIterator(object):
    thisown = property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    def __init__(self): raise AttributeError, "No constructor defined"
    __repr__ = _swig_repr
    __swig_destroy__ = _cegui.delete_WindowManagerValueIterator
WindowManagerValueIterator.next = new_instancemethod(_cegui.WindowManagerValueIterator_next,None,WindowManagerValueIterator)
WindowManagerValueIterator.__iter__ = new_instancemethod(_cegui.WindowManagerValueIterator___iter__,None,WindowManagerValueIterator)
WindowManagerValueIterator_swigregister = _cegui.WindowManagerValueIterator_swigregister
WindowManagerValueIterator_swigregister(WindowManagerValueIterator)

class WindowManagerItemIterator(object):
    thisown = property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    def __init__(self): raise AttributeError, "No constructor defined"
    __repr__ = _swig_repr
    __swig_destroy__ = _cegui.delete_WindowManagerItemIterator
WindowManagerItemIterator.next = new_instancemethod(_cegui.WindowManagerItemIterator_next,None,WindowManagerItemIterator)
WindowManagerItemIterator.__iter__ = new_instancemethod(_cegui.WindowManagerItemIterator___iter__,None,WindowManagerItemIterator)
WindowManagerItemIterator_swigregister = _cegui.WindowManagerItemIterator_swigregister
WindowManagerItemIterator_swigregister(WindowManagerItemIterator)

class WindowManager(_WindowManagerSingleton):
    thisown = property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, *args): 
        _cegui.WindowManager_swiginit(self,_cegui.new_WindowManager(*args))
    __swig_destroy__ = _cegui.delete_WindowManager
    getDefaultResourceGroup = staticmethod(_cegui.WindowManager_getDefaultResourceGroup)
    setDefaultResourceGroup = staticmethod(_cegui.WindowManager_setDefaultResourceGroup)
WindowManager.createWindow = new_instancemethod(_cegui.WindowManager_createWindow,None,WindowManager)
WindowManager.destroyWindow = new_instancemethod(_cegui.WindowManager_destroyWindow,None,WindowManager)
WindowManager.getWindow = new_instancemethod(_cegui.WindowManager_getWindow,None,WindowManager)
WindowManager.isWindowPresent = new_instancemethod(_cegui.WindowManager_isWindowPresent,None,WindowManager)
WindowManager.destroyAllWindows = new_instancemethod(_cegui.WindowManager_destroyAllWindows,None,WindowManager)
WindowManager.loadWindowLayout = new_instancemethod(_cegui.WindowManager_loadWindowLayout,None,WindowManager)
WindowManager.isDeadPoolEmpty = new_instancemethod(_cegui.WindowManager_isDeadPoolEmpty,None,WindowManager)
WindowManager.cleanDeadPool = new_instancemethod(_cegui.WindowManager_cleanDeadPool,None,WindowManager)
WindowManager.writeWindowLayoutToStream = new_instancemethod(_cegui.WindowManager_writeWindowLayoutToStream,None,WindowManager)
WindowManager.renameWindow = new_instancemethod(_cegui.WindowManager_renameWindow,None,WindowManager)
WindowManager.keys = new_instancemethod(_cegui.WindowManager_keys,None,WindowManager)
WindowManager.values = new_instancemethod(_cegui.WindowManager_values,None,WindowManager)
WindowManager.items = new_instancemethod(_cegui.WindowManager_items,None,WindowManager)
WindowManager.iterkeys = new_instancemethod(_cegui.WindowManager_iterkeys,None,WindowManager)
WindowManager.itervalues = new_instancemethod(_cegui.WindowManager_itervalues,None,WindowManager)
WindowManager.iteritems = new_instancemethod(_cegui.WindowManager_iteritems,None,WindowManager)
WindowManager.__iter__ = new_instancemethod(_cegui.WindowManager___iter__,None,WindowManager)
WindowManager_swigregister = _cegui.WindowManager_swigregister
WindowManager_swigregister(WindowManager)
WindowManager.GeneratedWindowNameBase = _cegui.cvar.WindowManager_GeneratedWindowNameBase
WindowManager_getDefaultResourceGroup = _cegui.WindowManager_getDefaultResourceGroup
WindowManager_setDefaultResourceGroup = _cegui.WindowManager_setDefaultResourceGroup

class _WindowRendererManagerSingleton(object):
    thisown = property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, *args): 
        _cegui._WindowRendererManagerSingleton_swiginit(self,_cegui.new__WindowRendererManagerSingleton(*args))
    __swig_destroy__ = _cegui.delete__WindowRendererManagerSingleton
    getSingleton = staticmethod(_cegui._WindowRendererManagerSingleton_getSingleton)
_WindowRendererManagerSingleton_swigregister = _cegui._WindowRendererManagerSingleton_swigregister
_WindowRendererManagerSingleton_swigregister(_WindowRendererManagerSingleton)
_WindowRendererManagerSingleton_getSingleton = _cegui._WindowRendererManagerSingleton_getSingleton

class WindowRendererManager(_WindowRendererManagerSingleton):
    thisown = property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, *args): 
        _cegui.WindowRendererManager_swiginit(self,_cegui.new_WindowRendererManager(*args))
    __swig_destroy__ = _cegui.delete_WindowRendererManager
    getSingleton = staticmethod(_cegui.WindowRendererManager_getSingleton)
    getSingletonPtr = staticmethod(_cegui.WindowRendererManager_getSingletonPtr)
WindowRendererManager.isFactoryPresent = new_instancemethod(_cegui.WindowRendererManager_isFactoryPresent,None,WindowRendererManager)
WindowRendererManager.getFactory = new_instancemethod(_cegui.WindowRendererManager_getFactory,None,WindowRendererManager)
WindowRendererManager.addFactory = new_instancemethod(_cegui.WindowRendererManager_addFactory,None,WindowRendererManager)
WindowRendererManager.removeFactory = new_instancemethod(_cegui.WindowRendererManager_removeFactory,None,WindowRendererManager)
WindowRendererManager.createWindowRenderer = new_instancemethod(_cegui.WindowRendererManager_createWindowRenderer,None,WindowRendererManager)
WindowRendererManager.destroyWindowRenderer = new_instancemethod(_cegui.WindowRendererManager_destroyWindowRenderer,None,WindowRendererManager)
WindowRendererManager_swigregister = _cegui.WindowRendererManager_swigregister
WindowRendererManager_swigregister(WindowRendererManager)
WindowRendererManager_getSingleton = _cegui.WindowRendererManager_getSingleton
WindowRendererManager_getSingletonPtr = _cegui.WindowRendererManager_getSingletonPtr

class _SystemSingleton(object):
    thisown = property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, *args): 
        _cegui._SystemSingleton_swiginit(self,_cegui.new__SystemSingleton(*args))
    __swig_destroy__ = _cegui.delete__SystemSingleton
    getSingleton = staticmethod(_cegui._SystemSingleton_getSingleton)
_SystemSingleton_swigregister = _cegui._SystemSingleton_swigregister
_SystemSingleton_swigregister(_SystemSingleton)
_SystemSingleton_getSingleton = _cegui._SystemSingleton_getSingleton

class System(_SystemSingleton,EventSet):
    thisown = property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, *args): 
        _cegui.System_swiginit(self,_cegui.new_System(*args))
    __swig_destroy__ = _cegui.delete_System
    getSingleton = staticmethod(_cegui.System_getSingleton)
    getSingletonPtr = staticmethod(_cegui.System_getSingletonPtr)
    DefaultFont = property(_cegui.System___getDefaultFont, fix_args(_cegui.System___setDefaultFont), None,
                    "System.DefaultFont -> type\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: System::getDefaultFont()\n\tset: void System::setDefaultFont(...)")



    DefaultMouseCursor = property(_cegui.System___getDefaultMouseCursor, fix_args(_cegui.System___setDefaultMouseCursor), None,
                    "System.DefaultMouseCursor -> type\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: System::getDefaultMouseCursor()\n\tset: void System::setDefaultMouseCursor(...)")

    DefaultTooltip = property(_cegui.System___getDefaultTooltip, fix_args(_cegui.System___setDefaultTooltip), None,
                    "System.DefaultTooltip -> type\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: System::getDefaultTooltip()\n\tset: void System::setDefaultTooltip(...)")


    setDefaultXMLParserName = staticmethod(_cegui.System_setDefaultXMLParserName)
    getDefaultXMLParserName = staticmethod(_cegui.System_getDefaultXMLParserName)
    EventNamespace = "System" 
    EventDefaultFontChanged = "DefaultFontChanged" 
    EventDefaultMouseCursorChanged = "DefaultMouseCursorChanged" 
    EventGUISheetChanged = "GUISheetChanged" 
    EventMouseMoveScalingChanged = "MouseMoveScalingChanged" 
    EventMultiClickAreaSizeChanged = "MultiClickAreaSizeChanged" 
    EventMultiClickTimeoutChanged = "MultiClickTimeoutChanged" 
    EventSingleClickTimeoutChanged = "SingleClickTimeoutChanged" 
    RequestRedraw = property(_cegui.System_RequestRedraw_get, _cegui.System_RequestRedraw_set)
    GUISheet = property(_cegui.System_GUISheet_get, _cegui.System_GUISheet_set, None,
                    "System.GUISheet -> CEGUI::Window *\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: CEGUI::Window * System::getGUISheet()\n\tset: void System::setGUISheet(CEGUI::Window *)")


    SingleClickTimeout = property(_cegui.System_SingleClickTimeout_get, _cegui.System_SingleClickTimeout_set, None,
                    "System.SingleClickTimeout -> double\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: double System::getSingleClickTimeout()\n\tset: void System::setSingleClickTimeout(double)")


    MultiClickTimeout = property(_cegui.System_MultiClickTimeout_get, _cegui.System_MultiClickTimeout_set, None,
                    "System.MultiClickTimeout -> double\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: double System::getMultiClickTimeout()\n\tset: void System::setMultiClickTimeout(double)")


    MultiClickToleranceAreaSize = property(_cegui.System_MultiClickToleranceAreaSize_get, _cegui.System_MultiClickToleranceAreaSize_set, None,
                    "System.MultiClickToleranceAreaSize -> const CEGUI::Size &\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: const CEGUI::Size & System::getMultiClickToleranceAreaSize()\n\tset: void System::setMultiClickToleranceAreaSize(const CEGUI::Size &)")


    WindowContainingMouse = property(_cegui.System_WindowContainingMouse_get, None, None,
                    "System.WindowContainingMouse -> CEGUI::Window *  (read-only)\n\nThis is equivalent to calling the C++ CEGUI method:\n\tget: CEGUI::Window * System::getWindowContainingMouse()")

    MouseMoveScaling = property(_cegui.System_MouseMoveScaling_get, _cegui.System_MouseMoveScaling_set, None,
                    "System.MouseMoveScaling -> float\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: float System::getMouseMoveScaling()\n\tset: void System::setMouseMoveScaling(float)")


    ModalTarget = property(_cegui.System_ModalTarget_get, _cegui.System_ModalTarget_set, None,
                    "System.ModalTarget -> CEGUI::Window *\n\nThis is equivalent to calling the C++ CEGUI methods:\n\tget: CEGUI::Window * System::getModalTarget()\n\tset: void System::setModalTarget(CEGUI::Window *)")


    SystemKeys = property(_cegui.System_SystemKeys_get, None, None,
                    "System.SystemKeys -> CEGUI::uint  (read-only)\n\nThis is equivalent to calling the C++ CEGUI method:\n\tget: CEGUI::uint System::getSystemKeys()")

    ScriptingModule = property(_cegui.System_ScriptingModule_get, None, None,
                    "System.ScriptingModule -> CEGUI::ScriptModule *  (read-only)\n\nThis is equivalent to calling the C++ CEGUI method:\n\tget: CEGUI::ScriptModule * System::getScriptingModule()")

    Renderer = property(_cegui.System_Renderer_get, None, None,
                    "System.Renderer -> CEGUI::Renderer *  (read-only)\n\nThis is equivalent to calling the C++ CEGUI method:\n\tget: CEGUI::Renderer * System::getRenderer()")

    ResourceProvider = property(_cegui.System_ResourceProvider_get, None, None,
                    "System.ResourceProvider -> CEGUI::ResourceProvider *  (read-only)\n\nThis is equivalent to calling the C++ CEGUI method:\n\tget: CEGUI::ResourceProvider * System::getResourceProvider()")

    XMLParser = property(_cegui.System_XMLParser_get, None, None,
                    "System.XMLParser -> CEGUI::XMLParser *  (read-only)\n\nThis is equivalent to calling the C++ CEGUI method:\n\tget: CEGUI::XMLParser * System::getXMLParser()")

System.renderGUI = new_instancemethod(_cegui.System_renderGUI,None,System)
System.setScriptingModule = new_instancemethod(_cegui.System_setScriptingModule,None,System)
System.executeScriptFile = new_instancemethod(_cegui.System_executeScriptFile,None,System)
System.executeScriptGlobal = new_instancemethod(_cegui.System_executeScriptGlobal,None,System)
System.executeScriptString = new_instancemethod(_cegui.System_executeScriptString,None,System)
System.notifyWindowDestroyed = new_instancemethod(_cegui.System_notifyWindowDestroyed,None,System)
System.injectMouseMove = new_instancemethod(_cegui.System_injectMouseMove,None,System)
System.injectMouseLeaves = new_instancemethod(_cegui.System_injectMouseLeaves,None,System)
System.injectMouseButtonDown = new_instancemethod(_cegui.System_injectMouseButtonDown,None,System)
System.injectMouseButtonUp = new_instancemethod(_cegui.System_injectMouseButtonUp,None,System)
System.injectKeyDown = new_instancemethod(_cegui.System_injectKeyDown,None,System)
System.injectKeyUp = new_instancemethod(_cegui.System_injectKeyUp,None,System)
System.injectMouseWheelChange = new_instancemethod(_cegui.System_injectMouseWheelChange,None,System)
System.injectMousePosition = new_instancemethod(_cegui.System_injectMousePosition,None,System)
System.injectTimePulse = new_instancemethod(_cegui.System_injectTimePulse,None,System)
System.injectChar = new_instancemethod(_cegui.System_injectChar,None,System)
System_swigregister = _cegui.System_swigregister
System_swigregister(System)
System.DefaultSingleClickTimeout = _cegui.cvar.System_DefaultSingleClickTimeout
System.DefaultMultiClickTimeout = _cegui.cvar.System_DefaultMultiClickTimeout
System.DefaultMultiClickAreaSize = _cegui.cvar.System_DefaultMultiClickAreaSize
System_getSingleton = _cegui.System_getSingleton
System_getSingletonPtr = _cegui.System_getSingletonPtr
System_setDefaultXMLParserName = _cegui.System_setDefaultXMLParserName
System_getDefaultXMLParserName = _cegui.System_getDefaultXMLParserName

class _GlobalEventSetSingleton(object):
    thisown = property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, *args): 
        _cegui._GlobalEventSetSingleton_swiginit(self,_cegui.new__GlobalEventSetSingleton(*args))
    __swig_destroy__ = _cegui.delete__GlobalEventSetSingleton
    getSingleton = staticmethod(_cegui._GlobalEventSetSingleton_getSingleton)
_GlobalEventSetSingleton_swigregister = _cegui._GlobalEventSetSingleton_swigregister
_GlobalEventSetSingleton_swigregister(_GlobalEventSetSingleton)
_GlobalEventSetSingleton_getSingleton = _cegui._GlobalEventSetSingleton_getSingleton

class GlobalEventSet(EventSet,_GlobalEventSetSingleton):
    thisown = property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, *args): 
        _cegui.GlobalEventSet_swiginit(self,_cegui.new_GlobalEventSet(*args))
    __swig_destroy__ = _cegui.delete_GlobalEventSet
    getSingleton = staticmethod(_cegui.GlobalEventSet_getSingleton)
    getSingletonPtr = staticmethod(_cegui.GlobalEventSet_getSingletonPtr)
GlobalEventSet.subscribeEvent = new_instancemethod(_cegui.GlobalEventSet_subscribeEvent,None,GlobalEventSet)
GlobalEventSet.fireEvent = new_instancemethod(_cegui.GlobalEventSet_fireEvent,None,GlobalEventSet)
GlobalEventSet_swigregister = _cegui.GlobalEventSet_swigregister
GlobalEventSet_swigregister(GlobalEventSet)
GlobalEventSet_getSingleton = _cegui.GlobalEventSet_getSingleton
GlobalEventSet_getSingletonPtr = _cegui.GlobalEventSet_getSingletonPtr

class CoordConverter(object):
    thisown = property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    windowToScreenX = staticmethod(_cegui.CoordConverter_windowToScreenX)
    windowToScreenY = staticmethod(_cegui.CoordConverter_windowToScreenY)
    windowToScreen = staticmethod(_cegui.CoordConverter_windowToScreen)
    screenToWindowX = staticmethod(_cegui.CoordConverter_screenToWindowX)
    screenToWindowY = staticmethod(_cegui.CoordConverter_screenToWindowY)
    screenToWindow = staticmethod(_cegui.CoordConverter_screenToWindow)
    def __init__(self, *args): 
        _cegui.CoordConverter_swiginit(self,_cegui.new_CoordConverter(*args))
    __swig_destroy__ = _cegui.delete_CoordConverter
CoordConverter_swigregister = _cegui.CoordConverter_swigregister
CoordConverter_swigregister(CoordConverter)
CoordConverter_windowToScreenX = _cegui.CoordConverter_windowToScreenX
CoordConverter_windowToScreenY = _cegui.CoordConverter_windowToScreenY
CoordConverter_windowToScreen = _cegui.CoordConverter_windowToScreen
CoordConverter_screenToWindowX = _cegui.CoordConverter_screenToWindowX
CoordConverter_screenToWindowY = _cegui.CoordConverter_screenToWindowY
CoordConverter_screenToWindow = _cegui.CoordConverter_screenToWindow

class PropertyHelper(object):
    thisown = property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    stringToFloat = staticmethod(_cegui.PropertyHelper_stringToFloat)
    stringToUint = staticmethod(_cegui.PropertyHelper_stringToUint)
    stringToInt = staticmethod(_cegui.PropertyHelper_stringToInt)
    stringToBool = staticmethod(_cegui.PropertyHelper_stringToBool)
    stringToSize = staticmethod(_cegui.PropertyHelper_stringToSize)
    stringToPoint = staticmethod(_cegui.PropertyHelper_stringToPoint)
    stringToRect = staticmethod(_cegui.PropertyHelper_stringToRect)
    stringToImage = staticmethod(_cegui.PropertyHelper_stringToImage)
    stringToColour = staticmethod(_cegui.PropertyHelper_stringToColour)
    stringToColourRect = staticmethod(_cegui.PropertyHelper_stringToColourRect)
    stringToUDim = staticmethod(_cegui.PropertyHelper_stringToUDim)
    stringToUVector2 = staticmethod(_cegui.PropertyHelper_stringToUVector2)
    stringToURect = staticmethod(_cegui.PropertyHelper_stringToURect)
    floatToString = staticmethod(_cegui.PropertyHelper_floatToString)
    uintToString = staticmethod(_cegui.PropertyHelper_uintToString)
    intToString = staticmethod(_cegui.PropertyHelper_intToString)
    boolToString = staticmethod(_cegui.PropertyHelper_boolToString)
    sizeToString = staticmethod(_cegui.PropertyHelper_sizeToString)
    pointToString = staticmethod(_cegui.PropertyHelper_pointToString)
    rectToString = staticmethod(_cegui.PropertyHelper_rectToString)
    imageToString = staticmethod(_cegui.PropertyHelper_imageToString)
    colourToString = staticmethod(_cegui.PropertyHelper_colourToString)
    colourRectToString = staticmethod(_cegui.PropertyHelper_colourRectToString)
    udimToString = staticmethod(_cegui.PropertyHelper_udimToString)
    uvector2ToString = staticmethod(_cegui.PropertyHelper_uvector2ToString)
    urectToString = staticmethod(_cegui.PropertyHelper_urectToString)
    def __init__(self, *args): 
        _cegui.PropertyHelper_swiginit(self,_cegui.new_PropertyHelper(*args))
    __swig_destroy__ = _cegui.delete_PropertyHelper
PropertyHelper_swigregister = _cegui.PropertyHelper_swigregister
PropertyHelper_swigregister(PropertyHelper)
PropertyHelper_stringToFloat = _cegui.PropertyHelper_stringToFloat
PropertyHelper_stringToUint = _cegui.PropertyHelper_stringToUint
PropertyHelper_stringToInt = _cegui.PropertyHelper_stringToInt
PropertyHelper_stringToBool = _cegui.PropertyHelper_stringToBool
PropertyHelper_stringToSize = _cegui.PropertyHelper_stringToSize
PropertyHelper_stringToPoint = _cegui.PropertyHelper_stringToPoint
PropertyHelper_stringToRect = _cegui.PropertyHelper_stringToRect
PropertyHelper_stringToImage = _cegui.PropertyHelper_stringToImage
PropertyHelper_stringToColour = _cegui.PropertyHelper_stringToColour
PropertyHelper_stringToColourRect = _cegui.PropertyHelper_stringToColourRect
PropertyHelper_stringToUDim = _cegui.PropertyHelper_stringToUDim
PropertyHelper_stringToUVector2 = _cegui.PropertyHelper_stringToUVector2
PropertyHelper_stringToURect = _cegui.PropertyHelper_stringToURect
PropertyHelper_floatToString = _cegui.PropertyHelper_floatToString
PropertyHelper_uintToString = _cegui.PropertyHelper_uintToString
PropertyHelper_intToString = _cegui.PropertyHelper_intToString
PropertyHelper_boolToString = _cegui.PropertyHelper_boolToString
PropertyHelper_sizeToString = _cegui.PropertyHelper_sizeToString
PropertyHelper_pointToString = _cegui.PropertyHelper_pointToString
PropertyHelper_rectToString = _cegui.PropertyHelper_rectToString
PropertyHelper_imageToString = _cegui.PropertyHelper_imageToString
PropertyHelper_colourToString = _cegui.PropertyHelper_colourToString
PropertyHelper_colourRectToString = _cegui.PropertyHelper_colourRectToString
PropertyHelper_udimToString = _cegui.PropertyHelper_udimToString
PropertyHelper_uvector2ToString = _cegui.PropertyHelper_uvector2ToString
PropertyHelper_urectToString = _cegui.PropertyHelper_urectToString



