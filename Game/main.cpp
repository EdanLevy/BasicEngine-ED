#include "InputManager.h"
// #include "../DisplayGLFW/display.h"
#include "game.h"
#include "../res/includes/glm/glm.hpp"

int main(int argc, char* argv[])
{
	const int DISPLAY_WIDTH = 512;
	const int DISPLAY_HEIGHT = 512;
	const float CAMERA_ANGLE = 0.0f;
	const float NEAR = 1.0f;
	const float FAR = 100.0f;

	Game* scn = new Game(CAMERA_ANGLE, (float)DISPLAY_WIDTH / DISPLAY_HEIGHT, NEAR, FAR);

	Display display(DISPLAY_WIDTH, DISPLAY_HEIGHT, "OpenGL");

	Init(display);

	scn->Init();

	display.SetScene(scn);
    scn->AddShape(Game::Plane,1,GL_TRIANGLES);
    scn->AddShape(Game::Plane,1,GL_TRIANGLES);
    scn->AddShape(Game::Plane,1,GL_TRIANGLES);
    scn->AddShape(Game::Plane,1,GL_TRIANGLES);

	scn->AddTexture("../res/textures/lena256.jpg", false);
	scn->SetShapeTex(1, 0);
    scn->SetShapeShader(1,2);
	scn->CustomDraw(1, 0, scn->BACK, true, false,0,1);

    scn->AddTexture("../res/textures/lena256.jpg", false);
    scn->SetShapeTex(2, 1);
    scn->CustomDraw(2, 0, scn->BACK, false, false, 1,2);

    scn->AddTexture("../res/textures/lena256.jpg", false);
    scn->SetShapeTex(3, 2);
    scn->CustomDraw(1, 0, scn->BACK, false, false, 2,3);

    scn->AddTexture("../res/textures/lena256.jpg", false);
    scn->SetShapeTex(4, 3);
    scn->CustomDraw(1, 0, scn->BACK, false, false, 3,4);

    scn->Motion();
	display.SwapBuffers();
	while (!display.CloseWindow())
	{
		//scn->Draw(1,0,scn->BACK,true,false);
		//scn->Motion();
		//display.SwapBuffers();
		display.PollEvents();

	}
	delete scn;
	return 0;
}
