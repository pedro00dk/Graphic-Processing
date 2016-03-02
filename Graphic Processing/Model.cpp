#include "Model.h"

Point Model::readPoint(char* line)
{
	// Cria um novo ponto
	Point p;

	// Lê a linha ingorando o primeiro token
	sscanf(line, "%*s %f %f %f", &p.x, &p.y, &p.z);

	// Retorna o ponto criado
	return p;
}

Coord Model::readCoord(char* line)
{
	// Cria uma nova coordenada
	Coord c;

	// Lê a linha ingorando o primeiro token
	sscanf(line, "%*s %f %f", &c.x, &c.y);

	// Retorna o ponto criado
	return c;
}

Face Model::readFace(char* line, vector<Point> vertices,
	vector<Point> normals, vector<Coord> textures)
{
	// Cria a nova face
	Face f;

	// Tokeniza a string da linha
	char* facetok = strtok(line, " ");
	// Ignora o primeiro token que é "f"
	facetok = strtok(NULL, " ");

	// Verifica o tipo usando o segundo token
	// Verifica o tipo de vertices na face (1- Vertice 2-Vertice//Normal 3-Vertice/Textura/Normal), começa com o tipo 1
	int faceType = 1;
	// Tamanho do token
	int toksize = strlen(facetok);
	for (int i = 0; i < toksize - 1; i++) // Varre os caracteres do token
	{
		if (facetok[i] == '/') // Verifica se é uma barra
		{
			if (facetok[i + 1] == '/') // Caso hajam duas barras coladas, o tipo é 2
			{
				faceType = 2;
			}
			else // Caso contrario, o tipo é 3
			{
				faceType = 3;
			}
			// Para o laço
			i = toksize;
		}
	}

	// Inteiros que guardam indices doas dados da face
	int iVertex;
	int iTexture;
	int iNormal;

	// Varre enquanto houverem tokens
	while (facetok != NULL)
	{
		// Trata o token de uma maneira diferente de acordo com o tipo
		switch (faceType)
		{
			// Subtrai 1 de todos os valores porque o obj começa a contar de 1
		case 1:
			sscanf(facetok, "%d", &iVertex);
			f.vertices.push_back(iVertex - 1);
			break;
		case 2:
			sscanf(facetok, "%d//%d", &iVertex, &iNormal);
			f.vertices.push_back(iVertex - 1);
			f.normals.push_back(iNormal - 1);
			break;
		case 3:
			sscanf(facetok, "%d/%d/%d", &iVertex, &iTexture, &iNormal);
			f.vertices.push_back(iVertex - 1);
			f.textures.push_back(iTexture - 1);
			f.normals.push_back(iNormal - 1);
			break;
		}
		// Obtem o proximo token
		facetok = strtok(NULL, " ");
	}

	// De acordo com o tipo, indica se a face tem textures e normais para os vertices
	switch (faceType)
	{
	case 1:
	case 2:
		f.hasVerticesTextures = false;
		break;
	case 3:
		f.hasVerticesTextures = true;
		break;
	}

	// Rentorna a face criada
	return f;
}

void Model::calculateVerticesNormals()
{
	// Limpa o vetor dew normais
	normals.clear();

	// Cria normais nulas para todos os vertices
	for (int i = 0; i < vertices.size(); i++)
	{
		Point p;
		p.x = p.y = p.z = 0;
		normals.push_back(p);
	}

	// Para todas as faces seta o indice da normal da face igual ao do vertice da face
	for (int i = 0; i < faces.size(); i++)
	{
		for (int j = 0; j < faces[i].vertices.size(); j++)
		{
			faces[i].normals.push_back(faces[i].vertices[j]);
		}
	}

	for (int i = 0; i < faces.size(); i++)
	{
		Point v1;
		v1.x = vertices[faces[i].vertices[0]].x - vertices[faces[i].vertices[1]].x;
		v1.y = vertices[faces[i].vertices[0]].y - vertices[faces[i].vertices[1]].y;
		v1.z = vertices[faces[i].vertices[0]].z - vertices[faces[i].vertices[1]].z;

		Point v2;
		v2.x = vertices[faces[i].vertices[2]].x - vertices[faces[i].vertices[1]].x;
		v2.y = vertices[faces[i].vertices[2]].y - vertices[faces[i].vertices[1]].y;
		v2.z = vertices[faces[i].vertices[2]].z - vertices[faces[i].vertices[1]].z;

		// Faz o produto vetorial e o retorna no formato de um ponto
		Point normal;
		normal.x = (v1.y * v2.z) - (v1.z * v2.y);
		normal.y = (v1.z * v2.x) - (v1.x * v2.z);
		normal.z = (v1.x * v2.y) - (v1.y * v2.x);

		for (int j = 0; j < faces[i].normals.size(); j++)
		{
			normals[faces[i].normals[j]].x += normal.x;
			normals[faces[i].normals[j]].y += normal.y;
			normals[faces[i].normals[j]].z += normal.z;
		}
	}

	// Normaliza as normais
	for (int i = 0; i < normals.size(); i++)
	{
		GLfloat norm = sqrt((normals[i].x * normals[i].x) + (normals[i].y * normals[i].y) + (normals[i].z * normals[i].z));
		cout << "alo" << endl;
		normals[i].x /= norm;
		normals[i].y /= norm;
		normals[i].z /= norm;
	}
}

