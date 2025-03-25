#include "HelperClass.h"
#include <vector>
#include <filesystem>
#include <windows.h>

#define MAX_SIZE_ARRAY 128
#define DEBUG
enum TypeArray
{
	T_INT,
	T_CHAR,
	T_VARCHAR
};

template <typename T>
struct Page
{


    int numberPage = 0;

    bool statusPage = false; // true - �������� ��������������, 
                             //false - �������� �� ��������������
    std::time_t timeModify = 0;

     
     //��������� � ������� �����//
    //������ �������� ������������ ������� �����(������ �����),
    //� ������� ������ ��� ������������� ������ �������������
    //�������, ����������� �� ��������. �������� ���� 0 ��������, ��� � ��� ������ ������ ����������
    std::vector<T> byteMap;
    size_t byteMapSize = MAX_SIZE_ARRAY;

    //������ ���������
    std::vector<T> elemArray;
    size_t elemArraySize = MAX_SIZE_ARRAY;


    /// <summary>
    /// ��������������� ��������� "<<" ��� �������� ������ ��� 
    /// ������� ��������� � ����
    /// </summary>
    static friend std::ofstream& operator << (std::ofstream& os, const Page<T>& page)
    {
        for (int i = 0; i < page.byteMapSize; i++)
        {
            os << page.byteMap[i];
        }
        for (int i = 0; i < page.elemArraySize; i++)
        {
            os << page.elemArray[i];
        }
        os << page.numberPage;
        os << page.statusPage;
        os << page.timeModify;
        return os;
    }
    Page(int _numberPage, bool _statusModify)
    {
        this->numberPage = _numberPage;
        this->statusPage = _statusModify;

    }
    ~Page(){    }
    // ���������� ������������ ������ ������ �������� ����� ���������� ��������� ��������� �������
    // ������: ������� ������������ ������ �������� 20, �� � ���� �� ������ ������ 10 ���������
    // ������� ����� ���-�� ������, �� ������ ��� ����� ����� ���-�� ���������, �.� ����� 10.
    /*T* resizeArray(T* array)
    {
        size_t sizeArray = 0;
        for (int i = 0; i < MAX_SIZE_ARRAY; i++)
        {
            if (sizeof(array [i]) != 0)
            {
                sizeArray += 1;
            }
            else { break; }
        }
        
        
        T* copyArray = new T [sizeArray];
        for (int i = 0; i < sizeArray; i++)
        {
            copyArray [i] = array [i];
        }
        array = copyArray;

#ifdef DEBUG
        std::cout << sizeArray << std::endl;
        std::cout << "������ � ��������� ��������" << std::endl;
        for (int i = 0; i < sizeArray; i++)
        {
            std::cout << array [i];
        }
        std::cout << std::endl;
#endif
        delete [] copyArray;
        return array;
    }*/
    void AddElemInArray(std::vector<T> _elemArray)
    {
        if (elemArray != NULL)
        {
            elemArray = _elemArray;
        }
        else
        {
            throw std::exception("");
        }
    }
    void PrintElemArray()
    {
        for (int i = 0; i < MAX_SIZE_ARRAY; i++)
        {
            std::cout << elemArray[i];
        }
    }
    size_t getTotalSize()
    {
        size_t totalSizeByteMap = 0;
        size_t totalSizeElemArray = 0;
        
        for (int i = 0; i < MAX_SIZE_ARRAY; i++)
        {
            totalSizeByteMap += sizeof(byteMap[i]);
            totalSizeElemArray += sizeof(elemArray[i]);
        }

        return sizeof(Page<T>) + totalSizeByteMap;// +totalSizeElemArray;
    }

