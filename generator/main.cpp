#define _USE_MATH_DEFINES
#include <math.h>

#include <array>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#include "model.hpp"

void drawPlane(std::string const& file, std::size_t length, std::size_t divisions) {
    float right_most = length / 2.;
    std::vector<float> base_points;
    float division_size = length / ((float) divisions);
    for (auto i = 0; i <= divisions; ++i) 
        base_points.push_back(right_most - (division_size * i));

    model m;
    for (auto i = 0; i <= divisions; ++i)
        for (auto j = 0; j <= divisions; ++j) 
            m.pushCoords(vertex_coords(base_points[i], 0., base_points[j]));
    /*
    16 12 8  4
    15 11 7  3
    14 10 6  2
    13 9  5  1
    */
    for (auto i = 0; i < divisions; ++i) {
        for (auto j = 1; j <= divisions; ++j) {
            auto index = (divisions + 1) * i + j;
            m.pushFace(face(index, index + 1, index + divisions + 1));
            m.pushFace(face(index + 1, index + divisions + 2, index + divisions + 1));
        }
    }
    m.write(file);
}

void drawBox(std::string const& file, std::size_t length, std::size_t divisions) {
    float right_most = length / 2.;
    float division_size = length / ((float) divisions);
    std::vector<float> base_points;
    // Base
    for (auto i = 0; i <= divisions; ++i) { // 0 - Têm de estar escondidos
        base_points.push_back(right_most - (division_size * i));
    }


    model m;
    for (auto i = 0; i <= divisions; ++i) {   // -O2, melhora-me isto
        for (auto j = 0; j <= divisions; ++j) { // -O2, melhora-me isto
            m.pushCoords(vertex_coords(base_points[i],base_points[divisions],0 - base_points[j]));
        }
    }
    // Topo
    for (auto i = 0; i <= divisions; ++i) {   // -O2, melhora-me isto
        for (auto j = 0; j <= divisions; ++j) { // -O2, melhora-me isto
            m.pushCoords(vertex_coords(base_points[i],0 - base_points[divisions],0 - base_points[j]));
        }
    }

    for (auto i = 0; i <= divisions; ++i) {
        for (auto j = 0; j <= divisions; ++j) { // -O2, melhora-me isto
            m.pushCoords(vertex_coords(base_points[divisions],base_points[i],0 - base_points[j]));
        }
    }

    for (auto i = 0; i <= divisions; ++i) {   // -O2, melhora-me isto
        for (auto j = 0; j <= divisions; ++j) { // -O2, melhora-me isto
            m.pushCoords(vertex_coords(base_points[0],base_points[i],0 - base_points[j]));
        }
    }

    for (auto i = 0; i <= divisions; ++i) {   // -O2, melhora-me isto
        for (auto j = 0; j <= divisions; ++j) { // -O2, melhora-me isto
            m.pushCoords(vertex_coords(base_points[i],(0 - base_points[j]),base_points[divisions]));
        }
    }

    for (auto i = 0; i <= divisions; ++i) {   // -O2, melhora-me isto
        for (auto j = 0; j <= divisions; ++j) { // -O2, melhora-me isto // 5
            m.pushCoords(vertex_coords(base_points[i],(0 - base_points[j]),base_points[0]));
        }
    }
    for (auto k=0;k<6;++k){
        if (k % 2 !=0  && k !=1 || k == 0 ){
            for(auto i = 0; i < divisions; ++i) {
                for(auto j = 1; j <= divisions; ++j) {
                    auto index = (divisions+1) * i + j+ k*(divisions+1)*(divisions+1);
                    m.pushFace(face(index,index+1,index+divisions+1));
                    m.pushFace(face(index + 1,index + divisions + 2,index + divisions + 1));
                }
            }
        }
        else if (k==1){
            for(auto i = 0; i < divisions; ++i) {
                for(auto j = 1; j <= divisions; ++j) {
                    auto index = (divisions+1) * i + j+ k*(divisions+1)*(divisions+1);
                    m.pushFace(face(index, index + divisions + 1, index + 1));
                    m.pushFace(face(index + 1,index + divisions + 1,index + divisions + 2));
                }
            }
        }
        else{
            for(auto i = 0; i < divisions; ++i) {
                for(auto j = 1; j <= divisions; ++j) {
                    auto index = (divisions+1) * i + j+ k*(divisions+1)*(divisions+1);
            // resultFaces.append(((index,index + divisions + 1,index+1)))
                    m.pushFace(face(index,index + divisions + 1,index+1));
            // resultFaces.append(((index + 1,index + divisions + 1,index + divisions + 2)))
                    m.pushFace(face(index + 1,index + divisions + 1,index + divisions + 2 ));
                }
            }
        }
    }
    m.write(file);
}

