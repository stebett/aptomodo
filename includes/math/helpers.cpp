//
// Created by ginko on 12/7/24.
//

#include "helpers.h"

int roundUpToNextMultiple(float value, int n) {
    if (n == 0) return value; // Avoid division by zero
    return round(value / n) * n;
}
