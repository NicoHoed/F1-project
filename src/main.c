#include "drivers.h"
#include "tracks.h"
#include "weekends.h"
#include <stdio.h>

#define MAX_DRIVERS 20
#define MAX_CIRCUITS 10

int main() {
    Driver drivers[MAX_DRIVERS];
    Circuit circuits[MAX_CIRCUITS];

    int num_drivers = load_drivers("data/drivers.txt", drivers, MAX_DRIVERS);
    int num_circuits = load_circuits("data/tracks.txt", circuits, MAX_CIRCUITS);

    if (num_drivers <= 0) {
        fprintf(stderr, "Erreur : Aucun pilote chargé depuis %s\n", "data/drivers.txt");
    }
    if (num_circuits <= 0) {
        fprintf(stderr, "Erreur : Aucun circuit chargé depuis %s\n", "data/tracks.txt");
    }


    for (int i = 0; i < num_circuits; i++) {
        simulate_weekend(drivers, num_drivers, circuits[i]);
    }

    return 0;
}
