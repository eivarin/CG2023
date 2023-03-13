#include<string>
#include<math.h>

#include <vector>
#include "transformation.hpp"
#include "wavefront.hpp"
#include "./rapid_xml/rapidxml.hpp"
#include <GL/glew.h>
#include <GL/glut.h>

std::vector<float> mergeVector(std::vector<float> v1, std::vector<float> v2){
	v1.insert( v1.end(), v2.begin(), v2.end() );
	return v1;
}

class group{
    std::vector<transformation> ts;
    std::vector<model> ms;
    std::vector<group> gs;
    GLfloat matrix[16];
    public:
        group(){
            
        }
        group(rapidxml::xml_node<> *node){
            for (rapidxml::xml_node<> *n = node->first_node(); n; n = n->next_sibling())
            {
                std::string tipo = n->name();
                if (tipo.compare("transform") == 0){
                    for (rapidxml::xml_node<> *t = n->first_node(); t; t = t->next_sibling())
                    {
                        ts.push_back(transformation(t));
                    }
                }
                else if (tipo.compare("models") == 0){
                    for (rapidxml::xml_node<> *m = n->first_node(); m; m = m->next_sibling())
                    {
                        ms.push_back(model(m->first_attribute("file")->value()));
                    }
                }
                else if (tipo.compare("group") == 0){
                    gs.push_back(group(n));
                }
            }
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
                t.apply();
                glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
            }
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
