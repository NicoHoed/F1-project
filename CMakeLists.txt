# Spécifie la version minimale de CMake requise
cmake_minimum_required(VERSION 3.10)

# Nom du projet
project(F1-project C)

# Définit le standard de C
set(CMAKE_C_STANDARD 99)

# Ajoute les fichiers sources
add_executable(SimulateurF1
        src/main.c
        src/driver.c
        src/championship.c
        src/weekend.c
        src/race.c
        src/track.c
        src/utils.c
)

# Inclut les headers
include_directories(include)
