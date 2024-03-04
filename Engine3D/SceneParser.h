//
// Created by denis on 2/14/24.
//

#ifndef GAME_SCENEPARSER_H
#define GAME_SCENEPARSER_H

#include "Ray.h"
#include <utility>
#include <vector>
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include "glm/detail/func_geometric.hpp"

class SceneObject {
protected:
    glm::vec4 color;
public:
    char type;

    SceneObject(char type, glm::vec4 color): type(type), color(color){}
    virtual float Intersect(const Ray &ray) = 0;
    //virtual glm::vec3 CalcHitpoint(const Ray &ray) = 0;
    virtual glm::vec3 getNormal(glm::vec3 hitpoint) = 0;
    virtual glm::vec4 getColor( glm::vec2 coord) const = 0;
};


class Sphere: public SceneObject {
public:
    Sphere(float radius, glm::vec3 coord, glm::vec4 color, char type)
            : radius(radius), coord(coord), SceneObject(type, color)  {}

    glm::vec3 coord;
    float radius;
//    glm::vec3 CalcHitpoint(const Ray &ray) override{
//        glm::vec3 origin= ray.origin - coord;
//        float a = glm::dot(ray.direction, ray.direction);
//        float b = 2.0f * glm::dot(origin, ray.direction);
//        float c = glm::dot(origin, origin) - radius * radius;
//        float discriminant = b * b - 4.0f * a * c;
//        float nearerHit = (-b - glm::sqrt(discriminant)) / (2.0f * a);
//        return origin + ray.direction * nearerHit;
//    }

    float Intersect(const Ray &ray) override{
        glm::vec3 origin= ray.origin - coord;
        float a = glm::dot(ray.direction, ray.direction);
        float b = 2.0f * glm::dot(ray.direction,origin);
        float c = glm::dot(origin, origin) - radius * radius;
        float discriminant = b * b - 4.0f * a * c;
        if(discriminant<0)
            return -1.0f;
        float nearerHit = (-b - glm::sqrt(discriminant)) / (2.0f * a);
        if (nearerHit >= 0.001f)
            return nearerHit;
        float furtherHit = (b - glm::sqrt(discriminant)) / (2.0f * a);
        if (furtherHit >= 0.001f) {
            return furtherHit;
        }
        return -1.0f;
    }

    glm::vec4 getColor(glm::vec2 coord) const override{
         return color;
    }
    glm::vec3 getNormal(glm::vec3 hitpoint) override{
        return hitpoint-coord;
    }
};

class Plane: public SceneObject {
public:
    float a, b, c, d;
    Plane(float a, float b, float c, float d, const glm::vec4& color, char type) :
    a(a), b(b), c(c), d(d), SceneObject(type, color) {}



//    glm::vec3 CalcHitpoint(const Ray &ray) override{
//        glm::vec3 normal=getNormal();
//        float t = (glm::dot(normal, glm::vec3(ray.origin)) +d) / glm::dot(normal, ray.direction);
//        return ray.origin +t * ray.direction;
//    }
    float Intersect(const Ray &ray) override{
        float denominator = glm::dot(glm::vec3(a, b, c), ray.direction);
        if (denominator < 0.0001f)
            return -1.0;
        float res = -(glm::dot(glm::vec3(a,b,c), ray.origin) + d) / denominator;
        if (res <= 0.001f) {
            return -1.0;
        }
        return res;
    }
    glm::vec4 getColor(glm::vec2 coord)const override{
       // coord = (coord+2.0f) * 0.25f;
       // float multiplier = 100;
       // float checkerColor= (int(multiplier*coord.x)%2) == (int(multiplier*coord.y) % 2)? 0.5f : 1.0f;
 //return glm::vec4(glm::vec3(color*checkerColor),color.a);
        float scale_parameter = 0.5f;

        float chessboard = 0;

        if (coord.x < 0) {
            chessboard += floor((0.5 - coord.x) / scale_parameter);

        }
        else {
            chessboard += floor(coord.x / scale_parameter);
        }
        if (coord.y < 0) {
            chessboard += floor((0.5 - coord.y) / scale_parameter);

        }
        else {
            chessboard += floor(coord.y / scale_parameter);
        }

        chessboard = (chessboard * 0.5) - int(chessboard * 0.5);
        chessboard *= 2;

        if (chessboard <= 0.5) {
            return 0.5f * color;

        }

        return color;
    }
    glm::vec3 getNormal(glm::vec3 hitpoint) override{
        if (glm::dot(glm::vec3(a,b,c), hitpoint) > 0.0001f)
            return -glm::vec3(a,b,c);
        else return glm::vec3(a,b,c);;
    }
private:
    //glm::vec3 getNormal() const {
    //    glm::vec3 normal(a, -b, c);
    //    return glm::normalize(normal);
    //}
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
    //    this->position.x=-this->position.x;
    }
    
};


class ParsedScene {
public:

    ParsedScene(const glm::vec3 &camaraPos, const glm::vec4 &ambientLightColor,
                std::vector<SceneObject*> sceneObj, std::vector<Light> lights, std::vector<Spotlight> spotlights);

    glm::vec3 camaraPos;
    glm::vec4 ambientLightColor;
    std::vector<Sphere> spheres;
    std::vector<SceneObject*> sceneObjects;
    std::vector<Light> lights;
    std::vector<Spotlight> spotlights;

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
