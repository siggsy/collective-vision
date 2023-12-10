#pragma once
#include "Vec2.hpp"


class Boid {
// ------------------------------------[ Properties ] --------------------------------------- //
public:
	Vec2 pos;
	real size;
	
// ---------------------------------- [ Constructors ] -------------------------------------- //
public:
	Boid(){}
	Boid(real x, real y) : pos{x,y}, size{1} {}
	Boid(real x, real y, real size) : pos{x,y}, size{size} {}
	
// ------------------------------------------------------------------------------------------ //
};
