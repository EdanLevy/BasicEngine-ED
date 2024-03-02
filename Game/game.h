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

    void RubiksCubeOperation(Faces face);

    void switchCubeRotationDirection() { clockwise = !clockwise; };

    ~Game(void);

private:
    RubiksCube *theCube;
    bool clockwise = true;
};

