#include "PageV1_1.hpp"
//Tests for PageV1_1
// Тесты сериализации и десериализации
void TestPageV1_1()
{
	std::time_t times = std::time(0);

	std::vector<std::string> elem(2, "6tvufhjgbkuvtdgjhgkubuvtdhgkjbuvthgcjbhkyuvthjbgyliykuvtfjkbgyligbfuybjguitfuyjgbiutyfubhjgkunighybguifyjhvkgnyfbjvh");
	std::vector<int> elem1(72, 19876543213456789);
	std::vector<char> elem2(180, 'a');
	std::vector<char> elem3{ 'a','b','c' };
	PageV1_1<std::string> page;
	//PageV1_1<int> page;
	//PageV1_1<char> page;
	page.SetTimeModify(times);

	page.SetElemArray(elem);
	//page.SetElemArray(elem1);
	//page.SetElemArray(elem2);
	//page.SetElemArray(elem3);
	page.SetBitMap();
	page.SetLenString(2);
	auto _elemArray = page.GetElemArray();
	auto _bitMap = page.GetBitMap();

	// Запись в файл
	std::ofstream file("abi.txt", std::ios::out || std::ios::in);
	if (file.is_open())
	{
		file << page;
	}
	else
	{
		file.open("abi.txt");
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

	std::string str = _elemArray[0];
	std::cout << str.size() * sizeof(char) << "+40" << std::endl;

	file.close();
}
void TestPageV1_1_1()
{

	PageV1_1<std::string> page;


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
	std::cout << std::endl;
	std::cout << "    " << _bitMap.size() << std::endl;
	std::cout << "Массив elemArray:";
	for (const auto& item : _elemArray)
	{
		std::cout << item << ' ';
	}
	std::cout << std::endl;
	std::cout << "    " << _elemArray.size() << '\n';

	readInfo.close();
}


void TestPageV1_1_2()
{

	std::time_t times = std::time(0);

	std::vector<int> elemInt(10, 45);
	std::vector<char> elemChar(10, 'c');
	std::vector<std::string> elemStr(10, "sdhgfkshsggfjsaivjrheyygukhffouuskhjkfstgvyjdhfjhfuisghfjklfaeyisghkjfhjldaisghjfkashg");

	PageV1_1<int> pageInt;
	PageV1_1<char> pageChar;
	PageV1_1<std::string> pageStr;

	pageInt.SetElemArray(elemInt);
	pageInt.SetLenString(2);
	pageInt.SetTimeModify(times);
	pageInt.SetBitMap();

	pageChar.SetElemArray(elemChar);
	pageChar.SetLenString(3);
	pageChar.SetTimeModify(times);
	pageChar.SetBitMap();

	pageStr.SetElemArray(elemStr);
	pageStr.SetTimeModify(times);
	pageStr.SetLenString(43);
	pageStr.SetBitMap();

	std::ofstream file("pages.txt", std::ios::out || std::ios::in);
	if (file.is_open())
	{
		file << pageInt;
		file << pageChar;
		file << pageStr;
	}
	else
	{
		file.open("pages.txt");
		file << pageInt;
		file << pageChar;
		file << pageStr;
	}

	PageV1_1<std::string> page;


	std::ifstream readInfo("pages.txt");

	if (!readInfo.is_open())
	{
		std::cerr << "Can't open file" << std::endl;
	}
	readInfo >> page;
	page.PrintPage();
	readInfo >> page;
	page.PrintPage();
	readInfo >> page;
	page.PrintPage();
	//pageInt.PrintPage();
	//pageChar.PrintPage();
	//pageStr.PrintPage();

}
void TestPageV1_1_3()
{
	std::time_t times = std::time(0);

	std::vector<std::string> elem(1, "asfdgsdfasfasfdfdfuytdfgfhghjiulerdr6dtyr6tvufhjgbkuvtdgjhgkubuvtdhgkjbuvthgcjbhkyuvthjbgyliykuvtfjkbgyligbfuybjguitfuyjgbiutyfubhjgkunighybguifyjhvkgnyfbjvh");
	std::vector<int> elem1(72, 19876543213456789);
	std::vector<char> elem2(180, 'a');
	std::vector<char> elem3{ 'a','b','c' };
	PageV1_1<std::string> page;
	//PageV1_1<int> page;
	//PageV1_1<char> page;
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

	std::string str = _elemArray[0];
	std::cout << str.size() * sizeof(char) << "+40" << std::endl;
	std::cout << "Размер массива elemArray:" << sizeof(page) << std::endl;
	file.close();
}
void TestPageV1_1_4()
{
	std::vector<std::string> elem(1, "1kkdffcgfhgyhuyjiko;lmjljgtfyrdtsdfxcvghjgugutfyerdfgxcvbhjkuiguetydfhcgvhjgkugutdvhfcgvhjvybfwhgrkjfdhjgiutbrhifjhiuhtrgrkyouhwga5p9uiorhukjkhlktjlgk9pu8yjoehlkjv;okpagjoyunkrhjljikpgjoy8dtijobkp9ytfuionyfb5");
	PageV1_1<std::string> page;
	page.SetElemArray(elem);
	page.SetLenString(2);
	//auto newElem = page.EraseString(elem, 2);
	std::vector<std::string> newElem = page.SplitString(elem, 5);
	for (auto& str : newElem)
	{
		std::cout << str << ' ';
	}
	std::cout << std::endl << newElem.size() << std::endl;
}
void TestPageV1_1_5()
{
	PageV1_1<std::string> page;

	std::fstream file("asd.txt", std::ios::in | std::ios::out);

	file << page;
	page.PrintPage();
}