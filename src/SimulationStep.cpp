#include "Simulation.hpp"
#include "Boid.hpp"

using namespace std;


// ----------------------------------- [ Functions ] ---------------------------------------- //


// TODO
Vec2 calculateVelocity(const ProjectionField& field){
	return {0,0};
}


// ----------------------------------- [ Functions ] ---------------------------------------- //


ProjectionField calculateProjectionField(const vector<unique_ptr<Boid>>& state, const Boid& boid){
	ProjectionField field = {};
	
	for (int i = 0 ; i < state.size() ; i++){
		if (state[i].get() != &boid){
			Interval p = boid.getProjection(*state[i]);
			insertInterval(field, p);
		}
	}
	
	return field;
}


// ----------------------------------- [ Functions ] ---------------------------------------- //


unique_ptr<Boid> simulateOne(const vector<unique_ptr<Boid>>& prevState, int i){
	unique_ptr<Boid> obj = make_unique<Boid>();
	const Boid& prevObj = *prevState[i];
	
	obj->size = prevObj.size;
	obj->pos += prevObj.velocity;
	obj->view = calculateProjectionField(prevState, prevObj);
	obj->velocity = calculateVelocity(obj->view);
	
	return obj;
}


// ----------------------------------- [ Functions ] ---------------------------------------- //


// TODO: threads
vector<unique_ptr<Boid>> simulationStep(const vector<unique_ptr<Boid>>& prevState){
	vector<unique_ptr<Boid>> state = {};
	
	for (int i = 0 ; i < prevState.size() ; i++){
		state.emplace_back(simulateOne(prevState, i));
	}
	
	return state;
}


// ------------------------------------------------------------------------------------------ //