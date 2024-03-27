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
    cam->getResolution(&W, &H);
    std::cout << "Resolucao: " << W << "x" << H << "\n";
    // Perspective* perspCam = dynamic_cast<Perspective*>(cam);
    // perspCam->getResolution(&W, &H);

    const float spp = 256.0f;

    int white = 0;
    int black = 0;
    int other = 0;
    int numintersections = 0;
    int numPrim = 0;
    int numIntersectionsNOT = 0;
    // main rendering loop: get primary rays from the camera until done
    for (y=0 ; y< H ; y++) {  // loop over rows
        for (x=0 ; x< W ; x++) { // loop over columns
            Ray primary;
            Intersection isect;
            bool intersected;
            RGB color;

            // for (int ss = 0; ss < spp; ss++) {
                // Generate Ray (camera)
                cam->GenerateRay(x, y, &primary);

                // trace ray (scene)
                intersected = scene->trace(primary, &isect);
                // if (intersected)
                //     numintersections++;

                int result = scene->trace2(primary, &isect);
                if (result == 10)
                    numintersections++;
                else if (result == 20)
                    numIntersectionsNOT++;


                // shade this intersection (shader) - remember: depth=0
                // RGB this_color = shd->shade(intersected, isect, 0);
                color = shd->shade(intersected, isect, 0);

                // color += this_color;
            // }                
            // color = color / spp;

            // Normalize color components
            color.R = std::min(1.0f, std::max(0.0f, color.R));
            color.G = std::min(1.0f, std::max(0.0f, color.G));
            color.B = std::min(1.0f, std::max(0.0f, color.B));

            if (color.R == 255 && color.G == 255 && color.B == 255)
                white++;
            else if (color.R == 0 && color.G == 0 && color.B == 0) 
                black++;
            else 
                other++;
                
            // write the result into the image frame buffer (image)
            img->set(x,y,color);
        } // loop over columns
    } // loop over rows

    // TESTE(s)
    std::cout<<std::endl;
    std::cout<<"Numero intersections (trace ray) : " << (numintersections) << std::endl;
    std::cout<<"Numero nao intersections trace ray : " << (numIntersectionsNOT) << std::endl;
    std::cout<<std::endl;
    std::cout<<"Numero pixels WHITE : " << (white) << " -> " << ((white/(W*H))*100) << "%" << std::endl;
    std::cout<<"Numero pixels BLACK : " << (black) << " -> " << ((black/(W*H))*100) << "%"  << std::endl;
    std::cout<<"Numero pixels OTHER (neste caso blue) : " << (other) << " -> " << ((other/(W*H))*100)  << "%" << std::endl;
    std::cout<<std::endl;

    // o chat disse isto:
    // Parece que sua imagem está sendo renderizada com a cor azul predominante. 
    // Isso sugere que pode haver um problema na geração de raios, na detecção de interseção ou na sombra deles.
    
    // *********************************************** CONTINUAR ***********************************************
}
