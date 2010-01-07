#include "OgreOdeUtility.h"

using namespace OgreOde;

const Real Utility::Infinity = dInfinity;

Real Utility::randomReal()
{
	return (Real)dRandReal();
}

