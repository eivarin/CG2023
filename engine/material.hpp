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
    float shininess;
public:
    material(){

    }
    material(rapidxml::xml_node<> *node){
        diffuse.x = std::stof(node->first_node("diffuse")->first_attribute("R")->value())/255.0;
        diffuse.y = std::stof(node->first_node("diffuse")->first_attribute("G")->value())/255.0;
        diffuse.z = std::stof(node->first_node("diffuse")->first_attribute("B")->value())/255.0;

        ambient.x = std::stof(node->first_node("ambient")->first_attribute("R")->value())/255.0;
        ambient.y = std::stof(node->first_node("ambient")->first_attribute("G")->value())/255.0;
        ambient.z = std::stof(node->first_node("ambient")->first_attribute("B")->value())/255.0;

        specular.x = std::stof(node->first_node("specular")->first_attribute("R")->value())/255.0;
        specular.y = std::stof(node->first_node("specular")->first_attribute("G")->value())/255.0;
        specular.z = std::stof(node->first_node("specular")->first_attribute("B")->value())/255.0;

        emissive.x = std::stof(node->first_node("emissive")->first_attribute("R")->value())/255.0;
        emissive.y = std::stof(node->first_node("emissive")->first_attribute("G")->value())/255.0;
        emissive.z = std::stof(node->first_node("emissive")->first_attribute("B")->value())/255.0;

        shininess = std::stof(node->first_node("shininess")->first_attribute("value")->value());
    }

    void apply(){
        float diffuseArr[4] = {diffuse.x, diffuse.y, diffuse.z, 1.0};
        glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuseArr);
        float ambientArr[4] = {ambient.x, ambient.y, ambient.z, 1.0};
        glMaterialfv(GL_FRONT, GL_AMBIENT, ambientArr);
        float specularArr[4] = {specular.x, specular.y, specular.z, 1.0};
        glMaterialfv(GL_FRONT, GL_SPECULAR, specularArr);
        float emissiveArr[4] = {emissive.x, emissive.y, emissive.z, 1.0};
        glMaterialfv(GL_FRONT, GL_EMISSION, emissiveArr);
        glMaterialf(GL_FRONT, GL_SHININESS, shininess);
    }
};


#endif