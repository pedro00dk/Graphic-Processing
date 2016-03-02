#include "Light.h"

void Light::init(GLint lightID, GLfloat* ambientLight, GLfloat* diffuseLight, GLfloat* specularLight, GLfloat* position)
{
	// Verifia se atingiu o maximo de luzes suportadas pelo openGL, ou se é um valor invalido
	if (lightID < 0 || lightID > 7)
	{
		throw ("Invalid id");
	}

	// Converte um id de luz para um GLenum que representa a luz no openGL
	glLightID = lightID + 0x4000;

	// Salva os dados da iluminação
	if (verifyRGB(ambientLight) && verifyRGB(diffuseLight) && verifyRGB(specularLight))
	{
		Light::ambientLight = ambientLight;
		Light::diffuseLight = diffuseLight;
		Light::specularLight = specularLight;
	}

	// Salva a posição atual da luz
	Light::position = position;

	// Define a luz ambiente
	glLightfv(glLightID, GL_AMBIENT, ambientLight);

	// Define a luz difusa
	glLightfv(glLightID, GL_DIFFUSE, diffuseLight);

	// Define a luz especular
	glLightfv(glLightID, GL_SPECULAR, specularLight);

	// Define a posição da luz
	glLightfv(glLightID, GL_POSITION, position);
}

Light::Light(GLint lightID)
{
	GLfloat position[3];
	position[0] = 0;
	position[1] = 20;
	position[2] = 0;

	GLfloat whiteAmbientLight[3];
	whiteAmbientLight[0] = 0.1;
	whiteAmbientLight[1] = 0.1;
	whiteAmbientLight[2] = 0.1;
	GLfloat whiteDiffuseLight[3];
	whiteDiffuseLight[0] = 1;
	whiteDiffuseLight[1] = 1;
	whiteDiffuseLight[2] = 1;
	GLfloat whiteSpecularLight[3];
	whiteSpecularLight[0] = 1;
	whiteSpecularLight[1] = 1;
	whiteSpecularLight[2] = 1;

	init(lightID, whiteAmbientLight, whiteDiffuseLight, whiteSpecularLight, position);
}

Light::Light(GLint lightID, GLfloat* position)
{
	GLfloat whiteAmbientLight[3];
	whiteAmbientLight[0] = 0.1;
	whiteAmbientLight[1] = 0.1;
	whiteAmbientLight[2] = 0.1;
	GLfloat whiteDiffuseLight[3];
	whiteDiffuseLight[0] = 1;
	whiteDiffuseLight[1] = 1;
	whiteDiffuseLight[2] = 1;
	GLfloat whiteSpecularLight[3];
	whiteSpecularLight[0] = 1;
	whiteSpecularLight[1] = 1;
	whiteSpecularLight[2] = 1;

	init(lightID, whiteAmbientLight, whiteDiffuseLight, whiteSpecularLight, position);
}

Light::Light(GLint lightID, GLfloat* ambientLight, GLfloat* diffuseLight, GLfloat* specularLight, GLfloat* position)
{
	init(lightID, ambientLight, diffuseLight, specularLight, position);
}


Light::~Light()
{
	// Deleta os ponteiros dos dados
	//delete ambientLight;
	//delete diffuseLight;
	//delete specularLight;
	//delete position;
}

// Verifica se dados RGB e de brilho são validos
bool Light::verifyRGB(GLfloat* valueRGB)
{
	return (valueRGB[0] >= MIN_RGB && valueRGB[0] <= MAX_RGB
		&& valueRGB[1] >= MIN_RGB && valueRGB[1] <= MAX_RGB
		&& valueRGB[2] >= MIN_RGB && valueRGB[2] <= MAX_RGB);
}

void Light::changeAmbient(GLfloat* ambientLight)
{
	// Salva o novo dado
	Light::ambientLight = ambientLight;

	// Define a luz difusa
	glLightfv(glLightID, GL_AMBIENT, ambientLight);
}

void Light::changeDiffuse(GLfloat* diffuseLight) {

	// Salva o novo dado
	Light::diffuseLight = diffuseLight;

	// Define a luz difusa
	glLightfv(glLightID, GL_DIFFUSE, diffuseLight);
}

