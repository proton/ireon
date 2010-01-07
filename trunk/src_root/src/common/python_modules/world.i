%module World
%{
#include "common/common_stdafx.h"
#include "common/world/world.h"
#include "common/world/generic_object/object.h"
#include "common/world/dynamic_object/dynamic.h"
#include "common/world/dynamic_object/character.h"
#include "common/world/dynamic_object/mob.h"
#include "common/world/static_object/static.h"
#include "common/world/static_object/static_prototype.h"
#include "common/world/managers/prototype_manager.h"
%} 

%include "common/world/world.h"
%include "common/world/generic_object/object.h"
%include "common/world/dynamic_object/dynamic.h"
%include "common/world/dynamic_object/character.h"
%include "common/world/dynamic_object/mob.h"
%include "common/world/static_object/static.h"
%include "common/world/static_object/static_prototype.h"
%include "common/world/managers/prototype_manager.h"