bool Model::verifyRGB(GLfloat* valueRGB)
{
	return (valueRGB[0] >= MIN_RGB && valueRGB[0] <= MAX_RGB
		&& valueRGB[1] >= MIN_RGB && valueRGB[1] <= MAX_RGB
		&& valueRGB[2] >= MIN_RGB && valueRGB[2] <= MAX_RGB);
}

bool Model::verifyShininess(GLfloat* shininess)
{
	return (shininess[0] >= MIN_SHININESS
		&& shininess[0] <= MAX_SHININESS);
}

Model::Model(char* objPath, GLfloat* diffuseColor, GLfloat* specularColor, GLfloat* emissiveColor, GLfloat* shininess)
{
	// Cria o stream que lerá o objeto
	fstream reader = fstream();

	// Abre o arquivo no modo de leitura
	reader.open(objPath, fstream::in);

	// Verifica se o arquivo foi aberto corretamente
	if (!reader.is_open())
	{
		throw ("Failed to load");
	}

	// Salva os dados do material do modelo
	if (verifyRGB(diffuseColor) && verifyRGB(specularColor) && verifyRGB(emissiveColor))
	{
		Model::diffuseColor = diffuseColor;
		Model::specularColor = specularColor;
		Model::emissiveColor = emissiveColor;
	}

	// Salva dado sobre o brilho do modelo
	if (verifyShininess(shininess))
	{
		Model::shininess = shininess;
	}

	// Buffer que salva cada linha
	char buff[256];

	// Varre enquanto não chega ao fim do arquivo
	while (!reader.eof())
	{
		// Obtem uma linha do stream
		reader.getline(buff, sizeof(buff));

		if (strlen(buff) >= 2 && strncmp("v ", buff, 2) == 0) // Verifica se é uma linha de vertice
		{
			// Salva o ponto criado no vetor de vertices
			vertices.push_back(readPoint(buff));
		}
		else if (strlen(buff) >= 3 && strncmp("vn ", buff, 3) == 0) // Verifica se é uma linha de normal
		{
			// Salva o ponto criado no vetor de normais
			normals.push_back(readPoint(buff));
		}
		else if (strlen(buff) >= 3 && strncmp("vt ", buff, 3) == 0) // Verifica se é uma linha de testura
		{
			// Salva o ponto criado no vetor de normais
			textures.push_back(readCoord(buff));
		}
		else if (strlen(buff) >= 2 && strncmp("f ", buff, 2) == 0) // Verifica se é uma linha de face
		{
			// Salva a face criada no vetor de faces da classe
			faces.push_back(readFace(buff, vertices, normals, textures));
		}
	}

	// Verifica se existem faces no modelo
	if (faces.size() == 0)
	{
		throw ("Invalid obj");
	}

	// Verifica se existem normais no modelo, se não, calcula normais
	if (faces[0].normals.size() == 0)
	{
		// Calucla as normais dos vertices
		calculateVerticesNormals();
	}
}

Model::~Model()
{
}

void Model::changeDiffuseColor(GLfloat* diffuseColor)
{
	if (verifyRGB(diffuseColor))
	{
		Model::diffuseColor = diffuseColor;
	}
	else
	{
		throw ("Invalid data");
	}
}

void Model::changeSpecularColor(GLfloat* specularColor)
{
	if (verifyRGB(specularColor))
	{
		Model::specularColor = specularColor;
	}
	else
	{
		throw ("Invalid data");
	}
}

void Model::changeEmissiveColor(GLfloat* emissiveColor)
{
	if (verifyRGB(emissiveColor))
	{
		Model::emissiveColor = emissiveColor;
	}
	else
	{
		throw ("Invalid data");
	}
}

