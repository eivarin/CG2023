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
#include <iostream>

void drawPlane(std::string file){
    std:: ofstream MyFile(file);
    MyFile << "glBegin(GL_LINES);\nglColor3f(1.0f, 0.0f, 0.0f);\nglVertex3f(-100.0f, 0.0f, 0.0f);\nglVertex3f( 100.0f, 0.0f, 0.0f);\nglColor3f(0.0f, 1.0f, 0.0f);\nglVertex3f(0.0f,-100.0f, 0.0f);\nglVertex3f(0.0f, 100.0f, 0.0f);\nglColor3f(0.0f, 0.0f, 1.0f);\nglVertex3f(0.0f, 0.0f,-100.0f);\nglVertex3f(0.0f, 0.0f, 100.0f);\nglEnd();\n\n";
    MyFile << "glBegin(GL_TRIANGLES);\nglVertex3f(3.0f, 0.0f, 3.0f);\nglVertex3f(3.0, 0.0f, 1.0f);\nglVertex3f(1.0f, 0.0f, 3.0f);\nglEnd();\n\nglBegin(GL_TRIANGLES);\nglVertex3f(3.0f, 0.0f, 1.0f);\nglVertex3f(1.0f, 0.0f, 1.0f);\nglVertex3f(1.0f, 0.0f, 3.0f);\nglEnd();\n\nglBegin(GL_TRIANGLES);\nglVertex3f(3.0f, 0.0f, 1.0f);\nglVertex3f(3.0, 0.0f, -1.0f);\nglVertex3f(1.0f, 0.0f, 1.0f);\nglEnd();\n\nglBegin(GL_TRIANGLES);\nglVertex3f(3.0f, 0.0f, -1.0f);\nglVertex3f(1.0, 0.0f, -1.0f);\nglVertex3f(1.0f, 0.0f, 1.0f);\nglEnd();\n\nglBegin(GL_TRIANGLES);\nglVertex3f(3.0f, 0.0f, -1.0f);\nglVertex3f(3.0, 0.0f, -3.0f);\nglVertex3f(1.0f, 0.0f, -1.0f);\nglEnd();\n\nglBegin(GL_TRIANGLES);\nglVertex3f(3.0f, 0.0f, -3.0f);\nglVertex3f(1.0, 0.0f, -3.0f);\nglVertex3f(1.0f, 0.0f, -1.0f);\nglEnd();\n\nglBegin(GL_TRIANGLES);\nglVertex3f(1.0f, 0.0f, 3.0f);\nglVertex3f(1.0, 0.0f, 1.0f);\nglVertex3f(-1.0f, 0.0f, 3.0f);\nglEnd();\n\nglBegin(GL_TRIANGLES);\nglVertex3f(1.0f, 0.0f, 1.0f);\nglVertex3f(-1.0, 0.0f, 1.0f);\nglVertex3f(-1.0f, 0.0f, 3.0f);\nglEnd();\n\nglBegin(GL_TRIANGLES);\nglVertex3f(1.0f, 0.0f, 1.0f);\nglVertex3f(1.0, 0.0f, -1.0f);\nglVertex3f(-1.0f, 0.0f, 1.0f);\nglEnd();\n\nglBegin(GL_TRIANGLES);\nglVertex3f(1.0f, 0.0f, -1.0f);\nglVertex3f(-1.0, 0.0f, -1.0f);\nglVertex3f(-1.0f, 0.0f, 1.0f);\nglEnd();\n\nglBegin(GL_TRIANGLES);\nglVertex3f(1.0f, 0.0f, -1.0f);\nglVertex3f(1.0, 0.0f, -3.0f);\nglVertex3f(-1.0f, 0.0f, -1.0f);\nglEnd();\n\nglBegin(GL_TRIANGLES);\nglVertex3f(1.0f, 0.0f, -3.0f);\nglVertex3f(-1.0f, 0.0f, -3.0f);\nglVertex3f(-1.0f, 0.0f, -1.0f);\nglEnd();\n\nglBegin(GL_TRIANGLES);\nglVertex3f(-1.0f, 0.0f, 3.0f);\nglVertex3f(-1.0, 0.0f, 1.0f);\nglVertex3f(-3.0f, 0.0f, 3.0f);\nglEnd();\n\nglBegin(GL_TRIANGLES);\nglVertex3f(-1.0f, 0.0f, 1.0f);\nglVertex3f(-3.0f, 0.0f, 1.0f);\nglVertex3f(-3.0f, 0.0f, 3.0f);\nglEnd();\n\nglBegin(GL_TRIANGLES);\nglVertex3f(-1.0f, 0.0f, 1.0f);\nglVertex3f(-1.0f, 0.0f, -1.0f);\nglVertex3f(-3.0f, 0.0f, 1.0f);\nglEnd();\n\nglBegin(GL_TRIANGLES);\nglVertex3f(-1.0f, 0.0f, -1.0f);\nglVertex3f(-3.0f, 0.0f, -1.0f);\nglVertex3f(-3.0f, 0.0f, 1.0f);\nglEnd();\n\nglBegin(GL_TRIANGLES);\nglVertex3f(-1.0f, 0.0f, -1.0f);\nglVertex3f(-1.0f, 0.0f, -3.0f);\nglVertex3f(-3.0f, 0.0f, -1.0f);\nglEnd();\n\nglBegin(GL_TRIANGLES);\nglVertex3f(-1.0f, 0.0f, -3.0f);\nglVertex3f(-3.0f, 0.0f, -3.0f);\nglVertex3f(-3.0f, 0.0f, -1.0f);\nglEnd();\n\n";
    MyFile.close();
}
int main(int argc, char **argv) {
    std::string s1 = "plane";
    // input line to draw a plane:  ./generator plane NameOfFileToOutput
	if (argc == 3 && s1.compare(argv[1]) == 0){
       drawPlane(argv[2]);
    } 
	return 1;
}