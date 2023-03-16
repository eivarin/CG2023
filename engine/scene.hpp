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
#include "group.hpp"
struct scene {
	camera cam;
	group main_group;
	int wHeight;
	int wWidth;
	bool normal_keys[256];
	bool special_keys[1024];
	bool coordsMenu = false;
	int timebase;
	int frames;
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
	rapidxml::xml_node<> *group_node = world->first_node("group");
	scene s = {
		.cam = camera(camera_node),
		.main_group = group(group_node),
		.wHeight = std::stoi(window->first_attribute("height")->value()),
		.wWidth = std::stoi(window->first_attribute("width")->value())
	};
	return s;
}

#endif