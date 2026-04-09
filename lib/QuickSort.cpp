#include "QuickSort.hpp"
#include <algorithm>

// =========================================================================
// ALGOTRACE CONTRIBUTOR IMPLEMENTATION (IMPLEMENTATION): QuickSort
// =========================================================================

QuickSort::QuickSort(std::vector<int>& d) : Algorithm(d) {
    pseudoCode = {
        "def quick_sort(arr, low, high):", // Line 0
        "    pivot = arr[high]",            // Line 1
        "    i = low - 1",                 // Line 2
        "    for j in range(low, high):",  // Line 3
        "        if arr[j] <= pivot:",     // Line 4
        "            i += 1",             // Line 5
        "            swap(arr[i], arr[j])",// Line 6
        "    swap(arr[i+1], arr[high])",   // Line 7
        "    return i + 1"                 // Line 8
    };

    currentCodeLine = 1;

    if (data.size() > 1) {
        rangeStack.push({0, data.size() - 1});
    } else {
        completed = true;
    }
}

void QuickSort::step() {
    is_modifying = false;

    if (completed) return;

    if (stepState == 0) {
        // Pop next subarray — skip trivial ranges one per frame
        currentCodeLine = 0;
        if (rangeStack.empty()) {
            completed = true;
            currentCodeLine = 8;
            currentIndex = 0;
            return;
        }
        auto [l, h] = rangeStack.top();
        rangeStack.pop();
        if (l >= h) return; // Trivial subarray, skip this frame
        low = l;
        high = h;
        i_valid = false;
        i = low;
        j = low;
        currentIndex = low;
        stepState = 1;
    }
    else if (stepState == 1) {
        // Set pivot to last element of subarray
        currentCodeLine = 1;
        pivot = data[high];
        currentIndex = high;
        stepState = 2;
    }
    else if (stepState == 2) {
        // Initialise i and j
        currentCodeLine = 2;
        i_valid = false;
        i = low;
        j = low;
        currentIndex = low;
        stepState = 3;
    }
    else if (stepState == 3) {
        // Inner loop condition
        currentCodeLine = 3;
        if (j >= high) {
            stepState = 6; // Loop done, do final swap
        } else {
            currentIndex = j;
            stepState = 4;
        }
    }
    else if (stepState == 4) {
        // Comparison: arr[j] <= pivot?
        currentCodeLine = 4;
        currentIndex = j;
        if ((size_t)data[j] <= pivot) {
            stepState = 5;
        } else {
            j++;
            stepState = 3;
        }
    }
    else if (stepState == 5) {
        // i += 1
        currentCodeLine = 5;
        if (!i_valid) {
            i = low; // First increment: logical low-1 becomes low
            i_valid = true;
        } else {
            i++;
        }
        currentIndex = i;
        stepState = 55;
    }
    else if (stepState == 55) {
        // swap(arr[i], arr[j])
        currentCodeLine = 6;
        std::swap(data[i], data[j]);
        is_modifying = true;
        j++;
        stepState = 3;
    }
    else if (stepState == 6) {
        // Final swap: place pivot in correct position
        currentCodeLine = 7;
        size_t pivotPos = i_valid ? i + 1 : low;
        std::swap(data[pivotPos], data[high]);
        is_modifying = true;
        currentIndex = pivotPos;
        // Push left and right subarrays
        if (pivotPos > 0 && low < pivotPos - 1) rangeStack.push({low, pivotPos - 1});
        if (pivotPos + 1 < high)               rangeStack.push({pivotPos + 1, high});
        stepState = 0;
    }
}