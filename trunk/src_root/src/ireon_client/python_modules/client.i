%module Client
%{
#include "ireon_client/stdafx.h"
#include "common/db/client_char_data.h"
#include "ireon_client/interface/client_event_manager.h"
#include "ireon_client/interface/client_interface.h"
#include "ireon_client/world/client_world.h"
%} 

%include "common\db\client_char_data.h"
// for support inheritance class
%import  "..\src\common\python_modules\event.i"
// parse headers
%include "ireon_client/interface/client_event_manager.h"

%import  "..\src\common\python_modules\interface.i"
%include "ireon_client/interface/client_interface.h"

%import  "..\src\common\python_modules\world.i"
%include "ireon_client/world/client_world.h"


