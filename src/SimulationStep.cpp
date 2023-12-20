#include "Simulation.hpp"
#include "Boid.hpp"
#include "Vec2.hpp"
#include <cmath>

using namespace std;


// ----------------------------------- [ Constants ] ---------------------------------------- //


const real γ  = 0.95; // Speed relaxation rate
const real α0 = 0.5; // Attraction/repultion (speed)
const real α1 = 0.08;
const real β0 = 0.1; // Attraction/repultion (angle)
const real β1 = 0.08;


// ----------------------------------- [ Functions ] ---------------------------------------- //


// ∫{a,b} cos(ϕ) dϕ
inline real cosIntegral(real a, real b){
	return sin(b) - sin(a);
}


// ∫{a,b} sin(ϕ) dϕ
inline real sinIntegral(real a, real b){
	return cos(a) - cos(b);
}


real calculateSpeed(const ProjectionField& P, const Vec2& velocity){
	const real prefSpeed = Boid::prefSpeed;
	const real speed = length(velocity);
	const real φ = angle(velocity);
	
	// -α0    * ∫(  cos(ϕ) P(ϕ)       dϕ)
	// +α0 α1 * ∫(  cos(ϕ) (∂ϕP(ϕ))²  dϕ)
	
	// ∫_{-π}^{+π}{ cos(ϕ) P(ϕ) dϕ }
	real intA = 0;
	for (const Interval& span : P){
		intA += cosIntegral(span.start - φ, span.end - φ);
	}
	
	// ∫_{-π}^{+π}{ cos(ϕ) (∂ϕP(ϕ))² dϕ }
	real intB = 0;
	for (const Interval& span : P){
		intB += cos(span.start - φ) + cos(span.end - φ);
	}
	
	const real intg = (α1*intB - intA) * α0;
	return γ * (prefSpeed - speed) + intg;
}


real calculateAngle(const ProjectionField& P, const Vec2& velocity){
	const real φ = angle(velocity);

	// -β0    * ∫(  sin(ϕ) P(ϕ)       dϕ)
	// +β0 β1 * ∫(  sin(ϕ) (∂ϕP(ϕ))²  dϕ)
	
	// ∫_{-π}^{+π}{ sin(ϕ) P(ϕ) dϕ }
	real intA = 0;
	for (const Interval& span : P){
		intA += sinIntegral(span.start - φ, span.end - φ);
	}
	
	// ∫_{-π}^{+π}{ sin(ϕ) (∂ϕP(ϕ))² dϕ }
	real intB = 0;
	for (const Interval& span : P){
		intB += sin(span.start - φ) + sin(span.end - φ);
	}
	
	return (β1*intB - intA) * β0;
}


Vec2 calculateVelocity(const ProjectionField& field, const Vec2& velocity){
	const real v = calculateSpeed(field, velocity);
	const real a = calculateAngle(field, velocity);
	const real φ = angle(velocity) + a;
	const real speed = length(velocity) + v;
	return Vec2(cos(φ), sin(φ)) * speed;
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
	obj->view = calculateProjectionField(prevState, prevObj);
	obj->velocity = calculateVelocity(obj->view, prevObj.velocity);
	obj->pos = prevObj.pos + prevObj.velocity;
	
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
