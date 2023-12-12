#pragma once
#include "Vec2.hpp"
#include <vector>


// ----------------------------------- [ Structures ] --------------------------------------- //


struct Interval {
	real start;
	real end;
};


/**
 * @brief The array asserts the following rules:
 *         a) All intervalls are within range of [0,2π] and `start` < `end`.
 *         b) There are no overlapping intervals;
 *         c) The array is sorted by starting points of intervals;
 */
typedef std::vector<Interval> ProjectionField;


// ----------------------------------- [ Functions ] ---------------------------------------- //

/**
 * @brief Insert interval into array that represents the whole
 *         projection field function.
 *        Assertions of the array are described in the typedef of `ProjectionField`.
 * @param field The projection field array.
 * @param interval The interval to insert.
 */
void insertInterval(ProjectionField& field, const Interval& interval);


// ------------------------------------------------------------------------------------------ //