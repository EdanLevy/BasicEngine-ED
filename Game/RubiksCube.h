//
// Created by Edan Levy on 02/03/2024.
//

#ifndef GAME_RUBIKSCUBE_H
#define GAME_RUBIKSCUBE_H

#include "glm/vec3.hpp"
#include "shape.h"
#include <vector>
#include <map>

struct Cube {
    int id;
    Shape *cubeMesh;

    Cube(int id, Shape *cubeMesh) : id(id), cubeMesh(cubeMesh) {}
};

enum Face {
    FRONT = 'F',
    BACK = 'B',
    LEFT = 'L',
    RIGHT = 'R',
    UP = 'U',
    DOWN = 'D'
};
enum RotationAxis {
    X_AXIS,
    Y_AXIS,
    Z_AXIS
};


class RubiksCube {

public:
    explicit RubiksCube(std::vector<Shape *> &cubs);

    std::vector<Cube *> cubeSlots;
    std::map<int, int> clockwiseFaceRotation =
            // maps from an index on the face to the desired index the cube should move to
            {
                    {0, 2},
                    {1, 5},
                    {2, 8},
                    {3, 1},
                    {4, 4},
                    {5, 7},
                    {6, 0},
                    {7, 3},
                    {8, 6}
            };
    std::map<int, int> counterClockwiseFaceRotation =
            {
                    {0, 6},
                    {1, 3},
                    {2, 0},
                    {3, 7},
                    {4, 4},
                    {5, 1},
                    {6, 8},
                    {7, 5},
                    {8, 2}
            };
    std::vector<int> upFaceIndices =
            {24, 25, 26,
             15, 16, 17,
             6, 7, 8};

    std::vector<int> downFaceIndices =
            {18, 19, 20,
             9, 10, 11,
             0, 1, 2};
    std::vector<int> leftFaceIndices =
            {24, 15, 6,
             21, 12, 3,
             18, 9, 0};
    std::vector<int> rightFaceIndices =
            {8, 17, 26,
             5, 14, 23,
             2, 11, 20,
            };
    std::vector<int> frontFaceIndices =
            {6, 7, 8,
             3, 4, 5,
             0, 1, 2,
            };
    std::vector<int> backFaceIndices =
            {24, 25, 26,
             21, 22, 23,
             18, 19, 20,};
    std::map<Face, std::vector<int>> faceToFaceIndices = {
            {UP,    upFaceIndices},
            {DOWN,  downFaceIndices},
            {LEFT,  leftFaceIndices},
            {RIGHT, rightFaceIndices},
            {FRONT, frontFaceIndices},
            {BACK,  backFaceIndices},
    };

    std::map<Face, RotationAxis> faceToRotationAxis{
            {UP,    Y_AXIS},
            {DOWN,  Y_AXIS},
            {LEFT,  X_AXIS},
            {RIGHT, X_AXIS},
            {FRONT, Z_AXIS},
            {BACK,  Z_AXIS},
    };


    ~RubiksCube();

    void rotateFace(Face face, bool clockwise);

};


#endif //GAME_RUBIKSCUBE_H
