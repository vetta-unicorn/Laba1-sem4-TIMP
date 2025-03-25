#pragma once
#include <iostream>
#include <fstream>
#include <string>

#pragma warning(disable : 4996)

class HelperClass
{
private:
	size_t elementSize;
	size_t totalElements;
public:
	void CreateFileH(std::string fileName);
	void WriteInFile(std::string fileName, const char* writingInfo []);
	int ReturnSizeFile(std::string fileName); // Return size file in bytes
	void WriteByPosition(std::string fileName, const char* writeInfo [], int pos);

	/// <param name="elemSize"> 
	/// - Передавать sizeof(type), где type - это тип элемента</param>
	/// <returns></returns>
	size_t ReturnCountElements(const std::string fileName, size_t elemSize);
};
