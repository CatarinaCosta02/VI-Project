//
//  renderer.hpp
//  VI-RT
//
//  Created by Luis Paulo Santos on 10/02/2023.
//

#ifndef renderer_hpp
#define renderer_hpp

#include "camera.hpp"
#include "scene.hpp"
#include "image.hpp"
#include "shader.hpp"

class Renderer {
protected:
    Camera *cam;
    Scene *scene;
    Image * img;
    Shader *shd;
public:
    Renderer (Camera *cam, Scene * scene, Image * img, Shader *shd): cam(cam), scene(scene), img(img), shd(shd) {}
    virtual void Render (float *cam_jitter) {}
};

#endif /* renderer_hpp */
