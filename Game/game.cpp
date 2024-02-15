#include "game.h"
//#include "glad/glad.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#define SCREEN_SIZE 800

static void printMat(const glm::mat4 mat)
{
	std::cout<<" matrix:"<<std::endl;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
			std::cout<< mat[j][i]<<" ";
		std::cout<<std::endl;
	}
}

Game::Game() : Scene()
{
}

Game::Game(float angle ,float relationWH, float near1, float far1) : Scene(angle,relationWH,near1,far1)
{ 	
}

void Game::Init()
{		

	AddShader("../res/shaders/pickingShader");	
	AddShader("../res/shaders/basicShader");


    AddShape(Plane,-1,TRIANGLES);
    screen = (unsigned  char *)malloc(SCREEN_SIZE * SCREEN_SIZE * 4);
    int width, height;
    size_t lineSize = SCREEN_SIZE * 4; // elements per line = SCREEN_SIZE * "RGBA"
    for (height = 0; height < SCREEN_SIZE; height++) {
        for (width = 0; width < SCREEN_SIZE; width++) {
            const size_t row = height * lineSize;
            const size_t col = width * 4;
            screen[row + col] = 255;
            screen[row + col + 1] = 255;
            screen[row + col + 2] = 255;
            //screen[row + col + 3] = 255; //NOTE: VALUE IS SET TO 1 IN SHADER, THIS VALUE IS OVERRIDDEN
        }
    }
	pickedShape = 0;
	
	SetShapeTex(0,0);
	MoveCamera(0,zTranslate,10);
	//pickedShape = -1;
	
	//ReadPixel(); //uncomment when you are reading from the z-buffer
}

void Game::Update(const glm::mat4 &MVP,const glm::mat4 &Model,const int  shaderIndx)
{
	Shader *s = shaders[shaderIndx];
	int r = ((pickedShape+1) & 0x000000FF) >>  0;
	int g = ((pickedShape+1) & 0x0000FF00) >>  8;
	int b = ((pickedShape+1) & 0x00FF0000) >> 16;
	s->Bind();
	s->SetUniformMat4f("MVP", MVP);
	s->SetUniformMat4f("Normal",Model);
	s->SetUniform4f("lightDirection", 0.0f , 0.0f, -1.0f, 0.0f);
	if(shaderIndx == 0)
		s->SetUniform4f("lightColor",r/255.0f, g/255.0f, b/255.0f,1.0f);
	else 
		s->SetUniform4f("lightColor",0.7f,0.8f,0.1f,1.0f);
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
	if(isActive)
	{
	}
}

Game::~Game(void)
{
}
