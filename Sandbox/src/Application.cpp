#include <iostream>

namespace GitGud
{
	__declspec(dllimport) void Print();
}

void main()
{
	GitGud::Print();
	std::cin.get();
}