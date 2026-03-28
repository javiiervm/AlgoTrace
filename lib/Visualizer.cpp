#include "Visualizer.hpp"
#include <random>
#include <algorithm> // Required for std::max to handle edge cases in resizing
#include <sstream> // Required for word-wrap

// --- THEME & COLOR PALETTE ---
// Defined globally to ensure UI consistency and make future theming easier.
const raylib::Color BG_COLOR(20, 20, 30, 255);
const raylib::Color PANEL_COLOR(35, 35, 50, 255);
const raylib::Color CODE_HL_COLOR(60, 80, 110, 255);
const raylib::Color COLOR_SURVIVOR(46, 204, 113, 255);
const raylib::Color COLOR_ELIMINATED(231, 76, 60, 255);
const raylib::Color COLOR_CHECKING(82, 173, 226, 255);

Visualizer::Visualizer(int w, int h, std::string title) : window(w, h, title) {
    // Optional: Uncomment the following line to allow the user to drag and resize 
    // the window in real-time. The UI math handles responsiveness automatically.
    // window.SetState(FLAG_WINDOW_RESIZABLE); 

    // Load custom TTF font for crisp, anti-aliased text scaling.
    customFont = raylib::Font("assets/font.ttf", 64);

    // Load custom sounds
    beepSound = raylib::Sound("assets/beep.wav");
    swapSound = raylib::Sound("assets/swap.wav");

    // Set the initial master volume for the entire application
    SetMasterVolume(currentVolume);
    
    // Populate the algorithm factory on startup
    registerAlgorithms();
}

void Visualizer::registerAlgorithms() {
    // It simply requests the list from the decoupled Registry.
    algorithmRegistry = getAvailableAlgorithms();
}

void Visualizer::resetData() {
    data.clear();
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(10, 100);
    
    // Generate an array of 15 random integers to be sorted
    for(int i = 0; i < 15; ++i) { 
        data.push_back(dis(gen));
    }
}

void Visualizer::run() {
    SetTargetFPS(60); // Lock framerate to ensure smooth UI interaction
    
    while (!window.ShouldClose()) {
        window.BeginDrawing();
        window.ClearBackground(BG_COLOR);
        
        // State Machine Router: Directs the render loop to the active screen
        if (currentState == AppState::MENU) {
            drawMenu();
        } else if (currentState == AppState::SORTING) {
            drawVisualizer();
        }

        window.EndDrawing();
    }
}

// --- MENU RENDERER (RESPONSIVE) ---
void Visualizer::drawMenu() {
    float w = window.GetWidth();
    float h = window.GetHeight();

    // 1. Dynamic Centering: Measure text width first to perfectly center it
    std::string title = "AlgoTrace";
    float titleWidth = customFont.MeasureText(title, 40, 2).x;
    customFont.DrawText(title, raylib::Vector2{(w - titleWidth) / 2.0f, h * 0.1f}, 40, 2, raylib::Color::Orange());
    
    std::string subTitle = "Select an Algorithm";
    float subTitleWidth = customFont.MeasureText(subTitle, 20, 1).x;
    customFont.DrawText(subTitle, raylib::Vector2{(w - subTitleWidth) / 2.0f, h * 0.15f + 30}, 20, 1, raylib::Color::LightGray());

    // 2. Responsive Geometry: Buttons shrink dynamically if the window becomes too narrow
    int btnWidth = (w > 340) ? 300 : w - 40; 
    int btnHeight = 60;
    int spacing = 20;
    int btnX = (w - btnWidth) / 2;
    int startY = h * 0.35f; // Start list at 35% down the screen height
    
    float totalListHeight = algorithmRegistry.size() * (btnHeight + spacing);

    // 3. Scroll Logic: Smoothly track mouse wheel movement
    menuScrollOffset += GetMouseWheelMove() * 30.0f; 
    
    // Calculate boundaries to prevent scrolling into empty space
    float maxScroll = 0.0f; 
    float minScroll = h - startY - totalListHeight - 50; 
    if (minScroll > 0) minScroll = 0; // Disable scroll if everything fits on screen
    
    // Clamp the scroll offset to the calculated limits
    if (menuScrollOffset > maxScroll) menuScrollOffset = maxScroll;
    if (menuScrollOffset < minScroll) menuScrollOffset = minScroll;

    // 4. Scissor Mode: Creates a masking rectangle. Anything drawn outside these 
    // bounds will be clipped, preventing buttons from overlapping the title.
    BeginScissorMode(0, startY - 10, w, h - startY + 10);

    for (size_t i = 0; i < algorithmRegistry.size(); ++i) {
        int btnY = startY + (i * (btnHeight + spacing)) + menuScrollOffset;
        raylib::Rectangle btnRect(btnX, btnY, btnWidth, btnHeight);
        
        // Prevent "ghost clicks" by checking hover only within the unmasked area
        bool isHovered = btnRect.CheckCollision(GetMousePosition()) && GetMouseY() > (startY - 10);
        raylib::Color btnColor = isHovered ? raylib::Color(60, 60, 80, 255) : PANEL_COLOR;
        
        btnRect.DrawRounded(0.2f, 8, btnColor);
        
        float textWidth = customFont.MeasureText(algorithmRegistry[i].name, 24, 1).x;
        customFont.DrawText(algorithmRegistry[i].name, 
                            raylib::Vector2{btnX + (btnWidth - textWidth)/2.0f, btnY + 18.0f}, 
                            24, 1, raylib::Color::White());

        // Action: Load the algorithm and switch the application state
        if (isHovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            resetData();
            currentAlgo = algorithmRegistry[i].createFunc(data);
            paused = false;
            currentState = AppState::SORTING;
        }
    }
    EndScissorMode(); 
}