void Light::changeSpecular(GLfloat* specularLight) {

	// Salva o novo dado
	Light::specularLight = specularLight;

	// Define a luz especular
	glLightfv(glLightID, GL_SPECULAR, specularLight);
}

void Light::changePosition(GLfloat* position) {

	// Salva a posição atual da luz
	Light::position = position;

	// Define a posição da luz
	glLightfv(glLightID, GL_POSITION, position);
}

void Light::setLightToWhite()
{
	Light::ambientLight[0] = 1;
	Light::ambientLight[1] = 1;
	Light::ambientLight[2] = 1;

	Light::diffuseLight[0] = 1;
	Light::diffuseLight[1] = 1;
	Light::diffuseLight[2] = 1;

	Light::specularLight[0] = 1;
	Light::specularLight[0] = 1;
	Light::specularLight[0] = 1;
}

void Light::enableLight()
{
	// Define a posição da luz
	glLightfv(glLightID, GL_POSITION, position);

	// Habilita a luz
	glEnable(glLightID);
}

void Light::disableLight()
{
	// Desabilita a luz
	glDisable(glLightID);
}

void Light::translate(GLfloat* translation) {

	// Calcula a nova posição e a deixa salva no vetor da posição atual
	Light::position[0] += translation[0];
	Light::position[1] += translation[1];
	Light::position[2] += translation[2];

	// Define a posição da luz
	glLightfv(glLightID, GL_POSITION, position);
}

void Light::rotateX(GLfloat	degree) {
	// Obtem matrix de rotação
	GLfloat* rotateMatrix = MatrixGenerator::createRotateXMatrix(degree);


	// Multiplica a matriz pelo vertice e o atualiza
	Point aux;
	aux.x = position[0];
	aux.y = position[1];
	aux.z = position[2];

	position[0] = (rotateMatrix[0] * aux.x) + (rotateMatrix[1] * aux.y) + (rotateMatrix[2] * aux.z);
	position[1] = (rotateMatrix[4] * aux.x) + (rotateMatrix[5] * aux.y) + (rotateMatrix[6] * aux.z);
	position[2] = (rotateMatrix[8] * aux.x) + (rotateMatrix[9] * aux.y) + (rotateMatrix[10] * aux.z);

	// Define a posição da luz
	glLightfv(glLightID, GL_POSITION, position);
}

void Light::rotateY(GLfloat	degree) {
	// Obtem matrix de rotação
	GLfloat* rotateMatrix = MatrixGenerator::createRotateYMatrix(degree);


	// Multiplica a matriz pelo vertice e o atualiza
	Point aux;
	aux.x = position[0];
	aux.y = position[1];
	aux.z = position[2];

	position[0] = (rotateMatrix[0] * aux.x) + (rotateMatrix[1] * aux.y) + (rotateMatrix[2] * aux.z);
	position[1] = (rotateMatrix[4] * aux.x) + (rotateMatrix[5] * aux.y) + (rotateMatrix[6] * aux.z);
	position[2] = (rotateMatrix[8] * aux.x) + (rotateMatrix[9] * aux.y) + (rotateMatrix[10] * aux.z);

	// Define a posição da luz
	glLightfv(glLightID, GL_POSITION, position);
}

void Light::rotateZ(GLfloat	degree) {
	// Obtem matrix de rotação
	GLfloat* rotateMatrix = MatrixGenerator::createRotateZMatrix(degree);


	// Multiplica a matriz pelo vertice e o atualiza
	Point aux;
	aux.x = position[0];
	aux.y = position[1];
	aux.z = position[2];

	position[0] = (rotateMatrix[0] * aux.x) + (rotateMatrix[1] * aux.y) + (rotateMatrix[2] * aux.z);
	position[1] = (rotateMatrix[4] * aux.x) + (rotateMatrix[5] * aux.y) + (rotateMatrix[6] * aux.z);
	position[2] = (rotateMatrix[8] * aux.x) + (rotateMatrix[9] * aux.y) + (rotateMatrix[10] * aux.z);

	// Define a posição da luz
	glLightfv(glLightID, GL_POSITION, position);
}