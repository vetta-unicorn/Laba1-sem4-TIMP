#include "VirtualMemory.hpp"
void TestVM_1()
{
    VirtualMemory vm;

    PageV1_1<int> page1;



    vm.write<PageV1_1<int>>(0, page1);


    PageV1_1<int> page2;
    if (vm.read(0, page2))
    {
        std::cout << "Read from address: 0" << std::endl;
        page2.PrintPage();
    }
}
void TestVM_2()
{
    VirtualMemory vmInt;
    VirtualMemory vmChar;
    VirtualMemory vmString;

    std::time_t times = std::time(0);

    std::vector<int> elemInt(10, 45);
    std::vector<char> elemChar(10, 'c');
    std::vector<std::string> elemStr(10, "sdhgfkshsggfjsaivjrheyyg");

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
    pageStr.SetLenString(12);
    pageStr.SetBitMap();

    vmInt.write<PageV1_1<int>>(0, pageInt);
    vmInt.write<PageV1_1<int>>(sizeof(pageInt), pageInt);
    vmChar.write<PageV1_1<char>>(0, pageChar);
    //  vmString.write(0, pageStr);

    PageV1_1<std::string> page3;
    PageV1_1<int> page2;
    PageV1_1<int> page4;
    if (vmInt.read(0, page2))
    {
        std::cout << "Read int VM:\n";
        page2.PrintPage();
    }
    if (vmInt.read(sizeof(page2), page2))
    {
        page2.PrintPage();
    }
    else
    {
        std::cout << "Failed to read page2 from vmInt\n";
    }
    if (vmInt.read(sizeof(page4) * 2, page4))
    {
        page2.PrintPage();
    }
    else
    {
        std::cout << "Failed to read page2 from vmInt\n";
    }
    /*PageV1_1<char> page1;
    if (vmChar.read(0, page1))
    {
        std::cout << "Read char VM:\n";
        page1.PrintPage();
    }
    else {
        std::cout << "Failed to read page1 from vmChar\n";
    }*/
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
}