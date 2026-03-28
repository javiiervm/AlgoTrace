#pragma once
#include <vector>
#include <string>
#include <memory>
#include <functional>
#include "Algorithm.hpp"

// =========================================================================
// ALGORITHM REGISTRY STRUCTURE
// =========================================================================

// Moved here from Visualizer to decouple UI from Algorithm management
struct AlgoRegistryEntry {
    std::string name;
    std::function<std::unique_ptr<Algorithm>(std::vector<int>&)> createFunc;
};

// Function declaration. The Visualizer will call this to get the algorithms.
std::vector<AlgoRegistryEntry> getAvailableAlgorithms();