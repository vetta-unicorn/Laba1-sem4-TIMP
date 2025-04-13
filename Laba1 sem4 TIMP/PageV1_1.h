#pragma once

#include <vector>
#include <string>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <exception>

#pragma warning(disable : 4996)

template <typename T>
struct PageV1_1
{
private:
	 
	int numberPage = 001;
	bool statusPage = false;

	std::string timeModify;

	std::vector<bool> bitMap;

	std::vector<T> elemArray;

	size_t bitMapSize, elemArraySize = 0;

	friend std::ostream& operator << (std::ostream& out, const PageV1_1<T>& page)
	{
		out << page.numberPage << ' ';
		out << page.statusPage << ' ';
		out << page.timeModify << ' ';
		for (std::vector<bool>::const_iterator it = page.bitMap.cbegin(); it != page.bitMap.cend(); ++it)
		{
			out << *it;
		}
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
			in >> page.numberPage >> page.statusPage >> page.timeModify;

			for (std::vector<bool>::iterator it = page.bitMap.begin(); it != page.bitMap.end(); ++it)
			{
				in >> *it;
			}
			for (typename std::vector<T>::iterator it = page.elemArray.begin(); it != page.elemArray.end(); ++it)
			{
				in >> *it;
			}
		}
		catch (const std::ios_base::failure& ex)
		{
			std::cerr << "Input error: " << ex.what() << std::endl;
			return in;
		}

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
		std::strftime(buffer, sizeof(buffer), "%d.%m.%y %H %M %S", localTime);
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
};


