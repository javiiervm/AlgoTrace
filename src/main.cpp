#include "Visualizer.hpp"

int main() {
    // Vertical layout (Mobile app / Widget style)
    const int windowWidth = 450;
    const int windowHeight = 750;

    Visualizer app(windowWidth, windowHeight, "AlgoTrace");
    app.run();
    
    return 0;
}