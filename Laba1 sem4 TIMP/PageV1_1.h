#pragma once

#include <vector>
#include <string>
#include <string_view>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <exception>
#include <algorithm>

#pragma warning(disable : 4996)

	template <typename T>
	struct PageV1_1
	{
	private:
	 
		int numberPage = 1;
		bool statusPage = false;
		size_t lenElemArray = 0;

		std::string timeModify;

		std::vector<bool> bitMap;

		std::vector<T> elemArray;

		size_t bitMapSize, elemArraySize = 0;

		friend std::ostream& operator << (std::ostream& out, const PageV1_1<T>& page)
		{
			out << page.numberPage << ' ';
			out << page.statusPage << ' ';
			out << page.lenElemArray << ' ';
			out << page.timeModify << ' ';
			for (std::vector<bool>::const_iterator it = page.bitMap.cbegin(); it != page.bitMap.cend(); ++it)
			{
				out << *it;
			}
			out << ' ';
			for (typename std::vector<T>::const_iterator it = page.elemArray.cbegin(); it != page.elemArray.cend(); ++it)
			{
				out << *it;
			}
			return out;
		}
		friend std::istream& operator >> (std::istream& in, PageV1_1<T>& page)
		{
			in.exceptions(std::ios::failbit);

			try
			{
				std::vector<std::string> elemStr;
				std::vector<int> elemInt;

				in >> page.numberPage >> page.statusPage >> page.lenElemArray;

				std::string date, time;
				in >> date >> time;
				page.timeModify = date + ' ' + time;

				std::string bitMapStr;
				in >> bitMapStr;
				
				if (bitMapStr.size() == 0)
				{
					throw std::invalid_argument("The size of the bitmap is zero!");
				}
				
				page.bitMap.clear();
				for (auto it = bitMapStr.begin(); it != bitMapStr.end(); ++it )
				{
					if 
						(*it == '0') page.bitMap.push_back(false);
					else if
						(*it == '1') page.bitMap.push_back(true);
					else 
						throw std::invalid_argument("Incorrect argument for bitMap!");
				}
				std::string elemArrayStr;
				in >> elemArrayStr;
				if (elemArrayStr.length() == 0)
				{
					throw std::invalid_argument("The size of the elemArray is zero!");
				}
				page.elemArray.clear();

				if constexpr (std::is_same_v<T, char>)
				{
					for (char c : elemArrayStr)
					{
						page.elemArray.push_back(c);
					}
				}
				else if constexpr(std::is_same_v<T, std::string>)
				{
					if (page.lenElemArray == 0) { throw std::invalid_argument("lenElemArray is zero!"); }

					// Извлечение подстрок с шагом 1
					for (size_t i = 0; i < elemArrayStr.size(); i+= page.lenElemArray)
					{
						std::string_view substr_view(elemArrayStr.data(), page.lenElemArray);
						page.elemArray.push_back(static_cast<std::string>(substr_view));
					}
				}
				else if constexpr(std::is_same_v<T, int>)
				{
					if (page.lenElemArray == 0) { throw std::invalid_argument("lenElemArray is zero!"); }
					for (size_t i = 0; i < elemArrayStr.size(); i += page.lenElemArray)
					{
						std::string elem = elemArrayStr.substr(i, page.lenElemArray);
						page.elemArray.push_back(std::stoi(elem));
					}
				}
			}
			catch (const std::ios_base::failure& ex)
			{
				std::cerr << "Input error: " << ex.what() << std::endl;
				in.clear();
				return in;
			}
			catch (std::invalid_argument& ex)
			{
				std::cerr << "Invaild argument: " << ex.what() << std::endl;
				in.clear();
				return in;
			}
			return in;
		}
	

	public:

		PageV1_1(int _numberPage, bool _statusPage)
		{
			this->numberPage = _numberPage;
			this->statusPage = _statusPage;
		}

	// Setters
	void SetTimeModify(std::time_t time)
	{
		char buffer[20];
		if (time == 0)
		{
			timeModify = "00.00.00 00:00:00";
		}
		std::tm* localTime = std::localtime(&time);
		if (localTime == nullptr)
		{
			timeModify = "Error time";
		}
		std::strftime(buffer, sizeof(buffer), "%d.%m.%y %H:%M:%S", localTime);
		timeModify = static_cast<std::string>(buffer);
	}
	void SetElemArray(std::vector<T> _elemArray)
	{
		size_t elemSize = _elemArray.size();
		//elemArray.resize(_elemArray.size());
		//elemArray.resize(elemSize);
		if (_elemArray.size() == 0)
		{
			std::exception("Error");
		}
		//elemArray.insert(elemArray.cbegin(), _elemArray.cbegin(), _elemArray.cend());
		for (typename std::vector<T>::iterator it = _elemArray.begin(); it != _elemArray.end(); ++it)
		{
			elemArray.push_back(*it);
		}
	}
	void SetBitMap()
	{
		size_t elemSize = elemArray.size();
		if (elemArray.size() == 0)
		{
			std::exception("Can't writing because elemSize = 0");
		}
		else
		{
			for (typename std::vector<T>::iterator it = elemArray.begin(); it != elemArray.end(); ++it)
			{
				if constexpr (std::is_same_v<T, std::string>)
				{
					if (*it != "")
					{
						bitMap.push_back(true);
					}
					else
					{
						bitMap.push_back(false);
					}
				}
				else if constexpr (std::is_same_v<T, int>)
				{
					if (*it != 0)
					{
						bitMap.push_back(true);
					}
					else
					{
						bitMap.push_back(false);
					}
				}
				else if constexpr (std::is_same_v<T, char>)
				{
					if (*it != '\0')
					{
						bitMap.push_back(true);
					}
					else
					{
						bitMap.push_back(false);
					}
				}
				else
				{
					static_assert(std::is_same_v<T, std::string> ||
						std::is_same_v<T, char> ||
						std::is_same_v<T, int>,
						"Unsupported type for this function!");
				}
			}
		}
		bitMap.resize(elemSize);
	}
	void SetLenString(int lenSize)
	{
		this->lenElemArray = lenSize;
	}

	// Getters
	std::string GetTimeModify()
	{
		if (timeModify == "") return "00.00.00 00:00:00";
		return timeModify;
	}
	std::vector<T> GetElemArray() { return elemArray; }
	int GetNumberPage() { return numberPage; }
	std::vector<bool> GetBitMap() { return bitMap; }
	bool GetStatusPage() { return statusPage; }
	size_t GetTotalSize() const 
	{
		size_t baseSize = sizeof(PageV1_1<T>);
		size_t totalbitMapSize = bitMap.size() * sizeof(bool);
		size_t totalElemArraySize = 0;

		if constexpr (std::is_same_v<T, std::string>)
		{
			for (const auto& str : elemArray)
			{
				totalElemArraySize += sizeof(std::string) + str.size() * sizeof(char);
			}
		}
		else
		{
			totalElemArraySize = elemArray.size() * sizeof(T);
		}

		size_t totalSize = baseSize + totalbitMapSize + totalElemArraySize;

		return totalSize;
	}
	int GetLenString() { return lenElemArray; }

	~PageV1_1()
	{
		bitMap.clear();
		elemArray.clear();
	}
};

