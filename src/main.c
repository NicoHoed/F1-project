#include "track.h"
#include "driver.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>

#define NUM_DRIVERS 20
#define MAX_LAPS 10

typedef struct {
    int driver_number;
    int best_time_ms;
    int best_s1;
    int best_s2;
    int best_s3;
    bool in_pit;
    bool is_out;
} DriverTime;

// Génère les temps pour chaque secteur en s’assurant que s1 + s2 + s3 = total_time
void generate_sector_times(int total_time, int *s1, int *s2, int *s3) {
    int base_time = total_time / 3;
    *s1 = base_time + (rand() % 100 - 50);
    *s2 = base_time + (rand() % 100 - 50);
    *s3 = total_time - *s1 - *s2;
}

// Simulation d'une session d'essai pour un pilote donné
void simulate_practice(int driver_index, int laps, Circuit circuit, int pipe_fd[]) {
    close(pipe_fd[0]); // Ferme le côté lecture du tube pour le processus enfant

    srand(time(NULL) + driver_index * 100);

    DriverTime result = { .driver_number = driver_index, .best_time_ms = circuit.max_time_ms, .in_pit = false, .is_out = false };
    result.best_s1 = circuit.max_time_ms;
    result.best_s2 = circuit.max_time_ms;
    result.best_s3 = circuit.max_time_ms;

    for (int lap = 0; lap < laps; lap++) {
        // Possibilité de rentrer au stand ou de quitter la séance
        if (rand() % 10 == 0) {
            result.in_pit = true;
            continue;
        }
        if (rand() % 20 == 0) {
            result.is_out = true;
            break;
        }

        // Générer un temps de tour et ses secteurs
        int lap_time_ms = (rand() % (circuit.max_time_ms - circuit.min_time_ms + 1)) + circuit.min_time_ms;
        int s1, s2, s3;
        generate_sector_times(lap_time_ms, &s1, &s2, &s3);

        if (lap_time_ms < result.best_time_ms) {
            result.best_time_ms = lap_time_ms;
            result.best_s1 = s1;
            result.best_s2 = s2;
            result.best_s3 = s3;
        }
    }

    write(pipe_fd[1], &result, sizeof(DriverTime));
    close(pipe_fd[1]);
    exit(0);
}

// Comparaison des temps pour le classement
int compare_times(const void *a, const void *b) {
    DriverTime *driverA = (DriverTime *)a;
    DriverTime *driverB = (DriverTime *)b;
    return driverA->best_time_ms - driverB->best_time_ms;
}

void display_results_ascii_table(DriverTime results[], Driver drivers[], int num_drivers) {
    // Trier les résultats par meilleur temps
    qsort(results, num_drivers, sizeof(DriverTime), compare_times);

    // Largeur de chaque colonne
    const int col1_width = 5;  // Position
    const int col2_width = 20; // NAME
    const int col3_width = 15; // TIME

    // Largeur totale du tableau
    const int total_width = col1_width + col2_width + col3_width + 6; // +6 pour les séparateurs

    // Ligne supérieure
    printf(" ▛");
    for (int i = 0; i < total_width - 4; i++) printf("▀");
    printf("▜\n");

    // En-têtes
    printf(" ▌%-*s│%-*s│%-*s▐\n", col1_width, "POS", col2_width, "NAME", col3_width, "TIME");

    // Ligne de séparation
    printf(" ▌");
    for (int i = 0; i < col1_width; i++) printf("─");
    printf("┼");
    for (int i = 0; i < col2_width; i++) printf("─");
    printf("┼");
    for (int i = 0; i < col3_width; i++) printf("─");
    printf("▐\n");

    // Corps du tableau
    for (int i = 0; i < num_drivers; i++) {
        int minutes = results[i].best_time_ms / 60000;
        int seconds = (results[i].best_time_ms % 60000) / 1000;
        int milliseconds = results[i].best_time_ms % 1000;

        char time_str[20];
        snprintf(time_str, sizeof(time_str), "%d:%02d:%03d", minutes, seconds, milliseconds);

        printf(" ▌%-*d│%-*s│%-*s▐\n",
               col1_width, i + 1, // Classement
               col2_width, drivers[results[i].driver_number].name,
               col3_width, time_str);
    }

    // Ligne inférieure
    printf(" ▙");
    for (int i = 0; i < total_width - 4; i++) printf("▄");
    printf("▟\n");
}


// Sauvegarde des résultats dans un fichier
void save_results_to_file(const char *filename, DriverTime results[], int num_drivers) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        perror("Erreur d'ouverture du fichier");
        return;
    }

    fprintf(file, "Classement final de la séance d'essais :\n");
    for (int i = 0; i < num_drivers; i++) {
        fprintf(file, "%d. Pilote %d - Temps : %d ms, Secteurs : S1=%d ms, S2=%d ms, S3=%d ms\n",
                i + 1, results[i].driver_number, results[i].best_time_ms,
                results[i].best_s1, results[i].best_s2, results[i].best_s3);
    }

    fclose(file);
}

int main() {
    Circuit circuits[24];
    initialize_all_circuits(circuits);
    Circuit current_circuit = circuits[0];

    Driver drivers[NUM_DRIVERS];
    initialize_all_drivers(drivers);

    int pipes[NUM_DRIVERS][2];
    DriverTime results[NUM_DRIVERS];

    for (int i = 0; i < NUM_DRIVERS; i++) {
        if (pipe(pipes[i]) == -1) {
            perror("Erreur de création de pipe");
            exit(EXIT_FAILURE);
        }

        pid_t pid = fork();
        if (pid == -1) {
            perror("Erreur de fork");
            exit(EXIT_FAILURE);
        } else if (pid == 0) {
            simulate_practice(i, MAX_LAPS, current_circuit, pipes[i]);
        }
    }

    for (int i = 0; i < NUM_DRIVERS; i++) {
        close(pipes[i][1]);
        read(pipes[i][0], &results[i], sizeof(DriverTime));
        close(pipes[i][0]);
        wait(NULL);
    }

    display_results_ascii_table(results, drivers, NUM_DRIVERS);
    save_results_to_file("practice_results.txt", results, NUM_DRIVERS);

    return 0;
}
