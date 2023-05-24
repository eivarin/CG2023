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
#include "texture.hpp"
#include "material.hpp"
#include "shared.hpp"

class model {
    private:
        bool lines, axis, hasTexture = false, hasMaterial = false;
        float axis_size;
        VBO m;
        texture t;
        material mat;
        
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
            lines = check_lines(node);
            axis = check_draw_axis(node);
            auto tex_node = node->first_node("texture");
            if (tex_node != 0)
            {
                t = texture(tex_node->first_attribute("file")->value());
                hasTexture = true;
            }
            auto mat_node = node->first_node("color");
            if (mat_node!=0)
            {
                mat = material(mat_node);
                hasMaterial = true;
            }
        }

        void prepModel(){
            m.prep();
            if (hasTexture) t.prep();
        }
        void drawModel(){
            glPushAttrib(GL_LIGHTING_BIT);
            if (axis) drawAxis(axis_size);
            if (hasTexture) t.apply();
            else if (hasMaterial) mat.apply();
            m.draw();
            glBindTexture(GL_TEXTURE_2D, 0);
            glPopAttrib();
        }
};

bool operator<(const vec3 l, const vec3 r) {
	return true;
}

#endif