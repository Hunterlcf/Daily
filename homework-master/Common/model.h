#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include "shader.h"

/************************************************************************
   created: 2018/01/18
   class: 
   author: linyi
   usage: model�ĳ�����࣬��������¼���������
		1. ����ģ��
		2. ��ӡ��Ϣ
		3. ��������
		4. ����ģ��
		5. ����������
************************************************************************/

struct SpecialPoint
{
	glm::vec3 pos;
	glm::vec3 normal;
	glm::vec3 sdf;
};

class Model
{
public:
	virtual bool loadModel(std::string str) =0;
	virtual void printInfo()=0;
	virtual void setupModel()=0;	
	virtual void drawModel(Shader *shader)=0;	
protected:
	GLuint VAO;
	GLuint VBO;
	GLuint EBO;		
};