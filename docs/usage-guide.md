# SyxLib Usage Guide

## Quick Start

### Prerequisites
- Windows Operating System
- C++ Compiler (MSVC, MinGW, or similar)
- Windows SDK

### Installation

1. **Download the library:**
   ```bash
   git clone https://github.com/SyxMem/Syx-Memory.git
   ```

2. **Add to your project:**
   - Copy `Syx/Syx.h` to your project's include directory
   - Include it in your source files: `#include "Syx.h"`

3. **Link required libraries:**
   - `Psapi.lib` (for module information)

## Usage Patterns

### 1. Finding Patterns in Memory

#### Use Case: Locating Game Functions

**Scenario:** You want to find a specific function in a game's memory.

```
Game Module Memory
┌─────────────────────────────────────┐
│ 0x10000000: 48 8B C8 E8 ...        │
│ 0x10000010: 90 90 90 E9 ...        │ ← Looking for this
│ 0x10000020: C3 CC CC CC ...        │
└─────────────────────────────────────┘
```

**Code:**
```cpp
#include "Syx.h"

// Pattern: Three NOP instructions (0x90)
BYTE pattern[] = { 0x90, 0x90, 0x90 };
char mask[] = "xxx";  // All bytes must match

uintptr_t gameBase = Syx::GetModuleHandleEx("game.exe");
uintptr_t foundAddr = Syx::FindPattern(gameBase, 0x1000000, pattern, mask);

if (foundAddr) {
    printf("Function found at: 0x%llX\n", foundAddr);
}
```

#### Use Case: Pattern with Wildcards

**Scenario:** You know part of the pattern but some bytes vary.

```
Pattern:  E8 ?? ?? ?? ?? 48 85 C0
Meaning:  E8 [call offset] 48 85 C0
          │   └─────────┘  │
          │   Unknown      │
          Call instruction Test RAX,RAX
```

**Code:**
```cpp
const wchar_t* moduleName = L"UserAssembly.dll";
char pattern[] = "\xE8\x00\x00\x00\x00\x48\x85\xC0";
char mask[] = "x????xxx";  // ? = wildcard

uintptr_t result = Syx::FindPatternA(moduleName, pattern, mask);
```

### 2. Writing to Memory (Nested Pointers)

#### Use Case: Modifying Player Health

**Scenario:** Game stores player health through multiple pointers.

```
Memory Structure:
GameBase (0x10000000)
  └─[+0x10]─> Ptr1 (0x20000000)
      └─[+0x20]─> Ptr2 (0x30000000)
          └─[+0x30]─> Health (int)

Flow:
1. Read value at GameBase
2. Add 0x10, read pointer
3. Add 0x20, read pointer
4. Add 0x30, write health value
```

**Code:**
```cpp
uintptr_t gameBase = Syx::GetModuleHandleEx("game.exe");
std::vector<uintptr_t> offsets = { 0x10, 0x20, 0x30 };
int newHealth = 100;

try {
    Syx::WritePTR(gameBase, offsets, newHealth);
    printf("Health set to %d\n", newHealth);
} catch (const std::exception& e) {
    printf("Error writing to memory: %s\n", e.what());
}
```

### 3. Function Hooking

#### Use Case: Intercepting Function Calls

**Scenario:** You want to log or modify behavior when a function is called.

```
Normal Flow:
Game Code → Original Function → Continue

Hooked Flow:
Game Code → JMP → Your Hook Function → Original Code → Continue
                     │
                     └─ Log, modify params, etc.
```

**64-bit Hook Example:**
```cpp
#ifdef _WIN64

// Original function signature
typedef void(*ProcessEventFunc)(void* obj, void* params);
ProcessEventFunc originalProcessEvent = nullptr;

// Your hook function
void MyHookFunction(void* obj, void* params) {
    printf("ProcessEvent called!\n");
    
    // Modify parameters or behavior here
    
    // Call original function
    originalProcessEvent(obj, params);
}

// Install the hook
void InstallHook() {
    uintptr_t targetAddr = Syx::FindPatternA(L"game.dll", 
        "\x48\x89\x5C\x24", "xxxx");
    
    if (targetAddr) {
        originalProcessEvent = reinterpret_cast<ProcessEventFunc>(
            Syx::DetourFunc64(
                reinterpret_cast<BYTE*>(targetAddr),
                reinterpret_cast<BYTE*>(MyHookFunction),
                14  // Length of instructions to replace
            )
        );
        printf("Hook installed at 0x%llX\n", targetAddr);
    }
}

#endif
```

**32-bit Hook Example:**
```cpp
#ifdef _WIN32

typedef int(*GameTickFunc)(float deltaTime);
GameTickFunc originalTick = nullptr;

int MyTickHook(float deltaTime) {
    // Custom code before original
    printf("Tick: %.2f\n", deltaTime);
    
    // Call original
    return originalTick(deltaTime);
}

void InstallHook() {
    uintptr_t tickAddr = 0x12345678;  // Known address
    
    originalTick = reinterpret_cast<GameTickFunc>(
        Syx::DetourFunc32(
            reinterpret_cast<BYTE*>(tickAddr),
            reinterpret_cast<BYTE*>(MyTickHook),
            5  // Minimum length for JMP instruction
        )
    );
}

#endif
```

