#include <iostream>
#include "PageV1_1.hpp"
#include "VirtualMemory.hpp"
int main()
{
	size_t choice;
	bool work = true;
	std::cout << "Список команд" << std::endl;
	std::cout << "1. Create" << std::endl;
	std::cout << "2. Input" << std::endl;
	std::cout << "3. Print" << std::endl;
	std::cout << "4. Exit" << std::endl;
	while (work)
	{
		std::cin >> choice;
		switch (choice)
		{
		case 1:
			break;
		case 2:
			break;
		case 3:
			break;
		case 4:
			work = false;
			break;
		default:
			break;
		}
	}
	return 0;
}