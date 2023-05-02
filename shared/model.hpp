#ifndef MODEL_HPP
#define MODEL_HPP

#include <math.h>

struct vec3 {
	float x;
	float y;
	float z;
    vec3(float x, float y, float z) : x(x), y(y), z(z) {
    }
    vec3(){

    }
    void normalize(){
        float vLen = sqrtf64(x*x+y*y+z*z);
        x = x / vLen;
        y = y / vLen;
        z = z / vLen;
    };
    vec3 get_cross_product(vec3& v){
        this->normalize();
        v.normalize();
        return vec3((this->y*v.z) - (this->z*v.y), (this->z*v.x) - (this->x*v.z), (this->x*v.y) - (this->y*v.x));
    };
};

struct vertex_ref {
    int coords;
    int normal;
    int texture;
    vertex_ref(int c, int n, int t) : coords(c), normal(n), texture(t) {
    }
    vertex_ref(){}
};

struct face {
    vertex_ref v[3];
    face(vertex_ref v1, vertex_ref v2, vertex_ref v3) {
        v[0] = v1;
        v[1] = v2;
        v[2] = v3;
    }
    face(){} 
};

#endif