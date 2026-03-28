#pragma once
#include "Algorithm.hpp"
#include <vector>
#include <string>

// =========================================================================
// ALGOTRACE CONTRIBUTOR IMPLEMENTATION: Pancake Sort
// =========================================================================

class PancakeSort : public Algorithm {
private:
    // --- VISUAL STATE VARIABLES ---
    size_t currentIndex = 0;       
    
    // --- CODE ANIMATION & AUDIO VARIABLES ---
    std::vector<std::string> pseudoCode; 
    int currentCodeLine = 0;             
    int stepState = 0;         
    bool is_modifying = false; 

    // --- ALGORITHM SPECIFIC VARIABLES ---
    size_t n = 0;
    size_t currSize = 0;
    size_t maxIndex = 0;
    size_t j = 0;
    
    // Variables para simular el bucle "flip" paso a paso
    size_t flipStart = 0;
    size_t flipEnd = 0;

public:
    PancakeSort(std::vector<int>& d);
    void step() override;
    
    std::string getName() const override { return "Pancake Sort"; } 
    
    std::string getDescription() const override { 
        return "Sorts an array by repeatedly finding the maximum element and flipping the sub-array to move it to its correct position."; 
    } 
    
    size_t getCurrentIndex() const override { return currentIndex; }
    const std::vector<std::string>& getCode() const override { return pseudoCode; }
    int getCurrentCodeLine() const override { return currentCodeLine; }
    bool isModifying() const override { return is_modifying; }
};