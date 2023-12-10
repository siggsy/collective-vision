#include "ProjectionField.hpp"
#include <algorithm>

using namespace std;


// ----------------------------------- [ Constants ] ---------------------------------------- //


static const double rad_to_deg = 180.0 / M_PI;


// ----------------------------------- [ Functions ] ---------------------------------------- //


Interval getProjection(const Boid& obj, const Boid& subj){
	const real d = distance(obj.pos, subj.pos);
	
	// Local projection
	const real r = subj.size;
	const real alpha = atan(r / d) * rad_to_deg;
	
	// Rotation
	const real xd = subj.pos.x - obj.pos.x;
	real beta = acos(xd / d) * rad_to_deg;
	
	// acos returns [0,180], correct for when subject is below object
	if (subj.pos.y < obj.pos.y)
		beta = 360 - beta;
	
	return {beta - alpha, beta + alpha};
}


// ----------------------------------- [ Functions ] ---------------------------------------- //


// Clamp angle between [0,360)
static real normalize(real alpha){
	if (alpha < 0){
		alpha -= ((int)alpha / 360) * 360;
		if (alpha < 0)
			alpha += 360;
	}
	else if (alpha >= 360){
		alpha -= ((int)alpha / 360) * 360;
	}
	return alpha;
}


void insertInterval(ProjectionField& field, const Interval& interval){
	real start = interval.start;
	real end;
	real span = interval.end - start;
	
	// Instant quit
	if (span <= 0){
		return;
	} else if (span >= 360){
		field.clear();
		field.emplace_back(0, 360);
		return;
	}
	
	start = normalize(start);
	end = start + span;
	
	// Split into two parts
	if (end > 360){
		insertInterval(field, {0, end - 360});
		end = 360;
	}
	
	// Find last element whose end point is larger than current start
	auto p_first = lower_bound(field.begin(), field.end(), start,
		[](const Interval& e, real x){
			return e.end < x;
		}
	);
	
	// Largest interval, append to back
	if (p_first == field.end()){
		field.emplace_back(start, end);
		return;
	}
	
	// Find last relevant element
	auto p_last = p_first;
	while ((p_last + 1) != field.end()){
		auto next = p_last + 1;
		
		if (next->start > end)
			break;
		else if (next->start > end)
			break;
		
		p_last = next;
	}
	
	// Assign start and end points
	if (start < p_first->start)
		p_first->start = start;
	if (end > p_last->end)
		p_first->end = end;
	else
		p_first->end = p_last->end;
	
	// Erase overlapping intervals
	if (p_first != p_last){
		field.erase(p_first + 1, p_last + 1);
	}
	
}


// ------------------------------------------------------------------------------------------ //