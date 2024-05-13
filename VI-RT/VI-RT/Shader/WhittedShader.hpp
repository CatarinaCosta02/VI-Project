//
//  AmbientShader.hpp
//  VI-RT-LPS
//
//  Created by Luis Paulo Santos on 14/03/2023.
//

// calcula a aparencia individual de cada pixel, neste caso, a cor do pixel consoante a luz ambiente e a luz direta
// somando a luz ambiente e a luz direta
#ifndef WhittedShader_hpp
#define WhittedShader_hpp

#include "shader.hpp"
#include "Phong.hpp"

class WhittedShader: public Shader {
    RGB background;
    RGB directLighting (Intersection isect, Phong *f);
    RGB specularReflection (Intersection isect, Phong *f, int depth);
public:
    WhittedShader (Scene *scene, RGB bg): background(bg), Shader(scene) {}
    RGB shade (bool intersected, Intersection isect, int depth); //override;
};

#endif /* AmbientShader_hpp */
