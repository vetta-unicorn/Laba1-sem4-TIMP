#include "ControlHeaders.hpp"

#include <variant>
using VariantType = std::variant<int, char, std::string>;

enum class Types {T_INT, T_CHAR, T_VARCHAR};

class UserCommands
{
private:
	
	std::vector<VirtualMemory> stack;
	size_t globalIndex;
	template <typename T>
	size_t getGlobalIndex(std::vector<VirtualMemory> vec)
	{
		if (vec.empty()) { throw std::exception("vec is void"); }
		globalIndex = 0;
		for (size_t i = 0; i < vec.size(); ++i)
		{
			PageV1_1<T> page;
			if (std::is_same_v<PageV1_1<T>, PageV1_1<char>>)
			{
				if (vec[i].read(0, page))
				{
					globalIndex += page.GetElemArray().size();
				}
			}
			else if (std::is_same_v<PageV1_1<T>, PageV1_1<int>>)
			{
				if (vec[i].read(0, page))
				{
					globalIndex += page.GetElemArray().size();
				}
			}
			else if (std::is_same_v<PageV1_1<T>, PageV1_1<std::string>>)
			{
				if (vec[i].read(0, page))
				{
					globalIndex += page.GetElemArray().size();
				}
			}
			else
			{
				std::cerr << "Failed to read PageV1_1<T> at index: " << i << std::endl;
			}
		}
		return globalIndex;
	}
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
		}
		case Types::T_VARCHAR:
		{
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
	void Input(size_t index, const T value)
	{
		// Проверка пустого стека
		if (stack.empty()) {
			throw std::runtime_error("Stack is empty");
		}

		// Проверка корректности индекса
		size_t totalElem = getGlobalIndex<T>(stack);
		if (index >= totalElem) {
			throw std::out_of_range("Index out of range: " + std::to_string(index) +
				", total elements: " + std::to_string(totalElem));
		}

		size_t currentIndex = 0;
		for (size_t i = 0; i < stack.size(); ++i) {
			PageV1_1<T> page;
			// Чтение страницы
			if (!stack[i].read(0, page)) {
				std::cerr << "Failed to read PageV1_1<T> at stack index: " << i << std::endl;
				throw std::runtime_error("Failed to read page at stack index: " + std::to_string(i));
			}

			// Проверка размера страницы
			auto elemArray = page.GetElemArray();
			auto bitMap = page.GetBitMap();
			size_t pageSize = elemArray.size();
			if (pageSize != bitMap.size()) {
				throw std::runtime_error("Inconsistent page size: elemArray size = " +
					std::to_string(pageSize) + ", bitMap size = " +
					std::to_string(bitMap.size()));
			}

			// Проверка, попадает ли index в текущую страницу
			if (index < currentIndex + pageSize) {
				size_t localIndex = index - currentIndex;

				// Проверка корректности localIndex
				if (localIndex >= pageSize) {
					throw std::runtime_error("Invalid local index: " + std::to_string(localIndex) +
						", page size: " + std::to_string(pageSize));
				}

				// Обновление данных страницы
				elemArray[localIndex] = value;
				bitMap[localIndex] = true;
				page.SetStatusPage(true);
				page.SetTimeModify(std::time(nullptr));

				// Запись страницы
				if (!stack[i].write(0, page)) {
					throw std::runtime_error("Failed to write page back to stack at index: " +
						std::to_string(i));
				}
				return;
			}
			currentIndex += pageSize;
		}

		// Если страница не найдена
		throw std::runtime_error("Could not find page for index: " + std::to_string(index));
	}
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
	bool stringToInt(const std::string& str, int& result)
	{
		try
		{
			size_t pos;
			result = std::stoi(str, &pos);
			return pos == str.length();
		}
		catch (...)
		{
			return false;
		}
	}
	bool stringToChar(const std::string& str, char& result)
	{
		if (str.length() == 0)
		{
			result = str[0];
			return true;
		}
		return false;
	}
	std::vector<VirtualMemory> GetVMStack()
	{
		return stack;
	}
	~UserCommands()
	{
		stack.clear();
	}
};