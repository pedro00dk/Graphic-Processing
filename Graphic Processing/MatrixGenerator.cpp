#include "MatrixGenerator.h"

GLfloat* MatrixGenerator::createTranslateMatrix(GLfloat x, GLfloat y, GLfloat z)
{
	GLfloat* r = (GLfloat*)malloc(sizeof(GLfloat)* 16);

	r[0] = 1; r[1] = 0; r[2] = 0; r[3] = x;
	r[4] = 0; r[5] = 1; r[6] = 0; r[7] = y;
	r[8] = 0; r[9] = 0; r[10] = 1; r[11] = z;
	r[12] = 0; r[13] = 0; r[14] = 0; r[15] = 1;

	return r;
}

GLfloat* MatrixGenerator::createRotateXMatrix(GLfloat degree)
{
	GLfloat sinT = sin(degree * ROTATE_NORMALIZER);
	GLfloat cosT = cos(degree * ROTATE_NORMALIZER);

	GLfloat* r = (GLfloat*)malloc(sizeof(GLfloat)* 16);

	r[0] = 1; r[1] = 0; r[2] = 0; r[3] = 0;
	r[4] = 0; r[5] = cosT; r[6] = -sinT; r[7] = 0;
	r[8] = 0; r[9] = sinT; r[10] = cosT; r[11] = 0;
	r[12] = 0; r[13] = 0; r[14] = 0; r[15] = 1;

	return r;
}

GLfloat* MatrixGenerator::createRotateYMatrix(GLfloat degree)
{
	GLfloat sinT = sin(degree * ROTATE_NORMALIZER);
	GLfloat cosT = cos(degree * ROTATE_NORMALIZER);

	GLfloat* r = (GLfloat*)malloc(sizeof(GLfloat)* 16);

	r[0] = cosT; r[1] = 0; r[2] = -sinT; r[3] = 0;
	r[4] = 0; r[5] = 1; r[6] = 0; r[7] = 0;
	r[8] = sinT; r[9] = 0; r[10] = cosT; r[11] = 0;
	r[12] = 0; r[13] = 0; r[14] = 0; r[15] = 1;

	return r;
}

GLfloat* MatrixGenerator::createRotateZMatrix(GLfloat degree)
{
	GLfloat sinT = sin(degree * ROTATE_NORMALIZER);
	GLfloat cosT = cos(degree * ROTATE_NORMALIZER);

	GLfloat* r = (GLfloat*)malloc(sizeof(GLfloat)* 16);

	r[0] = cosT; r[1] = -sinT; r[2] = 0; r[3] = 0;
	r[4] = sinT; r[5] = cosT; r[6] = 0; r[7] = 0;
	r[8] = 0; r[9] = 0; r[10] = 1; r[11] = 0;
	r[12] = 0; r[13] = 0; r[14] = 0; r[15] = 1;

	return r;
}

GLfloat* MatrixGenerator::createRodriguesMatrix(Point axis, GLfloat degree)
{
	GLfloat sinT = sin(degree * ROTATE_NORMALIZER);
	GLfloat cosT = cos(degree * ROTATE_NORMALIZER);

	GLfloat* r = (GLfloat*)malloc(sizeof(GLfloat)* 16);

	r[0] = cosT + (pow(axis.x, 2) * (1 - cosT));
	r[1] = (axis.x * axis.y * (1 - cosT) - (axis.z * sinT));
	r[2] = (axis.y * sinT) + (axis.x * axis.z * (1 - cosT));
	r[3] = 0;

	r[4] = (axis.z * sinT) + (axis.x * axis.y * (1 - cosT));
	r[5] = cosT + (pow(axis.y, 2) * (1 - cosT));;
	r[6] = (-axis.x * sinT) + (axis.y * axis.z * (1 - cosT));
	r[7] = 0;

	r[8] = (-axis.y * sinT) + (axis.x * axis.z * (1 - cosT));
	r[9] = (axis.x * sinT) + (axis.y * axis.z * (1 - cosT));
	r[10] = cosT + (pow(axis.z, 2) * (1 - cosT));;;
	r[11] = 0;

	r[12] = 0;
	r[13] = 0;
	r[14] = 0;
	r[15] = 1;

	return r;
}