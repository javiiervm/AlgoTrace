#include "BubbleSort.hpp"
#include <algorithm> // For std::swap

// =========================================================================
// ALGOTRACE CONTRIBUTOR IMPLEMENTATION (IMPLEMENTATION): BubbleSort
// =========================================================================

BubbleSort::BubbleSort(std::vector<int>& d) : Algorithm(d) {
    // Defines standard Bubble Sort pseudo-code for visualization
    pseudoCode = {
        "def bubble_sort(arr):",              // Line 0
        "    n = len(arr)",                  // Line 1
        "    for i in range(n):",            // Line 2 (Outer Loop)
        "        for j in range(0, n-i-1):", // Line 3 (Inner Loop)
        "            if arr[j] > arr[j+1]:", // Line 4 (Comparison)
        "                swap(arr[j], arr[j+1])",// Line 5 (Swap)
        "    return arr"                      // Line 6
    };
    
    // Set starting highlight to initializing array length
    currentCodeLine = 1; 
    arr_size = data.size();
}

void BubbleSort::step() {
    // Reset the audio modification flag at the beginning of every step
    is_modifying = false;

    // Do nothing if already sorted
    if (completed) return;

    // --- STATE MACHINE ---
    // Maps stepState to logical pseudo-code lines for visualization synchronization.
    
    if (stepState == 0) {
        // State 0: Setup outer loop condition.
        currentCodeLine = 2; // Line: "for i in range(n):"
        
        // Final Exit condition: If fully passed through
        if (arr_size < 2 || i >= arr_size) {
            completed = true;
            currentCodeLine = 6; // Highlight final return
            return;
        }
        
        // Entering outer loop: Reset inner counter and optimization flag
        j = 0;
        swapped_on_pass = false;
        
        stepState = 1; // Move to inner loop condition
    } 
    else if (stepState == 1) {
        // State 1: Inner loop condition checking.
        currentCodeLine = 3; // Line: "for j in range(0, n-i-1):"
        
        // Inner Exit condition: If current inner pass is complete
        if (j >= arr_size - i - 1) {
            // Check for optimization (if no swaps occurred, already sorted)
            if (!swapped_on_pass && i > 0) {
                completed = true;
                currentCodeLine = 6;
                return;
            }
            // Normal increment outer loop and loop back
            i++;
            stepState = 0;
            return; 
        }
        
        // Update pointers visually so user sees we are entering comparison
        primaryIndex = j;
        secondaryIndex = j + 1; // Used logically, Visualizer only highlights primaryIndex typically
        
        stepState = 2; // Move to Comparison state
    } 
    else if (stepState == 2) {
        // State 2: Comparison (If condition).
        currentCodeLine = 4; // Line: "if arr[j] > arr[j+1]:"
        
        // Logical decision based on pointers
        if (data[j] > data[j+1]) {
            stepState = 3; // Go to swap state
        } else {
            // Condition false, increment counter and loop inner again
            j++;
            stepState = 1; 
        }
    } 
    else if (stepState == 3) {
        // State 3: Swap execution.
        currentCodeLine = 5; // Line: "swap(arr[j], arr[j+1])"
        
        // Modify actual array
        std::swap(data[j], data[j+1]);
        swapped_on_pass = true; // Mark that pass modified data
        
        // Trigger the special audio sound for modifications!
        is_modifying = true;
        
        // Post-swap increment counter and loop inner again
        j++;
        stepState = 1;
    }
}