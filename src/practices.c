#include "practices.h"
#include "utils.h"
#include <stdio.h>

void simulate_practice(Driver drivers[], int num_drivers, Circuit circuit) {
    printf("Résultats des essais libres à %s :\n", circuit.name);
    for (int i = 0; i < num_drivers; i++) {
        int lap_time = generate_lap_time(circuit.min_time_ms, circuit.max_time_ms);
        char formatted_time[10];
        format_time(lap_time, formatted_time, sizeof(formatted_time));
        printf("%d. %s (%s) - Temps : %s\n",
            drivers[i].number, drivers[i].name, drivers[i].team, formatted_time);
    }
}
