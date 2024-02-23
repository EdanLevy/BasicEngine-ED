#include "InputManager.h"
// #include "../DisplayGLFW/display.h"
#include "game.h"
#include "../res/includes/glm/glm.hpp"
#define SCREEN_SIZE 512
int main(int argc,char *argv[])
{
	const int DISPLAY_WIDTH = SCREEN_SIZE;
	const int DISPLAY_HEIGHT = SCREEN_SIZE;
	const float CAMERA_ANGLE = 0.0f; // this needs
	const float NEAR = 1.0f;
	const float FAR = 100.0f;

	Game *scn = new Game(CAMERA_ANGLE,(float)DISPLAY_WIDTH/DISPLAY_HEIGHT,NEAR,FAR);
	
	Display display(DISPLAY_WIDTH, DISPLAY_HEIGHT, "OpenGL");
	
	Init(display);
    ParsedScene* ps = SceneParser("../res/scenes/scene1.txt").parse();
//    ps.AddSphere( Sphere(0.5f,glm::vec3 {-0.7f,-0.7f,1.0f},glm::vec3 {0.0f,1.0f,1.0f} ));
//    ps.AddSphere( Sphere(0.5f,glm::vec3 {0.6f,-0.5f,1.3f},glm::vec3 {1.0f,0.0f,0.0f} ));
//   // ps.AddSphere( Sphere(0.5f,glm::vec3 {0.0f,0.9f,4.0f},glm::vec3 {1.0f,0.0f,0.0f} ));
//    //ps.AddDirectionalLight(glm::vec3 {0.5f,0.0f,1.0f});
//    ps.AddDirectionalLight(glm::vec3 {-1.0f,1.0f,1.0f});
//   // ps.ambientLight=glm::vec3 {0.1f,0.2f,0.3f};
//    ps.cameraCoord=glm::vec3 {0.0f,0.0f,2.0f};


	scn->Init();
    scn->Render(SCREEN_SIZE, SCREEN_SIZE, *ps);
    display.SetScene(scn);
    //scn->AddTexture("../res/textures/plane.png", false);

    scn->AddTexture(SCREEN_SIZE, SCREEN_SIZE, scn->screen);
    //  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, SCREEN_SIZE, SCREEN_SIZE, 0, GL_RGBA, GL_UNSIGNED_BYTE, screen);
    scn->Draw(1,0,scn->BACK,true,false);

    //scn->Draw(1,0,scn->BACK,true,false);
    scn->Motion();
    display.SwapBuffers();
    while(!display.CloseWindow())
	{
		display.PollEvents();
	}
	delete scn;
    delete ps;
	return 0;
}
