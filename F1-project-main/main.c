#include "f1_championship.h"
#include <unistd.h>

#define NUM_PRACTICE_LAPS 5
#define NUM_QUALIFICATION_LAPS 5
#define NUM_RACE_LAPS 10
#define NUM_SPRINT_LAPS 5
#define TOUR_SLEEP_TIME 1
#define WEEKEND_NORMAL 0
#define WEEKEND_SPRINT 1
#define NUM_RACES 23 // Nombre de courses dans la saison

// Structure pour définir un Grand Prix
struct GrandPrix {
    const char* name;
    int weekend_type;
};

// Calendrier de la saison 2024
struct GrandPrix season_calendar[NUM_RACES] = {
    {"Bahrain GP", WEEKEND_NORMAL},
    {"Saudi Arabian GP", WEEKEND_NORMAL},
    {"Australian GP", WEEKEND_NORMAL},
    {"Japanese GP", WEEKEND_NORMAL},
    {"Chinese GP", WEEKEND_SPRINT},
    {"Miami GP", WEEKEND_SPRINT},
    {"Emilia Romagna GP", WEEKEND_NORMAL},
    {"Monaco GP", WEEKEND_NORMAL},
    {"Canadian GP", WEEKEND_NORMAL},
    {"Spanish GP", WEEKEND_NORMAL},
    {"Austrian GP", WEEKEND_SPRINT},
    {"British GP", WEEKEND_NORMAL},
    {"Hungarian GP", WEEKEND_NORMAL},
    {"Belgian GP", WEEKEND_NORMAL},
    {"Dutch GP", WEEKEND_NORMAL},
    {"Italian GP", WEEKEND_NORMAL},
    {"Azerbaijan GP", WEEKEND_SPRINT},
    {"Singapore GP", WEEKEND_NORMAL},
    {"United States GP", WEEKEND_SPRINT},
    {"Mexico City GP", WEEKEND_NORMAL},
    {"São Paulo GP", WEEKEND_SPRINT},
    {"Las Vegas GP", WEEKEND_NORMAL},
    {"Abu Dhabi GP", WEEKEND_NORMAL}
};

void wait_for_next_session() {
    printf("\nSession terminée.\n");
    printf("Appuyez sur Entrée pour continuer...\n");
    getchar();
}

void clear_screen() {
    printf("\033[H\033[J");
}

