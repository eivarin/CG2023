#define _USE_MATH_DEFINES
#include <math.h>

#include <array>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <tuple>
#include "model.hpp"


void drawPlane(std::string const& file, std::size_t length, std::size_t divisions) {
    float right_most = length / 2.;
    std::vector<float> base_points;
    float division_size = length / ((float) divisions);
    for (auto i = 0; i <= divisions; ++i) 
        base_points.push_back(right_most - (division_size * i));

    model m;
    for (auto i = 0; i <= divisions; ++i)
        for (auto j = 0; j <= divisions; ++j) 
            m.pushCoords(vertex_coords(base_points[i], 0., base_points[j]));
    /*
    16 12 8  4
    15 11 7  3
    14 10 6  2
    13 9  5  1
    */
    for (auto i = 0; i < divisions; ++i) {
        for (auto j = 1; j <= divisions; ++j) {
            auto index = (divisions + 1) * i + j;
            m.pushFace(face(index, index + 1, index + divisions + 1));
            m.pushFace(face(index + 1, index + divisions + 2, index + divisions + 1));
        }
    }
    m.write(file);
}

void drawBox(std::string const& file, std::size_t length, std::size_t divisions) {
    float right_most = length / 2.;
    float division_size = length / ((float) divisions);
    std::vector<float> base_points;
    // Base
    for (auto i = 0; i <= divisions; ++i) { // 0 - Têm de estar escondidos
        base_points.push_back(right_most - (division_size * i));
    }


    model m;
    for (auto i = 0; i <= divisions; ++i) {   // -O2, melhora-me isto
        for (auto j = 0; j <= divisions; ++j) { // -O2, melhora-me isto
            m.pushCoords(vertex_coords(base_points[i],base_points[divisions],0 - base_points[j]));
        }
    }
    // Topo
    for (auto i = 0; i <= divisions; ++i) {   // -O2, melhora-me isto
        for (auto j = 0; j <= divisions; ++j) { // -O2, melhora-me isto
            m.pushCoords(vertex_coords(base_points[i],0 - base_points[divisions],0 - base_points[j]));
        }
    }

    for (auto i = 0; i <= divisions; ++i) {
        for (auto j = 0; j <= divisions; ++j) { // -O2, melhora-me isto
            m.pushCoords(vertex_coords(base_points[divisions],base_points[i],0 - base_points[j]));
        }
    }

    for (auto i = 0; i <= divisions; ++i) {   // -O2, melhora-me isto
        for (auto j = 0; j <= divisions; ++j) { // -O2, melhora-me isto
            m.pushCoords(vertex_coords(base_points[0],base_points[i],0 - base_points[j]));
        }
    }

    for (auto i = 0; i <= divisions; ++i) {   // -O2, melhora-me isto
        for (auto j = 0; j <= divisions; ++j) { // -O2, melhora-me isto
            m.pushCoords(vertex_coords(base_points[i],(0 - base_points[j]),base_points[divisions]));
        }
    }

    for (auto i = 0; i <= divisions; ++i) {   // -O2, melhora-me isto
        for (auto j = 0; j <= divisions; ++j) { // -O2, melhora-me isto // 5
            m.pushCoords(vertex_coords(base_points[i],(0 - base_points[j]),base_points[0]));
        }
    }
    for (auto k=0;k<6;++k){
        if (k % 2 !=0  && k !=1 || k == 0 ){
            for(auto i = 0; i < divisions; ++i) {
                for(auto j = 1; j <= divisions; ++j) {
                    auto index = (divisions+1) * i + j+ k*(divisions+1)*(divisions+1);
                    m.pushFace(face(index,index+1,index+divisions+1));
                    m.pushFace(face(index + 1,index + divisions + 2,index + divisions + 1));
                }
            }
        }
        else if (k==1){
            for(auto i = 0; i < divisions; ++i) {
                for(auto j = 1; j <= divisions; ++j) {
                    auto index = (divisions+1) * i + j+ k*(divisions+1)*(divisions+1);
                    m.pushFace(face(index, index + divisions + 1, index + 1));
                    m.pushFace(face(index + 1,index + divisions + 1,index + divisions + 2));
                }
            }
        }
        else{
            for(auto i = 0; i < divisions; ++i) {
                for(auto j = 1; j <= divisions; ++j) {
                    auto index = (divisions+1) * i + j+ k*(divisions+1)*(divisions+1);
            // resultFaces.append(((index,index + divisions + 1,index+1)))
                    m.pushFace(face(index,index + divisions + 1,index+1));
            // resultFaces.append(((index + 1,index + divisions + 1,index + divisions + 2)))
                    m.pushFace(face(index + 1,index + divisions + 1,index + divisions + 2 ));
                }
            }
        }
    }
    m.write(file);
}

