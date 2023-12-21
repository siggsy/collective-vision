#include "Simulation.hpp"
#include <regex>

using namespace std;


// ----------------------------------- [ Constants ] ---------------------------------------- //


// <color> (<color> : <α0>,<α1>,<β0>,<β1>, <γ>)
const regex reg = regex(
	"^\\s*"
	"(\\d+)"
	"\\s*\\(\\s*"
		"(\\d+)"              "\\s*:\\s*"
		"(\\d+(?:\\.\\d+)?)"  "\\s*,\\s*"
		"(\\d+(?:\\.\\d+)?)"  "\\s*,\\s*"
		"(\\d+(?:\\.\\d+)?)"  "\\s*,\\s*"
		"(\\d+(?:\\.\\d+)?)"  "\\s*,\\s*"
		"(\\d+(?:\\.\\d+)?)"  "\\s*"
	"\\)"
	"\\s*$"
);


// ----------------------------------- [ Functions ] ---------------------------------------- //


bool parseParameter(const string& s, tuple<int,int>* outColor, SimParam* outParam){
	smatch m;
	if (regex_match(s, m, reg)){
		
		if (outColor != nullptr){
			int a =  stoi(m[1]);
			int b =  stoi(m[2]);
			*outColor = {a, b};
		}
		
		if (outParam != nullptr){
			outParam->α0 = strtod(m[3].first.base(), nullptr);
			outParam->α1 = strtod(m[4].first.base(), nullptr);
			outParam->β0 = strtod(m[5].first.base(), nullptr);
			outParam->β1 = strtod(m[6].first.base(), nullptr);
			outParam->γ  = strtod(m[7].first.base(), nullptr);
		}
		
		return true;
	}
	
	return false;
}


// ------------------------------------------------------------------------------------------ //