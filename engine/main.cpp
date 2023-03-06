#include <GL/glut.h>

#define _USE_MATH_DEFINES
#include <math.h>

#include <fstream>
#include <map>
#include <vector>
#include <string>

#include "rapidxml.hpp"
rapidxml::xml_document<> doc;


struct vertex_coords {
	float x;
	float y;
	float z;
};

struct vertex_texture {
	float x;
	float y;
};

struct vertex_normal {
	float x;
	float y;
	float z;
};

struct vertex_ref {
	int c;
	int t;
	int n;
};

struct face {
	vertex_ref v[3];
};

struct model {
	std::vector<vertex_coords> vs;
	std::vector<vertex_texture> ts;
	std::vector<vertex_normal> ns;
	std::vector<face> fs;
};

struct camera{
	vertex_coords c;
};

struct scene {
	vertex_coords c;
};

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

	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	// Set perspective
	gluPerspective(45.0f, ratio, 1.0f, 1000.0f);

	// return to the model view matrix mode
	glMatrixMode(GL_MODELVIEW);
}

model m1;
int i = 0;
int j = 3;
std::map<vertex_coords, int> dbls;

void aux(vertex_coords v) {
	glVertex3f(v.x, v.y, v.z);
	int index;
	if (j > 2)
	{
		j = 0;
		m1.fs.push_back((face){0});
	}
	if (dbls.find(v) == dbls.end())
	{
		std::pair<vertex_coords, int> p = std::pair<vertex_coords, int>(v, i);
		dbls.insert(p);
		m1.vs.push_back(v);
		index = i++;
	}
	else
	{
		index = dbls.at(v);
	}
	m1.fs.back().v[j++].c = index + 1;
}

bool writen = false;

// void printModel()
// {
// 	if (!writen)
// 	{
// 		FILE *file = fopen("cilinder.3d", "w");
// 		std::ofstream file;
// 		for (vertex_coords const& v : m1.vs)
// 			// fprintf(file, "v %f %f %f\n", v.x, v.y, v.z);
// 			file << "v " << v.x << ' ' << v.y << ' ' << v.z;
// 		for (face const& f : m1.fs)
// 		{
// 			fprintf(file, "f %d/%d/%d", f.v[0].c, f.v[0].t, f.v[0].n);
// 			fprintf(file, " %d/%d/%d", f.v[1].c, f.v[1].t, f.v[1].n);
// 			fprintf(file, " %d/%d/%d\n", f.v[2].c, f.v[2].t, f.v[2].n);
// 		}
// 		fclose(file);
// 		writen = true;
// 	}
// }

void drawCylinder(float radius, float height, int slices, float px, float py, float pz)
{
	float aCil = 0;
	float rotation = (2 * M_PI);
	float delta = rotation / slices;
	height = height / 2;
	glPolygonMode(GL_FRONT, GL_LINE);
	glBegin(GL_TRIANGLES);
	while (aCil <= rotation)
	{
		glColor3f(1.0f, 0.0f, 0.0f);
		float px1 = px + (radius * sin(aCil)), py1 = pz + (radius * cos(aCil));
		float px2 = px + (radius * sin(aCil + delta)), py2 = pz + (radius * cos(aCil + delta));

		aux((vertex_coords){.x = px, .y = height, .z = pz});
		aux((vertex_coords){.x = px1, .y = height, .z = py1});
		aux((vertex_coords){.x = px2, .y = height, .z = py2});

		aux((vertex_coords){.x = px1, .y = height, .z = py1});
		aux((vertex_coords){.x = px1, .y = -height, .z = py1});
		aux((vertex_coords){.x = px2, .y = -height, .z = py2});

		aux((vertex_coords){.x = px1, .y = height, .z = py1});
		aux((vertex_coords){.x = px2, .y = -height, .z = py2});
		aux((vertex_coords){.x = px2, .y = height, .z = py2});

		aux((vertex_coords){.x = px, .y = -height, .z = pz});
		aux((vertex_coords){.x = px2, .y = -height, .z = py2});
		aux((vertex_coords){.x = px1, .y = -height, .z = py1});
		aCil += delta;
	}
	glColor3f(1.0f, 0.0f, 0.0f);
	glEnd();
	// printModel();
}

int ai = 0;
int bi = 0;
int halfRotationSteps = 75;
float r = 1.0f;

float dx = 0.0f;
float dy = 0.0f;
float dz = 0.0f;

float px = 0.0f;
float py = 0.0f;
float pz = -5.0f;

bool wasd[4];
bool setas[4];

bool operator<(const vertex_coords l, const vertex_coords r) {
	return true;
}

void recalcDirection() {
	float fullRotationSteps = (M_PI / (halfRotationSteps * 2));
	float a = ai * fullRotationSteps;
	float b = bi * fullRotationSteps;
	dx = r * cos(b) * sin(a);
	dy = r * sin(b);
	dz = r * cos(b) * cos(a);
}

model loadModel(std::string const& fname) {
	std::ifstream file;
	file.open(fname);
	model m;
	// read the first word of the line
	for(std::string line_header; file >> line_header; ) {
		if(line_header.compare("v") == 0) {
			vertex_coords v;
			file >> v.x >> v.y >> v.z;
			m.vs.push_back(v);
		} else if(line_header.compare("vt") == 0) {
			vertex_texture t;
			file >> t.x >> t.y;
			m.ts.push_back(t);
		} else if (line_header.compare("vn") == 0 ) {
			vertex_normal n;
			file >> n.x >> n.y >> n.z;
			m.ns.push_back(n);
		} else if (line_header.compare("f") == 0 ) {
			face f = {0};
			char ignore;
			file >> f.v[0].c >> ignore >> f.v[0].t >> ignore >> f.v[0].n >> f.v[1].c >> ignore >> f.v[1].t >> ignore >> f.v[1].n >> f.v[2].c >> ignore >> f.v[2].t >> ignore >> f.v[2].n;
			m.fs.push_back(f);
		}
	}
	return m;
}

