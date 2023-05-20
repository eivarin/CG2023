#ifndef SHARED_H
#define SHARED_H

#include <GL/glew.h>
#include <GL/glut.h>

void drawAxis(float size){
    glPushAttrib(GL_LIGHTING_BIT);
    glBegin(GL_LINES);
    float redArr[4] = {1, 0, 0, 1.0};
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, redArr);
    float specularArr[4] = {0, 0, 0, 1.0};
    glMaterialfv(GL_FRONT, GL_SPECULAR, specularArr);
    float emissiveArr[4] = {0, 0, 0, 1.0};
    glMaterialfv(GL_FRONT, GL_EMISSION, emissiveArr);
    glMaterialf(GL_FRONT, GL_SHININESS, 0);
    glVertex3f(-size, 0.0f, 0.0f);
    glVertex3f( size, 0.0f, 0.0f);
    float greenArr[4] = {0, 1, 0, 1.0};
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, greenArr);
    glVertex3f(0.0f,-size, 0.0f);
    glVertex3f(0.0f, size, 0.0f);
    float blueArr[4] = {0, 0, 1, 1.0};
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, blueArr);
    glVertex3f(0.0f, 0.0f,-size);
    glVertex3f(0.0f, 0.0f, size);
    glEnd();
    glPopAttrib();
}

#endif