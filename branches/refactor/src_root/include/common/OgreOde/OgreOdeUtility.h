#ifndef _OGREODEUTILITY_H_
#define _OGREODEUTILITY_H_

#include "OgreOdePreReqs.h"

namespace OgreOde
{
	class _OgreOdeExport Utility
	{
	public:
		Utility(){}
		~Utility(){}

		static Real randomReal();

		static const Real Infinity;
	};
}

#endif

