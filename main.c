#include "f1_championship.h"
#include <unistd.h>

#define NUM_PRACTICE_LAPS 5
#define NUM_QUALIFICATION_LAPS 5
#define NUM_RACE_LAPS 10
#define TOUR_SLEEP_TIME 1        // Temps d'attente en secondes

// Fonction appuie sur Entrée avant de passer à la prochaine session
void wait_for_next_session() {
    printf("\nSession terminée.\n");
    printf("Appuyez sur Entrée pour continuer à la prochaine session...\n");
    getchar(); // Attend Entrée
}

// Fonction pour nettoyer l'écran terminal
void clear_screen() {
    printf("\033[H\033[J"); // Commande ANSI pour nettoyer l'écran
}

int main() {
    // Création de la mémoire partagée
    key_t key = ftok("shmfile", 65);        // Clé de la mémoire partagée
    int shmid = shmget(key, sizeof(struct SharedMemory), 0666 | IPC_CREAT); // ID de la mémoire partagée
    struct SharedMemory *shm = (struct SharedMemory *)shmat(shmid, (void *)0, 0);

    // Initialisation de la mémoire partagée
    initialize_shared_memory(shm);

    // Création d'un sémaphore pour synchronisation
    int sem_id = semget(IPC_PRIVATE, 1, 0666 | IPC_CREAT);  // Creation d'un sémaphore privé
    semctl(sem_id, 0, SETVAL, 1);                             // Initialisation à 1 (verrouille tout début)

    // Simulation des sessions d'entraînement (P1, P2, P3)
    for (int session = 1; session <= 3; session++) {
        printf("Starting Practice Session P%d...\n", session);
        for (int lap = 0; lap < NUM_PRACTICE_LAPS; lap++) {
            for (int i = 0; i < NUM_CARS; i++) {
                simulate_lap(shm, i, sem_id); // Simule un tour pour chaque voiture
            }
            clear_screen();
            printf("Practice Session P%d...\n", session);
            print_current_standings(shm);    // classement après chaque tour
            sleep(TOUR_SLEEP_TIME);           // Attente avant le prochain tour
        }
        char filename[20];
        sprintf(filename, "P%d_results.txt", session);  // Crée le nom du fichier pour les résultats
        save_session_results(shm, filename);            // Enregistre les résultats de la session
        wait_for_next_session();                        // Attente pour la prochaine session
    }

    // Simulation des sessions de qualification (Q1, Q2, Q3)
    for (int session = 1; session <= 3; session++) {
        printf("Starting Qualification Session Q%d...\n", session);
        for (int lap = 0; lap < NUM_QUALIFICATION_LAPS; lap++) {
            for (int i = 0; i < NUM_CARS; i++) {
                simulate_lap(shm, i, sem_id); // Simule un tour pour chaque voiture
            }
            clear_screen();
            printf("Qualification Session Q%d...\n", session);
            print_current_standings(shm);    // Affiche le classement après chaque tour
            sleep(TOUR_SLEEP_TIME);           // Attente avant le prochain tour
        }
        char filename[20];
        sprintf(filename, "Q%d_results.txt", session);  // Création du nom du fichier pour les résultats
        save_session_results(shm, filename);            // Enregistre les résultats de la session
        wait_for_next_session();

        // Élimination des voitures qui n'ont pas réussi à se qualifier pour les sessions suivantes
        if (session < 3) { // Exclut les voitures classées trop loin
            int cutoff = (session == 1) ? 15 : 10;
            sem_lock(sem_id);
            for (int i = cutoff; i < NUM_CARS; i++) {
                shm->cars[i].out = 1; // Marque ces voitures comme out
            }
            sem_unlock(sem_id);
        }
    }

    // Simulation de la course
    printf("Starting the race...\n");

    // Réinitialisation de toutes les voitures pour la course
    printf("Resetting all cars to active status for the race...\n");
    sem_lock(sem_id);
    for (int i = 0; i < NUM_CARS; i++) {
        shm->cars[i].out = 0; // Réinitialise la statut "out" des voitures
    }
    sem_unlock(sem_id);

    // Boucle de simulation de la course
    printf("Starting the Race...\n");
    for (int lap = 0; lap < NUM_RACE_LAPS; lap++) {
        for (int i = 0; i < NUM_CARS; i++) {
            simulate_lap(shm, i, sem_id); // Simule un tour pour chaque voiture
        }
        clear_screen();
        printf("Racing...\n");
        print_current_standings(shm);    // Affiche le classement en cours
        sleep(TOUR_SLEEP_TIME);           // Attente avant le prochain tour
    }
    save_session_results(shm, "Race_results.txt");  // Enregistre les résultats de la course
    wait_for_next_session();                        // Attente avant la fin de la simulation

    // Attribution des points basés sur les résultats de la course
    allocate_points(shm, 2); // 2 pour les points la course

    // Affichage des classements finaux du championnat
    clear_screen();
    print_championship_standings(shm);

    // Nettoyage et libération de la mémoire partagée et des sémaphores
    shmdt(shm);          // Détache la mémoire partagée
    shmctl(shmid, IPC_RMID, NULL);   // Supprime la mémoire partagée
    semctl(sem_id, 0, IPC_RMID);    // Libère le sémaphore

    return 0;
}
