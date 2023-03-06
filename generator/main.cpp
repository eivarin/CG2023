#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define _USE_MATH_DEFINES
#include <math.h>

#include <cstdlib>
#include <fstream>
#include <iomanip>
// #include <unordered_map>
#include <vector>
#include <string>

void drawPlane(std::string const& file, std::size_t length, std::size_t divisions) {
    float right_most = (length * divisions) / 2.;
    std::vector<float> base_points;
    for(auto i = 0; i <= divisions; ++i) {
        base_points.push_back(right_most - (length * i));
    }

    std::ofstream f;
    f.open(file);
    for(auto i = 0; i <= divisions; ++i) { // -O2, melhora-me isto
        for(auto j = 0; j <= divisions; ++j) { // -O2, melhora-me isto
            f << "v " << std::setprecision(6) << base_points[i] << ' ' << 0. << ' ' << (0-base_points[j]) << '\n';
        }
    }
    /*
    15 10  5
    14  9  4
    13  8  3
    12  7  2
    11  6  1
    */
    for(auto i = 0; i < divisions; ++i) {
        for(auto j = 1; j <= divisions; ++j) {
            auto index = (divisions+1) * i + j;
            f << "f " << index << "/0/0 ";
            f << index + divisions + 1 << "/0/0 ";
            f << index + 1 << "/0/0\n";
            f << "f " << index+1 << "/0/0 ";
            f << index + divisions + 1 << "/0/0 ";
            f << index + divisions + 2 << "/0/0\n";
        }
    }
    f.close();
}

int main(int argc, char **argv) {
    std::string s1 = "plane";
    // input line to draw a plane:  ./generator plane length divisions NameOfFileToOutput
	if (argc == 5 && s1.compare(argv[1]) == 0) {
        std::size_t length = std::stoul(argv[2]);
        std::size_t divisions = std::stoul(argv[3]);
        drawPlane(argv[4], length, divisions);
    } 
	return 1;
}
