#pragma once
#include "Utilities.h"
class Vector
{
public:
	Vector();
	Vector(float x, float y, float z);
	~Vector();

	float length();
	void normalize();
	float x;
	float y;
	float z;

	float operator * (const Vector &v2);
};
