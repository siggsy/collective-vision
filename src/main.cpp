#include <cstdio>
#include <fstream>
#include <iostream>
#include <numbers>
#include <ostream>
#include <random>
#include <chrono>

#include "ANSI.h"
#include "CLI.hpp"
#include "Boid.hpp"
#include "ProjectionField.hpp"
#include "Simulation.hpp"
#include "Vec2.hpp"


using namespace std;
using namespace std::chrono;


// ----------------------------------- [ Constants ] ---------------------------------------- //


constexpr double deg_to_rad = M_PI/180.0;
constexpr double rad_to_deg = 180.0/M_PI;

#define RAD(ϕ)	(ϕ*deg_to_rad)
#define DEG(ϕ)	(ϕ*rad_to_deg)


extern const char help[];


SimParam defaultSimulationParameters = {
	.γ  = 0.95,	// Speed relaxation rate
	.α0 = 0.5,	// Attraction/repultion (speed)
	.α1 = 0.08,
	.β0 = 0.1,	// Attraction/repultion (angle)
	.β1 = 0.08
};


// ----------------------------------- [ Functions ] ---------------------------------------- //


template<typename ...T>
void error(const char* format, T... args){
	printf(ANSI_BOLD "%s: " ANSI_RED "error: " ANSI_RESET, CLI::programName.c_str());
	printf(format, args...);
}


void printHelp(){
	printf(help, CLI::programName.c_str());
}


// ----------------------------------- [ Functions ] ---------------------------------------- //


void initRandom(vector<unique_ptr<Boid>>& boids, BoidParam& b, const Vec2& start, const Vec2& end){
	mt19937 gen(numbers::pi);
	uniform_real_distribution<> disX(start.x, end.x);
	uniform_real_distribution<> disY(start.y, end.y);
	for (int i = 0; i < b.count; i++){
		real x = disX(gen);
		real y = disY(gen);
		boids.emplace_back(new Boid(x, y, b.size, b.color, b.prefSpeed, b.maxSpeed));
	}
}

void writeProjectionField(ostream& out, const ProjectionField& field, bool colors = true){
	if (colors){
		const Interval& j = field.at(0);
		out << j.start << "," << j.end << "," << j.color;
		for (const Interval& i : field){
			out << "," << i.start << "," << i.end << "," << i.color;
			// out.write((const char*)(&i.start), sizeof(i.start));
			// out.write((const char*)(&i.end), sizeof(i.end));
			// real color = i.color;
			// out.write((const char*)(&color), sizeof(color));
		}
	} else {
		for (const Interval& i : field){
			out << i.start << "," << i.end;
			// out.write((const char*)(&i.start), sizeof(i.start));
			// out.write((const char*)(&i.end), sizeof(i.end));
		}
	}
}

void writeState(ostream& out, ostream& ovisual, const vector<unique_ptr<Boid>>& state){
	if (state.size() == 0) return;

	const Boid& b = *state[0];
	out << b.pos.x << " " << b.pos.y;
	writeProjectionField(ovisual, b.view, true);
	for (int i = 1; i < state.size(); i++){
		const Boid& b = *state[i];
		out << " " << b.pos.x << " " << b.pos.y;
		ovisual << " ";
		writeProjectionField(ovisual, b.view, true);
	}
	out << '\n';
	ovisual << '\n';
}


// ----------------------------------- [ Functions ] ---------------------------------------- //


void runSim(const int step_count, BoidParameters& boidParams, SimulationParameters& simParams, ostream& out, ostream& ovisual){
	vector<unique_ptr<Boid>> boids;

	if (boidParams.size() == 0){
		error("Missing boid types!\n");
		return;
	}


	cout << boidParams.size();
	for (int i = 0; i < boidParams.size(); i++){
		BoidParam& b = boidParams[i];
		if (i == boidParams.size() - 1)
			initRandom(boids, b, {100.0, 0}, {105.0, 5.0});
		else
			initRandom(boids, b, {0, 0}, {5.0, 5});
		out << b.count;
		if (i < boidParams.size() - 1)
			out << ' ';
	}
	out << '\n';

	// Output simulation states
	const int N = step_count;
	if (simParams.empty()){
		for (int i = 0; i < N; i++) {
			boids = simulationStep(defaultSimulationParameters, boids);
			writeState(out, ovisual, boids);
		}
	} else {
		for (int i = 0; i < N; i++) {
			boids = simulationStep(simParams, boids);
			writeState(out, ovisual, boids);
		}
	}
}


