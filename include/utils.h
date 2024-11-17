#ifndef UTILS_H
#define UTILS_H

#include <stddef.h>

// Génère un temps aléatoire pour un tour
int generate_lap_time(int min_time_ms, int max_time_ms);

// Formate un temps en millisecondes
void format_time(int time_ms, char *formatted_time, size_t size);

#endif
