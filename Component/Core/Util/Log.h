#pragma once
#include <iostream>
#include <initializer_list>

namespace Core
{
	template <typename T, typename... Types>
	void Print(const T& firstArg, const Types&... args)
	{
		std::initializer_list <int> { ([&args] {std::cout << args << std::endl; }(), 0)...};
	}
}