#pragma once

#include "scene.h"
#include "RubiksCube.h"

class Game : public Scene {
public:

    Game();

    Game(float angle, float relationWH, float near, float far);

    void Init();

    void Update(const glm::mat4 &MVP, const glm::mat4 &Model, const int shaderIndx);

    void ControlPointUpdate();

    void WhenRotate();

    void WhenTranslate();

    void Motion();

    void RubiksCubeOperation(Face face);

    void debugCube();

    void switchCubeRotationDirection() { clockwise = -clockwise; };
    void doubleCubeRotationAngle() {
        if(angle<180)
            angle*=2;
    }
    void halveCubeRotationAngle() {
        if(angle>45)
            angle/=2;
    }

    ~Game(void);

private:
    RubiksCube *theCube;
    float angle= 90.0f;
    int clockwise = -1.0f;
};

