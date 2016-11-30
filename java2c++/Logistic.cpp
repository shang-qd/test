#include "Logistic.h"

#include <stdio.h>
#include <vector>
#include <thread>
#include <chrono>
#include <math.h>
#include <limits>

Logistic::Logistic(int dim, int n) : m_dim(dim), m_n(n), m_a(0.001)
{
	// 观察数据
	m_x = shared_ptr<double>(new double[m_n * m_dim]);
	// 标注数据
	m_y = shared_ptr<double>(new double[m_n]);
	// 学习模型
	//m_k = new double[m_dim];
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

void Logistic::Study()
{
	for (int i = 0; i < m_n; i++)
	{
		for (int j = 0; j < m_dim; j++)
		{
			SetK(j, GetK(j) + m_a * Gradient(i, j));
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


double Logistic::Gradient(int i, int j) const
{
	return (GetY(i) - Fun(i)) * GetX(i,j);
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
	for (int j = 0; j < m_dim; j++)
	{
		SetK(j, j + 1);
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
		SetY(i, y_res);
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
		Study();
		if (i % 500 == 0)
		{
			for (int j = 0; j < m_dim; j++)
			{
				printf("%f  \n", GetK(j));
			}
			double l = Loss();
			printf("----%d----------------------loss = %f ---------\n", i, l);
			if (fabs(l) < epsilon)
			{
				break;
			}
			std::this_thread::sleep_for(std::chrono::seconds(2));
		}
	}
}