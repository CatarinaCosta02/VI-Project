//
//  AABB.hpp
//  VI-RT
//
//  Created by Luis Paulo Santos on 30/01/2023.
//

#ifndef BB_hpp
#define BB_hpp

#include "vector.hpp"
#include "ray.hpp"

#include <iostream>

typedef struct BB {
public:
    Point min, max;
    void update (Point p) {
        if (p.X < min.X) min.X = p.X;
        else if (p.X > max.X) max.X = p.X;
        if (p.Y < min.Y) min.Y = p.Y;
        else if (p.Y > max.Y) max.Y = p.Y;
        if (p.Z < min.Z) min.Z = p.Z;
        else if (p.Z > max.Z) max.Z = p.Z;
    }
    /*
     * I suggest you implement:
     *  bool intersect (Ray r) { }
     *
     * based on PBRT's 3rd ed. book , sec 3.1.2, pags 125..128 + 214,217,221
     *
     * or https://doi.org/10.1007/978-1-4842-7185-8_2
     *
     */

    
    // https://gamedev.stackexchange.com/questions/18436/most-efficient-aabb-vs-ray-collision-algorithms
    // eu segui isto
    // esta função verifica primeiro se o rai interseta o plano da Bounding Box
    bool intersect (Ray r) { 

        // if (r.pix_x == 320 && r.pix_y == 240) {
            // std::cout << "Ray: " << r.pix_x << " " << r.pix_y << std::endl;
        // }
        // primerio calcular os valores inversos das componentes do raio
        // calcular os valores de tmin e tmax
        float tminX = (min.X - r.o.X) * r.invDir.X;
        float tmaxX = (max.X - r.o.X) * r.invDir.X;
        float tminY = (min.Y - r.o.Y) * r.invDir.Y;
        float tmaxY = (max.Y - r.o.Y) * r.invDir.Y;
        float tminZ = (min.Z - r.o.Z) * r.invDir.Z;
        float tmaxZ = (max.Z - r.o.Z) * r.invDir.Z;

        // Encontra o tmin e tmax que são os menores e maiores entre os eixos
        float tmin = fmax(fmax(fmin(tminX, tmaxX), fmin(tminY, tmaxY)), fmin(tminZ, tmaxZ));
        float tmax = fmin(fmin(fmax(tminX, tmaxX), fmax(tminY, tmaxY)), fmax(tminZ, tmaxZ));

        if (tmin <= tmax && tmax >= 0 && tmin >= 0)
            return true;
        else 
            return false;
    }
} BB;

#endif /* AABB_hpp */
