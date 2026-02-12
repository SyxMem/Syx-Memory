# SyxLib Architecture

## Library Overview

```
┌─────────────────────────────────────────────────────────────┐
│                        SyxLib                                │
│                   Memory Operations Library                   │
└─────────────────────────────────────────────────────────────┘
                             │
        ┌────────────────────┼────────────────────┐
        │                    │                    │
        ▼                    ▼                    ▼
┌──────────────┐    ┌──────────────┐    ┌──────────────┐
│   Memory     │    │   Pattern    │    │   Function   │
│  Operations  │    │   Scanning   │    │   Hooking    │
└──────────────┘    └──────────────┘    └──────────────┘
        │                    │                    │
        ▼                    ▼                    ▼
┌──────────────┐    ┌──────────────┐    ┌──────────────┐
│ - WritePTR   │    │ - FindPattern│    │- DetourFunc32│
│ - GetModule  │    │- FindPatternA│    │- DetourFunc64│
│   HandleEx   │    │- DataCompare │    │              │
└──────────────┘    └──────────────┘    └──────────────┘
```

## Component Details

### 1. Memory Operations
Handles reading and writing to memory locations with support for nested pointers.

**Key Functions:**
- `WritePTR`: Write values through nested pointer chains
- `GetModuleHandleEx`: Get base address of loaded modules

### 2. Pattern Scanning
Searches for byte patterns in process memory with wildcard support.

**Key Functions:**
- `FindPattern`: Manual length pattern search
- `FindPatternA`: Auto-calculated length search
- `DataCompare`: Byte-by-byte comparison with masks

### 3. Function Hooking
Redirects function execution for both 32-bit and 64-bit applications.

**Key Functions:**
- `DetourFunc32`: Hook functions in 32-bit processes
- `DetourFunc64`: Hook functions in 64-bit processes

## Memory Layout

### Nested Pointer Dereferencing
```
Base Address (0x10000000)
    │
    ├─ Offset 1 (0x10) ──> Pointer 1 (0x20000000)
    │                           │
    │                           ├─ Offset 2 (0x20) ──> Pointer 2 (0x30000000)
    │                           │                           │
    │                           │                           ├─ Offset 3 (0x30) ──> Final Value
```

### Pattern Matching Process
```
Memory Region: [0x12345678 - 0x12345778]
Pattern:       [0x90, 0x90, 0x90]
Mask:          "xxx" (all bytes must match exactly)

Scan Process:
┌──────────────────────────────────────┐
│ Address  │ Bytes         │ Match?    │
├──────────────────────────────────────┤
│ 12345678 │ 48 8B C8      │ No        │
│ 12345679 │ 8B C8 E8      │ No        │
│ 1234567A │ 90 90 90      │ Yes! ✓    │
└──────────────────────────────────────┘
```

## Function Detour Mechanism

### 64-bit Detour
```
Original Function:              After Detour:
┌────────────┐                 ┌────────────┐
│ Original   │                 │ JMP far    │──┐
│ Code       │                 │ [rip+X]    │  │
│ ...        │                 └────────────┘  │
└────────────┘                                 │
                                               │
                                               ▼
                                        ┌──────────────┐
                                        │ Hook Memory  │
                                        ├──────────────┤
                                        │ Original     │
                                        │ Code Copy    │
                                        ├──────────────┤
                                        │ JMP back     │
                                        └──────────────┘
                                               │
                                               ▼
                                        ┌──────────────┐
                                        │ Your Detour  │
                                        │ Function     │
                                        └──────────────┘
```

### 32-bit Detour
```
Original Function:              After Detour:
┌────────────┐                 ┌────────────┐
│ Original   │                 │ JMP rel32  │──┐
│ Code       │                 │ (E9 XX XX) │  │
│ ...        │                 │ NOP NOP    │  │
└────────────┘                 └────────────┘  │
                                               │
                                               ▼
                                        ┌──────────────┐
                                        │ Your Detour  │
                                        │ Function     │
                                        └──────────────┘
```

## Thread Safety Considerations

⚠️ **Important**: SyxLib functions are **not thread-safe** by default.

- Memory writes should be synchronized
- Pattern searches read memory and can be safely parallelized
- Function detouring should be done before multi-threading or with proper synchronization

## Platform Support

| Feature          | Windows x86 | Windows x64 |
|------------------|-------------|-------------|
| Memory Ops       | ✓           | ✓           |
| Pattern Search   | ✓           | ✓           |
| DetourFunc32     | ✓           | ✓*          |
| DetourFunc64     | ✗           | ✓           |

*Can hook 32-bit code in WoW64 processes
