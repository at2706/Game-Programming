#pragma once
#include "Utilities.h"
class Vector
{
public:
	Vector();
	Vector(float x, float y, float z);
	~Vector();

	float dotProduct(Vector v2);
	float length() const;
	void normalize();
	float x;
	float y;
	float z;
};