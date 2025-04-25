#pragma once

#include <vector>
#include <string>
#include <string_view>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <exception>
#include <algorithm>

#define MAX_SIZE_INT 72
#define MAX_SIZE_CHAR 180

#pragma warning(disable : 4996)

	template <typename T>
	struct PageV1_1
	{
	private:
	 
		int numberPage = 0;
		bool statusPage = false;
		size_t lenElemArray = 0;
		size_t maxLen = 0;

		std::string timeModify;

		std::vector<bool> bitMap;

		std::vector<T> elemArray;

		size_t bitMapSize = 0, elemArraySize = 0;

		friend std::ostream& operator << (std::ostream& out, const PageV1_1<T>& page)
		{
	
			out << page.numberPage << ' ';
			out << page.statusPage << ' ';
			out << page.lenElemArray << ' ';
			out << page.maxLen << ' ';
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
			out << ' ';
			return out;
		}
		friend std::istream& operator >> (std::istream& in, PageV1_1<T>& page)
		{
			in.exceptions(std::ios::failbit);

			try
			{
				std::vector<std::string> elemStr;
				std::vector<int> elemInt;

				in >> page.numberPage >> page.statusPage >> page.lenElemArray >> page.maxLen;

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
					page.lenElemArray = 1;
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
		PageV1_1()
		{
			this->numberPage = 0;
			this->statusPage = false;
			this->lenElemArray = 0;
			if constexpr(std::is_same_v<T, int>)
			{
				this->elemArray = std::vector<int>(elemArraySize, 0);
			}
			else if constexpr (std::is_same_v<T, char>)
			{
				this->elemArray = std::vector<char>(elemArraySize, '0');
			}
			else if constexpr (std::is_same_v<T, std::string>)
			{
				this->elemArray = std::vector<std::string>(elemArraySize, "0");
			}
			else 
			{
				static_assert(std::is_same_v<T,int> || std::is_same_v<T, char> || std::is_same_v<T, std::string>,"Неподдерживаемый тип структуры");
			}
			SetBitMap();
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
	void SetElemArraySize(size_t size) { this->elemArraySize = size; }
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


	std::vector<std::string> EraseString(const std::vector<std::string>& strArray, size_t maxLen)
	{
		std::vector<std::string> result;
		for (auto& str : strArray)
		{
			for (size_t i = 0; i < str.length(); i += maxLen)
			{
				result.push_back(str.substr(i, maxLen));
			}
		}
		return result;
	}

	void PrintPage()
	{
		std::cout << GetTotalSize() << " (размер страницы в битах)" << std::endl;
		std::cout << "Номер страницы: " << GetNumberPage() << std::endl;
		std::cout << "Статус: " << (GetStatusPage() ? "true" : "false") << std::endl;
		std::cout << "Время создания/изменения: " << GetTimeModify() << std::endl;
		std::cout << "Длина одного элемента из elemArray: " << GetLenString() << std::endl;
		std::cout << "Массив bitMap:";
		for (const auto& item : bitMap)
		{
			std::cout << item;
		}
		std::cout << std::endl;
		std::cout << "    " << bitMap.size() << std::endl;
		std::cout << "Массив elemArray:";
		for (const auto& item : elemArray)
		{
			std::cout << item;
		}
		std::cout << std::endl;
		std::cout << "    " << elemArray.size() << '\n';
	}
	~PageV1_1()
	{
		bitMap.clear();
		elemArray.clear();
	}
};

