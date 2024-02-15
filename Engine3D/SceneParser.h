//
// Created by denis on 2/14/24.
//

#ifndef GAME_SCENEPARSER_H
#define GAME_SCENEPARSER_H


#include <vector>
#include "Sphere.h"
#include "Plane.h"

class SceneParser {
public:

private:

};

class ParsedScene {
public:
    std::vector<Sphere>  spheres;
    std::vector<Plane>  planes;

    glm::vec3  cameraCoord;
    glm::vec3  aLight; // alpha is 1.
    std::vector<glm::vec3> directionalLights;
    std::vector<glm::vec3> spotlights;

};
#endif //GAME_SCENEPARSER_H
