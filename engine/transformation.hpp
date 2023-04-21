#ifndef TRANSFORMATION_H
#define TRANSFORMATION_H


#include<string>
#include<math.h>
#include "wavefront.hpp"
#include "./rapid_xml/rapidxml.hpp"
#include <GL/glew.h>
#include <GL/glut.h>
#include <memory>

enum t_tipo : unsigned char { T, R, S};

class transformation{
    public:
        t_tipo t;
        transformation(){
            t = T;
        }
        virtual ~transformation() = default;
        transformation(rapidxml::xml_node<> *node){
            std::string tipo = node->name();
            if (tipo.compare("translate") == 0){
                t = T;
            }
            else if (tipo.compare("rotate") == 0){
                t = R;
            }
            else if (tipo.compare("scale") == 0){
                t = S;
            }
        }
        virtual void apply()=0;
        static transformation* from_rapidxml_node(rapidxml::xml_node<> *node);
};

class static_transf : public transformation{
    vertex_coords vx;
    float angle;
    public:
        static_transf(rapidxml::xml_node<> *node) : transformation(node){
            if (t == R) angle = std::stof(node->first_attribute("angle")->value());
            vx.x = std::stof(node->first_attribute("x")->value());
            vx.y = std::stof(node->first_attribute("y")->value());
            vx.z = std::stof(node->first_attribute("z")->value());
        }

        void apply() override{
            switch (t)
            {
            case T:
                glTranslatef(vx.x, vx.y, vx.z);
                break;
            case R:
                glRotatef(angle,vx.x, vx.y, vx.z);
                break;
            case S:
                glScalef(vx.x, vx.y, vx.z);
                break;
            default:
                break;
            }
        }
};

class animated_rotation : public transformation{
    vertex_coords axis;
    float time;
    public:
        animated_rotation(rapidxml::xml_node<> *node) : transformation(node){
            time = std::stof(node->first_attribute("time")->value());
            axis.x = std::stof(node->first_attribute("x")->value());
            axis.y = std::stof(node->first_attribute("y")->value());
            axis.z = std::stof(node->first_attribute("z")->value());
        }

        void apply() override{
            int curr_time = glutGet(GLUT_ELAPSED_TIME);
            glRotatef(curr_time / time ,axis.x, axis.y, axis.z);
        }
};

// t, t2, t3 e ps teem que estar definidos quando se chamar a macro
#define CatmullRomPosSingle(p0, p1, p2, p3) (float) ((-0.5*t3 + t2 -0.5*t) * p0 + (1.5*t3 - 2.5 * t2 + 1) * p1 + (-1.5*t3 + 2*t2 + 0.5*t) * p2 + (0.5*t3 - 0.5*t2) * p3)
#define CatmullRomDerivSingle(p0, p1, p2, p3) (float) ((-1.5*t2 + 2*t - 0.5) * p0 + (4.5*t2 - 5*t) * p1 + (-4.5*t2 + 4*t + 0.5) * p2 + (1.5*t2 - t) * p3)

#define CatmullRomPosCoord(coord) CatmullRomPosSingle(ps[0].coord, ps[1].coord, ps[2].coord, ps[3].coord)
#define CatmullRomDerivCoord(coord) CatmullRomDerivSingle(ps[0].coord, ps[1].coord, ps[2].coord, ps[3].coord)

#define CatmullRomPosVector {.x= CatmullRomPosCoord(x), .y=CatmullRomPosCoord(y), .z=CatmullRomPosCoord(z)}
#define CatmullRomDerivVector {.x= CatmullRomDerivCoord(x), .y=CatmullRomDerivCoord(y), .z=CatmullRomDerivCoord(z)}

#define CatmullRomMacro(isDeriv) (isDeriv) ? (vertex_coords) CatmullRomDerivVector : (vertex_coords) CatmullRomPosVector

class animated_translation : public transformation{
    vertex_coords old_y;
    std::vector<vertex_coords> points;
    int npoints, segments;
    float time;
    bool needsLine, align;
    GLuint vertices, count;

    vertex_coords CatmullRom(float t, vertex_coords *ps, bool isDeriv) {
        float t2 = pow(t,2), t3 = pow(t,3);
        return CatmullRomMacro(isDeriv);
    }

