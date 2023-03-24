#include <fstream>
#include <iomanip>
#include <string>
#include <vector>

struct vertex_coords {
	float x;
	float y;
	float z;
    vertex_coords(float x, float y, float z) : x(x), y(y), z(z) {
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

union face {
    vertex_ref v[3];
    struct {
        vertex_ref um;
        vertex_ref dois;
        vertex_ref tres;
    };
    face(int c1, int c2, int c3) : um({.c = c1}), dois({.c = c2}), tres({.c = c3}) {
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
            s << "f " << f.um.c   << "/" << f.um.t   << "/" << f.um.n   << 
                  " " << f.dois.c << "/" << f.dois.t << "/" << f.dois.n << 
                  " " << f.tres.c << "/" << f.tres.t << "/" << f.tres.n << "\n";
        s.close();
    }
};
