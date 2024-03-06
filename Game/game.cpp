#include "game.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

static void printMat(const glm::mat4 mat)
{
	std::cout << " matrix:" << std::endl;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
			std::cout << mat[j][i] << " ";
		std::cout << std::endl;
	}
}

Game::Game() : Scene()
{
}

Game::Game(float angle, float relationWH, float near1, float far1) : Scene(angle, relationWH, near1, far1)
{
}

void Game::Init()
{

	AddShader("../res/shaders/pickingShader");
	AddShader("../res/shaders/basicShader");

	AddTexture("../res/textures/plane.png",false);

    for (int i = 0; i < 3 ; ++i) { //0 - top , 1 - middle, 2 - bottom
        for (int j = 0; j < 3; ++j) { //0 - left, 1 - middle, 2 - right
            for (int k = 0; k < 3; ++k) { // 0 - back, 1 - middle, 2 - front
                int cubeIndex = i * 9 + j * 3 + k;
                AddShape(Cube, -1, TRIANGLES);
                SetShapeTex(cubeIndex, 0);
                shapes[cubeIndex]->MyScale(glm::vec3(0.5f));
                shapes[cubeIndex]->MyTranslate(glm::vec3(1.0f*j - 1.0f ,1.0f-1.0f*i, -(3.0f - 1.0f*(3-k))), 0);
                shapes[cubeIndex]->MakeTrans();
            }
        }
    }
    
    this->theCube = new RubiksCube(shapes);
    
	pickedShape = 0;

	SetShapeTex(0,0);
	MoveCamera(0, zTranslate, 10);
	pickedShape = -1;

	//ReadPixel(); //uncomment when you are reading from the z-buffer
}

void Game::Update(const glm::mat4& MVP, const glm::mat4& Model, const int  shaderIndx)
{
	Shader* s = shaders[shaderIndx];
	int r = ((pickedShape + 1) & 0x000000FF) >> 0;
	int g = ((pickedShape + 1) & 0x0000FF00) >> 8;
	int b = ((pickedShape + 1) & 0x00FF0000) >> 16;
	s->Bind();
	s->SetUniformMat4f("MVP", MVP);
	s->SetUniformMat4f("Normal", Model);
	s->SetUniform4f("lightDirection", 0.0f, 0.0f, -1.0f, 0.0f);
	if (shaderIndx == 0)
		s->SetUniform4f("lightColor", r / 255.0f, g / 255.0f, b / 255.0f, 1.0f);
	else
		s->SetUniform4f("lightColor", 0.7f, 0.8f, 0.1f, 1.0f);
	s->Unbind();
}

void Game::WhenRotate()
{
}

void Game::WhenTranslate()
{
}

void Game::Motion()
{
	if (isActive)
	{
	}
}

Game::~Game()
{
    delete theCube;
}

void Game::RubiksCubeOperation(Face face) {
    theCube->rotateFace(face, clockwise);
}
