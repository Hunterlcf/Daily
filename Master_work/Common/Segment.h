#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <glm/glm.hpp>
#include <map>
#include <set>
#include <ctime>
#include <algorithm>  
#include <iterator>

/************************************************************************
   created: 2018/02/01
   class: 
   author: linyi
   usage: ��obj�ļ����зָ���յ����ͨ�Խ��зָ�
************************************************************************/
namespace Segment
{
	using namespace std;
	inline int getRandom(int r);

	struct Point3
	{
		int posIdx;
		int textureIdx;
		int normalIdx;
		Point3() = default;
		Point3(int i, int j, int k) :posIdx(i), textureIdx(j), normalIdx(k){}
		Point3(const Point3 &p) 
		{
			posIdx = p.posIdx;
			textureIdx = p.textureIdx;
			normalIdx = p.normalIdx;
		}
		Point3 & operator=(const Point3 & p)
		{
			posIdx = p.posIdx;
			textureIdx = p.textureIdx;
			normalIdx = p.normalIdx;
			return *this;
		}
	};

	istream & operator>>(istream & infs, Point3 & p);

	ostream & operator<<(ostream & onfs, const Point3 & p);

	struct Triangle3
	{
		vector<Point3> vp;
		int meshId;
		int cluster;
		Triangle3() :cluster(-1){}
	};

	struct MeshInfo
	{
		int meshId;
		string meshName;
		string mMatrial;
	};

	class SegmentProcess
	{
	public:
		string filename;
		/*��ȡԭʼ������*/
		vector<glm::vec3> Normal;
		vector<glm::vec3> TextureCoord;
		vector<glm::vec3> Position;
		/*���������������*/
		vector<Triangle3> Triangles;
		/*�������ʵĶ�Ӧ��ϵ*/
		map<int, MeshInfo> mMatrial;	// ��¼meshidӳ���ϵ
		/*�洢ÿ������µĶ���*/
		vector<set<int> > point3Cluster;
		vector<set<int> > triangle3Cluster;
		set<int> unsolveTriangles;			// δ�����������
		int meshId = -1;
		int initK;

		SegmentProcess(string fname,int id);
		~SegmentProcess();

		/*��ʼ������������ģ��*/
		void loadModel();

		/*���ຯ�������þ������*/
		void clusterModel(string filename);

		/*���ຯ��������ԭʼ���������*/
		void genClusterSeed(int k);

		/*���ຯ��������һ������*/
		bool genClusterOrder();

		/*���ຯ����������о������*/
		void cluster();

		/*����������ϲ�����*/
		void unionCluster();

		/*�������������𶥵㴦��������ε�����*/
		void clusterTriangle(string filename, string clustername);

		/*�����������ӡÿ������µĶ���*/
		void printCluster(string filename);

		/*���ܺ�������ӡģ����Ϣ*/
		void printModel(ostream & f);
	private:
	};

}
