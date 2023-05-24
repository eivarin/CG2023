#ifndef LIGHT_H
#define LIGHT_H

#include <GL/glew.h>
#include <GL/glut.h>
#include <vector>
#include <string>
#include "model.hpp"
#include "./rapid_xml/rapidxml.hpp"

float dark[4] = {0.2, 0.2, 0.2, 1.0};
float white[4] = {1.0, 1.0, 1.0, 1.0};
float black[4] = {0.0f, 0.0f, 0.0f, 0.0f};

class light
{
private:
    vec3 pos, dir;
    float cutOff;
    bool hasPos = true, hasDir = true;
    GLenum gl_light;
public:
    light(rapidxml::xml_node<> *node, GLenum l){
        gl_light = l;
        std::string tipo = node->first_attribute("type")->value();
        if (tipo.compare("point") == 0) hasDir = false;
        else if (tipo.compare("directional") == 0) hasPos = false;
        if (hasPos){
            pos.x = std::stof(node->first_attribute("posx")->value());
            pos.y = std::stof(node->first_attribute("posy")->value());
            pos.z = std::stof(node->first_attribute("posz")->value());
        }
        if (hasDir){
            dir.x = std::stof(node->first_attribute("dirx")->value());
            dir.y = std::stof(node->first_attribute("diry")->value());
            dir.z = std::stof(node->first_attribute("dirz")->value());
            dir.normalize();
        }
        if (hasDir && hasPos) cutOff = std::stof(node->first_attribute("cutoff")->value());
    };
    void prep(){
        glEnable(gl_light);
        glLightfv(gl_light, GL_AMBIENT, white);
        glLightfv(gl_light, GL_DIFFUSE, white);
        glLightfv(gl_light, GL_SPECULAR, white);
    }
    void apply(){
        if (hasDir && hasPos) {
            float posArr[4] = {pos.x, pos.y, pos.z, 1};
            glLightfv(gl_light, GL_POSITION, posArr);
            float dirArr[3] = {dir.x, dir.y, dir.z};
            glLightfv(gl_light, GL_SPOT_DIRECTION, dirArr);
            glLightfv(gl_light, GL_SPOT_CUTOFF, &cutOff);
        }
        else if (hasPos){
            float posArr[4] = {pos.x, pos.y, pos.z, 1};
            glLightfv(gl_light, GL_POSITION, posArr);
        }
        else if (hasDir){
            float dirArr[4] = {dir.x, dir.y, dir.z, 0};
            glLightfv(gl_light, GL_POSITION, dirArr);
        }
    }
};
#endif