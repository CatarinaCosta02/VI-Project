#include "PathTracerShader.hpp"
#include "Phong.hpp"
#include "ray.hpp"
#include "AreaLight.hpp"
#include <stdlib.h>
#include <math.h>

const float m_pi = 3.14159265358979323846;
const float MAX_INTENSITY = 10.0f; // Valor de clamp para intensidade

RGB PathTracerShader::directLighting (Intersection isect, Phong *f) {
    RGB color(0., 0., 0.);
    Light *l;
    int l_ndx;
    const bool RANDOM_SAMPLE_ONE = true;
    float light_pdf;

    for (auto l_iter = scene->lights.begin(); l_iter != scene->lights.end(); l_iter++) {
        RGB this_l_color(0., 0., 0.);
        l = (Light *)(*l_iter);

        if (RANDOM_SAMPLE_ONE) {
            l_ndx = rand() % scene->numLights;
            l = scene->lights[l_ndx];
            light_pdf = 1.f / ((float)scene->numLights);
        }

        if (l->type == AMBIENT_LIGHT) {
            if (!f->Ka.isZero()) {
                RGB Ka = f->Ka;
                this_l_color = Ka * l->L();
            }
        }
        if (l->type == POINT_LIGHT) {
            if (!f->Kd.isZero()) {
                RGB L, Kd = f->Kd;
                Point lpoint;
                L = l->Sample_L(NULL, &lpoint);
                Vector Ldir = isect.p.vec2point(lpoint);
                const float Ldistance = Ldir.norm();
                Ldir.normalize();
                float cosL = Ldir.dot(isect.sn);

                if (cosL > 0.) {
                    Ray shadow(isect.p, Ldir);
                    shadow.pix_x = isect.pix_x;
                    shadow.pix_y = isect.pix_y;
                    shadow.FaceID = isect.FaceID;
                    shadow.adjustOrigin(isect.gn);

                    if (scene->visibility(shadow, Ldistance - EPSILON)) {
                        this_l_color = Kd * L * cosL;
                    }
                }
            }
        }
        if (l->type == AREA_LIGHT) {
            if (!f->Kd.isZero()) {
                RGB L, Kd = f->Kd;
                Point lpoint;
                float l_pdf;
                AreaLight *al = (AreaLight *)l;
                float rnd[2];
                rnd[0] = ((float)rand()) / ((float)RAND_MAX);
                rnd[1] = ((float)rand()) / ((float)RAND_MAX);
                L = al->Sample_L(rnd, &lpoint, l_pdf);
                Vector Ldir = isect.p.vec2point(lpoint);
                const float Ldistance = Ldir.norm();
                Ldir.normalize();
                float cosL = Ldir.dot(isect.sn);
                float cosL_LA = Ldir.dot(al->gem->normal);

                if (cosL > 0. && cosL_LA <= 0.) {
                    Ray shadow(isect.p, Ldir);
                    shadow.pix_x = isect.pix_x;
                    shadow.pix_y = isect.pix_y;
                    shadow.FaceID = isect.FaceID;
                    shadow.adjustOrigin(isect.gn);

                    if (scene->visibility(shadow, Ldistance - EPSILON)) {
                        this_l_color += (Kd * L * cosL) / l_pdf;
                    }
                }
            }
        }

        if (RANDOM_SAMPLE_ONE) {
            color = this_l_color / light_pdf;
            break;
        } else {
            color += this_l_color;
        }
    }

    // Clamp a intensidade da luz
    color.clamp(0.0, MAX_INTENSITY);

    return color;
}

