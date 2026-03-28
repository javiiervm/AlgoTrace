#pragma once
#include "Algorithm.hpp"
#include <vector>
#include <string>

// =========================================================================
// ALGOTRACE CONTRIBUTOR TEMPLATE (HEADER)
// -------------------------------------------------------------------------
// INSTRUCTIONS:
// 1. Rename 'TemplateSort' to your algorithm's name (e.g., 'BubbleSort').
// 2. Add any specific variables your algorithm needs in the 'private' section
//    (like loop counters, temporary swap values, etc.).
// 3. Implement the methods in the corresponding .cpp file.
// =========================================================================

class TemplateSort : public Algorithm {
private:
    // --- 1. VISUAL STATE VARIABLES ---
    // These variables tell the Visualizer what to highlight on the screen.
    size_t currentIndex = 0;       // The primary bar currently being checked (Cyan)
    size_t secondaryIndex = 0;     // (Optional) A secondary bar being compared
    
    // --- 2. CODE ANIMATION & AUDIO VARIABLES ---
    // These handle the pseudo-code box at the bottom and the sound effects.
    std::vector<std::string> pseudoCode; 
    int currentCodeLine = 0;             
    int stepState = 0;         // State machine tracker to pause execution line-by-line
    bool is_modifying = false; // Flag to trigger the special "swap/eliminate" sound

    // --- 3. ALGORITHM SPECIFIC VARIABLES ---
    // [ADD YOUR VARIABLES HERE]
    // Example for Bubble Sort:
    // int i = 0;
    // int j = 0;

public:
    // Constructor
    TemplateSort(std::vector<int>& d);
    
    // --- CORE LOGIC ---
    // This is called once every frame/tick. It must execute ONLY ONE tiny step
    // of the algorithm (e.g., one comparison or one swap) and then return.
    void step() override;
    
    // --- GETTERS (Do not change the signature of these) ---
    
    // Returns the display name for the Main Menu
    std::string getName() const override { return "Template Sort"; } // <-- CHANGE
    
    // Returns a short, 1-2 sentence explanation of the algorithm's logic
    std::string getDescription() const override { return "A brief explanation of how this algorithm sorts data."; } // <-- CHANGE
    
    // Returns the index of the bar to be highlighted in Cyan
    size_t getCurrentIndex() const override { return currentIndex; }
    
    // Returns the pseudo-code array
    const std::vector<std::string>& getCode() const override { return pseudoCode; }
    
    // Returns the line of code currently being executed (0-indexed)
    int getCurrentCodeLine() const override { return currentCodeLine; }
    
    // Tells the audio engine if we are currently modifying the array (for the special beep)
    bool isModifying() const override { return is_modifying; }
    
    // (Optional) Override this if your algorithm eliminates bars
    // bool isEliminated(size_t index) const override { return false; }
};