// Example 2: Memory Writing with Nested Pointers
// This example demonstrates how to write values through pointer chains

#include "../Syx/Syx.h"
#include <iostream>
#include <vector>

int main()
{
    std::cout << "=== SyxLib Memory Writing Example ===" << std::endl;
    
    // Example: Writing to a nested pointer structure
    // Real-world scenario: Game data is often stored like this:
    // BaseAddress -> [Offset1] -> [Offset2] -> [Offset3] -> ActualValue
    
    // Get the module base address (use your target module name)
    const char* moduleName = nullptr;  // nullptr = current executable
    uintptr_t baseAddress = Syx::GetModuleHandleEx(moduleName);
    
    if (!baseAddress) {
        std::cerr << "Failed to get module base address!" << std::endl;
        return 1;
    }
    
    std::cout << "Base address: 0x" << std::hex << baseAddress << std::endl;
    
    // Define the pointer chain offsets
    // These would be determined through reverse engineering
    std::vector<uintptr_t> offsets = { 0x10, 0x20, 0x30 };
    
    std::cout << "\nPointer chain: " << std::endl;
    std::cout << "  Base: 0x" << std::hex << baseAddress << std::endl;
    for (size_t i = 0; i < offsets.size(); i++) {
        std::cout << "  [+" << std::dec << i << "] Offset: 0x" 
                  << std::hex << offsets[i] << std::endl;
    }
    
    // Example 1: Writing an integer value
    std::cout << "\n[Example 1] Writing integer value" << std::endl;
    int valueToWrite = 12345;
    
    try {
        Syx::WritePTR(baseAddress, offsets, valueToWrite);
        std::cout << "Successfully wrote value: " << std::dec << valueToWrite << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error writing to memory: " << e.what() << std::endl;
    }
    
    // Example 2: Writing a float value
    std::cout << "\n[Example 2] Writing float value" << std::endl;
    float floatValue = 99.99f;
    
    try {
        Syx::WritePTR(baseAddress, offsets, floatValue);
        std::cout << "Successfully wrote float value: " << floatValue << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error writing to memory: " << e.what() << std::endl;
    }
    
    // Example 3: Writing a double value
    std::cout << "\n[Example 3] Writing double value" << std::endl;
    double doubleValue = 123.456;
    
    try {
        Syx::WritePTR(baseAddress, offsets, doubleValue);
        std::cout << "Successfully wrote double value: " << doubleValue << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error writing to memory: " << e.what() << std::endl;
    }
    
    // Example 4: Handling errors gracefully
    std::cout << "\n[Example 4] Error handling" << std::endl;
    std::vector<uintptr_t> invalidOffsets = { 0xDEADBEEF };  // Invalid offset
    
    try {
        Syx::WritePTR(baseAddress, invalidOffsets, 42);
        std::cout << "Value written (this shouldn't print)" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "Caught expected error: " << e.what() << std::endl;
        std::cout << "Error handling works correctly!" << std::endl;
    }
    
    std::cout << "\n=== Example Complete ===" << std::endl;
    std::cout << "\nNote: In a real application, you would:" << std::endl;
    std::cout << "  1. Use valid offsets from reverse engineering" << std::endl;
    std::cout << "  2. Validate addresses before writing" << std::endl;
    std::cout << "  3. Handle errors appropriately" << std::endl;
    
    return 0;
}
