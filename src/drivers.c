#include "drivers.h"
#include <stdio.h>
#include <string.h>

int load_drivers(const char *filename, Driver drivers[], int max_drivers) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Erreur d'ouverture du fichier des pilotes");
        return -1;
    }

    int count = 0;
    while (count < max_drivers && fscanf(file, "%d,%49[^,],%49[^\n]\n",
            &drivers[count].number, drivers[count].name, drivers[count].team) == 3) {
        count++;
            }

    fclose(file);
    return count;
}
