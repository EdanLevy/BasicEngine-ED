#include "InputManager.h"
// #include "../DisplayGLFW/display.h"
#include "game.h"
#include "../res/includes/glm/glm.hpp"
#define SCREEN_SIZE 800
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
	
	scn->Init();
    //scn->Render();
  //  int width, height;
  //  size_t lineSize = SCREEN_SIZE * 4; // elements per line = SCREEN_SIZE * "RGBA"
  //  for (height = 0; height < SCREEN_SIZE; height++) {
  //      for (width = 0; width < SCREEN_SIZE; width++) {
  //          glm::vec2 coord = {-1, -1};
  //          const size_t row = height * lineSize;
  //          const size_t col = width * 4;
  //          // glm::vec2 coord = { (float)width / (float)SCREEN_SIZE, (float)height / (float)SCREEN_SIZE }; // 0->1
  //          //   coord = coord * 2.0f - 1.0f; // 0 -> 1 to -1 -> 1
  //          coord.x += ((2.0f / SCREEN_SIZE) / 2.0f) + (2.0f / SCREEN_SIZE) * height;
  //          coord.y += ((2.0f / SCREEN_SIZE) / 2.0f) + (2.0f / SCREEN_SIZE) * width;
  //          // std::cout << coord.x << "," << coord.y << std::endl;
  //          if((abs(coord.x) <0.5) && (abs(coord.y) <0.5) ) {
  //        //  if (height % 2 == 0 && width % 2 == 0) {
  //              scn->screen[((width + height * SCREEN_SIZE) * 4)] = 0;
  //              scn->screen[((width + height * SCREEN_SIZE) * 4) + 1] = 0;
  //              scn->screen[((width + height * SCREEN_SIZE) * 4) + 2] = 0;
  //              scn->screen[((width + height * SCREEN_SIZE) * 4) + 3] = 0;
  //          } else {
  //              scn->screen[((width + height * SCREEN_SIZE) * 4)] = 255;
  //              scn->screen[((width + height * SCREEN_SIZE) * 4)+1] = 255;
  //              scn->screen[((width + height * SCREEN_SIZE) * 4)+2] = 255;
  //              scn->screen[((width + height * SCREEN_SIZE) * 4)+3] = 255;
  //          }
  //      }
  //      }

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
	return 0;
}
