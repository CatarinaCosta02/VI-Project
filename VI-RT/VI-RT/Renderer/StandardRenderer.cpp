//
//  StandardRenderer.cpp
//  VI-RT-LPS
//
//  Created by Luis Paulo Santos on 14/03/2023.
//

#include "StandardRenderer.hpp"
#include "perspective.hpp"

void StandardRenderer::Render() {
//   void StandardRenderer::Render() {
    int W=0,H=0;  // resolution
    int x,y;
    
    // get resolution from the camera
    Perspective* perspCam = dynamic_cast<Perspective*>(cam);
    perspCam->getResolution(&W, &H);

    int white = 0;
    int black = 0;
    int other = 0;
    int NOTgeneratedRay = 0;
    int numintersections = 0;
    int numIntersectionsNOT = 0;
    const bool jitter = true; // Mudar para true

    // main rendering loop: get primary rays from the camera until done
    for (y=0 ; y< H ; y++) {  // loop over rows
        for (x=0 ; x< W ; x++) { // loop over columns
            //Ray primary;
            //Intersection isect;
            //bool intersected;
            RGB color(0.,0.,0.);
            for (int ss=0; ss<spp; ss++) {
                Ray primary;
            
                if (jitter) {
                
                    float jitterV[2];
                    jitterV[0] = ((float)rand())/((float)RAND_MAX);
                    jitterV[1] = ((float)rand())/((float)RAND_MAX);

                    // Generate Ray (camera) 
                    cam->GenerateRay(x, y, &primary, jitterV);       
                    //bool generatedRay = perspCam->GenerateRay(x, y, &primary, jitterV);

                } else {
                    cam->GenerateRay(x, y, &primary);
                    //bool generatedRay = perspCam->GenerateRay(x, y, &primary);
                }

                // trace ray (scene)
                Intersection isect;
                bool intersected = scene->trace(primary, &isect);

                if (intersected == true)
                    numintersections++;
                else 
                    numIntersectionsNOT++;
                // shade this intersection (shader) - remember: depth=0
                RGB this_color = shd->shade(intersected, isect, 0);
                if (color.R == 255 && color.G == 255 && color.B == 255)
                    white++;
                else if (color.R == 0 && color.G == 0 && color.B == 0) 
                    black++;
                else 
                    other++;
                color += this_color;
                    
            }
            color = color/spp;                
            // write the result into the image frame buffer (image)
            img->set(x,y,color);
        } // loop over columns
    } // loop over rows

    // TESTE(s)
    std::cout<<std::endl;
    std::cout<< "******** Renderer/StandardRenderer.cpp ********" << std::endl;
    if (NOTgeneratedRay != 0) {
            float NOTgeneratedRay_100 = (static_cast<float>(NOTgeneratedRay) / (W * H)) * 100.0f;
        std::cout<<"Numero generated rays ERROR : " << NOTgeneratedRay << " px" << " -> " << NOTgeneratedRay_100  << std::endl;
    }
    if (numintersections == 0)
        std::cout << "Nenhuma interseção detetada!" << std::endl;
    else {
        float numintersections_100 = (static_cast<float>(numintersections) / (W * H)) * 100.0f;
        std::cout << "Numero intersections (trace ray) : " << numintersections << " px" << " -> " << numintersections_100 << "%" << std::endl;
    }
    float numIntersectionsNOT_100 = (static_cast<float>(numIntersectionsNOT) / (W * H)) * 100.0f;
    std::cout<<"Numero nao intersections trace ray : " << numIntersectionsNOT << " px" << " -> " << numIntersectionsNOT_100  << "%" << std::endl;
    std::cout<<std::endl;
    float white_100 = (static_cast<float>(white) / (W * H)) * 100.0f;
    float black_100 = (static_cast<float>(black) / (W * H)) * 100.0f;
    float other_100 = (static_cast<float>(other) / (W * H)) * 100.0f;
    std::cout<<"Numero pixels WHITE : " << white << " px" << " -> " << white_100 << "%" << std::endl;
    std::cout<<"Numero pixels BLACK : " << black << " px" << " -> " << black_100 << "%"  << std::endl;
    std::cout<<"Numero pixels OTHER : " << other << " px" << " -> " << other_100  << "%" << std::endl;
    std::cout<< "***********************************************" << std::endl;
    std::cout<<std::endl;
}