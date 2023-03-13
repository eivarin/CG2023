#include<string>
#include<math.h>
#include "wavefront.hpp"
#include "./rapid_xml/rapidxml.hpp"
#include <GL/glew.h>
#include <GL/glut.h>

enum t_tipo : unsigned char { T, R, S};

struct transformation {
    vertex_coords vector;
    float angle;
    t_tipo t;
    transformation(rapidxml::xml_node<> *node){
        std::string tipo = node->name();
        if (tipo.compare("translate") == 0){
            t = T;
        }
        else if (tipo.compare("rotate") == 0){
            t = R;
            angle = std::stof(node->first_attribute("angle")->value());
        }
        else if (tipo.compare("scale") == 0){
            t = S;
        }
        vector.x = std::stof(node->first_attribute("x")->value());
        vector.y = std::stof(node->first_attribute("y")->value());
        vector.z = std::stof(node->first_attribute("z")->value());
    }

    void apply(){
        switch (t)
        {
        case T:
            glTranslatef(vector.x, vector.y, vector.z);
            break;
        case R:
            glRotatef(angle,vector.x, vector.y, vector.z);
            break;
        case S:
            glScalef(vector.x, vector.y, vector.z);
            break;
        default:
            break;
        }
    }
};