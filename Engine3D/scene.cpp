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
            glm::vec3 pixelVal = TraceRay(ray, ps, 0);
            pixelVal = glm::clamp(pixelVal, glm::vec3(0.0f), glm::vec3(1.0f));
            screen[((height + width * widthSize) * 4)] = (int) (pixelVal.r * 255);
            screen[((height + width * widthSize) * 4) + 1] = (int) (pixelVal.g * 255);
            screen[((height + width * widthSize) * 4) + 2] = (int) (pixelVal.b * 255);
        }
    }
}
glm::vec3 Scene::TraceRay(const Ray &ray, const ParsedScene &ps, int recDepth) {
   if(recDepth >5)
       return glm::vec3 (0.0f);
    glm::vec3 pixelColor= glm::vec3 (0.0f);
    struct SceneObject* nearestObject= nullptr;
    float nearestObjectDist=std::numeric_limits<float>::max(); //any distance always is smaller than this value
    glm::vec3 normal= glm::vec3 (0.0f);
    glm::vec3 hitPoint= glm::vec3 (0.0f);
    for(SceneObject* obj: ps.sceneObjects){
        float intersection = obj->Intersect(ray);
       // if(intersection < 0) { continue;}
        if( intersection>=0 && intersection < nearestObjectDist ){
            hitPoint = ray.origin+intersection*ray.direction;
            nearestObject=obj;
            nearestObjectDist=intersection;
            normal=obj->getNormal(hitPoint);
        }

    }
    if (nearestObject == nullptr){
        return glm::vec3 (0.0f);
    }
    if(nearestObject->type=='r'){
        if(recDepth>=5)
            return glm::vec3(0.0f);
        glm::vec3 refNormal = glm::normalize(nearestObject->getNormal(hitPoint));
        glm::vec3 refIn = glm::normalize(-ray.direction);
        glm::vec3 refOut = glm::normalize(2.0f * glm::dot(refNormal, refIn) * refNormal - refIn);
        //glm::vec3 refOut = glm::reflect(refIn,refNormal);
        Ray newRay;
        newRay.origin=hitPoint;
        newRay.direction=refOut;
        return TraceRay(newRay,ps,recDepth+1);
    }
    glm::vec4 color= nearestObject->getColor(glm::vec2(hitPoint));
    pixelColor+= glm::vec3(ps.ambientLightColor * color);
    for (const auto &light: ps.lights) {
        bool isBLocked=false;
        for (SceneObject *obj: ps.sceneObjects) {
            Ray lightRay;
            lightRay.origin=hitPoint;
            lightRay.direction=-light.direction;
            if (obj->Intersect(lightRay)>0 && obj != nearestObject){
                isBLocked = true;
                break;
            }
        }
       // if(true){
        if(!isBLocked) {
            glm::vec3 normalVec = glm::normalize(normal);
            glm::vec3 lightDir = glm::normalize(-light.direction);
            float diffuseDot = glm::max(glm::dot(lightDir, normalVec), 0.0f);
            glm::vec3 diffuse = diffuseDot * glm::vec3(color) * glm::vec3(light.intensity);
           glm::vec3 reflectionDir=glm::reflect(-lightDir,normalVec);
            glm::vec3 CameraRay = glm::normalize(ray.origin - hitPoint);
            float specularDot = glm::max(glm::dot(reflectionDir, CameraRay), 0.0f); // Ensure non-negative
            float coEfficient = 0.7;
            glm::vec3 specular = glm::vec3(coEfficient * std::pow(specularDot, color.a) * light.intensity);
            pixelColor += diffuse + specular;
        }
    }

    for (const auto &spotlight: ps.spotlights) {
        glm::vec3 lightDir = glm::normalize(hitPoint - glm::vec3(spotlight.position));
        double hitAngle = (glm::dot(spotlight.direction, lightDir)/(glm::length(spotlight.direction)*glm::length(lightDir)));
        if (hitAngle >= spotlight.position.a) {
            bool isBlocked = false;
            for (SceneObject *obj: ps.sceneObjects) {
                Ray lightRay;
                lightRay.origin = glm::vec3(spotlight.position);
                lightRay.direction = lightDir;
                if (obj->Intersect(lightRay)>0 && nearestObject->Intersect(lightRay) > obj->Intersect(lightRay)){
                    isBlocked = true;
                    break;
                }
        }
          //  if(true){
            if (!isBlocked) {
                glm::vec3 lightDir = glm::normalize(-spotlight.direction);
                glm::vec3 normalVec = glm::normalize(normal);
                float diffuseDot = glm::max(glm::dot(lightDir, normalVec), 0.0f);
                glm::vec3 diffuse = diffuseDot * glm::vec3(color) * glm::vec3(spotlight.intensity);
                glm::vec3 reflectionDir = glm::normalize(2.0f * glm::dot(normalVec, lightDir) * normalVec - lightDir);
                glm::vec3 cameraRay = glm::normalize(ray.origin - hitPoint);
                float specularDot = glm::max(glm::dot(reflectionDir, cameraRay), 0.0f); // Ensure non-negative
                float coEfficient = 0.7;
                glm::vec3 specular = glm::vec3(
                        coEfficient * std::pow(specularDot, color.a) *
                        spotlight.intensity);
                pixelColor += diffuse + specular;
            }
        }
    }
    return pixelColor;
}

/*
 *  scalar for coord in calculation determines checkers size, bigger scalar smaller checkers
 */
float Scene::isCheckersDiffusionDistribution(glm::vec2 coord) {
    coord = (coord+1.0f) * 0.5f;
    //coord = glm::normalize(coord);
    //return (int) (10 * coord.x) % 2 == (int) (10 * coord.y) % 2 ? 0.5f : 1.0f;
    return (int(50*coord.x)%2) == (int(50*coord.y) % 2)? 0.5f : 1.0f;
   // return (1.5*coord.x < 0.5f) ^ (1.5*coord.y < 0.5f) ? 0.5f : 1.0f;
}



	 
	