## Complete Examples

### Example 1: Simple Memory Scanner

```cpp
#include "Syx.h"
#include <iostream>

int main() {
    // Find all occurrences of a pattern
    const wchar_t* moduleName = L"target.dll";
    MODULEINFO modInfo = Syx::GetModuleInfo(moduleName);
    
    if (modInfo.lpBaseOfDll) {
        uintptr_t base = reinterpret_cast<uintptr_t>(modInfo.lpBaseOfDll);
        uintptr_t size = modInfo.SizeOfImage;
        
        BYTE pattern[] = { 0x48, 0x8B, 0xC8 };
        char mask[] = "xxx";
        
        printf("Scanning 0x%llX bytes...\n", size);
        
        uintptr_t result = Syx::FindPattern(base, size, pattern, mask);
        if (result) {
            printf("Pattern found at: 0x%llX\n", result);
            printf("Offset from base: 0x%llX\n", result - base);
        } else {
            printf("Pattern not found.\n");
        }
    }
    
    return 0;
}
```

### Example 2: Multi-level Pointer Manager

```cpp
#include "Syx.h"
#include <vector>
#include <string>

class PointerManager {
private:
    uintptr_t baseAddress;
    std::vector<uintptr_t> offsets;
    
public:
    PointerManager(const char* moduleName) {
        baseAddress = Syx::GetModuleHandleEx(moduleName);
    }
    
    void setOffsets(const std::vector<uintptr_t>& offs) {
        offsets = offs;
    }
    
    template<typename T>
    bool write(T value) {
        try {
            Syx::WritePTR(baseAddress, offsets, value);
            return true;
        } catch (...) {
            return false;
        }
    }
};

// Usage
int main() {
    PointerManager pm("game.dll");
    pm.setOffsets({ 0x10, 0x20, 0x30 });
    
    if (pm.write<int>(999)) {
        printf("Successfully wrote value!\n");
    }
    
    return 0;
}
```

## Best Practices

### ✓ Do's

1. **Always validate addresses before use:**
   ```cpp
   uintptr_t addr = Syx::FindPattern(...);
   if (addr) {
       // Safe to use
   }
   ```

2. **Use try-catch for memory operations:**
   ```cpp
   try {
       Syx::WritePTR(base, offsets, value);
   } catch (const std::exception& e) {
       // Handle error
   }
   ```

3. **Store hook lengths correctly:**
   - 32-bit: Minimum 5 bytes for JMP
   - 64-bit: Varies, check disassembly

### ✗ Don'ts

1. **Don't ignore null returns:**
   ```cpp
   // Bad
   auto addr = Syx::FindPattern(...);
   *reinterpret_cast<int*>(addr) = 0;  // Crash if addr is 0!
   ```

2. **Don't hook with insufficient length:**
   ```cpp
   // Bad - might corrupt instructions
   DetourFunc64(src, dst, 3);  // Too short!
   ```

3. **Don't forget error handling:**
   ```cpp
   // Bad - exception will crash program
   Syx::WritePTR(base, offsets, value);  // What if it fails?
   ```

## Troubleshooting

### Common Issues

| Issue | Cause | Solution |
|-------|-------|----------|
| Pattern not found | Wrong pattern/mask | Verify with memory viewer |
| Access violation | Invalid pointer | Check each offset level |
| Hook not working | Wrong instruction length | Use disassembler to verify |
| Module not found | Wrong name | Check with Process Explorer |

### Debug Tips

1. **Verify module is loaded:**
   ```cpp
   MODULEINFO info = Syx::GetModuleInfo(L"module.dll");
   if (info.lpBaseOfDll == nullptr) {
       printf("Module not loaded!\n");
   }
   ```

2. **Test pattern with known address:**
   ```cpp
   // If you know the address, verify pattern matches
   uintptr_t knownAddr = 0x12345678;
   BYTE* data = reinterpret_cast<BYTE*>(knownAddr);
   // Check first few bytes manually
   ```

3. **Log pointer chain:**
   ```cpp
   uintptr_t* ptr = reinterpret_cast<uintptr_t*>(base);
   for (size_t i = 0; i < offsets.size(); i++) {
       printf("Level %zu: 0x%llX\n", i, reinterpret_cast<uintptr_t>(ptr));
       ptr = reinterpret_cast<uintptr_t*>(*ptr + offsets[i]);
   }
   ```

## Performance Considerations

- Pattern scanning is O(n) - larger regions take longer
- Use smallest possible search region
- Cache found addresses instead of re-scanning
- Function hooks have minimal runtime overhead

## Security Notes

⚠️ **Warning**: This library modifies process memory and hooks functions.

- Only use on processes you own or have permission to modify
- Some anti-cheat systems detect these operations
- Always respect software licenses and terms of service
