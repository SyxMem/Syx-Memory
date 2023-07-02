#include <stdexcept>
#include <Windows.h>

/**
 * SyxLib is a C++ class that provides functions for memory operations and pattern searching in a Windows environment.
 * It simplifies common memory-related tasks such as reading and writing memory, searching for patterns in memory,
 * and retrieving module base addresses.
 *
 * Author: Afdul2021
 */

class Syx
{
public:
    /**
     * Compares data byte by byte using a mask.
     *
     * @param pData Pointer to the data to compare.
     * @param bMask Pointer to the comparison mask.
     * @param szMask Pointer to the mask string.
     * @return True if the data matches the mask, false otherwise.
     */
    static bool DataCompare(const BYTE* pData, const BYTE* bMask, const char* szMask)
    {
        for (; *szMask; ++szMask, ++pData, ++bMask)
        {
            if (*szMask == 'x' && *pData != *bMask)
            {
                return false;
            }
        }
        return (*szMask) == NULL;
    }

    /**
     * Searches for a pattern in a specified memory region.
     *
     * @param dwAddress Starting address of the memory region.
     * @param dwLen Length of the memory region to search.
     * @param bMask Pointer to the pattern mask.
     * @param szMask Pointer to the mask string.
     * @return The address where the pattern was found, or 0 if not found.
     */
    static DWORD64 FindPattern(DWORD64 dwAddress, DWORD dwLen, BYTE* bMask, char* szMask)
    {
        for (DWORD i = 0; i < dwLen; i++)
        {
            if (DataCompare((BYTE*)(dwAddress + i), bMask, szMask))
            {
                return (DWORD64)(dwAddress + i);
            }
        }
        return 0;
    }

    /**
     * Writes a value to a memory location specified by a nested pointer.
     *
     * @param baseAddress The base address to start the nested pointer dereferencing.
     * @param offsets A vector of offsets that define the path to the desired memory location.
     * @param valueHere The value to be written to the memory location.
     * @throw std::runtime_error if an invalid pointer is encountered during the dereferencing process.
     */
    template <typename T>
    static void WritePTR(uintptr_t baseAddress, const std::vector<uintptr_t>& offsets, T valueHere)
    {
        uintptr_t* currentPtr = (uintptr_t*)baseAddress;

        try {
            for (size_t i = 0; i < offsets.size(); i++) {
                if (!currentPtr) {
                    throw std::runtime_error("Invalid pointer encountered.");
                }
                currentPtr = (uintptr_t*)(*currentPtr + offsets[i]);
            }

            if (!currentPtr) {
                throw std::runtime_error("Invalid pointer encountered.");
            }

            *reinterpret_cast<T*>(currentPtr) = valueHere;
        }
        catch (const std::exception& e) {
            printf("%s", e.what());
            throw;
        }
    }

    /**
     * Retrieves the base address of a module loaded in the current process.
     *
     * @param moduleName The name of the module.
     * @return The base address of the module if found, or nullptr if the module is not loaded.
     */
    static uintptr_t GetModuleHandleEx(const char* moduleName)
    {
        return reinterpret_cast<uintptr_t>(GetModuleHandleA(moduleName));
    }
};

