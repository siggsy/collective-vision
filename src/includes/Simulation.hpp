#pragma once
#include <vector>
#include <memory>
#include <unordered_map>

#include "Vec2.hpp"


class Boid;


// ----------------------------------- [ Structures ] --------------------------------------- //


struct SimParam {
	real γ;			// Speed relaxation rate
	real α0;		// Attraction/repultion (speed)
	real α1;
	real β0;		// Attraction/repultion (angle)
	real β1;
};


typedef std::vector<std::unique_ptr<Boid>> SimulationState;
typedef std::unordered_map<int,SimParam> SimulationParameters;


// ----------------------------------- [ Functions ] ---------------------------------------- //


/**
 * @brief Progress the simulation to the next step.
 *        Disregard object colors.
 * @param param Parameters for all objects.
 * @param objects Previous simulation state.
 * @return New simulation state with new objects. Object order is not preserved.
 */
SimulationState simulationStep(const SimParam& param, const SimulationState& objects);


/**
 * @brief Progress the simulation to the next step.
 *        Use separate parameters for each colored view interval.
 * @param params Map of color to parameters for each possible color of the objects.
 * @param objects Previous simulation state.
 * @throws `std::out_of_range` When `params` does not contain an entry for a color of an interval or object.
 * @return New simulation state with new objects. Object order is not preserved.
 */
SimulationState simulationStep(const SimulationParameters& params, const SimulationState& objects);


// ----------------------------------- [ Functions ] ---------------------------------------- //

/**
 * @brief Parse simulation parameters from string.
 *        Format: `<color>(<α0>,<α1>,<β0>,<β1>,<γ>)`
 *        Example: `0(0.5, 0.08, 0.1, 0.08, 0.95)`
 * @return SimParam if parsing successful else null.
 */
bool parseParameter(const std::string&, int* outColor, SimParam* outParam);


// ------------------------------------------------------------------------------------------ //