void draw_sphere(std::string const& file, ssize_t radius, std::size_t slices, std::size_t stacks) {
    model m;
    float stack_angle = M_PI / stacks;
    float slice_angle = (2 * M_PI) / slices;
    m.pushCoords(vertex_coords(0,radius,0));
    for (std::size_t i = 1; i < stacks; ++i) {
        for (std::size_t j = 0; j < slices; ++j) {
            float curr_b = i * stack_angle;
            float sinb = sin(curr_b);
            float x = sin(j * slice_angle) * sinb * radius;
            float y = cos(curr_b) * radius;
            float z = cos(j * slice_angle) * sinb * radius;
            m.pushCoords(vertex_coords(x, y, z));
        }
    }
    m.pushCoords(vertex_coords(0,-radius,0));

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
        m.pushFace(face(1,(i%slices) + 2,((i+1)%slices) + 2));
    }
    // "faces"
    for(std::size_t i = 0; i < stacks - 2; ++i) {
        for(std::size_t j = 0; j < slices; ++j) {
            std::size_t r = (j%slices) + 2 + (i * slices);
            std::size_t l = ((j+1)%slices) + 2 + (i * slices);
            m.pushFace(face(r,l + slices,l));
            m.pushFace(face(r,r+slices,l+slices));

        }
    }
    // "base" 2
    std::size_t bottom_most = slices*(stacks-1)+2;
    for(std::size_t i = 0; i < slices; ++i) {
        m.pushFace(face(bottom_most,bottom_most - slices + (i+1)%slices,bottom_most - slices + (i%slices)));
    }
    m.write(file);
}

void draw_ring(std::string const& file, ssize_t radius, ssize_t slices, ssize_t width) {
    model m;
    float slice_angle = (2 * M_PI) / slices;
    
    ssize_t dif = width > 0 || radius - width > 0 ? radius - width : 0;
    for(auto i = 0; i <= slices; ++i) {
        float angle = i * slice_angle;

        float x1 = sin(angle) * radius;
        float z1 = cos(angle) * radius;
        m.pushCoords(vertex_coords(x1, 0, z1));

        float x2 = sin(angle) * dif;
        float z2 = cos(angle) * dif;
        m.pushCoords(vertex_coords(x2, 0, z2));
    }
    /*
    1 3 5 7
    2 4 6 8
    */
    for(auto i = 0; i < slices; ++i) {
        // 1 2 3 e 3 2 1
        m.pushFace(face(2*i+1, 2*i+2, 2*i+3));
        m.pushFace(face(2*i+3, 2*i+2, 2*i+1));
        // 2 4 3 e 3 4 2
        m.pushFace(face(2*i+2, 2*i+4, 2*i+3));
        m.pushFace(face(2*i+3, 2*i+4, 2*i+2));
    }
    m.write(file);
}

