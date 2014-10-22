#include "Matrix.h"

Matrix::Matrix(){
	identity();
}

Matrix::~Matrix(){
}

void Matrix::identity(){
	for (GLuint y = 0; y < MATRIX_SIZE; ++y)
		for (GLuint x = 0; x < MATRIX_SIZE; ++x){
			if(y != x) m[y][x] = 0;
			else m[y][x] = 1;
		}
}

Matrix Matrix::inverse(){
	return *this;
}

Matrix Matrix::operator*(const Matrix &m2){
	Matrix product;

	for (int y = 0; y < MATRIX_SIZE; y++)
		for (int x = 0; x < MATRIX_SIZE; x++){
			product.m[y][x] = 0;
			for (int z = 0; z < MATRIX_SIZE; z++)
				product.m[y][x] += m[y][z] * m2.m[z][x];
		}

	return product;
}

Vector Matrix::operator*(const Vector &v2){
	Vector product(0, 0, 0);

	product.x = m[0][0] * v2.x + m[0][1] * v2.y + m[0][2] * v2.z + m[0][3];
	product.y = m[1][0] * v2.x + m[1][1] * v2.y + m[1][2] * v2.z + m[1][3];
	product.z = m[2][0] * v2.x + m[2][1] * v2.y + m[2][2] * v2.z + m[2][3];

	return product;
}