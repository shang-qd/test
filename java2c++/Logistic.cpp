#include <stdio.h>
#include <thread>
#include <chrono>
#include <math.h>
#include <limits>
#include <assert.h>
#include <vector>
#include <random>

#include "Logistic.h"

Logistic::Logistic(int dim, int n) : m_dim(dim), m_n(n), m_a(0.001)
{
	// 观察数据
	m_x = shared_ptr<double>(new double[m_n * m_dim]);
	// 标注数据
	m_y = shared_ptr<double>(new double[m_n]);
	// 学习模型
	m_k = shared_ptr<double>(new double[m_dim]);
}

Logistic::~Logistic()
{
}

double Logistic::GetX(int i, int j) const
{
	return m_x.get()[i * m_dim + j];
}

double Logistic::GetY(int i) const
{
	return m_y.get()[i];
}

double Logistic::GetK(int j) const
{
	return m_k.get()[j];
}

void Logistic::SetX(int i, int j,double v)
{
	m_x.get()[i * m_dim + j] = v;
}

void Logistic::SetY(int i,double v)
{
	 m_y.get()[i] = v;
}

void Logistic::SetK(int j,double v)
{
	m_k.get()[j] = v;
}

// 随机梯度下降
void Logistic::SGD()
{
	for (int i = 0; i < m_n; i++)
	{
		// 求梯度
		double d_g = GetY(i) - Fun(i);
		// 各个更新向量
		for (int j = 0; j < m_dim; j++)
		{
			SetK(j, GetK(j) + m_a * d_g * GetX(i, j));
		}
	}
}

// 标准梯度下降
void Logistic::GD()
{
	for (int j = 0; j < m_dim; j++)
	{
		double tmp_d = 0;
		for (int i = 0; i < m_n; i++)
		{
			 tmp_d += (GetY(i) - Fun(i)) * GetX(i,j);
		}
		SetK(j, GetK(j) + m_a * tmp_d);
	}
}

// 迷你梯度下降
void Logistic::MGD(int mini)
{
	for (int k = 0; k < m_n; k += mini)
	{
		for (int j = 0; j < m_dim; j++)
		{
			double tmp_d = 0;
			for (int i = k; i < (k + mini); i++)
			{
				tmp_d += (GetY(i) - Fun(i)) * GetX(i, j);
			}
			SetK(j, GetK(j) + m_a * tmp_d);
		}
	}
}

double Logistic::Fun(int i) const
{
	double res = 0;
	for (int j = 0; j < m_dim; j++)
	{
		res += GetX(i, j) * GetK(j);
	}
	return res;
}

double Logistic::Loss() const
{
	double res = 0;
	for (int i = 0; i < m_n; i++)
	{
		double f = (Fun(i) - GetY(i));
		res += (f * f) / 2;
	}
	return res;
}

void Logistic::Test()
{
	std::random_device rd;
	// 高质量的随机数
	std::mt19937 gen(rd());
	// 正太分布
	std::normal_distribution<double> nd(1.0, 0.5);

	for (int j = 0; j < m_dim; j++)
	{
		SetK(j, 10 - j);
	}
	for (int i = 0; i < m_n; i++)
	{
		double y_res = 0;
		for (int j = 0; j < m_dim; j++)
		{
			double res = rand() % 17;
			SetX(i, j, res);
			y_res += res * GetK(j);
		}
		SetY(i, y_res + nd(gen));
	}

	// 加载数据结束 
	//printf("----%d----------------------loss = %f ---------\n", 0, Loss());
	for (int j = 0; j < m_dim; j++)
	{
		SetK(j, 0);
	}
	int len = std::numeric_limits<int>::max();

	double epsilon = std::numeric_limits<double>::epsilon();

	for (int i = 0; i < len; i++)
	{
		SGD();
		if (i % 500 == 0)
		{
			for (int j = 0; j < m_dim; j++)
			{
				printf("%f  \n", GetK(j));
			}
			double l = Loss();
			printf("----%d----------------------loss = %f ---------\n", i, l);
			// 浮点数的比较应该这样，如果直接跟0比较,需要的精度真的是太高。
			if (fabs(l) < epsilon || i > 10000)
			{
				break;
			}
			std::this_thread::sleep_for(std::chrono::seconds(2));
		}
	}
}