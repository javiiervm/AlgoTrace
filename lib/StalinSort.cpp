#include "StalinSort.hpp"

// =========================================================================
// ALGOTRACE CONTRIBUTOR IMPLEMENTATION: StalinSort (SOURCE)
// =========================================================================

StalinSort::StalinSort(std::vector<int>& d) : Algorithm(d) {
    // [STEP 1]: Define pseudo-code
    pseudoCode = {
        "def stalin_sort(arr):",                // Line 0
        "    max_val = arr[0]",                 // Line 1
        "    for i in range(1, len(arr)):",     // Line 2
        "        if arr[i] >= max_val:",        // Line 3
        "            max_val = arr[i]",         // Line 4
        "        else:",                        // Line 5
        "            eliminate(arr[i])",        // Line 6
        "    return arr"                        // Line 7
    };
    
    currentCodeLine = 1; 
    
    // [STEP 2]: Initialize custom variables
    eliminated.resize(data.size(), false); // Everyone survives initially
    
    if (!data.empty()) {
        currentMax = data[0]; // The first element is always in order
    } else {
        completed = true;
    }
}

void StalinSort::step() {
    // Reset the audio modification flag at the beginning of every step
    is_modifying = false; 

    if (completed) return;

    // [STEP 3 & 4]: The State Machine
    if (stepState == 0) {
        // Entering the loop
        currentCodeLine = 2; // "for i in range..."
        
        // Exit condition
        if (currentIndex >= data.size()) {
            completed = true;
            currentCodeLine = 7; // Highlight the 'return' line
            currentIndex = -1;   // Remove highlights from bars
            return;
        }
        
        stepState = 1; 
    } 
    else if (stepState == 1) {
        // Making the comparison
        currentCodeLine = 3; // "if arr[i] >= max_val:"
        
        if (data[currentIndex] >= currentMax) {
            stepState = 2; // Element is in order (survives)
        } else {
            stepState = 3; // Element is out of order (purged)
        }
    } 
    else if (stepState == 2) {
        // Element Survives: Update the max value
        currentCodeLine = 4; // "max_val = arr[i]"
        
        currentMax = data[currentIndex];
        
        currentIndex++;
        stepState = 0; // Return to loop
    }
    else if (stepState == 3) {
        // Element Eliminated: Purge it
        currentCodeLine = 6; // "eliminate(arr[i])"
        
        eliminated[currentIndex] = true;
        
        // Trigger the special audio sound for modifications/eliminations!
        is_modifying = true; 
        
        currentIndex++;
        stepState = 0; // Return to loop
    }
}