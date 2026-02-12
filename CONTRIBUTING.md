# Contributing to SyxLib

First off, thank you for considering contributing to SyxLib! It's people like you that make SyxLib such a great tool.

## Table of Contents

- [Code of Conduct](#code-of-conduct)
- [How Can I Contribute?](#how-can-i-contribute)
  - [Reporting Bugs](#reporting-bugs)
  - [Suggesting Enhancements](#suggesting-enhancements)
  - [Pull Requests](#pull-requests)
- [Development Setup](#development-setup)
- [Coding Standards](#coding-standards)
- [Commit Message Guidelines](#commit-message-guidelines)

## Code of Conduct

This project and everyone participating in it is governed by respect and professionalism. By participating, you are expected to uphold this code.

## How Can I Contribute?

### Reporting Bugs

Before creating bug reports, please check existing issues to avoid duplicates. When you create a bug report, include as many details as possible:

- **Use a clear and descriptive title**
- **Describe the exact steps to reproduce the problem**
- **Provide specific examples**
- **Describe the behavior you observed and what you expected**
- **Include your environment details** (OS version, compiler, architecture)
- **Add code samples** if applicable

**Example Bug Report:**
```
Title: DetourFunc64 returns nullptr on Windows 11

Environment:
- Windows 11 Pro 22H2
- MSVC 2022
- x64 architecture

Steps to reproduce:
1. Call DetourFunc64 with valid parameters
2. Function returns nullptr
3. No memory is allocated

Expected: Hook should be installed
Actual: nullptr returned, hook fails

Code sample:
[paste minimal reproducible code]
```

### Suggesting Enhancements

Enhancement suggestions are tracked as GitHub issues. When creating an enhancement suggestion, include:

- **Clear and descriptive title**
- **Detailed description of the proposed functionality**
- **Explain why this enhancement would be useful**
- **List any alternatives you've considered**
- **Provide code examples** if possible

### Pull Requests

1. **Fork the repository** and create your branch from `main`
2. **Make your changes** following the coding standards
3. **Test your changes** thoroughly
4. **Update documentation** if needed
5. **Write clear commit messages**
6. **Submit a pull request**

#### Pull Request Checklist

- [ ] Code follows the project's coding standards
- [ ] Comments are added for complex logic
- [ ] Documentation is updated
- [ ] Examples are provided if adding new features
- [ ] Code compiles without warnings
- [ ] Changes are tested on both x86 and x64 if applicable

## Development Setup

### Requirements

- Windows 10 or later
- Visual Studio 2017 or later (or compatible C++ compiler)
- Windows SDK

### Building

SyxLib is a header-only library. To use it:

1. Clone the repository:
   ```bash
   git clone https://github.com/SyxMem/Syx-Memory.git
   cd Syx-Memory
   ```

2. Include the header in your project:
   ```cpp
   #include "Syx/Syx.h"
   ```

3. Link against required libraries:
   - Psapi.lib

### Testing Your Changes

1. Create a test application that uses the modified functionality
2. Test on both x86 and x64 architectures
3. Verify with different Windows versions if possible
4. Ensure no memory leaks or access violations occur

## Coding Standards

### C++ Style Guidelines

1. **Naming Conventions:**
   - Classes: PascalCase (e.g., `Syx`)
   - Functions: PascalCase (e.g., `FindPattern`)
   - Variables: camelCase (e.g., `baseAddress`)
   - Constants: UPPER_SNAKE_CASE (e.g., `MAX_SIZE`)

2. **Formatting:**
   - Use 4 spaces for indentation (no tabs)
   - Place opening braces on the same line
   - One statement per line
   - Maximum line length: 120 characters

3. **Documentation:**
   - Use Doxygen-style comments for functions
   - Document all parameters and return values
   - Add usage examples for complex functions

**Example:**
```cpp
/**
 * Searches for a pattern in a specified memory region.
 *
 * @param dwAddress Starting address of the memory region.
 * @param dwLen Length of the memory region to search.
 * @param bMask Pointer to the pattern mask.
 * @param szMask Pointer to the mask string ('x' = exact match, '?' = wildcard).
 * @return The address where the pattern was found, or 0 if not found.
 * 
 * @example
 * BYTE pattern[] = { 0x90, 0x90, 0x90 };
 * char mask[] = "xxx";
 * uintptr_t addr = Syx::FindPattern(base, size, pattern, mask);
 */
static uintptr_t FindPattern(uintptr_t dwAddress, uintptr_t dwLen, 
                             BYTE* bMask, char* szMask);
```

4. **Error Handling:**
   - Use exceptions for error conditions
   - Validate input parameters
   - Return meaningful error messages
   - Don't leave memory in inconsistent state

5. **Memory Management:**
   - Avoid memory leaks
   - Use RAII principles where applicable
   - Check pointers before dereferencing
   - Clean up resources in destructors

### Code Review Checklist

Before submitting code, ensure:

- [ ] Code is properly formatted
- [ ] All functions are documented
- [ ] Error handling is implemented
- [ ] No compiler warnings
- [ ] Memory leaks are checked
- [ ] Code is tested
- [ ] Examples are updated if needed

## Commit Message Guidelines

### Format

```
<type>: <subject>

<body>

<footer>
```

### Types

- **feat**: New feature
- **fix**: Bug fix
- **docs**: Documentation only changes
- **style**: Code style changes (formatting, no code change)
- **refactor**: Code refactoring
- **test**: Adding or updating tests
- **chore**: Maintenance tasks

### Examples

Good commit messages:
```
feat: Add support for AVX2 pattern scanning

Implement SIMD-accelerated pattern search using AVX2 instructions
for improved performance on modern processors.

Closes #123
```

```
fix: DetourFunc64 memory allocation failure on Windows 11

Fixed issue where VirtualAlloc would fail due to address space
constraints on Windows 11. Now properly iterates through memory
regions to find suitable allocation space.

Fixes #456
```

```
docs: Add troubleshooting guide for common hooking issues

Added detailed guide covering:
- Instruction boundary alignment
- Anti-cheat detection
- Debugging hook failures
```

## Questions?

Feel free to open an issue with the `question` label if you have any questions about contributing!

## License

By contributing to SyxLib, you agree that your contributions will be licensed under the MIT License.
