#include "Simulation.hpp"
#include "Boid.hpp"
#include <regex>

using namespace std;


// ----------------------------------- [ Constants ] ---------------------------------------- //


// <color> (<color> : <α0>,<α1>,<β0>,<β1>, <γ>)
static const regex sim_reg = regex(
	"^\\s*"
	"(\\d+)"
	"\\s*\\(\\s*"
		"(\\d+)"                "\\s*:\\s*"
		"(-?\\d+(?:\\.\\d+)?)"  "\\s*,\\s*"
		"(-?\\d+(?:\\.\\d+)?)"  "\\s*,\\s*"
		"(-?\\d+(?:\\.\\d+)?)"  "\\s*,\\s*"
		"(-?\\d+(?:\\.\\d+)?)"  "\\s*,\\s*"
		"(-?\\d+(?:\\.\\d+)?)"  "\\s*"
	"\\)"
	"\\s*$"
);


// (<count>, <color>, <size>, <prefSpeed>)
static const regex boid_reg = regex(
	"^\\s*"
	"\\(\\s*"
		"(\\d+)"              "\\s*,\\s*"
		"(\\d+)"              "\\s*,\\s*"
		"(\\d+(?:\\.\\d+)?)"  "\\s*,\\s*"
		"(\\d+(?:\\.\\d+)?)"  "\\s*,\\s*"
		"(\\d+(?:\\.\\d+)?)"  "\\s*\\s*"
	"\\)"
	"\\s*$"
);


// ----------------------------------- [ Functions ] ---------------------------------------- //


bool parseParameter(const string& s, BoidParam& outParam){
	smatch m;
	if (regex_match(s, m, boid_reg)){
		outParam.count = stoi(m[1]);
		outParam.color = stoi(m[2]);
		outParam.size = strtod(m[3].first.base(), nullptr);
		outParam.prefSpeed = strtod(m[4].first.base(), nullptr);
		outParam.maxSpeed = strtod(m[5].first.base(), nullptr);
		return true;
	}
	return false;
}


// ----------------------------------- [ Functions ] ---------------------------------------- //


bool parseParameter(const string& s, tuple<int,int>& outColor, SimParam& outParam){
	smatch m;
	if (regex_match(s, m, sim_reg)){
		int a =  stoi(m[1]);
		int b =  stoi(m[2]);
		outColor = {a, b};
		
		outParam.α0 = strtod(m[3].first.base(), nullptr);
		outParam.α1 = strtod(m[4].first.base(), nullptr);
		outParam.β0 = strtod(m[5].first.base(), nullptr);
		outParam.β1 = strtod(m[6].first.base(), nullptr);
		outParam.γ  = strtod(m[7].first.base(), nullptr);
		
		return true;
	}
	return false;
}


// ------------------------------------------------------------------------------------------ //
