#include <omp.h>
#include <iostream>
#include "StandardRenderer.hpp"
#include "perspective.hpp"
#include <chrono> // Para medir o tempo

void StandardRenderer::Render() {
    int W = 0, H = 0;  // resolução
    int x, y;

    int ss;
    Ray primary;
    Intersection isect;
    RGB this_color(0., 0., 0.);
    RGB color(0., 0., 0.);
    

    // Obter a resolução da câmera
    Perspective* perspCam = dynamic_cast<Perspective*>(cam);
    perspCam->getResolution(&W, &H);

    const bool jitter = true;

    int chunkSize = 5;

    int numThreads = omp_get_max_threads();
    omp_set_num_threads(numThreads);
    std::cout << "estao a ser usadas " << numThreads << " threads com um chunkSize de " << chunkSize <<  std::endl;
    

#pragma omp parallel shared(W, H, perspCam) num_threads(numThreads)
{
#pragma omp for schedule(dynamic, chunkSize)
    for (int y = 0; y < H; y++) {  // loop over rows
        for (int x = 0; x < W; x++) { // loop over columns
            RGB color(0., 0., 0.);
            for (int ss = 0; ss < spp; ss++) {

                float jitterV[2];
                jitterV[0] = ((float)rand()) / ((float)RAND_MAX);
                jitterV[1] = ((float)rand()) / ((float)RAND_MAX);

                // Generate Ray (camera)
                perspCam->GenerateRay(x, y, &primary, jitterV);

                // trace ray (scene)
                bool intersected = scene->trace(primary, &isect);

                // shade this intersection (shader)
                this_color = shd->shade(intersected, isect, 0);


                #pragma omp critical
                {
                    color += this_color;
                }
                
            }
            color = color / spp;

            // write the result into the image frame buffer (image)
            img->set(x, y, color);

        } // loop over columns
    }   // loop over rows
}
}