#ifndef _SAMPLE_
#define _SAMPLE_
#include "callback.h"
#include "camera.h"
#include "shader.h"
#include "glCallback.h"
#include "offmodel.h"
#include "specialPointsModel.h"
#include "nearestPairModel.h"
#include "lightModel.h"
#include <vector>

/************************************************************************
   created: 2018/01/18
   class: Sample
   author: linyi
   usage: ���ƻ��࣬�����������shader��model�ȶ���࣬�̳��Իص�����
************************************************************************/

class Sample :public ICallbacks
{
public:
	Sample();
	virtual ~Sample();
	virtual void MouseAction(double x, double y);
	virtual void KeyAction(int key,int action);
	virtual void RenderScene();
	void sampleInit();
	void sampleRun();
	void drawModel();
	void treatKey();
private:
	Camera * camera;
	Shader * shader;
	offModel * offmodel1;
	offModel * offmodel2;
	NearestPairModel * nearModel;
	SpecialPointsModel * spModel1;
	SpecialPointsModel * spModel2;
	LightModel * lightModel1;
	LightModel * lightModel2;
	bool firstMouse;
	double lastX, lastY;
	int width, height;
	GLfloat deltaTime;
	GLfloat lastFrame;
	std::vector<bool> key;
};

#endif