#include "tracks.h"
#include <stdio.h>

int load_circuits(const char *filename, Circuit circuits[], int max_circuits) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Erreur d'ouverture du fichier des circuits");
        return -1;
    }

    int count = 0;
    while (count < max_circuits && fscanf(file, "%49[^,],%d,%d\n",
            circuits[count].name, &circuits[count].min_time_ms, &circuits[count].max_time_ms) == 3) {
        count++;
            }

    fclose(file);
    return count;
}
