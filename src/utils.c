#include "utils.h"

int clamp(int v, int a, int b) {
    return v < a ? a : (v > b ? b : v);
}