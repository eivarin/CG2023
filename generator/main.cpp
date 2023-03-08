#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define _USE_MATH_DEFINES
#include <math.h>

#include <array>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
// #include <unordered_map>
#include <string>
#include <vector>

void drawPlane(std::string const& file, std::size_t length,
    std::size_t divisions) {
    float right_most = (length * divisions) / 2.;
    std::vector<float> base_points;
    for (auto i = 0; i <= divisions; ++i) {
        float chunk = (float) length/(float)divisions;
        base_points.push_back(right_most - (chunk)*i);
    }

    std::ofstream f;
    f.open(file);
    for (auto i = 0; i <= divisions; ++i) {   // -O2, melhora-me isto
        for (auto j = 0; j <= divisions; ++j) { // -O2, melhora-me isto
            f << "v " << std::setprecision(6) << base_points[i] << ' ' << 0. << ' '
                << (0 - base_points[j]) << '\n';
        }
    }
    /*
    15 10  5
    14  9  4
    13  8  3
    12  7  2
    11  6  1
    */
    for (auto i = 0; i < divisions; ++i) {
        for (auto j = 1; j <= divisions; ++j) {
            auto index = (divisions + 1) * i + j;
            f << "f " << index << "/0/0 ";
            f << index + divisions + 1 << "/0/0 ";
            f << index + 1 << "/0/0\n";
            f << "f " << index + 1 << "/0/0 ";
            f << index + divisions + 1 << "/0/0 ";
            f << index + divisions + 2 << "/0/0\n";
        }
    }
    f.close();
}

void drawBox(std::string const& file, std::size_t length,
    std::size_t divisions) {
    float right_most = (length * divisions) / 2.;
    std::vector<float> base_points;
    float chunk = (float) length/(float)divisions;
    // Base
    for (auto i = 0; i <= divisions; ++i) { // 0 - Têm de estar escondidos
        base_points.push_back(right_most - ((length) * i));
    }

    std::ofstream f;
    f.open(file);
    for (auto i = 0; i <= divisions; ++i) {   // -O2, melhora-me isto
        for (auto j = 0; j <= divisions; ++j) { // -O2, melhora-me isto
            f << "v " << std::setprecision(6) << base_points[i]*chunk << ' '
                << base_points[divisions]*chunk << ' ' << (0 - base_points[j])*chunk<< '\n';
        }
    }
    // Topo
    for (auto i = 0; i <= divisions; ++i) {   // -O2, melhora-me isto
        for (auto j = 0; j <= divisions; ++j) { // -O2, melhora-me isto
            f << "v " << std::setprecision(6) << base_points[i]*chunk << ' '
                << -(base_points[divisions])*chunk<< ' ' << (0 - base_points[j])*chunk
                << '\n'; // 1
        }
    }

    for (auto i = 0; i <= divisions; ++i) {
        for (auto j = 0; j <= divisions; ++j) { // -O2, melhora-me isto
            f << "v " << std::setprecision(6) << base_points[divisions]*chunk << ' '
                << base_points[i]*chunk << ' ' << (0 - base_points[j])*chunk
                << '\n'; // 2 - Têm de estar escondidas
        }
    }

    for (auto i = 0; i <= divisions; ++i) {   // -O2, melhora-me isto
        for (auto j = 0; j <= divisions; ++j) { // -O2, melhora-me isto
            f << "v " << std::setprecision(6) << base_points[0]*chunk << ' '
                << base_points[i]*chunk << ' ' << (0 - base_points[j])*chunk << '\n'; // 3
        }
    }

    for (auto i = 0; i <= divisions; ++i) {   // -O2, melhora-me isto
        for (auto j = 0; j <= divisions; ++j) { // -O2, melhora-me isto
            f << "v " << std::setprecision(6) << base_points[i]*chunk << ' '
                << (0 - base_points[j])*chunk << ' ' << base_points[divisions]*chunk
                << '\n'; // 4- Têm de estar escondidas
        }
    }

    for (auto i = 0; i <= divisions; ++i) {   // -O2, melhora-me isto
        for (auto j = 0; j <= divisions; ++j) { // -O2, melhora-me isto // 5
            f << "v " << std::setprecision(6) << base_points[i]*chunk << ' '
                << (0 - base_points[j])*chunk << ' ' << base_points[0]*chunk << '\n';
        }
    }
    /* Base
    20  15 10  5
    19  14  9  4
    18  13  8  3
    17  12  7  2
    16  11  6  1
    Topo
    40  35 30  25
    39  34  29  24
    38  33  28  23
    37  32  27  22
    36  31  26  21

    */
   for (auto k=0;k<6;++k){
        if (k % 2 !=0  && k !=1 || k == 0 ){
            for(auto i = 0; i < divisions; ++i) {
                for(auto j = 1; j <= divisions; ++j) {
                    auto index = (divisions+1) * i + j+ k*(divisions+1)*(divisions+1);
                    f << "f " << index << "/0/0 ";
                    f << index + 1 << "/0/0 ";
                    f << index + divisions + 1 << "/0/0\n";
                    f << "f " << index + 1 << "/0/0 ";
                    f << index + divisions + 2 << "/0/0 ";
                    f << index + divisions + 1 << "/0/0\n";
                }
            }
        }
        else if (k==1){
            for(auto i = 0; i < divisions; ++i) {
                for(auto j = 1; j <= divisions; ++j) {
                    auto index = (divisions+1) * i + j+ k*(divisions+1)*(divisions+1);
                    f << "f " << index << "/0/0 ";
                    f << index + divisions + 1 << "/0/0 ";
                    f << index + 1 << "/0/0\n";
                    f << "f " << index + 1 << "/0/0 ";
                    f << index + divisions + 1 << "/0/0 ";
                    f << index + divisions + 2 << "/0/0\n";
                }
            }
        }
        else{
            for(auto i = 0; i < divisions; ++i) {
                for(auto j = 1; j <= divisions; ++j) {
                    auto index = (divisions+1) * i + j+ k*(divisions+1)*(divisions+1);
                    f << "f " << index << "/0/0 ";
                    f << index + divisions + 1 << "/0/0 ";
                    f << index + 1 << "/0/0\n";
                    f << "f " << index+1 << "/0/0 ";
                    f << index + divisions + 1 << "/0/0 ";
                    f << index + divisions + 2 << "/0/0\n";
                }
            }
        }
    }
    f.close();
}

