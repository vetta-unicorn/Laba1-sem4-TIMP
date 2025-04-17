//#include "PageV1_1.h"
#include "PageV1_1.h"
#include <unordered_map>
#include <windows.h>
template <typename T>
class BookV1
{
private:
	//std::unordered_map<size_t, PageV1_1<T>> book;
	//PageV1_1<T> bufferpage;

	int lenString;
public:
	BookV1(int _lenString = 0)
	{
		this->lenString = _lenString;
	}
	std::vector<PageV1_1<T>> ReadInfo(const std::string& _fileName)
	{
		std::ifstream readInfo(_fileName);
		if (!readInfo.is_open())
		{
			std::cerr << "Cannot open file: " << _fileName << std::endl;
			return std::vector<PageV1_1<T>>();
		}

		std::vector<PageV1_1<T>> pages;
		while(true)
		{
			int numberPage;
			bool statusPage;
			std::string date, time1, time2, time3;

			readInfo >> numberPage >> statusPage >> date >> time1 >> time2 >> time3;
			if (readInfo.fail())
			{
				if (readInfo.eof()) break;
				std::cerr << "" << _fileName << std::endl;
				break;
			}
			std::string timeModify = date + " " + time1 + " " + time2 + " " + time3 + " ";


			if constexpr (std::is_same_v<T, int>)
			{
				
			}
		}
	}
	std::vector<PageV1_1<T>> WriteInfo(std::string _fileName)
	{

	}
};
int TestBookV1()
{
	return 0;
}
int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	TestPageV1_1();
	TestPageV1_1_1();
	return 0;
}