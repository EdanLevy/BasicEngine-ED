#include "scene.h"
#include <iostream>
#include <fstream>
#include "glad/include/glad/glad.h"

#define PI 3.14159265358979323846
#define LOW_THRESHOLD 0.3
#define HIGH_THRESHOLD 0.7
#define IMAGE_SIZE 256
#define WHITE_PIXEL 255
#define BLACK_PIXEL 0

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

void Scene::CustomDraw(int shaderIndx, int cameraIndx, int buffer, bool toClear, bool debugMode, int screenNum,
                       int shapeIndex) {
    glEnable(GL_DEPTH_TEST);
    glm::mat4 Normal = MakeTrans();

    glm::mat4 MVP = cameras[cameraIndx]->GetViewProjection() * glm::inverse(cameras[cameraIndx]->MakeTrans());
    int p = pickedShape;
    if (toClear) {
        if (shaderIndx > 0)
            Clear(1, 1, 1, 1);
        else
            Clear(0, 0, 0, 0);
    }
    if (screenNum == 0) {
        glViewport(0, IMAGE_SIZE, IMAGE_SIZE, IMAGE_SIZE);
    }
    if (screenNum == 1) {
        glViewport(IMAGE_SIZE, IMAGE_SIZE, IMAGE_SIZE, IMAGE_SIZE);
        ApplyEdgeFilter(screenNum);
    }
    if (screenNum == 2) {
        glViewport(0, 0, IMAGE_SIZE, IMAGE_SIZE);
    }
    if (screenNum == 3) {
        glViewport(IMAGE_SIZE, 0, IMAGE_SIZE, IMAGE_SIZE);
        FloydSteinbergFilter(screenNum);
    }
    if (shapes[shapeIndex]->Is2Render()) {
        glm::mat4 Model = Normal * shapes[shapeIndex]->MakeTrans();

        if (shaderIndx > 0) {
            Update(MVP, Model, shapes[shapeIndex]->GetShader());
            shapes[shapeIndex]->Draw(shaders, textures, false);
        } else { //picking
            Update(MVP, Model, 0);
            shapes[shapeIndex]->Draw(shaders, textures, true);
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

void Scene::ApplyEdgeFilter(int screenNum) {
    Texture *tex = textures[shapes[screenNum + 1]->GetTexture()];
    tex->Bind(tex->GetSlot());
    GLubyte *pixels = new GLubyte[IMAGE_SIZE * IMAGE_SIZE * 4];
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
    int x, y; // line and column
    size_t lineSize = IMAGE_SIZE * 4; // elements per line = IMAGE_SIZE * "RGBA"
    auto *gradientMagnitude = new double[IMAGE_SIZE * IMAGE_SIZE]; //single value for each pixel
    auto *gradientDirection = new double[IMAGE_SIZE * IMAGE_SIZE]; //single value for each pixel
    CalculateGradientSobel(pixels, gradientMagnitude, gradientDirection);
    const double pi = 3.14159265358979323846;
    for (y = 1; y < IMAGE_SIZE - 1; y++) {
        for (x = 1; x < IMAGE_SIZE - 1; x++) {
            const size_t row = y * lineSize;
            const size_t col = x * 4;

            ApplyGaussOnPixel(x, y, pixels);
            size_t gradRow = y * IMAGE_SIZE;
            size_t gradCol = x;
            if (gradientDirection[gradRow + gradCol] < PI / 4) { //case: angle= 0
                if (gradientMagnitude[gradCol + gradRow] > gradientMagnitude[gradCol + gradRow + 1] &&
                    gradientMagnitude[gradCol + gradRow] > gradientMagnitude[gradCol + gradRow - 1])
                    Edge(row, col, pixels);
                else NonEdge(row, col, pixels);
            } else if (gradientDirection[gradRow + gradCol] < (PI / 2)) { //angle =4
                if (gradientMagnitude[gradCol + gradRow] > gradientMagnitude[gradRow - IMAGE_SIZE + gradCol + 1] &&
                    gradientMagnitude[gradCol + gradRow] > gradientMagnitude[gradRow + IMAGE_SIZE + gradCol - 1])
                    Edge(row, col, pixels);
                else NonEdge(row, col, pixels);
            } else if (gradientDirection[gradRow + gradCol] < (3 * PI) / 4) { //angle =90
                if (gradientMagnitude[gradCol + gradRow] > gradientMagnitude[gradCol + gradRow + IMAGE_SIZE] &&
                    gradientMagnitude[gradCol + gradRow] > gradientMagnitude[gradCol + gradRow - IMAGE_SIZE])
                    Edge(row, col, pixels);
                else NonEdge(row, col, pixels);
            } else { //angle= 135
                if (gradientMagnitude[gradCol + gradRow] > gradientMagnitude[gradRow + IMAGE_SIZE + gradCol + 1] &&
                    gradientMagnitude[gradCol + gradRow] > gradientMagnitude[gradRow - IMAGE_SIZE + gradCol - 1])
                    Edge(row, col, pixels);
                else NonEdge(row, col, pixels);
            }
            if (pixels[row + col] == WHITE_PIXEL && gradientMagnitude[gradCol + gradRow] < 0.7 * WHITE_PIXEL) {
                NonEdge(row, col, pixels);
            } else if (pixels[row + col] == WHITE_PIXEL && gradientMagnitude[gradCol + gradRow] > 0.3 * WHITE_PIXEL) {
                Edge(row, col, pixels);
            }
        }
    }
    delete[] gradientMagnitude;
    delete[] gradientDirection;
    PrintToFile("img4.txt",pixels, false);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, IMAGE_SIZE, IMAGE_SIZE, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
}

void Scene::NonEdge(size_t row, size_t col, unsigned char *pixels) {
    pixels[row + col] = BLACK_PIXEL;
    pixels[row + col + 1] = BLACK_PIXEL;
    pixels[row + col + 2] = BLACK_PIXEL;
    pixels[row + col + 3] = WHITE_PIXEL;
}

void Scene::Edge(size_t row, size_t col, unsigned char *pixels) {
    pixels[row + col] = WHITE_PIXEL;
    pixels[row + col + 1] = WHITE_PIXEL;
    pixels[row + col + 2] = WHITE_PIXEL;
    pixels[row + col + 3] = WHITE_PIXEL;

}

void Scene::ApplyGaussOnPixel(size_t Xcoo, size_t Ycoo, unsigned char *pixels) {
    const size_t lineSize = IMAGE_SIZE * 4; // elements per line = IMAGE_SIZE * "RGBA"
    const size_t row = Ycoo * lineSize;
    const size_t col = Xcoo * 4;
    GLfloat r, g, b;
    r = pixels[row + col];
    g = pixels[row + col + 1];
    b = pixels[row + col + 2];
    float rCon = 0.0, gCon = 0.0, bCon = 0.0;
    for (int x = -1; x < 2; x++) {
        for (int y = -1; y < 2; y++) {
            size_t neighborRow = row + x * lineSize;
            size_t neighborCol = col + y * 4;
            //   if (neighborRow >= 0 && neighborRow < IMAGE_SIZE * lineSize &&
            //       neighborCol >= 0 && neighborCol < IMAGE_SIZE * 4) {
            rCon += pixels[neighborRow + neighborCol] * GaussianKernel[x + 1][y + 1];
            gCon += pixels[neighborRow + neighborCol + 1] * GaussianKernel[x + 1][y + 1];
            bCon += pixels[neighborRow + neighborCol + 2] * GaussianKernel[x + 1][y + 1];
            //    }
        }
    }
    pixels[row + col] = rCon;
    pixels[row + col + 1] = gCon;
    pixels[row + col + 2] = bCon;

}

void Scene::CalculateGradientSobel(unsigned char *pixels, double *gradientMagnitude, double *gradientDirection) {
    size_t x, y; // line and column of the pixel
    size_t LineSize = IMAGE_SIZE * 4; // elements per line = IMAGE_SIZE * "RGBA"
    for (y = 1; y < IMAGE_SIZE - 1; y++) {
        for (x = 1; x < IMAGE_SIZE - 1; x++) {
            const size_t row = y * LineSize;
            const size_t col = x * 4;
            int sobelX = 0, sobelY = 0;
            for (int a = -1; a < 2; a++) {
                for (int b = -1; b < 2; b++) {
                    size_t neighborRow = row + a * LineSize;
                    size_t neighborCol = col + b * 4;
                    //using R values, since imagine is gray scaled
                    sobelX += pixels[neighborRow + neighborCol] * SobelHorizontalKernel[a + 1][b + 1];
                    sobelY += pixels[neighborRow + neighborCol] * SobelVerticalKernel[a + 1][b + 1];
                }
            }
            gradientMagnitude[y * IMAGE_SIZE + x] = std::sqrt(sobelX * sobelX + sobelY * sobelY);
            gradientDirection[y * IMAGE_SIZE + x] = std::atan2(sobelY, sobelX);
            // double j, k;
            // k = gradientMagnitude[row + col];
            // j = gradientDirection[row + col];
        }
    }
}

void Scene::FloydSteinbergFilter(int screenNum) {
    Texture *tex = textures[shapes[screenNum + 1]->GetTexture()];
    tex->Bind(tex->GetSlot());
    GLubyte *pixels = new GLubyte[IMAGE_SIZE * IMAGE_SIZE * 4];
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
    int x, y; // line and column
    size_t LineSize = IMAGE_SIZE * 4; // elements per line = IMAGE_SIZE * "RGBA"
    for (y = 0; y < IMAGE_SIZE - 1; y++) {
        for (x = 1; x < IMAGE_SIZE - 1; x++) {
            const size_t row = y * LineSize;
            const size_t col = x * 4;
            auto oldVal=pixels[row+col];
            int rounderVal=(oldVal/16) *16;
            auto error=oldVal-rounderVal;
            ChangePixel(pixels, row+col, rounderVal);
            ChangePixel(pixels,pixels[col+row + 4           ],pixels[col+row + 4           ] + error * 7 / 16);
            ChangePixel(pixels,pixels[col+row - 4 + LineSize],pixels[col+row - 4 + LineSize] + error * 3 / 16);
            ChangePixel(pixels,pixels[col+row     + LineSize],pixels[col+row     + LineSize] + error * 5 / 16);
            ChangePixel(pixels,pixels[col+row + 4 + LineSize],pixels[col+row + 4 + LineSize] + error * 1 / 16);
        }
    }
    PrintToFile("img6.txt",pixels, true);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, IMAGE_SIZE, IMAGE_SIZE, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

}

void Scene::ChangePixel(GLubyte *pixels, size_t index, int Val) const {
    pixels[index]= Val;
    pixels[index+1]= Val;
    pixels[index+2]= Val;
}
void Scene::PrintToFile(std::string fileName, unsigned char* buffer, bool isGrayScale){

    std::ofstream outfile(fileName);

    if (outfile.is_open()) {
        size_t arraySize = 256*256*4;
        for (size_t i = 0; i < arraySize; i+=4) {
            if (!isGrayScale) {
                outfile << static_cast<int>(buffer[i] / 255); // Convert to int to print numeric value
            } else {
                outfile << static_cast<int>(buffer[i] / 15); // Convert to int to print numeric value
            }
            if (i < arraySize - 1) {
                outfile << ","; // Add comma for all elements except the last one
            }
        }
        outfile.close();

        std::cout << "String successfully written to " << fileName << std::endl;
    } else {
        std::cerr << "Error opening the file: " << fileName << std::endl;
    }

};




