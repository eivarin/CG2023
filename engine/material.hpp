#ifndef MATERIAL_H
#define MATERIAL_H

#include <GL/glew.h>
#include <GL/glut.h>
#include <vector>
#include <string>
#include "model.hpp"
#include "./rapid_xml/rapidxml.hpp"


/*
<color>
    <diffuse R="200" G="200" B="000" />
    <ambient R="50" G="50" B="00" />
    <specular R="0" G="0" B="0" />
    <emissive R="0" G="0" B="0" />
    <shininess value="0" />
</color>
*/

class material
{
private:
    vec3 diffuse, ambient, specular, emissive;
    bool has_diffuse = false, has_ambient = false, has_specular = false, has_emissive = false, has_shininess = false;
    float shininess;
public:
    material(){

    }
    material(rapidxml::xml_node<> *node){
        auto n = node->first_node("diffuse");
        if (n != 0)
        {
            diffuse.x = std::stof(n->first_attribute("R")->value())/255.0;
            diffuse.y = std::stof(n->first_attribute("G")->value())/255.0;
            diffuse.z = std::stof(n->first_attribute("B")->value())/255.0;
            has_diffuse = true;
        }
        

        n = node->first_node("ambient");
        if (n != 0)
        {
            ambient.x = std::stof(n->first_attribute("R")->value())/255.0;
            ambient.y = std::stof(n->first_attribute("G")->value())/255.0;
            ambient.z = std::stof(n->first_attribute("B")->value())/255.0;
            has_ambient = true;
        }

        n = node->first_node("specular");
        if (n != 0)
        {
            specular.x = std::stof(n->first_attribute("R")->value())/255.0;
            specular.y = std::stof(n->first_attribute("G")->value())/255.0;
            specular.z = std::stof(n->first_attribute("B")->value())/255.0;
            has_specular = true;
        }

        n = node->first_node("emissive");
        if (n != 0)
        {
            emissive.x = std::stof(n->first_attribute("R")->value())/255.0;
            emissive.y = std::stof(n->first_attribute("G")->value())/255.0;
            emissive.z = std::stof(n->first_attribute("B")->value())/255.0;
            has_emissive = true;
        }


        n = node->first_node("shininess");
        if (n != 0)
        {
            shininess = std::stof(n->first_attribute("value")->value());
            has_shininess = true;
        }
    }

    void apply(){
        float diffuseArr[4] = {diffuse.x, diffuse.y, diffuse.z, 1.0};
        float ambientArr[4] = {ambient.x, ambient.y, ambient.z, 1.0};
        float specularArr[4] = {specular.x, specular.y, specular.z, 1.0};
        float emissiveArr[4] = {emissive.x, emissive.y, emissive.z, 1.0};
        if (has_diffuse) glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuseArr);
        if (has_ambient) glMaterialfv(GL_FRONT, GL_AMBIENT, ambientArr);
        if (has_specular) glMaterialfv(GL_FRONT, GL_SPECULAR, specularArr);
        if (has_emissive) glMaterialfv(GL_FRONT, GL_EMISSION, emissiveArr);
        if (has_shininess) glMaterialf(GL_FRONT, GL_SHININESS, shininess);
    }
};


#endif