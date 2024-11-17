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
    int num_circuits = load_circuits("data/circuits.txt", circuits, MAX_CIRCUITS);

    if (num_drivers <= 0 || num_circuits <= 0) {
        fprintf(stderr, "Erreur lors du chargement des données.\n");
        return 1;
    }

    for (int i = 0; i < num_circuits; i++) {
        simulate_weekend(drivers, num_drivers, circuits[i]);
    }

    return 0;
}
