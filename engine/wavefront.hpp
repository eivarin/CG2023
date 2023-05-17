#ifndef WAVEFRONT_H
#define WAVEFRONT_H

#include <GL/glew.h>
#include <GL/glut.h>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <math.h>
#include "VBO.hpp"
#include "model.hpp"
#include "./rapid_xml/rapidxml.hpp"

struct color {
    float r;
    float g;
    float b;
};

class model {
    private:
        color clr;
        bool lines, axis;
        float axis_size;
        VBO m;
        void drawAxis(){
            glBegin(GL_LINES);
            float size = axis_size;
            glColor3f(1.0f, 0.0f, 0.0f);
            glVertex3f(-size, 0.0f, 0.0f);
            glVertex3f( size, 0.0f, 0.0f);
            glColor3f(0.0f, 1.0f, 0.0f);
            glVertex3f(0.0f,-size, 0.0f);
            glVertex3f(0.0f, size, 0.0f);
            glColor3f(0.0f, 0.0f, 1.0f);
            glVertex3f(0.0f, 0.0f,-size);
            glVertex3f(0.0f, 0.0f, size);
            glEnd();
        }
        bool check_draw_axis(rapidxml::xml_node<> *model){
            auto atrb = model->first_attribute("axis");
            if (atrb != 0){
                axis_size = std::stof(model->first_attribute("axis")->value());
            }
            return atrb;
        }
        bool check_lines(rapidxml::xml_node<> *model){
            auto r = true;
            auto atrb = model->first_attribute("lines");
            if (atrb != 0){
                std::string s = atrb->value();
                r = (s.compare("true") == 0) || (s.compare("True") == 0) || (s.compare("1") == 0);
            }
            return r;
        }
    public:
        model(rapidxml::xml_node<> *node) : m(node->first_attribute("file")->value()){
            rapidxml::xml_attribute<char> *r_atrib = node->first_attribute("r");
            if (r_atrib){
                clr.r = std::stof(r_atrib->value());
                clr.g = std::stof(node->first_attribute("g")->value());
                clr.b = std::stof(node->first_attribute("b")->value());
            }
            else {
                clr = {.r = 1.0f, .g = 1.0f, .b = 1.0f };
            }
            lines = check_lines(node);
            axis = check_draw_axis(node);
        }

        void prepModel(){
            m.prep();
        }
        void drawModel(){
            glMatrixMode(GL_COLOR);
            glPushMatrix();
            if (axis) drawAxis();
            glColor3f(clr.r, clr.g, clr.b);
            glMatrixMode(GL_MODELVIEW);
            if (lines) glPolygonMode(GL_FRONT, GL_LINE);
            else glPolygonMode(GL_FRONT, GL_FILL);
            m.draw(GL_TRIANGLES);
            glMatrixMode(GL_COLOR);
            glPopMatrix();
            glMatrixMode(GL_MODELVIEW);
        }
};

bool operator<(const vec3 l, const vec3 r) {
	return true;
}

#endif