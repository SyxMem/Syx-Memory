# SyxLib Quick Reference Card

## 🎯 At a Glance

### Include & Setup
```cpp
#include "Syx.h"
// Link: Psapi.lib
```

## 📋 Function Reference

### Memory Operations

| Function | Purpose | Return |
|----------|---------|--------|
| `GetModuleHandleEx(name)` | Get module base | `uintptr_t` |
| `WritePTR(base, offsets, value)` | Write via pointer chain | `void` |

```cpp
// Get module
uintptr_t base = Syx::GetModuleHandleEx("game.dll");

// Write value
std::vector<uintptr_t> offsets = {0x10, 0x20};
Syx::WritePTR(base, offsets, 999);
```

### Pattern Scanning

| Function | Purpose | Return |
|----------|---------|--------|
| `FindPattern(addr, len, pattern, mask)` | Manual length | `uintptr_t` |
| `FindPatternA(module, pattern, mask)` | Auto length | `uintptr_t` |
| `DataCompare(data, pattern, mask)` | Compare bytes | `bool` |

```cpp
// Method 1: Manual
BYTE pat[] = {0x90, 0x90};
char mask[] = "xx";
uintptr_t addr = Syx::FindPattern(0x10000000, 0x1000, pat, mask);

// Method 2: Auto
char pat2[] = "\xE8\x00\x00\x00\x00";
char mask2[] = "x????";
uintptr_t addr2 = Syx::FindPatternA(L"game.dll", pat2, mask2);
```

### Function Hooking

| Function | Architecture | Min Length |
|----------|--------------|------------|
| `DetourFunc32(src, dst, len)` | x86 | 5 bytes |
| `DetourFunc64(src, dst, len)` | x64 | 14 bytes |

```cpp
// 64-bit hook
typedef void(*FuncType)(int);
FuncType original = reinterpret_cast<FuncType>(
    Syx::DetourFunc64((BYTE*)0x140001000, (BYTE*)MyHook, 14)
);

// 32-bit hook  
typedef int(*FuncType32)(float);
FuncType32 original32 = reinterpret_cast<FuncType32>(
    Syx::DetourFunc32((BYTE*)0x401000, (BYTE*)MyHook32, 5)
);
```

## 🎨 Patterns & Masks

### Mask Syntax
- `x` = Exact byte match required
- `?` = Wildcard (any byte)

### Examples
```
Pattern: 48 8B C8 E8 ?? ?? ?? ?? 90
Mask:    x  x  x  x  ?  ?  ?  ?  x
Meaning: Match 48 8B C8 E8, ignore 4 bytes, match 90
```

## 🔍 Common Patterns

### Pattern Types
| Pattern | Purpose |
|---------|---------|
| `48 89 5C 24 08` | Function prologue (x64) |
| `55 8B EC` | Function prologue (x86) |
| `E8 ?? ?? ?? ??` | CALL instruction |
| `E9 ?? ?? ?? ??` | JMP instruction |
| `90 90 90` | NOP sled |
| `C3` | RET instruction |

## ⚡ Quick Examples

### Find and Modify
```cpp
// Find pattern
uintptr_t addr = Syx::FindPatternA(L"game.dll", 
    "\x48\x8B\xC8", "xxx");

// Write through pointers
if (addr) {
    std::vector<uintptr_t> offs = {0x10, 0x20};
    Syx::WritePTR(addr, offs, 9999);
}
```

### Hook Function
```cpp
#ifdef _WIN64
typedef int(*OrigFunc)(int, int);
OrigFunc orig = nullptr;

int MyHook(int a, int b) {
    return orig(a * 2, b * 2);  // Modify behavior
}

void Install() {
    uintptr_t target = /* find address */;
    orig = (OrigFunc)Syx::DetourFunc64(
        (BYTE*)target, (BYTE*)MyHook, 14
    );
}
#endif
```

## 🛡️ Error Handling

```cpp
// Always validate
uintptr_t addr = Syx::FindPattern(...);
if (!addr) {
    printf("Pattern not found!\n");
    return;
}

// Use try-catch
try {
    Syx::WritePTR(base, offsets, value);
} catch (const std::exception& e) {
    printf("Error: %s\n", e.what());
}
```

## ⚠️ Common Mistakes

| ❌ Wrong | ✅ Correct |
|---------|-----------|
| `DetourFunc64(..., 5)` | `DetourFunc64(..., 14)` |
| `char mask[] = "x?x"` with `{0x90,0x00,0x90}` | Use actual bytes for pattern |
| Ignoring return value | Check if address != 0 |
| No try-catch on WritePTR | Wrap in try-catch |

## 🔧 Debugging Tips

```cpp
// 1. Verify module
MODULEINFO info = Syx::GetModuleInfo(L"game.dll");
printf("Base: %p, Size: %X\n", info.lpBaseOfDll, info.SizeOfImage);

// 2. Test pattern at known address
uintptr_t known = 0x12345678;
BYTE* data = (BYTE*)known;
// Check bytes match

// 3. Log pointer chain
uintptr_t* ptr = (uintptr_t*)base;
for (int i = 0; i < offsets.size(); i++) {
    printf("Lvl %d: %p\n", i, ptr);
    ptr = (uintptr_t*)(*ptr + offsets[i]);
}
```

## 📊 Architecture Comparison

| Feature | x86 | x64 |
|---------|-----|-----|
| Pointer size | 4 bytes | 8 bytes |
| Min hook size | 5 bytes | 14 bytes |
| JMP encoding | E9 rel32 | FF 25 [rip+off] |
| Hook function | DetourFunc32 | DetourFunc64 |

## 🚀 Performance

- Pattern scan: O(n) - minimize search region
- WritePTR: O(depth) - minimize pointer chain
- Hooks: Minimal overhead after installation

## 📚 Resources

- **Docs**: `docs/architecture.md`, `docs/usage-guide.md`
- **Examples**: `examples/*.cpp`
- **Full API**: `README.md`

---

**Version 1.3** | [GitHub](https://github.com/SyxMem/Syx-Memory) | [Website](https://sysmem.syferx.net)
