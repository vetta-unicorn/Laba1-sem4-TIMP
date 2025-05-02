#include <iostream>

#include "UserCommands.hpp"
int main()
{
	UserCommands us;
	size_t choice;
	bool work = true;
	std::cout << "List commands" << std::endl;
	std::cout << "1. Create" << std::endl;
	std::cout << "2. Input" << std::endl;
	std::cout << "3. Print" << std::endl;
	std::cout << "4. Exit" << std::endl;
	std::cout << "5. Print stack" << std::endl;
	while (work)
	{
		std::string fileName;
		size_t typePage;
		std::cout << "Enter choice: ";
		std::cin >> choice;
		std::cout << std::endl;
		try
		{
			switch (choice)
			{
			case 1:
				std::cout << "Enter file name: ";
				std::cin >> fileName ;

				std::cout << "List of types" << std::endl;
				std::cout << "1. INT" << std::endl;
				std::cout << "2. CHAR" << std::endl;
				std::cout << "3. VARCHAR" << std::endl;
				std::cout << "Enter type: ";
				std::cin >> typePage;

				try
				{
					switch (typePage)
					{
					case 1:
						if (us.Create(fileName, Types::T_INT, 45))
						{
							std::cout << "It has now been created" << std::endl;
						}
						else
						{
							std::cout << "Error: Cannot create or write file" << std::endl;
						}
						break;
					case 2:
						if (us.Create(fileName, Types::T_CHAR, 1))
						{
							std::cout << "It has now been created" << std::endl;
						}
						else
						{
							std::cout << "Error: Cannot create or write file" << std::endl;
						}
						break;
					case 3:
						size_t maxLenStr;
						std::cout << "Enter max size for varchar type: ";
						std::cin >> maxLenStr;
						if (us.Create(fileName, Types::T_VARCHAR, maxLenStr))
						{
							std::cout << "It has now been created" << std::endl;
						}
						else
						{
							std::cout << "Error: Cannot create or write file" << std::endl;
						}
						break;
					}
					break;
				}
				catch (const std::exception& ex)
				{
					std::cerr << ex.what() << std::endl;
				}


			case 2:
			{
				size_t index;
				std::string value;

				int intResult;
				char charResult;
				std::cout << "Enter index: ";
				std::cin >> index;

				std::cout << "Enter value: ";
				std::cin >> value;
				if (us.stringToInt(value, intResult))
				{
					us.Input(index, intResult);
				}
				else if (us.stringToChar(value, charResult))
				{
					us.Input(index, charResult);
				}
				else
				{
					us.Input(index, value);
				}
			}
			
				break;
			case 3:
				break;
			case 4:
				work = false;
				break;
			case 5:
			{
				us.PrintVMStack(Types::T_CHAR);
			}
			break;
			default:
				break;
			}
		}
		catch (const std::exception& ex)
		{
			std::cerr << ex.what() << std::endl;
		}
		
	}
	return 0;
}