void Model::changeShininess(GLfloat* shininess)
{
	if (verifyShininess(shininess))
	{
		Model::shininess = shininess;
	}
	else
	{
		throw ("Invalid data");
	}
}

void Model::setMaterialToBlank()
{
	Model::diffuseColor[0] = 0.5;
	Model::diffuseColor[1] = 0.5;
	Model::diffuseColor[2] = 0.5;

	Model::specularColor[0] = 1;
	Model::specularColor[1] = 1;
	Model::specularColor[2] = 1;

	Model::emissiveColor[0] = 0.5;
	Model::emissiveColor[1] = 0.5;
	Model::emissiveColor[2] = 0.5;

	Model::shininess[0] = 32;
}

void Model::render()
{
	// Setando dados do material antes de renderizar
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuseColor);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specularColor);
	//glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emissiveColor);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shininess);

	// Desenha as faces
	for (Face f : faces) {
		glBegin(GL_POLYGON);
		for (int i = 0; i < f.vertices.size(); i++) {

			Point n = normals[f.normals[i]];
			glNormal3d(n.x, n.y, n.z);

			Point p = vertices[f.vertices[i]];
			glVertex3d(p.x, p.y, p.z);
		}
		glEnd();
	}
}

void Model::selectRender()
{
	// Setando dados do material antes de renderizar
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuseColor);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specularColor);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emissiveColor);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shininess);

	glEnable(GL_LIGHTING);

	// Desenha as faces
	for (Face f : faces) {
		glBegin(GL_POLYGON);
		for (int i = 0; i < f.vertices.size(); i++) {

			Point n = normals[f.normals[i]];
			glNormal3d(n.x, n.y, n.z);


			Point p = vertices[f.vertices[i]];
			glVertex3d(p.x, p.y, p.z);
		}
		glEnd();
	}

	// Desenha linhas nas faces
	// Desabilita a luz
	glDisable(GL_LIGHTING);

	// Seta a cor da linhas
	glColor3d(1, 0, 0);

	// Seta o tamanho da linha
	glLineWidth(0.01);
	for (Face f : faces) {
		glBegin(GL_LINE_LOOP);
		for (int i = 0; i < f.vertices.size(); i++) {
			Point p = vertices[f.vertices[i]];
			glVertex3d(p.x, p.y, p.z);
		}
		glEnd();
	}
	// Reabilita a luz
	glEnable(GL_LIGHTING);
}

void Model::translate(GLfloat* translation)
{
	// A translação só precisa ser aplicada aos vertices
	for (int i = 0; i < vertices.size(); i++)
	{
		vertices[i].x += translation[0];
		vertices[i].y += translation[1];
		vertices[i].z += translation[2];
	}
}

void Model::rotateX(GLfloat degree)
{
	// Obtem matrix de rotação
	GLfloat* rotateMatrix = MatrixGenerator::createRotateXMatrix(degree);

	// Multiplica pelos vertices
	for (int i = 0; i < vertices.size(); i++)
	{
		// Multiplica a matriz pelo vertice e o atualiza
		Point aux;
		aux.x = vertices[i].x;
		aux.y = vertices[i].y;
		aux.z = vertices[i].z;

		vertices[i].x = (rotateMatrix[0] * aux.x) + (rotateMatrix[1] * aux.y) + (rotateMatrix[2] * aux.z);
		vertices[i].y = (rotateMatrix[4] * aux.x) + (rotateMatrix[5] * aux.y) + (rotateMatrix[6] * aux.z);
		vertices[i].z = (rotateMatrix[8] * aux.x) + (rotateMatrix[9] * aux.y) + (rotateMatrix[10] * aux.z);

		// Multiplica a matriz peles normais para atualizar a iluminação
		aux.x = normals[i].x;
		aux.y = normals[i].y;
		aux.z = normals[i].z;

		normals[i].x = (rotateMatrix[0] * aux.x) + (rotateMatrix[1] * aux.y) + (rotateMatrix[2] * aux.z);
		normals[i].y = (rotateMatrix[4] * aux.x) + (rotateMatrix[5] * aux.y) + (rotateMatrix[6] * aux.z);
		normals[i].z = (rotateMatrix[8] * aux.x) + (rotateMatrix[9] * aux.y) + (rotateMatrix[10] * aux.z);
	}

	// Apaga a matriz de rotação
	delete rotateMatrix;
}

