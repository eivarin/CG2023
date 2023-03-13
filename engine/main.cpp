#include <GL/glew.h>
#include <GL/glut.h>

#define _USE_MATH_DEFINES
#include <math.h>
#include <fstream>
#include <iostream>
#include <map>
#include <vector>
#include <string>
#include "./rapid_xml/rapidxml.hpp"
#include "wavefront.hpp"
#include "camera.hpp"
#include "scene.hpp"

scene cena;

void changeSize(int w, int h) {
	// Prevent a divide by zero, when window is too short
	// (you cant make a window with zero width).
	if (h == 0)
		h = 1;

	// compute window's aspect ratio
	float ratio = w * 1.0 / h;

	// Set the projection matrix as current
	glMatrixMode(GL_PROJECTION);
	// Load Identity Matrix
	glLoadIdentity();

	cena.wHeight = h;
	cena.wWidth = w;
	cena.cam.ratio = w * 1.0 / h; 

	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	cena.cam.setPerspective();

	// return to the model view matrix mode
	glMatrixMode(GL_MODELVIEW);
}

void drawAxis(){
	glBegin(GL_LINES);
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(-1000.0f, 0.0f, 0.0f);
	glVertex3f( 1000.0f, 0.0f, 0.0f);
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.0f,-1000.0f, 0.0f);
	glVertex3f(0.0f, 1000.0f, 0.0f);
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f,-1000.0f);
	glVertex3f(0.0f, 0.0f, 1000.0f);
	glEnd();
}


void renderScene(void) {
	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set the camera
	glLoadIdentity();
	cena.cam.placeGlut();
	drawAxis();
	glPolygonMode(GL_FRONT, GL_LINE);
	glColor3f(1.0f,1.0f,1.0f);
	cena.main_group.drawGroup();
	glutSwapBuffers();
}

void update(int t)
{
	cena.cam.processKeys(cena.normal_keys, cena.special_keys);
    glutTimerFunc(16, update, 0);
    glutPostRedisplay();
}

void processKeys(unsigned char c, int xx, int yy) {
	cena.normal_keys[c] = true;
}

void processUpKeys(unsigned char c, int xx, int yy) {
	cena.normal_keys[c] = false;
}

void processSpecialKeys(int key, int xx, int yy) {
	cena.special_keys[key] = true;
}

void processSpecialUpKeys(int key, int xx, int yy) {
	cena.special_keys[key] = false;
}

int main(int argc, char **argv) {
	// init GLUT and the window
	std::string fname;
	if (argc == 2) fname = argv[1];
	else fname = "example.xml";
	cena = loadScene(fname);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(cena.wWidth, cena.wHeight);
	glutCreateWindow("CG@DI-UM");
	glewInit();
	glEnableClientState(GL_VERTEX_ARRAY);
	cena.main_group.prepGroup();
	// Required callback registry
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
	glutTimerFunc(0, update, 0);

	// Callback registration for keyboard processing
	glutKeyboardFunc(processKeys);
	glutKeyboardUpFunc(processUpKeys);
	glutSpecialFunc(processSpecialKeys);
	glutSpecialUpFunc(processSpecialUpKeys);

	//  OpenGL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);


	// enter GLUT's main cycle
	glutMainLoop();


	return 1;
}