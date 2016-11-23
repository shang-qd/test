#include "mapping.h"
#include <assert.h>

// 只有95%没有报警才是安全的
const float linear_middle = 0.05f;

// 当报警率在80%以上的时候，就是100%的危险
const float linear_top = 0.80f;

float linear(float f)
{
	assert(f >= 0 && f <= 1);
	float res = 0;
	if (f > linear_middle)
	{
		// 如果百分之85的都报危险，那就是危险的
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