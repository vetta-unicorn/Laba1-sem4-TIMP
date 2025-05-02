#include "PageV1_1.hpp"
#include <type_traits>
#include <cstring>

#define MAX_MEMORY_SIZE 512

class VirtualMemory
{
private:
    uint8_t memory[MAX_MEMORY_SIZE]; // ������������� ������ ������
    bool initMemory[MAX_MEMORY_SIZE]; // ���� ������������� ��� ������� �����
    size_t used_size;                // ������� ������� ������

public:
    VirtualMemory() : used_size(0)
    {
        std::memset(memory, 0, MAX_MEMORY_SIZE);
        std::memset(initMemory, 0, MAX_MEMORY_SIZE);
    }
    template <typename T>
    bool write(size_t address, const T& data)
    {
        // ���������, ���������� �� �����
        size_t data_size = data.GetTotalSize();
        if (address >= MAX_MEMORY_SIZE || address + data_size > MAX_MEMORY_SIZE) {
            std::cerr << "Error: Address out of bounds or insufficient memory (address: "
                << address << ", size: " << data_size << ")\n";
            return false;
        }

        // ����������� ������ � ������
        data.Serialize(memory + address);

        for (size_t i = address; i < address + data_size; ++i)
        {
            initMemory[i] = true;
        }

        // ��������� �������������� ������
        used_size = std::max(used_size, address + data_size);
        return true;
    }

    template <typename T>
    bool read(size_t address, T& data) const
    {
        if (address >= MAX_MEMORY_SIZE) {
            std::cerr << "Error: Address out of bounds (address: " << address
                << ", max: " << MAX_MEMORY_SIZE << ")\n";
            return false;
        }

        // ������������, ��� data.serialized_size() ����� ���������� ������
        size_t data_size = data.GetTotalSize();
        if (address + data_size > MAX_MEMORY_SIZE) {
            std::cerr << "Error: Address range out of bounds (address: " << address
                << ", size: " << data_size << ", max: " << MAX_MEMORY_SIZE << ")\n";
            return false;
        }

        // ���������, ��� ��� ����� � ��������� ����������������
        for (size_t i = address; i < address + data_size; ++i) {
            if (!initMemory[i]) {
                std::cerr << "Error: Attempt to read uninitialized memory at address " << i << "\n";
                return false;
            }
        }

        // ������������� ������
        data.Deserialize(memory + address);
        return true;
    }

    size_t getUsedSize() const { return used_size; }
    size_t getCapacity() const { return MAX_MEMORY_SIZE; }
};
