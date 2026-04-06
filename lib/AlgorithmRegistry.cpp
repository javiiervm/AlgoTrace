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
#include "StalinSort.hpp"
#include "GnomeSort.hpp"
#include "PancakeSort.hpp"
#include "QuickSort.hpp"

std::vector<AlgoRegistryEntry> getAvailableAlgorithms() {
    std::vector<AlgoRegistryEntry> registry;

    // [STEP 2]: Register your algorithm here
    registry.push_back({"Bubble Sort", [](std::vector<int>& d) { return std::make_unique<BubbleSort>(d); }});
    registry.push_back({"Heap Sort",   [](std::vector<int>& d) { return std::make_unique<HeapSort>(d); }});
    registry.push_back({"Stalin Sort",   [](std::vector<int>& d) { return std::make_unique<StalinSort>(d); }});
    registry.push_back({"Gnome Sort", [](std::vector<int>& d) { return std::make_unique<GnomeSort>(d); }});
    registry.push_back({"Pancake Sort", [](std::vector<int>& d) { return std::make_unique<PancakeSort>(d); }});
    registry.push_back({"Quick Sort",   [](std::vector<int>& d) { return std::make_unique<QuickSort>(d); }});

    return registry;
}