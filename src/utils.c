#include "utils.h"

int clamp(int v, int a, int b) {
    if (v < a) return a;
    if (v > b) return b;
    return v;
}