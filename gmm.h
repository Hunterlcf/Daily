#ifndef _GMM_H
#define _GMM_H
#include <vector>
#include <cmath>
using namespace std;
class GMM
{
public:
	void Init(const vector<double> &inputData, const int clustNum = 5, double eps = 0.01, double max_steps = 20);
	void train();
	int predicate(double x);//Ԥ�����������������һ��
	void print();
protected:
	int clusterNum;				// ����
	vector<double> means;
	vector<double> means_bkp;		// ��һ�εĵ�������
	vector<double> sigmas;
	vector<double> sigmas_bkp;		// ��һ�εĵ�������
	vector<double> probilities;
	vector<double> probilities_bkp;
	vector<vector<double>> memberships;		// �洢������һ�����
	vector<vector<double>> memberships_bkp;
	vector<double> data;
	int dataNum;		// ��������
	double epslon;		// ������ֵ
	double max_steps;	// ��������
private:
	double gauss(const double x, const double m, const double sigma);
};
#endif
