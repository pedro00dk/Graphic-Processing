#pragma once

// Permite o acesso as constantes de math
#define _USE_MATH_DEFINES

#include <iostream>
#include <cmath>
#include <gl/glut.h>
#include "Point.h"

// Constante que normaliza frequencia da onda de seno e conseno para o openGL
#define ROTATE_NORMALIZER (GLfloat) M_PI / 180

/*
Classe com metodos estaticos para gerar matrizes para transformações no openGL
*/
class MatrixGenerator
{
public:
	// Cria uma matriz de translação de acordo com os parametros
	static GLfloat* createTranslateMatrix(GLfloat x, GLfloat y, GLfloat z);

	// Cria matrizes que rotacionam nos eixos definidos nos metodos
	static GLfloat* createRotateXMatrix(GLfloat degree);
	static GLfloat* createRotateYMatrix(GLfloat degree);
	static GLfloat* createRotateZMatrix(GLfloat degree);

	// Cria uma matriz que rotaciona ao redor de um eixo definido (representado por um vetor)
	static GLfloat* createRodriguesMatrix(Point axis, GLfloat degree);
};

