#pragma once

#include <memory>

using namespace std;

class Logistic
{
private:
	// 数据的维数
	const int m_dim;
	// 样本数
	const int m_n;
	// 学习率
	const double m_a;

	// 观察数据 
	// 变量这样定义不用写麻烦的拷贝构造函数和赋值函数了
	shared_ptr<double> m_x;
	// 标注数据
	shared_ptr<double> m_y;
	// 学习模型
	shared_ptr<double> m_k;

private:

	// 原函数
	double Fun(int i) const;

	// 损失函数
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

	// 随机梯度下降
	void SGD();	
	
	// 标准梯度下降
	void GD();

	// 迷你梯度下降
	void MGD(int mini);

	// 测试函数
	void Test();
};

