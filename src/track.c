#include "track.h"
#include <string.h>

// Initialisation d'un circuit
void initialize_circuit(Circuit *circuit, const char *location, const char *gp_name, int laps) {
    strncpy(circuit->location, location, sizeof(circuit->location) - 1);
    circuit->location[sizeof(circuit->location) - 1] = '\0';
    strncpy(circuit->gp_name, gp_name, sizeof(circuit->gp_name) - 1);
    circuit->gp_name[sizeof(circuit->gp_name) - 1] = '\0';
    circuit->laps = laps;
}

// Initialisation de tous les circuits
void initialize_all_circuits(Circuit circuits[]) {

    initialize_circuit(&circuits[0], "Bahrain International Circuit", "Barhain Grand Prix", 57);
    initialize_circuit(&circuits[1], "Jeddah Corniche Circuit", "Saudi Arabian Grand Prix", 50);
    initialize_circuit(&circuits[2], "Albert Park Circuit", "Australian Grand Prix", 58);
    initialize_circuit(&circuits[3], "Suzuka International Racing Course", "Japanese Grand Prix", 53);
    initialize_circuit(&circuits[4], "Shanghai International Circuit", "Chinese Grand Prix", 56);
    initialize_circuit(&circuits[5], "Miami International Autodrome", "Miami Grand Prix", 57);
    initialize_circuit(&circuits[6], "Imola Circuit", "Emilia Romagna Grand Prix", 63);
    initialize_circuit(&circuits[7], "Circuit de Monaco", "Monaco Grand Prix", 78);
    initialize_circuit(&circuits[8], "Circuit Gilles Villeneuve", "Canadian Grand Prix", 70);
    initialize_circuit(&circuits[9], "Circuit de Barcelona-Catalunya", "Spanish Grand Prix", 66);
    initialize_circuit(&circuits[10], " Red Bull Ring", "Austrian Grand Prix", 71);
    initialize_circuit(&circuits[11], "Silverstone Circuit", "British Grand Prix", 52);
    initialize_circuit(&circuits[12], "Hungaroring", "Hungarian Grand Prix", 70);
    initialize_circuit(&circuits[13], "Circuit de Spa-Francorchamps", "Belgian Grand Prix", 44);
    initialize_circuit(&circuits[14], "Circuit Zandvoort", "Dutch Grand Prix", 72);
    initialize_circuit(&circuits[15], "Monza Circuit", "Italian Grand Prix", 53);
    initialize_circuit(&circuits[16], "Baku City Circuit", "Azerbaijan Grand Prix", 51);
    initialize_circuit(&circuits[17], "Marina Bay Street Circuit", "Singapore Grand Prix", 62);
    initialize_circuit(&circuits[18], "Circuit of the Americas", "United States Grand Prix", 56);
    initialize_circuit(&circuits[19], "Autódromo Hermanos Rodríguez", "Mexico City Grand Prix", 71);
    initialize_circuit(&circuits[20], "Interlagos Circuit", "São Paulo Grand Prix", 71);
    initialize_circuit(&circuits[21], "Las Vegas Strip Circuit", "Las Vegas Grand Prix", 50);
    initialize_circuit(&circuits[22], "Lusail International Circuit", "Qatar Grand Prix", 57);
    initialize_circuit(&circuits[23], "Yas Marina Circuit", "Abu Dhabi Grand Prix", 58);
}
