#pragma once

#include <vector>
#include "Coord.h"
#include "Point.h"

/*
Estrutura que armazena os dados de uma face, seus vertices, normais e coordenadas
*/
struct Face {
	std::vector<int> vertices;
	std::vector<int> normals;
	std::vector<int> textures;

	// Indica se existem textures para todos os vertices
	bool hasVerticesTextures;
};