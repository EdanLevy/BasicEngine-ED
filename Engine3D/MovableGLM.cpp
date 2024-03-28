#include "MovableGLM.h"
#include <stdio.h>
#include "../res/includes/glm/gtc/matrix_transform.hpp"
#include "../res/includes/glm/gtc/quaternion.hpp"


static void printMat(const glm::mat4 mat)
{
	printf(" matrix: \n");
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
			printf("%f ", mat[j][i]);
		printf("\n");
	}
}

MovableGLM::MovableGLM()
{
	ZeroTrans();
}

glm::mat4 MovableGLM::MakeTrans(const glm::mat4 &prevTransformations) const
{
	return prevTransformations * MakeTrans();
}

glm::mat4 MovableGLM::MakeTrans() const
{
	return   rot * trans * scl;
	//return   trans * scl * rot;
}

void MovableGLM::MyTranslate(glm::vec3 delta,int mode)
{
	trans = glm::translate(trans,delta);
}
void MovableGLM::MySpecialRotate(float angle, const glm::vec3 &vec, int mode) {
    // prevRot=glm::rotate(rot,angle,vec);
//    rot = glm::rotate(rot, angle, glm::mat3(glm::transpose(prevRot)) * vec);
    rot = glm::rotate(rot, angle, glm::mat3(glm::transpose(prevRot)) * vec);
    prevRot=rot;
}
void  MovableGLM::MyRotate(float angle,const glm::vec3 &vec,int mode)
{
   // prevRot=glm::rotate(rot,angle,vec);
	rot = glm::rotate(rot, angle,glm::mat3(glm::transpose(prevRot)) * vec);
   // prevRot=rot;

}

void  MovableGLM::MyScale(glm::vec3 scale)
{
	scl = glm::scale(scl,scale);
}

void MovableGLM::ZeroTrans()
{

	trans = glm::mat4(1.0);
	rot = glm::mat4(1.0);
	scl = glm::mat4(1.0);
    prevRot=glm::mat4(1.0);
}


