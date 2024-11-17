#include <stdio.h>
#include <string.h>
#include "drivers.h"

int load_drivers(const char *filename, Driver *drivers, int max_drivers) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Erreur d'ouverture du fichier CSV");
        return 0;
    }

    int driver_count = 0;

    char header[100];
    fgets(header, sizeof(header), file);

    while (fscanf(file, "%d,%d,%49[^,],%49[^,],%29[^\n]",
                  &drivers[driver_count].id,
                  &drivers[driver_count].number,
                  drivers[driver_count].name,
                  drivers[driver_count].team,
                  drivers[driver_count].nationality) == 5) {
        driver_count++;
        if (driver_count >= max_drivers) break;
                  }

    fclose(file);
    return driver_count;
}