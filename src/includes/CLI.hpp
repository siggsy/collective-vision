#include <string>
#include <vector>


// ------------------------------------[ Variables ] ---------------------------------------- //


namespace CLI {
	extern std::string programName;
	extern std::string output;
	extern std::vector<std::string> colors;
	extern bool help;
}


// ----------------------------------- [ Functions ] ---------------------------------------- //


namespace CLI {
	void parse(int argc, char const* const* argv);
	void clear();
}


// ------------------------------------------------------------------------------------------ //