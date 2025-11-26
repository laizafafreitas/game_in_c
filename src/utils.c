#include "utils.h"
#include "lib/screen.h"
#include "string.h"

int clamp(int v, int a, int b) {
    if (v < a) return a;
    if (v > b) return b;
    return v;
}

void drawCentered(int y, const char *text)
{
    int centerX = (SCRSTARTX + SCRENDX) / 2;
    int len = (int)strlen(text);
    int x = centerX - len / 2;
    screenGotoxy(x, y);
    printf("%s", text);
}
