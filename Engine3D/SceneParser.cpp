//
// Created by denis on 2/14/24.
//

#include <fstream>
#include <iostream>
#include <sstream>
#include "SceneParser.h"


ParsedScene *SceneParser::parse() {
    std::ifstream file(filename);
    std::string line;

    glm::vec3 camaraPos;
    glm::vec4 ambientLightColor;
    std::vector<Sphere> spheres = std::vector<Sphere>();
    std::vector<Plane> planes = std::vector<Plane>();
    std::vector<Light> lights = std::vector<Light>();
    std::vector<Spotlight> spotlights = std::vector<Spotlight>();

    if (!file.is_open()) {
        std::cerr << "Unable to open file!" << std::endl;
        return nullptr;
    }

    std::vector<glm::vec4> objs = std::vector<glm::vec4>();
    std::vector<char> objsTypes = std::vector<char>();
    std::vector<glm::vec4> objColors = std::vector<glm::vec4>();
    std::vector<glm::vec4> lightDirections = std::vector<glm::vec4>();
    std::vector<glm::vec4> lightIntensity = std::vector<glm::vec4>();
    std::vector<glm::vec4> lightPosition = std::vector<glm::vec4>();

    while (std::getline(file, line)) {
        // split the line by spaces
        std::istringstream iss(line);
        std::vector<std::string> tokens;
        char type;
        float a, b, c, d;
        iss >> type;
        switch (type) {
            case 'e':
                iss >> camaraPos.x >> camaraPos.y >> camaraPos.z;
                break;
            case 'a':
                iss >> ambientLightColor.r >> ambientLightColor.g >> ambientLightColor.b >> ambientLightColor.a;
                break;
            case 'o':
            case 'r':
            case 't':
                iss >> a >> b >> c >> d;
                objs.emplace_back(a, b, c, d);
                objsTypes.emplace_back(type);
                break;
            case 'c':
                iss >> a >> b >> c >> d;
                objColors.emplace_back(a, b, c, d);
                break;
            case 's':
            case 'd':
                iss >> a >> b >> c >> d;
                lightDirections.emplace_back(a, b, c, d);
                break;
            case 'i':
                iss >> a >> b >> c >> d;
                lightIntensity.emplace_back(a, b, c, d);
                break;
            case 'p':
                iss >> a >> b >> c >> d;
                lightPosition.emplace_back(a, b, c,d);
                break;
            default:
                std::cerr << "Unknown type: " << type << std::endl;
                break;
        }

    }

    for (int i = 0; i < objs.size(); ++i) {
        if (objs[i].w > 0.0f) {
            // todo: added "-" to x component to make the position correct. maybe we dont need to do this?
            spheres.emplace_back(objs[i].w, glm::vec3(objs[i].x, -objs[i].y, objs[i].z), objColors[i], objsTypes[i]);
        } else {
            planes.emplace_back(objs[i].x, objs[i].y, objs[i].z, objs[i].w, objColors[i], objsTypes[i]);
        }
    }

    for (int i = 0; i < lightDirections.size(); ++i) {
        if (lightDirections[i].w == 0) { // directional light
            lights.emplace_back(glm::vec3(lightDirections[i].x, lightDirections[i].y, lightDirections[i].z),
                                lightIntensity[i]);
        } else { // spotlight
            spotlights.emplace_back(glm::vec3(lightDirections[i]),lightIntensity[i],glm::vec4(lightPosition[i].x, lightPosition[i].y, lightPosition[i].z,lightPosition[i].w));
        }

    }

    for (int i = 0; i < spotlights.size(); ++i) {
        spotlights[i].direction = glm::normalize(spotlights[i].direction);
        spotlights[i].position = lightPosition[i];
    }

    return new ParsedScene(camaraPos, ambientLightColor, spheres, planes, lights, spotlights);

}

SceneParser::~SceneParser() = default;


ParsedScene::ParsedScene(const glm::vec3 &camaraPos, const glm::vec4 &ambientLightColor, std::vector<Sphere> spheres,
                         std::vector<Plane> planes, std::vector<Light> lights, std::vector<Spotlight> spotlights) {
    this->camaraPos = camaraPos;
    this->ambientLightColor = ambientLightColor;
    this->spheres = std::move(spheres);
    this->planes = std::move(planes);
    this->lights = std::move(lights);
    this->spotlights = std::move(spotlights);

}
