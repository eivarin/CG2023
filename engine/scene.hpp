#ifndef SCENE_H
#define SCENE_H


#include <GL/glew.h>
#include <GL/glut.h>
#include <fstream>
#include <iostream>
#include <map>
#include <vector>
#include <string>
#include "./rapid_xml/rapidxml.hpp"
#include "wavefront.hpp"
#include "camera.hpp"

struct scene {
	camera cam;
	std::vector<model> ms;
	int wHeight;
	int wWidth;
	bool normal_keys[256];
	bool special_keys[1024];
	GLuint vertices, verticeCount;
};

scene loadScene(std::string const& fname){
	std::ifstream file(fname, std::ios::binary | std::ios::ate);
	std::streamsize size = file.tellg();
	file.seekg(0, std::ios::beg);
	std::vector<char> buffer(size);
	file.read(buffer.data(), size);
	
	rapidxml::xml_document<> doc;
	doc.parse<0>(reinterpret_cast<char*>(buffer.data()));
	rapidxml::xml_node<> *world = doc.first_node("world");
	rapidxml::xml_node<> *window = world->first_node("window");
	rapidxml::xml_node<> *camera_node = world->first_node("camera");
	rapidxml::xml_node<> *models = world->first_node("group")->first_node("models");
	scene s = {
		.cam = camera(camera_node),
		.wHeight = std::stoi(window->first_attribute("height")->value()),
		.wWidth = std::stoi(window->first_attribute("width")->value()),
	};
	for (rapidxml::xml_node<> *m = models->first_node(); m; m = m->next_sibling())
	{
		s.ms.push_back(model(m->first_attribute("file")->value()));
	}
	return s;
}

#endif