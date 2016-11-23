#include "mapping.h"
#include <assert.h>

// ֻ��95%û�б������ǰ�ȫ��
const float linear_middle = 0.05f;

// ����������80%���ϵ�ʱ�򣬾���100%��Σ��
const float linear_top = 0.80f;

float linear(float f)
{
	assert(f >= 0 && f <= 1);
	float res = 0;
	if (f > linear_middle)
	{
		// ����ٷ�֮85�Ķ���Σ�գ��Ǿ���Σ�յ�
		if (linear_middle > linear_top)
		{
			res = 100;
		}
		else
		{
			res = (f - linear_middle) * 50.0f / (linear_top - linear_middle) + 50.0f;
		}
	}
	else if (f < linear_middle)
	{
		res = f *  50.0f / linear_middle;
	}
	else
	{
		res = 50;
	}
	assert(res >= 0 && res <= 100);
	return res;
}