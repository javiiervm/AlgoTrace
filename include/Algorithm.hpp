#pragma once
#include <vector>
#include <string>

// =========================================================================
// CONTRIBUTOR TEMPLATE:
// To add a new algorithm, inherit from this class and implement all methods.
// =========================================================================
class Algorithm {
protected:
    std::vector<int>& data; 
    bool completed = false; 

public:
    Algorithm(std::vector<int>& d) : data(d) {}
    virtual ~Algorithm() = default;

    // 1. Core Logic: Executed once per timer tick
    virtual void step() = 0;                 
    
    // 2. Info: Display name in the menu and visualizer
    virtual std::string getName() const = 0; 
    
    // 3. Graph Data: Tell the UI how to color the bars
    virtual size_t getCurrentIndex() const = 0;
    virtual bool isModifying() const { return false; }  // Returns true if the current step is modifying the array (for sound purposes)
    virtual bool isEliminated(size_t index) const { return false; }
    
    // 4. Code Visualization: Provide the pseudo-code and active line
    virtual const std::vector<std::string>& getCode() const = 0; 
    virtual int getCurrentCodeLine() const = 0;                  

    bool isCompleted() const { return completed; }
};