    void getRotationMatrix(float t, vertex_coords *ps, float * m){
        vertex_coords deriv_X = CatmullRom(t, ps, true);
        vertex_coords deriv_Z = deriv_X.get_cross_product(old_y);
        vertex_coords deriv_Y = deriv_Z.get_cross_product(deriv_X);
        deriv_Y.normalize();
        m[0] = deriv_X.x; m[1] = deriv_X.y; m[2]  = deriv_X.z; m[3] = 0;
        m[4] = deriv_Y.x; m[5] = deriv_Y.y; m[6]  = deriv_Y.z; m[7] = 0;
        m[8] = deriv_Z.x; m[9] = deriv_Z.y; m[10] = deriv_Z.z; m[11] = 0;
        m[12] = 0;        m[13] = 0;   m[14] = 0;    m[15] = 1;
        old_y = deriv_Y;
    }


    float getLocalPoints(float gt, vertex_coords *ps){
        float t = gt * npoints; // this is the real global t
        int index = floor(t);  // which segment
        t = t - index; // where within  the segment

        // indices store the points
        int indices[4]; 
        indices[0] = (index + npoints-1)% npoints;	
        indices[1] = (indices[0]+1)% npoints;
        indices[2] = (indices[1]+1)% npoints; 
        indices[3] = (indices[2]+1)% npoints;
        for (size_t i = 0; i < 4; i++) ps[i] = points[indices[i]];
        return t;
    }

    float getCurrentPoints(vertex_coords *ps){
        float curr_time = glutGet(GLUT_ELAPSED_TIME), bla;
        float gt = modf((curr_time / 1000) / (float) time, &bla);
        return getLocalPoints(gt, ps);
    }

    
    void drawLine(){
        glBindBuffer(GL_ARRAY_BUFFER, vertices);
        glVertexPointer(3, GL_FLOAT, 0, 0);
        glDrawArrays(GL_LINE_LOOP, 0, count);
    }

    bool check_align(rapidxml::xml_node<> *transf){
        auto r = false;
        auto atrb = transf->first_attribute("align");
        if (atrb != 0){
            std::string s = atrb->value();
            r = (s.compare("true") == 0) || (s.compare("True") == 0) || (s.compare("1") == 0);
        }
        return r;
    }

    bool check_line(rapidxml::xml_node<> *transf){
        auto r = false;
        auto atrb = transf->first_attribute("segments");
        if (atrb != 0){
            segments = std::stoi(transf->first_attribute("segments")->value());
        }
        return atrb;
    }


    public:
        inline static std::vector<animated_translation*> at_vector;
        animated_translation(rapidxml::xml_node<> *node) : transformation(node){
            time = std::stof(node->first_attribute("time")->value());
            align = check_align(node);
            old_y = {.x=0, .y=1, .z=0};
            for (rapidxml::xml_node<> *n = node->first_node(); n; n = n->next_sibling())
            {
                std::string nome = n->name();
                if (nome.compare("point") == 0){
                    vertex_coords new_v;
                    new_v.x = std::stof(n->first_attribute("x")->value());
                    new_v.y = std::stof(n->first_attribute("y")->value());
                    new_v.z = std::stof(n->first_attribute("z")->value());
                    points.push_back(new_v);
                }
            }
            npoints = points.size();
            needsLine = check_line(node);
        }

        void prepLine(){
            std::vector<float> vList;
            for (size_t i = 0; i <= segments; i++)
            {
                vertex_coords ps[4];
                float gt = i / (float) segments;
                float t = getLocalPoints(gt, ps);
                vertex_coords pos = CatmullRom(t, ps, false);
                vList.push_back(pos.x);
                vList.push_back(pos.y);
                vList.push_back(pos.z);
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

        void apply() override{
            if (needsLine){
                drawLine();
            }
            vertex_coords ps[4];
            float t = getCurrentPoints(ps);
            vertex_coords pos = CatmullRom(t, ps, false);
            glTranslatef(pos.x, pos.y, pos.z);
            if (align){
                float m[16];
                getRotationMatrix(t, ps, m);
                glMultMatrixf(m);
            }
        }
};

transformation* transformation::from_rapidxml_node(rapidxml::xml_node<> *node){
    transformation* t;
    bool is_animated = (node->first_attribute("time") != 0);
    if (is_animated && node->first_node()){
        animated_translation* at = new animated_translation(node);
        animated_translation::at_vector.push_back(at);
        t = at;
    }
    else if (is_animated){
        t = new animated_rotation(node);
    }
    else
    {
        t = new static_transf(node);
    }
    return t;
}

#endif