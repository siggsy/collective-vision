#pragma once
#include "Vec2.hpp"
#include "ProjectionField.hpp"


class Boid {
// ----------------------------------- [ Constants ] ---------------------------------------- //
public:
	static constexpr real prefSpeed = 1;
	
// ------------------------------------[ Properties ] --------------------------------------- //
public:
	real size = 1;
	Vec2 pos = {0,0};
	Vec2 velocity = {0,0};
	ProjectionField view = {};
	
// ---------------------------------- [ Constructors ] -------------------------------------- //
public:
	Boid(){}
	Boid(real x, real y) : pos{x,y} {}
	Boid(real x, real y, real size) : size{size}, pos{x,y} {}
	
// ----------------------------------- [ Functions ] ---------------------------------------- //
public:
	Interval getProjection(const Boid& other) const;
	
// ------------------------------------------------------------------------------------------ //
};
