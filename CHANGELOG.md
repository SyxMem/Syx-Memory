# Changelog

All notable changes to SyxLib will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [1.3] - 2023-XX-XX

### Current Version Features

#### Added
- Memory comparison using masks with `DataCompare`
- Pattern searching in memory regions with `FindPattern`
- Auto-length pattern searching with `FindPatternA`
- Nested pointer dereferencing for writing values with `WritePTR`
- Module base address retrieval with `GetModuleHandleEx`
- Function detouring for 32-bit environments with `DetourFunc32`
- Function detouring for 64-bit environments with `DetourFunc64`
- Template-based memory writing supporting multiple data types

#### Features
- **Pattern Scanning**
  - Manual length specification
  - Automatic length calculation
  - Wildcard support for flexible matching
  - Module-based scanning

- **Memory Operations**
  - Type-safe pointer writing
  - Multi-level pointer dereferencing
  - Automatic error handling
  - Exception-based error reporting

- **Function Hooking**
  - 32-bit and 64-bit support
  - Instruction preservation
  - Original function calling
  - Memory page allocation for hooks

## [Future Releases]

### Planned Features

#### Version 1.4 (Planned)
- [ ] Read memory through pointer chains (ReadPTR)
- [ ] Batch pattern scanning for multiple patterns
- [ ] Memory region enumeration
- [ ] Process handle management utilities
- [ ] Improved error messages with context

#### Version 1.5 (Planned)
- [ ] Cross-process memory operations
- [ ] Pattern caching for performance
- [ ] SIMD-accelerated pattern scanning
- [ ] Hook management system
- [ ] Automatic hook restoration

#### Version 2.0 (Future)
- [ ] Support for ARM64 architecture
- [ ] Linux/Unix compatibility layer
- [ ] Python bindings
- [ ] GUI tool for pattern finding
- [ ] Comprehensive test suite

## Version History

### [1.3] - Current
- Full-featured memory manipulation library
- Complete function detouring support
- Comprehensive documentation

### [1.2] - Previous (Estimated)
- Added 64-bit function detouring
- Enhanced pattern scanning
- Improved error handling

### [1.1] - Previous (Estimated)
- Added auto-length pattern scanning
- Introduced WritePTR function
- Basic module information retrieval

### [1.0] - Initial Release (Estimated)
- Basic pattern scanning functionality
- Simple memory operations
- 32-bit support

---

## Contributing

See [CONTRIBUTING.md](CONTRIBUTING.md) for how to contribute to SyxLib.

## Support

For questions, bug reports, or feature requests, please open an issue on GitHub:
https://github.com/SyxMem/Syx-Memory/issues
