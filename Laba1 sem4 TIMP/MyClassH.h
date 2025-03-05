#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <string>

class MyClass
{
public:
	void CreateFile(std::string fileName);
	void WriteInFile(std::string fileName, const char* writingInfo[]);
	int ReturnSizeFile(std::string fileName); // Return size file in bytes
	void WriteByPosition(std::string fileName, const char* writeInfo[], int pos);

};