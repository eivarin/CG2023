#ifndef CAMERA_H
#define CAMERA_H

#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/freeglut.h>
#include <string>
#include <math.h>


#include "model.hpp"
#include "./rapid_xml/rapidxml.hpp"

class camera{
    private:
        vec3 position;
        vec3 look_at;
        vec3 up;
        vec3 crab_vector;
        vec3 true_up;
        float fov;
        float near;
        float far;
        int halfRotationSteps;
        int alpha;
        int beta;
        float stepDimension;
        void reverseCalc(){
            vec3 d = vec3(look_at.x - position.x, look_at.y - position.y, look_at.z - position.z);
            d.normalize();
            float b = asin(d.y);
            float sin_a = d.x / cos(b);
            float cos_a = d.z / cos(b);

            float a = 0.0;
            if (sin_a >= 0.0) {
                a = acos(cos_a);
            } else {
                a = -acos(cos_a);
            }
            float fullRotationSteps = (M_PI / (halfRotationSteps * 2));
            alpha = a/fullRotationSteps;
            beta = b/fullRotationSteps;
        }
    public:
        float fps;
        float ratio;
        camera(){
            position = vec3(0.0f, 2.0f, -10.0f);
            look_at = vec3(0,0,0);
            up = vec3(0,1,0);
            fov = 60;
            near = 1;
            far = 1000; 
            halfRotationSteps = 75;
            alpha = 0;
            beta = 0;
        }
        camera(rapidxml::xml_node<> *c){
            rapidxml::xml_node<> *projection = c->first_node("projection");
            rapidxml::xml_node<> *new_position = c->first_node("position");
            rapidxml::xml_node<> *new_look_at = c->first_node("lookAt");
            rapidxml::xml_node<> *new_up = c->first_node("up");
            position = vec3(
                std::stof(new_position->first_attribute("x")->value()),
                std::stof(new_position->first_attribute("y")->value()),
                std::stof(new_position->first_attribute("z")->value())
            );
            look_at = vec3(
                std::stof(new_look_at->first_attribute("x")->value()),
                std::stof(new_look_at->first_attribute("y")->value()),
                std::stof(new_look_at->first_attribute("z")->value())
            );
            up = vec3(
                std::stof(new_up->first_attribute("x")->value()),
                std::stof(new_up->first_attribute("y")->value()),
                std::stof(new_up->first_attribute("z")->value())
            );
            fov = std::stof(projection->first_attribute("fov")->value());
            near = std::stof(projection->first_attribute("near")->value());
            far = std::stof(projection->first_attribute("far")->value());
            halfRotationSteps = 1000;
            stepDimension = 0.05f;
            reverseCalc();
            this->recalcDirection();
        }
        void resetCamera(){
            look_at = vec3(0,0,0);
            reverseCalc();
            this->recalcDirection();
        }
        void setPerspective(){
            glLoadIdentity();
            gluPerspective(fov, ratio, near, far);
        }
        void recalcDirection() {
            float fullRotationSteps = (M_PI / (halfRotationSteps * 2));
            float a = alpha * fullRotationSteps;
            float b = beta * fullRotationSteps;
            look_at.x = cos(b) * sin(a);
            look_at.y = sin(b);
            look_at.z = cos(b) * cos(a);
            crab_vector = look_at.get_cross_product(up);
            true_up = look_at.get_cross_product(crab_vector);
        }
        void processMouseCamera(int x, int y){
            int centerX = glutGet(GLUT_WINDOW_WIDTH) / 2;
            int centerY = glutGet(GLUT_WINDOW_HEIGHT) / 2;
            alpha -= (x - centerX) ;
            float nBeta = (y - centerY) ;
            beta -= (beta - nBeta > -halfRotationSteps) and (beta - nBeta < halfRotationSteps) ? nBeta : 0;
            recalcDirection();
            if (x != centerX || y != centerY) glutWarpPointer(centerX, centerY);
            glutPostRedisplay();
        }
        void processCameraKeys(bool *normal_keys, bool *special_keys){
            bool changed = false;
            float step = stepDimension;
            if(normal_keys['w'] || normal_keys['W']){
                position.x += step * look_at.x;
                position.y += step * look_at.y;
                position.z += step * look_at.z;
                changed = true;
            }
            if(normal_keys['s'] || normal_keys['S']){
                position.x -= step * look_at.x;
                position.y -= step * look_at.y;
                position.z -= step * look_at.z;
                changed = true;
            }
            if(normal_keys['d'] || normal_keys['D']){
                position.x += 2 * step * crab_vector.x;
                position.z += 2 * step * crab_vector.z;
                changed = true;
            }
            if(normal_keys['a'] || normal_keys['A']){
                position.x -= 2 * step * crab_vector.x;
                position.z -= 2 * step * crab_vector.z;
                changed = true;
            }
            if(normal_keys[' ']){
                position.y += step;
                changed = true;
            }
            if(normal_keys['m']){
                stepDimension *= (stepDimension < 16) ? 2: 1;
            }
            if(normal_keys['n']){
                stepDimension /= (stepDimension > 0.01f) ? 2: 1;
            }
            if(normal_keys['r']){
                resetCamera();
            }
            if(normal_keys['Q']){
                glutLeaveMainLoop();
            }
            if(special_keys[112]){
                position.y -= step;
                changed = true;
            }
            if(special_keys[GLUT_KEY_UP]){
                beta = ((beta + 1) < halfRotationSteps) ? beta += 1 : beta;
                changed = true;
            }
            if(special_keys[GLUT_KEY_DOWN]){
                beta = ((beta - 1) > -halfRotationSteps) ? beta -= 1 : beta;
                changed = true;
            }
            if(special_keys[GLUT_KEY_LEFT]){
                alpha += 1;
                changed = true;
            }
            if(special_keys[GLUT_KEY_RIGHT]){
                alpha -= 1;
                changed = true;
            }
            if (changed)
		        recalcDirection();
        }
        void placeGlut(){
            glLoadIdentity();
            gluLookAt(position.x, position.y, position.z,
			  position.x + look_at.x, position.y + look_at.y, position.z + look_at.z,
			  up.x, up.y, up.z);
        }
        void drawCoords(float w, float h){
            glMatrixMode(GL_PROJECTION);
            glPushMatrix();
            glLoadIdentity();
            // set projection so that coordinates match window pixels
            gluOrtho2D(0, w, h, 0);
            glMatrixMode(GL_MODELVIEW);
            glDisable(GL_DEPTH_TEST);
            glPushMatrix();
            glLoadIdentity();
            glRasterPos2d(10, 30);
            std::string coords = "x:" + std::to_string(position.x) + " y: " + std::to_string(position.y) + " z: " + std::to_string(position.z) + " \nfps: " + std::to_string((int) fps);

            for (const char *c = coords.c_str(); *c != '\0'; c++) {
                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
            }
            glMatrixMode(GL_PROJECTION);
            glPopMatrix();
            glMatrixMode(GL_MODELVIEW);
            glPopMatrix();
            glEnable(GL_DEPTH_TEST);
        }
};

#endif