void drawCone(std::string const& file,int height, int radius, int slices, int stacks)
{   
    int points=1;
    model m;
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
        m.pushCoords(vertex_coords(0.0f, curr_h, 0.0f));
        ++points;
        m.pushCoords(vertex_coords(px1 * curr_r, curr_h - stack_h, py1  * curr_r));
        ++points;
        m.pushCoords(vertex_coords(px2 * curr_r, curr_h - stack_h, py2  * curr_r));
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
            
            m.pushCoords(vertex_coords(px1 * curr_r, curr_h, py1 * curr_r));
            ++points;
            m.pushCoords(vertex_coords(px1 * (curr_r+stack_r), curr_h - stack_h, py1 * (curr_r+stack_r)));
            ++points;
            m.pushCoords(vertex_coords(px2 * (curr_r+stack_r), curr_h - stack_h, py2 * (curr_r+stack_r)));
            ++points;

            m.pushCoords(vertex_coords(px2 * curr_r, curr_h, py2 * curr_r));
            ++points;
            m.pushCoords(vertex_coords(px1 * curr_r, curr_h, py1 * curr_r));
            ++points;
            m.pushCoords(vertex_coords(px2 * (curr_r+stack_r), curr_h - stack_h, py2 * (curr_r+stack_r)));
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
        m.pushCoords(vertex_coords(px2 * curr_r, curr_h, py2  * curr_r));
        ++points;
        m.pushCoords(vertex_coords(px1 * curr_r, curr_h, py1  * curr_r));
        ++points;
        m.pushCoords(vertex_coords(0.0f, curr_h, 0.0f));
        ++points;
        i++;
    }

    for (auto i=1;i+3<=points;i=i+3){
        m.pushFace(face(i,i+1,i+2));
    }
    m.write(file);
}
std::vector<std::string> split (const std::string &s, char delim) 
{
    std::vector<std::string> result;
    std::stringstream ss (s);
    std::string item;

    while (getline (ss, item, delim)) {
        result.push_back (item);
    }

    return result;
}
std::tuple <float,float,float> calcPoint(std::tuple <float,float,float> p,int x){
    return std::tuple <float,float,float>((std::get<0>(p))*x,(std::get<1>(p))*x,(std::get<2>(p)) *x);
}
std::tuple <float,float,float> calcPointFloat(std::tuple <float,float,float> p,float x){
    return std::tuple <float,float,float>((std::get<0>(p))*x,(std::get<1>(p))*x,(std::get<2>(p)) *x);
}
std::tuple <float,float,float> calculaPointMatrixFinal(int c[4],std::tuple<float,float,float> matrixPontos[4]){
    std::tuple <float,float,float> pontosInt [4];
    std::tuple <float,float,float> res;
    for (int i = 0;i<4;i++){
        res = calcPoint(matrixPontos[i],c[i]);
        pontosInt[i] = res;
    }
    std::tuple <float,float,float> final=std::tuple<float,float,float>(0.0,0.0,0.0);

    for (int k = 0;k<4;k++){
        (std::get<0>(final)) +=  (std::get<0>(pontosInt[k]));
        (std::get<1>(final)) +=  (std::get<1>(pontosInt[k]));
        (std::get<2>(final)) +=  (std::get<2>(pontosInt[k]));
    }
    return final;
}
std::tuple <float,float,float> calculaPointMatrixFinalFloat(float c[4],std::tuple<float,float,float> matrixPontos[4]){
    std::tuple <float,float,float> pontosInt [4];
    std::tuple <float,float,float> res;
    for (int i = 0;i<4;i++){
        res = calcPointFloat(matrixPontos[i],c[i]);
        pontosInt[i] = res;
    }
    /*
    std:: cout << "Pontos int:";
    for ( auto i = 0; i< 4;i++){
        std:: cout <<std::to_string(std::get<0>(pontosInt[i])) << " " << std::to_string(std::get<1>(pontosInt[i])) << " " << std::to_string(std::get<2>(pontosInt[i]))<<"|";
    }
    std:: cout << "\n";
    */
    std::tuple <float,float,float> final=std::tuple<float,float,float>(0.0,0.0,0.0);

    for (int k = 0;k<4;k++){
        (std::get<0>(final)) +=  (std::get<0>(pontosInt[k]));
        (std::get<1>(final)) +=  (std::get<1>(pontosInt[k]));
        (std::get<2>(final)) +=  (std::get<2>(pontosInt[k]));
    }
    return final;
}
void calculatePointsToTeaPot(std::string const& fileInput,std::string const& fileOutput)
{   
    /************************PARSING DO FICHEIRO DE INPUT INICIADA***********************************************/
    int nPontos = 0;
    int lineNumber;
    int it = 0; 
    int column = 0;
    int line = 0;

    std::tuple<float,float,float> arrayVertices[290];
    int arrayPatches[128][4];

    std::string myText;
    std::ifstream MyReadFile(fileInput);

    while (getline (MyReadFile, myText)) {
        if (it == 0) {
            lineNumber = stoi(myText);
            it++;
        }
        else if (it >=1 && it <=lineNumber){
            std::vector<std::string> res = split(myText,',');
            for(auto i: res){
                if (column < 4){
                    arrayPatches[line][column] = stoi(i);
                    column++;
                }
                else{
                    line++;
                    column = 0;
                    arrayPatches[line][column] = stoi(i);
                    column++;
                }
            }
            it++;
        }
        else if (it > lineNumber+1){
                std::vector<std::string> res = split(myText,',');
                arrayVertices[nPontos] = std::tuple<float,float,float>(stof(res[0]),stof(res[1]),stof(res[2]));
                nPontos++;
                it++;

        }
        else {
            it++;
        }
    }
    /*
    for (int i = 0 ;i <128;i++){
        for (int k = 0; k < 4;k++){
            std:: cout << arrayPatches[i][k] << " ";
        }
        std::cout << "\n";
    }
    */
    /*
    for (int i = 0 ; i< 290;i++){
        std::cout << get<0>(arrayVertices[i])<<" "<< get<1>(arrayVertices[i])<<" "<< get<2>(arrayVertices[i])<<"\n";
    }
    */
    MyReadFile.close();

    /************************PARSING DO FICHEIRO DE INPUT TERMINADA***********************************************/
    // Nivel de seleção é 1 , logo u = 0.1 e v = 0.1
    // Isto é só para um patch 
    std::tuple<float,float,float> pontosFinais[11][11];
    int bezierMatrix [4][4]={{-1,3,-3,1},
                            {3,-6,3,0},
                            {-3,3,0,0},
                            {1,0,0,0}};
    int linhasPatch = 0;
    std::ofstream MyFile(fileOutput);
    while(linhasPatch!=128){
        std::tuple<float,float,float> pontosPatch [4][4] = {{arrayVertices[arrayPatches[linhasPatch][0]],arrayVertices[arrayPatches[linhasPatch+1][0]],arrayVertices[arrayPatches[linhasPatch+2][0]],arrayVertices[arrayPatches[linhasPatch+3][0]]},
                                                            {arrayVertices[arrayPatches[linhasPatch][1]],arrayVertices[arrayPatches[linhasPatch+1][1]],arrayVertices[arrayPatches[linhasPatch+2][1]],arrayVertices[arrayPatches[linhasPatch+3][1]]},
                                                            {arrayVertices[arrayPatches[linhasPatch][2]],arrayVertices[arrayPatches[linhasPatch+1][2]],arrayVertices[arrayPatches[linhasPatch+2][2]],arrayVertices[arrayPatches[linhasPatch+3][2]]},
                                                            {arrayVertices[arrayPatches[linhasPatch][3]],arrayVertices[arrayPatches[linhasPatch+1][3]],arrayVertices[arrayPatches[linhasPatch+2][3]],arrayVertices[arrayPatches[linhasPatch+3][3]]}}; 
        std::tuple<float,float,float> bezierMatrixXPontosPatch [4][4];
        for (auto i = 0;i<4;i++){
            for (auto j = 0;j<4;j++){
                    bezierMatrixXPontosPatch[i][j] = calculaPointMatrixFinal(bezierMatrix[i],pontosPatch[j]);
            }
        }
        std::tuple<float,float,float> bezierMatrixXPontosPatchXbezierMatrix [4][4];
        for (auto i = 0;i<4;i++){
            for (auto j = 0;j<4;j++){
                    bezierMatrixXPontosPatchXbezierMatrix[i][j] = calculaPointMatrixFinal(bezierMatrix[j],bezierMatrixXPontosPatch[i]);
            }
        }
        std::tuple<float,float,float> bezierMatrixXPontosPatchXbezierMatrixTrans [4][4];
        for (auto i = 0;i<4;i++){
            for (auto j = 0;j<4;j++){
                    bezierMatrixXPontosPatchXbezierMatrixTrans[i][j] = bezierMatrixXPontosPatchXbezierMatrix [j][i];
            }
        }
        /*
        for (auto i = 0;i<4;i++){
            for (auto j = 0;j<4;j++){
                    std:: cout << std::to_string(std::get<0>(bezierMatrixXPontosPatchXbezierMatrixTrans[i][j])) << " " << std::to_string(std::get<1>(bezierMatrixXPontosPatchXbezierMatrixTrans[i][j])) << " " << std::to_string(std::get<2>(bezierMatrixXPontosPatchXbezierMatrixTrans[i][j])) << "|";
            }
            std:: cout << "\n";
        }
        */
        for(int u = 0;u<=10;u++){
            float uf = float(u)/10;
            float u_vetor [4] = {uf*uf*uf,uf*uf,uf,1};
            for(int v = 0;v<=10;v++){
                float vf =float(v)/10;
                float v_vetor [4] = {vf*vf*vf,vf*vf,vf,1};

                std::tuple<float,float,float> u_vetorXcalculada[4];
                for (auto i = 0; i < 4;i++){
                    u_vetorXcalculada[i]= calculaPointMatrixFinalFloat(u_vetor,bezierMatrixXPontosPatchXbezierMatrixTrans[i]);
                }
                /*
                std::cout<< "Vetor v-";
                for (auto i = 0; i < 4;i++){
                std::cout<<v_vetor[i]<< " ";
                }
                std::cout << "\n";
                */
                std::tuple<float,float,float> puv;
                puv = calculaPointMatrixFinalFloat(v_vetor,u_vetorXcalculada);
                std::cout<< "Puv:";
                std::cout <<std::to_string(std::get<0>(puv)) << " " << std::to_string(std::get<1>(puv)) << " " << std::to_string(std::get<2>(puv)) << "|";
                std::cout << "\n";
                pontosFinais[u][v] = puv;

            }
        }

        for (int i = 0;i<=10;i++){
            for(int k = 0;k<=10;k++){
                MyFile << "v " << std::to_string(std::get<0>(pontosFinais[i][k])) << " " << std::to_string(std::get<1>(pontosFinais[i][k])) << " " << std::to_string(std::get<2>(pontosFinais[i][k]))<<"\n";
            }
        }
        linhasPatch += 4;
    }

    for (int i = 0;i<32;i++){
        for(int k = 1;k<=11;k++){
            MyFile << "f " << i*121+k+1 << "/0/0 " << i*121+k << "/0/0  " << i*121+k+11 <<"/0/0\n";
            MyFile << "f " << i*121+k+1 << "/0/0 " << i*121+k+11 << "/0/0  " << i*121+k+12 <<"/0/0\n";
        }
    }

    MyFile.close();
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
    // input line to draw a ring: ./generator ring radius slices width
    else if (argc == 6 && std::string("ring").compare(argv[1]) == 0) {
        ssize_t radius = std::stoul(argv[2]);
        ssize_t slices = std::stoul(argv[3]);
        size_t width = std::stoul(argv[4]);
        draw_ring(argv[5], radius, slices, width);
    }
    // input line to give a file with points to draw a teapot: ./generator fileOutput fileInput
    else{
        
        std::string fileInput = "../teapot.patch";
        std::string fileOutput = "teapot.3d";
        calculatePointsToTeaPot(fileInput,fileOutput);
    }
    return 0;
}
