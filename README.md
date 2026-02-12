# 🧠 SyxLib - Windows Memory Operations Library

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Version](https://img.shields.io/badge/version-1.3-blue.svg)](https://github.com/SyxMem/Syx-Memory)
[![Platform](https://img.shields.io/badge/platform-Windows-lightgrey.svg)](https://github.com/SyxMem/Syx-Memory)

SyxLib is a powerful, header-only C++ library for Windows that simplifies memory operations, pattern scanning, and function hooking. Perfect for game modding, reverse engineering, debugging, and memory research.

### ✨ Version 1.3

## 📋 Table of Contents

- [Features](#-features)
- [Architecture Overview](#-architecture-overview)
- [Quick Start](#-quick-start)
- [Installation](#-installation)
- [Usage Examples](#-usage-examples)
- [API Documentation](#-api-documentation)
- [Documentation](#-documentation)
- [FAQ](#-faq)
- [Troubleshooting](#-troubleshooting)
- [Contributing](#-contributing)
- [License](#-license)
- [Support](#-support)

## 🚀 Features

- **🔍 Pattern Scanning** - Find byte patterns in memory with wildcard support
- **💾 Memory Operations** - Read/write memory through nested pointer chains
- **🪝 Function Hooking** - Detour functions in both 32-bit and 64-bit environments
- **📦 Module Management** - Retrieve base addresses of loaded modules
- **🎯 Type-Safe** - Template-based memory operations for any data type
- **⚡ Performance** - Efficient pattern scanning and memory access
- **🛡️ Error Handling** - Exception-based error reporting for robust code

## 🏗️ Architecture Overview

```
┌─────────────────────────────────────────────────┐
│              SyxLib Core Components              │
└─────────────────────────────────────────────────┘
         │              │              │
    ┌────▼───┐    ┌────▼────┐   ┌─────▼─────┐
    │ Memory │    │ Pattern │   │ Function  │
    │   Ops  │    │ Scanner │   │  Hooking  │
    └────────┘    └─────────┘   └───────────┘
```

### Component Breakdown

| Component | Functions | Purpose |
|-----------|-----------|---------|
| **Memory Operations** | `WritePTR`, `GetModuleHandleEx` | Read/write through pointer chains |
| **Pattern Scanner** | `FindPattern`, `FindPatternA`, `DataCompare` | Locate byte patterns in memory |
| **Function Hooking** | `DetourFunc32`, `DetourFunc64` | Intercept and redirect function calls |

For detailed architecture information, see [docs/architecture.md](docs/architecture.md).

## 🎯 Quick Start

### Prerequisites

- **OS**: Windows 10 or later
- **Compiler**: MSVC 2017+, MinGW, or compatible
- **Architecture**: x86 or x64

### Simple Example

```cpp
#include "Syx.h"

int main() {
    // Find a pattern in memory
    const wchar_t* module = L"game.dll";
    char pattern[] = "\x48\x8B\xC8\xE8";
    char mask[] = "xxxx";
    
    uintptr_t address = Syx::FindPatternA(module, pattern, mask);
    
    if (address) {
        printf("Pattern found at: 0x%llX\n", address);
        
        // Write through nested pointers
        std::vector<uintptr_t> offsets = {0x10, 0x20, 0x30};
        Syx::WritePTR(address, offsets, 999);
    }
    
    return 0;
}
```



## 📦 Installation

### Method 1: Direct Include (Recommended)

1. **Clone or download** the repository:
   ```bash
   git clone https://github.com/SyxMem/Syx-Memory.git
   ```

2. **Copy the header** to your project:
   ```
   YourProject/
   ├── include/
   │   └── Syx.h          ← Copy from Syx/Syx.h
   └── src/
       └── main.cpp
   ```

3. **Include in your code**:
   ```cpp
   #include "Syx.h"
   ```

4. **Link required libraries** in your project settings:
   - `Psapi.lib` (required for module information)

### Method 2: Git Submodule

```bash
cd YourProject
git submodule add https://github.com/SyxMem/Syx-Memory.git external/SyxLib
```

Then include with: `#include "external/SyxLib/Syx/Syx.h"`

### Build Configuration

#### Visual Studio
Add to your project properties:
- **C/C++ → General → Additional Include Directories**: Path to Syx.h
- **Linker → Input → Additional Dependencies**: `Psapi.lib`

#### CMake
```cmake
target_link_libraries(YourProject PRIVATE Psapi)
```

#### g++/MinGW
```bash
g++ main.cpp -lPsapi -o output.exe
```

## 💡 Usage Examples

### Example 1: Pattern Scanning

Find byte patterns in process memory with support for wildcards:

#### Manual Length Scanning
```cpp
#include "Syx.h"

int main() {
    uintptr_t baseAddress = 0x10000000;
    DWORD searchLength = 0x100000;
    
    // Pattern: Three NOP instructions (0x90)
    BYTE pattern[] = { 0x90, 0x90, 0x90 };
    char mask[] = "xxx";  // 'x' = exact match
    
    uintptr_t found = Syx::FindPattern(baseAddress, searchLength, pattern, mask);
    
    if (found) {
        printf("✓ Pattern found at: 0x%llX\n", found);
    } else {
        printf("✗ Pattern not found\n");
    }
    
    return 0;
}
```

#### Automatic Length Pattern Scanning with Wildcards
```cpp
int main() {
    const wchar_t* moduleName = L"UserAssembly.dll";
    
    // Pattern with wildcards (0x99 will be ignored)
    char pattern[] = "\xE8\x99\x99\x99\x99\x48\x85\xC0\x74\x99";
    char mask[]    = "x????xxxx?";  // '?' = wildcard
    
    uintptr_t address = Syx::FindPatternA(moduleName, pattern, mask);
    
    if (address) {
        printf("✓ Found at: 0x%llX\n", address);
    }
    
    return 0;
}
```

### Example 2: Memory Writing with Nested Pointers

Write values through multi-level pointer chains:

```cpp
#include "Syx.h"
#include <vector>

int main() {
    // Get module base address
    uintptr_t baseAddress = Syx::GetModuleHandleEx("game.dll");
    
    // Define pointer chain: Base → [0x10] → [0x20] → [0x30] → Value
    std::vector<uintptr_t> offsets = { 0x10, 0x20, 0x30 };
    
    try {
        // Write different data types
        Syx::WritePTR(baseAddress, offsets, 12345);      // int
        Syx::WritePTR(baseAddress, offsets, 99.99f);     // float
        Syx::WritePTR(baseAddress, offsets, 123.456);    // double
        
        printf("✓ Values written successfully!\n");
    } 
    catch (const std::exception& e) {
        printf("✗ Error: %s\n", e.what());
    }
    
    return 0;
}
```

### Example 3: Function Hooking

Intercept and modify function behavior:

#### 64-bit Function Detour
```cpp
#ifdef _WIN64

typedef void(*ProcessEventFunc)(void* obj, void* params);
ProcessEventFunc originalProcessEvent = nullptr;

void MyHookFunction(void* obj, void* params) {
    printf("🪝 Function hooked! Processing...\n");
    
    // Modify behavior here
    
    // Call original function
    originalProcessEvent(obj, params);
}

void InstallHook() {
    // Find target function
    uintptr_t targetAddr = Syx::FindPatternA(L"game.dll", 
        "\x48\x89\x5C\x24\x08", "xxxxx");
    
    if (targetAddr) {
        originalProcessEvent = reinterpret_cast<ProcessEventFunc>(
            Syx::DetourFunc64(
                reinterpret_cast<BYTE*>(targetAddr),
                reinterpret_cast<BYTE*>(MyHookFunction),
                14  // Instruction length
            )
        );
        
        printf("✓ Hook installed at: 0x%llX\n", targetAddr);
    }
}

#endif
```

#### 32-bit Function Detour
```cpp
#ifdef _WIN32

typedef int(*GameTickFunc)(float deltaTime);
GameTickFunc originalTick = nullptr;

int MyTickHook(float deltaTime) {
    printf("🪝 Tick: %.2fms\n", deltaTime);
    return originalTick(deltaTime);
}

void InstallHook32() {
    uintptr_t tickAddr = 0x401000;  // Known address
    
    originalTick = reinterpret_cast<GameTickFunc>(
        Syx::DetourFunc32(
            reinterpret_cast<BYTE*>(tickAddr),
            reinterpret_cast<BYTE*>(MyTickHook),
            5  // Minimum 5 bytes for JMP
        )
    );
}

#endif
```

### More Examples

Complete working examples are available in the [`examples/`](examples/) directory:
- [`pattern_scanning.cpp`](examples/pattern_scanning.cpp) - Pattern search demonstrations
- [`memory_writing.cpp`](examples/memory_writing.cpp) - Nested pointer examples
- [`function_hooking.cpp`](examples/function_hooking.cpp) - Hooking techniques

## 📚 API Documentation

### Memory Operations

#### `GetModuleHandleEx`
```cpp
static uintptr_t GetModuleHandleEx(const char* moduleName)
```
Retrieves the base address of a loaded module.

**Parameters:**
- `moduleName` - Name of the module (e.g., "game.dll"), or `nullptr` for current process

**Returns:** Base address of the module, or 0 if not found

**Example:**
```cpp
uintptr_t base = Syx::GetModuleHandleEx("game.dll");
```

---

#### `WritePTR`
```cpp
template <typename T>
static void WritePTR(uintptr_t baseAddress, const std::vector<uintptr_t>& offsets, T valueHere)
```
Writes a value through a chain of nested pointers.

**Parameters:**
- `baseAddress` - Starting address
- `offsets` - Vector of offsets to follow
- `valueHere` - Value to write (any type)

**Throws:** `std::runtime_error` if pointer chain is invalid

**Example:**
```cpp
std::vector<uintptr_t> offsets = {0x10, 0x20, 0x30};
Syx::WritePTR(base, offsets, 999);
```

### Pattern Scanning

#### `FindPattern`
```cpp
static uintptr_t FindPattern(uintptr_t dwAddress, uintptr_t dwLen, BYTE* bMask, char* szMask)
```
Searches for a byte pattern in a memory region with manual length specification.

**Parameters:**
- `dwAddress` - Starting address
- `dwLen` - Length of region to search
- `bMask` - Byte pattern to find
- `szMask` - Mask string ('x' = exact, '?' = wildcard)

**Returns:** Address where pattern was found, or 0 if not found

---

#### `FindPatternA`
```cpp
static uintptr_t FindPatternA(const wchar_t* module, char* pattern, char* mask)
```
Searches for a pattern with automatic length calculation.

**Parameters:**
- `module` - Module name to search in
- `pattern` - Byte pattern (wildcards as any value)
- `mask` - Mask string ('x' = exact, '?' = wildcard)

**Returns:** Address where pattern was found, or NULL if not found

---

#### `DataCompare`
```cpp
static bool DataCompare(const BYTE* pData, const BYTE* bMask, const char* szMask)
```
Compares data byte-by-byte using a mask.

**Parameters:**
- `pData` - Data to compare
- `bMask` - Pattern to compare against
- `szMask` - Mask string

**Returns:** `true` if data matches pattern, `false` otherwise

### Function Hooking

#### `DetourFunc64`
```cpp
static const void* DetourFunc64(BYTE* const src, const BYTE* dest, const unsigned int jumplength)
```
Detours a function in 64-bit environment.

**Parameters:**
- `src` - Address of function to hook
- `dest` - Address of your hook function
- `jumplength` - Length of instructions to overwrite (must be ≥14)

**Returns:** Pointer to original function code (use to call original)

**⚠️ Important:** Verify instruction boundaries with a disassembler!

---

#### `DetourFunc32`
```cpp
static const void* DetourFunc32(BYTE* const src, const BYTE* dest, const unsigned int length)
```
Detours a function in 32-bit environment.

**Parameters:**
- `src` - Address of function to hook
- `dest` - Address of your hook function  
- `length` - Length of instructions to overwrite (minimum 5)

**Returns:** Pointer to original function code

**⚠️ Important:** Minimum 5 bytes required for JMP instruction!

### Helper Functions

#### `GetModuleInfo`
```cpp
static MODULEINFO GetModuleInfo(const wchar_t* szModule)
```
Retrieves detailed information about a module.

**Parameters:**
- `szModule` - Module name (wide string)

**Returns:** `MODULEINFO` structure with base address and size

## 📖 Documentation

### Quick Reference

- **[Architecture Overview](docs/architecture.md)** - Visual component breakdown and memory diagrams
- **[Usage Guide](docs/usage-guide.md)** - Detailed usage patterns with scenarios
- **[Examples Directory](examples/)** - Complete working code examples

### External Resources

- **[Official Documentation](https://sysmem.syferx.net/index.php)** - Comprehensive online documentation
- **[GitHub Repository](https://github.com/SyxMem/Syx-Memory)** - Source code and issues

## ❓ FAQ

### General Questions

**Q: Is SyxLib thread-safe?**  
A: No, by default SyxLib functions are not thread-safe. Synchronize access when using from multiple threads.

**Q: Can I use this on Linux or macOS?**  
A: No, SyxLib is Windows-specific and uses Windows API functions (VirtualAlloc, VirtualProtect, etc.).

**Q: Does it work with anti-cheat systems?**  
A: Many anti-cheat systems detect memory manipulation and function hooking. Use at your own risk.

**Q: What's the performance impact of pattern scanning?**  
A: Pattern scanning is O(n) where n is the search region size. For best performance, minimize the search region.

### Technical Questions

**Q: How do I determine the correct hook length?**  
A: Use a disassembler (IDA Pro, Ghidra, x64dbg) to ensure you're not splitting instructions. Count complete instruction bytes until you have at least 5 (32-bit) or 14 (64-bit) bytes.

**Q: Why does my hook crash the program?**  
A: Common causes:
- Incorrect instruction length (split instruction)
- Stack alignment issues
- Calling convention mismatch
- Not calling the original function

**Q: Can I hook the same function multiple times?**  
A: Yes, but you need to hook in sequence and maintain the chain properly.

**Q: What types can WritePTR handle?**  
A: Any type supported by C++ templates: `int`, `float`, `double`, custom structs, etc.

## 🔧 Troubleshooting

### Common Issues and Solutions

| Problem | Cause | Solution |
|---------|-------|----------|
| 🔴 Pattern not found | Wrong pattern or mask | Verify bytes with a memory viewer/debugger |
| 🔴 Access violation on write | Invalid pointer chain | Validate each offset level individually |
| 🔴 Hook doesn't execute | Wrong hook length | Disassemble and count instruction bytes |
| 🔴 Program crashes after hook | Split instruction | Adjust length to complete instructions |
| 🔴 Module not found | Wrong name or not loaded | Check module name with Process Explorer |
| 🔴 DetourFunc64 returns nullptr | Memory allocation failed | System may not have suitable address space |

### Debug Tips

1. **Validate module loading:**
   ```cpp
   MODULEINFO info = Syx::GetModuleInfo(L"module.dll");
   if (info.lpBaseOfDll == nullptr) {
       printf("Module not loaded!\n");
   }
   ```

2. **Test pattern at known address:**
   ```cpp
   // If you know the address, verify your pattern matches
   uintptr_t knownAddr = 0x12345678;
   BYTE* data = reinterpret_cast<BYTE*>(knownAddr);
   // Manually check bytes
   ```

3. **Log pointer chain:**
   ```cpp
   uintptr_t* ptr = reinterpret_cast<uintptr_t*>(base);
   for (size_t i = 0; i < offsets.size(); i++) {
       printf("Level %zu: 0x%llX\n", i, 
              reinterpret_cast<uintptr_t>(ptr));
       ptr = reinterpret_cast<uintptr_t*>(*ptr + offsets[i]);
   }
   ```

### Best Practices

✅ **Do:**
- Always validate return values
- Use try-catch blocks for memory operations
- Test on controlled environments first
- Keep backups before modifying memory
- Respect software licenses and terms of service

❌ **Don't:**
- Ignore null/zero return values
- Hook with insufficient instruction length
- Modify memory without understanding consequences
- Use on online games with anti-cheat
- Distribute cheats or malicious tools

## 🤝 Contributing

We welcome contributions! Please see our [Contributing Guide](CONTRIBUTING.md) for details.

### How to Contribute

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

### Development

See [CONTRIBUTING.md](CONTRIBUTING.md) for:
- Code style guidelines
- Commit message format
- Testing procedures
- Development setup

## 📄 License

SyxLib is open-source software released under the [MIT License](LICENSE).

```
MIT License - Copyright (c) 2023 SyxMem

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software...
```

See [LICENSE](LICENSE) file for full details.

## 💬 Support

### Get Help

- **📖 Documentation**: [docs/](docs/) directory
- **💡 Examples**: [examples/](examples/) directory  
- **🐛 Bug Reports**: [GitHub Issues](https://github.com/SyxMem/Syx-Memory/issues)
- **💬 Discussions**: [GitHub Discussions](https://github.com/SyxMem/Syx-Memory/discussions)
- **🌐 Website**: [https://sysmem.syferx.net](https://sysmem.syferx.net/index.php)

### Found a Bug?

Please [open an issue](https://github.com/SyxMem/Syx-Memory/issues/new) with:
- Clear description
- Steps to reproduce
- Expected vs actual behavior
- Environment details (OS, compiler, architecture)
- Code sample if applicable

## 🌟 Star History

If you find SyxLib useful, please consider giving it a star ⭐ on GitHub!

## 📊 Project Status

- ✅ **Stable**: Core functionality is production-ready
- 🔄 **Active**: Maintained and accepting contributions
- 📈 **Growing**: Documentation and examples expanding

## 🙏 Acknowledgments

- **Author**: SyferX / RealSyferX
- **Contributors**: Thank you to all contributors!
- **Community**: Thanks to the reverse engineering community for feedback and support

---

<div align="center">

**[⬆ Back to Top](#-syxlib---windows-memory-operations-library)**

Made with ❤️ by the SyxLib team

</div>
