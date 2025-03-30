#include <iostream>
#include <vector>
#include <windows.h>
#include <string>
#include <fstream>
#include <filesystem>

#define MAX_SIZE_ARRAY 128

#pragma warning(disable : 4996)

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
    std::vector<byte> byteMap;
    size_t byteMapSize = MAX_SIZE_ARRAY;

    //������ ���������
    std::vector<T> elemArray;
    size_t elemArraySize = MAX_SIZE_ARRAY;

    std::vector<T> bufferElem;
    /// <summary>
    /// ��������������� ��������� "<<" ��� �������� ������ ��� 
    /// ������� ��������� � ����
    /// </summary>
    friend std::ofstream& operator << (std::ofstream& os, const Page<T>& page)
    {
        for (std::vector<byte>::const_iterator it = page.byteMap.cbegin(); it != page.byteMap.cend(); ++it)
        {
            os << *it;
        }
        for (typename std::vector<T>::const_iterator it = page.elemArray.cbegin(); it != page.elemArray.cend(); ++it)
        {
            os << *it;
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
    Page(int _numberPage, bool _statusModify, const std::vector<byte> _byteMap, const std::vector<T> _elemArray)
    {
        this->numberPage = _numberPage;
        this->statusPage = _statusModify;
        AddByteMap(_byteMap);
        AddElements(_elemArray);
    }
    Page(int _numberPage, bool _statusModify, const std::vector<T> _elemArray)
    {
        this->numberPage = _numberPage;
        this->statusPage = _statusModify;
        AddElements(_elemArray);
        OverwritingBytemap();
    }
    void SetVectorSize()
    {
        byteMap.resize(byteMapSize);
        elemArray.resize(elemArraySize);
    }
    void OverwritingBytemap()
    {
        byteMap.clear();
        for (typename std::vector<T>::iterator it1 = elemArray.begin(); it1 != elemArray.end(); ++it1)
        {
            if constexpr (std::is_same_v<T, int>)
            {
                if (*it1 != 0)
                {
                    byteMap.push_back(1);
                }
                else
                {
                    byteMap.push_back(0);
                }
            }
            else if constexpr (std::is_same_v<T, std::string>)
            {
                if (*it1 != "")
                {
                    byteMap.push_back(1);
                }
                else
                {
                    byteMap.push_back(0);
                }
            }
            else if constexpr (std::is_same_v<T, char>)
            {
                if (*it1 != '\0')
                {
                    byteMap.push_back(1);
                }
                else
                {
                    byteMap.push_back(0);
                }
            }
            else
            {
                static_assert(std::is_same_v<T, int> || std::is_same_v<T, std::string> || std::is_same_v<T, char>, "Unsupported type for OverwritingBytemap");
            }
            
        }
    }
    void AddByteMap(const std::vector<byte>& vec)
    {
        byteMap.insert(byteMap.end(), vec.begin(), vec.end());
    }
    void AddElements (const std::vector<T>& vec)
    {
        elemArray.insert(elemArray.end(), vec.begin(), vec.end());
    }
    void PrintByteMap()
    {
        for (std::vector<byte>::iterator it = byteMap.begin(); it != byteMap.end(); ++it)
        {
            std::cout << (int)*it;
        }
        std::cout << std::endl;
    }
    void PrintElemArray()
    {
        for (typename std::vector<T>::iterator it = elemArray.begin(); it != elemArray.end(); ++it)
        {
            std::cout << *it;
        }
        std::cout << std::endl;
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

        return sizeof(Page<T>) + totalSizeByteMap + totalSizeElemArray;
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
    void printVector(const std::vector<T>& vec)
    {
        for (auto it = vec.cbegin(); it != vec.cend(); ++it)
        {
            std::cout << *it;
        }
    }
};

void TestStruct1()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    Page<char> page(1, true);
    const std::time_t times = std::time(0);


    const std::vector<byte> byteMap(127,1);

    const std::vector<char> elem(127, 'a');

    page.timeModify = times;

    page.AddByteMap(byteMap);
    page.AddElements(elem);

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
    std::cout << "����� ��������/���������: " << page.getFormattedTime() << std::endl;
    std::cout << "������ byteMap:";
    page.PrintByteMap();
    std::cout << "������ elemArray:";
    page.PrintElemArray();
}
void TestStruct2()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    
    const std::time_t times = std::time(0);

    const std::vector<char> elem = { 'a','b','c' };

    Page<char> page(1, true, elem);

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
    std::cout << "����� ��������/���������: " << page.getFormattedTime() << std::endl;
    std::cout << "������ byteMap:";
    page.PrintByteMap();
    std::cout << "������ elemArray:";
    page.PrintElemArray();
}
void TestStruct3()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);


    const std::time_t times = std::time(0);

    const std::vector<std::string> elem(10, "sdoiaytbhlkiswdfhgbikuhjnoedlfkituhlnskfbyhltbgl,bsajynfdughnidlsfktyhnbsakujytrknansjbynvhvcmm186hvc872cv47193tv8uo6yq4v87921sjhkgbhvghnkjfhnsgubsf");

    Page<std::string> page(1, true, elem);

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
    std::cout << "����� ��������/���������: " << page.getFormattedTime() << std::endl;
    std::cout << "������ byteMap:";
    page.PrintByteMap();
    std::cout << "������ elemArray:";
    page.PrintElemArray();
}
void TestStruct4()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);


    const std::time_t times = std::time(0);

    const std::vector<int> elem(100000, 12);

    Page<int> page(1, true, elem);

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
    std::cout << "����� ��������/���������: " << page.getFormattedTime() << std::endl;
    std::cout << "������ byteMap:";
    page.PrintByteMap();
    std::cout << "������ elemArray:";
    page.PrintElemArray();
}
int main()
{
    //TestStruct1();
    std::cout << "---------------------------------" << std::endl;
    //TestStruct2();
    std::cout << "---------------------------------" << std::endl;
    //TestStruct3();
    std::cout << "---------------------------------" << std::endl;
    TestStruct4();
    return 0; 
}