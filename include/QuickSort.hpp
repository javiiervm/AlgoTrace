#pragma once
#include "Algorithm.hpp"
#include <vector>
#include <string>
#include <stack>

// =========================================================================
// ALGOTRACE CONTRIBUTOR IMPLEMENTATION (HEADER): QuickSort
// -------------------------------------------------------------------------
// CONTRIBUTOR NOTES:
// Quick Sort works by selecting a pivot (last element) and partitioning
// the array so elements smaller than the pivot come before it.
// An explicit stack replaces recursion to stay compatible with the
// frame-by-frame state machine architecture.
// =========================================================================

class QuickSort : public Algorithm {
private:
    // --- 1. VISUAL STATE VARIABLES ---
    size_t currentIndex = 0;       // The primary bar currently being checked (Cyan)

    // --- 2. CODE ANIMATION & AUDIO VARIABLES ---
    std::vector<std::string> pseudoCode;
    int currentCodeLine = 0;
    int stepState = 0;
    bool is_modifying = false;

    // --- 3. ALGORITHM SPECIFIC VARIABLES ---
    std::stack<std::pair<size_t, size_t>> rangeStack; // Pending subarrays to sort
    size_t low = 0;        // Current subarray start
    size_t high = 0;       // Current subarray end (pivot position)
    size_t pivot = 0;      // Pivot value
    size_t i = 0;          // Smaller-element boundary
    size_t j = 0;          // Current element being compared
    bool i_valid = false;  // Replaces i = low-1 (avoids unsigned underflow)

public:
    QuickSort(std::vector<int>& d);

    void step() override;

    std::string getName() const override { return "Quick Sort"; }
    std::string getDescription() const override {
        return "Picks a pivot element and partitions the array around it, then sorts each side independently.";
    }

    size_t getCurrentIndex() const override { return currentIndex; }
    const std::vector<std::string>& getCode() const override { return pseudoCode; }
    int getCurrentCodeLine() const override { return currentCodeLine; }
    bool isModifying() const override { return is_modifying; }
};