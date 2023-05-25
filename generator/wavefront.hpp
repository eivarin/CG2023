#include <fstream>
#include <iomanip>
#include <string>
#include <vector>
#include <cmath>
#include "model.hpp"


class model
{
private:
    std::vector<vec3> cs;
    std::vector<vec3> ts;
    std::vector<vec3> ns;
    std::vector<face> fs;
public:
    void pushCoords(vec3 c){
        cs.push_back(c);
    }
    void pushTexture(vec3 t){
        ts.push_back(t);
    }
    void pushNormal(vec3 n){
        ns.push_back(n);
    }
    void pushFace(face f){
        fs.push_back(f);
    }
    void write(std::string const& file){
        std::ofstream s;
        s.open(file);
        for(auto& c: cs)
            s << "v " << std::noshowpoint << std::fixed << std::setprecision(6) << c.x << ' ' << c.y << ' ' << c.z << '\n';
        for(auto& t: ts)
            s << "vt " << std::noshowpoint << std::fixed << std::setprecision(6) << t.x << ' ' << t.y << '\n';
        for(auto& n: ns){
            if (std::isnan(n.x)) n = vec3(0,1,0);
            s << "vn " << std::noshowpoint << std::fixed << std::setprecision(6) << n.x << ' ' << n.y << ' ' << n.z << '\n';
        }
        for(auto& f: fs)
            s << "f " << f.v[0].coords   << "/" << f.v[0].texture   << "/" << f.v[0].normal  << 
                  " " << f.v[1].coords << "/" << f.v[1].texture << "/" << f.v[1].normal << 
                  " " << f.v[2].coords << "/" << f.v[2].texture << "/" << f.v[2].normal << "\n";
        s.close();
    }
};
