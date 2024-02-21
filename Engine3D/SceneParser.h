//
// Created by denis on 2/14/24.
//

#ifndef GAME_SCENEPARSER_H
#define GAME_SCENEPARSER_H


#include <utility>
#include <vector>
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include "glm/detail/func_geometric.hpp"

struct Sphere {
public:
    Sphere(float radius, glm::vec3 coord, glm::vec4 color, char type)
            : radius(radius), coord(coord), color(color), type(type) {}

    char type;
    float radius;
    glm::vec3 coord;
    glm::vec4 color;
};

struct Plane {
public:
    char type;
    int a, b, c, d;
    //Calculate normal?
    glm::vec4 color;

    Plane(int a, int b, int c, int d, const glm::vec4& color, char type) : a(a), b(b), c(c), d(d), color(color), type(type) {}
};

struct Eye {
    float x, y, z, w;
};

struct AmbientLight {
    float r, g, b, a;
};

struct Light {
public:
    glm::vec3 direction;
    glm::vec4 intensity;
    
    Light(glm::vec3 direction, glm::vec4 intensity) {
        this->direction = direction;
        this->intensity = intensity;
    }

};

struct Spotlight {
public:
    glm::vec3 direction;
    glm::vec4 intensity;
    glm::vec4 position;
    
    Spotlight(glm::vec3 direction, glm::vec4 intensity, glm::vec4 position) {
        this->direction = direction;
        this->intensity = intensity;
        this->position = position;
    }
    
};


class ParsedScene {
public:

    ParsedScene(const glm::vec3 &camaraPos, const glm::vec4 &ambientLightColor, std::vector<Sphere> spheres,
                std::vector<Plane> planes, std::vector<Light> lights, std::vector<Spotlight> spotlights);

    glm::vec3 camaraPos;
    glm::vec4 ambientLightColor;
    std::vector<Sphere> spheres;
    std::vector<Plane> planes;
    std::vector<Light> lights;
    std::vector<Spotlight> spotlights;

    void AddPlane(const Plane &plane) {
        planes.push_back(plane);
    }

    void AddSphere(const Sphere &sphere) {
        spheres.push_back(sphere);
    }

};

class SceneParser {
public:
    explicit SceneParser(std::string filename): filename(std::move(filename)) {}

    ~SceneParser();

    ParsedScene *parse();

private:
    std::string filename;

};


#endif //GAME_SCENEPARSER_H
