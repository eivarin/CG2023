#ifndef WAVEFRONT_H
#define WAVEFRONT_H

#include <GL/glew.h>
#include <GL/glut.h>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>

struct vertex_coords {
	float x;
	float y;
	float z;
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
};

class model {
    private:
        std::vector<vertex_coords> vs;
        std::vector<vertex_texture> ts;
        std::vector<vertex_normal> ns;
        std::vector<face> fs;
        GLuint vertices, count;
        void loadModel(std::string const& fname) {
            std::ifstream file;
            file.open(fname);
            // read the first word of the line
            for(std::string line_header; file >> line_header; ) {
                if(line_header.compare("v") == 0) {
                    vertex_coords v;
                    file >> v.x >> v.y >> v.z;
                    vs.push_back(v);
                } else if(line_header.compare("vt") == 0) {
                    vertex_texture t;
                    file >> t.x >> t.y;
                    ts.push_back(t);
                } else if (line_header.compare("vn") == 0 ) {
                    vertex_normal n;
                    file >> n.x >> n.y >> n.z;
                    ns.push_back(n);
                } else if (line_header.compare("f") == 0 ) {
                    face f = {0};
                    char ignore;
                    file >> f.v[0].c >> ignore >> f.v[0].t >> ignore >> f.v[0].n >> f.v[1].c >> ignore >> f.v[1].t >> ignore >> f.v[1].n >> f.v[2].c >> ignore >> f.v[2].t >> ignore >> f.v[2].n;
                    fs.push_back(f);
                }
            }
        }
        void drawVertex(vertex_ref v_ref, std::vector<float> &vList){
            vertex_coords c = vs.at(v_ref.c-1);
            // VERTEX_TEXTURE t = m.ts.at(v_ref.t-1);
            // VERTEX_NORMAL n = m.ns.at(v_ref.n-1);
            vList.push_back(c.x);
            vList.push_back(c.y);
            vList.push_back(c.z);
        }
    public:
        model(std::string filename){
            loadModel(filename);
        }

        void prepModel(){
            int i = 0;
            std::vector<float> vList;
            for(face& f : fs) {
                i++;
                drawVertex(f.v[0], vList);
                drawVertex(f.v[1], vList);
                drawVertex(f.v[2], vList);
            }
            count = vList.size()/3;
            glGenBuffers(1, &vertices);
            glBindBuffer(GL_ARRAY_BUFFER, vertices);
            glBufferData(
                GL_ARRAY_BUFFER, // tipo do buffer, só é relevante na altura do desenho
                sizeof(float) * vList.size(), // tamanho do vector em bytes
                vList.data(), // os dados do array associado ao vector
                GL_STATIC_DRAW); // indicativo da utilização (estático e para desenho)
        }
        void drawModel(){
            glBindBuffer(GL_ARRAY_BUFFER, vertices);
            glVertexPointer(3, GL_FLOAT, 0, 0);
            glDrawArrays(GL_TRIANGLES, 0, count);
        }
};

bool operator<(const vertex_coords l, const vertex_coords r) {
	return true;
}

#endif