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
    glm::vec3  cameraCoord=glm::vec3 {0.0f,0.0f,2.0f};
    glm::vec3  ambientLight=glm::vec3 {0.0f,0.0f,0.0f}; // alpha is 1.
    std::vector<glm::vec3> directionalLights;
    std::vector<glm::vec3> spotlights;
    void AddPlane(const Plane& plane) {
        planes.push_back(plane);
    }
    void AddSphere(const Sphere& sphere) {
        spheres.push_back(sphere);
    }
    void AddDirectionalLight(const glm::vec3& coord) {
        directionalLights.push_back(coord);
    }

};
#endif //GAME_SCENEPARSER_H