void Model::rotateY(GLfloat degree)
{
	// Obtem matrix de rotação
	GLfloat* rotateMatrix = MatrixGenerator::createRotateYMatrix(degree);

	// Multiplica pelos vertices
	for (int i = 0; i < vertices.size(); i++)
	{
		// Multiplica a matriz pelo vertice e o atualiza
		Point aux;
		aux.x = vertices[i].x;
		aux.y = vertices[i].y;
		aux.z = vertices[i].z;

		vertices[i].x = (rotateMatrix[0] * aux.x) + (rotateMatrix[1] * aux.y) + (rotateMatrix[2] * aux.z);
		vertices[i].y = (rotateMatrix[4] * aux.x) + (rotateMatrix[5] * aux.y) + (rotateMatrix[6] * aux.z);
		vertices[i].z = (rotateMatrix[8] * aux.x) + (rotateMatrix[9] * aux.y) + (rotateMatrix[10] * aux.z);

		// Multiplica a matriz peles normais para atualizar a iluminação
		aux.x = normals[i].x;
		aux.y = normals[i].y;
		aux.z = normals[i].z;

		normals[i].x = (rotateMatrix[0] * aux.x) + (rotateMatrix[1] * aux.y) + (rotateMatrix[2] * aux.z);
		normals[i].y = (rotateMatrix[4] * aux.x) + (rotateMatrix[5] * aux.y) + (rotateMatrix[6] * aux.z);
		normals[i].z = (rotateMatrix[8] * aux.x) + (rotateMatrix[9] * aux.y) + (rotateMatrix[10] * aux.z);
	}

	// Apaga a matriz de rotação
	delete rotateMatrix;
}

void Model::rotateZ(GLfloat degree)
{
	// Obtem matrix de rotação
	GLfloat* rotateMatrix = MatrixGenerator::createRotateZMatrix(degree);

	// Multiplica pelos vertices
	for (int i = 0; i < vertices.size(); i++)
	{
		// Multiplica a matriz pelo vertice e o atualiza
		Point aux;
		aux.x = vertices[i].x;
		aux.y = vertices[i].y;
		aux.z = vertices[i].z;

		vertices[i].x = (rotateMatrix[0] * aux.x) + (rotateMatrix[1] * aux.y) + (rotateMatrix[2] * aux.z);
		vertices[i].y = (rotateMatrix[4] * aux.x) + (rotateMatrix[5] * aux.y) + (rotateMatrix[6] * aux.z);
		vertices[i].z = (rotateMatrix[8] * aux.x) + (rotateMatrix[9] * aux.y) + (rotateMatrix[10] * aux.z);

		// Multiplica a matriz peles normais para atualizar a iluminação
		aux.x = normals[i].x;
		aux.y = normals[i].y;
		aux.z = normals[i].z;

		normals[i].x = (rotateMatrix[0] * aux.x) + (rotateMatrix[1] * aux.y) + (rotateMatrix[2] * aux.z);
		normals[i].y = (rotateMatrix[4] * aux.x) + (rotateMatrix[5] * aux.y) + (rotateMatrix[6] * aux.z);
		normals[i].z = (rotateMatrix[8] * aux.x) + (rotateMatrix[9] * aux.y) + (rotateMatrix[10] * aux.z);
	}

	// Apaga a matriz de rotação
	delete rotateMatrix;
}

void Model::scale(GLfloat rate)
{
	// Calcula a posição do objeto em relação a origem do sistema
	GLfloat modelDirection[3] = { 0, 0, 0 };

	// Faz a media de todos os pontos do objeto
	for (int i = 0; i < vertices.size(); i++)
	{
		modelDirection[0] += vertices[i].x;
		modelDirection[1] += vertices[i].y;
		modelDirection[2] += vertices[i].z;
	}
	modelDirection[0] /= vertices.size();
	modelDirection[1] /= vertices.size();
	modelDirection[2] /= vertices.size();

	// Translada objeto para o centro (negando modelDirection)
	modelDirection[0] = -modelDirection[0];
	modelDirection[1] = -modelDirection[1];
	modelDirection[2] = -modelDirection[2];
	translate(modelDirection);

	// Aplica a escala
	for (int i = 0; i < vertices.size(); i++)
	{
		vertices[i].x *= rate;
		vertices[i].y *= rate;
		vertices[i].z *= rate;
	}

	// Move o modelo devolta para para a posição original
	modelDirection[0] = -modelDirection[0];
	modelDirection[1] = -modelDirection[1];
	modelDirection[2] = -modelDirection[2];
	translate(modelDirection);
}