#include "Vector.h"

Vector::Vector()
{
}

Vector::Vector(float x, float y, float z)
	: x(x), y(y), z(z){
}

Vector::~Vector(){
}

float Vector::dotProduct(Vector v2){
	return (x * v2.x) + (y * v2.y);
}

float Vector::length(){
	return sqrt(x*x + y*y + z*z);
}

void Vector::normalize(){
	x /= length();
	y /= length();
	z /= length();
}

float Vector::operator*(const Vector &v2){
	return (x * v2.x) + (y * v2.y);
}