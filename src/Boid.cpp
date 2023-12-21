#include "Boid.hpp"
#include <numbers>

using namespace std::numbers;


// ----------------------------------- [ Functions ] ---------------------------------------- //


Interval Boid::getProjection(const Boid& other) const {
	const real d = distance(this->pos, other.pos);
	
	// Local projection
	const real r = other.size;
	const real alpha = (real)(atan(r / d));
	
	// Rotation
	const real xd = other.pos.x - this->pos.x;
	real beta = (real)(acos(xd / d));
	
	// acos returns [0,π], correct for when subject is below object
	if (other.pos.y < this->pos.y)
		beta = (real)(2*pi) - beta;
	
	return Interval {
		.start = beta - alpha,
		.end = beta + alpha,
		.z = d,
		.color = other.color
	};
}


/*
Interval Boid::getProjection(const Boid& other) const {
	const real d = distance(this->pos, other.pos);
	
	// Local projection
	const real r = other.size;
	const real alpha = (real)(atan(r / d));
	
	// Rotation
	const real xd = other.pos.x - this->pos.x;
	real beta = (real)(acos(xd / d));
	
	// acos returns [0,π], correct for when subject is below object
	if (other.pos.y < this->pos.y)
		beta = (real)(2*pi) - beta;
	
	return {beta - alpha, beta + alpha};
}
*/

// ------------------------------------------------------------------------------------------ //