void simulate_race_weekend(struct SharedMemory *shm, int sem_id, const struct GrandPrix *gp) {
    printf("\n=== %s ===\n", gp->name);

    // Practice sessions
    if (gp->weekend_type == WEEKEND_NORMAL) {
        for (int session = 1; session <= 3; session++) {
            printf("Starting Practice Session P%d...\n", session);
            for (int lap = 0; lap < NUM_PRACTICE_LAPS; lap++) {
                for (int i = 0; i < NUM_CARS; i++) {
                    simulate_lap(shm, i, sem_id);
                }
                clear_screen();
                printf("%s - Practice Session P%d...\n", gp->name, session);
                print_current_standings(shm);
                sleep(TOUR_SLEEP_TIME);
            }
            char filename[50];
            sprintf(filename, "%s_P%d_results.txt", gp->name, session);
            save_session_results(shm, filename);
            wait_for_next_session();
        }
    } else {
        printf("Starting Practice Session P1...\n");
        for (int lap = 0; lap < NUM_PRACTICE_LAPS; lap++) {
            for (int i = 0; i < NUM_CARS; i++) {
                simulate_lap(shm, i, sem_id);
            }
            clear_screen();
            printf("%s - Practice Session P1...\n", gp->name);
            print_current_standings(shm);
            sleep(TOUR_SLEEP_TIME);
        }
        save_session_results(shm, "P1_results.txt");
        wait_for_next_session();
    }

    // Qualification
    for (int session = 1; session <= 3; session++) {
        printf("Starting Qualification Session Q%d...\n", session);
        for (int lap = 0; lap < NUM_QUALIFICATION_LAPS; lap++) {
            for (int i = 0; i < NUM_CARS; i++) {
                simulate_lap(shm, i, sem_id);
            }
            clear_screen();
            printf("%s - Qualification Session Q%d...\n", gp->name, session);
            print_current_standings(shm);
            sleep(TOUR_SLEEP_TIME);
        }
        char filename[50];
        sprintf(filename, "%s_Q%d_results.txt", gp->name, session);
        save_session_results(shm, filename);
        wait_for_next_session();

        if (session < 3) {
            int cutoff = (session == 1) ? 15 : 10;
            sem_lock(sem_id);
            for (int i = cutoff; i < NUM_CARS; i++) {
                shm->cars[i].out = 1;
            }
            sem_unlock(sem_id);
        }
    }

    // Sprint Race
    if (gp->weekend_type == WEEKEND_SPRINT) {
        printf("Starting Sprint Race...\n");

        sem_lock(sem_id);
        for (int i = 0; i < NUM_CARS; i++) {
            shm->cars[i].out = 0;
            shm->cars[i].best_lap_time = MAX_LAP_TIME;
        }
        sem_unlock(sem_id);

        for (int lap = 0; lap < NUM_SPRINT_LAPS; lap++) {
            for (int i = 0; i < NUM_CARS; i++) {
                simulate_lap(shm, i, sem_id);
            }
            clear_screen();
            printf("%s - Sprint Race - Lap %d/%d\n", gp->name, lap + 1, NUM_SPRINT_LAPS);
            print_current_standings(shm);
            sleep(TOUR_SLEEP_TIME);
        }

        char filename[50];
        sprintf(filename, "%s_Sprint_results.txt", gp->name);
        save_session_results(shm, filename);
        allocate_points(shm, 1);
        print_championship_standings(shm);
        wait_for_next_session();
    }

    // Main Race
    printf("Starting the %s Grand Prix...\n", gp->name);
    sem_lock(sem_id);
    for (int i = 0; i < NUM_CARS; i++) {
        shm->cars[i].out = 0;
    }
    sem_unlock(sem_id);

    for (int lap = 0; lap < NUM_RACE_LAPS; lap++) {
        for (int i = 0; i < NUM_CARS; i++) {
            simulate_lap(shm, i, sem_id);
        }
        clear_screen();
        printf("%s - Main Race - Lap %d/%d\n", gp->name, lap + 1, NUM_RACE_LAPS);
        print_current_standings(shm);
        sleep(TOUR_SLEEP_TIME);
    }
    char filename[50];
    sprintf(filename, "%s_Race_results.txt", gp->name);
    save_session_results(shm, filename);
    allocate_points(shm, 2);

    clear_screen();
    printf("\nChampionship Standings after %s:\n", gp->name);
    print_championship_standings(shm);
    wait_for_next_session();
}

int main() {
    key_t key = ftok("shmfile", 65);
    int shmid = shmget(key, sizeof(struct SharedMemory), 0666 | IPC_CREAT);
    struct SharedMemory *shm = (struct SharedMemory *)shmat(shmid, (void *)0, 0);
    initialize_shared_memory(shm);

    int sem_id = semget(IPC_PRIVATE, 1, 0666 | IPC_CREAT);
    semctl(sem_id, 0, SETVAL, 1);

    printf("Starting F1 2024 Season Simulation\n");
    printf("Press Enter to begin...\n");
    getchar();

    // Simulate each race in the season
    for (int race = 0; race < NUM_RACES; race++) {
        simulate_race_weekend(shm, sem_id, &season_calendar[race]);
    }

    // Final Championship Standings
    clear_screen();
    printf("\n=== Final 2024 F1 Championship Standings ===\n");
    print_championship_standings(shm);

    shmdt(shm);
    shmctl(shmid, IPC_RMID, NULL);
    semctl(sem_id, 0, IPC_RMID);

    return 0;
}