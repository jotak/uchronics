#ifndef _MOVES_HELPER_H
#define _MOVES_HELPER_H

#include "Movement.h"

class MovesHelper
{
public:
	static Movement * newConstantMove(Coords3D vector, double fSpeed);
	static Movement * newDecelMove(Coords3D vector, double fFullSpeed);
	static Movement * newAccelMove(Coords3D vector, double fFullSpeed);
	static Movement * newAccelDecelMove(Coords3D vector, double fFullSpeed);
};

#endif