void draw_sphere(std::string const& file, ssize_t radius, std::size_t slices, std::size_t stacks) {
    model m;
    float stack_angle = M_PI / stacks;
    float slice_angle = (2 * M_PI) / slices;
    m.pushCoords(vertex_coords(0,radius,0));
    for (std::size_t i = 1; i < stacks; ++i) {
        for (std::size_t j = 0; j < slices; ++j) {
            float curr_b = i * stack_angle;
            float sinb = sin(curr_b);
            float x = sin(j * slice_angle) * sinb * radius;
            float y = cos(curr_b) * radius;
            float z = cos(j * slice_angle) * sinb * radius;
            m.pushCoords(vertex_coords(x, y, z));
        }
    }
    m.pushCoords(vertex_coords(0,-radius,0));

    /*
    esfera 1 4 4
              1
     5  4  3  2
     9  8  7  6
    13 12 11 10
    14
    */
    // "base" 1
    for(std::size_t i = 0; i < slices; ++i) {
        m.pushFace(face(1,(i%slices) + 2,((i+1)%slices) + 2));
    }
    // "faces"
    for(std::size_t i = 0; i < stacks - 2; ++i) {
        for(std::size_t j = 0; j < slices; ++j) {
            std::size_t r = (j%slices) + 2 + (i * slices);
            std::size_t l = ((j+1)%slices) + 2 + (i * slices);
            m.pushFace(face(r,l + slices,l));
            m.pushFace(face(r,r+slices,l+slices));

        }
    }
    // "base" 2
    std::size_t bottom_most = slices*(stacks-1)+2;
    for(std::size_t i = 0; i < slices; ++i) {
        m.pushFace(face(bottom_most,bottom_most - slices + (i+1)%slices,bottom_most - slices + (i%slices)));
    }
    m.write(file);
}

void drawCone(std::string const& file,int height, int radius, int slices, int stacks)
{   
    int points=1;
    model m;
    float delta = (2*M_PI) / (float)slices;
    float stack_h = height / (float)stacks;
    float curr_h = height;
    float stack_r = radius / (float)stacks;
    float curr_r = 0;
    int i = 0;
    curr_r += stack_r;
    while (i < slices)
    {
        float aCil = delta*i;
        float px1 = (sin(aCil)), py1 = (cos(aCil));
        float px2 = (sin(aCil + delta)), py2 = (cos(aCil + delta));
        m.pushCoords(vertex_coords(0.0f, curr_h, 0.0f));
        ++points;
        m.pushCoords(vertex_coords(px1 * curr_r, curr_h - stack_h, py1  * curr_r));
        ++points;
        m.pushCoords(vertex_coords(px2 * curr_r, curr_h - stack_h, py2  * curr_r));
        ++points;
        i++;
    }
    curr_h-=stack_h;
    while (curr_h > 0)
    {
        i = 0;
        while (i < slices)
        {
            float aCil = delta*i;
            float px1 = (sin(aCil)), py1 = (cos(aCil));
            float px2 = (sin(aCil + delta)), py2 = (cos(aCil + delta));
            
            m.pushCoords(vertex_coords(px1 * curr_r, curr_h, py1 * curr_r));
            ++points;
            m.pushCoords(vertex_coords(px1 * (curr_r+stack_r), curr_h - stack_h, py1 * (curr_r+stack_r)));
            ++points;
            m.pushCoords(vertex_coords(px2 * (curr_r+stack_r), curr_h - stack_h, py2 * (curr_r+stack_r)));
            ++points;

            m.pushCoords(vertex_coords(px2 * curr_r, curr_h, py2 * curr_r));
            ++points;
            m.pushCoords(vertex_coords(px1 * curr_r, curr_h, py1 * curr_r));
            ++points;
            m.pushCoords(vertex_coords(px2 * (curr_r+stack_r), curr_h - stack_h, py2 * (curr_r+stack_r)));
            ++points;
            i++;
        }
        curr_h-=stack_h;
        curr_r+=stack_r;
    }
    i=0;
    while (i < slices)
    {
        float aCil = delta*i;
        float px1 = (sin(aCil)), py1 = (cos(aCil));
        float px2 = (sin(aCil + delta)), py2 = (cos(aCil + delta));
        m.pushCoords(vertex_coords(px2 * curr_r, curr_h, py2  * curr_r));
        ++points;
        m.pushCoords(vertex_coords(px1 * curr_r, curr_h, py1  * curr_r));
        ++points;
        m.pushCoords(vertex_coords(0.0f, curr_h, 0.0f));
        ++points;
        i++;
    }

    for (auto i=1;i+3<=points;i=i+3){
        m.pushFace(face(i,i+1,i+2));
    }
    m.write(file);
}
int main(int argc, char** argv) {
    std::string s1 = "plane";
    std::string s2 = "box";
    // input line to draw a plane:  ./generator plane length divisions
    // NameOfFileToOutput
    if (argc == 5 && s1.compare(argv[1]) == 0) {
        std::size_t length = std::stoul(argv[2]);
        std::size_t divisions = std::stoul(argv[3]);
        drawPlane(argv[4], length, divisions);

    }
    // input line to draw a box: ./generator box length divisions
    // NameofFileToOutput
    else if (argc == 5 && s2.compare(argv[1]) == 0) {
        std::size_t length = std::stoul(argv[2]);
        std::size_t divisions = std::stoul(argv[3]);
        drawBox(argv[4], length, divisions); // Plano de baixo da box
    }
    // input line to draw a sphere: ./generator sphere radius slices stacks
    // NameofFileToOutput
    else if (argc == 6 && std::string("sphere").compare(argv[1]) == 0) {
        ssize_t radius = std::stol(argv[2]);
        std::size_t slices = std::stoul(argv[3]);
        std::size_t stacks = std::stoul(argv[4]);
        draw_sphere(argv[5], radius, slices, stacks);
    }
    // input line to draw a cone: ./generator cone radius heigth slices stacks NameofFileToOutput
    else if (argc == 7 && std::string("cone").compare(argv[1]) == 0) {
        int radius = atoi(argv[2]);
        int heigth = atoi(argv[3]);
        int slices = atoi(argv[4]);
        int stacks = atoi(argv[5]);
        drawCone(argv[6],heigth, radius, slices, stacks);
    }
    return 0;
}
