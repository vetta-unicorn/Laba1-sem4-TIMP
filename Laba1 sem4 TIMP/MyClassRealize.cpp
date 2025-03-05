#include "MyClassH.h"


void MyClass::CreateFile(std::string fileName)
{
	std::ofstream out;
	out.open(fileName);
	out.close();
}
void MyClass::WriteInFile(std::string fileName, const char* writeInfo[]) 
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

void MyClass::WriteByPosition(std::string fileName, const char* writeInfo[], int pos)
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

int MyClass::ReturnSizeFile(std::string fileName)
{
	std::ifstream fs(fileName, std::ios::binary | std::ios::ate);
	return fs.tellg() * 8;
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