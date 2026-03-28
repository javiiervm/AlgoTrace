#pragma once
#include "Algorithm.hpp"
#include <vector>
#include <string>

// =========================================================================
// ALGOTRACE CONTRIBUTOR IMPLEMENTATION: Gnome Sort
// =========================================================================

class GnomeSort : public Algorithm {
private:
    // --- VISUAL STATE VARIABLES ---
    size_t currentIndex = 0;       
    
    // --- CODE ANIMATION & AUDIO VARIABLES ---
    std::vector<std::string> pseudoCode; 
    int currentCodeLine = 0;             
    int stepState = 0;         
    bool is_modifying = false; 

    // --- ALGORITHM SPECIFIC VARIABLES ---
    size_t pos = 0;
    size_t n = 0;

public:
    GnomeSort(std::vector<int>& d);
    void step() override;
    
    std::string getName() const override { return "Gnome Sort"; } 
    
    std::string getDescription() const override { 
        return "Moves an element to its proper place by a series of swaps, similar to a garden gnome rearranging flower pots."; 
    } 
    
    size_t getCurrentIndex() const override { return currentIndex; }
    const std::vector<std::string>& getCode() const override { return pseudoCode; }
    int getCurrentCodeLine() const override { return currentCodeLine; }
    bool isModifying() const override { return is_modifying; }
};