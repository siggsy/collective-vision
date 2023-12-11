#pragma once
#include <vector>
#include <memory>


class Boid;


// ----------------------------------- [ Functions ] ---------------------------------------- //


/**
 * @brief Progress the simulation to the next step.
 * @param objects Previous simulation state.
 * @return New simulation state with new objects. Object order is not preserved.
 */
std::vector<std::unique_ptr<Boid>> simulationStep(const std::vector<std::unique_ptr<Boid>>& objects);


// ------------------------------------------------------------------------------------------ //