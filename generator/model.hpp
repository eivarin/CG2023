#include <array>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

struct vertex_coords {
	float x;
	float y;
	float z;
    vertex_coords(float x, float y, float z){
        this->x = x;
        this->y = y;
        this->z = z;
    }
};

struct vertex_texture {
	float x;
	float y;
};

struct vertex_normal {
	float x;
	float y;
	float z;
};

struct vertex_ref {
	int c;
	int t;
	int n;
};

struct face {
	vertex_ref v[3];
    face(int c1, int c2, int c3){
        v[0] = {0};
        v[0].c = c1;
        v[1] = {0};
        v[1].c = c2;
        v[2] = {0};
        v[2].c = c3;
    }
};

class model
{
    private:
        std::vector<vertex_coords> vs;
        std::vector<vertex_texture> ts;
        std::vector<vertex_normal> ns;
        std::vector<face> fs;
    public:
        void pushCoords(vertex_coords c){
            vs.push_back(c);
        }
        void pushTexture(vertex_texture t){
            ts.push_back(t);
        }
        void pushNormal(vertex_normal n){
            ns.push_back(n);
        }
        void pushFace(face f){
            fs.push_back(f);
        }
        void write(std::string const& file){
            std::ofstream s;
            s.open(file);
            for(auto& v: vs)
                s << "v " << std::noshowpoint << std::fixed << std::setprecision(6) << v.x << ' ' << v.y << ' ' << v.z << '\n';
            for(auto& f: fs)
                s << "f " << f.v[0].c << "/" << f.v[0].t << "/" << f.v[0].n << 
                      " " << f.v[1].c << "/" << f.v[1].t << "/" << f.v[1].n << 
                      " " << f.v[2].c << "/" << f.v[2].t << "/" << f.v[2].n << "\n";
            s.close();
        }
};
