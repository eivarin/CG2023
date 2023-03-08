#ifndef CAMERA_H
#define CAMERA_H

#include <GL/glew.h>
#include <GL/glut.h>
#include <math.h>


#include "wavefront.hpp"
#include "./rapid_xml/rapidxml.hpp"

class camera{
    private:
        vertex_coords position;
        vertex_coords look_at;
        vertex_coords up;
        float fov;
        float near;
        float far;
        int halfRotationSteps;
        int alpha;
        int beta;
        void reverseCalc(){
            float dx = look_at.x - position.x;
            float dy = look_at.y - position.y;
            float dz = look_at.z - position.z;
            float vLen = sqrtf64(dx*dx+dy*dy+dz*dz);
            dx = dx / vLen;
            dy = dy / vLen;
            dz = dz / vLen;
            float b = asin(dy);
            float a = asin(dx/cos(b));
            float fullRotationSteps = (M_PI / (halfRotationSteps * 2));
            alpha = a/fullRotationSteps;
            beta = b/fullRotationSteps;
        }
    public:
        float ratio;
        camera(){
            position = {
                .x = 0.0f,
                .y = 2.0f,
                .z = -10.0f
            };
            look_at = {
                .x = 0.0f,
                .y = 0.0f,
                .z = 0.0f
            };
            up = {
                .x = 0,
                .y = 1,
                .z = 0
            };
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
            position = {
                .x = std::stof(new_position->first_attribute("x")->value()),
                .y = std::stof(new_position->first_attribute("y")->value()),
                .z = std::stof(new_position->first_attribute("z")->value())
            };
            look_at = {
                .x = std::stof(new_look_at->first_attribute("x")->value()),
                .y = std::stof(new_look_at->first_attribute("y")->value()),
                .z = std::stof(new_look_at->first_attribute("z")->value())
            };
            up = {
                .x = std::stof(new_up->first_attribute("x")->value()),
                .y = std::stof(new_up->first_attribute("y")->value()),
                .z = std::stof(new_up->first_attribute("z")->value())
            };
            fov = std::stof(projection->first_attribute("fov")->value());
            near = std::stof(projection->first_attribute("near")->value());
            far = std::stof(projection->first_attribute("far")->value());
            halfRotationSteps = 75;
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
        }
        void processKeys(bool *normal_keys, bool *special_keys){
            bool changed = false;
            if(normal_keys['w']){
                position.x += 0.05 * look_at.x;
                position.y += 0.05 * look_at.y;
                position.z += 0.05 * look_at.z;
                changed = true;
            }
            if(normal_keys['s']){
                position.x -= 0.05 * look_at.x;
                position.y -= 0.05 * look_at.y;
                position.z -= 0.05 * look_at.z;
                changed = true;
            }
            if(normal_keys['d']){
                position.x += 0.05 * -look_at.z;
                position.z += 0.05 * look_at.x;
                changed = true;
            }
            if(normal_keys['a']){
                position.x -= 0.05 * -look_at.z;
                position.z -= 0.05 * look_at.x;
                changed = true;
            }
            if(normal_keys[' ']){
                position.y += 0.05;
                changed = true;
            }
            if(special_keys[112]){
                position.y -= 0.05;
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
			  0.0f, 1.0f, 0.0f);
        }
};

#endif