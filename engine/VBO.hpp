#ifndef VBO_H
#define VBO_H

#include <GL/glew.h>
#include <GL/glut.h>
#include <vector>
#include <string>
#include "model.hpp"

class VBO
{
private:
    std::vector<vec3> cs, ns, ts;
    std::vector<face> fs;
    GLenum mode;
    unsigned int vertices, normals, texCoords;
    bool hasNormals, hasTexCoords;
    int count;
    void loadModel(std::string const& fname) {
        std::ifstream file;
        file.open(fname);
        // read the first word of the line
        for(std::string line_header; file >> line_header; ) {
            if(line_header.compare("v") == 0) {
                vec3 c;
                file >> c.x >> c.y >> c.z;
                cs.push_back(c);
            } else if(line_header.compare("vt") == 0) {
                vec3 t;
                file >> t.x >> t.y;
                ts.push_back(t);
            } else if (line_header.compare("vn") == 0 ) {
                vec3 n;
                file >> n.x >> n.y >> n.z;
                ns.push_back(n);
            } else if (line_header.compare("f") == 0 ) {
                face f;
                char ignore;
                file >> f.v[0].coords >> ignore >> f.v[0].texture >> ignore >> f.v[0].normal >> f.v[1].coords >> ignore >> f.v[1].texture >> ignore >> f.v[1].normal >> f.v[2].coords >> ignore >> f.v[2].texture >> ignore >> f.v[2].normal;
                fs.push_back(f);
            }
        }
        hasNormals = (ns.size() > 0);
        hasTexCoords = (ts.size() > 0);
    }
    void drawVertex(vertex_ref v_ref, std::vector<float> &cList, std::vector<float> &tList, std::vector<float> &nList){
        vec3 c,n,t;
        c = cs.at(v_ref.coords-1);
        cList.insert(cList.end(), {c.x, c.y, c.z});
        if (v_ref.texture > 0)
        {
            t = ts.at(v_ref.texture-1);
            tList.insert(tList.end(), {t.x, t.y});
        }
        if (v_ref.normal > 0)
        {
            n = ns.at(v_ref.normal-1);
            nList.insert(nList.end(), {n.x, n.y, n.z});
        }
    }
public:
    VBO(std::string const& fileName){
        loadModel(fileName);
        mode = GL_TRIANGLES;
    }
    void prep(){
        std::vector<float> cList, tList, nList;
        for(face& f : fs) for (size_t j = 0; j < 3; j++) drawVertex(f.v[j], cList, tList, nList);
        count = cList.size() / 3;
        glGenBuffers(1, &vertices);
        glBindBuffer(GL_ARRAY_BUFFER, vertices);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * count, cList.data(), GL_STATIC_DRAW);
        if (hasNormals){
            glGenBuffers(1, &normals);
            glBindBuffer(GL_ARRAY_BUFFER, normals);
            glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * count, nList.data(), GL_STATIC_DRAW);
        }
        if(hasTexCoords){
            glGenBuffers(1, &texCoords);
            glBindBuffer(GL_ARRAY_BUFFER, texCoords);
            glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 2 * count, tList.data(), GL_STATIC_DRAW);
        }
    }

    void draw(){
        glBindBuffer(GL_ARRAY_BUFFER,vertices);
        glVertexPointer(3,GL_FLOAT,0,0);
        if (hasNormals){
            glBindBuffer(GL_ARRAY_BUFFER,normals);
            glNormalPointer(GL_FLOAT,0,0);
        }
        if(hasTexCoords){
            glBindBuffer(GL_ARRAY_BUFFER,texCoords);
            glTexCoordPointer(2,GL_FLOAT,0,0);
        }
        glDrawArrays(mode, 0, count);
    }
};


#endif