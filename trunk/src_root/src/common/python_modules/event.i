%module Event
%{
#define _OISExport
#include "common/common_stdafx.h"
#include "common/interface/event_manager.h"
#include "extlib/OIS/OIS.h"
typedef OIS::Object Object;
%}
%ignore swigConverterFunc;
%ignore swigTypeInfo;
%ignore swigCastInfo;

typedef unsigned int uint;
typedef unsigned long ulong;
typedef unsigned short ushort;
typedef unsigned char uchar;
typedef std::string String;
typedef unsigned char byte;
typedef OIS::Object Object;

#define _OISExport
%include "extlib/OIS/OISKeyboard.h"


%typemap(in)  (swigTypeInfo *typeInfo, SWIGTYPE * )
{
   $1 = (swigTypeInfo*) ($2_descriptor);
}

%include "common/interface/event_manager.h"


