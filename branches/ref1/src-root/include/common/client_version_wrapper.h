#ifndef __CLIENT_VERSION_WRAPPER_H_
#define __CLIENT_VERSION_WRAPPER_H_

#include "ireon_client/version.h"

#ifdef PRODUCT_VERSION
	#define CURRENT_CLIENT_VERSION PRODUCT_VERSION
	#undef PRODUCT_VERSION
#elif
	#define CURRENT_CLIENT_VERSION 0,0,0,0
#endif

#ifdef PRODUCT_VERSION_STR
	#define CURRENT_CLIENT_VERSION_STR PRODUCT_VERSION_STR
	#undef PRODUCT_VERSION_STR
#elif
	#define CURRENT_CLIENT_VERSION_STR "0,0,0,0"
#endif

#ifdef FILE_VERSION
	#undef FILE_VERSION
#endif
#ifdef FILE_VERSION_STR
	#undef FILE_VERSION_STR
#endif

#endif
