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
	std::vector<rgba> data;
	
// ---------------------------------- [ Constructors ] -------------------------------------- //
public:
	Image(int w, int h) : w{w}, h{h} {
		data.reserve(w*h);
		data.resize(w*h, rgba(255,255,255,255));
	}
	
// ----------------------------------- [ Functions ] ---------------------------------------- //
public:
	inline rgba& px(int x, int y){
		return data[x + w*y];
	}
	
	inline const rgba& px(int x, int y) const {
		return data[x + w*y];
	}
	
public:
	inline uint8_t* raw(){
		return (uint8_t*)data.data();
	}
	
	inline const uint8_t* raw() const {
		return (const uint8_t*)data.data();
	}
	
public:
	inline int size() const {
		return data.size();
	} 
	
	inline size_t rawSize() const {
		return data.size() * sizeof(*data.data());
	}
	
// ----------------------------------- [ Functions ] ---------------------------------------- //
public:
	inline void clear(rgba color = rgba(255,255,255,255)){
		data.clear();
		data.resize(w*h, color);
	}
	
// ----------------------------------- [ Operators ] ---------------------------------------- //
public:
	inline rgba& operator[](int i){
		return data[i];
	}
	
// ------------------------------------------------------------------------------------------ //
};