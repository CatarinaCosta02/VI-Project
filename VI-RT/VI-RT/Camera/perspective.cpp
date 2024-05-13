
//
//  perspective.cpp
//  VI-RT
//
//  Created by Luis Paulo Santos on 10/02/2023.
//

#include "perspective.hpp"  // Inclui o cabeçalho da classe Perspective
#include <iostream>
#include <cstdlib>

Perspective::Perspective(Point Eye, const Point At, Vector Up, const int W, const int H, const float fovW, const float fovH)
    : Eye(Eye), At(At), Up(Up), W(W), H(H), fovW(fovW), fovH(fovH) {

    // Vetor para armazenar a direção para frente
    // Calcula o vetor "Forward" representando a direção do olho (Eye) para o ponto de destino (At)
    Vector Forward = Eye.vec2point(At);
    Forward.normalize(); // Normaliza o vetor 'Forward'

    // Calcula o vetor 'Right' como o produto vetorial entre o vetor 'Up' e 'Forward'
    Vector Right = Up.cross(Forward);
    Right.normalize();  // Normaliza o vetor 'Right'

    // Calcula o vetor 'Up' como o produto vetorial entre os vetores 'Forward' e 'Right'
    Vector up = Forward.cross(Right);
    up.normalize();  // Normaliza o vetor 'Up'

    // Preenche a matriz de transformação 'c2w' com os vetores normalizados 'Right', 'Up' e 'Forward'
    c2w[0][0] = Right.X; c2w[0][1] = Right.Y; c2w[0][2] = Right.Z;
    c2w[1][0] = up.X; c2w[1][1] = up.Y; c2w[1][2] = up.Z;
    c2w[2][0] = Forward.X; c2w[2][1] = Forward.Y; c2w[2][2] = Forward.Z;
}



// Implementação do método GenerateRay da classe Perspective
bool Perspective::GenerateRay(const int x, const int y, Ray *r, const float *cam_jitter) {
    float xc;  // Variável para armazenar a coordenada x normalizada do dispositivo (NDC)
    float yc;  // Variável para armazenar a coordenada y normalizada do dispositivo (NDC)

    // Reduzir artefatos de aliasing, como serrilhados ou "dentes de serra"
    // especialmente em bordas diagonais ou linhas finas. 
    // O jitter é uma forma de introduzir uma pequena quantidade de aleatoriedade nas 
    // coordenadas dos pixels ou dos raios de luz, a fim de distribuir melhor a amostragem ao longo dessas bordas.
    // Verifica se foi aplicado jitter à câmera
    if (cam_jitter == NULL) {
        // Se não houver jitter, calcula as coordenadas NDC sem alterações
        xc = 2.f * ((float)x + .5f)/W - 1.f;
        yc = 2.f * ((float)y + .5f)/W - 1.f;
    } else {
        // Se houver jitter, calcula as coordenadas NDC com base no jitter aplicado
        xc = 2.f * ((float)x + cam_jitter[0]) / W - 1.f;
        yc = 2.f * ((float)y + cam_jitter[1])/H - 1.f;
    }

    // ... Generate primary ray

    // Cria um vetor de direção usando as coordenadas NDC calculadas
    Vector dir = Vector(xc, yc, 1);
    

    // Transforma o vetor de direção do espaço da câmera para o espaço do mundo
    Vector dirWorld = Vector(
        c2w[0][0] * dir.X + c2w[0][1] * dir.Y + c2w[0][2] * dir.Z,
        c2w[1][0] * dir.X + c2w[1][1] * dir.Y + c2w[1][2] * dir.Z,
        c2w[2][0] * dir.X + c2w[2][1] * dir.Y + c2w[2][2] * dir.Z
    );

    // Ambas as formas são equivalentes e realizam a mesma operação. 
    //A primeira forma (*r = Ray(Eye, dirWorld);) é uma expressão mais compacta, 
    //enquanto a segunda forma (r->o = Eye; r->dir = dirWorld;) divide a 
    //atribuição em duas linhas separadas para maior clareza.

    *r = Ray(Eye, dirWorld); // Define o raio com origem na posição da câmera e direção transformada
    // r->o = Eye; // Define a origem do raio como a posição da câmera
    // r->dir = dirWorld; // Define a direção do raio como a direção transformada
    
    // foi adicionada a inversa (serve para a bb)
    Vector inv;
    inv.X = 1/dirWorld.X;
    inv.Y = 1/dirWorld.Y;
    inv.Z = 1/dirWorld.Z;
    r->invDir = inv;
    
    r->pix_x = x;
    r->pix_y = y;

    return true;
}



void Perspective::Information() {
    // Imprime informações sobre a posição do olho (Eye) e do ponto para onde a câmera está a apontar (At)
    std::cout << "Posicao do Olho (Eye): (" << Eye.X << ", " << Eye.Y << ", " << Eye.Z << ")" << std::endl;
    std::cout << "Ponto para onde a camera esta a apontar (At): (" << At.X << ", " << At.Y << ", " << At.Z << ")" << std::endl;

    // Imprime informações sobre a largura (W) e altura (H) da janela de visualização
    std::cout << "Largura da janela de visualizacao (W): " << W << std::endl;
    std::cout << "Altura da janela de visualizacao (H): " << H << std::endl;

    // Imprime informações sobre os ângulos de campo de visão horizontal (fovW) e vertical (fovH)
    std::cout << "Angulo de campo de visao horizontal (fovW): " << fovW << " graus" << std::endl;
    std::cout << "Angulo de campo de visao vertical (fovH): " << fovH << " graus" << std::endl;

    std::cout << "A camara esta funcional!" << std::endl;
}
