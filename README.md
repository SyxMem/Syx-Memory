# SyxLib

SyxLib is a C++ class that provides functions for memory operations and pattern searching in a Windows environment. It simplifies common memory-related tasks such as reading and writing memory, searching for patterns in memory, and retrieving module base addresses.

## Features

- Memory comparison using masks
- Pattern searching in memory regions
- Nested pointer dereferencing for writing values to memory
- Retrieving the base address of loaded modules

## Documentation

Comprehensive documentation for SyxLib is coming soon! It will provide detailed information on how to use the library and its available functions.

Stay tuned for updates on the documentation website.

## Usage

1. Include the `Syx.h` header file in your project.
2. To use the memory utility functions, call them directly using the `Syx::` scope resolution operator. You don't need to create an instance of the `Syx` class.

### Example: Searching for a Pattern

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

## Example: Writing to a Memory Location with Nested Pointers
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

## Contributions
Contributions to the SyxLib project are welcome! If you find any issues or have suggestions for improvements, feel free to open an issue or submit a pull request.

## License
The SyxLib is open-source software released under the [MIT License](https://github.com/SyxMem/Syx-Memory/blob/main/LICENSE).
