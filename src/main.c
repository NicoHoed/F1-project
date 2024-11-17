#include <stdio.h>
#include "tracks.h"
#include "drivers.h"

int main() {
    Track tracks[MAX_TRACKS];
    int num_tracks = load_tracks("../data/tracks.csv", tracks, MAX_TRACKS);

    if (num_tracks > 0) {
        printf("Circuits chargés : %d\n", num_tracks);
        for (int i = 0; i < num_tracks; i++) {
            printf("ID: %d, Nom: %s, Temps min: %d ms, Temps max: %d ms\n",
                tracks[i].id, tracks[i].name, tracks[i].min_time_ms, tracks[i].max_time_ms);
        }
    } else {
        printf("Aucun circuit chargé.\n");
    }

    // Charger les pilotes
    Driver drivers[MAX_DRIVERS];
    int num_drivers = load_drivers("../data/drivers.csv", drivers, MAX_DRIVERS);

    if (num_drivers > 0) {
        printf("\nPilotes chargés : %d\n", num_drivers);
        for (int i = 0; i < num_drivers; i++) {
            printf("ID: %d, Nom: %s, Équipe: %s, Nationalité: %s\n",
                drivers[i].id, drivers[i].name, drivers[i].team, drivers[i].nationality);
        }
    } else {
        printf("Aucun pilote chargé.\n");
    }

    return 0;
}