void loadScene(std::string const& fname){

}

void drawVertex(model m, vertex_ref v_ref){
	vertex_coords c = m.vs.at(v_ref.c-1);
	// VERTEX_TEXTURE t = m.ts.at(v_ref.t-1);
	// VERTEX_NORMAL n = m.ns.at(v_ref.n-1);
	glVertex3f(c.x, c.y, c.z);
}

void drawModel(model m){
	glPolygonMode(GL_FRONT, GL_LINE);
	glBegin(GL_TRIANGLES);
	glColor3f(1.0f, 0.0f, 0.0f);
	int i = 0;
	for(face& f : m.fs) {
		i++;
		drawVertex(m,f.v[0]);
		drawVertex(m,f.v[1]);
		drawVertex(m,f.v[2]);
	}
	glEnd();
}

void processPressedKeys(){
	bool changed = false;
	if(setas[0]){
		px += 0.05 * dx;
		py += 0.05 * dy;
		pz += 0.05 * dz;
		changed = true;
	}
	if(setas[1]){
		px -= 0.05 * dx;
		py -= 0.05 * dy;
		pz -= 0.05 * dz;
		changed = true;
	}
	if(setas[2]){
		px += 0.05 * -dz;
		pz += 0.05 * dx;
		changed = true;
	}
	if(setas[3]){
		px -= 0.05 * -dz;
		pz -= 0.05 * dx;
		changed = true;
	}
	if(wasd[0]){
		bi = ((bi + 1) < halfRotationSteps) ? bi += 1 : bi;
		changed = true;
	}
	if(wasd[1]){
		bi = ((bi - 1) > -halfRotationSteps) ? bi -= 1 : bi;\
		changed = true;
	}
	if(wasd[2]){
		ai += 1;
		changed = true;
	}
	if(wasd[3]){
		ai -= 1;
		changed = true;
	}
	if (changed)
		recalcDirection();
}

void drawAxis(){
	glBegin(GL_LINES);
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(-100.0f, 0.0f, 0.0f);
	glVertex3f( 100.0f, 0.0f, 0.0f);
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.0f,-100.0f, 0.0f);
	glVertex3f(0.0f, 100.0f, 0.0f);
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f,-100.0f);
	glVertex3f(0.0f, 0.0f, 100.0f);
	glEnd();
}

model m2;
void renderScene(void) {
	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	processPressedKeys();

	// set the camera
	glLoadIdentity();
	gluLookAt(px, py, pz,
			  px + dx, py + dy, pz + dz,
			  0.0f, 1.0f, 0.0f);
	drawAxis();
	// drawCylinder(1, 2, 10, -2.0f, 0.0f, 0.0f);
	drawModel(m2);
	// End of frame
	glutSwapBuffers();
}

void processKeys(unsigned char c, int xx, int yy) {
	switch (c)
	{
	case 'w':
		wasd[0] = true;
		break;
	case 's':
		wasd[1] = true;
		break;
	case 'a':
		wasd[2] = true;
		break;
	case 'd':
		wasd[3] = true;
		break;
	default:
		break;
	}
	glutPostRedisplay();
}

void processUpKeys(unsigned char c, int xx, int yy) {
	switch (c)
	{
	case 'w':
		wasd[0] = false;
		break;
	case 's':
		wasd[1] = false;
		break;
	case 'a':
		wasd[2] = false;
		break;
	case 'd':
		wasd[3] = false;
		break;
	default:
		break;
	}
	glutPostRedisplay();
}

void processSpecialKeys(int key, int xx, int yy) {
	switch (key)
	{
	case GLUT_KEY_UP:
		setas[0] = true;
		break;

	case GLUT_KEY_DOWN:
		setas[1] = true;
		break;

	case GLUT_KEY_RIGHT:
		setas[2] = true;
		break;

	case GLUT_KEY_LEFT:
		setas[3] = true;
		break;

	default:
		break;
	}
	glutPostRedisplay();
}

void processSpecialUpKeys(int key, int xx, int yy) {
	switch (key)
	{
	case GLUT_KEY_UP:
		setas[0] = false;
		break;

	case GLUT_KEY_DOWN:
		setas[1] = false;
		break;

	case GLUT_KEY_RIGHT:
		setas[2] = false;
		break;

	case GLUT_KEY_LEFT:
		setas[3] = false;
		break;

	default:
		break;
	}
	glutPostRedisplay();
}


int main(int argc, char **argv) {
	// init GLUT and the window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 800);
	glutCreateWindow("CG@DI-UM");
	recalcDirection();
	// Required callback registry
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);

	// Callback registration for keyboard processing
	glutKeyboardFunc(processKeys);
	glutKeyboardUpFunc(processUpKeys);
	glutSpecialFunc(processSpecialKeys);
	glutSpecialUpFunc(processSpecialUpKeys);


	// doc.parse<0>(text);

	//  OpenGL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	std::string s = "m1.obj";
	m2 = loadModel(s);

	// enter GLUT's main cycle
	glutMainLoop();

	// printModel();

	return 1;
}
