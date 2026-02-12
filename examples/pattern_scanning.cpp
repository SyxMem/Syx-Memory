// Example 1: Basic Pattern Scanning
// This example demonstrates how to find a specific byte pattern in memory

#include "../Syx/Syx.h"
#include <iostream>

int main()
{
    std::cout << "=== SyxLib Pattern Scanning Example ===" << std::endl;
    
    // Example 1: Manual length pattern scanning
    std::cout << "\n[Example 1] Manual Length Pattern Scanning" << std::endl;
    
    // Define the pattern we're looking for (three NOP instructions)
    BYTE pattern[] = { 0x90, 0x90, 0x90 };
    char mask[] = "xxx";  // 'x' means byte must match exactly
    
    // Get the base address of the current module
    uintptr_t moduleBase = Syx::GetModuleHandleEx(nullptr); // nullptr = current exe
    
    if (moduleBase) {
        std::cout << "Module base address: 0x" << std::hex << moduleBase << std::endl;
        
        // Search for the pattern in the first 0x100000 bytes
        uintptr_t foundAddress = Syx::FindPattern(moduleBase, 0x100000, pattern, mask);
        
        if (foundAddress) {
            std::cout << "Pattern found at: 0x" << std::hex << foundAddress << std::endl;
            std::cout << "Offset from base: 0x" << (foundAddress - moduleBase) << std::endl;
        } else {
            std::cout << "Pattern not found in specified range." << std::endl;
        }
    }
    
    // Example 2: Pattern with wildcards
    std::cout << "\n[Example 2] Pattern Scanning with Wildcards" << std::endl;
    
    // This pattern has wildcards (?) for bytes that can vary
    // Useful for finding call instructions where the offset changes
    char wildcardPattern[] = "\xE8\x00\x00\x00\x00\x48\x8B";  // CALL ?????, MOV
    char wildcardMask[] = "x????xx";  // '?' means ignore this byte
    
    const wchar_t* targetModule = nullptr;  // Current process
    MODULEINFO modInfo = Syx::GetModuleInfo(targetModule);
    
    if (modInfo.lpBaseOfDll) {
        std::cout << "Module size: 0x" << std::hex << modInfo.SizeOfImage << " bytes" << std::endl;
        
        uintptr_t result = Syx::FindPatternA(targetModule, wildcardPattern, wildcardMask);
        
        if (result) {
            std::cout << "Pattern with wildcards found at: 0x" << std::hex << result << std::endl;
        } else {
            std::cout << "Pattern not found." << std::endl;
        }
    }
    
    std::cout << "\n=== Example Complete ===" << std::endl;
    
    return 0;
}
