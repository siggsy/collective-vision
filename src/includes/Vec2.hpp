#pragma once
#include <cmath>


// ----------------------------------- [ Structures ] --------------------------------------- //


typedef float real;


struct Vec2 {
	real x;
	real y;
};


// ----------------------------------- [ Functions ] ---------------------------------------- //


inline real distance(const Vec2& a, const Vec2& b){
	const real x = b.x - a.x;
	const real y = b.y - a.y;
	return sqrt(x*x + y*y);
}


// ------------------------------------------------------------------------------------------ //