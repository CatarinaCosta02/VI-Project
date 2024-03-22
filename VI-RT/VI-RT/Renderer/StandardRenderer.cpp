//
//  StandardRenderer.cpp
//  VI-RT-LPS
//
//  Created by Luis Paulo Santos on 14/03/2023.
//

#include "StandardRenderer.hpp"

void StandardRenderer::Render(float *cam_jitter) {
    int W=0,H=0;  // resolution
    int x,y;

    // get resolution from the camera
    cam->getResolution(&W, &H);
    std::cout << "Resolucao: " << W << "x" << H << "\n";
    
    // main rendering loop: get primary rays from the camera until done
    for (y=0 ; y< H ; y++) {  // loop over rows
        for (x=0 ; x< W ; x++) { // loop over columns
            Ray primary;
            Intersection isect;
            bool intersected;
            RGB color;
          
            // Generate Ray (camera)
            bool True = cam->GenerateRay(x, y, &primary, cam_jitter);
            
            // trace ray (scene)
            // ...
            
            // shade this intersection (shader) - remember: depth=0
            // ...
            
            // write the result into the image frame buffer (image)
            img->set(x,y,color);
            
        } // loop over columns
    }   // loop over rows
}
