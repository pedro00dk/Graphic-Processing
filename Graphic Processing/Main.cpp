#include <stdlib.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <ctime>
#include <gl\glut.h>
#include "Camera.h"
#include "MatrixGenerator.h"
#include "Model.h"
#include "Light.h"

using namespace std;

// Prototipos de função
void loadModels();
void createLights();

void display();
void reshape(GLint width, GLint height);
void keyboard(GLubyte key, GLint x, GLint y);
void keyUp(GLubyte key, GLint x, GLint y);
void mouseClick(GLint button, GLint state, GLint x, GLint y);
void mouseMov(GLint x, GLint y);
void mouseWheel(int button, int dir, int x, int y);
void initGL();

///////////////////////////////////////////////////////////
// Cores padrão dos modelos
GLfloat diffuseColor[3] = { 0.65, 0.65, 0.65 };
GLfloat specularcolor[3] = { 1, 1, 1 };
GLfloat emissiceColor[3] = { 0, 0, 0 };
GLfloat shininess[1] = { 64 };
GLfloat density = 0.01;

///////////////////////////////////////////////////////////
// Cameras
Camera camera0 = Camera();
Camera camera1 = Camera();
///////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////
// Vetores de objetos
vector<Model> models;
vector<Light> lights;

// Modelo selecionado
int selected = 0;
///////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////
// Modo diretor
bool director = false;
///////////////////////////////////////////////////////////


int main(int argc, char** argv)
{
	// Inicializa o glut
	glutInit(&argc, argv);

	// Seta buffers duplos e modo de cor rgba
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

	// Define o tamanho da janela e a posição inicial
	glutInitWindowSize(1280, 720);
	glutInitWindowPosition(0, 0);

	// Inicia a janela
	glutCreateWindow("Graphic Processing");

	// Metod GL que devem ser setados inicialmente
	initGL();

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);

	glutKeyboardFunc(keyboard);
	glutKeyboardUpFunc(keyUp);
	glutMouseFunc(mouseClick);
	glutMotionFunc(mouseMov);
//	glutMouseWheelFunc(mouseWheel);
	cout << glutDeviceGet(GLUT_NUM_MOUSE_BUTTONS) << endl;
	glutMainLoop();
	return 0;
}

