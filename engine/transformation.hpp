#include<string>
#include<math.h>
#include "wavefront.hpp"
#include "./rapid_xml/rapidxml.hpp"
#include <GL/glew.h>
#include <GL/glut.h>
#include <memory>

enum t_tipo : unsigned char { T, R, S};

class transformation{
    public:
        t_tipo t;
        transformation(){
            t = T;
        }
        virtual ~transformation() = default;
        transformation(rapidxml::xml_node<> *node){
            std::string tipo = node->name();
            if (tipo.compare("translate") == 0){
                t = T;
            }
            else if (tipo.compare("rotate") == 0){
                t = R;
            }
            else if (tipo.compare("scale") == 0){
                t = S;
            }
        }
        virtual void apply()=0;
        static std::unique_ptr<transformation> from_rapidxml_node(rapidxml::xml_node<> *node);
};

class static_transf : public transformation{
    vertex_coords vx;
    float angle;
    public:
        static_transf(rapidxml::xml_node<> *node) : transformation(node){
            if (t == R) angle = std::stof(node->first_attribute("angle")->value());
            vx.x = std::stof(node->first_attribute("x")->value());
            vx.y = std::stof(node->first_attribute("y")->value());
            vx.z = std::stof(node->first_attribute("z")->value());
        }

        void apply() override{
            switch (t)
            {
            case T:
                glTranslatef(vx.x, vx.y, vx.z);
                break;
            case R:
                glRotatef(angle,vx.x, vx.y, vx.z);
                break;
            case S:
                glScalef(vx.x, vx.y, vx.z);
                break;
            default:
                break;
            }
        }
};

class animated_transf : public transformation{
    vertex_coords vx;
    std::vector<vertex_coords> points;
    float time;
    bool align;
    bool check_align(rapidxml::xml_node<> *transf){
        auto r = false;
        auto atrb = transf->first_attribute("align");
        if (atrb != 0){
            std::string s = atrb->value();
            r = s.compare("true") || s.compare("True") || s.compare("1");
        }
        return r;
    }
    public:
        animated_transf(rapidxml::xml_node<> *node) : transformation(node){
            time = std::stof(node->first_attribute("time")->value());
            if (t == R){
                vx.x = std::stof(node->first_attribute("x")->value());
                vx.y = std::stof(node->first_attribute("y")->value());
                vx.z = std::stof(node->first_attribute("z")->value());
            }
            else if (t == T){
                align = check_align(node);
                for (rapidxml::xml_node<> *n = node->first_node(); n; n = n->next_sibling())
                {
                    std::string nome = n->name();
                    if (nome.compare("point") == 0){
                        vertex_coords new_v;
                        new_v.x = std::stof(n->first_attribute("x")->value());
                        new_v.y = std::stof(n->first_attribute("y")->value());
                        new_v.z = std::stof(n->first_attribute("z")->value());
                        points.push_back(new_v);
                    }
                }
            }
        }

        void apply() override{
            int curr_time = glutGet(GLUT_ELAPSED_TIME);
            switch (t)
            {
            case T:
                // wrong
                glTranslatef(vx.x, vx.y, vx.z);
                break;
            case R:
                glRotatef(curr_time / time ,vx.x, vx.y, vx.z);
                break;
            default:
                break;
            }
        }
};

std::unique_ptr<transformation> transformation::from_rapidxml_node(rapidxml::xml_node<> *node){
    std::unique_ptr<transformation> t;
    if (node->first_attribute("time")){
        t = std::make_unique<animated_transf>(node);
    }
    else
    {
        t = std::make_unique<static_transf>(node);
    }
    return t;
}
