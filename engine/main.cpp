#include <GL/glut.h>

#define _USE_MATH_DEFINES
#include <math.h>

#include <fstream>
#include <map>
#include <vector>
#include <string>
#include <iostream>

#include "rapidxml.hpp"


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
	vertex_coords position;
	vertex_coords look_at;
	vertex_coords up;
	float fov;
	float ratio;
	float near;
	float far;
	int halfRotationSteps;
	int alpha;
	int beta;
};

struct scene {
	camera cam;
	std::vector<model> ms;
	int wHeight;
	int wWidth;
	bool normal_keys[256];
	bool special_keys[1024];
};

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

	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	// Set perspective
	gluPerspective(cena.cam.fov, ratio, cena.cam.near, cena.cam.far);

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

bool operator<(const vertex_coords l, const vertex_coords r) {
	return true;
}

void recalcDirection() {
	float fullRotationSteps = (M_PI / (cena.cam.halfRotationSteps * 2));
	float a = cena.cam.alpha * fullRotationSteps;
	float b = cena.cam.beta * fullRotationSteps;
	cena.cam.look_at.x = cos(b) * sin(a);
	cena.cam.look_at.y = sin(b);
	cena.cam.look_at.z = cos(b) * cos(a);
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

scene loadScene(std::string const& fname){
	std::ifstream file(fname, std::ios::binary | std::ios::ate);
	std::streamsize size = file.tellg();
	file.seekg(0, std::ios::beg);
	std::vector<char> buffer(size);
	file.read(buffer.data(), size);
	
	rapidxml::xml_document<> doc;
	doc.parse<0>(reinterpret_cast<char*>(buffer.data()));
	rapidxml::xml_node<> *world = doc.first_node("world");
	rapidxml::xml_node<> *window = world->first_node("window");
	rapidxml::xml_node<> *projection = world->first_node("camera")->first_node("projection");
	rapidxml::xml_node<> *position = world->first_node("camera")->first_node("position");
	rapidxml::xml_node<> *look_at = world->first_node("camera")->first_node("lookAt");
	rapidxml::xml_node<> *up = world->first_node("camera")->first_node("up");
	rapidxml::xml_node<> *models = world->first_node("group")->first_node("models");
	scene s = {0};
	s.wHeight = std::stol(window->first_attribute("height")->value());
	s.wWidth = std::stol(window->first_attribute("width")->value());
	s.cam = { 
		.position = {
			.x = std::stof(position->first_attribute("x")->value()),
			.y = std::stof(position->first_attribute("y")->value()),
			.z = std::stof(position->first_attribute("z")->value())
		},
		.look_at = {
			.x = std::stof(look_at->first_attribute("x")->value()),
			.y = std::stof(look_at->first_attribute("y")->value()),
			.z = std::stof(look_at->first_attribute("z")->value())
		},
		.up = {
			.x = std::stof(up->first_attribute("x")->value()),
			.y = std::stof(up->first_attribute("y")->value()),
			.z = std::stof(up->first_attribute("z")->value())
		},
		.fov = std::stof(projection->first_attribute("fov")->value()),
		.ratio = (float) s.wWidth / (float) s.wHeight,
		.near = std::stof(projection->first_attribute("near")->value()),
		.far = std::stof(projection->first_attribute("far")->value()),
		.halfRotationSteps = 75,
		.alpha = 0, 
		.beta = 0
		};
	for (rapidxml::xml_node<> *model = models->first_node(); model; model = model->next_sibling())
	{
		s.ms.push_back(loadModel(model->first_attribute("file")->value()));
	}
	return s;
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
	glColor3f(1.0f, 1.0f, 0.0f);
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
	if(cena.special_keys[GLUT_KEY_UP]){
		cena.cam.position.x += 0.05 * cena.cam.look_at.x;
		cena.cam.position.y += 0.05 * cena.cam.look_at.y;
		cena.cam.position.z += 0.05 * cena.cam.look_at.z;
		changed = true;
	}
	if(cena.special_keys[GLUT_KEY_DOWN]){
		cena.cam.position.x -= 0.05 * cena.cam.look_at.x;
		cena.cam.position.y -= 0.05 * cena.cam.look_at.y;
		cena.cam.position.z -= 0.05 * cena.cam.look_at.z;
		changed = true;
	}
	if(cena.special_keys[GLUT_KEY_RIGHT]){
		cena.cam.position.x += 0.05 * -cena.cam.look_at.z;
		cena.cam.position.z += 0.05 * cena.cam.look_at.x;
		changed = true;
	}
	if(cena.special_keys[GLUT_KEY_LEFT]){
		cena.cam.position.x -= 0.05 * -cena.cam.look_at.z;
		cena.cam.position.z -= 0.05 * cena.cam.look_at.x;
		changed = true;
	}
	if(cena.normal_keys['w']){
		cena.cam.beta = ((cena.cam.beta + 1) < cena.cam.halfRotationSteps) ? cena.cam.beta += 1 : cena.cam.beta;
		changed = true;
	}
	if(cena.normal_keys['s']){
		cena.cam.beta = ((cena.cam.beta - 1) > -cena.cam.halfRotationSteps) ? cena.cam.beta -= 1 : cena.cam.beta;
		changed = true;
	}
	if(cena.normal_keys['a']){
		cena.cam.alpha += 1;
		changed = true;
	}
	if(cena.normal_keys['d']){
		cena.cam.alpha -= 1;
		changed = true;
	}
	if (changed)
		recalcDirection();
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

void drawCone(int height, int radius, int slices, int stacks)
{
    float delta = (2*M_PI) / (float)slices;
    float stack_h = height / (float)stacks;
    float curr_h = height;
    float stack_r = radius / (float)stacks;
    float curr_r = 0;
    int i = 0;
    glPolygonMode(GL_FRONT, GL_LINE);
    glBegin(GL_TRIANGLES);
    curr_r += stack_r;
    while (i < slices)
    {
        float aCil = delta*i;
        float px1 = (sin(aCil)), py1 = (cos(aCil));
        float px2 = (sin(aCil + delta)), py2 = (cos(aCil + delta));
        glVertex3f(0.0f, curr_h, 0.0f);
        glVertex3f(px1 * curr_r, curr_h - stack_h, py1  * curr_r);
        glVertex3f(px2 * curr_r, curr_h - stack_h, py2  * curr_r);
        i++;
    }
    curr_h-=stack_h;
    while (curr_h > 0)
    {
        i = 0;
        while (i < slices)
        {
            float aCil = delta*i;
            float px1 = (sin(aCil)), py1 = (cos(aCil));
            float px2 = (sin(aCil + delta)), py2 = (cos(aCil + delta));
            
            glVertex3f(px1 * curr_r, curr_h, py1 * curr_r);
            glVertex3f(px1 * (curr_r+stack_r), curr_h - stack_h, py1 * (curr_r+stack_r));
            glVertex3f(px2 * (curr_r+stack_r), curr_h - stack_h, py2 * (curr_r+stack_r));

            glVertex3f(px2 * curr_r, curr_h, py2 * curr_r);
            glVertex3f(px1 * curr_r, curr_h, py1 * curr_r);
            glVertex3f(px2 * (curr_r+stack_r), curr_h - stack_h, py2 * (curr_r+stack_r));
            i++;
        }
        curr_h-=stack_h;
        curr_r+=stack_r;
    }
    i=0;
    while (i < slices)
    {
        float aCil = delta*i;
        float px1 = (sin(aCil)), py1 = (cos(aCil));
        float px2 = (sin(aCil + delta)), py2 = (cos(aCil + delta));
        glVertex3f(px2 * curr_r, curr_h, py2  * curr_r);
        glVertex3f(px1 * curr_r, curr_h, py1  * curr_r);
        glVertex3f(0.0f, curr_h, 0.0f);
        i++;
    }
    
    glEnd();
}

void drawSphere(int radius, int slices, int stacks)
{
    float delta = (2*M_PI) / (float)slices;
	float stack = M_PI / (float)stacks;
    int curr_delta = 0;
	int curr_stack = 1;
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glBegin(GL_TRIANGLES);
	while (curr_delta < slices)
	{
		float alpha = delta*curr_delta;
		float beta = (stack*curr_stack) - (M_PI/2);
		float px1 = radius * cos(beta) * sin(alpha); float py1 = (radius * sin(beta)) + radius; float pz1 = radius * cos(beta) * cos(alpha);
		float px2 = radius * cos(beta) * sin(alpha+delta); float py2 = (radius * sin(beta)) + radius; float pz2 = radius * cos(beta) * cos(alpha+delta);
		float px3,px4,py3,py4,pz3,pz4;
		glColor3f(1.0f, 0.0f, 1.0f);
		glVertex3f(px2,py2,pz2);
		glVertex3f(px1,py1,pz1);
		glVertex3f(0.0f,0.0f,0.0f);

		while (curr_stack < stacks)
		{
			beta = (stack*curr_stack) - (M_PI/2);
			px1 = radius * cos(beta - stack) * sin(alpha); 
			py1 = (radius * sin(beta - stack)) + radius; 
			pz1 = radius * cos(beta - stack) * cos(alpha);
			
			px2 = radius * cos(beta) * sin(alpha); 
			py2 = (radius * sin(beta)) + radius; 
			pz2 = radius * cos(beta) * cos(alpha);

			px3 = radius *  cos(beta - stack) * sin(alpha+delta); 
			py3 = (radius * sin(beta - stack)) + radius; 
			pz3 = radius *  cos(beta - stack) * cos(alpha+delta);

			px4 = radius * cos(beta) * sin(alpha+delta); 
			py4 = (radius * sin(beta)) + radius; 
			pz4 = radius * cos(beta) * cos(alpha+delta);

			glVertex3f(px1,py1,pz1);
			glVertex3f(px4,py4,pz4);
			glVertex3f(px2,py2,pz2);

			glVertex3f(px4,py4,pz4);
			glVertex3f(px1,py1,pz1);
			glVertex3f(px3,py3,pz3);

			curr_stack++;
		}
		curr_stack--;
		beta = (stack*curr_stack) - (M_PI/2);
		px1 = radius * cos(beta) * sin(alpha); py1 = (radius * sin(beta)) + radius; pz1 = radius * cos(beta) * cos(alpha);
		px2 = radius * cos(beta) * sin(alpha+delta); py2 = (radius * sin(beta)) + radius; pz2 = radius * cos(beta) * cos(alpha+delta);
		glVertex3f(0.0f,2*radius,0.0f);
		glVertex3f(px1,py1,pz1);
		glVertex3f(px2,py2,pz2);

		curr_delta++;
		curr_stack = 1;
	}
	
    glEnd();
}

void renderScene(void) {
	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set the camera
	glLoadIdentity();
	gluLookAt(cena.cam.position.x, cena.cam.position.y, cena.cam.position.z,
			  cena.cam.position.x + cena.cam.look_at.x, cena.cam.position.y + cena.cam.look_at.y, cena.cam.position.z + cena.cam.look_at.z,
			  0.0f, 1.0f, 0.0f);
	drawAxis();
	//drawCylinder(1, 2, 10, -2.0f, 0.0f, 0.0f);
	//drawSphere(1, 100, 100);
	// drawModel(m2);
	for(auto& m : cena.ms){
		drawModel(m);
	}
	// End of frame
	glutSwapBuffers();
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

void update(int t)
{
    processPressedKeys();
    glutTimerFunc(16, update, 0);
    glutPostRedisplay();
}

int main(int argc, char **argv) {
	// init GLUT and the window
	std::string fname;
	if (argc == 2) fname = argv[1];
	else fname = "example.xml";
	
	cena = loadScene(fname);
	recalcDirection();
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(cena.wWidth, cena.wHeight);
	glutCreateWindow("CG@DI-UM");
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