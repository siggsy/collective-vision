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
	bool help = false;
}


// ----------------------------------- [ Constants ] ---------------------------------------- //


enum OptionID : int {
	NONE = INT32_MIN,
	HELP,
	OUTPUT,
	COLOR
} selected_opt;


const char* const short_options = "h" "o:" "c:";


const struct option long_options[] = {
	{"help",   no_argument,       (int*)&selected_opt, OptionID::HELP   },
	{"output", required_argument, (int*)&selected_opt, OptionID::OUTPUT },
	{"color",  required_argument, (int*)&selected_opt, OptionID::COLOR  },
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
	help = false;
}


// ------------------------------------------------------------------------------------------ //