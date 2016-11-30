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
	shared_ptr<double> m_x;
	// 标注数据
	shared_ptr<double> m_y;
	// 学习模型
	shared_ptr<double> m_k;

private:
	// 梯度函数 i 表示数据的样本，j 表示数据的维数
	double Gradient(int i, int j) const;

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

	// 学习函数
	void Study();	
	
	// 测试函数
	void Test();
};

