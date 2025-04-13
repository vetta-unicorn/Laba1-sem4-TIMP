//#include "PageV1_1.h"
#include "PageV1_1.cpp"
#include <unordered_map>
#include <windows.h>
template <typename T>
class BookV1
{
private:
	std::unordered_map<size_t, PageV1_1<T>> book;
	PageV1_1<T> bufferpage;
public:
	void ReadInfo(std::string _fileName)
	{
		std::ifstream readInfo(_fileName, std::ios::out);
		if (readInfo.is_open())
		{
			std::find();
		}
	}
};
int main()
{


	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);


	std::time_t times = std::time(0);

	std::vector<std::string> elem(200, "12");
	std::vector<int> elem1(200, 12);
	std::vector<char> elem2(200, '0x21');
	
	//PageV1_1<std::string> page(1, false);
	//PageV1_1<int> page(1,false);
	PageV1_1<char> page(1, false);
	page.SetTimeModify(times);

	//page.SetElemArray(elem);
	//page.SetElemArray(elem1);
	page.SetElemArray(elem2);
	page.SetBitMap();

	auto _elemArray = page.GetElemArray();
	auto _bitMap = page.GetBitMap();

	// Запись в файл
	std::ofstream file("abibas.txt", std::ios::out || std::ios::in);
	if (file.is_open())
	{
		file << page;
	}
	else
	{
		file.open("abibas.txt");
		file << page;
	}
	std::cout << page.GetTotalSize() << " (размер страницы в байтах)" << std::endl;
	std::cout << "Номер страницы: " << page.GetNumberPage() << std::endl;
	std::cout << "Статус: " << (page.GetStatusPage() ? "true" : "false") << std::endl;
	std::cout << "Время создания/изменения: " << page.GetTimeModify() << std::endl;
	std::cout << "Массив bitMap:";
	for (const auto& item : _bitMap)
	{
		std::cout << item;
	}
	std::cout <<"    " << _bitMap.size() << std::endl;
	std::cout << "Массив elemArray:";
	for (const auto& item : _elemArray)
	{
		std::cout << item;
	}
	std::cout << "    " << _elemArray.size() << '\n';
	
	file.close();
	return 0;
}
