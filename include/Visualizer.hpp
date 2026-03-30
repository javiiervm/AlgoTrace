#pragma once
#include <raylib-cpp.hpp>
#include <vector>
#include <memory>
#include <string>
#include <functional>
#include "Algorithm.hpp"
#include "AlgorithmRegistry.hpp"

// State Machine enum to manage the application's current view.
// This prevents drawing the menu and the visualizer at the same time.
enum class AppState {
    MENU,
    SORTING
};

class Visualizer {
private:
    raylib::Window window;
    raylib::AudioDevice audio;
    raylib::Font customFont;
    raylib::Sound beepSound;
    raylib::Sound swapSound;

    float currentVolume = 0.5f; // Volume control for sound

    std::vector<int> data;
    std::unique_ptr<Algorithm> currentAlgo;
    
    // Application state management
    AppState currentState = AppState::MENU;
    std::vector<AlgoRegistryEntry> algorithmRegistry;

    // Scroll tracking for the main menu list
    float menuScrollOffset = 0.0f;
    
    // Timer controls to decouple the algorithm's execution speed from the screen's Framerate (FPS)
    bool paused = false;    
    float stepTimer = 0.0f; 
    float stepDelay = 0.3f; 

    // Final animation variables
    int completionIndex = 0;  // Current highlighted bar
    float completionTimer = 0.0f;  // Fast timer

    float speedMultiplier = 1.0f;  // 1.0f = 100% (normal)

    // Internal utility functions
    void resetData();
    void registerAlgorithms(); // Centralized place to add new algorithms to the menu
    
    // Rendering modules separated by state to keep the main loop clean
    void drawMenu();
    void drawVisualizer();
    void drawGraphArea();
    void drawCodeArea();

public:
    Visualizer(int w, int h, std::string title);
    void run();
};