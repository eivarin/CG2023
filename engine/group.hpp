#include<string>
#include<math.h>

#include <vector>
#include "transformation.hpp"
#include "wavefront.hpp"
#include "./rapid_xml/rapidxml.hpp"
#include <GL/glew.h>
#include <GL/glut.h>
#include <memory>
#include <sstream>
#include <string>

class group{
    std::vector<transformation*> ts;
    std::vector<model> ms;
    std::vector<group> gs;
    GLfloat matrix[16];
    void loadGroup(rapidxml::xml_node<> *node){
        for (rapidxml::xml_node<> *n = node->first_node(); n; n = n->next_sibling())
        {
            std::string tipo = n->name();
            if (tipo.compare("transform") == 0){
                for (rapidxml::xml_node<> *t = n->first_node(); t; t = t->next_sibling())
                {
                    ts.push_back(transformation::from_rapidxml_node(t));
                }
            }
            else if (tipo.compare("models") == 0){
                for (rapidxml::xml_node<> *m = n->first_node(); m; m = m->next_sibling())
                {
                    ms.push_back(model(m));
                }
            }
            else if (tipo.compare("group") == 0){
                gs.push_back(group(n));
            }
        }
    }
    public:
        group(){
            
        }
        group(rapidxml::xml_node<> *node){
            auto attr = node->first_attribute("file");
            if (attr!=0) {
                rapidxml::xml_document<> doc;
                std::ifstream file(attr->value());
                std::stringstream buffer;
                buffer << file.rdbuf();
                file.close();
                std::string content(buffer.str());
                doc.parse<0>(&content[0]);
                auto loaded_group = doc.first_node("world")->first_node("group");
                loadGroup(loaded_group);
            }
            else loadGroup(node);
        }
        void prepGroup(){
            for(auto& m : ms){
                m.prepModel();
            }
            for(auto& g : gs){
                g.prepGroup();
            }
        }
        void applyTransfs(){
            for(auto& t : ts){
                t->apply();
            }
            glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
        }
        void drawGroup(){
            glPushMatrix();
            applyTransfs();
            for (auto& m : ms){
                m.drawModel();
            }
            for(auto& g :gs){
                g.drawGroup();
            }
            glPopMatrix();
        }

};
