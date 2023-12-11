#include "Boid.hpp"


// ----------------------------------- [ Constants ] ---------------------------------------- //


static const double rad_to_deg = 180.0 / M_PI;


// ----------------------------------- [ Functions ] ---------------------------------------- //


Interval Boid::getProjection(const Boid& other) const {
	const real d = distance(this->pos, other.pos);
	
	// Local projection
	const real r = other.size;
	const real alpha = atan(r / d) * rad_to_deg;
	
	// Rotation
	const real xd = other.pos.x - this->pos.x;
	real beta = acos(xd / d) * rad_to_deg;
	
	// acos returns [0,180], correct for when subject is below object
	if (other.pos.y < this->pos.y)
		beta = 360 - beta;
	
	return {beta - alpha, beta + alpha};
}


// ------------------------------------------------------------------------------------------ //