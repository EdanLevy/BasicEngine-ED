//
// Created by Edan Levy on 02/03/2024.
//

#include "RubiksCube.h"
#include <iostream>

RubiksCube::RubiksCube(std::vector<Shape *> &cubs) {
    this->cubeSlots = std::vector<Cube *>();
    this->cubeSlots.reserve(26);
    for (int i = 0; i < cubs.size(); i++) {
        this->cubeSlots.emplace_back(new Cube(i, cubs[i]));
    }
}

RubiksCube::~RubiksCube() {
    this->cubeSlots.clear();
}

void RubiksCube::rotateFace(Faces face, bool clockwise) {
    auto faceIndices = this->faceToFaceIndices[face];
    auto rotationMap = clockwise ? this->clockwiseFaceRotation : this->counterClockwiseFaceRotation;

    
    // mapping from cube index to be moved to the cube index of the new location
    std::map<int, int> thisFaceRotationMapping = {}; 
    for (int i = 0; i < 9; i++) {
        thisFaceRotationMapping[faceIndices[i]] = faceIndices[rotationMap[i]];
    }

    //collecting the Cubes to place them in the position  from the thisFaceRotationMapping
    std::map<Cube *, int> cubeToNewLocation; // the that will be moved
    for (auto &it: thisFaceRotationMapping) {
        auto key = it.first;
        auto value = it.second;
        cubeToNewLocation[this->cubeSlots[key]] = value;
    }
    // we have all the cube pointers and their new positions
    // now we have to move them
    for (auto &it: cubeToNewLocation) {
        this->cubeSlots[it.second] = it.first;
    }

    for (int i = 0; i < 9; ++i) {
        int cubeId = this->cubeSlots[faceIndices[i]]->id;
        std::cout << "Cube " << cubeId << " moved to position " << faceIndices[i] << std::endl;
        
    }
}



