'''/** Class representing colour.
    @remarks
        Colour is represented as 4 components, each of which is a
        floating-point value from 0.0 to 1.0.
    @par
        The 3 "normal: colour components are red, green and blue, a higher
        number indicating greater amounts of that component in the colour.
        The forth component is the "alpha" value, which represents
        transparency. In this case, 0.0 is completely transparent and 1.0 is
        fully opaque.
*/'''
class ColourValue:

    Black = 0
    White = 0
    Red = 0
    Green = 0
    Blue = 0

    r,g,b,a = 0, 0, 0, 0
    val = []
    
    def __init__(self, red = 1.0, green = 1.0, blue = 1.0, alpha = 1.0 ):
        self.r = red
        self.g = green
        self.b = blue
        self.a = alpha
    
    ##//---------------------------------------------------------------------
    #bool ColourValue::operator==(const ColourValue& rhs) const
    def __cmp__(self, rhs):
        if  self.r == rhs.r and self.g == rhs.g and self.b == rhs.b and self.a == rhs.a:
                return 0
        else:
            return -1
    def __rcmp__(self, rhs):
        return -cmp(self, rhs)
    ##//---------------------------------------------------------------------
    
    
    
#       /** Retrieves colour as RGBA.
#        #//---------------------------------------------------------------------
    def getAsRGBA(self):
        val8 = 0
        val32 = 0
    
        ##// Convert to 32bit pattern
        ##// (RGBA = 8888)
    
        ##// Red
        val8 = int(self.r * 255)
        val32 = val8 << 24
    
        ##// Green
        val8 = int(self.g * 255)
        val32 += val8 << 16
    
        ##// Blue
        val8 = int(self.b * 255)
        val32 += val8 << 8
    
        ##// Alpha
        val8 = int(self.a * 255)
        val32 += val8
    
        return val32
        
#    /** Retrieves colour as ARGB.
#    #//---------------------------------------------------------------------
    def getAsARGB(self):
        val8 = 0
        val32 = 0
    
        ##// Convert to 32bit pattern
        ##// (ARGB = 8888)
    
        ##// Alpha
        val8 = int(a * 255)
        val32 = val8 << 24
    
        ##// Red
        val8 = int(r * 255)
        val32 += val8 << 16
    
        ##// Green
        val8 = int(g * 255)
        val32 += val8 << 8
    
        ##// Blue
        val8 = int(b * 255)
        val32 += val8
    
    
        return val32
    
    
#     /** Retrieves colours as ABGR */
#    #//---------------------------------------------------------------------
    def getAsABGR(self):
        val8 = 0
        val32 = 0
    
        #// Convert to 32bit pattern
        #// (ABRG = 8888)
    
        #// Alpha
        val8 = int(a * 255)
        val32 = val8 << 24
    
        #// Blue
        val8 = int(b * 255)
        val32 += val8 << 16
    
        #// Green
        val8 = int(g * 255)
        val32 += val8 << 8
    
        #// Red
        val8 = int(r * 255)
        val32 += val8
    
    
        return val32
  
    
    # /** Sets colour as RGBA.
    #//---------------------------------------------------------------------
    def setAsRGBA(self, val):
        val32 = val
    
        #// Convert from 32bit pattern
        #// (RGBA = 8888)
    
        #// Red
        r = int(val32 >> 24) / 255.0
    
        #// Green
        g = int(val32 >> 16) / 255.0
    
        #// Blue
        b = int(val32 >> 8) / 255.0
    
        #// Alpha
        a = int(val32) / 255.0
    #//---------------------------------------------------------------------
    def setAsARGB(self, val):
    
        val32 = val
    
        #// Convert from 32bit pattern
        #// (ARGB = 8888)
    
        #// Alpha
        a = int(val32 >> 24) / 255.0
    
        #// Red
        r = int(val32 >> 16) / 255.0
    
        #// Green
        g = int(val32 >> 8) / 255.0
    
        #// Blue
        b = int(val32) / 255.0
    
    #/** Sets colour as ABGR.
    #//---------------------------------------------------------------------
    def setAsABGR(val):
        val32 = val
    
        #// Convert from 32bit pattern
        #// (ABGR = 8888)
    
        #// Alpha
        a = int(val32 >> 24) / 255.0
    
        #// Blue
        b = int(val32 >> 16) / 255.0
    
        #// Green
        g = int(val32 >> 8) / 255.0
    
        #// Red
        r = int(val32) / 255.0
  
    #// arithmetic operations
    def __add__(self, rkVector):
        kSum = ColourValue(self.r + rkVector.r, self.g + rkVector.g, self.b + rkVector.b, self.a + rkVector.a)
        return kSum
        

    def __sub__(self, rkVector ):
        kDiff = ColourValue()

        kDiff.r = self.r - rkVector.r
        kDiff.g = self.g - rkVector.g
        kDiff.b = self.b - rkVector.b
        kDiff.a = self.a - rkVector.a

        return kDiff
    
    def __mul__ (self, other ):
        kProd = ColourValue()

        if isinstance( other, ColourValue) :
            kProd.r = other.r * self.r
            kProd.g = other.g * self.g
            kProd.b = other.b * self.b
            kProd.a = other.a * self.a
        else:
            kProd.r = other * self.r
            kProd.g = other * self.g
            kProd.b = other * self.b
            kProd.a = other * self.a

        return kProd
    
    def __rmul__ (self, other ):
        return self.__mul__(other)
    
    def __div__ (self, other ):
        kProd = ColourValue()

        if isinstance( other, ColourValue) :
            kProd.r =  self.r / other.r 
            kProd.g =  self.g / other.g
            kProd.b =  self.b / other.b 
            kProd.a =  self.a / other.a
        else:
            kProd.r =  self.r / other
            kProd.g =  self.g / other
            kProd.b =  self.b / other
            kProd.a =  self.a / other

        return kProd
    

   
'''   

    #// arithmetic updates
    inline ColourValue& operator += ( const ColourValue& rkVector )
    {
        r += rkVector.r
        g += rkVector.g
        b += rkVector.b
        a += rkVector.a

        return *this
    }

    inline ColourValue& operator -= ( const ColourValue& rkVector )
    {
        r -= rkVector.r
        g -= rkVector.g
        b -= rkVector.b
        a -= rkVector.a

        return *this
    }

    inline ColourValue& operator *= (const float fScalar )
    {
        r *= fScalar
        g *= fScalar
        b *= fScalar
        a *= fScalar
        return *this
    }

    inline ColourValue& operator /= (const float fScalar )
    {
        assert( fScalar != 0.0 )

        float fInv = 1.0 / fScalar

        r *= fInv
        g *= fInv
        b *= fInv
        a *= fInv

        return *this
    }
'''
ColourValue.Black = ColourValue(0.0,0.0,0.0)
ColourValue.White = ColourValue(1.0,1.0,1.0)
ColourValue.Red = ColourValue(1.0,0.0,0.0)
ColourValue.Green = ColourValue(0.0,1.0,0.0)
ColourValue.Blue = ColourValue(0.0,0.0,1.0)

