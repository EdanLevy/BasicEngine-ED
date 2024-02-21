#include "scene.h"
#include <iostream>
#include "glad/include/glad/glad.h"

static void printMat(const glm::mat4 mat) {
    printf(" matrix: \n");
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++)
            printf("%f ", mat[j][i]);
        printf("\n");
    }
}

Scene::Scene() {
    //
    glLineWidth(5);

    cameras.push_back(new Camera(60.0f, 1.0, 0.1f, 100.0f));
    pickedShape = -1;
    depth = 0;
    cameraIndx = 0;
    xold = 0;
    yold = 0;
    isActive = false;
}

Scene::Scene(float angle, float relationWH, float near1, float far1) {
    //glLineWidth(5);
    cameras.push_back(new Camera(angle, relationWH, near1, far1));
    pickedShape = -1;
    depth = 0;
    cameraIndx = 0;
    xold = 0;
    yold = 0;
    isActive = false;
}

void Scene::AddShapeFromFile(const std::string &fileName, int parent, unsigned int mode) {
    chainParents.push_back(parent);
    shapes.push_back(new Shape(fileName, mode));
}

void Scene::AddShape(int type, int parent, unsigned int mode) {
    chainParents.push_back(parent);
    shapes.push_back(new Shape(type, mode));
}

void Scene::AddShapeCopy(int indx, int parent, unsigned int mode) {
    chainParents.push_back(parent);
    shapes.push_back(new Shape(*shapes[indx], mode));
}

void Scene::AddShader(const std::string &fileName) {
    shaders.push_back(new Shader(fileName));
}

void Scene::AddTexture(const std::string &textureFileName, bool for2D) {
    textures.push_back(new Texture(textureFileName));
}

void Scene::AddTexture(int width, int height, unsigned char *data) {
    textures.push_back(new Texture(width, height, data));
}

void Scene::AddCamera(glm::vec3 &pos, float fov, float relationWH, float zNear, float zFar) {
    cameras.push_back(new Camera(fov, relationWH, zNear, zFar));
    cameras.back()->MyTranslate(pos, 0);
}

void Scene::Draw(int shaderIndx, int cameraIndx, int buffer, bool toClear, bool debugMode) {
    glEnable(GL_DEPTH_TEST);
    glm::mat4 Normal = MakeTrans();

    glm::mat4 MVP = cameras[cameraIndx]->GetViewProjection() * glm::inverse(cameras[cameraIndx]->MakeTrans());
    int p = pickedShape;
    if (toClear) {
        if (shaderIndx > 0)
            Clear(1, 0, 1, 1);
        else
            Clear(0, 0, 0, 0);
    }
    //Render();
    for (unsigned int i = 0; i < shapes.size(); i++) {
        if (shapes[i]->Is2Render()) {
            glm::mat4 Model = Normal * shapes[i]->MakeTrans();

            if (shaderIndx > 0) {
                Update(MVP, Model, shapes[i]->GetShader());
                shapes[i]->Draw(shaders, textures, false);
            } else { //picking
                Update(MVP, Model, 0);
                shapes[i]->Draw(shaders, textures, true);
            }
        }
    }
    pickedShape = p;
}

void Scene::MoveCamera(int cameraIndx, int type, float amt) {
    switch (type) {
        case xTranslate:
            cameras[cameraIndx]->MyTranslate(glm::vec3(amt, 0, 0), 0);
            break;
        case yTranslate:
            cameras[cameraIndx]->MyTranslate(glm::vec3(0, amt, 0), 0);
            break;
        case zTranslate:
            cameras[cameraIndx]->MyTranslate(glm::vec3(0, 0, amt), 0);
            break;
        case xRotate:
            cameras[cameraIndx]->MyRotate(amt, glm::vec3(1, 0, 0), 0);
            break;
        case yRotate:
            cameras[cameraIndx]->MyRotate(amt, glm::vec3(0, 1, 0), 0);
            break;
        case zRotate:
            cameras[cameraIndx]->MyRotate(amt, glm::vec3(0, 0, 1), 0);
            break;
        default:
            break;
    }
}

void Scene::ShapeTransformation(int type, float amt) {
    if (glm::abs(amt) > 1e-5) {
        switch (type) {
            case xTranslate:
                shapes[pickedShape]->MyTranslate(glm::vec3(amt, 0, 0), 0);
                break;
            case yTranslate:
                shapes[pickedShape]->MyTranslate(glm::vec3(0, amt, 0), 0);
                break;
            case zTranslate:
                shapes[pickedShape]->MyTranslate(glm::vec3(0, 0, amt), 0);
                break;
            case xRotate:
                shapes[pickedShape]->MyRotate(amt, glm::vec3(1, 0, 0), 0);
                break;
            case yRotate:
                shapes[pickedShape]->MyRotate(amt, glm::vec3(0, 1, 0), 0);
                break;
            case zRotate:
                shapes[pickedShape]->MyRotate(amt, glm::vec3(0, 0, 1), 0);
                break;
            default:
                break;
        }
    }

}

