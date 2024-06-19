//
//  main.cpp
//  VI-RT
//
//  Created by Luis Paulo Santos on 30/01/2023.
//

#include <iostream>
#include "scene.hpp"
#include "perspective.hpp"
#include "StandardRenderer.hpp"
#include "ImagePPM.hpp"
#include "ImageJPG.hpp"
#include "ImagePFM.hpp"
#include "ImageOpenEXR.hpp"
#include "AmbientShader.hpp"
#include "WhittedShader.hpp"
#include "DistributedShader.hpp"
#include "PathTracerShader.hpp"
#include "AmbientLight.hpp"
#include "PointLight.hpp"
#include "AreaLight.hpp"
#include <omp.h>
#include <time.h>

void addSquareLight(Point p, float size, RGB intensity, std::vector <Light *> *lights,int *num) {
    Point v1 = Point(p.X - size, p.Y, p.Z - size);
    Point v2 = Point(p.X + size, p.Y, p.Z - size);
    Point v3 = Point(p.X + size, p.Y, p.Z + size);
    Point v4 = Point(p.X - size, p.Y, p.Z + size);
    Vector ex = v2.vec2point(v1);
    Vector fx = v3.vec2point(v1);
    Vector n = ex.cross(fx);
    n.normalize();


    AreaLight* l1 = new AreaLight(intensity, v1, v2, v3, n);
    AreaLight* l2 = new AreaLight(intensity, v1, v3, v4, n);
    lights->push_back(l1);
    lights->push_back(l2);
    *num += 2;
}

// Function to add multiple point lights to the scene
// void addPointLights(Scene &scene, int numLights) {
//     RGB lightColor(0.85f, 0.85f, 0.85f);

//     Point basePosition(288,508,282);
//     int spacing = 5;

//     for (int i = 0; i < numLights; ++i) {
//         Point position(basePosition.X + i * spacing, basePosition.Y, basePosition.Z + i * spacing);
//         PointLight *pointLight = new PointLight(lightColor, position);
//         scene.lights.push_back(pointLight);
//         scene.numLights++;
//     }
// }



