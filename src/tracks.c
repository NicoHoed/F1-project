#include "tracks.h"
#include <stdio.h>
#include <stdbool.h>

int load_circuits(const char *filename, Circuit circuits[], int max_circuits) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Erreur d'ouverture du fichier");
        return 0; // Retourne 0 pour indiquer qu'aucun pilote n'a été chargé
    }



    int count = 0;
    while (count < max_circuits && fscanf(file, "%49[^,],%d,%d\n",
            circuits[count].name, &circuits[count].min_time_ms, &circuits[count].max_time_ms) == 3) {
        count++;
            }

    fclose(file);
    return count;
}
