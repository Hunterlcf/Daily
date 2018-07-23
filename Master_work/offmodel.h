#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <cmath>
#include <random>
#include <unordered_set>
#include "shader.h"
#include "model.h"


/************************************************************************
   created: 2018/01/18
   class: offModel
   author: linyi
   usage: ����off�ļ���model���̳���Model����
************************************************************************/
class offModel:public Model
{
public:
	float maxEdge;
	glm::vec3 box_center;
	offModel(std::string str);
	~offModel();
	virtual bool loadModel(std::string str);
	virtual void printInfo();
	virtual void setupModel();	
	virtual void drawModel(Shader *shader);
	
	void readyToWork();		

	void setupModel_SDF();		// ת��Ϊ�б���ʹ��EBO		
	void setupModel_Normal();	// ʹ�÷���
	void setupModel_Edge();		// ��ȡ��Ե��ͬʱ��SDF������һ��ͨ��
	void setupModel_SpecialPoints();	// ������

	void loadTriangleSDF(std::string);
	void findLocalMax();	// Ѱ�Ҿֲ�����
	void saveSpecialPoints2File(std::string);
	void saveAllPoints(std::string);

	void drawModel_withoutEBO(Shader *shader);
	void drawModel_specialPoints(Shader *shader);

	void modelTransform();
	void setOffset(float o);
	void saveModelMatrix(std::string str);

	glm::mat4 modelMat;
	
private:
	std::vector<glm::vec3> Points;	// ����Ϣ
	std::vector<glm::ivec3> Triangles;	// ������Ƭ
	std::vector<float> trianglesSDF;	// ������SDF
	std::vector<glm::vec3> trianglesNormal;	// �����η���
	std::vector<float> Dihedral;	// �����
	std::vector<glm::vec3> sPoints;		// ������
	std::vector<glm::vec3> sColor;
	std::vector<glm::vec3> sNormal;
	int pointsCnt;
	int trianglesCnt;
	int edgesCnt;
	int specialPointsCnt;
	GLuint pVAO;		// �����VAO
	GLuint pVBO;
	glm::vec3 box_min = glm::vec3(1e10);
	glm::vec3 box_max = glm::vec3(-1e10);	
	std::unordered_map<int, std::vector<int> > adjactRelation;	// �洢�ڽӹ�ϵ

	void findTrianglesNormal();	// ���������η���
	void findAdjactRelation();	// ����������
	void findDihedral();		// ��������	

	float offset=0;
};