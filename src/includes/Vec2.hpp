#pragma once
#include <cmath>


// ----------------------------------- [ Structures ] --------------------------------------- //


typedef float real;


struct Vec2 {
	real x;
	real y;
};


// ----------------------------------- [ Functions ] ---------------------------------------- //


inline real distance(const Vec2& a, const Vec2& b){
	const real x = b.x - a.x;
	const real y = b.y - a.y;
	return sqrt(x*x + y*y);
}


// ----------------------------------- [ Operators ] ---------------------------------------- //


inline Vec2 operator+(const Vec2& a, const Vec2& b){
	return {a.x + b.x, a.y + b.y};
}


inline Vec2 operator-(const Vec2& a, const Vec2& b){
	return {a.x - b.x, a.y - b.y};
}


inline Vec2 operator*(const Vec2& a, real n){
	return {a.x * n, a.x * n};
}


inline Vec2 operator/(const Vec2& a, real n){
	return {a.x / n, a.x / n};
}


// ----------------------------------- [ Operators ] ---------------------------------------- //


inline Vec2& operator+=(Vec2& a, const Vec2& b){
	a.x += b.x;
	a.y += b.y;
	return a;
}


inline Vec2& operator-=(Vec2& a, const Vec2& b){
	a.x -= b.x;
	a.y -= b.y;
	return a;
}


inline Vec2& operator*=(Vec2& a, real n){
	a.x *= n;
	a.y *= n;
	return a;
}


inline Vec2& operator/=(Vec2& a, real n){
	a.x /= n;
	a.y /= n;
	return a;
}


// ------------------------------------------------------------------------------------------ //