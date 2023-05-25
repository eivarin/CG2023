#ifndef WAVEFRONT_H
#define WAVEFRONT_H

#include <GL/glew.h>
#include <GL/glut.h>
#include <vector>
#include <string>
#include <map>
#include <fstream>
#include <iostream>
#include <math.h>
#include "VBO.hpp"
#include "model.hpp"
#include "./rapid_xml/rapidxml.hpp"
#include "texture.hpp"
#include "material.hpp"
#include "shared.hpp"

std::map<std::string, int> vbo_map;
std::vector<VBO> vbo_array;
std::map<std::string, int> texture_map;
std::vector<texture> texture_array;

class model {
    private:
        bool lines, axis, hasTexture = false, hasMaterial = false, forceMaterial = false;
        float axis_size;
        int vbo_id, texture_id;
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
        model(rapidxml::xml_node<> *node){
            std::string fname = node->first_attribute("file")->value();
            if(vbo_map.count(fname) > 0){
                vbo_id = vbo_map[fname];
            }
            else{
                auto v = VBO(node->first_attribute("file")->value());
                vbo_id = vbo_array.size();
                vbo_map[fname] = vbo_id;
                vbo_array.push_back(v);
            }
            lines = check_lines(node);
            axis = check_draw_axis(node);
            auto tex_node = node->first_node("texture");
            if (tex_node != 0)
            {
                fname = tex_node->first_attribute("file")->value();
                if(texture_map.count(fname) > 0){
                    texture_id = texture_map[fname];
                }
                else{
                    auto t = texture(tex_node->first_attribute("file")->value());
                    texture_id = texture_array.size();
                    texture_map[fname] = texture_id;
                    texture_array.push_back(t);
                }
                hasTexture = true;
            }
            auto mat_node = node->first_node("color");
            if (mat_node!=0)
            {
                mat = material(mat_node);
                forceMaterial = (mat_node->first_attribute("force") != 0);
                hasMaterial = true;
            }
        }

        void prepModel(){
            vbo_array[vbo_id].prep();
            if (hasTexture) texture_array[texture_id].prep();
        }
        void drawModel(){
            glPushAttrib(GL_LIGHTING_BIT);
            if (axis) drawAxis(axis_size);

            if (hasTexture) texture_array[texture_id].apply();
            else if (hasMaterial) mat.apply();
            if (forceMaterial) mat.apply();
            vbo_array[vbo_id].draw();
            glBindTexture(GL_TEXTURE_2D, 0);
            glPopAttrib();
        }
};

bool operator<(const vec3 l, const vec3 r) {
	return true;
}

#endif