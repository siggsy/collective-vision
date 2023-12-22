#include "Boid.hpp"
#include <regex>
using namespace std;


// ----------------------------------- [ Constants ] ---------------------------------------- //


// (<count>, <color>, <size>, <prefSpeed>)
const regex reg = regex(
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


bool parseParameter(const string& s, BoidParam* outParam){
	smatch m;
	if (regex_match(s, m, reg)){
		
		if (outParam != nullptr){
			outParam->count = stoi(m[1]);
			outParam->color = stoi(m[2]);
			outParam->size = strtod(m[3].first.base(), nullptr);
			outParam->prefSpeed = strtod(m[4].first.base(), nullptr);
			outParam->maxSpeed = strtod(m[5].first.base(), nullptr);
		}
		
		return true;
	}
	
	return false;
}


// ------------------------------------------------------------------------------------------ //
