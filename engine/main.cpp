#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define _USE_MATH_DEFINES
#include <math.h>

#include <fstream>
#include <map>
#include <vector>
#include <string>

typedef struct
{
	float x;
	float y;
	float z;
} VERTEX_COORDS;

struct vertex_texture
{
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

typedef struct face
{
	vertex_ref v[3];
} face;

struct model {
	std::vector<VERTEX_COORDS> vs;
	std::vector<vertex_texture> ts;
	std::vector<vertex_normal> ns;
	std::vector<face> fs;
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
std::map<VERTEX_COORDS, int> dbls;

void aux(VERTEX_COORDS v) {
	glVertex3f(v.x, v.y, v.z);
	int index;
	if (j > 2)
	{
		j = 0;
		m1.fs.push_back((face){0});
	}
	if (dbls.find(v) == dbls.end())
	{
		std::pair<VERTEX_COORDS, int> p = std::pair<VERTEX_COORDS, int>(v, i);
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

void printModel()
{
	if (!writen)
	{
		FILE *file = fopen("cilinder.3d", "w");
		for (VERTEX_COORDS v : m1.vs)
			fprintf(file, "v %f %f %f\n", v.x, v.y, v.z);
		for (face f : m1.fs)
		{
			fprintf(file, "f %d/%d/%d", f.v[0].c, f.v[0].t, f.v[0].n);
			fprintf(file, " %d/%d/%d", f.v[1].c, f.v[1].t, f.v[1].n);
			fprintf(file, " %d/%d/%d\n", f.v[2].c, f.v[2].t, f.v[2].n);
		}
		fclose(file);
		writen = true;
	}
}

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

		aux((VERTEX_COORDS){.x = px, .y = height, .z = pz});
		aux((VERTEX_COORDS){.x = px1, .y = height, .z = py1});
		aux((VERTEX_COORDS){.x = px2, .y = height, .z = py2});

		aux((VERTEX_COORDS){.x = px1, .y = height, .z = py1});
		aux((VERTEX_COORDS){.x = px1, .y = -height, .z = py1});
		aux((VERTEX_COORDS){.x = px2, .y = -height, .z = py2});

		aux((VERTEX_COORDS){.x = px1, .y = height, .z = py1});
		aux((VERTEX_COORDS){.x = px2, .y = -height, .z = py2});
		aux((VERTEX_COORDS){.x = px2, .y = height, .z = py2});

		aux((VERTEX_COORDS){.x = px, .y = -height, .z = pz});
		aux((VERTEX_COORDS){.x = px2, .y = -height, .z = py2});
		aux((VERTEX_COORDS){.x = px1, .y = -height, .z = py1});
		aCil += delta;
	}
	glColor3f(1.0f, 0.0f, 0.0f);
	glEnd();
	printModel();
}

int ai = 0;
int bi = 0;
int steps = 16;
float r = 1.0f;

float dx = 0.0f;
float dy = 0.0f;
float dz = 0.0f;

float px = 0.0f;
float py = 0.0f;
float pz = -5.0f;

bool operator<(const VERTEX_COORDS l, const VERTEX_COORDS r) {
	return true;
}

void recalcDirection() {
	float step = (M_PI / (steps * 2));
	float a = ai * step;
	float b = bi * step;
	dx = r * cos(b) * sin(a);
	dy = r * sin(b);
	dz = r * cos(b) * cos(a);
}


model loadModel(std::string_view fname) {
	std::ifstream file;
	file.open(fname.cbegin());
	model m;
	// read the first word of the line
	for(std::string line_header; file >> line_header; ) {
		if(line_header.compare("v") == 0) {
			VERTEX_COORDS v;
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
			file >> f.v[0].c >> f.v[0].t >> f.v[0].n >> f.v[1].c >> f.v[1].t >> f.v[1].n >> f.v[2].c >> f.v[2].t >> f.v[2].n;
			m.fs.push_back(f);
		}
	}
	return m;
}

void drawVertex(model m, vertex_ref v_ref){
	VERTEX_COORDS c = m.vs.at(v_ref.c-1);
	// VERTEX_TEXTURE t = m.ts.at(v_ref.t-1);
	// VERTEX_NORMAL n = m.ns.at(v_ref.n-1);
	glVertex3f(c.x, c.y, c.z);
}

void drawModel(model m){
	glPolygonMode(GL_FRONT, GL_LINE);
	glBegin(GL_TRIANGLES);
	glColor3f(1.0f, 0.0f, 0.0f);
	for(face& f : m.fs) {
		drawVertex(m,f.v[0]);
		drawVertex(m,f.v[1]);
		drawVertex(m,f.v[2]);
	}
	glEnd();
}
model m2;
void renderScene(void) {
	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set the camera
	glLoadIdentity();
	gluLookAt(px, py, pz,
			  px + dx, py + dy, pz + dz,
			  0.0f, 1.0f, 0.0f);

	// drawCylinder(1, 2, 10, -2.0f, 0.0f, 0.0f);
	drawModel(m2);
	// End of frame
	glutSwapBuffers();
}

void processKeys(unsigned char c, int xx, int yy) {
	bool changed = true;
	switch (c)
	{
	case 'w':
		bi = ((bi + 1) < steps) ? bi += 1 : bi;
		break;
	case 's':
		bi = ((bi - 1) > -steps) ? bi -= 1 : bi;
		break;

	case 'a':
		ai += 1;
		break;

	case 'd':
		ai -= 1;
		break;

	default:
		changed = false;
		break;
	}
	if (changed)
		recalcDirection();
	glutPostRedisplay();
}

void processSpecialKeys(int key, int xx, int yy) {
	switch (key)
	{
	case GLUT_KEY_UP:
		px += dx;
		py += dy;
		pz += dz;
		break;

	case GLUT_KEY_DOWN:
		px -= dx;
		py -= dy;
		pz -= dz;
		break;

	case GLUT_KEY_RIGHT:
		px += -dz;
		pz += dx;
		break;

	case GLUT_KEY_LEFT:
		px -= -dz;
		pz -= dx;
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
	glutSpecialFunc(processSpecialKeys);

	//  OpenGL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	std::string_view s = "m1.obj";
	m2 = loadModel(s);

	// enter GLUT's main cycle
	glutMainLoop();

	printModel();

	return 1;
}