RGB PathTracerShader::specularReflection (Intersection isect, Phong *f, int depth) {
    RGB color(0., 0., 0.);
    Vector Rdir, s_dir;
    float pdf;

    float cos = isect.gn.dot(isect.wo);
    Rdir = 2.f * cos * isect.gn - isect.wo;

    if (f->Ns < 1000) {
        float rnd[2];
        rnd[0] = ((float)rand()) / ((float)RAND_MAX);
        rnd[1] = ((float)rand()) / ((float)RAND_MAX);

        Vector S_around_N;
        const float cos_theta = powf(rnd[1], 1. / (f->Ns + 1.));
        S_around_N.Z = cos_theta;
        const float aux_r1 = powf(rnd[1], 2. / (f->Ns + 1.));
        S_around_N.Y = sinf(2. * m_pi * rnd[0]) * sqrtf(1. - aux_r1);
        S_around_N.X = cosf(2. * m_pi * rnd[0]) * sqrtf(1. - aux_r1);
        const float cos_pow = powf(cos_theta, f->Ns) / (2.f * m_pi);
        pdf = (f->Ns + 1.f) * cos_pow;

        Vector Rx, Ry;
        Rdir.CoordinateSystem(&Rx, &Ry);

        s_dir = S_around_N.Rotate(Rx, Ry, Rdir);

        Ray specular(isect.p, s_dir);

        specular.pix_x = isect.pix_x;
        specular.pix_y = isect.pix_y;
        specular.FaceID = isect.FaceID;
        specular.adjustOrigin(isect.gn);

        bool intersected;
        Intersection s_isect;
        intersected = scene->trace(specular, &s_isect);

        RGB Rcolor = shade(intersected, s_isect, depth + 1);

        color = (f->Ks * Rcolor) / pdf;

    } else {
        Ray specular(isect.p, Rdir);
        specular.pix_x = isect.pix_x;
        specular.pix_y = isect.pix_y;
        specular.FaceID = isect.FaceID;
        specular.adjustOrigin(isect.gn);

        bool intersected;
        Intersection s_isect;
        intersected = scene->trace(specular, &s_isect);

        RGB Rcolor = shade(intersected, s_isect, depth + 1);

        color = (f->Ks * Rcolor);
    }

    // Clamp a intensidade da luz
    color.clamp(0.0, MAX_INTENSITY);

    return color;
}

RGB PathTracerShader::diffuseReflection (Intersection isect, Phong *f, int depth) {
    RGB color(0., 0., 0.);
    Vector dir;
    float pdf;

    float rnd[2];
    rnd[0] = ((float)rand()) / ((float)RAND_MAX);
    rnd[1] = ((float)rand()) / ((float)RAND_MAX);

    Vector D_around_Z;
    float cos_theta = D_around_Z.Z = sqrt(rnd[1]);
    D_around_Z.Y = sinf(2. * m_pi * rnd[0]) * sqrt(1. - rnd[1]);
    D_around_Z.X = cosf(2. * m_pi * rnd[0]) * sqrt(1. - rnd[1]);
    pdf = cos_theta / (m_pi);

    Vector Rx, Ry;
    isect.gn.CoordinateSystem(&Rx, &Ry);

    dir = D_around_Z.Rotate(Rx, Ry, isect.gn);
    Ray diffuse(isect.p, dir);

    diffuse.pix_x = isect.pix_x;
    diffuse.pix_y = isect.pix_y;
    diffuse.FaceID = isect.FaceID;
    diffuse.adjustOrigin(isect.gn);

    Intersection d_isect;
    bool intersected = scene->trace(diffuse, &d_isect);

    if (!d_isect.isLight) {
        RGB Rcolor = shade(intersected, d_isect, depth + 1);
        color = (f->Kd * cos_theta * Rcolor) / pdf;
    }

    // Clamp a intensidade da luz
    color.clamp(0.0, MAX_INTENSITY);

    return color;
}

RGB PathTracerShader::shade(bool intersected, Intersection isect, int depth) {
    RGB color(0., 0., 0.);

    if (!intersected) {
        return (background);
    }

    if (isect.isLight) {
        return isect.Le;
    }

    Phong *f = (Phong *)isect.f;

    float rnd_russian = ((float)rand()) / ((float)RAND_MAX);
    if (depth < MAX_DEPTH || rnd_russian < continue_p) {
        RGB lcolor;
        float rnd = ((float)rand()) / ((float)RAND_MAX);
        float s_p = f->Ks.Y() / (f->Ks.Y() + f->Kd.Y());
        if (rnd < s_p) {
            lcolor = specularReflection(isect, f, depth + 1) / s_p;
        } else {
            lcolor = diffuseReflection(isect, f, depth + 1) / (1. - s_p);
        }
        if (depth < MAX_DEPTH) {
            color += lcolor;
        } else {
            color += lcolor / continue_p;
        }
    }

    if (!f->Kd.isZero()) {
        color += directLighting(isect, f);
    }

    return color;
}
