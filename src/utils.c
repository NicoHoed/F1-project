#include "utils.h"
#include <stdlib.h>
#include <stdio.h>

int generate_lap_time(int min_time_ms, int max_time_ms) {
    return rand() % (max_time_ms - min_time_ms + 1) + min_time_ms;
}

void format_time(int time_ms, char *formatted_time, size_t size) {
    int minutes = time_ms / 60000;
    int seconds = (time_ms % 60000) / 1000;
    int milliseconds = time_ms % 1000;

    snprintf(formatted_time, size, "%d:%02d.%03d", minutes, seconds, milliseconds);
}
