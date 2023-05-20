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
#include "light.hpp"
#include "shared.hpp"

GLenum ls[8] = { GL_LIGHT0, GL_LIGHT1, GL_LIGHT2, GL_LIGHT3, GL_LIGHT4, GL_LIGHT5, GL_LIGHT6, GL_LIGHT7};

struct scene {
	camera cam;
	group main_group;
	std::vector<light> lights;
	int wHeight, wWidth, timebase, frames;
	bool needsAxis, normal_keys[256], special_keys[1024], coordsMenu = false;
	void prep(){
		for (auto& l: lights) l.prep();
		main_group.prepGroup();
	}
	void draw(){
		cam.placeGlut();
		for (auto& l: lights) l.apply();
		if (needsAxis) drawAxis(1000000);
		glPolygonMode(GL_FRONT, GL_FILL);
		main_group.drawGroup();
		if (coordsMenu) cam.drawCoords(wWidth,wHeight);
	}
};

bool check_draw_axis(rapidxml::xml_node<> *window){
	auto r = true;
	auto atrb = window->first_attribute("axis");
	if (atrb != 0){
		std::string s = atrb->value();
		r = (s.compare("true") == 0) || (s.compare("True") == 0) || (s.compare("1") == 0);
	}
	return r;
}

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
		.wWidth = std::stoi(window->first_attribute("width")->value()),
		.needsAxis = check_draw_axis(window)
	};
	rapidxml::xml_node<> *lights_node = world->first_node("lights");
	rapidxml::xml_node<> *n = lights_node->first_node();
	if (n) for (size_t i = 0; i < 8 && n; n = n->next_sibling()) s.lights.push_back(light(n, ls[i++]));
	return s;
}

#endif