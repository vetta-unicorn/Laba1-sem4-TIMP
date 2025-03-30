//#include "PageV1.cpp"
//#include <unordered_map>
//
//template <typename T>
//class BookV1
//{
//private:
//	std::unordered_map<size_t, Page<T>> book;
//	Page bufferPage;
//public:
//	void CreateBookFile(std::string fileName)
//	{
//		if (!(std::filesystem::exists(fileName)))
//		{
//			std::ofstream file(fileName);
//			file.close;
//		}
//	}
//	void WritePageInFile(std::string fileName, Page<T>& page)
//	{
//		if ((std::filesystem::exists(fileName)))
//		{
//			std::ofstream file(fileName, std::ios::in);
//			file << page;
//			file.close();
//		}
//	}
//	void OverwritingPage(std::string fileName, Page<T>& page)
//	{
//		page.statusPage = true;
//		page.timeModify = page.getFormattedTime();
//		WritePageInFile(fileName, page);
//	}
//	void CreateNewPage(const Page<T>& page)
//	{
//		if (page.getTotalSize() % 512))
//		{
//			book [page.numberPage + 1] = page;
//		}
//		else {}
//	}
//	void AddToBook(const Page<T>& page)
//	{
//		book[page.numberPage] = page;
//	}
//};