void draw_sphere(std::string const& fname, ssize_t radius, std::size_t slices, std::size_t stacks) {
    std::ofstream f;
    f.open(fname);
    float stack_angle = M_PI / stacks;
    float slice_angle = (2 * M_PI) / slices;
    f << "v " << 0 << ' ' << radius << ' ' << 0 << '\n';
    for (std::size_t i = 1; i < stacks; ++i) {
        for (std::size_t j = 0; j < slices; ++j) {
            float sinb = sin((i * M_PI) / stacks);
            f << "v " << std::noshowpoint << std::fixed << std::setprecision(6)
                << sin(j * (2 * M_PI / slices)) * sinb * radius
                << ' ' << cos((i * M_PI) / stacks) * radius
                << ' ' << cos(j * (2 * M_PI / slices)) * sinb * radius << '\n';
        }
    }
    f << "v " << 0 << ' ' << -radius << ' ' << 0 << '\n';

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
        f << "f " << 1 << "/0/0 ";
        f << (i%slices) + 2 << "/0/0 ";
        f << ((i+1)%slices) + 2 << "/0/0\n";
    }
    // "faces"
    for(std::size_t i = 0; i < stacks - 2; ++i) {
        for(std::size_t j = 0; j < slices; ++j) {
            std::size_t r = (j%slices) + 2 + (i * slices);
            std::size_t l = ((j+1)%slices) + 2 + (i * slices);
            // |/
            f << "f " << r << "/0/0 ";
            f << l + slices << "/0/0 ";
            f << l << "/0/0\n";
            // \|
            f << "f " << r << "/0/0 ";
            f << r + slices << "/0/0 ";
            f << l + slices << "/0/0\n";

        }
    }
    // "base" 2
    std::size_t bottom_most = slices*(stacks-1)+2;
    for(std::size_t i = 0; i < slices; ++i) {
        f << "f " << bottom_most << "/0/0 ";
        f << bottom_most - slices + (i+1)%slices << "/0/0 ";
        f << bottom_most - slices + (i%slices) << "/0/0\n";
    }
}
void drawCone(std::string const& file,int height, int radius, int slices, int stacks)
{   
    int points=1;
    std::ofstream f;
    f.open(file);
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
        // glVertex3f(0.0f, curr_h, 0.0f);
        f << "v " << std::setprecision(6) << 0.0 << ' '<< curr_h<< ' ' << 0.0 << '\n';
        ++points;
        // glVertex3f(px1 * curr_r, curr_h - stack_h, py1  * curr_r);
        f << "v " << std::setprecision(6) << px1 * curr_r << ' '<<curr_h - stack_h<< ' ' << py1  * curr_r << '\n';
        ++points;
        // glVertex3f(px2 * curr_r, curr_h - stack_h, py2  * curr_r);
        f << "v " << std::setprecision(6) << px2 * curr_r << ' '<<curr_h - stack_h<< ' ' << py2  * curr_r << '\n';
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
            
            //glVertex3f(px1 * curr_r, curr_h, py1 * curr_r);
            f << "v " << std::setprecision(6) << px1 * curr_r << ' '<<curr_h << ' ' <<py1 * curr_r<< '\n';
            ++points;
            //glVertex3f(px1 * (curr_r+stack_r), curr_h - stack_h, py1 * (curr_r+stack_r));
            f << "v " << std::setprecision(6) << px1 * (curr_r+stack_r) << ' '<<curr_h - stack_h << ' ' << py1 * (curr_r+stack_r)<< '\n';
            ++points;
            //glVertex3f(px2 * (curr_r+stack_r), curr_h - stack_h, py2 * (curr_r+stack_r));
            f << "v " << std::setprecision(6) << px2 * (curr_r+stack_r) << ' '<<curr_h - stack_h<< ' ' <<py2 * (curr_r+stack_r) << '\n';
            ++points;

            //glVertex3f(px2 * curr_r, curr_h, py2 * curr_r);
            f << "v " << std::setprecision(6) << px2 * curr_r << ' '<< curr_h<< ' ' <<py2 * curr_r<< '\n';
            ++points;
            //glVertex3f(px1 * curr_r, curr_h, py1 * curr_r);
            f << "v " << std::setprecision(6) << px1 * curr_r << ' '<<curr_h<< ' ' <<py1 * curr_r << '\n';
            ++points;
            // glVertex3f(px2 * (curr_r+stack_r), curr_h - stack_h, py2 * (curr_r+stack_r));
            f << "v " << std::setprecision(6) << px2 * (curr_r+stack_r) << ' '<<curr_h - stack_h<< ' ' <<py2 * (curr_r+stack_r) << '\n';
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
        //glVertex3f(px2 * curr_r, curr_h, py2  * curr_r);
        f << "v " << std::setprecision(6) << px2 * curr_r << ' '<< curr_h<< ' ' <<py2  * curr_r<< '\n';
        ++points;
        //glVertex3f(px1 * curr_r, curr_h, py1  * curr_r);
        f << "v " << std::setprecision(6) << px1 * curr_r << ' '<< curr_h<< ' ' <<py1  * curr_r<< '\n';
        ++points;
        //glVertex3f(0.0f, curr_h, 0.0f);
        f << "v " << std::setprecision(6) << 0.0 << ' '<< curr_h<< ' ' << 0.0 << '\n';
        ++points;
        i++;
    }

    for (auto i=1;i+3<=points;i=i+3){
        f << "f " << i <<"/0/0 ";
        f << i+1 <<"/0/0 ";
        f << i+2 <<"/0/0\n";
    }
    f.close();
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
    return 1;
}
