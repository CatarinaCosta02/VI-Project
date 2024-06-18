#include <omp.h>
#include <iostream>
#include "StandardRenderer.hpp"
#include "perspective.hpp"
#include <chrono> // Para medir o tempo

#define CHUNK 100

const float spp = 256.0f;

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

    
    int maxThreads = omp_get_max_threads();
    std::cout << "Max OpenMP threads available: " << maxThreads << std::endl;
    omp_set_num_threads(maxThreads);

    int numThreads = omp_get_num_threads(); // Armazena o número de threads no domínio paralelo atual
    int currentThreadNum = omp_get_thread_num(); // Obtém o número da thread atual
    int teamSize = omp_get_team_size(0); // Obtém o tamanho do time para a thread atual no nível mais externo
    int secondLevelTeamSize = omp_get_team_size(1); // Obtém o tamanho do time para a thread atual no segundo nível de aninhamento
    bool isInParallel = omp_in_parallel(); // Verifica se o código está sendo executado dentro de um bloco paralelo

    std::cout << "Current thread number: " << currentThreadNum << std::endl;
    std::cout << "Number of threads in the domain: " << numThreads << std::endl;
    std::cout << "Team size for the current thread: " << teamSize << std::endl;
    std::cout << "Is the code running inside a parallel block? " << (isInParallel? "Yes" : "No") << std::endl;


#pragma omp parallel shared(W, H, perspCam) num_threads(numThreads)
{
#pragma omp for schedule(dynamic, CHUNK)
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