GLuint textures[10];
void loadGLTextures()
{
	//carrega uma imagem diretamente como uma nova textura OpenGL
	//textures[0] = SOIL_load_OGL_texture( "NeHe.bmp", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
	if (textures[0] == 0)
	{
		cout << "sem textura" << endl;
	}
	glBindTexture(GL_TEXTURE_2D, textures[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
}

void loadModels()
{
	//models.push_back(Model("obj\\CAT.OBJ", diffuseColor, specularcolor, emissiceColor, shininess));
	//models.push_back(Model("obj\\Cow.obj", diffuseColor, specularcolor, emissiceColor, shininess));
	//models.push_back(Model("obj\\cube.obj", diffuseColor, specularcolor, emissiceColor, shininess));
	//models.push_back(Model("obj\\eagle.obj", diffuseColor, specularcolor, emissiceColor, shininess));
	models.push_back(Model("obj\\shark.obj", diffuseColor, specularcolor, emissiceColor, shininess));
	//models.push_back(Model("obj\\spheretri.obj", diffuseColor, specularcolor, emissiceColor, shininess));
	//models.push_back(Model("obj\\whale.obj", diffuseColor, specularcolor, emissiceColor, shininess));
}

void createLights()
{
	// Luzes
	GLfloat ambientLightW[3] = { 0.1, 0.1, 0.1 };
	GLfloat diffuseLightW[3] = { 0.2, 0.2, 0.9 };
	GLfloat specularLightW[3] = { 1, 1, 1 };
	GLfloat positionW[3] = { 50, 50, 0 };
	Light wLight = Light(1, ambientLightW, diffuseLightW, specularLightW, positionW);

	GLfloat ambientLightR[3] = { 0.1, 0.1, 0.1 };
	GLfloat diffuseLightR[3] = { 0.9, 0.25, 0.25 };
	GLfloat specularLightR[3] = { 1, 0.85, 0.85 };
	GLfloat positionR[3] = { 0, -20, 0 };
	Light rLight = Light(1, ambientLightR, diffuseLightR, specularLightR, positionR);

	wLight.enableLight();
	rLight.enableLight();
}


// Função callback responsavel pelo desenho
void display()
{
	// Limpa buffers de cor e profundidade
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//test sombra

	// Seta a camera
	glMatrixMode(GL_MODELVIEW);
	GLfloat* cameraExtrinsec = camera0.getCameraMatrix();
	glLoadIdentity();
	glMultMatrixf(cameraExtrinsec);
	delete cameraExtrinsec;

	// Desabilita a iluminação e desenha os eixos
	glDisable(GL_LIGHTING);
	glBegin(GL_LINES);
	glColor3f(1, 0, 0);
	glVertex3f(10, 0, 0);
	glColor3f(1, 1, 0);
	glVertex3f(-10, 0, 0);
	glColor3f(0, 1, 0);
	glVertex3f(0, 10, 0);
	glColor3f(0, 1, 1);
	glVertex3f(0, -10, 0);
	glColor3f(0, 0, 1);
	glVertex3f(0, 0, 10);
	glColor3f(1, 0, 1);
	glVertex3f(0, 0, -10);
	glEnd();
	// Reabilita a iluminação
	glEnable(GL_LIGHTING);

	// Renderiza modelo selecionado
	for (int i = 0; i < models.size(); i++)
	{
		if (i == selected)
		{
			models[i].selectRender();
		}
		else
		{
			models[i].render();
		}
	}
	
	//testes de nevoa
	GLint inteiro = 3;
	GLfloat cor[4] = { 0, 0, 0, 1 };
	glEnable(GL_FOG);
	glFogi(GL_FOG_INDEX, 30);
	glFogi(GL_FOG_MODE, GL_LINEAR);
	glFogf(GL_FOG_START, 1);
	glFogf(GL_FOG_END, 30);
	glFogfv(GL_FOG_COLOR, cor);
	
	// Executa comandos
	glFlush();

	// Alterna os buffers
	glutSwapBuffers();
}

// Boolean que verifica se a primeira execuусo do reshape
bool firstReshape = true;

// Funусo callback responsavel por atualizar o sistema ao redimensionar
void reshape(int width, int height)
{
	// Evita divisсo por 0
	if (height == 0) height = 1;
	if (width == 0) width = 1;

	// A viewport é toda a area da tela
	glViewport(0, 0, width, height);

	// Faz apenas na primeira chamada
	if (firstReshape) {

		// Seleciona a matriz de projecao para ser operada
		glMatrixMode(GL_PROJECTION);

		// Carrega a matriz identidade na matriz de projeусo
		glLoadIdentity();

		// Seta o frustum da cameraera (as laterais são vrificadas para a imagem não ficar espremida ou esticada
		glFrustum(-1, 1, -1, 1, 1, 500);

		// Indica que não é mais o primeiro reshape
		firstReshape = false;
	}
}
bool nPressed = false;
void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'w':
		camera0.translateDirector(0.5);
		glutPostRedisplay();
		break;
	case 's':
		camera0.translateDirector(-0.5);
		glutPostRedisplay();
		break;
	case 'a':
		camera0.translateNormal(0.5);
		glutPostRedisplay();
		break;
	case 'd':
		camera0.translateNormal(-0.5);
		glutPostRedisplay();
		break;
	case 'n' :
		density += 0.5;
		cout << density << endl;
		glFogf(GL_FOG_DENSITY, density);
		glutPostRedisplay();
		nPressed = true;
		break;

	}
	
	// Taxa de movimentação da translação
	GLfloat trRate = 0.4;

	// Angulo da rotação
	GLfloat rtRate = 1;

	GLfloat translation[3];
	GLfloat* rotation;

	switch (key)
	{
	case '1':
		translation[0] = -trRate;
		translation[1] = 0;
		translation[2] = 0;
		models[selected].translate(translation);
		glutPostRedisplay();
		break;
	case '2':
		translation[0] = trRate;
		translation[1] = 0;
		translation[2] = 0;
		models[selected].translate(translation);
		glutPostRedisplay();
		break;
	case '3':
		translation[0] = 0;
		translation[1] = -trRate;
		translation[2] = 0;
		models[selected].translate(translation);
		glutPostRedisplay();
		break;
	case '4':
		translation[0] = 0;
		translation[1] = trRate;
		translation[2] = 0;
		models[selected].translate(translation);
		glutPostRedisplay();
		break;
	case '5':
		translation[0] = 0;
		translation[1] = 0;
		translation[2] = -trRate;
		models[selected].translate(translation);
		glutPostRedisplay();
		break;
	case '6':
		translation[0] = 0;
		translation[1] = 0;
		translation[2] = trRate;
		models[selected].translate(translation);
		glutPostRedisplay();
		break;
	case '7':
		models[selected].rotateX(rtRate);
		glutPostRedisplay();
		break;
	case '8':
		models[selected].rotateY(rtRate);
		glutPostRedisplay();
		break;
	case '9':
		models[selected].rotateZ(rtRate);
		glutPostRedisplay();
		break;
	case '-':
		// Redução em 1%
		models[selected].scale(0.99);
		glutPostRedisplay();
		break;
	case '=':
		// Aumento em 1%
		models[selected].scale(1.01);
		glutPostRedisplay();
		break;
	case '.':
		selected = (selected < models.size() - 1) ? selected + 1 : 0;
		glutPostRedisplay();
		break;
	case ',':
		selected = (selected > 0) ? selected - 1 : models.size() - 1;
		glutPostRedisplay();
		break;
	}
}

