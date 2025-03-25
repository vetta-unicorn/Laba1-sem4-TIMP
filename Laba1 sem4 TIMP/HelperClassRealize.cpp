#include "HelperClass.h"


void HelperClass::CreateFileH(std::string fileName)
{
	std::ofstream out;
	out.open(fileName);
	out.close();
}
void HelperClass::WriteInFile(std::string fileName, const char* writeInfo[])
{
	std::ofstream out;
	int lenght = 0;

	lenght = sizeof(writeInfo);
//	std::cout << lenght << std::endl;

	out.open(fileName, std::ios::in);
	
	if (writeInfo == NULL)
	{
		throw std::invalid_argument("Argument cannot equals NULL!");
	}
	else 
	{
		for (int i = 0; i < lenght; i++)
		{
			out << writeInfo[i];
		}
		out << std::endl;
	}
	out.close();
}

void HelperClass::WriteByPosition(std::string fileName, const char* writeInfo[], int pos)
{
	std::ofstream out;
	int lenght = 0;

	lenght = sizeof(writeInfo);
	//  std::cout << lenght << std::endl;
	out.open(fileName, std::ios::in | std::ios::out);

	if (writeInfo == NULL)
	{
		throw std::invalid_argument("Argument cannot equals NULL!");
	}
	if (out.is_open())
	{
		out.seekp(pos);
		for (int i = 0; i < lenght; i++)
		{
			out << writeInfo[i];
		}
		out << std::endl;
	}
	else
	{
		throw ("Cannot open file!");
	}
	out.close();

}
int HelperClass::ReturnSizeFile(std::string fileName)
{
	std::ifstream fs(fileName, std::ios::binary | std::ios::ate);
	return fs.tellg() * 8;
}
size_t HelperClass::ReturnCountElements(std::string fileName, size_t elemSize)
{
	std::fstream file;
	file.open(fileName, std::ios::in | std::ios::out | std::ios::binary);

	if (!file.is_open())
	{
		throw std::runtime_error("Неудалось открыть файл!");
	}
	file.seekg(0, std::ios::end);
	totalElements = file.tellg() / elemSize;
	file.seekg(0, std::ios::beg);
	file.close();
}
//int main()
//{
//	MyClass dada;
//
//	const char* writeInfo[10] = {"12", "12", "12", "12", "12", "12", "12", "12", "12", "12"};
//	
//	dada.CreateFile("dada1.txt");
//
//	dada.WriteInFile("dada1.txt", writeInfo);
//
//	int size = dada.ReturnSizeFile("dada1.txt");
//	std::cout << size << std::endl;
//	
//	return 0;
//}