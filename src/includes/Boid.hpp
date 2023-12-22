#pragma once
#include "Vec2.hpp"
#include "ProjectionField.hpp"
#include <string>


class Boid {
// ------------------------------------[ Properties ] --------------------------------------- //
public:
	real size = 0.5;
	real prefSpeed = 0.5;
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
	Boid(real x, real y, real size, int color, real prefSpeed) : size{size}, pos{x,y}, color{color}, prefSpeed{prefSpeed} {}
	
// ----------------------------------- [ Functions ] ---------------------------------------- //
public:
	Interval getProjection(const Boid& other) const;
	
// ------------------------------------------------------------------------------------------ //
};


// ----------------------------------- [ Structures ] --------------------------------------- //


struct BoidParam {
	int count;
	int color;
	real size;
	real prefSpeed;
};

typedef std::vector<BoidParam> BoidParameters;


// ----------------------------------- [ Functions ] ---------------------------------------- //

/**
 * @brief Parse simulation parameters from string.
 *        Format: `(<count>,<color>,<size>,<prefSpeed>)`
 *        Example: `(0.5, 0.08, 0.1, 0.08, 0.95)`
 * @return SimParam if parsing successful else null.
 */
bool parseParameter(const std::string&, BoidParam* outParam);


// ------------------------------------------------------------------------------------------ //