// =========================================================================
// VISUALIZER RENDERER IMPLEMENTATION
// =========================================================================

// --- MAIN VISUALIZER LOOP ---
void Visualizer::drawVisualizer() {
    float w = window.GetWidth();
    float h = window.GetHeight();

    // 1. Input Handling: Core playback and navigation controls
    if (IsKeyPressed(KEY_SPACE)) paused = !paused;
    if (IsKeyPressed(KEY_R)) { 
        resetData(); 
        for(const auto& entry : algorithmRegistry) {
            if(entry.name == currentAlgo->getName()) {
                currentAlgo = entry.createFunc(data);
                break;
            }
        }
        paused = false;
    }
    if (IsKeyPressed(KEY_BACKSPACE)) {
        currentState = AppState::MENU;
        currentAlgo.reset(); 
        return; 
    }
    
    // --- MASTER VOLUME CONTROL ---
    if (IsKeyPressed(KEY_UP)) {
        currentVolume += 0.1f; 
        if (currentVolume > 1.0f) currentVolume = 1.0f; 
        SetMasterVolume(currentVolume);
    }
    if (IsKeyPressed(KEY_DOWN)) {
        currentVolume -= 0.1f; 
        if (currentVolume < 0.0f) currentVolume = 0.0f; 
        SetMasterVolume(currentVolume);
    }

    // 2. Execution Timer & Audio Engine
    if (!paused && !currentAlgo->isCompleted()) {
        stepTimer += GetFrameTime();
        if (stepTimer >= stepDelay) {
            currentAlgo->step(); 
            stepTimer = 0.0f;    

            int currentIndex = currentAlgo->getCurrentIndex();
            
            if (currentIndex >= 0 && currentIndex < data.size()) {
                float value = data[currentIndex];
                float pitch = 0.5f + (value / 100.0f) * 2.0f; 

                if (currentAlgo->isModifying()) {
                    swapSound.SetPitch(pitch);
                    swapSound.Play();
                } else {
                    beepSound.SetPitch(pitch);
                    beepSound.Play();
                }
            }
        }
    }

    // 3. UI Header
    // A. Main Title: Added more breathing room from the top (Y=25)
    std::string algoName = currentAlgo->getName();
    float titleWidth = customFont.MeasureText(algoName, 32, 2).x;
    customFont.DrawText(algoName, raylib::Vector2{(w - titleWidth) / 2.0f, 25}, 32, 2, raylib::Color::Orange());
    
    // B. Subtitle / Description: Shifted down to Y=70 to clear the title completely
    std::string rawDesc = currentAlgo->getDescription();
    
    std::stringstream ss(rawDesc);
    std::string word;
    std::string currentLine;
    
    float fontSize = 16.0f;
    float currentDescY = 70.0f; // Increased margin
    int descSidePadding = 40;   
    float allowedDescWidth = w - (descSidePadding * 2);

    bool firstWord = true;

    while (ss >> word) {
        std::string testLine = currentLine;
        if (!firstWord) testLine += " ";
        testLine += word;

        float testWidth = customFont.MeasureText(testLine, fontSize, 1).x;

        if (testWidth <= allowedDescWidth) {
            currentLine = testLine;
            firstWord = false;
        } else {
            float lineWidth = customFont.MeasureText(currentLine, fontSize, 1).x;
            customFont.DrawText(currentLine, raylib::Vector2{(w - lineWidth) / 2.0f, currentDescY}, fontSize, 1, raylib::Color::LightGray());
            
            currentLine = word;
            currentDescY += 22.0f; 
        }
    }
    float lineWidth = customFont.MeasureText(currentLine, fontSize, 1).x;
    customFont.DrawText(currentLine, raylib::Vector2{(w - lineWidth) / 2.0f, currentDescY}, fontSize, 1, raylib::Color::LightGray());
    
    // Interactive BACK button
    raylib::Rectangle backBtn(20, 25, 80, 30);
    bool backHover = backBtn.CheckCollision(GetMousePosition());
    customFont.DrawText("< BACK", raylib::Vector2{20, 30}, 18, 1, backHover ? raylib::Color::White() : raylib::Color::Gray());
    if (backHover && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        currentState = AppState::MENU;
        currentAlgo.reset();
        return;
    }

    // 4. Render main components
    drawGraphArea();
    drawCodeArea();
    
    // 5. UI Footer (Now Responsive)
    std::string footer1 = "SPACE: Pause  |  R: Reset";
    std::string footer2 = "BACKSPACE: Menu  |  UP/DOWN: Volume (" + std::to_string((int)(currentVolume * 100)) + "%)";
    std::string fullFooter = footer1 + "  |  " + footer2;
    
    float fullWidth = customFont.MeasureText(fullFooter, 16, 1).x;
    
    // If the window is too narrow, split the footer into two rows
    if (fullWidth < w - 20) {
        customFont.DrawText(fullFooter, raylib::Vector2{(w - fullWidth) / 2.0f, h - 30}, 16, 1, raylib::Color::Gray());
    } else {
        float w1 = customFont.MeasureText(footer1, 16, 1).x;
        float w2 = customFont.MeasureText(footer2, 16, 1).x;
        customFont.DrawText(footer1, raylib::Vector2{(w - w1) / 2.0f, h - 45}, 16, 1, raylib::Color::Gray());
        customFont.DrawText(footer2, raylib::Vector2{(w - w2) / 2.0f, h - 25}, 16, 1, raylib::Color::Gray());
    }
}

