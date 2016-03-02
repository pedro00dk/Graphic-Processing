#include "Camera.h"
#include "MatrixGenerator.h"
Camera::Camera()
{
	// Inicializa a matriz de parametros extrincecos
	extrinsec[0] = extrinsec[5] = extrinsec[10] = extrinsec[15] = 1;
	extrinsec[1] = extrinsec[2] = extrinsec[3] = extrinsec[4]
		= extrinsec[6] = extrinsec[7] = extrinsec[8] = extrinsec[9]
		= extrinsec[11] = extrinsec[12] = extrinsec[13] = extrinsec[14] = 0;
}


Camera::~Camera()
{
}

void Camera::translateDirector(GLfloat rate){

	// As coordenadas do vetor diretor da camera são 0, 0, 1
	GLfloat* trMatrix = MatrixGenerator::createTranslateMatrix(0 * rate, 0 * rate, 1 * rate);

	// matriz que salva a multiplicação
	GLfloat aux[16] = {
		0, 0, 0, 0,
		0, 0, 0, 0,
		0, 0, 0, 0,
		0, 0, 0, 0 };

	// Multiplica as matrizes
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			for (int k = 0; k < 4; k++)
			{
				aux[(i * 4) + j] += trMatrix[(i * 4) + k] * extrinsec[(k * 4) + j];
			}
		}
	}

	// Salva aux na matriz extrinseca
	for (int i = 0; i < 16; i++)
	{
		extrinsec[i] = aux[i];
	}

	// Apaga a matriz de translação
	delete trMatrix;
}

void Camera::translateNormal(GLfloat rate)
{
	// As coordenadas do vetor normal da camera são 1, 0, 0
	GLfloat* trMatrix = MatrixGenerator::createTranslateMatrix(1 * rate, 0 * rate, 0 * rate);

	// matriz que salva a multiplicação
	GLfloat aux[16] = {
		0, 0, 0, 0,
		0, 0, 0, 0,
		0, 0, 0, 0,
		0, 0, 0, 0 };

	// Multiplica as matrizes
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			for (int k = 0; k < 4; k++)
			{
				aux[(i * 4) + j] += trMatrix[(i * 4) + k] * extrinsec[(k * 4) + j];
			}
		}
	}

	// Salva aux na matriz extrinseca
	for (int i = 0; i < 16; i++)
	{
		extrinsec[i] = aux[i];
	}

	// Apaga a matriz de translação
	delete trMatrix;
}

void Camera::rotateNormalX(GLfloat degree)
{
	Point cameraVectorX;
	cameraVectorX.x = 1;
	cameraVectorX.y = 0;
	cameraVectorX.z = 0;

	// Pega a terceira coluna da matriz extrinseca (representa o eixo z da camera)
	GLfloat* trMatrix = MatrixGenerator::createRodriguesMatrix(cameraVectorX, degree);

	// matriz que salva a multiplicação
	GLfloat aux[16] = {
		0, 0, 0, 0,
		0, 0, 0, 0,
		0, 0, 0, 0,
		0, 0, 0, 0 };

	// Multiplica as matrizes
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			for (int k = 0; k < 4; k++)
			{
				aux[(i * 4) + j] += trMatrix[(i * 4) + k] * extrinsec[(k * 4) + j];
			}
		}
	}

	// Salva aux na matriz extrinseca
	for (int i = 0; i < 16; i++)
	{
		extrinsec[i] = aux[i];
	}

	// Apaga a matriz de translação
	delete trMatrix;
}

void Camera::rotateWorldY(GLfloat degree){
	Point cameraVectorY;
	cameraVectorY.x = extrinsec[1];
	cameraVectorY.y = extrinsec[5];
	cameraVectorY.z = extrinsec[9];

	// Pega a terceira coluna da matriz extrinseca (representa o eixo z da camera)
	GLfloat* trMatrix = MatrixGenerator::createRodriguesMatrix(cameraVectorY, degree);

	// matriz que salva a multiplicação
	GLfloat aux[16] = {
		0, 0, 0, 0,
		0, 0, 0, 0,
		0, 0, 0, 0,
		0, 0, 0, 0 };

	// Multiplica as matrizes
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			for (int k = 0; k < 4; k++)
			{
				aux[(i * 4) + j] += trMatrix[(i * 4) + k] * extrinsec[(k * 4) + j];
			}
		}
	}

	// Salva aux na matriz extrinseca
	for (int i = 0; i < 16; i++)
	{
		extrinsec[i] = aux[i];
	}

	// Apaga a matriz de translação
	delete trMatrix;
}

GLfloat* Camera::getCameraMatrix()
{
	GLfloat* extrinsecCopy = (GLfloat*)malloc(sizeof(GLfloat)* 16);

	// copia a matriz transpondo-a ao mesmo tempo
	extrinsecCopy[0] = extrinsec[0];
	extrinsecCopy[4] = extrinsec[1];
	extrinsecCopy[8] = extrinsec[2];
	extrinsecCopy[12] = extrinsec[3];
	extrinsecCopy[1] = extrinsec[4];
	extrinsecCopy[5] = extrinsec[5];
	extrinsecCopy[9] = extrinsec[6];
	extrinsecCopy[13] = extrinsec[7];
	extrinsecCopy[2] = extrinsec[8];
	extrinsecCopy[6] = extrinsec[9];
	extrinsecCopy[10] = extrinsec[10];
	extrinsecCopy[14] = extrinsec[11];
	extrinsecCopy[3] = extrinsec[12];
	extrinsecCopy[7] = extrinsec[13];
	extrinsecCopy[11] = extrinsec[14];
	extrinsecCopy[15] = extrinsec[15];

	return extrinsecCopy;
}

