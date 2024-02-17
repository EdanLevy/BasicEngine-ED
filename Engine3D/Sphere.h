//
// Created by denis on 2/15/24.
//

#ifndef GAME_SPHERE_H
#define GAME_SPHERE_H

#include "glm/vec3.hpp"

class Sphere {
public:
    Sphere(const float radius,const glm::vec3& coord,const glm::vec3& color ){
        this->radius=radius; this->coord=coord; this->color=color;
    }
    float radius;
    glm::vec3 coord;
    glm::vec3 color;
};


#endif //GAME_SPHERE_H
