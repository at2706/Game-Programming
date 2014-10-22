#pragma once
#include "Utilities.h"
#include "Vector.h"
class Matrix
{
public:
	Matrix();
	~Matrix();
	
	union {
		float m[4][4];
		float ml[16];
	};
	void identity();
	Matrix inverse();
	Matrix operator * (const Matrix &m2);
	Vector operator * (const Vector &v2);
};
