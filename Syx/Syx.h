#include <stdexcept>
#include <vector>
#include <Windows.h>

class Syx
{
public:
    /**
     * Constructs a Syx instance.
     *
     * @param baseAddress The base address to start the nested pointer dereferencing.
     */
    Syx(uintptr_t baseAddress) : baseAddress_(baseAddress) {}

    /**
     * Writes a value to a memory location specified by a nested pointer.
     * This function performs nested pointer dereferencing to calculate the final memory address.
     * It checks the validity of each pointer level and throws an exception if an invalid pointer is encountered.
     *
     * @param offsets A vector of offsets that define the path to the desired memory location.
     * @param valueHere The value to be written to the memory location.
     *
     * @throw std::runtime_error if an invalid pointer is encountered during the dereferencing process.
     */
    template <typename T>
    void WritePTR(const std::vector<uintptr_t>& offsets, T valueHere)
    {
        uintptr_t* currentPtr = (uintptr_t*)baseAddress_;

        // Perform nested pointer dereferencing
        try {
            for (size_t i = 0; i < offsets.size(); i++) {
                // Check if the current pointer is null or invalid
                if (!currentPtr) {
                    throw std::runtime_error("Invalid pointer encountered.");
                }

                // Update the pointer to the next level
                currentPtr = (uintptr_t*)(*currentPtr + offsets[i]);
            }

            // Check if the final pointer is valid for writing
            if (!currentPtr) {
                throw std::runtime_error("Invalid pointer encountered.");
            }

            // Assign the desired value to the memory location
            *reinterpret_cast<T*>(currentPtr) = valueHere;
        }
        catch (const std::exception& e) {
            // Handle the error or return an appropriate value
            // Example: log an error or throw the exception to the caller
            // ...
            printf("%s", e.what());

            // In this example, we throw the exception to the caller
            throw;
        }
    }

    /**
     * Retrieves the base address of a module loaded in the current process.
     *
     * @param moduleName The name of the module.
     * @return The base address of the module if found, or nullptr if the module is not loaded.
     */
    uintptr_t GetModuleHandleEx(const char* moduleName)
    {
        return reinterpret_cast<uintptr_t>(GetModuleHandleA(moduleName));
    }

private:
    uintptr_t baseAddress_;
};