int main(int argc, const char * argv[]) {
    Scene scene;
    Perspective *cam; // Camera
    ImagePPM *imgPPM;    // Image
    ImageJPG *imgJPG;
    ImagePFM *imgPFM;
    ImageOpenEXR *imgOpenEXR;
    Shader *shd;
    bool success;
    clock_t start, end;
    double cpu_time_used;

    success = scene.Load("./VI-RT/Scene/tinyobjloader/models/cornell_box_VI.obj");
    // success = scene.Load("./VI-RT/Scene/tinyobjloader/models/debug.txt");
    
    if (!success) {
        std::cout << "ERROR!! :o\n";
        return 1;
    }
    std::cout << std::endl;
    std::cout << "Scene Load: SUCCESS!! :-)\n";


    // create the shader
    //RGB background(0.5, 0.05, 0.5); // roxo
    //shd = new DistributedShader(&scene, background);

    // add an ambient light to the scene
    AmbientLight *ambient = new AmbientLight(RGB(0.1,0.1,0.1));
    scene.lights.push_back(ambient);
    scene.numLights++;


    // Modifiquem aqui para adicionar quanta pointLights queremos
    //addPointLights(scene, 8);
    //PointLight *pl1 = new PointLight(RGB(1, 1, 1), Point(288, 508, 282));
    //scene.lights.push_back(pl1);
    //scene.numLights++;


    
    
    // Modifiquem aqui para adicionar quantas areaLights quiserem
    // luz do centro
    addSquareLight(Point(288, 547.99, 288), 60, RGB(0.6f, 0.6f, 0.6f),  &(scene.lights), &(scene.numLights));
    // // luzes dos cantos
    //addSquareLight(Point(110, 548, 110), 30, RGB(0.65, 0.65, 0.65),  &(scene.lights), &(scene.numLights));
    //addSquareLight(Point(450, 548, 450), 30, RGB(0.65, 0.65, 0.65),  &(scene.lights), &(scene.numLights));
    //addSquareLight(Point(110, 548, 450), 30, RGB(0.65, 0.65, 0.65),  &(scene.lights), &(scene.numLights));
    //addSquareLight(Point(450, 548, 110), 30, RGB(0.65, 0.65, 0.65),  &(scene.lights), &(scene.numLights));
    

    for (auto l = scene.lights.begin() ; l != scene.lights.end() ; l++) {
        if ((*l)->type == AMBIENT_LIGHT) {
            std::cout << "AmbientLight guardada" << std::endl;
        }
        if ((*l)->type == POINT_LIGHT) {
            std::cout << "PointLight guardada" << std::endl;
        }
        if ((*l)->type == AREA_LIGHT) {
            std::cout << "AreaLight guardada" << std::endl;
        }
    }
    

    scene.printSummary();
    std::cout << std::endl;
        
    // Image resolution
    const int W= 1024;
    const int H= 1024;


    // Parâmetros da câmera
    const Point Eye = {280, 275, -330}, At = {280, 265, 0};
    const Vector Up = {0, -1, 0};
    const float fovW = 90.f;
    const float fovH = fovW * (float)H / (float)W; // em graus
    const float fovWrad = fovW * 3.14f / 180.f, fovHrad = fovH * 3.14f / 180.f; // para radianos
    cam = new Perspective(Eye, At, Up, W, H, fovWrad, fovHrad);

    cam->Information();
    std::cout << std::endl;

    // Cor de fundo
    RGB background(0.2, 0.2, 0.2); // cinza forte

    // Definir o shader
    shd = new PathTracerShader(&scene, background);

    // Declarar o renderizador
    int spp = 2048; // amostras por pixel

    // Chose the image format
    int n;
    std::cout << "Chose a format for the image, by default is PPM:\n 1 - JPG\n 2 - PFM\n 3 - OpenEXR\n";
    std::cin >> n;

    switch (n) {
        case 1:
        {
            std::cout << "The image will be saved in a JPG format!\n";
            imgJPG = new ImageJPG(W,H);
            StandardRenderer myRenderJPG(cam, &scene, imgJPG, shd, spp);
            start = omp_get_wtime();
            myRenderJPG.Render();
            end = omp_get_wtime();
            cpu_time_used = end - start;
            imgJPG->Save("MyImage");
            break;
        }
        case 2:
        {
            std::cout << "The image will be saved in a PFM format!\n";
            imgPFM = new ImagePFM(W,H);
            StandardRenderer myRenderPFM(cam, &scene, imgPFM, shd, spp);
            start = omp_get_wtime();
            myRenderPFM.Render();
            end = omp_get_wtime();
            cpu_time_used = end - start;
            imgPFM->Save("MyImage");
            break;
        }
        case 3:
        {
            std::cout << "The image will be saved in a OpenEXR format!\n";
            imgOpenEXR = new ImageOpenEXR(W,H);
            StandardRenderer myRenderOpenEXR(cam, &scene, imgOpenEXR, shd, spp);
            start = omp_get_wtime();
            myRenderOpenEXR.Render();
            end = omp_get_wtime();
            cpu_time_used = end - start;
            imgOpenEXR->Save("MyImage");
            imgOpenEXR->Display("MyImage");
            break;
        }
        default:
        {
            std::cout << "The image will be saved in a PPM format!\n";
            imgPPM = new ImagePPM(W,H);
            StandardRenderer myRenderDefault(cam, &scene, imgPPM, shd, spp);
            start = clock();
            myRenderDefault.Render();
            end = clock();
            cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
            imgPPM->Save("MyImage.ppm");
            break;
        }
    }
    
    fprintf(stdout, "Rendering time = %.3lf secs\n\n", cpu_time_used);
    
    std::cout << "That's all, folks!" << std::endl;
    return 0;
}