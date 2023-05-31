#ifndef SHARED_H
#define SHARED_H

#include <GL/glew.h>
#include <GL/glut.h>
#include <vector>

GLuint vertices[3], count[3];

void drawLine(float size[3], std::vector<float> &vect){
    int tesselation = 100;
    for (int i = 1; i <= tesselation; i++){
        vect.push_back(-size[0]/i);
        vect.push_back(-size[1]/i); 
        vect.push_back(-size[2]/i);
    }
    vect.push_back(0);
    vect.push_back(0); 
    vect.push_back(0);
    for (int i = tesselation; i > 0; i--){
        vect.push_back(size[0]/i);
        vect.push_back(size[1]/i); 
        vect.push_back(size[2]/i);
    }
}

float redArr[4] = {1, 0, 0, 1.0};
float blackArr[4] = {0, 0, 0, 1.0};
float greenArr[4] = {0, 1, 0, 1.0};
float blueArr[4] = {0, 0, 1, 1.0};

void prepLines(){
    std::vector<float> xList, yList, zList;
    float x[3] = {1, 0, 0};
    float y[3] = {0, 1, 0};
    float z[3] = {0, 0, 1};
    drawLine(x, xList);
    drawLine(y, yList);
    drawLine(z, zList);
    count[0] = xList.size()/3;
    count[1] = yList.size()/3;
    count[2] = zList.size()/3;
    glGenBuffers(3, vertices);
    glBindBuffer(GL_ARRAY_BUFFER, vertices[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * xList.size(), xList.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, vertices[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * yList.size(), yList.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, vertices[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * zList.size(), zList.data(), GL_STATIC_DRAW);
}

void drawBuf(int pos){
    glBindBuffer(GL_ARRAY_BUFFER, vertices[pos]);
    glVertexPointer(3, GL_FLOAT, 0, 0);
    glDrawArrays(GL_LINE_STRIP, 0, count[pos]);
}

void drawAxis(int size){
    glPushAttrib(GL_LIGHTING_BIT);
    glPushMatrix();
    glScalef(size, size, size);
    glMaterialfv(GL_FRONT, GL_AMBIENT, redArr);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, blackArr);
    glMaterialfv(GL_FRONT, GL_SPECULAR, blackArr);
    glMaterialfv(GL_FRONT, GL_EMISSION, blackArr);
    glMaterialf(GL_FRONT, GL_SHININESS, 0);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    drawBuf(0);
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, greenArr);
    drawBuf(1);
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, blueArr);
    drawBuf(2);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glPopMatrix();
    glPopAttrib();
}

#endif