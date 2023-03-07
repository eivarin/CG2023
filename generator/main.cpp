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
// #include <unordered_map>
#include <string>
#include <vector>

void drawPlane(std::string const& file, std::size_t length,
    std::size_t divisions) {
    float right_most = (length * divisions) / 2.;
    std::vector<float> base_points;
    for (auto i = 0; i <= divisions; ++i) {
        base_points.push_back(right_most - (length * i));
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
    // Base
    for (auto i = 0; i <= divisions; ++i) { // 0 - Têm de estar escondidos
        base_points.push_back(right_most - (length * i));
    }

    std::ofstream f;
    f.open(file);
    for (auto i = 0; i <= divisions; ++i) {   // -O2, melhora-me isto
        for (auto j = 0; j <= divisions; ++j) { // -O2, melhora-me isto
            f << "v " << std::setprecision(6) << base_points[i] << ' '
                << base_points[divisions] << ' ' << (0 - base_points[j]) << '\n';
        }
    }
    // Topo
    for (auto i = 0; i <= divisions; ++i) {   // -O2, melhora-me isto
        for (auto j = 0; j <= divisions; ++j) { // -O2, melhora-me isto
            f << "v " << std::setprecision(6) << base_points[i] << ' '
                << -(base_points[divisions]) << ' ' << (0 - base_points[j])
                << '\n'; // 1
        }
    }

    for (auto i = 0; i <= divisions; ++i) {
        for (auto j = 0; j <= divisions; ++j) { // -O2, melhora-me isto
            f << "v " << std::setprecision(6) << base_points[divisions] << ' '
                << base_points[i] << ' ' << (0 - base_points[j])
                << '\n'; // 2 - Têm de estar escondidas
        }
    }

    for (auto i = 0; i <= divisions; ++i) {   // -O2, melhora-me isto
        for (auto j = 0; j <= divisions; ++j) { // -O2, melhora-me isto
            f << "v " << std::setprecision(6) << base_points[0] << ' '
                << base_points[i] << ' ' << (0 - base_points[j]) << '\n'; // 3
        }
    }

    for (auto i = 0; i <= divisions; ++i) {   // -O2, melhora-me isto
        for (auto j = 0; j <= divisions; ++j) { // -O2, melhora-me isto
            f << "v " << std::setprecision(6) << base_points[i] << ' '
                << (0 - base_points[j]) << ' ' << base_points[divisions]
                << '\n'; // 4- Têm de estar escondidas
        }
    }

    for (auto i = 0; i <= divisions; ++i) {   // -O2, melhora-me isto
        for (auto j = 0; j <= divisions; ++j) { // -O2, melhora-me isto // 5
            f << "v " << std::setprecision(6) << base_points[i] << ' '
                << (0 - base_points[j]) << ' ' << base_points[0] << '\n';
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
<<<<<<< Updated upstream
   for (auto k=0;k<6;++k){
        if (k % 2 !=0  && k !=1 || k == 0 ){
            for(auto i = 0; i < divisions; ++i) {
                for(auto j = 1; j <= divisions; ++j) {
                    auto index = (divisions+1) * i + j+ k*(divisions+1)*(divisions+1);
=======
    for (auto k = 0; k < 6; ++k) {
        if (k % 2 == 0) {
            for (auto i = 0; i < divisions; ++i) {
                for (auto j = 1; j <= divisions; ++j) {
                    auto index =
                        (divisions + 1) * i + j + k * (divisions + 1) * (divisions + 1);
>>>>>>> Stashed changes
                    f << "f " << index << "/0/0 ";
                    f << index + 1 << "/0/0 ";
                    f << index + divisions + 1 << "/0/0\n";
                    f << "f " << index + 1 << "/0/0 ";
                    f << index + divisions + 2 << "/0/0 ";
                    f << index + divisions + 1 << "/0/0\n";
                }
            }
        }
<<<<<<< Updated upstream
        else if (k==1){
            for(auto i = 0; i < divisions; ++i) {
                for(auto j = 1; j <= divisions; ++j) {
                    auto index = (divisions+1) * i + j+ k*(divisions+1)*(divisions+1);
=======
        else {
            for (auto i = 0; i < divisions; ++i) {
                for (auto j = 1; j <= divisions; ++j) {
                    auto index =
                        (divisions + 1) * i + j + k * (divisions + 1) * (divisions + 1);
>>>>>>> Stashed changes
                    f << "f " << index << "/0/0 ";
                    f << index + divisions + 1 << "/0/0 ";
                    f << index + 1 << "/0/0\n";
                    f << "f " << index + 1 << "/0/0 ";
                    f << index + divisions + 1 << "/0/0 ";
                    f << index + divisions + 2 << "/0/0\n";
                }
            }
        }
<<<<<<< Updated upstream
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
=======
>>>>>>> Stashed changes
    }
    f.close();
}

void draw_sphere(std::string const& fname, std::size_t radius, std::size_t slices, std::size_t stacks) {
    std::ofstream f(fname, std::ofstream::out);
    float alpha = -M_PI / 2;
    float beta = 0.;
    /*
    é exatamente igual ao cilindro, mas mudam-se 2 ângulos em vez de se mudar apenas 1
    coordenadas a qualquer momento:
        P(sin(alpha) * r, STACK * ((float)radius / stacks) - radius, cos(beta) * r)
    */
    float slice_size = ((float) radius) / stacks;
    for (std::size_t i = 1; i < stacks; ++i) {
        continue;
    }
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
    else if (argc == 6 && std::string("sphere").compare(argv[1])) {
        std::size_t radius = std::stoul(argv[2]);
        std::size_t slices = std::stoul(argv[3]);
        std::size_t stacks = std::stoul(argv[4]);
        draw_sphere(argv[5], radius, slices, stacks);
    }
    return 1;
}
