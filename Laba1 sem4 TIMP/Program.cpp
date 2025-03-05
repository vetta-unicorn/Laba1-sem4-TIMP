#include <iostream>
#include <chrono>
#include <ctime>
#include <vector>
#include <fstream>
#include <string>
#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable : 4996)

class AbsReader
{

public:
    virtual int PageNumber(long int d) {};
    virtual int ReadMap(int index) {};
    virtual int WriteInfo(int index, std::string mass) {};

};

struct Page
{
    int pagenumber;
    int pagestatus;
    time_t time;
    char byteMap [64];
};




class NumberReader : AbsReader
{
private:
    std::string fileName;
    long int byteMapSize;
    std::string fileType;
    int lineLength;
    Page book[];
public:

    NumberReader(std::string _fileName, long int _byteMapSize, std::string _fileType, int _lineLength=0)
    {
        fileName = _fileName;
        byteMapSize = _byteMapSize;
        fileType = _fileType;
        lineLength = _lineLength;
        BookInizilizate();
    };

    // возвращает номер страницы
    int PageNumber(long int d, Page book[])
    {
        int pagenumber = d / 64;
       
        if (pagenumber > sizeof(book))
        {
            pagenumber = -1;
        }

        return pagenumber;
    };

    int ReadMap(int index) {};
    int WriteInfo(int index, std::string mass) {};

    /*Page OldPage(Page book[])
    {
        int pagenumber = -1;
        time_t time = 0;

        for(Page page : book) 
        {
            if (page.time > time)
                {
                    time = page.time;
                }
        }

    }*/

    Page BookInizilizate()
    {
        //тут надо крч найти развер файла, потом от него отнять 2 и поделить на размер страницы с верхушной нацело+1. 
        // потом сделать цыкл и инициализировать
    }

};

void cycle()
{
    int ind = -1;
    while (ind != 0)
    {
        std::cout << "Select an action\n";
        std::cout << "0 - exit\n";
        std::cout << "1 - read info\n";
        std::cout << "2 - write info\n";
        std::cin >> ind;
        switch (ind)
        {
        case 0:
        {
            break;
        }
        case 1:
        {
            //тут общаяя функция на чтение
            break;
        }
        case 2:
        {
            //тут общая функция записи
            break;
        }


        }
    }
};

void Reading(std::string filename, std::fstream file)
{
    int index = -1;
    std::cout << "Enter the block number\n";
    std::cin >> index;
    file.open(filename, std::ios::in);
    


    //file.seekg(plase);



}


int main()
{
    std::fstream file; 
    const char* filename = "dada1.bin";
    std:: string filename_str = "dada1.bin";
    file.open(filename);
    //вид файла спрашиввать
    //NumberReader b(filename_str, 64,  );
    






    file.close();
    std::cout << "Hello World!\n";
    return 0;
}


