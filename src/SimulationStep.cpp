#include "Simulation.hpp"
#include <cmath>
#include <stdexcept>

#include "Boid.hpp"
#include "Vec2.hpp"


using namespace std;


// ----------------------------------- [ Functions ] ---------------------------------------- //


// ∫{a,b} cos(ϕ) dϕ
inline real cosIntegral(real a, real b){
	return sin(b) - sin(a);
}


// ∫{a,b} sin(ϕ) dϕ
inline real sinIntegral(real a, real b){
	return cos(a) - cos(b);
}


/**
 * @throws `std::out_of_range` When `params` does not contain an entry for a color of an interval or object.
 */
inline const SimParam& getParam(const SimulationParameters& params, int self, int other){
	auto p = params.find({self, other});
	if (p != params.end())
		return p->second;
	else
		throw out_of_range("Missing simulation parameters for color mapping '(" + to_string(self) + ":" + to_string(other) + ")'.");
}


// ----------------------------------- [ Functions ] ---------------------------------------- //


real calculateSpeed(const SimParam& param, const ProjectionField& P, const Vec2& velocity, const real prefSpeed, int color){
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
	
	const real intg = (param.α1 * intB - intA) * param.α0;
	return param.γ * (prefSpeed - speed) + intg;
}


/**
 * @throws `std::out_of_range` When `params` does not contain an entry for a color of an interval or object.
 */
real calculateSpeed(const SimulationParameters& params, const ProjectionField& P, const Vec2& velocity, const real prefSpeed, int color){
	const real speed = length(velocity);
	const real φ = angle(velocity);
	
	// ∫_{-π}^{+π}( -α0 cos(ϕ) P(ϕ) dϕ)
	real intA = 0;
	for (const Interval& span : P){
		const SimParam& param = getParam(params, color, span.color);
		intA -= cosIntegral(span.start - φ, span.end - φ) * param.α0;
	}
	
	// ∫_{-π}^{+π}( +α0 α1 cos(ϕ) (∂ϕP(ϕ))² dϕ)
	real intB = 0;
	for (const Interval& span : P){
		const SimParam& param = getParam(params, color, span.color);
		intB += (cos(span.start - φ) + cos(span.end - φ)) * param.α0 * param.α1;
	}
	
	const SimParam& param = getParam(params, color, color);
	return param.γ * (prefSpeed - speed) + (intA + intB);
}


// ----------------------------------- [ Functions ] ---------------------------------------- //


real calculateAngle(const SimParam& param, const ProjectionField& P, const Vec2& velocity, int color){
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
	
	return (param.β1 * intB - intA) * param.β0;
}


real calculateAngle(const SimulationParameters& params, const ProjectionField& P, const Vec2& velocity, int color){
	const real φ = angle(velocity);

	// ∫_{-π}^{+π}( -β0 sin(ϕ) P(ϕ) dϕ)
	real intA = 0;
	for (const Interval& span : P){
		const SimParam& param = getParam(params, color, span.color);
		intA -= sinIntegral(span.start - φ, span.end - φ) * param.β0;
	}
	
	// ∫_{-π}^{+π}( +β0 β1 sin(ϕ) (∂ϕP(ϕ))² dϕ)
	real intB = 0;
	for (const Interval& span : P){
		const SimParam& param = getParam(params, color, span.color);
		intB += (sin(span.start - φ) + sin(span.end - φ)) * param.β0 * param.β1;
	}
	
	return intA + intB;
}


// ----------------------------------- [ Functions ] ---------------------------------------- //


template<typename PARAMS>
Vec2 calculateVelocity(const PARAMS& params, const ProjectionField& field, const Vec2& velocity, const real prefSpeed, const real maxSpeed, int color){
	const real v = calculateSpeed(params, field, velocity, prefSpeed, color);
	const real a = calculateAngle(params, field, velocity, color);
	const real φ = angle(velocity) + a;
	const real speed = max(length(velocity) + v, maxSpeed);
	return Vec2(cos(φ), sin(φ)) * speed;
}


ProjectionField calculateProjectionField(const SimulationState& state, const Boid& boid){
	ProjectionField field = {};
	
	for (const unique_ptr<Boid>& b : state){
		if (b.get() != &boid){
			Interval p = boid.getProjection(*b);
			insertInterval(field, p);
		}
	}
	
	return field;
}


// ----------------------------------- [ Functions ] ---------------------------------------- //


template<typename PARAMS>
unique_ptr<Boid> simulateOne(const PARAMS& params, const vector<unique_ptr<Boid>>& prevState, int i){
	unique_ptr<Boid> obj = make_unique<Boid>();
	const Boid& prevObj = *prevState[i];
	
	obj->size = prevObj.size;
	obj->color = prevObj.color;
	obj->view = calculateProjectionField(prevState, prevObj);
	obj->velocity = calculateVelocity(params, obj->view, prevObj.velocity, prevObj.prefSpeed, prevObj.maxSpeed, obj->color);
	obj->pos = prevObj.pos + prevObj.velocity;
	
	return obj;
}

// ----------------------------------- [ Functions ] ---------------------------------------- //


// TODO: threads
template<typename PARAMS>
static SimulationState _simulationStep(const PARAMS& params, const SimulationState& prevState){
	SimulationState state = {};
	
	for (int i = 0 ; i < prevState.size() ; i++){
		state.emplace_back(simulateOne(params, prevState, i));
	}
	
	return state;
}


SimulationState simulationStep(const SimParam& param, const SimulationState& objects){
	return _simulationStep(param, objects);
}


SimulationState simulationStep(const SimulationParameters& params, const SimulationState& objects){
	return _simulationStep(params, objects);
}


// ------------------------------------------------------------------------------------------ //