// --- GRAPHICS PANEL RENDERER ---
void Visualizer::drawGraphArea() {
    float w = window.GetWidth();
    float h = window.GetHeight();

    int pX = 20;
    int pW = w - 40;
    
    // Shifted down to Y=140 to guarantee space for Title + description
    int pY = 140; 
    
    // Account for 140px header + 90px footer space (230px total) to avoid overlapping UI
    int availableHeight = h - 230; 
    if (availableHeight < 100) availableHeight = 100; // Safety clamp for extreme resizing
    int pH = availableHeight / 2;  
    
    raylib::Rectangle(pX, pY, pW, pH).DrawRounded(0.05f, 10, PANEL_COLOR);
    
    float barWidth = (float)(pW - 20) / data.size();
    float maxBarHeight = pH - 40;
    float baseY = pY + pH - 20; 
    
    for (size_t i = 0; i < data.size(); i++) {
        float height = (data[i] / 100.0f) * maxBarHeight;
        if (currentAlgo->isEliminated(i)) height = 8.0f; 
        
        float posX = pX + 10 + (i * barWidth);
        float posY = baseY - height; 
        
        raylib::Color barColor = raylib::Color(170, 70, 210, 255); 
        if (currentAlgo->isEliminated(i)) barColor = COLOR_ELIMINATED;
        else if (i == currentAlgo->getCurrentIndex()) barColor = COLOR_CHECKING;
        else if (i < currentAlgo->getCurrentIndex()) barColor = COLOR_SURVIVOR;
        
        raylib::Rectangle(posX, posY, std::max(1.0f, barWidth - 4), height).DrawRounded(0.5f, 8, barColor);
    }
}

// --- CODE PANEL RENDERER ---
void Visualizer::drawCodeArea() {
    float w = window.GetWidth();
    float h = window.GetHeight();

    int pX = 20;
    int pW = w - 40;
    
    // Math must match Graph Area to ensure perfect 50/50 split
    int availableHeight = h - 230; 
    if (availableHeight < 100) availableHeight = 100;
    int panelHeight = availableHeight / 2;
    
    // Start immediately below Graph panel + 15px gap
    int pY = 140 + panelHeight + 15; 
    
    raylib::Rectangle(pX, pY, pW, panelHeight).DrawRounded(0.05f, 10, PANEL_COLOR);
    
    const auto& code = currentAlgo->getCode();
    int activeLine = currentAlgo->getCurrentCodeLine();
    int textY = pY + 20;
    
    BeginScissorMode(pX, pY, pW, panelHeight);

    for (size_t i = 0; i < code.size(); i++) {
        if (i == activeLine) {
            raylib::Rectangle(pX + 5, textY - 2, pW - 10, 26).DrawRounded(0.1f, 4, CODE_HL_COLOR);
        }
        
        raylib::Color fontColor = (i == activeLine) ? raylib::Color::White() : raylib::Color::LightGray();
        customFont.DrawText(code[i], raylib::Vector2{(float)pX + 15, (float)textY}, 18, 1, fontColor);
        textY += 30; 
    }
    
    EndScissorMode();
}