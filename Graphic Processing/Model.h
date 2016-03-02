#pragma once

// Permite o usao de funções C não seguras
#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <fstream>
#include <vector>
#include <gl/glut.h>
#include "Coord.h"
#include "Point.h"
#include "Face.h"
#include "MatrixGenerator.h"

// Define os limites minimos e maximos para dados RGB
#define MIN_RGB (GLfloat) 0
#define MAX_RGB (GLfloat) 1

// Define os limites minimos e maximos para o brilho
#define MIN_SHININESS (GLfloat) 0
#define MAX_SHININESS (GLfloat) 128

using namespace std;

/*
Classe que armazena um modelo carregado de um arquivo obj
*/
class Model
{
private:
	// Vetores de vertices, normais e textruas do modelo
	vector<Point> vertices;
	vector<Point> normals;
	vector<Coord> textures;

	// Vetor de faces do modelo
	vector<Face> faces;

	// Dados sobre o material do modelo (Tamanho [3] -> [R][G][B])
	GLfloat* diffuseColor;
	GLfloat* specularColor;
	GLfloat* emissiveColor;

	// Dado sobre o brilho do objeto, influencia na especularidade (Tamanho [1] -> [S])
	GLfloat* shininess;

	// Metodos auxiliares que obtem um ponto, coordenada ou face de uma linha obj
	Point readPoint(char* line);
	Coord readCoord(char* line);
	Face readFace(char* line, vector<Point> vertices, vector<Point> normals, vector<Coord> textures);

	// Calcula as normais do modelo (se o mesmo não possuir)
	void calculateVerticesNormals();

	// Verifica se dados RGB e de brilho são validos
	bool verifyRGB(GLfloat* valueRGB);
	bool verifyShininess(GLfloat* shininess);

public:
	// Construtor recebe caminho para arquivo obj (Parametros que são arrays todos com tamanho [3] -> [R][G][B]) exceto shininess (Tamanho [1] -> [S])
	Model(char* objPath, GLfloat* diffuseColor, GLfloat* specularColor, GLfloat* emissiveColor, GLfloat* shininess);

	~Model();

	// Muda a cor difusa do modelo
	void changeDiffuseColor(GLfloat* diffuseColor); // diffuseColor[3] -> [R][G][B]

	// Muda a cor especular do modelo
	void changeSpecularColor(GLfloat* specularColor); // specularColor[3] -> [R][G][B]

	// Muda de emissão do modelo
	void changeEmissiveColor(GLfloat* emissiveColor); // emissiveColor[3] -> [R][G][B]

	// Muda a taxa de brilho do objeto
	void changeShininess(GLfloat* shininess);

	// Seta os dados do material para em branco
	void setMaterialToBlank();

	// Metodo que redenriza o objeto (de acordo com os parametros previamente setados)
	void render();

	// Renderisa o objeto como se estivesse selecionado
	void selectRender();

	// Translada nos eixos definidos (3 parametros [x][y][z])
	void translate(GLfloat* translation);

	// Rotaciona a luz nos eixos correspondentes
	void rotateX(GLfloat degree);
	void rotateY(GLfloat degree);
	void rotateZ(GLfloat degree);

	// Faz a escala no modelo de acordo com a taxa recebida
	void scale(GLfloat rate);
};