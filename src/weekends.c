#include "weekends.h"
#include "practices.h"
#include <stdio.h>

void simulate_weekend(Driver drivers[], int num_drivers, Circuit circuit) {
    printf("=== Weekend à %s ===\n", circuit.name);
    simulate_practice(drivers, num_drivers, circuit);
    // Ajouter les qualifications et la course
}