    /// <summary>
    /// ���������� ���� ������� ��:��:�� ��:��:��
    /// </summary>
    std::string getFormattedTime() const
    {
        if (timeModify == 0)
        {
            return "00.00.00 00:00:00"; // ���� ����� �� ������
        }
        std::tm* localTime = std::localtime(&timeModify); // ����������� time_t � tm
        if (localTime == nullptr)
        {
            return "������ �������";
        }
        char buffer [20]; // ���������� ��� "��:��:�� ��:��:��"
        std::strftime(buffer, sizeof(buffer), "%d.%m.%y %H:%M:%S", localTime);
        return std::string(buffer);
    }
};
//template <typename T>
//class WorkerClass
//{
//private:
//
//	HelperClass help;
//	Page<T> bufferPage;
//
//    std::vector<Page<T>> dataBlocks;
//
//	TypeArray typeArray;
//	int sizeArray;
//	int lenString = 0;
//
//    // ���� �� �������, �.� �� �������, ���������� �� ��� ��� ���!
//	/*T WriteInFile(std::string _fileName, size_t _sizeArray,
//				  TypeArray _typeArray, size_t _lenArray)
//	{
//		std::ofstream fileOut(_fileName, std::ios::in | std::ios::out);
//		if (!fileOut.is_open())
//		{
//			help.CreateFileH(_fileName);
//			bufferPage = Page<T>{};
//		}
//		if (_typeArray == T_INT)
//		{
//			bufferPage = Page<int>();
//            bufferPage.SetArraySize(_sizeArray);
//
//
//		}
//		if (_typeArray == T_CHAR)
//		{
//			bufferPage = Page<char>();
//		}
//        if (_typeArray == T_VARCHAR)
//        {
//            bufferPage = Page<std::string>();
//        }
//	}*/
//
//public:
//	WorkerClass(std::string _fileName, size_t _sizeArray, 
//				TypeArray _typeArray, size_t _lenArray = 0)
//	{
//		
//		this->lenString = _lenArray;
//		this->sizeArray = _sizeArray;
//		this->typeArray = _typeArray;
//        
//		if (!(std::filesystem::exists(_fileName)))
//		{
//            help.CreateFileH(_fileName);
//            std::ofstream fileOut(_fileName, std::ios::in | std::ios::out);
//            fileOut << bufferPage;
//            fileOut.close();
//		}
//		
//	}
//	T WriteInFile(std::string& _fileName, const Page<T>& page)
//	{
//		std::ofstream fileOut(_fileName, std::ios::in | std::ios::out);
//
//		if (!fileOut.is_open())
//		{
//			throw std::exception("Cannot open file!");
//		}
//
//		if (page == NULL)
//		{
//			throw std::invalid_argument("Argument cannot equals NULL!");
//		}
//		else
//		{
//			fileOut << page;
//		}
//		return 0;
//	}
//	T WriteInFile(std::string fileName)
//	{
//
//	}
//
//    T CreateNewPage()
//    {
//
//    }
//};


class adad
{
public:
    adad() {}
    void TestStruct()
    {
        SetConsoleCP(1251);
        SetConsoleOutputCP(1251);

        Page<char> page(1, false);
        std::time_t times = std::time(0);


        const std::vector<char> elem{ '1','2','3' };
        const std::vector<byte> temp{ 1,0 };

        for (auto i = 0; i < elem.begin(); i++)
        {
            std::cout << elem[i];
        }
        for (auto i = 0; i < temp.begin(); i++)
        {
            std::cout << temp [i];
        }
        page.timeModify = times;


        // ������ � ����
        std::ofstream file("adada.txt", std::ios::out);
        if (file.is_open())
        {
            file << page;
            file.close();
        }
        else
        {
            std::cout << "�� ������� ������� ����\n";
        }

        std::cout << page.getTotalSize() << " (������ �������� � ������)" << std::endl;
        std::cout << "����� ��������: " << page.numberPage << std::endl;
        std::cout << "������: " << (page.statusPage ? "true" : "false") << std::endl;
        std::cout << "����� ��������/���������" << page.getFormattedTime() << std::endl;
        std::cout << "������ byteMap: ";
        for (int i = 0; i < page.byteMapSize; i++)
        {
            std::cout << page.byteMap [i];
        }

        std::cout << std::endl;
        for (int i = 0; i < page.elemArraySize; i++)
        {

        }
        std::cout << std::endl;
        page.PrintElemArray();
    }
};
void TestClass(){}
int main()
{
    adad d;
    d.TestStruct();
    return 0;
}