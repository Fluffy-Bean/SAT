#ifndef MAIN_H
#define MAIN_H

#define WINDOW_WIDTH 1400
#define WINDOW_HEIGHT 900
#define WINDOW_WIDTH_HALF (WINDOW_WIDTH / 2)
#define WINDOW_HEIGHT_HALF (WINDOW_HEIGHT / 2)
#define WINDOW_TITLE "SAT Demo"

// Finds length of array, but only works with arrays on the Stack, not the Heap, Pointers or Params/Args
#define lenof(n) (sizeof(n) / sizeof(n[0]))

#endif // MAIN_H
