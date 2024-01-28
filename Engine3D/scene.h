#pragma once
#include "shader.h"
#include "shape.h"
#include "camera.h"
#include "VertexArray.hpp"
#include <vector>

int getPixelIntencity(const unsigned char pixelValue);

class Scene : public MovableGLM
{

public:
	enum axis { xAxis, yAxis, zAxis };
	enum transformations { xTranslate, yTranslate, zTranslate, xRotate, yRotate, zRotate, xScale, yScale, zScale, xCameraTranslate, yCameraTranslate, zCameraTranslate };
	enum modes { POINTS, LINES, LINE_LOOP, LINE_STRIP, TRIANGLES, TRIANGLE_STRIP, TRIANGLE_FAN, QUADS };
	enum buffers { COLOR, DEPTH, STENCIL, BACK, FRONT, ACCUM, NONE };
	enum shapes { Axis, Plane, Cube, Octahedron, Tethrahedron, LineCopy, MeshCopy };

	Scene();
	Scene(float angle, float relationWH, float near, float far);

	void AddShapeFromFile(const std::string& fileName, int parent, unsigned int mode);
	virtual void AddShape(int type, int parent, unsigned int mode);
	void AddShapeCopy(int indx, int parent, unsigned int mode);

	void AddShader(const std::string& fileName);
	void AddTexture(const std::string& textureFileName, bool for2D);
	void AddTexture(int width, int height, unsigned char* data);

	void AddCamera(glm::vec3& pos, float fov, float relationWH, float zNear, float zFar);

	void ZeroShapesTrans();

	virtual void Update(const glm::mat4& MVP, const glm::mat4& Normal, const int  shaderIndx) = 0;
	virtual void WhenTranslate() {};
	virtual void WhenRotate() {};
	virtual void Motion() {};
	virtual void Draw(int shaderIndx, int cameraIndx, int buffer, bool toClear, bool debugMode);
	virtual void CustomDraw(int shaderIndx, int cameraIndx, int buffer, bool toClear, bool debugMode, int screenNum,int shapeIndex);

	virtual ~Scene(void);

	void MoveCamera(int cameraIndx, int type, float amt);
	void ShapeTransformation(int type, float amt);

	float Picking(int x, int y);
	void Resize(int width, int hight);

	inline void SetParent(int indx, int newValue) { chainParents[indx] = newValue; }
	inline float GetNear(int cameraIndx) { return cameras[cameraIndx]->GetNear(); }
	inline float GetFar(int cameraIndx) { return cameras[cameraIndx]->GetFar(); }

	void ReadPixel();
    void ApplyEdgeFilter(int screenNum);
    void ApplyGaussOnPixel(size_t Xcoo, size_t Ycoo,  unsigned char *pixels);
    void CalculateGradientSobel(const unsigned char *pixels, double *gradientMagnitude, double *gradientDirection);

	inline float GetAngle(int cameraIndx) { return cameras[cameraIndx]->GetAngle(); }
	inline void Activate() { isActive = true; }
	inline void Deactivate() { isActive = false; }
	void HideShape(int shpIndx);
	void UnhideShape(int shpIndx);

	void UpdatePosition(float xpos, float ypos);
	void MouseProccessing(int button);
	bool inline IsActive() const { return isActive; }

	inline void SetShapeTex(int shpIndx, int texIndx) { shapes[shpIndx]->SetTexture(texIndx); }
	inline void SetShapeShader(int shpIndx, int shdrIndx) { shapes[shpIndx]->SetShader(shdrIndx); }

private:
    const float GaussianKernel[3][3]={ {1.0/ 16.0, 2.0 / 16.0, 1.0 / 16.0},
                         {2.0 / 16.0, 4.0 / 16.0, 2.0 / 16.0},
                         {1.0 / 16.0, 2.0 / 16.0, 1.0/ 16.0}};
    const int SobelHorizontalKernel[3][3]={ {-1,0,1},
                                       {-2,0,2},
                                       {-1,0,1}};
    const int SobelVerticalKernel[3][3]={ {1,2,1},
                                       {0,0,0},
                                       {-1,-2,-1}};
	std::vector<Camera*> cameras;

	float depth;
	int xold, yold, xrel, yrel;
	int cameraIndx;
	void Clear(float r, float g, float b, float a);

protected:
	std::vector<Shape*> shapes;
	std::vector<Shader*> shaders;
	std::vector<int> chainParents;
	std::vector<Texture*> textures;

	int pickedShape;

	bool isActive;

    static void NonEdge(size_t row, size_t col, unsigned char *pixels);

    static void Edge(size_t row, size_t col, unsigned char *pixels);

    void FloydSteinbergFilter(int screenNum);
    
    void HalftoningFilter(int screenNum);

    static void ChangePixel(unsigned char *pixels, size_t index, int Val) ;

    static void PrintToFile(const std::string& fileName,  const unsigned char* buffer, bool isGrayScale);

    static void ApplyHalftonePatternOnPixel(int level, size_t x, size_t y, unsigned char *pixels);
};

