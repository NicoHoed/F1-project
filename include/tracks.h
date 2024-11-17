#ifndef TRACKS_H
#define TRACKS_H

typedef struct {
    char name[50];
    int min_time_ms;
    int max_time_ms;
} Circuit;

// Charge les circuits depuis un fichier
int load_circuits(const char *filename, Circuit circuits[], int max_circuits);

#endif
