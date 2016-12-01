#pragma once

#include <memory>

using namespace std;

class Logistic
{
private:
	// ���ݵ�ά��
	const int m_dim;
	// ������
	const int m_n;
	// ѧϰ��
	const double m_a;

	// �۲����� 
	// �����������岻��д�鷳�Ŀ������캯���͸�ֵ������
	shared_ptr<double> m_x;
	// ��ע����
	shared_ptr<double> m_y;
	// ѧϰģ��
	shared_ptr<double> m_k;

private:

	// ԭ����
	double Fun(int i) const;

	// ��ʧ����
	double Loss() const;

	double GetX(int i,int j) const;
	double GetY(int i) const;
	double GetK(int j) const;

	void SetX(int i, int j,double v);
	void SetY(int i,double v);
	void SetK(int j, double v);

public:

	Logistic(int dim,int n);
	
	virtual ~Logistic();

	// ����ݶ��½�
	void SGD();	
	
	// ��׼�ݶ��½�
	void GD();

	// �����ݶ��½�
	void MGD(int mini);

	// ���Ժ���
	void Test();
};

