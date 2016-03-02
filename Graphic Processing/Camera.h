#pragma once

#include <iostream>
#include <GL/glut.h>
#include "Point.h"

using namespace std;

/*
Classe que faz as transforma��es na camera, rota��es e transla��es, para que funciona corretamente,
todas as fun��es devem ser chamadas dessa classe
*/
class Camera
{
private:
	// Parametros extrinsecos da camera
	GLfloat extrinsec[16];

public:
	// Construtor n�o recebe nada
	Camera();
	~Camera();

	// Translada no sentido do vetor diretor da camera
	void translateDirector(GLfloat rate);

	// Translada no vetor normal a camera
	void translateNormal(GLfloat rate);

	// Rotaciona no eixo normal X da camera
	void rotateNormalX(GLfloat degree);

	// Rotaciona no eixo do vetor Y do mundo
	void rotateWorldY(GLfloat degree);

	// Retorna uma copia da matriz de parametros extrinsecos
	GLfloat* getCameraMatrix();
};