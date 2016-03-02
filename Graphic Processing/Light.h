#pragma once

#include "MatrixGenerator.h"
#include <gl/glut.h>

// Define os limites minimos e maximos para dados RGB
#define MIN_RGB (GLfloat) 0
#define MAX_RGB (GLfloat) 1

/*
Classe que representa uma luz no openGL
*/
class Light
{
private:
	// Identificador da luz no openGL
	GLenum glLightID;

	// Dados da iluminação (Tamanho [3] -> [R][G][B])
	GLfloat* ambientLight;
	GLfloat* diffuseLight;
	GLfloat* specularLight;

	// Posição atual da luz (Tamanho [3] -> [X][Y][Z])
	GLfloat* position;

	// Verifica se dados RGB e de brilho são validos
	bool verifyRGB(GLfloat* valueRGB);

	// Metodo chamado plos contrutores já que não é possivel char um dentro de outro
	void init(GLint lightID, GLfloat* ambientLight, GLfloat* diffuseLight, GLfloat* specularLight, GLfloat* position);
public:
	//Cria uma nova fonte luz com a cor branca na origem do sistema
	Light(GLint lightID);

	//Cria uma nova fonte luz com a cor branca na posição passada
	Light(GLint lightID, GLfloat* position);

	// Cria uma nova fonte de luz (Parametros que são arrays todos com tamanho [3] -> [R][G][B])
	Light(GLint lightID, GLfloat* ambientLight, GLfloat* diffuseLight, GLfloat* specularLight, GLfloat* position);
	~Light();

	// Metodos que mudam os parametros da luz

	// Muda a cor do ambiente
	void changeAmbient(GLfloat* ambientLight); // ambientLight[3] -> [R][G][B]

	// Muda a cor do componente difuso da luz
	void changeDiffuse(GLfloat* diffuseLight); // diffuzeLight[3] -> [R][G][B]

	// Muda a cor do componente especular da luz
	void changeSpecular(GLfloat* specularLight); // specularLight[3] -> [R][G][B]

	// Muda a posição da luz
	void changePosition(GLfloat* position); // postion [3] -> [X][Y][Z]

	// Seta a luz para branco
	void setLightToWhite();

	// Habilita ou desabilita a luz (a luz começa desabilitada)
	void enableLight();
	void disableLight();

	// Translada nos eixos definidos (3 parametros [x][y][z])
	void translate(GLfloat* translation);

	// Rotaciona a luz nos eixos correspondentes
	void rotateX(GLfloat degree);
	void rotateY(GLfloat degree);
	void rotateZ(GLfloat degree);
};

