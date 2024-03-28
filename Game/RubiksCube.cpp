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




void RubiksCube::debugCubeState() {
    std::cout << "Front: " << std::endl;
    this->debugCubeFaceState(FRONT);

    std::cout << "Back: " << std::endl;
    this->debugCubeFaceState(BACK);

    std::cout << "Left: " << std::endl;
    this->debugCubeFaceState(LEFT);

    std::cout << "Right: " << std::endl;
    this->debugCubeFaceState(RIGHT);

    std::cout << "Up: " << std::endl;
    this->debugCubeFaceState(UP);

    std::cout << "Down: " << std::endl;
    this->debugCubeFaceState(DOWN);
}

void RubiksCube::debugCubeFaceState(Face face) {
    auto faceIndices = this->faceToFaceIndices[face];
    for (int i = 0; i < 9; ++i) {
        int cubeId = this->cubeSlots[faceIndices[i]]->id;
        std::cout <<" "<< cubeId << " ";
        if((i+1)%3==0)
            std::cout << "\n";
    }
}

void RubiksCube::rotateFace(Face face, float clockwise,float angle) {
    std::cout << "Face Rotation changes:" << std::endl;
    auto faceIndices = this->faceToFaceIndices[face];
    auto rotationMap = clockwise==-1 ? this->clockwiseFaceRotation : this->counterClockwiseFaceRotation;
    // mapping from cube index to be moved to the cube index of the new location
   std::map<int, int> thisFaceRotationMapping = {};
   for (int i = 0; i < 9; i++) {
       thisFaceRotationMapping[faceIndices[i]] = faceIndices[rotationMap[i]];
   }
    //collecting the Cubes to place them in the position  from the thisFaceRotationMapping
    std::map<Cube *, int> cubeToNewLocation; // the that will be moved
    for (auto &it: thisFaceRotationMapping) {
        auto origin = it.first;
        auto destination = it.second;
        cubeToNewLocation[this->cubeSlots[origin]] = destination;
    }
    // we have all the cube pointers and their new positions
    // now we have to move them
    bool aligned=true;
    float someAngle=cubeToNewLocation.begin()->first->currAngle;
    //take an angle of one of the cubes we're moving, check to see if all the cubes in that face are at the same angle
    for(std::pair<Cube *const, int> cube: cubeToNewLocation){
        if(cube.first->currAngle!=someAngle) {
            aligned = false;
            break;
        }
    }
    if(aligned) {
        for (int i = 0; i < 9; ++i) {
            int cubeId = this->cubeSlots[faceIndices[i]]->id;
            std::cout <<" "<< cubeId << " ";
            if((i+1)%3==0)
                std::cout << "\n";
        }
        for (auto &it: cubeToNewLocation) {
            it.first->currAngle+=angle;
            if(it.first->currAngle>=90) {
                it.first->currAngle-=90;
                this->cubeSlots[it.second] = it.first;
            }
        }
//        for (int i = 0; i < 9; ++i) {
//            int cubeId = this->cubeSlots[faceIndices[i]]->id;
//          //  std::cout << "Cube " << cubeId << " moved to position " << faceIndices[i] << std::endl;
//        std::cout <<" "<< cubeId << " ";
//        if((i+1)%3==0)
//            std::cout << "\n";
//        }
    std::cout << "-->" << std::endl;
    for (int i = 0; i < 9; ++i) {
        int cubeId = this->cubeSlots[faceIndices[i]]->id;
        std::cout <<" "<< cubeId << " ";
        if((i+1)%3==0)
            std::cout << "\n";
    }

        if (faceToRotationAxis[face] == X_AXIS) {
            if(face==LEFT) clockwise*=-1;
            for (int i = 0; i < 9; ++i) {
                this->cubeSlots[faceIndices[i]]->cubeMesh->MySpecialRotate(clockwise * angle,
                                                                           glm::vec3(1.0f, 0.0f, 0.0f), 0);

            }
        } else if (faceToRotationAxis[face] == Y_AXIS) {
            if(face==DOWN) clockwise*=-1;
            for (int i = 0; i < 9; ++i) {
                this->cubeSlots[faceIndices[i]]->cubeMesh->MySpecialRotate(clockwise * angle,
                                                                           glm::vec3(0.0f, 1.0f, 0.0f), 0);
            }
        } else {
            if(face==BACK) clockwise*=-1;
            for (int i = 0; i < 9; ++i) {
                this->cubeSlots[faceIndices[i]]->cubeMesh->MySpecialRotate(clockwise * angle,
                                                                           glm::vec3(0.0f, 0.0f, 1.0f), 0);
            }
        }
    }
}
float RubiksCube::floatMod(Cube* cube,float currAngle,float addedDegrees){
    float newAngle=currAngle+addedDegrees;
    while(newAngle>=90) {
        cube->currAngle-=90;
    }
    return newAngle;
}







