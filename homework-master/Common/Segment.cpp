#pragma once

#include "segment.h"

//using namespace Segment;
namespace Segment
{
	istream & operator>>(istream & infs, Point3 & p)
	{
		infs >> p.posIdx >> p.textureIdx >> p.normalIdx;
		return infs;
	}


	ostream & operator<<(ostream & onfs, const Point3 & p)
	{
		onfs << p.posIdx << p.textureIdx << p.normalIdx;
		return onfs;
	}

	inline int getRandom(int r)
	{
		return rand() % r;
	}

	SegmentProcess::SegmentProcess(string fname,int id) :initK(id),filename(fname)
	{
		loadModel();
	}

	SegmentProcess::~SegmentProcess()
	{

	}

	/*��ʼ������������ģ��*/
	void SegmentProcess::loadModel()
	{
		ifstream infs(filename);
		if (!infs.is_open())
		{
			cerr << "open file error!\n";
			exit(0);
		}
		string tmp;
		glm::vec3 v;
		while (!infs.eof())
		{
			infs >> tmp;
			if (tmp == "v")
			{
				infs >> v[0] >> v[1] >> v[2];
				this->Position.push_back(v);
			}
			else if (tmp == "vt")
			{
				infs >> v[0] >> v[1] >> v[2];
				this->TextureCoord.push_back(v);
			}
			else if (tmp == "vn")
			{
				infs >> v[0] >> v[1] >> v[2];
				this->Normal.push_back(v);
			}
			else if (tmp == "g")
			{
				meshId++;		// �ı�meshid
				infs >> tmp;
				mMatrial[meshId].meshId = meshId;
				mMatrial[meshId].meshName = tmp;
			}
			else if (tmp == "usemtl")
			{
				infs >> tmp;
				mMatrial[meshId].mMatrial = tmp;
			}
			else if (tmp == "f")
			{
				vector<string> vs(3);
				infs >> vs[0] >> vs[1] >> vs[2];
				glm::ivec3 iv;
				if (sscanf(vs[0].c_str(), "%d/%d/%d", &iv[0], &iv[1], &iv[2]) == 1)	// ����������
				{
					vector<Point3> vp;
					for (auto s : vs)
					{
						Point3 p(-1, -1, -1);
						sscanf(s.c_str(), "%d//%d", &p.posIdx, &p.normalIdx);
						vp.push_back(p);
					}
					Triangle3 t;
					t.meshId = meshId;		// �趨��������ε����
					t.vp = vp;
					Triangles.push_back(t);
				}
				else if (sscanf(vs[0].c_str(), "%d/%d/%d", &iv[0], &iv[1], &iv[2]) == 3)	// �������궼��
				{
					vector<Point3> vp;
					for (auto s : vs)
					{
						Point3 p(-1, -1, -1);
						sscanf(s.c_str(), "%d/%d/%d", &p.posIdx, &p.textureIdx, &p.normalIdx);
						vp.push_back(p);
					}
					Triangle3 t;
					t.meshId = meshId;
					t.vp = vp;
					Triangles.push_back(t);
				}
				else
				{

				}
			}
			else
			{
				getline(infs, tmp);
			}
		}
		infs.close();
	}

	/*���ຯ�������þ������*/
	void SegmentProcess::clusterModel(string filename)
	{
		genClusterSeed(initK);
		cluster();
		printCluster(filename);
	}

	/*���ຯ��������ԭʼ���������*/
	void SegmentProcess::genClusterSeed(int k)
	{
		int cnt = 0;
		printf("k:%d\n", k);
		int size = Triangles.size();
		for (int i = 0; i < k; ++i)
		{
			int idx = getRandom(size);
			if (Triangles[idx].cluster != -1)		// �Ѿ�ѡ���ˣ��˳�
				continue;
			set<int> si;
			si.insert(Triangles[idx].vp[0].posIdx);
			si.insert(Triangles[idx].vp[1].posIdx);
			si.insert(Triangles[idx].vp[2].posIdx);
			point3Cluster.push_back(si);
			Triangles[idx].cluster = point3Cluster.size();		// �趨��𣬱�־����������Ѿ�ѡ��
			cnt++;
		}
		for (int i = 0; i < Triangles.size(); ++i)		// ��δ������뵽һ��set��
		{
			if (Triangles[i].cluster == -1)
			{
				unsolveTriangles.insert(i);
			}
		}
		printf("cluster size: %d\n", cnt);
		printf("unsolveTriangles size: %d\n", unsolveTriangles.size());
		unionCluster();
		printf("generator complete!\n");
	}

	/*���ຯ���������µ�һ������*/
	bool SegmentProcess::genClusterOrder()
	{
		printf("unsolveTrianglesCnt: %d\n", unsolveTriangles.size());
		if (unsolveTriangles.size() == 0)		// �Ѿ�û��δ�����
			return false;
		int idx = *(unsolveTriangles.begin());
		// �������
		set<int> si;
		si.insert(Triangles[idx].vp[0].posIdx);
		si.insert(Triangles[idx].vp[1].posIdx);
		si.insert(Triangles[idx].vp[2].posIdx);
		point3Cluster.push_back(si);
		Triangles[idx].cluster = point3Cluster.size();		// �趨���
		// ɾ����Ԫ��
		unsolveTriangles.erase(idx);
		return true;
	}

