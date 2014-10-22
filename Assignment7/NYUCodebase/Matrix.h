#pragma once
#include "Utilities.h"
#include "Vector.h"
#define MATRIX_SIZE 4
class Matrix
{
public:
	Matrix();
	~Matrix();
	
	union {
		float m[MATRIX_SIZE][MATRIX_SIZE];
		float ml[16];
	};
	void identity();
	Matrix inverse();
	Matrix operator * (const Matrix &m2);
	Vector operator * (const Vector &v2);
};
