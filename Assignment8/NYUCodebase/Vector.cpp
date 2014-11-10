#include "Vector.h"

Vector::Vector(){
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
}

Vector::Vector(float x, float y, float z)
	: x(x), y(y), z(z){
}

Vector::~Vector(){
}

float Vector::dotProduct(Vector v2){
	return (x * v2.x) + (y * v2.y);
}

float Vector::length() const{
	return sqrt(x*x + y*y + z*z);
}

void Vector::normalize(){
	x /= length();
	y /= length();
	z /= length();
}
