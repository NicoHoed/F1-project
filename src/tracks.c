#include <stdio.h>
#include "tracks.h"

int load_tracks(const char *filename, Track *tracks, int max_tracks) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Erreur d'ouverture du fichier CSV");
        return 0;
    }

    int track_count = 0;

    // Lire et ignorer l'en-tête
    char header[100];
    if (fgets(header, sizeof(header), file) == NULL) {
        fclose(file);
        return 0;
    }

    // Lire chaque ligne de données
    while (fscanf(file, "%d,%49[^,],%d,%d\n", &tracks[track_count].id, tracks[track_count].name,
                  &tracks[track_count].min_time_ms, &tracks[track_count].max_time_ms) == 4) {
        track_count++;
        if (track_count >= max_tracks) break;
                  }

    fclose(file);
    return track_count;
}