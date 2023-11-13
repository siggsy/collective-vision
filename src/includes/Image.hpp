#pragma once
#include <cstdint>
#include <vector>


struct rgba {
	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t a;
};


class Image {
// ------------------------------------[ Properties ] --------------------------------------- //
public:
	int w;
	int h;
	std::vector<rgba> px;
	
// ---------------------------------- [ Constructors ] -------------------------------------- //
public:
	Image(int w, int h) : w{w}, h{h} {
		px.reserve(w*h);
		px.resize(w*h, {0,0,0,255});
	}
	
// ----------------------------------- [ Functions ] ---------------------------------------- //
public:
	inline const uint8_t* raw() const {
		return (const uint8_t*)px.data();
	}
	
	inline size_t rawSize() const {
		return px.size() * sizeof(*px.data());
	}
	
// ----------------------------------- [ Operators ] ---------------------------------------- //
public:
	inline rgba& operator[](int i){
		return px[i];
	}
	
// ------------------------------------------------------------------------------------------ //
};