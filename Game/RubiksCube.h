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
    Shape* cubeMesh;
    
    Cube(int id, Shape* cubeMesh): id(id), cubeMesh(cubeMesh) {}
};

enum Faces {
    FRONT ='F',
    BACK = 'B',
    LEFT = 'L',
    RIGHT = 'R',
    UP = 'U',
    DOWN = 'D'
};


class RubiksCube {
    
    public:
    explicit RubiksCube(std::vector<Shape*> &cubs);

    std::vector<Cube*> cubeSlots;
    std::map<int, int> clockwiseFaceRotation = 
    // maps from an index on the face to the desired index the cube should move to
            {
                {0,2},{1,5},{2,8},
                {3,1},{4,4},{5,7},
                {6,0},{7,3},{8,6}
            };
    std::map<int, int> counterClockwiseFaceRotation = 
            {
                {0,6},{1,3},{2,0},
                {3,7},{4,4},{5,1},
                {6,8},{7,5},{8,2}
            };
    std::vector<int> topFaceIndices = 
            {0,1,2,
             3,4,5,
             6,7,8};
 
    std::vector<int> middleYAxisFaceIndices = 
            {9 ,10,11,
             12,13,14,
             15,16,17};
    std::vector<int> bottomFaceIndices = 
            {18,19,20,
             21,22,23,
             24,25,26};
    std::vector<int> leftFaceIndices =
            {0,3,6,
             9,12,15,
             18,21,24};
    std::vector<int> middleXAxisFaceIndices =
            { 1, 4, 7,
              10,13,16,
              19,22,25};
    std::vector<int> rightFaceIndices = 
            { 2, 5, 8,
              11,14,17,
              20,23,26
            };
    std::vector<int> frontFaceIndices =
            { 6,7,8,
              15,16,17,
              24,25,26,
            };
    std::vector<int> backFaceIndices =
            { 0, 1, 2,
              9, 10,11,
              18,19,20,
            };
    std::map<Faces, std::vector<int>> faceToFaceIndices = {
            {UP,topFaceIndices},
            { DOWN,bottomFaceIndices},
            { LEFT,leftFaceIndices},
            { RIGHT,rightFaceIndices},
            { FRONT,frontFaceIndices},
            { BACK,backFaceIndices},
    };
    
    
    ~RubiksCube();
    
   void rotateFace(Faces face, bool clockwise);
    
};


#endif //GAME_RUBIKSCUBE_H
