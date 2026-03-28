#include "TemplateSort.hpp"

// =========================================================================
// ALGOTRACE CONTRIBUTOR TEMPLATE (IMPLEMENTATION)
// -------------------------------------------------------------------------
// Welcome! To make the algorithm visually pleasing, we don't sort the array
// in a single `while` loop. Instead, we use a "State Machine" in the step()
// function. Every time step() is called, we execute ONE line of our pseudo-code,
// update the visual variables, and exit.
// =========================================================================

TemplateSort::TemplateSort(std::vector<int>& d) : Algorithm(d) {
    // [STEP 1]: Define your algorithm's pseudo-code here.
    // Try to keep it simple, python-like, and under 10 lines if possible.
    pseudoCode = {
        "def template_sort(arr):",              // Line 0
        "    for i in range(len(arr)):",        // Line 1
        "        # Do some comparison",         // Line 2
        "        if arr[i] > arr[i+1]:",        // Line 3
        "            swap(arr[i], arr[i+1])",   // Line 4
        "    return arr"                        // Line 5
    };
    
    // Set the starting line of code to highlight
    currentCodeLine = 1; 
    
    // [STEP 2]: Initialize any of your custom variables here
    // i = 0;
    // j = 0;
}

void TemplateSort::step() {
    // Reset the audio modification flag at the beginning of every step
    is_modifying = false; 

    // Safety check: If already sorted, do nothing.
    if (completed) return;

    // [STEP 3]: Define the exit condition. 
    // How do we know the array is fully sorted?
    /*
    if (my_exit_condition_met) {
        completed = true;
        currentCodeLine = 5; // Highlight the 'return' line
        currentIndex = -1;   // Remove highlights from bars
        return;
    }
    */

    // [STEP 4]: The State Machine.
    // Map each state to a line of your pseudoCode.
    
    if (stepState == 0) {
        // Example: Entering a loop
        currentCodeLine = 1; // Highlight "for i in range..."
        
        // Move to the next logical step
        stepState = 1; 
    } 
    else if (stepState == 1) {
        // Example: Making a comparison
        currentCodeLine = 3; // Highlight "if arr[i] > arr[i+1]:"
        
        // Update visual pointers so the user sees what we are comparing
        currentIndex = 0; // Update this with your actual loop variable
        
        // Logic decision
        bool needsSwap = false; /* (data[currentIndex] > data[currentIndex+1]) */
        
        if (needsSwap) {
            stepState = 2; // Go to swap state
        } else {
            stepState = 0; // Skip swap, loop again
        }
    } 
    else if (stepState == 2) {
        // Example: Swapping elements
        currentCodeLine = 4; // Highlight "swap(...)"
        
        // Actually modify the 'data' array here
        // std::swap(data[currentIndex], data[currentIndex+1]);
        
        // Trigger the special audio sound for modifications!
        is_modifying = true; 
        
        // Return to the loop
        stepState = 0;
    }
}