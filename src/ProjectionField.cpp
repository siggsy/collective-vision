#include "ProjectionField.hpp"
#include <numbers>
#include <algorithm>

using namespace std;
using namespace std::numbers;


// // DEBUG
// #include <iostream>
// #include "ANSI.h"
// constexpr double deg_to_rad = M_PI/180.0;
// constexpr double rad_to_deg = 180.0/M_PI;

// #define RAD(ϕ)	(ϕ*deg_to_rad)
// #define DEG(ϕ)	(ϕ*rad_to_deg)
// #define PINT	(printf(" [%f,%f]\n", DEG(start), DEG(end)))


// ----------------------------------- [ Functions ] ---------------------------------------- //


// Clamp angle between [0,2π)
static real normalize(real alpha){
	if (alpha < 0){
		alpha -= (int)(alpha / (2*pi)) * (2*pi);
		if (alpha < 0)
			alpha += 2*pi;
	}
	else if (alpha >= 2*pi){
		alpha -= (int)(alpha / (2*pi)) * (2*pi);
	}
	return alpha;
}


// ----------------------------------- [ Functions ] ---------------------------------------- //


inline Interval rebase(const Interval& original, real start, real end){
	return (
		Interval {
			.start = start,
			.end = end,
			.z = original.z,
			.color = original.color
		}
	);
}


void insertInterval(ProjectionField& field, const Interval& interval){
	real start = interval.start;
	real end;
	real span = interval.end - start;
	
	// Instant quit
	if (span <= 0){
		return;
	} else if (span >= 2*pi){
		field.clear();
		field.emplace_back(rebase(interval, 0, (real)(2*pi)));
		return;
	}
	
	start = normalize(start);
	end = start + span;
	
	// Split into two parts
	if (end > 2*pi){
		insertInterval(field, rebase(interval, 0, end - (real)(2*pi)));
		end = 2*pi;
	}
	
	
	// Find last element whose end point is larger than current start
	auto p = lower_bound(field.begin(), field.end(), start,
		[](const Interval& e, real x){
			return e.end < x;
		}
	);
	
	
	bool expandPrev = false;
	while (start < end){
		// printf("\n");
		// printf("  [%.0f,%.0f]  %s\n", DEG(start), DEG(end), expandPrev ? "EXPAND" : "");
		
		// Independant
		if (p == field.end() || end <= p->start){
			// printf(ANSI_PURPLE "finish" ANSI_RESET " [%f,%f]\n\n", DEG(start), DEG(end));
			
			if (expandPrev)
				(p-1)->end = end;
			else
				field.insert(p, rebase(interval, start, end));
			return;
		}
		
		// Insert part before current interval
		else if (start < p->start){
			// printf(ANSI_GREEN "before" ANSI_RESET " [%f,%f]\n", DEG(start), DEG(p->start));
			
			if (expandPrev){
				(p-1)->end = p->start;
			} else {
				p = field.insert(p, rebase(interval, start, p->start)) + 1;
			}
			
			expandPrev = true;
			start = p->start;
		}
		
		else if (start >= p->end){
			// printf(ANSI_RED "SKIP\n" ANSI_RESET);
			p++;
			expandPrev = false;
		}
		
		// Skip foreground
		else if (interval.z >= p->z){
			// printf(ANSI_YELLOW "skip" ANSI_RESET " [%.0f,%.0f]\n", DEG(start), DEG(p->end));
			start = p->end;
			p++;
			expandPrev = false;
		}
		
		// Replace whole interval
		else if (start == p->start && end >= p->end){
			// printf(ANSI_YELLOW "replace" ANSI_RESET " [%.0f,%.0f]\n", DEG(p->start), DEG(p->end));
			
			if (expandPrev){
				(p-1)->end = p->end;
				start = p->end;
				p = field.erase(p);
			} else {
				*p = rebase(interval, p->start, p->end);
				start = p->end;
				p++;
			}
			
			expandPrev = true;
		}
			
		// Replace left
		else if (start == p->start && end < p->end){
			// printf(ANSI_YELLOW "replace left" ANSI_RESET " [%.0f,%.0f][%.0f,%.0f]\n\n", DEG(start), DEG(end), DEG(end), DEG(p->end));
			p->start = end;
			
			if (expandPrev){
				(p-1)->end = end;
			} else {
				field.insert(p, rebase(interval, start, end));
			}
			
			return;
		}
		
		// shorten right
		else if (end >= p->end){
			// printf(ANSI_YELLOW "shorten right" ANSI_RESET " [%.0f,%.0f]\n", DEG(p->start), DEG(start));
			p->end = start;
			p++;
			expandPrev = false;
		}
		
		// Replace middle
		else {
			auto tmp = p->end;
			p->end = start;
			// printf(ANSI_YELLOW "change middle" ANSI_RESET " [%.0f,%.0f][%.0f,%.0f][%.0f,%.0f]\n\n", DEG(p->start), DEG(p->end), DEG(start), DEG(end), DEG(end), DEG(tmp));
			p = field.insert(p + 1, rebase(interval, start, end)) + 1;
			field.insert(p, rebase(p[-2], end, tmp));
			return;
		}
		
	}
	
	// printf(ANSI_PURPLE "finish" ANSI_RESET " [%.0f,%.0f]\n", DEG(start), DEG(end));
	// printf("\n");
}


// ----------------------------------- [ Functions ] ---------------------------------------- //


// inline Interval rebase(const Interval& original, real start, real end){
// 	return (
// 		Interval {
// 			.start = start,
// 			.end = end
// 		}
// 	);
// }


// void insertInterval(ProjectionField& field, const Interval& interval){
// 	real start = interval.start;
// 	real end;
// 	real span = interval.end - start;
	
// 	// Instant quit
// 	if (span <= 0){
// 		return;
// 	} else if (span >= 2*pi){
// 		field.clear();
// 		field.emplace_back(rebase(interval, 0, (real)(2*pi)));
// 		return;
// 	}
	
// 	start = normalize(start);
// 	end = start + span;
	
// 	// Split into two parts
// 	if (end > 2*pi){
// 		insertInterval(field, rebase(interval, 0, end - (real)(2*pi)));
// 		end = 2*pi;
// 	}
	
	
// 	// Find last element whose end point is larger than current start
// 	auto p_first = lower_bound(field.begin(), field.end(), start,
// 		[](const Interval& e, real x){
// 			return e.end < x;
// 		}
// 	);
	
	
// 	// Interval is independant
// 	if (p_first == field.end()){
// 		field.emplace_back(rebase(interval, start, end));
// 		return;
// 	} else if (end < p_first->start){
// 		field.insert(p_first, rebase(interval, start, end));
// 		return;
// 	}
	
	
// 	// Find last relevant element
// 	auto p_last = p_first;
// 	while ((p_last + 1) != field.end()){
// 		auto next = p_last + 1;
		
// 		if (next->start > end)
// 			break;
// 		else if (next->start > end)
// 			break;
		
// 		p_last = next;
// 	}
	
// 	// Assign start and end points
// 	if (start < p_first->start)
// 		p_first->start = start;
// 	if (end > p_last->end)
// 		p_first->end = end;
// 	else
// 		p_first->end = p_last->end;
	
// 	// Erase overlapping intervals
// 	if (p_first != p_last){
// 		field.erase(p_first + 1, p_last + 1);
// 	}
	
// }


// ------------------------------------------------------------------------------------------ //