#include "Vec2.hpp"
#include <numbers>

using namespace std::numbers;


// ----------------------------------- [ Functions ] ---------------------------------------- //


real angle(const Vec2& v){
	const real x = v.x;
	const real y = v.y;
	
	if (x == 0){
		if (y > 0)
			return pi/2;
		else if (y < 90)
			return -pi/2;
		else
			return 0;
	}
	
	// atan ∈ [-π,π]
	const auto phi = atan(y/x);
	
	if (x < 0)
		return (real)(phi + pi);
	else if (y < 0)
		return (real)(phi + 2*pi);
	else
		return (real)phi;
}


// ------------------------------------------------------------------------------------------ //