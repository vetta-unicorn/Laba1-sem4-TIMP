#include "ControlHeaders.hpp"

enum class Types {T_INT, T_CHAR, T_VARCHAR};

class UserCommands
{
private:
	Types typePage;
	std::vector<VirtualMemory> stack;
public:
	UserCommands() {}

	bool Create(std::string fileName, Types type, size_t maxLenStr)
	{
		std::string fullName = fileName;
		if (maxLenStr == 0)
		{
			throw std::invalid_argument("Argument maxLenStr is zero");
			return false; 
		}
		std::fstream file(fullName, std::ios::out | std::ios::in | std::ios::trunc);
		if (!file.is_open())
		{
			throw std::runtime_error("Cannot open or create file: " + fullName);
		}
		switch (type)
		{
		case Types::T_CHAR:
		{
			typePage = Types::T_CHAR;
			PageV1_1<char> page;
			VirtualMemory vm;
			file << page;
			if (!file.good())
			{
				throw std::runtime_error("Failed to write page to file: " + fullName);
			}
			if (vm.write(0, page))
			{
				stack.push_back(vm);
				return true;
			}
		}
		case Types::T_INT:
		{
			typePage = Types::T_INT;
			PageV1_1<int> page;
			VirtualMemory vm;
			file << page;
			if (!file.good())
			{
				throw std::runtime_error("Failed to write page to file: " + fullName);
			}
			if (vm.write(0, page))
			{
				stack.push_back(vm);
				return true;
			}
			else 
			{
				throw std::exception("ge-ge-ge");
			}
		}
		case Types::T_VARCHAR:
		{
			typePage = Types::T_VARCHAR;

			PageV1_1 <std::string> page;
			VirtualMemory vm;
			page.SetLenString(maxLenStr);
			file << page;
			if (!file.good())
			{
				throw std::runtime_error("Failed to write page to file: " + fullName);
			}
			if (vm.write(0, page))
			{
				stack.push_back(vm);
				return true;
			}
		}
		default:
		{
			std::cerr << "Unknown type\n";
			break; 
		}
		}
		file.close();
		return false;
	}
	template <typename T>
	void Input(size_t index, T& value);
	void Print(size_t index);
	void Exit();

	void PrintVMStack(Types type)
	{
		if (stack.size() == 0) { throw std::runtime_error("Stack is void"); }
		for (size_t i = 0; i < stack.size(); ++i)
		{
			std::cout << "VirtualMemory #" << i << "(Used size: " << stack[i].getUsedSize()
				<< ", Capacity: " << stack[i].getCapacity() << ")" << std::endl;

			switch (type)
			{
			case Types::T_INT:
			{
				PageV1_1<int> page;
				if (stack[i].read(0, page))
				{
					page.PrintPage();
				}
				else
				{
					std::cerr << "Failed to read PageV1_1<int>\n";
				}
				break;
			}
			case Types::T_CHAR:
			{
				PageV1_1<char> page;
				if (stack[i].read(0, page))
				{
					page.PrintPage();
				}
				else
				{
					std::cerr << "Failed to read PageV1_1<char>\n";
				}
				break;
			}
			case Types::T_VARCHAR:
			{
				PageV1_1<std::string> page;
				if (stack[i].read(0, page))
				{
					page.PrintPage();
				}
				else
				{
					std::cerr << "Failed to read PageV1_1<string>\n";
				}
				break;
			}
			default:
				std::cerr << "Unknown type\n";
				break;
			}
			std::cout << "------------------------------------------\n";
		}
	}
	std::vector<VirtualMemory> GetVMStack()
	{
		return stack;
	}
};