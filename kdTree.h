#pragma once
#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
using namespace std;

int dimUsed;

// kd���Ľڵ㶨��
struct KdNode
{
	KdNode * parent;
	KdNode * leftChild;
	KdNode * rightChild;
	vector<int> val;	//�洢������
	int axis;		// ʹ�õ���
	KdNode(vector<int> data, int ax)
	{
		val = data;
		axis = ax;
		parent = NULL;
		leftChild = NULL;
		rightChild = NULL;
	}
};

// �����Ե�nλ���бȽϣ���������
bool cmp(vector<int> a, vector<int> b)
{
	if (a[dimUsed] < b[dimUsed])
		return true;
	return false;
}

ostream & operator<<(ostream & os, vector<int> vi)
{
	os << "(";
	for (int i = 0; i < vi.size(); i++)
		cout << vi[i] << ",";
	os << ")";
	return os;
}

// �������ݣ���һ��������������ά�ȣ��������num��dimά�ȵ����ݣ�ÿ��һ��
bool loadData(string filename, vector<vector<int> > &data)
{
	ifstream infs(filename);
	if (infs.is_open())
	{
		int num,dim;
		infs >> num>>dim;
		vector<int> d(dim);
		for (int i = 0; i < num; i++)
		{
			for (int j = 0; j < dim; j++)
				infs >> d[j];
			data.push_back(d);
		}
		return true;
	}
	return false;
}

// ����Nά��������
int disVector(vector<int> a, vector<int> b)
{
	int sum = 0;
	for (int i = 0; i < a.size(); i++)
		sum += (a[i] - b[i])*(a[i] - b[i]);
	return sum;
}

class KdTree
{
private:
	int dimension;
	vector<vector<int> > data;
	KdNode * root;
public:
	KdTree(vector<vector<int> > d, int dim)
	{
		dimension = dim;
		data = d;
	}
	void createTree()
	{
		// �ݹ齨��
		root = createTreeNode(0, data.size()-1,0);
	}

	// create Kd Tree struct
	KdNode * createTreeNode(int left, int right,int dim)
	{
		if (right < left)
			return NULL;
		dimUsed = dim;
		// ����kά��������
		sort(data.begin() + left, data.begin() + right+1, cmp);
		int mid = (left + right+1) / 2;
		KdNode * r = new KdNode(data[mid], dim);
		r->leftChild = createTreeNode(left, mid - 1, (dim + 1) % dimension);
		if (r->leftChild != NULL)
			r->leftChild->parent = r;
		r->rightChild = createTreeNode(mid + 1, right, (dim + 1) % dimension);
		if (r->rightChild != NULL)
			r->rightChild->parent = r;
		return r;
	}

	void printKdTree()
	{
		printKdTreeNode(root);
	}

	void printKdTreeNode(KdNode * r)
	{
		if (r == NULL)
			return;
		printKdTreeNode(r->leftChild);
		cout << r->val << "\t";
		printKdTreeNode(r->rightChild);
	}

	// ����kd��
	KdNode * searchKdTree(vector<int> d)
	{
		int dim = 0,minDis = 10000000;
		KdNode * r = root;
		KdNode * tmp;
		while (r != NULL)
		{
			tmp = r;
			if (d[dim] < r->val[dim])							
				r = r->leftChild;	
			else
				r = r->rightChild;
			dim = (dim + 1) % dimension;
		}
		// �ҵ����ڵ��Ǹ��ڵ�
		minDis = min(disVector(d, tmp->val),minDis);
		KdNode * nearNode = tmp;
		cout << endl<<"nearest node: "<<tmp->val << endl;
		// �˻ص����ڵ�
		while (tmp->parent != NULL)
		{
			tmp = tmp->parent;
			// �жϸ��ڵ��Ƿ���������������������ڵ�
			if (disVector(tmp->val, d) < minDis)
			{
				nearNode = tmp;
				minDis = disVector(tmp->val, d);
			}
			cout << "now parent node: " << tmp->val << endl;
			KdNode * son;
			// �жϵ�ǰ�����ľ��룬���С��minDis������е���һ����в���
			if (abs(tmp->val[tmp->axis] - d[tmp->axis]) < minDis)
			{
				if (tmp->val[tmp->axis] > d[tmp->axis])
					son = tmp->rightChild;
				else
					son = tmp->leftChild;
				searchKdTreeNode(d, minDis, nearNode, son);
			}
		}	
		// �Ը��ڵ��������������������
		/*if (abs(tmp->val[tmp->axis] - d[tmp->axis]) < minDis)
		{
			if (tmp->val[tmp->axis] > d[tmp->axis])
				tmp = tmp->rightChild;
			else
				tmp = tmp->leftChild;
			searchKdTreeNode(d, minDis, nearNode, tmp);
		}*/
		return nearNode;
	}

	// ���ҵ�ǰ�ڵ��µ������
	void searchKdTreeNode(vector<int> d,int &minDis,KdNode * &nearNode,KdNode * tmp)
	{
		// �ݹ���ֹ
		if (tmp == NULL)
			return;
		cout << "now node: " << tmp->val << endl;
		// �жϵ�ǰ�ڵ��Ƿ�С��
		if (disVector(tmp->val, d) < minDis)
		{
			minDis = disVector(tmp->val, d);
			nearNode = tmp;
		}
		// �������ڵ�ľ���С��minDis����������߶���Ҫ����������ֻ��Ҫ����һ�����
		if (abs(tmp->val[tmp->axis] - d[tmp->axis]) < minDis)
		{
			searchKdTreeNode(d, minDis, nearNode, tmp->leftChild);
			searchKdTreeNode(d, minDis, nearNode, tmp->rightChild);
		}
		else
		{
		// ѡ��������һ�����
			if (tmp->val[tmp->axis] > d[tmp->axis])
				searchKdTreeNode(d, minDis, nearNode, tmp->leftChild);
			else
				searchKdTreeNode(d, minDis, nearNode, tmp->rightChild);
		}
		
	}

};

// ����kd��
void testKdTree()
{
	vector<vector<int> > data;
	loadData("kd.txt", data);
	KdTree * kdtree = new KdTree(data, data[0].size());
	kdtree->createTree();
	kdtree->printKdTree();
	cout << endl;
	vector<int> vi(2);
	cin >> vi[0] >> vi[1];
	KdNode * r = kdtree->searchKdTree(vi);
	cout << r->val << endl;
}