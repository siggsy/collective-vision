#pragma once
#include "Vec2.hpp"
#include "ProjectionField.hpp"


class Boid {
// ----------------------------------- [ Constants ] ---------------------------------------- //
public:
	static constexpr real prefSpeed = 0.5;
	
// ------------------------------------[ Properties ] --------------------------------------- //
public:
	real size = 0.5;
	Vec2 pos = {0,0};
	Vec2 velocity = {0,0};
	int color = 0;
	ProjectionField view = {};
	
// ---------------------------------- [ Constructors ] -------------------------------------- //
public:
	Boid(){}
	Boid(real x, real y) : pos{x,y} {}
	Boid(real x, real y, real size) : size{size}, pos{x,y} {}
	Boid(real x, real y, real size, int color) : size{size}, pos{x,y}, color{color} {}
	
// ----------------------------------- [ Functions ] ---------------------------------------- //
public:
	Interval getProjection(const Boid& other) const;
	
// ------------------------------------------------------------------------------------------ //
};
