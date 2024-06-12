//
//  RGB.hpp
//  VI-RT
//
//  Created by Luis Paulo Santos on 30/01/2023.
//
// Defines the RGB class, representing a color with red, green, and blue components.
// Includes operators for addition, multiplication, and other common operations.

#ifndef RGB_hpp
#define RGB_hpp

#include <algorithm> // Necessário para std::min e std::max

class RGB {
public:
    float R, G, B;
    RGB ():R(0.),G(0.),B(0.) {}
    RGB (float r, float g, float b):R(r),G(g),B(b) {}
    RGB (float *rgb):R(rgb[0]),G(rgb[1]),B(rgb[2]) {}
    ~RGB () {}
    RGB& operator+=(const RGB& rhs){
        this->R += rhs.R;
        this->G += rhs.G;
        this->B += rhs.B;
        return *this;
    }
    RGB operator+(RGB const& obj) const {
        RGB res;
        res.R = R + obj.R;
        res.G = G + obj.G;
        res.B = B + obj.B;
        return res;
    }
    RGB operator*(RGB const& obj) const {
        RGB res;
        res.R = R * obj.R;
        res.G = G * obj.G;
        res.B = B * obj.B;
        return res;
    }
    RGB operator*(float const& f) const {
        RGB res;
        res.R = R * f;
        res.G = G * f;
        res.B = B * f;
        return res;
    }
    RGB operator/(float const& f) const {
        RGB res;
        res.R = R / f;
        res.G = G / f;
        res.B = B / f;
        return res;
    }
    float Y() const {
        return (R*0.2126 + G*0.7152 + B*0.0722 );
    }
    bool isZero () const {
        return ((R==0.) && (G==0.) && (B==0.));
    }

    // Adicionar método clamp
    void clamp(float minVal, float maxVal) {
        R = std::max(minVal, std::min(R, maxVal));
        G = std::max(minVal, std::min(G, maxVal));
        B = std::max(minVal, std::min(B, maxVal));
    }
};

#endif /* RGB_hpp */

// o método clamp é uma função utilizada para restringir (ou "clamp") 
// os valores de uma variável dentro de um intervalo especificado. 
// No contexto de renderização de imagens, o clamp é frequentemente usado para garantir que os valores de cor não excedam certos limites, 
// evitando artefatos visuais como "fireflies" (pontos brancos muito brilhantes) e garantindo que as cores fiquem dentro de um intervalo de 
// valores que pode ser exibido corretamente por dispositivos de exibição comuns.

// O clamp ajusta o valor atual para que ele fique dentro do intervalo definido pelo valor mínimo e máximo. 
// Se o valor atual for menor que o valor mínimo, ele é ajustado para o valor mínimo. Se for maior que o valor máximo, 
// ele é ajustado para o valor máximo. Se estiver dentro do intervalo, o valor permanece inalterado.