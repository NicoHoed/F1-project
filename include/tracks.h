#ifndef TRACKS_H
#define TRACKS_H

#define MAX_TRACKS 100  // Nombre maximal de circuits

typedef struct {
    int id;
    char name[50];
    int min_time_ms;
    int max_time_ms;
} Track;

int load_tracks(const char *filename, Track *tracks, int max_tracks);

#endif // TRACKS_H
