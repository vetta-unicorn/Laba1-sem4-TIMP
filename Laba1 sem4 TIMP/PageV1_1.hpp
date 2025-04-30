#pragma once

#include <vector>
#include <string>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <exception>

#define MAX_SIZE_INT 72
#define MAX_SIZE_CHAR 180
#define MAX_MEMORY_SIZE 512


//C++17 is required for compilation

#pragma warning(disable : 4996)

	template <typename T>
	struct PageV1_1
	{
	private:
	 
		int numberPage = 0;
		bool statusPage = false;

		// Длина одного элемента в elemArray
		size_t lenElemArray = 0;
		
		std::string timeModify;

		std::vector<bool> bitMap;
		std::vector<T> elemArray;

		size_t elemArraySize_t = 0;

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
						std::string_view substr_view(elemArrayStr.data() + i, page.lenElemArray);
						page.elemArray.push_back(static_cast<std::string>(substr_view));
					}
					std::vector<std::string> buffer = page.SplitString(page.elemArray, page.lenElemArray);
					page.elemArray.insert(page.elemArray.begin(), buffer.begin(), buffer.end());
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

		PageV1_1()
		{
			this->numberPage = 0;
			this->statusPage = false;
			this->lenElemArray = 0;
			if constexpr(std::is_same_v<T, int>)
			{
				this->elemArray = std::vector<int>(elemArraySize_t, 0);
			}
			else if constexpr (std::is_same_v<T, char>)
			{
				this->elemArray = std::vector<char>(elemArraySize_t, '0');
			}
			else if constexpr (std::is_same_v<T, std::string>)
			{
				this->elemArray = std::vector<std::string>(elemArraySize_t, "0");
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
	void SetElemArraySize(size_t size) { this->elemArraySize_t = size; }
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
		size_t size = 0;
		size += sizeof(numberPage);
		size += sizeof(statusPage);
		size += sizeof(lenElemArray);
		size += sizeof(size_t) + timeModify.size();
		size += sizeof(size_t) + bitMap.size() * sizeof(bool);
		size += sizeof(size_t);
		if constexpr (std::is_same_v<T, std::string>)
		{
			for (const auto& str : elemArray)
			{
				size += sizeof(size_t) + str.size();
			}
		}
		else
		{
			size = elemArray.size() * sizeof(T);
		}

		return size;
	}
	int GetLenString() { return lenElemArray; }

	std::vector<std::string> SplitString(const std::vector<std::string>& strArray, size_t maxLen)
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

	void Serialize(uint8_t* buffer) const {
		size_t offset = 0;

		// Проверка типов
		static_assert(std::is_same_v<T, std::string> || std::is_same_v<T, int> || std::is_same_v<T, char>,
			"Unsupported type for PageV1_1");

		// Сериализация numberPage
		if (offset + sizeof(numberPage) > MAX_MEMORY_SIZE) {
			throw std::runtime_error("Buffer too small for numberPage");
		}
		std::memcpy(buffer + offset, &numberPage, sizeof(numberPage));
		offset += sizeof(numberPage);

		// Сериализация statusPage
		if (offset + sizeof(statusPage) > MAX_MEMORY_SIZE) {
			throw std::runtime_error("Buffer too small for statusPage");
		}
		std::memcpy(buffer + offset, &statusPage, sizeof(statusPage));
		offset += sizeof(statusPage);

		// Сериализация lenElemArray
		if (offset + sizeof(lenElemArray) > MAX_MEMORY_SIZE) {
			throw std::runtime_error("Buffer too small for lenElemArray");
		}
		std::memcpy(buffer + offset, &lenElemArray, sizeof(lenElemArray));
		offset += sizeof(lenElemArray);

		// Сериализация timeModify
		size_t timeModifySize = timeModify.size();
		if (offset + sizeof(timeModifySize) > MAX_MEMORY_SIZE) {
			throw std::runtime_error("Buffer too small for timeModifySize");
		}
		std::memcpy(buffer + offset, &timeModifySize, sizeof(timeModifySize));
		offset += sizeof(timeModifySize);
		if (offset + timeModifySize > MAX_MEMORY_SIZE) {
			throw std::runtime_error("Buffer too small for timeModify data");
		}
		std::memcpy(buffer + offset, timeModify.data(), timeModifySize); // Исправлено: копируем timeModifySize байт
		offset += timeModifySize;

		// Сериализация bitMap
		size_t bitMapSize = bitMap.size();
		if (offset + sizeof(bitMapSize) > MAX_MEMORY_SIZE) {
			throw std::runtime_error("Buffer too small for bitMapSize");
		}
		std::memcpy(buffer + offset, &bitMapSize, sizeof(bitMapSize));
		offset += sizeof(bitMapSize);
		if (offset + bitMapSize * sizeof(bool) > MAX_MEMORY_SIZE) {
			throw std::runtime_error("Buffer too small for bitMap data");
		}
		for (bool b : bitMap) {
			std::memcpy(buffer + offset, &b, sizeof(b));
			offset += sizeof(b);
		}

		// Сериализация elemArray
		size_t elemArraySize = elemArray.size();
		if (offset + sizeof(elemArraySize) > MAX_MEMORY_SIZE) {
			throw std::runtime_error("Buffer too small for elemArraySize");
		}
		std::memcpy(buffer + offset, &elemArraySize, sizeof(elemArraySize));
		offset += sizeof(elemArraySize);
		if constexpr (std::is_same_v<T, std::string>) {
			for (const auto& str : elemArray) {
				size_t strSize = str.size();
				if (offset + sizeof(strSize) > MAX_MEMORY_SIZE) {
					throw std::runtime_error("Buffer too small for string size");
				}
				std::memcpy(buffer + offset, &strSize, sizeof(strSize));
				offset += sizeof(strSize);
				if (offset + strSize > MAX_MEMORY_SIZE) {
					throw std::runtime_error("Buffer too small for string data");
				}
				std::memcpy(buffer + offset, str.data(), strSize);
				offset += strSize;
			}
		}
		else if constexpr (std::is_same_v<T, int> || std::is_same_v<T, char>) {
			if (offset + elemArraySize * sizeof(T) > MAX_MEMORY_SIZE) {
				throw std::runtime_error("Buffer too small for elemArray data");
			}
			for (const auto& elem : elemArray) {
				std::memcpy(buffer + offset, &elem, sizeof(elem));
				offset += sizeof(elem);
			}
		}
		else {
			static_assert(false, "Unsupported type for serialization");
		}
	}
	void Deserialize(const uint8_t* buffer)
	{
		size_t offset = 0;
		
		std::memcpy(&numberPage, buffer + offset, sizeof(numberPage));
		offset += sizeof(numberPage);

		std::memcpy(&statusPage, buffer + offset, sizeof(statusPage));
		offset += sizeof(statusPage);

		std::memcpy(&lenElemArray, buffer + offset, sizeof(lenElemArray));
		offset += sizeof(lenElemArray);

		size_t timeModifySize;
		std::memcpy(&timeModifySize, buffer + offset, sizeof(timeModifySize));
		offset += sizeof(timeModifySize);
		timeModify.assign(reinterpret_cast<const char*>(buffer + offset), timeModifySize);
		offset += timeModifySize;

		size_t bitMapSize;
		std::memcpy(&bitMapSize, buffer + offset, sizeof(bitMapSize));
		offset += sizeof(bitMapSize);
		bitMap.clear();
		bitMap.reserve(bitMapSize);
		for (size_t i = 0; i < bitMapSize; ++i)
		{
			bool b; 
			std::memcpy(&b, buffer + offset, sizeof(b));
			bitMap.push_back(b);
			offset += sizeof(b);
		}

		size_t elemArraySize;
		std::memcpy(&elemArraySize, buffer + offset, sizeof(elemArraySize));
		offset += sizeof(elemArraySize);
		elemArray.clear();
		elemArray.reserve(elemArraySize);
		if constexpr (std::is_same_v<T, std::string>)
		{
			for (size_t i = 0; i < elemArraySize; ++i)
			{
				size_t strSize;
				std::memcpy(&strSize, buffer + offset, sizeof(strSize));
				offset += sizeof(strSize);
				std::string str(reinterpret_cast<const char*>(buffer + offset), strSize);
				elemArray.push_back(str);
				offset += strSize;
			
			}
		}
		if constexpr (std::is_same_v<T, int>)
		{
			for (size_t i = 0; i < elemArraySize; ++i)
			{
				int elem;
				std::memcpy(&elem, buffer + offset, sizeof(elem));
				elemArray.push_back(elem);
				offset += sizeof(elem);
			}
		}
		if constexpr (std::is_same_v<T, char>)
		{
			for (size_t i = 0; i < elemArraySize; ++i)
			{
				char elem;
				std::memcpy(&elem, buffer + offset, sizeof(elem));
				elemArray.push_back(elem);
				offset += sizeof(elem);
			}
		}
		else
		{
			static_assert(std::is_same_v<T, int> || std::is_same_v<T, char> || std::is_same_v<T, std::string>,"Unsupported type");
		}
	}
	void PrintPage()
	{
		std::cout << GetTotalSize() << " (size page in bytes)" << std::endl;
		std::cout << "Number Page: " << GetNumberPage() << std::endl;
		std::cout << "Status: " << (GetStatusPage() ? "true" : "false") << std::endl;
		std::cout << "Time create/modify: " << GetTimeModify() << std::endl;
		std::cout << "Len one elements from elemArray" << GetLenString() << std::endl;
		std::cout << "Array bitMap:";
		for (const auto& item : bitMap)
		{
			std::cout << item;
		}
		std::cout << std::endl;
		std::cout << "    " << bitMap.size() << std::endl;
		std::cout << "Array elemArray:";
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
