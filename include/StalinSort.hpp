#pragma once
#include "Algorithm.hpp"
#include <vector>
#include <string>

// =========================================================================
// ALGOTRACE CONTRIBUTOR IMPLEMENTATION: StalinSort (HEADER)
// =========================================================================

class StalinSort : public Algorithm {
private:
    // --- 1. VISUAL STATE VARIABLES ---
    size_t currentIndex = 1;       // The primary bar currently being checked (Cyan)
    
    // --- 2. CODE ANIMATION & AUDIO VARIABLES ---
    std::vector<std::string> pseudoCode; 
    int currentCodeLine = 0;             
    int stepState = 0;         // State machine tracker
    bool is_modifying = false; // Flag to trigger the special "eliminate" sound

    // --- 3. ALGORITHM SPECIFIC VARIABLES ---
    std::vector<bool> eliminated; // Tracks which elements have been "purged"
    int currentMax = 0;           // The highest value seen so far

public:
    // Constructor
    StalinSort(std::vector<int>& d);
    
    // --- CORE LOGIC ---
    void step() override;
    
    // --- GETTERS ---
    std::string getName() const override { return "Stalin Sort"; } 
    
    // Brief explanation of the algorithm's logic
    std::string getDescription() const override { 
        return "Anything out of order gets deleted, the survivors are ordered."; 
    } 
    
    size_t getCurrentIndex() const override { return currentIndex; }
    const std::vector<std::string>& getCode() const override { return pseudoCode; }
    int getCurrentCodeLine() const override { return currentCodeLine; }
    bool isModifying() const override { return is_modifying; }
    
    // Override to tell the Visualizer which bars to shrink visually
    bool isEliminated(size_t index) const override { 
        if (index < eliminated.size()) return eliminated[index];
        return false;
    }
};