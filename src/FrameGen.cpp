#include "FrameGen.hpp"
#include <cmath>

#include "Image.hpp"
#include "Boid.hpp"

#include <chrono>
using namespace std::chrono;

using namespace std;


// ----------------------------------- [ Constants ] ---------------------------------------- //


static const float antiAlias = 2; // px


const array<rgba,3> colors = {
	rgba(255,   0,   0, 255),
	rgba(  0, 255,   0, 255),
	rgba(  0,   0, 255, 255),
};


// ----------------------------------- [ Functions ] ---------------------------------------- //


rgba blend(rgba bg, rgba fg){
	if (fg.a == 0){
		return bg;
	}
	
	float bg_a = bg.a / 255.0f;
	float fg_a = fg.a / 255.0f;
	float a = fg_a + bg_a*(1 - fg_a);

	float fg_mul = fg_a / a;
	float bg_mul = (bg_a * (1 - fg_a)) / a;
		
	float r = (fg.r * fg_mul) + (bg.r * bg_mul);
	float g = (fg.g * fg_mul) + (bg.g * bg_mul);
	float b = (fg.b * fg_mul) + (bg.b * bg_mul);
	
	return rgba((uint8_t)r, (uint8_t)g, (uint8_t)b, (uint8_t)(a*255));
}


// Not fast enough
/*
rgba blend(rgba bg, rgba fg){
	if (fg.a == 0){
		return bg;
	}
	
	const int p = 16384; // max sqrt(2^31)/2 = 23'170
	
	const int fg_a = (fg.a * p) / 255;
	const int bg_a = (bg.a * p) / 255;
	const int a    = fg_a + bg_a*(p - fg_a)/p;
	
	const int fg_r = (fg.r * p) / 255;
	const int bg_r = (fg.r * p) / 255;
	const int r = (((fg_r * fg_a) + (((bg_r * bg_a) / p) * (p - fg_a))) / a) * 255 / p;
	
	const int fg_g = (fg.g * p) / 255;
	const int bg_g = (fg.g * p) / 255;
	const int g = (((fg_g * fg_a) + (((bg_g * bg_a) / p) * (p - fg_a))) / a) * 255 / p;
	
	const int fg_b = (fg.b * p) / 255;
	const int bg_b = (fg.b * p) / 255;
	const int b = (((fg_b * fg_a) + (((bg_b * bg_a) / p) * (p - fg_a))) / a) * 255 / p;
	
	return rgba(r, g, b, (a * 255)/p);
}
*/


// ----------------------------------- [ Functions ] ---------------------------------------- //


void draw(const Boid& boid, Image& img, int scale, rgba color = colors[0]){
	const float px = boid.pos.x * scale;
	const float py = boid.pos.y * scale;
	const float r  = boid.size * scale;
	const float rr = r*r;
	const float sr = r + antiAlias;
	const float zr = sr - r;
	
	int x_start = max((int)floor(px - sr - 1), 0);
	int x_end   = min((int)ceil( px + sr + 1), img.w);
	int y_start = max((int)floor(py - sr - 1), 0);
	int y_end   = min((int)ceil( py + sr + 1), img.h);
	
	for (int y = y_start ; y < y_end ; y++){
	for (int x = x_start ; x < x_end ; x++){
		float dx = px - x;
		float dy = py - y;
		float dd = (dx*dx + dy*dy);
		
		// Fill
		if (dd <= rr){
			img.px(x,y) = color;
		}
		
		// Anti aliasing
		else {
			float alpha = 1 - (sqrt(dd) - r) / zr;
			
			if (0 < alpha && alpha <= 1.0f){
				rgba& px = img.px(x,y);
				px = blend(px, rgba(color.r, color.g, color.b, color.a * alpha));
			}
			
		}
		
	}}
}


// ----------------------------------- [ Functions ] ---------------------------------------- //


void generateFrame(const vector<unique_ptr<Boid>>& data, Image& img, int scale){
	img.clear({0,0,0,0});
	
	for (int i = 0 ; i < data.size() ; i++){
		const rgba& color = colors[i % colors.size()];
		draw(*data[i], img, scale, color);
	}
	
	// Add white background
	for (int i = 0 ; i < img.size() ; i++){
		img[i] = blend(rgba(255,255,255,255), img[i]);
	}
	
}


// ------------------------------------------------------------------------------------------ //