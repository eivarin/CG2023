#ifndef WAVEFRONT_H
#define WAVEFRONT_H

#include <GL/glew.h>
#include <GL/glut.h>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <math.h>
#include "model.hpp"
#include "./rapid_xml/rapidxml.hpp"

struct color {
    float r;
    float g;
    float b;
};

class model {
    private:
        std::vector<vec3> cs;
        std::vector<vec3> ts;
        std::vector<vec3> ns;
        std::vector<face> fs;
        color clr;
        bool lines, axis;
        float axis_size;
        GLuint vertices, count;
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
        void loadModel(std::string const& fname) {
            std::ifstream file;
            file.open(fname);
            // read the first word of the line
            for(std::string line_header; file >> line_header; ) {
                if(line_header.compare("v") == 0) {
                    vec3 c;
                    file >> c.x >> c.y >> c.z;
                    cs.push_back(c);
                } else if(line_header.compare("vt") == 0) {
                    vec3 t;
                    file >> t.x >> t.y;
                    ts.push_back(t);
                } else if (line_header.compare("vn") == 0 ) {
                    vec3 n;
                    file >> n.x >> n.y >> n.z;
                    ns.push_back(n);
                } else if (line_header.compare("f") == 0 ) {
                    face f;
                    char ignore;
                    file >> f.v[0].coords >> ignore >> f.v[0].texture >> ignore >> f.v[0].normal >> f.v[1].coords >> ignore >> f.v[1].texture >> ignore >> f.v[1].normal >> f.v[2].coords >> ignore >> f.v[2].texture >> ignore >> f.v[2].normal;
                    fs.push_back(f);
                }
            }
        }
        void drawVertex(vertex_ref v_ref, std::vector<float> &vList){
            vec3 c = cs.at(v_ref.coords-1);
            // VERTEX_TEXTURE t = m.ts.at(v_ref.t-1);
            // VERTEX_NORMAL n = m.ns.at(v_ref.n-1);
            vList.push_back(c.x);
            vList.push_back(c.y);
            vList.push_back(c.z);
        }
    public:
        model(rapidxml::xml_node<> *node){
            loadModel(node->first_attribute("file")->value());
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
            int i = 0;
            std::vector<float> vList;
            for(face& f : fs) {
                i++;
                drawVertex(f.v[0], vList);
                drawVertex(f.v[1], vList);
                drawVertex(f.v[2], vList);
            }
            count = vList.size()/3;
            glGenBuffers(1, &vertices);
            glBindBuffer(GL_ARRAY_BUFFER, vertices);
            glBufferData(
                GL_ARRAY_BUFFER, // tipo do buffer, só é relevante na altura do desenho
                sizeof(float) * vList.size(), // tamanho do vector em bytes
                vList.data(), // os dados do array associado ao vector
                GL_STATIC_DRAW); // indicativo da utilização (estático e para desenho)
        }
        void drawModel(){
            glMatrixMode(GL_COLOR);
            glPushMatrix();
            if (axis) drawAxis();
            glColor3f(clr.r, clr.g, clr.b);
            glMatrixMode(GL_MODELVIEW);
            if (lines) glPolygonMode(GL_FRONT, GL_LINE);
            else glPolygonMode(GL_FRONT, GL_FILL);
            glBindBuffer(GL_ARRAY_BUFFER, vertices);
            glVertexPointer(3, GL_FLOAT, 0, 0);
            glDrawArrays(GL_TRIANGLES, 0, count);
            glMatrixMode(GL_COLOR);
            glPopMatrix();
            glMatrixMode(GL_MODELVIEW);
        }
};

bool operator<(const vec3 l, const vec3 r) {
	return true;
}

#endif