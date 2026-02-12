// Example 3: Function Hooking (Detouring)
// This example demonstrates how to hook/detour functions in both 32-bit and 64-bit

#include "../Syx/Syx.h"
#include <iostream>

// ============================================
// 64-bit Hook Example
// ============================================
#ifdef _WIN64

// Define the function signature we want to hook
typedef int(*TargetFunction)(int param1, int param2);

// Store the original function pointer
TargetFunction originalFunction = nullptr;

// Our hook/detour function
int HookedFunction(int param1, int param2)
{
    std::cout << "=== Hook Called ===" << std::endl;
    std::cout << "Original parameters: param1=" << param1 
              << ", param2=" << param2 << std::endl;
    
    // You can modify parameters here
    int modifiedParam1 = param1 * 2;
    int modifiedParam2 = param2 * 2;
    
    std::cout << "Modified parameters: param1=" << modifiedParam1 
              << ", param2=" << modifiedParam2 << std::endl;
    
    // Call the original function with modified or original parameters
    int result = originalFunction(modifiedParam1, modifiedParam2);
    
    std::cout << "Original function returned: " << result << std::endl;
    std::cout << "===================" << std::endl;
    
    // You can also modify the return value here
    return result;
}

void InstallHook64()
{
    std::cout << "\n[64-bit Hook Example]" << std::endl;
    
    // In a real scenario, you would find this address using pattern scanning
    // For this example, we'll use a hypothetical address
    // uintptr_t targetAddress = Syx::FindPatternA(L"game.dll", pattern, mask);
    uintptr_t targetAddress = 0x140001000;  // Example address
    
    if (targetAddress) {
        std::cout << "Target function at: 0x" << std::hex << targetAddress << std::endl;
        
        // Install the hook
        // The length (14 in this case) depends on the instruction length
        // You need to disassemble to ensure you're not breaking instructions
        originalFunction = reinterpret_cast<TargetFunction>(
            Syx::DetourFunc64(
                reinterpret_cast<BYTE*>(targetAddress),
                reinterpret_cast<BYTE*>(HookedFunction),
                14  // Length of instructions to overwrite
            )
        );
        
        if (originalFunction) {
            std::cout << "Hook installed successfully!" << std::endl;
            std::cout << "Original function stored at: 0x" << std::hex 
                      << reinterpret_cast<uintptr_t>(originalFunction) << std::endl;
        } else {
            std::cout << "Failed to install hook!" << std::endl;
        }
    } else {
        std::cout << "Target function not found!" << std::endl;
    }
}

#endif

// ============================================
// 32-bit Hook Example
// ============================================
#ifdef _WIN32

typedef int(*TargetFunction32)(int param);
TargetFunction32 originalFunction32 = nullptr;

int HookedFunction32(int param)
{
    std::cout << "=== 32-bit Hook Called ===" << std::endl;
    std::cout << "Parameter: " << param << std::endl;
    
    // Call original
    int result = originalFunction32(param);
    
    std::cout << "Result: " << result << std::endl;
    return result;
}

void InstallHook32()
{
    std::cout << "\n[32-bit Hook Example]" << std::endl;
    
    // Example target address
    uintptr_t targetAddress = 0x401000;
    
    if (targetAddress) {
        std::cout << "Target function at: 0x" << std::hex << targetAddress << std::endl;
        
        // Install the hook (minimum 5 bytes for JMP instruction)
        originalFunction32 = reinterpret_cast<TargetFunction32>(
            Syx::DetourFunc32(
                reinterpret_cast<BYTE*>(targetAddress),
                reinterpret_cast<BYTE*>(HookedFunction32),
                5  // Minimum 5 bytes for 32-bit JMP
            )
        );
        
        if (originalFunction32) {
            std::cout << "32-bit hook installed successfully!" << std::endl;
        } else {
            std::cout << "Failed to install 32-bit hook!" << std::endl;
        }
    }
}

#endif

// ============================================
// Main Function
// ============================================
int main()
{
    std::cout << "=== SyxLib Function Hooking Example ===" << std::endl;
    
    // Detect architecture
    #ifdef _WIN64
        std::cout << "Running in 64-bit mode" << std::endl;
        InstallHook64();
    #else
        std::cout << "Running in 32-bit mode" << std::endl;
        InstallHook32();
    #endif
    
    std::cout << "\n=== Important Notes ===" << std::endl;
    std::cout << "1. Always verify instruction boundaries before hooking" << std::endl;
    std::cout << "2. Use a disassembler to determine the correct length" << std::endl;
    std::cout << "3. Save the original function pointer to call it later" << std::endl;
    std::cout << "4. Test thoroughly - incorrect hooks can crash the process" << std::endl;
    std::cout << "5. Be aware of anti-cheat systems in games" << std::endl;
    
    std::cout << "\n=== Example Complete ===" << std::endl;
    
    return 0;
}

// ============================================
// Real-World Hook Example
// ============================================
/*
// Example: Hooking a game function that processes damage

#ifdef _WIN64

typedef void(*ProcessDamageFunc)(void* entity, float damage);
ProcessDamageFunc originalProcessDamage = nullptr;

void HookedProcessDamage(void* entity, float damage)
{
    // Log damage
    std::cout << "Entity taking damage: " << damage << std::endl;
    
    // Make player invulnerable by setting damage to 0
    if (IsPlayer(entity)) {
        damage = 0.0f;
        std::cout << "Player damage nullified!" << std::endl;
    }
    
    // Call original function
    originalProcessDamage(entity, damage);
}

void HookDamageSystem()
{
    // Find the ProcessDamage function
    char pattern[] = "\x48\x89\x5C\x24\x08\x57\x48\x83\xEC\x20";
    char mask[] = "xxxxxxxxxx";
    
    uintptr_t damageFunc = Syx::FindPatternA(L"game.dll", pattern, mask);
    
    if (damageFunc) {
        originalProcessDamage = reinterpret_cast<ProcessDamageFunc>(
            Syx::DetourFunc64(
                reinterpret_cast<BYTE*>(damageFunc),
                reinterpret_cast<BYTE*>(HookedProcessDamage),
                15
            )
        );
        
        if (originalProcessDamage) {
            std::cout << "Damage system hooked successfully!" << std::endl;
        }
    }
}

#endif
*/
