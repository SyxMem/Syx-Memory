# Syx

Syx is a C++ library that provides memory access and manipulation capabilities for a target process. It allows you to read and write values to specific memory locations, perform nested pointer dereferencing, and retrieve the base address of loaded modules.

## Features

- Write values to memory locations specified by nested pointers
- Perform nested pointer dereferencing for complex data structures
- Retrieve the base address of loaded modules by name
- Robust error handling for invalid memory accesses

## Installation

To use Syx in your C++ project, follow these steps:

1. Clone the repository:

   ```shell
   git clone https://github.com/your-organization/syx.git
   ```

1. Include the necessary header files in your project:
    ```cpp
    #include "syx/syx.h"
    ```

2. Build and link the Syx library in your project.




# Usage

1. Create an instance of the `Syx` class by providing the base address of the target process.

   ```cpp
   uintptr_t baseAddress = /* Set the base address of your target process */;
   Syx syx(baseAddress);
   ```

2. Use the available functions to access and manipulate memory in the target process. 
   - WritePTR: Write a value to a memory location specified by a nested pointer.
      ```cpp
      std::vector<uintptr_t> offsets = { 0x10, 0x20, 0x30 }; // Example offsets
      int valueToWrite = 42;
      syx.WritePTR(offsets, valueToWrite);
      ```

   - GetModuleHandleEx: Get the base address of a loaded module by name.
     ```cpp
      const char* moduleName = "kernel32.dll"; // Example module name
      uintptr_t moduleBase = syx.GetModuleHandleEx(moduleName);
      if (moduleBase != 0) {
        // Module found, perform operations with the module base address
      } else {
        // Module not found
      }
     ```

   - Handle exceptions for error scenarios. The functions may throw `std::runtime_error` if an invalid memory access is encountered.
     ```cpp
      try {
        // Use Syx functions
      } catch (const std::runtime_error& e) {
        // Handle the error, such as logging or displaying an error message
      std::cerr << "Error: " << e.what() << std::endl;
      }
     ```

## Upcoming Features

The following features are planned for future updates:

- Read values from memory locations specified by nested pointers.
- Enumerate and access process modules.
- Read and write structures or objects in memory.
- Support for more advanced memory manipulation techniques.

Stay tuned for updates!

# Contributing
Contributions are welcome! If you have any suggestions, bug reports, or feature requests, please open an issue or submit a pull request.

# License
This project is licensed under the MIT License.

# Contact
For inquiries, please contact:

Afdul
GitHub: Afdul2021

Email: safetynet1333712@gmail.com
