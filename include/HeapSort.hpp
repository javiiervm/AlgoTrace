#pragma once
#include "Algorithm.hpp"
#include <vector>
#include <string>

// =========================================================================
// ALGOTRACE CONTRIBUTOR IMPLEMENTATION (HEADER): HeapSort
// -------------------------------------------------------------------------
// CONTRIBUTOR NOTES:
// Heap Sort works by maintaining parts of the list in a binary heap structure.
// Phase 1: Build a Max Heap (rearranges array into binary heap).
// Phase 2: Repeatedly extract the max (root) by swapping with last, reduce
// heap size, and "heapify" the new root.
// This implementation manages the atomic "heapify" sifting steps inside step().
// =========================================================================

class HeapSort : public Algorithm {
private:
    // --- 1. VISUAL STATE VARIABLES ---
    size_t currentIndex = 0;       // Highlighted primary node (Cyan)
    
    // --- 2. CODE ANIMATION & AUDIO VARIABLES ---
    std::vector<std::string> pseudoCode; 
    int currentCodeLine = 0;             
    int stepState = 0;         // State machine tracker to pause execution line-by-line
    bool is_modifying = false; // Flag to trigger the special "swap" sound

    // --- 3. ALGORITHM SPECIFIC VARIABLES ---
    size_t n = 0;               // Total array size
    size_t buildPassIdx = 0;    // Pointer for Phase 1 loop
    size_t extractPassIdx = 0;  // Pointer for Phase 2 loop
    bool isBuildingHeap = true; // Phase flag

    // Atomic Sift Down state trackers (used instead of recursion)
    size_t sift_node_idx = 0;
    size_t heap_size_active = 0;

public:
    // Constructor
    HeapSort(std::vector<int>& d);
    
    // --- CORE LOGIC ---
    // Executed once every visual tick. Atomic logic for phases and sift steps.
    void step() override;
    
    // --- GETTERS ---
    std::string getName() const override { return "Heap Sort"; }
    std::string getDescription() const override { 
        return "Converts the array into a binary max-heap, then repeatedly extracts the largest element to build the sorted list."; 
    }
    
    // Base interface only exposes currentIndex
    size_t getCurrentIndex() const override { return currentIndex; }
    
    const std::vector<std::string>& getCode() const override { return pseudoCode; }
    int getCurrentCodeLine() const override { return currentCodeLine; }
    
    // Tells the audio engine if we are currently modifying the array
    bool isModifying() const override { return is_modifying; }
};