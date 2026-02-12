# SyxLib Examples

This directory contains complete, working examples demonstrating various SyxLib features.

## 📂 Available Examples

### 1. [pattern_scanning.cpp](pattern_scanning.cpp)
Demonstrates pattern scanning techniques:
- Manual length pattern scanning
- Automatic length scanning
- Using wildcards in patterns
- Module-based scanning

**Topics Covered:**
- `FindPattern()` - Manual search
- `FindPatternA()` - Automatic search
- Wildcard masks with '?' character
- Module information retrieval

**Compile:**
```bash
g++ pattern_scanning.cpp -lPsapi -o pattern_scanning.exe
```

---

### 2. [memory_writing.cpp](memory_writing.cpp)
Shows how to write values through nested pointer chains:
- Writing integers, floats, doubles
- Multi-level pointer dereferencing
- Error handling
- Type-safe memory operations

**Topics Covered:**
- `WritePTR()` with different types
- Pointer chain offsets
- Exception handling
- Validating addresses

**Compile:**
```bash
g++ memory_writing.cpp -lPsapi -o memory_writing.exe
```

---

### 3. [function_hooking.cpp](function_hooking.cpp)
Comprehensive function hooking examples:
- 64-bit function detouring
- 32-bit function detouring
- Calling original functions
- Real-world hooking scenarios

**Topics Covered:**
- `DetourFunc64()` for x64 hooks
- `DetourFunc32()` for x86 hooks
- Hook function signatures
- Instruction length considerations

**Compile:**
```bash
# For 64-bit
g++ function_hooking.cpp -lPsapi -m64 -o function_hooking_x64.exe

# For 32-bit
g++ function_hooking.cpp -lPsapi -m32 -o function_hooking_x86.exe
```

---

## 🚀 Quick Start

### Compiling All Examples

Using Visual Studio:
```cmd
cl /EHsc pattern_scanning.cpp /link Psapi.lib
cl /EHsc memory_writing.cpp /link Psapi.lib
cl /EHsc function_hooking.cpp /link Psapi.lib
```

Using g++/MinGW:
```bash
g++ pattern_scanning.cpp -lPsapi -o pattern_scanning.exe
g++ memory_writing.cpp -lPsapi -o memory_writing.exe
g++ function_hooking.cpp -lPsapi -o function_hooking.exe
```

### Running Examples

Most examples are demonstrations and will run showing example output:
```bash
./pattern_scanning.exe
./memory_writing.exe
./function_hooking.exe
```

## 📝 Notes

- **Educational Purpose**: These examples are for learning and demonstration
- **Modify for Your Use**: Adapt addresses, patterns, and offsets for your specific needs
- **Test Safely**: Always test in controlled environments
- **Error Handling**: Examples include proper error handling patterns

## 🔍 Understanding the Code

Each example includes:
- ✅ Detailed comments explaining each step
- ✅ Error handling demonstrations
- ✅ Best practices
- ✅ Real-world use cases
- ✅ Output showing what to expect

## 🛠️ Customization

To adapt examples for your needs:

1. **Pattern Scanning**: Replace patterns with bytes from your target
2. **Memory Writing**: Update offsets based on your pointer chain
3. **Function Hooking**: Find target addresses using pattern scanning

## ⚠️ Important Warnings

- These examples modify process memory
- Some examples require valid target addresses to work
- Always respect software licenses and terms of service
- Some operations may be detected by anti-cheat systems

## 📚 Learn More

- **Main README**: [../README.md](../README.md)
- **Architecture Guide**: [../docs/architecture.md](../docs/architecture.md)
- **Usage Guide**: [../docs/usage-guide.md](../docs/usage-guide.md)
- **Contributing**: [../CONTRIBUTING.md](../CONTRIBUTING.md)

## 💡 Tips

1. **Start Simple**: Begin with pattern_scanning.cpp
2. **Understand Pointers**: Study memory_writing.cpp carefully
3. **Learn Disassembly**: Essential for function_hooking.cpp
4. **Use Debuggers**: Tools like x64dbg help understand memory layout
5. **Read Documentation**: Check inline comments and docs folder

## 🐛 Troubleshooting

**Example won't compile?**
- Ensure you're linking Psapi.lib
- Check include path to Syx.h
- Verify compiler supports C++11 or later

**Example crashes?**
- Examples use hypothetical addresses
- Replace with valid addresses from your target process
- Ensure proper architecture (x86/x64) matching

**Pattern not found?**
- Verify pattern bytes are correct
- Check mask syntax ('x' for exact, '?' for wildcard)
- Ensure module name is correct

---

<div align="center">

**Happy Learning! 🎓**

Need help? [Open an issue](https://github.com/SyxMem/Syx-Memory/issues)

</div>
