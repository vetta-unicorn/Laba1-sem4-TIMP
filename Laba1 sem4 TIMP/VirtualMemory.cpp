#include "PageV1_1.h"
#include <type_traits>
#include <cstring>

#define MAX_MEMORY_SIZE 512

template <typename T>
class VirtualMemory
{
private:
    uint8_t memory[MAX_MEMORY_SIZE]; // Фиксированный массив байтов
    bool initMemory[MAX_MEMORY_SIZE]; // Флаг инициализации для каждого байта
    size_t used_size;                // Текущий занятый размер

public:
    VirtualMemory() : used_size(0)
    {
        std::memset(memory, 0, MAX_MEMORY_SIZE);
        std::memset(initMemory, 0, MAX_MEMORY_SIZE);
    }

    bool write(size_t address, const T& data)
    {
        // Проверяем, достаточно ли места
        size_t data_size = data.GetTotalSize();
        if (address >= MAX_MEMORY_SIZE || address + data_size > MAX_MEMORY_SIZE) {
            std::cerr << "Error: Address out of bounds or insufficient memory (address: "
                << address << ", size: " << data_size << ")\n";
            return false;
        }

        // Сериализуем данные в память
        data.Serialize(memory + address);

        for (size_t i = address; i < address + data_size; ++i)
        {
            initMemory[i] = true;
        }

        // Обновляем использованный размер
        used_size = std::max(used_size, address + data_size);
        return true;
    }

    bool read(size_t address, T& data) const
    {
        if (address >= MAX_MEMORY_SIZE) {
            std::cerr << "Error: Address out of bounds (address: " << address
                << ", max: " << MAX_MEMORY_SIZE << ")\n";
            return false;
        }

        // Предполагаем, что data.serialized_size() вернёт корректный размер
        size_t data_size = data.GetTotalSize();
        if (address + data_size > MAX_MEMORY_SIZE) {
            std::cerr << "Error: Address range out of bounds (address: " << address
                << ", size: " << data_size << ", max: " << MAX_MEMORY_SIZE << ")\n";
            return false;
        }

        // Проверяем, что все байты в диапазоне инициализированы
        for (size_t i = address; i < address + data_size; ++i) {
            if (!initMemory[i]) {
                std::cerr << "Error: Attempt to read uninitialized memory at address " << i << "\n";
                return false;
            }
        }

        // Десериализуем данные
        data.Deserialize(memory + address);
        return true;
    }

    size_t getUsedSize() const { return used_size; }
    size_t getCapacity() const { return MAX_MEMORY_SIZE; }
};
void TestVM_1()
{
	VirtualMemory<PageV1_1<int>> vm;
	
	PageV1_1<int> page1;
    


	vm.write(0, page1);


	PageV1_1<int> page2;
	if (vm.read(0, page2))
	{
		std::cout << "Read from address: 0" << std::endl;
		page2.PrintPage();
	}
}
void TestVM_2()
{
    VirtualMemory<PageV1_1<int>> vmInt;
    VirtualMemory<PageV1_1<char>> vmChar;
    VirtualMemory<PageV1_1<std::string>> vmString;

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

    vmInt.write(0, pageInt);
    vmChar.write(0, pageChar);
  //  vmString.write(0, pageStr);

    PageV1_1<std::string> page3;
    PageV1_1<int> page2;
    if (vmInt.read(0, page2))
    {
        std::cout << "Read int VM:\n";
        page2.PrintPage(); 
    }
    else
    {
        std::cout << "Failed to read page2 from vmInt\n";
    }

    PageV1_1<char> page1;
    if (vmChar.read(0, page1)) 
    {
        std::cout << "Read char VM:\n";
        page1.PrintPage();
    }
    else {
        std::cout << "Failed to read page1 from vmChar\n";
    }
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
int main()
{
    
	//TestVM_1();
    TestVM_2();
	return 0;
}