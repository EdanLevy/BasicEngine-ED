#include "scene.h"
#include <iostream>
#include "glad/include/glad/glad.h"
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

	Scene::Scene()
	{
		//
		glLineWidth(5);
	
		cameras.push_back(new Camera(60.0f,1.0,0.1f,100.0f));		
		pickedShape = -1;
		depth = 0;
		cameraIndx = 0;
		xold = 0;
		yold = 0;
		isActive = false;
	}

	Scene::Scene(float angle,float relationWH,float near1, float far1)
	{
		//glLineWidth(5);
		cameras.push_back(new Camera(angle,relationWH,near1,far1));
		pickedShape = -1;
		depth = 0;
		cameraIndx = 0;
		xold = 0;
		yold = 0;
		isActive = false;
	}

	void Scene::AddShapeFromFile(const std::string& fileName,int parent,unsigned int mode)
	{
		chainParents.push_back(parent);
		shapes.push_back(new Shape(fileName,mode));
	}

	void Scene::AddShape(int type, int parent,unsigned int mode)
	{
		chainParents.push_back(parent);
		shapes.push_back(new Shape(type,mode));
	}

	void Scene::AddShapeCopy(int indx,int parent,unsigned int mode)
	{
		chainParents.push_back(parent);
		shapes.push_back(new Shape(*shapes[indx],mode));
	}

	void Scene::AddShader(const std::string& fileName)
	{
		shaders.push_back(new Shader(fileName));
	}

	void Scene::AddTexture(const std::string& textureFileName,bool for2D)
	{
		textures.push_back(new Texture(textureFileName));
	}

	void Scene::AddTexture(int width,int height, unsigned char *data)
	{
		textures.push_back(new Texture(width,height,data));
	}

	void Scene::AddCamera(glm::vec3& pos , float fov,float relationWH , float zNear, float zFar)
	{
		cameras.push_back(new Camera(fov,relationWH,zNear,zFar));
		cameras.back()->MyTranslate(pos,0);
	}

	void Scene::Draw(int shaderIndx,int cameraIndx,int buffer,bool toClear,bool debugMode)
	{
		glEnable(GL_DEPTH_TEST);
		glm::mat4 Normal = MakeTrans();
	
		glm::mat4 MVP = cameras[cameraIndx]->GetViewProjection()*glm::inverse(cameras[cameraIndx]->MakeTrans());
		int p = pickedShape;
		if(toClear)
		{
			if(shaderIndx>0)
				Clear(1,0,1,1);
			else
				Clear(0,0,0,0);
		}
        //Render();
		for (unsigned int i=0; i<shapes.size();i++)
		{
			if(shapes[i]->Is2Render())
			{
				glm::mat4 Model = Normal * shapes[i]->MakeTrans();

				if(shaderIndx > 0)
				{
					Update(MVP,Model,shapes[i]->GetShader());
					shapes[i]->Draw(shaders,textures,false);
				}
				else
				{ //picking
					Update(MVP,Model,0);
					shapes[i]->Draw(shaders,textures,true);
				}
			}
		}
		pickedShape = p;
	}
	void Scene::MoveCamera(int cameraIndx,int type,float amt)
	{
		switch (type)
		{
			case xTranslate:
				cameras[cameraIndx]->MyTranslate(glm::vec3(amt,0,0),0);
			break;
			case yTranslate:
				cameras[cameraIndx]->MyTranslate(glm::vec3(0,amt,0),0);
			break;
			case zTranslate:
				cameras[cameraIndx]->MyTranslate(glm::vec3(0,0,amt),0);
			break;
			case xRotate:
				cameras[cameraIndx]->MyRotate(amt,glm::vec3(1,0,0),0);
			break;
			case yRotate:
				cameras[cameraIndx]->MyRotate(amt,glm::vec3(0,1,0),0);
			break;
			case zRotate:
				cameras[cameraIndx]->MyRotate(amt,glm::vec3(0,0,1),0);
			break;
			default:
				break;
		}
	}

	void Scene::ShapeTransformation(int type,float amt)
	{
		if(glm::abs(amt)>1e-5)
		{
			switch (type)
			{
			case xTranslate:
				shapes[pickedShape]->MyTranslate(glm::vec3(amt,0,0),0);
			break;
			case yTranslate:
				shapes[pickedShape]->MyTranslate(glm::vec3(0,amt,0),0);
			break;
			case zTranslate:
				shapes[pickedShape]->MyTranslate(glm::vec3(0,0,amt),0);
			break;
			case xRotate:
				shapes[pickedShape]->MyRotate(amt,glm::vec3(1,0,0),0);
			break;
			case yRotate:
				shapes[pickedShape]->MyRotate(amt,glm::vec3(0,1,0),0);
			break;
			case zRotate:
				shapes[pickedShape]->MyRotate(amt,glm::vec3(0,0,1),0);
			break;
			default:
				break;
			}
		}

	}

	void Scene::Resize(int width,int height)
	{
	
		cameras[0]->SetProjection(cameras[0]->GetAngle(),(float)width/height);
		glViewport(0,0,width,height);
		std::cout<<cameras[0]->GetRelationWH()<<std::endl;
	}

	float Scene::Picking(int x,int y)
	{
		
		
		return 0;
	}
	//return coordinates in global system for a tip of arm position is local system 
	void Scene::MouseProccessing(int button)
	{
		if(pickedShape == -1)
		{
			if(button == 1 )
			{				

				MyTranslate(glm::vec3(-xrel/20.0f,0,0),0);
				MyTranslate(glm::vec3(0,yrel/20.0f,0),0);
				WhenTranslate();
			}
			else
			{
				MyRotate(xrel/2.0f,glm::vec3(1,0,0),0);
				MyRotate(yrel/2.0f,glm::vec3(0,0,1),0);
				WhenRotate();
			}
		}
	}

	void Scene::ZeroShapesTrans()
	{
		for (unsigned int i = 0; i < shapes.size(); i++)
		{
			shapes[i]->ZeroTrans();
		}
	}

	void Scene::ReadPixel()
	{
		glReadPixels(1,1,1,1,GL_DEPTH_COMPONENT,GL_FLOAT,&depth);
	}

	void Scene::UpdatePosition(float xpos, float ypos)
	{
		xrel = xold - xpos;
		yrel = yold - ypos;
		xold = xpos;
		yold = ypos;
	}

	void Scene::HideShape(int shpIndx)
	{
			shapes[shpIndx]->Hide();
	}

	void Scene::UnhideShape(int shpIndx)
	{
			
			shapes[shpIndx]->Unhide();
	}

	void Scene::Clear(float r, float g, float b, float a)
	{
		glClearColor(r, g, b, a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	Scene::~Scene(void)
{
	for (Shape* shp : shapes)
		{
			delete shp;
		}
	for (Camera* cam : cameras)
		{
			delete cam;
		}
	for(Shader* sdr: shaders)
		{
			delete sdr;
		}
	for(Texture* tex: textures)
		{
			delete tex;
		}

}
/**
 * loops over the screen and for each pixal, renders it according to the parsed scene in ps
 * @param widthSize screen width
 * @param heightSize screen hight
 * @param ps parsed scene
 */
void Scene::Render(int widthSize, int  heightSize, const ParsedScene& ps)
{
    int width, height;
    for (height = 0; height < heightSize; height++) {
        for (width = 0; width < widthSize; width++) {
            glm::vec2 coord= {-1,-1};
            coord.x+=((2.0f/widthSize)/2.0f)+(2.0f/widthSize)*height;
            coord.y+=((2.0f/heightSize)/2.0f)+(2.0f/heightSize)*width;
            glm::vec3 pixelVal= PerPixel(coord, ps);
            screen[((height +width*widthSize)*4)] = (int)(pixelVal.r*255);
            screen[((height +width*widthSize)*4)+1] = (int)(pixelVal.g*255);
            screen[((height +width*widthSize)*4)+2] = (int)(pixelVal.b*255);
            }
        }
}

/**
 * given a pixel vector in range (-1,-1) to (1,1) on the screen, colors it according to the scene parsed in ps
 * @param coord pixel on the screen
 * @param ps parsed scene data
 * @return vector for rgb color in range 0-1 (need to multiply it by 255 for real colors)
 */
glm::vec3 Scene::PerPixel(const glm::vec2& coord, const ParsedScene& ps)
{
    glm::vec3 pixelVal={0.0f,0.0f,0.0f}; //rgb values, returned object. defaults to black

    //glm::vec3 cameraCoord= {0.0f,0.0f,2.0f};
    glm::vec3 cameraCoord= ps.cameraCoord;
    for (const auto & sphere : ps.spheres) {
        //glm::vec3 sphereCenter={0.0f,0.0f,0.0f};
        glm::vec3 sphereCenter=sphere.coord;
        float sphereRadius=sphere.radius;
        //  glm::vec3 rayOrigin(0.0f, 0.0f, 0.0f);
        // Convert pixel coordinates to normalized device coordinates (NDC)
        glm::vec3 rayDirection(coord.x - cameraCoord.x, coord.y -cameraCoord.y, 0.0f - cameraCoord.z);
        //      rayDirection = glm::normalize(rayDirection);
        // Calculate coefficients for the quadratic equation
        float a = glm::dot(rayDirection, rayDirection);
        float b = 2.0f * glm::dot(cameraCoord+sphereCenter, rayDirection);
        float c = glm::dot(cameraCoord+sphereCenter, cameraCoord+sphereCenter) - (sphereRadius * sphereRadius);

        // Calculate the discriminant
        float discriminant = b * b - 4.0f * a * c;
        // Check for intersection
        if (discriminant >= 0) {
            pixelVal+=ps.ambientLight; //TODO: IN FUTURE ASSURE THIS IS ONLY APPLIED ONCE
            float nearestHit= (-b - glm::sqrt(discriminant)) / (2.0f*a); //solve quadratic equation, since a is positive, -b -sqrt will always be smaller
            glm::vec3 hitPoint= cameraCoord - sphereCenter + rayDirection* nearestHit;
            glm::vec3 normal= glm::normalize(hitPoint);
            for(const auto & light : ps.directionalLights) {
                glm::vec3 lightDir = glm::normalize(light - sphereCenter);
                float shade = glm::max(glm::dot(normal, lightDir), 0.0f);
                pixelVal+= sphere.color * shade;
            }
        }
    }
    return pixelVal;
}
/*
 *  scalar for coord in calculation determines checkers size, bigger scalar smaller checkers
 */
float Scene::isCheckersDiffusionDistribution( glm::vec3 coord){
    coord = coord * 0.5f +1.0f;
    return (int)(10* coord.x) % 2 == (int)( 10* coord.y) % 2 ? 0.5f : 1.5f;
}



	 
	