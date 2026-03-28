#include "Visualizer.hpp"
#include <random>
#include <algorithm> // Required for std::max to handle edge cases in resizing

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

// --- VISUALIZER RENDERER (RESPONSIVE) ---
void Visualizer::drawVisualizer() {
    float w = window.GetWidth();
    float h = window.GetHeight();

    // 1. Input Handling
    if (IsKeyPressed(KEY_SPACE)) paused = !paused;
    if (IsKeyPressed(KEY_R)) { 
        resetData(); 
        // Restart the current algorithm by looking it up in the factory registry
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
        currentAlgo.reset(); // Free memory when leaving the visualizer
        return; 
    }
    
    // --- MASTER VOLUME CONTROL ---
    if (IsKeyPressed(KEY_UP)) {
        currentVolume += 0.1f; // Increase by 10%
        if (currentVolume > 1.0f) currentVolume = 1.0f; // Max volume is 1.0
        SetMasterVolume(currentVolume);
    }
    if (IsKeyPressed(KEY_DOWN)) {
        currentVolume -= 0.1f; // Decrease by 10%
        if (currentVolume < 0.0f) currentVolume = 0.0f; // Min volume is 0.0 (Muted)
        SetMasterVolume(currentVolume);
    }

    // 2. Execution Timer: Triggers a logic step only when enough time has passed
    if (!paused && !currentAlgo->isCompleted()) {
        stepTimer += GetFrameTime();
        if (stepTimer >= stepDelay) {
            currentAlgo->step();
            stepTimer = 0.0f; // Reset accumulation timer

            // --- DYNAMIC AUDIO FEEDBACK ---
            int currentIndex = currentAlgo->getCurrentIndex();
            
            // Safety check: ensure the index is within bounds before reading data
            if (currentIndex >= 0 && currentIndex < data.size()) {
                float value = data[currentIndex];
                
                // Calculate pitch dynamically. 
                // Assumes data values range from roughly 10 to 100.
                // Pitch maps from 0.5f (low tone) to 2.5f (high tone).
                float pitch = 0.5f + (value / 100.0f) * 2.0f; 

                // Play the appropriate sound based on the step's semantic action
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
    std::string algoName = currentAlgo->getName();
    float titleWidth = customFont.MeasureText(algoName, 32, 2).x;
    customFont.DrawText(algoName, raylib::Vector2{(w - titleWidth) / 2.0f, 25}, 32, 2, raylib::Color::Orange());
    
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
    
    // 5. UI Footer (Now with dynamic volume display)
    std::string footerText = "SPACE: Pause  |  R: Reset  |  BACKSPACE: Menu  |  UP/DOWN: Volume (" + std::to_string((int)(currentVolume * 100)) + "%)";
    float footerWidth = customFont.MeasureText(footerText, 16, 1).x;
    customFont.DrawText(footerText, raylib::Vector2{(w - footerWidth) / 2.0f, h - 30}, 16, 1, raylib::Color::Gray());
}

void Visualizer::drawGraphArea() {
    float w = window.GetWidth();
    float h = window.GetHeight();

    // Relative Math: Divide the available screen space equally between both panels
    int pX = 20;
    int pW = w - 40;
    int pY = 80; 
    int availableHeight = h - 140; // Account for Header and Footer margins
    int pH = availableHeight / 2;  
    
    raylib::Rectangle(pX, pY, pW, pH).DrawRounded(0.05f, 10, PANEL_COLOR);
    
    float barWidth = (float)(pW - 20) / data.size();
    float maxBarHeight = pH - 40;
    
    // Strict baseline so all bars rest exactly on the bottom edge of the panel
    float baseY = pY + pH - 20; 
    
    for (size_t i = 0; i < data.size(); i++) {
        float height = (data[i] / 100.0f) * maxBarHeight;
        
        // Force a tiny height for eliminated elements to keep the rounded pill shape visible
        if (currentAlgo->isEliminated(i)) height = 8.0f; 
        
        float posX = pX + 10 + (i * barWidth);
        float posY = baseY - height; 
        
        // Determine the bar's color based on its semantic state within the algorithm
        raylib::Color barColor = raylib::Color(170, 70, 210, 255); // Default Unsorted
        if (currentAlgo->isEliminated(i)) barColor = COLOR_ELIMINATED;
        else if (i == currentAlgo->getCurrentIndex()) barColor = COLOR_CHECKING;
        else if (i < currentAlgo->getCurrentIndex()) barColor = COLOR_SURVIVOR;
        
        // Draw the pill shape. std::max prevents drawing glitches if the window is shrunk too far.
        raylib::Rectangle(posX, posY, std::max(1.0f, barWidth - 4), height).DrawRounded(0.5f, 8, barColor);
    }
}

void Visualizer::drawCodeArea() {
    float w = window.GetWidth();
    float h = window.GetHeight();

    int pX = 20;
    int pW = w - 40;
    int availableHeight = h - 140;
    int panelHeight = availableHeight / 2;
    int pY = 80 + panelHeight + 15; // Start immediately below the Graph panel
    
    raylib::Rectangle(pX, pY, pW, panelHeight).DrawRounded(0.05f, 10, PANEL_COLOR);
    
    const auto& code = currentAlgo->getCode();
    int activeLine = currentAlgo->getCurrentCodeLine();
    int textY = pY + 20;
    
    // Scissor Mode masks the text, ensuring it never overlaps the footer on tiny screens
    BeginScissorMode(pX, pY, pW, panelHeight);

    for (size_t i = 0; i < code.size(); i++) {
        // Draw the background highlight for the currently executing line
        if (i == activeLine) {
            raylib::Rectangle(pX + 5, textY - 2, pW - 10, 26).DrawRounded(0.1f, 4, CODE_HL_COLOR);
        }
        
        raylib::Color fontColor = (i == activeLine) ? raylib::Color::White() : raylib::Color::LightGray();
        customFont.DrawText(code[i], raylib::Vector2{(float)pX + 15, (float)textY}, 18, 1, fontColor);
        
        textY += 30; // Vertical spacing per line
    }
    
    EndScissorMode();
}