void keyUp(GLubyte key, GLint x, GLint y)
{
	if (key == 'n')
	{
		nPressed = false;
	}
}

Coord lastPosition;

void mouseClick(GLint button, GLint state, int x, int y)
{
	y = -y;
	cout << button << endl;
	if (state == GLUT_DOWN)
	{
		lastPosition.x = x;
		lastPosition.y = y;

		cout << "down" << endl;
	}
	else if (state == GLUT_UP)
	{
		cout << "up" << endl;
	}
	if (button == GLUT_WHEEL_UP && nPressed)
	{
		printf("Wheel Up\n");
		density += 0.01;
		glFogf(GL_FOG_DENSITY, density);
	}
	else if (button == GLUT_WHEEL_DOWN && nPressed)
	{
		printf("Wheel Down\n");
		density -= 0.01;
		if (density < 0) density = 0;
		glFogf(GL_FOG_DENSITY, density);
	}
}

void mouseMov(GLint x, GLint y)
{
	//x = -x;

	y = -y;

	if (lastPosition.x == 0 && lastPosition.y == 0)
	{
		lastPosition.x = x;
		lastPosition.y = y;
	}
	else
	{
		Point axis;
		axis.x = x - lastPosition.x;
		axis.y = y - lastPosition.y;
		axis.z = 0;

		camera0.rotateNormalX(0.1 * axis.y);
		camera0.rotateWorldY(0.1 * axis.x);
		lastPosition.x = x;
		lastPosition.y = y;

		glutPostRedisplay();
	}
}

// Seta os parametros iniciais do openGL
void initGL()
{
	// Seta a cor de fundo para preto e opaco
	glClearColor(0, 0, 0, 1);

	// Seta a profundidade do fundo para muito longe
	glClearDepth(1.0f);

	// Habilita o teste de profundidade para z
	glEnable(GL_DEPTH_TEST);

	// Seta a funcao que define o tipo do teste de profundidade
	glDepthFunc(GL_LEQUAL);

	// Habilita sombreamento suave (Gouraud)
	glShadeModel(GL_SMOOTH);

	// Habilita correушes de perspectiva melhorando a qualidade da imagem
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	// Habilita iluminação (pode ser desabilitada para pintar algum sem iluminação)
	glEnable(GL_LIGHTING);


	// Carrega todos os modelos
	loadModels();

	// Cria todas as luzes
	createLights();
}
