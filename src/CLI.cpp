#include "CLI.hpp"

extern "C" {
	#include <getopt.h>
}

#include <cstdint>
#include <cstring>
#include <unordered_set>
#include <stdexcept>
#include <iostream>

#define DEFAULT_APP_NAME	"vision-simulation" 


using namespace std;
using namespace CLI;


// ------------------------------------[ Variables ] ---------------------------------------- //



namespace CLI {
	string programName = DEFAULT_APP_NAME;
	string output;
	vector<string> colors;
	int prey_count;
	int predator_count;
	bool help = false;
}


// ----------------------------------- [ Constants ] ---------------------------------------- //


enum OptionID : int {
	NONE = INT32_MIN,
	HELP,
	OUTPUT,
	PREY,
	PREDATOR,
	COLOR
} selected_opt;


const char* const short_options = "h" "o:" "c:" "a:" "b:";


const struct option long_options[] = {
	{"help",     no_argument,       (int*)&selected_opt, OptionID::HELP     },
	{"output",   required_argument, (int*)&selected_opt, OptionID::OUTPUT   },
	{"color",    required_argument, (int*)&selected_opt, OptionID::COLOR    },
	{"prey",     required_argument, (int*)&selected_opt, OptionID::PREY     },
	{"predator", required_argument, (int*)&selected_opt, OptionID::PREDATOR },
	{0, 0, 0, 0}
};


// ----------------------------------- [ Functions ] ---------------------------------------- //


OptionID shortOptionToLong(char c){
	switch (c){
		case 'h':
			return OptionID::HELP;
		case 'o':
			return OptionID::OUTPUT;
		case 'c':
			return OptionID::COLOR;
		case 'a':
			return OptionID::PREY;
		case 'b':
			return OptionID::PREDATOR;
		default:
			return OptionID::NONE;
	}
}


// ----------------------------------- [ Functions ] ---------------------------------------- //


int getLength(void const* const* v){
	int n = 0;
	while (*v != nullptr){
		n++;
		v++;
	}
	return n;
}


// ----------------------------------- [ Functions ] ---------------------------------------- //


void CLI::parse(int argc, char const* const* argv){
	if (argv == nullptr || *argv == nullptr){
		return;
	}
	
	// Modifyable copy of args
	const int len = getLength((void const* const*)argv);
	const char** v = new const char*[len];
	copy(&argv[0], &argv[len], v);
	
	
	opterr = 0;
	optind = 1;
	programName = v[0];
	int prevOpt = optind;
	
	while (true){
		selected_opt = OptionID::NONE;
		const int c = getopt_long(argc, (char* const*)v, short_options, long_options, NULL);
		
		// Error
		if (c == '?'){
			if (optopt >= 0 && (shortOptionToLong(optopt) == OptionID::NONE)){
				int i = (optind > prevOpt) ? optind - 1 : optind;
				throw runtime_error(string("Unrecognized option '").append(v[i]).append("'."));
			} else {
				throw runtime_error(string("Missing option argument '").append(v[optind-1]).append("'."));
			}
		}
		
		else if (c > 0){
			selected_opt = shortOptionToLong(c);
		} else if (c < 0){
			break;
		}
		
		
		// Handle long options
		switch (selected_opt){
			
			case OptionID::HELP:
				help = true;
				break;
			
			case OptionID::OUTPUT:
				output = optarg;
				break;
			
			case OptionID::PREY:
				prey_count = atoi(optarg);
				if (prey_count < 0)
					throw runtime_error("Prey count must be non-negative.");
				break;
			
			case OptionID::PREDATOR:
				predator_count = atoi(optarg);
				if (predator_count < 0)
					throw runtime_error("Predator count must be non-negative.");
				break;
			
			case OptionID::COLOR:
				colors.emplace_back(optarg);
				break;
			
		}
		
		prevOpt = optind;
	};
	
	// Non-option arguments
	if (optind < argc){
		throw runtime_error(string("Unknown option '").append(v[optind]).append("'."));
	}
	
	delete[] v;
	return;
}


// ----------------------------------- [ Functions ] ---------------------------------------- //


void CLI::clear(){
	programName = DEFAULT_APP_NAME;
	output.clear();
	colors.clear();
	prey_count = 0;
	predator_count = 0;
	help = false;
}


// ------------------------------------------------------------------------------------------ //