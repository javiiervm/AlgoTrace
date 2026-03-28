#include "GnomeSort.hpp"
#include <algorithm> 

GnomeSort::GnomeSort(std::vector<int>& d) : Algorithm(d) {
    // Adaptación del código de la imagen a pseudocódigo legible
    pseudoCode = {
        "def gnome_sort(arr):",
        "    pos = 0",
        "    while pos < len(arr):",
        "        if pos == 0 or arr[pos] >= arr[pos - 1]:",
        "            pos += 1",
        "        else:",
        "            swap(arr[pos], arr[pos - 1])",
        "            pos -= 1",
        "    return arr"
    };
    
    currentCodeLine = 1; 
    n = data.size();
    pos = 0;
}

void GnomeSort::step() {
    is_modifying = false; 

    if (completed) return;

    if (stepState == 0) {
        currentCodeLine = 2; // "while pos < len(arr):"
        if (pos >= n) {
            completed = true;
            currentCodeLine = 8; // Highlight return
            currentIndex = -1;
            return;
        }
        stepState = 1; 
    } 
    else if (stepState == 1) {
        currentCodeLine = 3; // "if pos == 0 or arr[pos] >= arr[pos - 1]:"
        currentIndex = pos;
        
        if (pos == 0 || data[pos] >= data[pos - 1]) {
            stepState = 2; // Avanzar
        } else {
            stepState = 3; // Retroceder y cambiar
        }
    } 
    else if (stepState == 2) {
        currentCodeLine = 4; // "pos += 1"
        pos++;
        stepState = 0; // Volver al inicio del bucle
    }
    else if (stepState == 3) {
        currentCodeLine = 6; // "swap(arr[pos], arr[pos - 1])"
        std::swap(data[pos], data[pos - 1]);
        is_modifying = true; // ¡Sonido de cambio!
        currentIndex = pos - 1; 
        stepState = 4;
    }
    else if (stepState == 4) {
        currentCodeLine = 7; // "pos -= 1"
        pos--;
        stepState = 0; // Volver al inicio del bucle
    }
}