#include "Test.h"
#include <iostream>

void test_1()
{
	std::vector<comm_info> vci;
	for (int i = 0; i < 1000; i++)
	{
		comm_info ci(100);
		ci.rand();
		vci.push_back(move(ci));
	}

	for (comm_info ci : vci)
	{
		float f = anger_rate(ci.id, ci.get_danger(), ci.get_all());
		std::cout << linear(f) << std::endl;
	}
}