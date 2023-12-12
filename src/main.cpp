#include <iostream>
#include <cmath>

#include "ANSI.h"
// #include "Image.hpp"
// #include "ffmpeg.hpp"
#include "Boid.hpp"
#include "ProjectionField.hpp"
#include "Simulation.hpp"


using namespace std;


// ----------------------------------- [ Constants ] ---------------------------------------- //


constexpr double deg_to_rad = M_PI/180.0;
constexpr double rad_to_deg = 180.0/M_PI;

#define RAD(ϕ)	(ϕ*deg_to_rad)
#define DEG(ϕ)	(ϕ*rad_to_deg)


// ----------------------------------- [ Functions ] ---------------------------------------- //


// int fftest(){
// 	Image img = Image(360,360);
	
// 	// Red image
// 	for (int i = 0 ; i < (img.w*img.h) ; i++){
// 		img[i].r = 255;
// 	}
	
// 	int err = ffEncode(img, "bin/img.png", true);
// 	if (err != 0){
// 		cerr << ANSI_RED "Failed to encode image using ffmpeg." ANSI_RESET "\n";
// 		exit(err);
// 	}
	
// 	return 0;
// }


// ----------------------------------- [ Functions ] ---------------------------------------- //


// void f(Boid a, Boid b){
// 	Interval i = a.getProjection(b);
// 	printf("(%2.0f,%2.0f):  [%6.1f, %6.1f]\n", b.pos.x, b.pos.y, i.start, i.end);
// }


// void projectionFieldExample(){
// 	vector<Interval> p = {};
	
// 	insertInterval(p, {RAD(0),RAD(10)});
// 	insertInterval(p, {RAD(20),RAD(30)});
// 	insertInterval(p, {RAD(40),RAD(50)});
// 	insertInterval(p, {RAD(60),RAD(70)});
// 	insertInterval(p, {RAD(80),RAD(90)});
// 	insertInterval(p, {RAD(150),RAD(160)});
// 	insertInterval(p, {RAD(200),RAD(250)});
	
// 	printf("P: \n");
// 	for (const Interval& i : p){
// 		printf("  [%.1f, %.1f]\n", DEG(i.start), DEG(i.end));
// 	}
	
// 	printf("\n");
// 	printf(ANSI_GREEN "insert [%d, %d]\n" ANSI_RESET, -45, 45);
// 	insertInterval(p, {RAD(-45), RAD(45)});
// 	printf("\n");
	
// 	printf("P: \n");
// 	for (const Interval& i : p){
// 		printf("  [%.1f, %.1f]\n", DEG(i.start), DEG(i.end));
// 	}
	
// 	printf("\n");
// 	printf(ANSI_GREEN "insert [%d, %d]\n" ANSI_RESET, 180, 199);
// 	insertInterval(p, {RAD(180), RAD(199)});
// 	printf("\n");
	
// 	printf("P: \n");
// 	for (const Interval& i : p){
// 		printf("  [%.1f, %.1f]\n", DEG(i.start), DEG(i.end));
// 	}
	
// 	printf("\n");
// 	printf(ANSI_GREEN "insert [%d, %d]\n" ANSI_RESET, -360, 360);
// 	insertInterval(p, {RAD(-360), RAD(360)});
// 	printf("\n");
	
// 	printf("P: \n");
// 	for (const Interval& i : p){
// 		printf("  [%.1f, %.1f]\n", DEG(i.start), DEG(i.end));
// 	}
	
// }


// ----------------------------------- [ Functions ] ---------------------------------------- //


void simTest(){
	vector<unique_ptr<Boid>> f0 = {};
	f0.emplace_back(new Boid(0,0));
	f0.emplace_back(new Boid(1,0));
	
	vector<unique_ptr<Boid>> f1 = simulationStep(f0);
}


// --------------------------------- [ Main Function ] -------------------------------------- //


int main(int argc, char const* const* argv){
	printf("================================\n");
	
	// projectionFieldExample();
	simTest();
	
	return 0;
}


// ------------------------------------------------------------------------------------------ //