void Scene::Resize(int width, int height) {

    cameras[0]->SetProjection(cameras[0]->GetAngle(), (float) width / height);
    glViewport(0, 0, width, height);
    std::cout << cameras[0]->GetRelationWH() << std::endl;
}

float Scene::Picking(int x, int y) {


    return 0;
}

//return coordinates in global system for a tip of arm position is local system 
void Scene::MouseProccessing(int button) {
    if (pickedShape == -1) {
        if (button == 1) {

            MyTranslate(glm::vec3(-xrel / 20.0f, 0, 0), 0);
            MyTranslate(glm::vec3(0, yrel / 20.0f, 0), 0);
            WhenTranslate();
        } else {
            MyRotate(xrel / 2.0f, glm::vec3(1, 0, 0), 0);
            MyRotate(yrel / 2.0f, glm::vec3(0, 0, 1), 0);
            WhenRotate();
        }
    }
}

void Scene::ZeroShapesTrans() {
    for (unsigned int i = 0; i < shapes.size(); i++) {
        shapes[i]->ZeroTrans();
    }
}

void Scene::ReadPixel() {
    glReadPixels(1, 1, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);
}

void Scene::UpdatePosition(float xpos, float ypos) {
    xrel = xold - xpos;
    yrel = yold - ypos;
    xold = xpos;
    yold = ypos;
}

void Scene::HideShape(int shpIndx) {
    shapes[shpIndx]->Hide();
}

void Scene::UnhideShape(int shpIndx) {

    shapes[shpIndx]->Unhide();
}

