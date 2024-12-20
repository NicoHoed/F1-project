#include "f1_championship.h"
#include <unistd.h> // Pour sleep()

#define NUM_PRACTICE_LAPS 5
#define NUM_QUALIFICATION_LAPS 5
#define NUM_RACE_LAPS 10
#define TOUR_SLEEP_TIME 1 // Temps d'attente en secondes


void wait_for_next_session() {
    printf("\nSession terminée.\n");
    printf("Appuyez sur Entrée pour continuer à la prochaine session...\n");
    getchar(); // Attend que l'utilisateur appuie sur Entrée
}


// Function to clear the terminal screen
void clear_screen() {
    printf("\033[H\033[J");
}
int main() {
    // Create shared memory
    key_t key = ftok("shmfile", 65);
    int shmid = shmget(key, sizeof(struct SharedMemory), 0666 | IPC_CREAT);
    struct SharedMemory *shm = (struct SharedMemory *)shmat(shmid, (void *)0, 0);

    // Initialize shared memory
    initialize_shared_memory(shm);

    // Create a semaphore for synchronization
    int sem_id = semget(IPC_PRIVATE, 1, 0666 | IPC_CREAT);
    semctl(sem_id, 0, SETVAL, 1);

    // Simulate practice sessions (P1, P2, P3)
    for (int session = 1; session <= 3; session++) {
        printf("Starting Practice Session P%d...\n", session);
        for (int lap = 0; lap < NUM_PRACTICE_LAPS; lap++) {
            for (int i = 0; i < NUM_CARS; i++) {
                simulate_lap(shm, i, sem_id);
            }
            clear_screen();
            printf("Practice Session P%d...\n", session);
            print_current_standings(shm);
            sleep(TOUR_SLEEP_TIME);
        }
        char filename[20];
        sprintf(filename, "P%d_results.txt", session);
        save_session_results(shm, filename);
        wait_for_next_session();
    }


    // Simulate qualification sessions (Q1, Q2, Q3)
    for (int session = 1; session <= 3; session++) {
        printf("Starting Qualification Session Q%d...\n", session);
        for (int lap = 0; lap < NUM_QUALIFICATION_LAPS; lap++) { 
            for (int i = 0; i < NUM_CARS; i++) {
                simulate_lap(shm, i, sem_id);
            }
            clear_screen();
            printf("Qualification Session Q%d...\n", session);
            print_current_standings(shm);
            sleep(TOUR_SLEEP_TIME);
        }
        char filename[20];
        sprintf(filename, "Q%d_results.txt", session);
        save_session_results(shm, filename);
        wait_for_next_session();

        // Remove cars that did not qualify for the next session
        if (session < 3) {
            int cutoff = (session == 1) ? 15 : 10;
            sem_lock(sem_id);
            for (int i = cutoff; i < NUM_CARS; i++) {
                shm->cars[i].out = 1;
            }
            sem_unlock(sem_id);
        }
    }

    // Simulate the race

    printf("Starting the race...\n");

    // Reset all cars to active status for the race
    printf("Resetting all cars to active status for the race...\n");
    sem_lock(sem_id);
    for (int i = 0; i < NUM_CARS; i++) {
        shm->cars[i].out = 0; // Reset the 'out' status
    }
    sem_unlock(sem_id);

    // Race simulation loop
    printf("Starting the Race...\n");
    for (int lap = 0; lap < NUM_RACE_LAPS; lap++) { 
        for (int i = 0; i < NUM_CARS; i++) {
            simulate_lap(shm, i, sem_id);
        }
        clear_screen();
        printf("Racing\n");
        print_current_standings(shm);
        sleep(TOUR_SLEEP_TIME);
    }
    save_session_results(shm, "Race_results.txt");
    wait_for_next_session();

        // Allocate points based on the race results
    allocate_points(shm, 2); 

    // Print championship standings
    clear_screen(); 
    print_championship_standings(shm);

    // Clean up
    shmdt(shm);
    shmctl(shmid, IPC_RMID, NULL);
    semctl(sem_id, 0, IPC_RMID);

    return 0;
}