#pragma once
#include <omp.h>
#include <iostream>
#include "StandardRenderer.hpp"
#include "perspective.hpp"
#include <chrono> // Para medir o tempo

void StandardRenderer::Render() {
    int W = 0, H = 0;  // resolução
    int x, y;

    // Obter a resolução da câmera
    Perspective* perspCam = dynamic_cast<Perspective*>(cam);
    perspCam->getResolution(&W, &H);

    // int white = 0;
    // int black = 0;
    // int other = 0;
    // int NOTgeneratedRay = 0;
    // int numintersections = 0;
    // int numIntersectionsNOT = 0;
    const bool jitter = true; // Mudar para true

    // auto start = std::chrono::high_resolution_clock::now();

    // Usar OpenMP para paralelizar o loop principal de renderização
#pragma omp parallel default(none) shared(perspCam, W, H)
    {
#pragma omp for schedule(static) // Parallelize outer loop
    for (y = 0; y < H; y++) {  // loop sobre as linhas
        for (x = 0; x < W; x++) { // loop sobre as colunas
            RGB color(0., 0., 0.);
            for (int ss = 0; ss < spp; ss++) {
                Ray primary;

                if (jitter) {
                    float jitterV[2];
                    jitterV[0] = ((float)rand()) / ((float)RAND_MAX);
                    jitterV[1] = ((float)rand()) / ((float)RAND_MAX);

                    // Gerar Raio (câmera)
                    cam->GenerateRay(x, y, &primary, jitterV);
                } else {
                    cam->GenerateRay(x, y, &primary);
                }

                // Traçar o raio (cena)
                Intersection isect;
                bool intersected = scene->trace(primary, &isect);

                // if (intersected) {
                //     numintersections++;
                // } else {
                //     numIntersectionsNOT++;
                // }

                // Sombrear essa interseção (shader) - lembrar: depth=0
                RGB this_color = shd->shade(intersected, isect, 0);
                color += this_color;
            }
            color = color / spp;

            // Escrever o resultado no buffer de imagem (imagem)
            img->set(x, y, color);

            // if (color.R == 255 && color.G == 255 && color.B == 255) {
            //     white++;
            // } else if (color.R == 0 && color.G == 0 && color.B == 0) {
            //     black++;
            // } else {
            //     other++;
            // }
        }
    }
    }

    // auto end = std::chrono::high_resolution_clock::now();
    // std::chrono::duration<float> duration = end - start;
    // int num_threads;
    // #pragma omp parallel
    // {
    //     num_threads = omp_get_num_threads();
    // }
    // float render_time = duration.count() / num_threads;

    // // Imprimir estatísticas
    // std::cout << std::endl;
    // std::cout << "******** Renderer/StandardRenderer.cpp ********" << std::endl;
    // std::cout << "Tempo de renderização (ajustado por threads): " << render_time << " segundos" << std::endl;
    // if (NOTgeneratedRay != 0) {
    //     float NOTgeneratedRay_100 = (static_cast<float>(NOTgeneratedRay) / (W * H)) * 100.0f;
    //     std::cout << "Número de rays não gerados: " << NOTgeneratedRay << " px" << " -> " << NOTgeneratedRay_100 << "%" << std::endl;
    // }
    // if (numintersections == 0) {
    //     std::cout << "Nenhuma interseção detectada!" << std::endl;
    // } else {
    //     float numintersections_100 = (static_cast<float>(numintersections) / (W * H)) * 100.0f;
    //     std::cout << "Número de interseções (trace ray): " << numintersections << " px" << " -> " << numintersections_100 << "%" << std::endl;
    // }
    // float numIntersectionsNOT_100 = (static_cast<float>(numIntersectionsNOT) / (W * H)) * 100.0f;
    // std::cout << "Número de não interseções (trace ray): " << numIntersectionsNOT << " px" << " -> " << numIntersectionsNOT_100 << "%" << std::endl;
    // std::cout << std::endl;
    // float white_100 = (static_cast<float>(white) / (W * H)) * 100.0f;
    // float black_100 = (static_cast<float>(black) / (W * H)) * 100.0f;
    // float other_100 = (static_cast<float>(other) / (W * H)) * 100.0f;
    // std::cout << "Número de pixels BRANCOS: " << white << " px" << " -> " << white_100 << "%" << std::endl;
    // std::cout << "Número de pixels PRETOS: " << black << " px" << " -> " << black_100 << "%" << std::endl;
    // std::cout << "Número de pixels OUTROS: " << other << " px" << " -> " << other_100 << "%" << std::endl;
    // std::cout << "***********************************************" << std::endl;
    // std::cout << std::endl;
}
