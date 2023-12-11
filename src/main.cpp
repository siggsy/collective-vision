#include <iostream>

#include "ANSI.h"
#include "Image.hpp"
#include "ffmpeg.hpp"
#include "Boid.hpp"
#include "ProjectionField.hpp"

#include <cmath>
#include <algorithm>


using namespace std;


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


void f(Boid a, Boid b){
	Interval i = getProjection(a, b);
	printf("(%2.0f,%2.0f):  [%6.1f, %6.1f]\n", b.pos.x, b.pos.y, i.start, i.end);
}


void projectionExample(){
	vector<Interval> p = {};
	
	insertInterval(p, {0,10});
	insertInterval(p, {20,30});
	insertInterval(p, {40,50});
	insertInterval(p, {60,70});
	insertInterval(p, {80,90});
	insertInterval(p, {150,160});
	insertInterval(p, {200,250});
	
	printf("P: \n");
	for (const Interval& i : p){
		printf("  [%.0f, %.0f]\n", i.start, i.end);
	}
	
	printf("\n");
	printf(ANSI_GREEN "insert [%d, %d]\n" ANSI_RESET, -45, 45);
	insertInterval(p, {-45,45});
	printf("\n");
	
	printf("P: \n");
	for (const Interval& i : p){
		printf("  [%.0f, %.0f]\n", i.start, i.end);
	}
	
	printf("\n");
	printf(ANSI_GREEN "insert [%d, %d]\n" ANSI_RESET, 180, 200);
	insertInterval(p, {180,200});
	printf("\n");
	
	printf("P: \n");
	for (const Interval& i : p){
		printf("  [%.0f, %.0f]\n", i.start, i.end);
	}
	
	printf("\n");
	printf(ANSI_GREEN "insert [%d, %d]\n" ANSI_RESET, -360, 360);
	insertInterval(p, {-360,360});
	printf("\n");
	
	printf("P: \n");
	for (const Interval& i : p){
		printf("  [%.0f, %.0f]\n", i.start, i.end);
	}
	
}



// --------------------------------- [ Main Function ] -------------------------------------- //


int main(int argc, char const* const* argv){
	printf("================================\n");
	projectionExample();
	return 0;
}


// ------------------------------------------------------------------------------------------ //