// ----------------------------------- [ Functions ] ---------------------------------------- //


// void writeProjectionField(ostream& out, const ProjectionField& field, bool colors = true){
// 	if (colors){
// 		for (const Interval& i : field){
// 			out.write((const char*)(&i.start), sizeof(i.start));
// 			out.write((const char*)(&i.end), sizeof(i.end));
// 			real color = i.color;
// 			out.write((const char*)(&color), sizeof(color));
// 		}
// 	} else {
// 		for (const Interval& i : field){
// 			out.write((const char*)(&i.start), sizeof(i.start));
// 			out.write((const char*)(&i.end), sizeof(i.end));
// 		}
// 	}
// }

// void coloredProjectionTest(ostream& out){
// 	ColoredProjectionField p = {};
// 	p.emplace_back(RAD(50),  RAD(100),  10, 1);
// 	p.emplace_back(RAD(150), RAD(200), -10, 2);
// 	p.emplace_back(RAD(250), RAD(300),  10, 3);
	
	
// 	printf("P: \n");
// 	for (const ColoredInterval& i : p){
// 		printf("  [%5.1f, %5.1f]  %d\n", DEG(i.start), DEG(i.end), i.color);
// 	}
	
	
// 	insertInterval(p, {RAD(90), RAD(110), 5, 0});
// 	insertInterval(p, {RAD(140), RAD(201), 5, 0});
// 	insertInterval(p, {RAD(280), RAD(290), 5, 0});
	
	
// 	printf("P: \n");
// 	for (const ColoredInterval& i : p){
// 		printf("  [%5.1f, %5.1f]  %d\n", DEG(i.start), DEG(i.end), i.color);
// 	}
	
// 	printProjectionField(out, p);
// }


void runSpeedTest(const int step_count, BoidParameters& boidParams, SimulationParameters& simParams, ostream& out, ostream& ovisual){
	const time_point t = system_clock::now();
	{
		runSim(CLI::stepCount, boidParams, simParams, out, ovisual);
	}
	const duration<float,milli> dt = (system_clock::now() - t);
	printf("Time: %.3fms\n", dt.count());
}


// ----------------------------------- [ Functions ] ---------------------------------------- //


unique_ptr<ostream> openOutput(const string& path){
	if (path.empty())
		return make_unique<ofstream>();
	else if (path == "-")
		return make_unique<ostream>(cout.rdbuf());
	else
		return make_unique<ofstream>(path);
}


bool parseSIMParams(const vector<string>& args, SimulationParameters& out_params){
	ParamKey key;
	SimParam val;
	bool status = true;
	
	for (const string& s : args){
		if (parseParameter(s, key, val)){
			out_params.emplace(key, val);
		} else {
			error("Failed to parse simulation parameter '" ANSI_YELLOW "%s" ANSI_RESET "'.\n", s.c_str());
			status = false;
		}
	}
	
	return status;
}


bool parseBOIDParams(const vector<string>& args, BoidParameters& out_params){
	BoidParam boid;
	bool status = true;
	
	for (const string& s : args){
		if (parseParameter(s, boid)){
			out_params.emplace_back(boid);
		} else {
			error("Failed to parse boid parameter '" ANSI_YELLOW "%s" ANSI_RESET "'.\n", s.c_str());
			status = false;
		}
	}
	
	return status;
}


// --------------------------------- [ Main Function ] -------------------------------------- //


int main(int argc, char const* const* argv){
	try {
		CLI::parse(argc, argv);
	} catch (const exception& e){
		error("%s\n", e.what());
		return 1;
	}
	
	if (CLI::help){
		printHelp();
		return 0;
	}
	
	unique_ptr<ostream> out = openOutput(CLI::output);
	unique_ptr<ostream> out_visual = openOutput(CLI::output + ".p");

	SimulationParameters simParams = {};
	BoidParameters boidParams = {};
	
	if (!parseSIMParams(CLI::colors, simParams)){
		return 1;
	} else if (!parseBOIDParams(CLI::boids, boidParams)){
		return 1;
	}
	
	try {
		runSim(CLI::stepCount, boidParams, simParams, *out, *out_visual);
	} catch (const exception& e){
		error("%s\n", e.what());
		return 1;
	}
	
	return 0;
}


// ------------------------------------------------------------------------------------------ //