// Тесты сериализации и десериализации
int TestPageV1_1()
{



	std::time_t times = std::time(0);

	std::vector<std::string> elem(100, "123");
	std::vector<int> elem1(200, 12);
	std::vector<char> elem2(200, 'a');
	std::vector<char> elem3{ 'a','b','c' };
	PageV1_1<std::string> page(1, false);
	//PageV1_1<int> page(1,false);
	//PageV1_1<char> page(1, false);
	page.SetTimeModify(times);

	page.SetElemArray(elem);
	//page.SetElemArray(elem1);
	//page.SetElemArray(elem2);
	//page.SetElemArray(elem3);
	page.SetBitMap();
	page.SetLenString(3);
	auto _elemArray = page.GetElemArray();
	auto _bitMap = page.GetBitMap();

	// Запись в файл
	std::ofstream file("abibasss.txt", std::ios::out || std::ios::in);
	if (file.is_open())
	{
		file << page;
	}
	else
	{
		file.open("abibadadas.txt");
		file << page;
	}
	std::cout << "Запись в файл .txt" << std::endl;
	std::cout << page.GetTotalSize() << " (размер страницы в байтах)" << std::endl;
	std::cout << "Номер страницы: " << page.GetNumberPage() << std::endl;
	std::cout << "Статус: " << (page.GetStatusPage() ? "true" : "false") << std::endl;
	std::cout << "Время создания/изменения: " << page.GetTimeModify() << std::endl;
	std::cout << "Размер одного элемента elemArray: " << page.GetLenString() << std::endl;
	std::cout << "Массив bitMap:";
	for (const auto& item : _bitMap)
	{
		std::cout << item;
	}
	std::cout << "    " << _bitMap.size() << std::endl;
	std::cout << "Массив elemArray:";
	for (const auto& item : _elemArray)
	{
		std::cout << item;
	}
	std::cout << "    " << _elemArray.size() << '\n';

	file.close();
	return 0;
}
int TestPageV1_1_1()
{

	PageV1_1<std::string> page(1, false);


	std::ifstream readInfo("abibadadas.txt");

	if (!readInfo.is_open())
	{
		std::cerr << "Can't open file" << std::endl;
	}
	readInfo >> page;
	auto _bitMap = page.GetBitMap();
	auto _elemArray = page.GetElemArray();
	//page.SetLenString(1);
	std::cout << "Чтение из файла" << std::endl;
	std::cout << page.GetTotalSize() << " (размер страницы в байтах)" << std::endl;
	std::cout << "Номер страницы: " << page.GetNumberPage() << std::endl;
	std::cout << "Статус: " << (page.GetStatusPage() ? "true" : "false") << std::endl;
	std::cout << "Время создания/изменения: " << page.GetTimeModify() << std::endl;
	std::cout << "Длина одного элемента из elemArray: " << page.GetLenString() << std::endl;
	std::cout << "Массив bitMap:";
	for (const auto& item : _bitMap)
	{
		std::cout << item;
	}
	//std::cout << std::endl;
	std::cout << "    " << _bitMap.size() << std::endl;
	std::cout << "Массив elemArray:";
	for (const auto& item : _elemArray)
	{
		std::cout << item;
	}
	//std::cout << std::endl;
	std::cout << "    " << _elemArray.size() << '\n';

	readInfo.close();
	return 0;
}
