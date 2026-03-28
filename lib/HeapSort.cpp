#include "HeapSort.hpp"
#include <algorithm> // For std::swap

// =========================================================================
// ALGOTRACE CONTRIBUTOR IMPLEMENTATION (IMPLEMENTATION): HeapSort
// =========================================================================

HeapSort::HeapSort(std::vector<int>& d) : Algorithm(d) {
    // Standard Pythonic Heap Sort representation for visualization
    pseudoCode = {
        "def heap_sort(arr):",                     // Line 0
        "    n = len(arr)",                         // Line 1
        "    for i in range(n // 2 - 1, -1, -1):", // Line 2 (Phase 1 Loop: Build Heap)
        "        heapify(arr, n, i)",               // Line 3 (Visualizes Sifting node i)
        "    for i in range(n - 1, 0, -1):",        // Line 4 (Phase 2 Loop: Extract Max)
        "        swap(arr[0], arr[i])",             // Line 5 (Swap root with last)
        "        heapify(arr, i, 0)",               // Line 6 (Visualizes Sifting root)
        "    return arr"                            // Line 7
    };
    
    // Set starting highlight to initializing parameters
    currentCodeLine = 1; 
    n = data.size();

    // Setup initial conditions for algorithm logic
    if (n >= 2) {
        buildPassIdx = (n / 2); // Start building heap from last parent. (Note: using logic adjustment vs python pseudo-code for exact index mapping later)
    } else {
        completed = true; // Already sorted if n < 2
    }
}

void HeapSort::step() {
    // Reset the audio modification flag at the beginning of every step
    is_modifying = false;

    // Safety check: If sorted or too small, return
    if (completed || n < 2) return;

    // --- STATE MACHINE ---
    
    if (stepState == 0) {
        // State 0: Outer loop condition checking. Routing between Phases.
        if (isBuildingHeap) {
            currentCodeLine = 2; // Line: "for i in range(n // 2 - 1...)" (Build Heap loop)
            if (buildPassIdx == 0) {
                // Building complete, switch phases
                isBuildingHeap = false;
                extractPassIdx = n - 1; // Init extract counter
                stepState = 0; // Return state, loop router will handle Phase 2 next frame
                return;
            } else {
                buildPassIdx--; // Decrement index of node to heapify
                stepState = 10; // State Phase1 Heapify Node i
                return;
            }
        } else {
            currentCodeLine = 4; // Line: "for i in range(n - 1, 0, -1):" (Extract loop)
            if (extractPassIdx == 0) {
                // Sorted completely
                completed = true;
                currentCodeLine = 7;
                return;
            } else {
                // Route to Swap then Heapify states
                stepState = 20; 
                return;
            }
        }
    } 

    // =========================================================================
    // PHASE 1: BUILD MAX HEAP (Building loop logic states)
    // =========================================================================
    
    else if (stepState == 10) {
        // State 10: Build Phase -> Initial call to sift down node 'i' in heap of size 'n'
        currentCodeLine = 3; // Line: "heapify(arr, n, i)"
        
        // Setup internal sift state for build index
        sift_node_idx = buildPassIdx;
        heap_size_active = n;
        
        // Visually highlight the parent node we are sifting down
        currentIndex = sift_node_idx;
        
        stepState = 30; // Move to atomic sift loop start state
    }

    // =========================================================================
    // PHASE 2: EXTRACT MAX (Extraction loop logic states)
    // =========================================================================
    
    else if (stepState == 20) {
        // State 20: Extract Phase -> Highlight and execute swap root with last unsorted
        currentCodeLine = 5; // Line: "swap(arr[0], arr[i])"
        
        currentIndex = extractPassIdx; // Visual pointer shows where max element goes
        
        // Perform swap
        std::swap(data[0], data[extractPassIdx]);
        
        // Trigger the special audio sound for modifications!
        is_modifying = true;

        stepState = 21; // Move to prepare heapify call
    }
    else if (stepState == 21) {
        // State 21: Extract Phase -> Highlight preparation to sift down root in reduced heap size
        currentCodeLine = 6; // Line: "heapify(arr, i, 0)"
        
        // Setup internal sift state for root in reduced size heap (i)
        sift_node_idx = 0;
        heap_size_active = extractPassIdx; // Size reduces every extraction pass
        
        currentIndex = sift_node_idx; // Visually highlight root starting sift down
        
        stepState = 30; // Move to atomic sift loop start state
    }

    // =========================================================================
    // INTERNAL SIFT DOWN: Atomic iteration logic (replaces recursive heapify)
    // maps roughly to the pseudo-code logic of Lines 3/6 during sift steps.
    // =========================================================================

    else if (stepState == 30) {
        // State 30: Internal Sift Down -> Pre-Comparison Step
        currentCodeLine = isBuildingHeap ? 3 : 6; 
        
        size_t parent = sift_node_idx;
        size_t largest = parent;
        size_t left = 2 * parent + 1;
        size_t right = 2 * parent + 2;

        // Perform boundary checks and max comparison logic
        if (left < heap_size_active && data[left] > data[largest]) {
            largest = left;
        }
        if (right < heap_size_active && data[right] > data[largest]) {
            largest = right;
        }

        // Decision point of the sift
        if (largest != parent) {
            // Need a swap and continue sifting down new node
            sift_node_idx = largest; // Track the new 'node_idx' sifting continues with
            
            // visually highlight sifting path (parent swapping with child)
            currentIndex = sift_node_idx; 

            stepState = 31; // Proceed to sift swap execution state next frame
        } else {
            // Sift Down complete for this iteration pass
            if (isBuildingHeap) {
                stepState = 0; // Return to Build outer loop check
            } else {
                extractPassIdx--; // Decrement extract outer loop counter
                stepState = 0; // Return to Extract outer loop check
            }
        }
    }
    else if (stepState == 31) {
        // State 31: Internal Sift Down -> execute data modification (swap parent/child)
        currentCodeLine = isBuildingHeap ? 3 : 6; // Keep heapify highlighted visually
        
        // Recalculate original parent visually
        size_t parent_original = (sift_node_idx - 1) / 2;
        if (sift_node_idx == 0) parent_original = 0; 

        // Modify array data
        std::swap(data[parent_original], data[sift_node_idx]);
        
        // Trigger the special audio sound for modifications!
        is_modifying = true;

        // Continue sifting loop iteratively
        stepState = 30; 
    }
}