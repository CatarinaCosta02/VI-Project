//
//  PointLight.hpp
//  VI-RT-LPS
//
//  Created by Luis Paulo Santos on 06/04/2023.
//

#ifndef AreaLight_hpp
#define AreaLight_hpp

#include "light.hpp"
#include "triangle.hpp"
#include <math.h>

class AreaLight: public Light {
public:
    RGB intensity, power;
    Triangle *gem;
    float pdf;
    AreaLight (RGB _power, Point _v1, 
    Point _v2, Point _v3, Vector _n): power(_power) {
        type = AREA_LIGHT;
        gem = new Triangle (_v1, _v2, _v3, _n);
        pdf = 1.f/gem->area();  // for uniform sampling over the area
        intensity = _power * pdf;
    }
    ~AreaLight () {delete gem;}
    // return the Light RGB radiance for a given point : p
    RGB L (Point p) {return power;}
    RGB L () {return power;}
    // return a point p, RGB radiance and pdf given a pair of random number in [0..[
    RGB Sample_L(float *r, Point *p, float &_pdf) {
    
    // Amostrar um ponto aleatório dentro do triângulo usando coordenadas baricêntricas
    // Aqui, estamos usando coordenadas baricêntricas para amostrar um ponto aleatório dentro do triângulo.
    // Isso nos permite amostrar pontos uniformemente distribuídos dentro do triângulo.
    const float sqrt_r0 = sqrtf(r[0]);
    const float alpha = 1.f - sqrt_r0;
    const float beta = (1.f-r[1]) * sqrt_r0;
    const float gamma = r[1] * sqrt_r0;

    // Calcula o ponto de amostragem dentro do triângulo
    // Usamos as coordenadas baricêntricas para interpolar os vértices do triângulo
    // e calcular as coordenadas tridimensionais do ponto de amostragem dentro do triângulo.
    p->X = alpha * gem->v1.X + beta * gem->v2.X + gamma * gem->v3.X;
    p->Y = alpha * gem->v1.Y + beta * gem->v2.Y + gamma * gem->v3.Y;
    p->Z = alpha * gem->v1.Z + beta * gem->v2.Z + gamma * gem->v3.Z;

    // Calcula a densidade de probabilidade de amostragem no ponto de amostragem
    // Aqui, estamos atribuindo a densidade de probabilidade de amostragem ao parâmetro _pdf.
    // Geralmente, isso é apenas um valor constante que representa a densidade de probabilidade uniforme dentro do triângulo.
    _pdf = pdf;

    // Retorna a intensidade da luz no ponto de amostragem
    // Aqui, estamos retornando a intensidade da luz no ponto de amostragem.
    // Isso geralmente é constante para luzes de área, então não muda com a posição de amostragem dentro do triângulo.
    return intensity;
    }

};

#endif /* AreaLight_hpp */
