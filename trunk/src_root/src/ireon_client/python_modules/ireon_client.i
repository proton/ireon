%module IreonClient
%include stl.i 
%include "..\src\common\python_modules\ogre_types.i"
%include "..\src\common\python_modules\event.i"
%include "..\src\common\python_modules\interface.i"
%include "..\src\common\python_modules\world.i"

/* instantiate the required template specializations */
typedef unsigned int uint;
typedef unsigned long ulong;
typedef unsigned short ushort;
typedef unsigned char uchar;
typedef unsigned char byte;

typedef std::string String;

namespace std {
    %template(IntVector)    vector<int>;
    %template(DoubleVector) vector<double>;
    %template(ByteVector) vector<byte>;

} 
%include "client.i"
%include "app.i"