void Scene::Clear(float r, float g, float b, float a) {
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

Scene::~Scene(void) {
    for (Shape *shp: shapes) {
        delete shp;
    }
    for (Camera *cam: cameras) {
        delete cam;
    }
    for (Shader *sdr: shaders) {
        delete sdr;
    }
    for (Texture *tex: textures) {
        delete tex;
    }

}

/**
 * loops over the screen and for each pixal, renders it according to the parsed scene in ps
 * @param widthSize screen width
 * @param heightSize screen hight
 * @param ps parsed scene
 */
void Scene::Render(int widthSize, int heightSize, const ParsedScene &ps) {
    int width, height;
    for (height = 0; height < heightSize; height++) {
        for (width = 0; width < widthSize; width++) {
            glm::vec2 coord = {-1, -1};
            coord.x += ((2.0f / widthSize) / 2.0f) + (2.0f / widthSize) * height;
            coord.y += ((2.0f / heightSize) / 2.0f) + (2.0f / heightSize) * width;
            //glm::vec3 pixelVal= glm::vec3 {1.0,1.0,0};
           // pixelVal*=isCheckersDiffusionDistribution(coord);
           Ray ray;
           ray.origin=glm::vec3 (ps.camaraPos);
           ray.direction=glm::normalize(glm::vec3(coord.x- ps.camaraPos.x,coord.y- ps.camaraPos.y,0.0f- ps.camaraPos.z));
            glm::vec3 pixelVal = TraceRay(ray, ps);
            pixelVal = glm::clamp(pixelVal, glm::vec3(0.0f), glm::vec3(1.0f));
            screen[((height + width * widthSize) * 4)] = (int) (pixelVal.r * 255);
            screen[((height + width * widthSize) * 4) + 1] = (int) (pixelVal.g * 255);
            screen[((height + width * widthSize) * 4) + 2] = (int) (pixelVal.b * 255);
        }
    }
}
bool intersectPlane(const Plane &plane, const glm::vec3 &rayDirection) {
    // Calculate dot product of ray direction and plane normal
    glm::vec3 normal=glm::normalize(glm::vec3(plane.a,plane.b,plane.c));
    float dotProduct = glm::dot(rayDirection, normal);

    // If dot product is not zero, ray intersects the plane
    return dotProduct != 0.0f;
}

float calculateIntersectionDist(const Plane &plane, const glm::vec3 &rayOrigin, const glm::vec3 &rayDirection) {
    // Calculate t
    float t = -(glm::dot(glm::vec3(plane.a, plane.b, plane.c), glm::vec3(rayOrigin.x, rayOrigin.y, rayOrigin.z)) +
                plane.d) /
              glm::dot(glm::vec3(plane.a, plane.b, plane.c), rayDirection);
    // Calculate intersection point
    return t;
}

glm::vec3 Scene::TraceRay(const Ray &ray, const ParsedScene &ps) {
    glm::vec3 pixelColor= glm::vec3 (0.0f);
    const Sphere* hitSphere= nullptr;
    float nearestSphereDist=std::numeric_limits<float>::max(); //any distance always is smaller than this value
    const struct Plane* hitPlane= nullptr;
    float nearestPLaneDist=std::numeric_limits<float>::max(); //any distance always is smaller than this value
    for(const auto &sphere: ps.spheres) {
        glm::vec3 origin= ray.origin - sphere.coord;
        float a = glm::dot(ray.direction, ray.direction);
        float b = 2.0f * glm::dot(origin, ray.direction);
        float c = glm::dot(origin, origin) - sphere.radius * sphere.radius;

        float discriminant = b * b - 4.0f * a * c;

        if (discriminant >= 0.0f) {
            float nearerHit = (-b - glm::sqrt(discriminant)) / (2.0f * a);
            if(nearerHit < nearestSphereDist){
                nearestSphereDist = nearerHit;
                hitSphere= &sphere;
            }
        }
    }
    for (const auto &plane: ps.planes) {
        //   glm::vec3 rayDirection(coord.x - cameraCoord.x, coord.y - cameraCoord.y, 0.0f - cameraCoord.z);
        //rayDirection = glm::normalize(ray.direction);
        //calculate if the ray intersects the plane
        bool intersect = intersectPlane(plane, ray.direction);
        if (!intersect) { continue; }
        float intersection = calculateIntersectionDist(plane, ray.origin, ray.direction);
        if(intersection < nearestPLaneDist){
            hitPlane = &plane;
            nearestPLaneDist=intersection;
        }

        // Calculate lighting
//        glm::vec4 finalColor = calculateLighting(ps, surfaceColor, normal);

    }
  //  glm::vec4 surfaceColor = plane.color;

//    pixelVal = {plane.color.x, plane.color.y, plane.color.z};
//    pixelVal += glm::vec3(ps.ambientLightColor * plane.color);
//    pixelVal *= isCheckersDiffusionDistribution(coord);
    glm::vec3 color;
    glm::vec3 hitPoint;
    if(hitSphere == nullptr && hitPlane== nullptr) //didn't hit any object
        return glm::vec3(0.0f);
    if(hitSphere != nullptr & hitPlane == nullptr) { //ray has hit a sphere
        color=glm::vec3(hitSphere->color);
        glm::vec3 origin = ray.origin - hitSphere->coord;
        hitPoint = origin + ray.direction * nearestSphereDist;

    }
    if(hitSphere == nullptr & hitPlane != nullptr) { //ray has hit a plane

    }
    else {
        if(nearestSphereDist< nearestPLaneDist){
            color=glm::vec3(hitSphere->color);
            glm::vec3 origin = ray.origin - hitSphere->coord;
            hitPoint = origin + ray.direction * nearestSphereDist;
        }
        else {

        }
    }
    pixelColor+= glm::vec3(ps.ambientLightColor) * color;

    //ambient color
   // glm::vec3 origin = ray.origin - hitSphere->coord;
   // glm::vec3 hitPoint = origin + ray.direction * nearestSphereDist;
    glm::vec3 normal = glm::normalize(hitPoint);
    for (const auto &light: ps.lights) {
        glm::vec3 lightDir = glm::normalize(glm::vec3(-light.direction.x,-light.direction.y,light.direction.z));
        //NOTE: HAD TO NEGATE DIRECTION WHEN COMPARING WITH EXPECTED RESULT, NO IDEA WHY
        float lightIntensity = glm::max(glm::dot(normal, -lightDir), 0.0f); // == cos(angle)
        pixelColor += glm::vec3(hitSphere->color) * glm::vec3 (light.intensity) * lightIntensity;
        glm::vec3 reflectDir = glm::reflect(lightDir, normal);
        float materialSpecular= 0.7f;
        float specular = glm::pow(glm::max(glm::dot(normal, reflectDir), 0.0f), 3*hitSphere->color.a);
        pixelColor += glm::vec3(hitSphere->color * light.intensity) * specular* materialSpecular;
        //TODO: SPECULAR TOO SPREAD OUT, WHY?
    }

    for (const auto &spotlight: ps.spotlights) {
        glm::vec3 lightDir = glm::normalize(glm::vec3(-spotlight.direction.x,-spotlight.direction.y,spotlight.direction.z));
        //NOTE: HAD TO NEGATE DIRECTION WHEN COMPARING WITH EXPECTED RESULT, NO IDEA WHY
        float coneFactor = glm::dot(-lightDir, glm::normalize(spotlight.direction));
        if (coneFactor <= spotlight.position.a) {
            float lightIntensity = glm::max(glm::dot(normal, -lightDir), 0.0f); // == cos(angle)
            pixelColor += glm::vec3(hitSphere->color) * glm::vec3(spotlight.intensity) * lightIntensity;
            glm::vec3 reflectDir = glm::reflect(lightDir, normal);
            float materialSpecular = 0.7f;
            float specular = glm::pow(glm::max(glm::dot(normal, reflectDir), 0.0f), hitSphere->color.a);
            pixelColor += glm::vec3(hitSphere->color * spotlight.intensity) * specular * materialSpecular;
            //TODO: SPECULAR TOO SPREAD OUT, WHY?
        }
    }
    return pixelColor;
}

/*
 *  scalar for coord in calculation determines checkers size, bigger scalar smaller checkers
 */
float Scene::isCheckersDiffusionDistribution(glm::vec2 coord) {
    coord = (coord+1.0f) * 0.5f;
    return (int) (10 * coord.x) % 2 == (int) (10 * coord.y) % 2 ? 0.5f : 1.0f;
}



	 
	