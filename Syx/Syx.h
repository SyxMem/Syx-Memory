#include <stdexcept>
#include <vector>
#include <Windows.h>

/**
 * SyxLib is a C++ class that provides functions for memory operations and pattern searching in a Windows environment.
 * It simplifies common memory-related tasks such as reading and writing memory, searching for patterns in memory,
 * and retrieving module base addresses.
 *
 * Author: SyferX
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
    static uintptr_t FindPattern(uintptr_t dwAddress, uintptr_t dwLen, BYTE* bMask, char* szMask)
    {
        for (uintptr_t i = 0; i < dwLen; i++)
        {
            if (DataCompare((BYTE*)(dwAddress + i), bMask, szMask))
            {
                return (uintptr_t)(dwAddress + i);
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

    /**
     * Detours a function by replacing its code with a jump to a detour function.
     *
     * @param src Pointer to the function to detour.
     * @param dest Pointer to the detour function.
     * @param jumplength Length of the jump instruction.
     * @return A pointer to the original code of the function.
     */
    static const void* DetourFunc64(BYTE* const src, const BYTE* dest, const unsigned int jumplength)
    {
        // Allocate a memory page that is going to contain executable code.
        MEMORY_BASIC_INFORMATION mbi;
        for (SIZE_T addr = (SIZE_T)src; addr > (SIZE_T)src - 0x80000000; addr = (SIZE_T)mbi.BaseAddress - 1)
        {
            if (!VirtualQuery((LPCVOID)addr, &mbi, sizeof(mbi)))
            {
                break;
            }

            if (mbi.State == MEM_FREE)
            {
                if (presenthook64 = (HookContext*)VirtualAlloc(mbi.BaseAddress, 0x1000, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE))
                {
                    break;
                }
            }
        }

        // If allocating a memory page failed, the function failed.
        if (!presenthook64)
        {
            return nullptr;
        }

        // Save original code and apply detour. The detour code is:
        // push rax
        // movabs rax, 0xCCCCCCCCCCCCCCCC
        // xchg rax, [rsp]
        // ret
        BYTE detour[] = { 0x50, 0x48, 0xB8, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0x48, 0x87, 0x04, 0x24, 0xC3 };
        const unsigned int length = jumplength;
        memcpy(presenthook64->original_code, src, length);
        memcpy(&presenthook64->original_code[length], detour, sizeof(detour));
        *(SIZE_T*)&presenthook64->original_code[length + 3] = (SIZE_T)src + length;

        // Build a far jump to the destination function.
        *(WORD*)&presenthook64->far_jmp = 0x25FF;
        *(DWORD*)(presenthook64->far_jmp + 2) = (DWORD)((SIZE_T)presenthook64 - (SIZE_T)src + FIELD_OFFSET(HookContext, dst_ptr) - 6);
        presenthook64->dst_ptr = (SIZE_T)dest;

        // Write the hook to the original function.
        DWORD flOld = 0;
        VirtualProtect(src, 6, PAGE_EXECUTE_READWRITE, &flOld);
        memcpy(src, presenthook64->far_jmp, sizeof(presenthook64->far_jmp));
        VirtualProtect(src, 6, flOld, &flOld);

        // Return a pointer to the original code.
        return presenthook64->original_code;
    }

    /**
     * Detours a function in a 32-bit environment.
     *
     * @param src Pointer to the source function to be detoured.
     * @param dest Pointer to the destination function where the execution should be redirected.
     * @param length Length of the overwritten instructions.
     * @return Pointer to the original code of the detoured function.
     */
    static const void* DetourFunc32(BYTE* const src, const BYTE* dest, const unsigned int length)
    {
        const auto detourLength = length < 5 ? 5 : length;

        BYTE* jmp = new BYTE[detourLength];
        DWORD dwOldProtect, dwBkup, dwRelAddr;
        VirtualProtect(src, detourLength, PAGE_EXECUTE_READWRITE, &dwOldProtect);
        dwRelAddr = (DWORD)(dest - src) - 5;
        *src = 0xE9;
        *(DWORD*)(src + 1) = dwRelAddr;
        for (unsigned int i = 5; i < detourLength; i++)
        {
            *(src + i) = 0x90;
        }
        VirtualProtect(src, detourLength, dwOldProtect, &dwBkup);

        memcpy(jmp, src, detourLength);
        jmp += detourLength;
        *(jmp - 5) = 0xE9;
        *(DWORD*)(jmp - 4) = (DWORD)((src - jmp) - 5);

        return (jmp - detourLength);
    }

private:
    struct HookContext
    {
        BYTE original_code[64];
        SIZE_T dst_ptr;
        BYTE far_jmp[6];
    };

    static HookContext* presenthook64;
};


//Don't touch, let it be here.
Syx::HookContext* Syx::presenthook64;
