#pragma once
#include "Algorithm.hpp"
#include <vector>
#include <string>

// =========================================================================
// ALGOTRACE CONTRIBUTOR IMPLEMENTATION (HEADER): BubbleSort
// -------------------------------------------------------------------------
// CONTRIBUTOR NOTES:
// Bubble Sort works by repeatedly swapping adjacent elements if they are
// in the wrong order. This implementation uses visual pointers for the
// two inner loop indices being compared.
// =========================================================================

class BubbleSort : public Algorithm {
private:
    // --- 1. VISUAL STATE VARIABLES ---
    size_t primaryIndex = 0;       // Highlighted pointer 'j' (Cyan)
    size_t secondaryIndex = 0;     // Highlighted pointer 'j+1' (Custom color in visualizer)
    
    // --- 2. CODE ANIMATION & AUDIO VARIABLES ---
    std::vector<std::string> pseudoCode; 
    int currentCodeLine = 0;             
    int stepState = 0;         // State machine tracker to pause execution line-by-line
    bool is_modifying = false; // Flag to trigger the special "swap" sound

    // --- 3. ALGORITHM SPECIFIC VARIABLES ---
    size_t arr_size = 0;  // Current length of array
    size_t i = 0;         // Outer loop counter
    size_t j = 0;         // Inner loop counter
    bool swapped_on_pass = false; // Flag for optimization

public:
    // Constructor
    BubbleSort(std::vector<int>& d);
    
    // --- CORE LOGIC ---
    // Executed once every visual tick.
    void step() override;
    
    // --- GETTERS ---
    std::string getName() const override { return "Bubble Sort"; }
    std::string getDescription() const override { 
        return "Repeatedly steps through the list, comparing adjacent elements and swapping them if they are in the wrong order."; 
    }
    
    // Using Algorithm::getCurrentIndex getter to return primary pointer 'j'
    size_t getCurrentIndex() const override { return primaryIndex; }
    
    // Returns the pseudo-code array
    const std::vector<std::string>& getCode() const override { return pseudoCode; }
    
    // Returns the line of code currently being executed (0-indexed)
    int getCurrentCodeLine() const override { return currentCodeLine; }
    
    // Tells the audio engine if we are currently modifying the array
    bool isModifying() const override { return is_modifying; }
};