#include <GL/glew.h>
#include <GL/glxew.h>
#include <GL/glut.h>
#include <IL/il.h>


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
#include "transformation.hpp"

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

void calcFrames(){
	float fps;
	int time;
	cena.frames++;
	time = glutGet(GLUT_ELAPSED_TIME);
	if (time - cena.timebase > 1000) {
		fps = cena.frames*1000.0/(time-cena.timebase);
		glutSetWindowTitle(std::to_string(fps).c_str());
		cena.cam.fps = fps;
		cena.timebase = time;
		cena.frames = 0;
	}
}



void renderScene(void) {
	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	calcFrames();
	glLoadIdentity();
	cena.draw();
	glFinish();
	glutSwapBuffers();
}

void update(int t)
{
	cena.cam.processCameraKeys(cena.normal_keys, cena.special_keys);
    glutTimerFunc(16, update, 0);
    glutPostRedisplay();
}

void processMouse(int x, int y){
	cena.cam.processMouseCamera(x, y);
}

void processKeys(unsigned char c, int xx, int yy) {
	if (c >= 'a' && c <= 'z') cena.normal_keys[c - 32] = false;
	if (c >= 'A' && c <= 'Z') cena.normal_keys[c + 32] = false;
	cena.normal_keys[c] = true;
}

void processUpKeys(unsigned char c, int xx, int yy) {
	if (c >= 'a' && c <= 'z') cena.normal_keys[c - 32] = false;
	if (c >= 'A' && c <= 'Z') cena.normal_keys[c + 32] = false;
	cena.normal_keys[c] = false;
}

void processSpecialKeys(int key, int xx, int yy) {
	if (key == GLUT_KEY_F3) cena.coordsMenu = !cena.coordsMenu;
	cena.special_keys[key] = true;
}

void processSpecialUpKeys(int key, int xx, int yy) {
	cena.special_keys[key] = false;
}


void glDebugOutput(GLenum source, GLenum type, unsigned int id, GLenum severity, GLsizei length, const char *message, const void *userParam)
{
    // ignore non-significant error/warning codes
    if(id == 131169 || id == 131185 || id == 131218 || id == 131204) return; 

    std::cout << "---------------" << std::endl;
    std::cout << "Debug message (" << id << "): " <<  message << std::endl;

    switch (source)
    {
        case GL_DEBUG_SOURCE_API:             std::cout << "Source: API"; break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cout << "Source: Window System"; break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Source: Shader Compiler"; break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cout << "Source: Third Party"; break;
        case GL_DEBUG_SOURCE_APPLICATION:     std::cout << "Source: Application"; break;
        case GL_DEBUG_SOURCE_OTHER:           std::cout << "Source: Other"; break;
    } std::cout << std::endl;

    switch (type)
    {
        case GL_DEBUG_TYPE_ERROR:               std::cout << "Type: Error"; break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated Behaviour"; break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cout << "Type: Undefined Behaviour"; break; 
        case GL_DEBUG_TYPE_PORTABILITY:         std::cout << "Type: Portability"; break;
        case GL_DEBUG_TYPE_PERFORMANCE:         std::cout << "Type: Performance"; break;
        case GL_DEBUG_TYPE_MARKER:              std::cout << "Type: Marker"; break;
        case GL_DEBUG_TYPE_PUSH_GROUP:          std::cout << "Type: Push Group"; break;
        case GL_DEBUG_TYPE_POP_GROUP:           std::cout << "Type: Pop Group"; break;
        case GL_DEBUG_TYPE_OTHER:               std::cout << "Type: Other"; break;
    } std::cout << std::endl;
    
    switch (severity)
    {
        case GL_DEBUG_SEVERITY_HIGH:         std::cout << "Severity: high"; break;
        case GL_DEBUG_SEVERITY_MEDIUM:       std::cout << "Severity: medium"; break;
        case GL_DEBUG_SEVERITY_LOW:          std::cout << "Severity: low"; break;
        case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: notification"; break;
    } std::cout << std::endl;
    std::cout << std::endl;
};

void enableMultisample(int msaa)
{
    if (msaa)
    {
        glEnable(GL_MULTISAMPLE);
        glHint(GL_MULTISAMPLE_FILTER_HINT_NV, GL_NICEST);

        // detect current settings
        GLint iMultiSample = 0;
        GLint iNumSamples = 0;
        glGetIntegerv(GL_SAMPLE_BUFFERS, &iMultiSample);
        glGetIntegerv(GL_SAMPLES, &iNumSamples);
        // printf("MSAA on, GL_SAMPLE_BUFFERS = %d, GL_SAMPLES = %d\n", iMultiSample, iNumSamples);
    }
    else
    {
        glDisable(GL_MULTISAMPLE);
        // printf("MSAA off\n");
    }   
}

int main(int argc, char **argv) {
	// init GLUT and the window
	std::string fname;
	if (argc == 2) fname = argv[1];
	else fname = "example.xml";
	
	animated_translation::at_vector = std::vector<animated_translation*>();
	cena = loadScene(fname);
	glutInit(&argc, argv);
	glutSetOption(GLUT_MULTISAMPLE, 4);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA| GLUT_MULTISAMPLE);
	enableMultisample(4);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(cena.wWidth, cena.wHeight);
	cena.timebase = glutGet(GLUT_ELAPSED_TIME);
	glutCreateWindow("CG@DI-UM");
	ilInit();
	glewInit();
	if (cena.hasLighting){
		glEnable(GL_LIGHTING);
		glEnable(GL_RESCALE_NORMAL);
		glEnable(GL_TEXTURE_2D);
		// controls global ambient light
		float amb[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, amb);
	}



	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	cena.prep();
	for (auto at : animated_translation::at_vector) at->prepLine();
	// Required callback registry
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
	glutTimerFunc(0, update, 0);

	// Callback registration for keyboard processing
	glutSetKeyRepeat(GLUT_KEY_REPEAT_ON);
	glutKeyboardFunc(processKeys);
	glutKeyboardUpFunc(processUpKeys);
	glutSpecialFunc(processSpecialKeys);
	glutSpecialUpFunc(processSpecialUpKeys);
	glutPassiveMotionFunc(processMouse);
	glutMotionFunc(processMouse);
	glutSetCursor(GLUT_CURSOR_NONE);
	Display *dpy = glXGetCurrentDisplay();
    GLXDrawable drawable = glXGetCurrentDrawable();
    const int interval = -1;

    if (drawable) {
        glXSwapIntervalEXT(dpy, drawable, interval);
    }
	//  OpenGL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_CLAMP);


	glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS); 
    glDebugMessageCallback(glDebugOutput, nullptr);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
	// enter GLUT's main cycle
	glutMainLoop();


	return 1;
}
