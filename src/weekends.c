#include "weekends.h"
#include "practices.h"
#include <stdio.h>

void simulate_weekend(Driver drivers[], int num_drivers, Track track) {
    printf("=== Weekend à %s ===\n", track.name);
    simulate_practice(drivers, num_drivers, track);
    // Ajouter les qualifications et la course
}
