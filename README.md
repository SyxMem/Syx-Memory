# SyxLib

SyxLib is a C++ class that provides functions for memory operations and pattern searching in a Windows environment. It simplifies common memory-related tasks such as reading and writing memory, searching for patterns in memory, and retrieving module base addresses.
### Version : 1.3

## Features

- Memory comparison using masks
- Pattern searching in memory regions
- Nested pointer dereferencing for writing values to memory
- Retrieving the base address of loaded modules
- Function detouring: Detour functions in both 32-bit and 64-bit environments.

## Function Detouring

The SyxLib library supports function detouring in both 32-bit and 64-bit environments. It provides two functions for detouring functions: `DetourFunc32` and `DetourFunc64`.

### DetourFunc32

DetourFunc32 is used to detour functions in a 32-bit environment.

```cpp
const void* DetourFunc32(BYTE* const src, const BYTE* dest, const unsigned int length);
```

### DetourFunc64
DetourFunc64 is used to detour functions in a 64-bit environment.

```cpp
const void* DetourFunc64(BYTE* const src, const BYTE* dest, const unsigned int jumplength);
```

- `src`: Pointer to the source function to be detoured.
- `dest`: Pointer to the destination function where the execution should be redirected.
- `length`: Length of the overwritten instructions.
- `Returns`: Pointer to the original code of the detoured function.



## Documentation

Comprehensive [documentation for SyxLib ](https://syxmem.syferx.net/index.php)It will provide detailed information on how to use the library and its available functions.

## Usage

1. Include the `Syx.h` header file in your project.
2. To use the memory utility functions, call them directly using the `Syx::` scope resolution operator. You don't need to create an instance of the `Syx` class.

### Example: Searching for a Pattern

#### Manual Length Pattern Scanning
```cpp
#include "Syx.h"

int main()
{
    DWORD64 addressToSearch = 0x12345678;
    DWORD searchLength = 100;
    BYTE pattern[] = { 0x90, 0x90, 0x90 };
    char mask[] = "xxx";

    DWORD64 foundAddress = Syx::FindPattern(addressToSearch, searchLength, pattern, mask);
    if (foundAddress)
    {
        printf("Pattern found at address: 0x%llX\n", foundAddress);
    }
    else
    {
        printf("Pattern not found.\n");
    }

    return 0;
}
```

#### Auto Length Pattern Scanning
```cpp
int main()
{
    uintptr_t Address[1024];
    const wchar_t* UserAssembly = L"UserAssembly.dll";
    char Address1M[] = "x????xxxx?xxx?xxxx?????xx";
    char NoFallDamage[] = "\xE8\x99\x99\x99\x99\x48\x85\xC0\x74\x99\x48\x8B\x4F\x99\x48\x85\xC9\x0F\x99\x99\x99\x99\x99\x8B\x51";
    Address[1] = Syx::FindPatternA(UserAssembly, NoFallDamage, Address1M);

    return 0;
}


```



### Example: Writing to a Memory Location with Nested Pointers
```cpp
#include "Syx.h"

int main()
{
    uintptr_t baseAddress = Syx::GetModuleHandleEx("ModuleName.dll");
    std::vector<uintptr_t> offsets = { 0x10, 0x20, 0x30 };
    int valueToWrite = 42;

    try
    {
        Syx::WritePTR(baseAddress, offsets, valueToWrite);
        printf("Value successfully written to memory.\n");
    }
    catch (const std::exception& e)
    {
        printf("Error: %s\n", e.what());
    }

    return 0;
}
```

### Example: DetourFunc32/64 function to detour a function:
```cpp

// Address to hook
uintptr_t Address = 0x37C5D2FA;

// Hook64 Location
#ifdef _WIN64
oProcessEvent = reinterpret_cast(Syx::DetourFunc64(reinterpret_cast(Address), reinterpret_cast(hProcessEvent64), 18));
#endif

// Hook32 Location
#ifdef _WIN32
oProcessEvent = reinterpret_cast(Syx::DetourFunc32(reinterpret_cast(Address), reinterpret_cast(hProcessEvent32), 5));
#endif

```

## Contributions
Contributions to the SyxLib project are welcome! If you find any issues or have suggestions for improvements, feel free to open an issue or submit a pull request.

## License
The SyxLib is open-source software released under the [MIT License](https://github.com/SyxMem/Syx-Memory/blob/main/LICENSE).
