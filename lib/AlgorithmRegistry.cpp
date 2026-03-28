#include "AlgorithmRegistry.hpp"

// =========================================================================
// ALGOTRACE CONTRIBUTOR REGISTRY
// -------------------------------------------------------------------------
// INSTRUCTIONS:
// 1. Include your algorithm's header file below.
// 2. Add your algorithm to the registry vector inside the function.
// =========================================================================

// [STEP 1]: Include your headers here
#include "BubbleSort.hpp"
#include "HeapSort.hpp"

std::vector<AlgoRegistryEntry> getAvailableAlgorithms() {
    std::vector<AlgoRegistryEntry> registry;

    // [STEP 2]: Register your algorithm here
    registry.push_back({"Bubble Sort", [](std::vector<int>& d) { return std::make_unique<BubbleSort>(d); }});
    registry.push_back({"Heap Sort",   [](std::vector<int>& d) { return std::make_unique<HeapSort>(d); }});

    return registry;
}