	/*���ຯ����������о������*/
	void SegmentProcess::cluster()
	{
		while (true)
		{
			while (true)
			{
				bool f = false;
				set<int> clusterIdx;
				// ��δ�������������ѡ��һ��
				for (auto it = unsolveTriangles.begin(); it != unsolveTriangles.end(); ++it)
				{
					int idx = *it;
					Triangle3 tr = Triangles[idx];		// ��Ӧ��������			
					for (int i = 0; i < point3Cluster.size(); ++i)
					{
						set<int> &s = point3Cluster[i];	// ÿһ�����
						if (s.size() == 0)
							continue;
						set<int> p;						// ���������ε�������ŵ�����֮�У������󽻼�
						p.insert(tr.vp[0].posIdx);
						p.insert(tr.vp[1].posIdx);
						p.insert(tr.vp[2].posIdx);
						set<int> s3;
						set_intersection(s.begin(), s.end(), p.begin(), p.end(), inserter(s3, s3.begin()));
						if (s3.size() > 0)		// �н���
						{
							// ����cluster
							set_union(s.begin(), s.end(), p.begin(), p.end(), inserter(s, s.begin()));
							f = true;
							Triangles[idx].cluster = i;	// ��������־
							clusterIdx.insert(idx);	// ���ڼ�¼��һЩ�Ѿ������ˣ���δ���༯����ȥ��
							break;
						}
					}
				}
				if (f)		// ��һ���и��£�����δ����������μ���
				{
					set<int> tmp;
					set_difference(unsolveTriangles.begin(), unsolveTriangles.end(), clusterIdx.begin(), clusterIdx.end(), inserter(tmp, tmp.begin()));
					unsolveTriangles = tmp;
					unionCluster();		// ���¼���
				}
				else
				{
					break;	// ���û��һ������ɹ�����Ҫ�ٲ���һ������
				}
			}
			if (!genClusterOrder())
				break;
		}
	}

	/*����������ϲ�����*/
	void SegmentProcess::unionCluster()
	{
		for (int i = 0; i < point3Cluster.size() - 1; ++i)
		{
			if (point3Cluster[i].size() == 0)
				continue;
			for (int j = i + 1; j < point3Cluster.size(); ++j)
			{
				set<int> tmp;
				set_intersection(point3Cluster[i].begin(), point3Cluster[i].end(), point3Cluster[j].begin(), point3Cluster[j].end(), inserter(tmp, tmp.begin()));
				if (tmp.size() > 0)	// ����н���
				{
					set_union(point3Cluster[i].begin(), point3Cluster[i].end(), point3Cluster[j].begin(), point3Cluster[j].end(), inserter(point3Cluster[i], point3Cluster[i].begin()));
					point3Cluster[j].clear();
				}
			}
		}
	}

	/*�������������𶥵㴦��������ε�����*/
	void SegmentProcess::clusterTriangle(string filename, string clustername)
	{
		vector<set<int> > pointsCluster;
		ifstream infs(filename);
		int clusterCnt;
		infs >> clusterCnt;
		for (int i = 0; i < clusterCnt; ++i)
		{
			int pointsCnt;
			infs >> pointsCnt;
			set<int> v;
			int tmp;
			for (int j = 0; j < pointsCnt; ++j)
			{
				infs >> tmp;
				v.insert(tmp);
			}
			pointsCluster.push_back(v);		// �洢ÿһ��Ķ���
		}
		infs.close();
		ofstream onfs(clustername);
		onfs << pointsCluster.size() << endl;
		for (int i = 0; i < Triangles.size(); ++i)
		{
			Triangle3 tr = Triangles[i];
			set<int> si;
			si.insert(tr.vp[0].posIdx);
			si.insert(tr.vp[1].posIdx);
			si.insert(tr.vp[2].posIdx);
			for (int j = 0; j < pointsCluster.size(); ++j)
			{
				set<int> &tmps = pointsCluster[j];
				set<int> s3;
				set_intersection(tmps.begin(), tmps.end(), si.begin(), si.end(), inserter(s3, s3.begin()));
				if (s3.size() > 0)		// �н�����������
				{
					onfs << j << endl;
					break;
				}
			}
		}
		onfs.close();
	}

	/*�����������ӡÿ������µĶ���*/
	void SegmentProcess::printCluster(string filename)
	{
		int cnt = 0;
		FILE * f = fopen(filename.c_str(), "w");
		/*ͳ������*/
		for (int i = 0; i < point3Cluster.size(); ++i)
		{
			set<int> &tmp = point3Cluster[i];
			if (tmp.size() == 0)
				continue;
			++cnt;
		}
		fprintf(f, "%d\n", cnt);
		for (int i = 0; i < point3Cluster.size(); ++i)
		{
			set<int> &tmp = point3Cluster[i];
			if (tmp.size() == 0)
				continue;
			fprintf(f, "%d\n", tmp.size());
			for (auto it = tmp.begin(); it != tmp.end(); ++it)
			{
				fprintf(f, "%d ", *it);
			}
			fprintf(f, "\n");
		}
		fclose(f);
		printf("cluster cnt: %d\n", cnt);
	}

	/*���ܺ�������ӡģ����Ϣ*/
	void SegmentProcess::printModel(ostream & f)
	{
		for (auto it = mMatrial.begin(); it != mMatrial.end(); it++)
		{
			f << it->first << endl;
			f << it->second.meshId << " " << it->second.meshName << " " << it->second.mMatrial << endl;
		}
		f << "Triangles count: " << this->Triangles.size() << endl;
		f << "Position count: " << this->Position.size() << endl;
		f << "Normal count: " << this->Normal.size() << endl;
		f << "TextureCoord count: " << this->TextureCoord.size() << endl;
	}
}

