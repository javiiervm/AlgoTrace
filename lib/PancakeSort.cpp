#include "PancakeSort.hpp"
#include <algorithm>

PancakeSort::PancakeSort(std::vector<int>& d) : Algorithm(d) {
    pseudoCode = {
        "def pancake_sort(arr):",
        "    for currSize in range(len(arr), 1, -1):",
        "        maxIndex = 0",
        "        for j in range(1, currSize):",
        "            if arr[j] > arr[maxIndex]: maxIndex = j",
        "        if maxIndex != currSize - 1:",
        "            flip(arr, maxIndex) # Move max to front",
        "            flip(arr, currSize - 1) # Move max to final pos",
        "    return arr"
    };
    
    currentCodeLine = 1; 
    n = data.size();
    currSize = n;
}

void PancakeSort::step() {
    is_modifying = false;

    if (completed) return;

    if (stepState == 0) {
        currentCodeLine = 1; // "for currSize in range..."
        if (currSize <= 1) {
            completed = true;
            currentCodeLine = 8;
            currentIndex = -1;
            return;
        }
        // Inicializar el interior del bucle
        maxIndex = 0;
        j = 1;
        stepState = 1; 
    }
    else if (stepState == 1) {
        currentCodeLine = 2; // "maxIndex = 0"
        currentIndex = maxIndex;
        stepState = 2;
    }
    else if (stepState == 2) {
        currentCodeLine = 3; // "for j in range(1, currSize):"
        if (j < currSize) {
            stepState = 3;
        } else {
            stepState = 4; // Termina de buscar el max, vamos al if
        }
    }
    else if (stepState == 3) {
        currentCodeLine = 4; // "if arr[j] > arr[maxIndex]: ..."
        currentIndex = j;
        if (data[j] > data[maxIndex]) {
            maxIndex = j;
        }
        j++;
        stepState = 2; // Volver al bucle j
    }
    else if (stepState == 4) {
        currentCodeLine = 5; // "if maxIndex != currSize - 1:"
        currentIndex = maxIndex;
        if (maxIndex != currSize - 1) {
            // Preparar el primer Flip (darle la vuelta a la espátula hasta el maximo)
            flipStart = 0;
            flipEnd = maxIndex;
            stepState = 5; 
        } else {
            // Ya está en su sitio, reducimos el tamaño y volvemos al for exterior
            currSize--;
            stepState = 0; 
        }
    }
    else if (stepState == 5) {
        currentCodeLine = 6; // "flip(arr, maxIndex)"
        
        // Simular el flip paso a paso para que se vea en la animación
        if (flipStart < flipEnd) {
            std::swap(data[flipStart], data[flipEnd]);
            is_modifying = true; // ¡Sonido!
            currentIndex = flipEnd;
            flipStart++;
            flipEnd--;
        } else {
            // Primer flip terminado, preparamos el segundo flip (hasta el final de los no ordenados)
            flipStart = 0;
            flipEnd = currSize - 1;
            stepState = 6;
        }
    }
    else if (stepState == 6) {
        currentCodeLine = 7; // "flip(arr, currSize - 1)"
        
        // Simular el segundo flip paso a paso
        if (flipStart < flipEnd) {
            std::swap(data[flipStart], data[flipEnd]);
            is_modifying = true; // ¡Sonido!
            currentIndex = flipEnd;
            flipStart++;
            flipEnd--;
        } else {
            // Segundo flip terminado, elemento en su posición final.
            currSize--;
            stepState = 0; // Volver al bucle exterior
        }
    }
}