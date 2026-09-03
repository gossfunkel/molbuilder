#include "raylib.h"
#include "raymath.h"
#include <stdlib.h>

#define TAU (M_PI*2.f)

#define SCREEN_WIDTH 600
#define SCREEN_HEIGHT 600

typedef struct Atom Atom;

typedef struct Bond {
    Atom *a;
    Atom *b;
} Bond;

typedef struct Atom {
    size_t atomic_num;
    float *nuclear_charge;
    float atomic_mass;
    Vector3 position;
    size_t num_bonds;
    Bond bonds[8];
} Atom;

Color colours[10] = {
    BLUE, SKYBLUE, ORANGE, MAGENTA, GREEN, GOLD, DARKGREEN, RED, LIME, LIGHTGRAY
};

// effective charges for each available orbital
float *e_q_table[10] = {
    (float[1]){1.f},            // H
    (float[1]){1.688f},         // He
    (float[2]){2.691f, 1.279f}, // Li
    (float[2]){3.685f, 1.912f}, // Be
    (float[3]){4.680f, 2.576f, 2.421f}, // B
    (float[3]){5.673f, 3.217f, 3.136f}, // C
    (float[3]){6.665f, 3.847f, 3.834f}, // N
    (float[3]){7.658f, 4.492f, 4.453f}, // O
    (float[3]){8.650f, 5.128f, 5.100f}, // F
    (float[3]){9.642f, 5.758f, 5.758f}  // Ne
};

float mass_table[10] = {
    1.0080f, // H
    4.0026f, // He
    6.94f, // Li
    9.0122f, // Be
    10.806f, // B
    12.0116f, // C
    14.00728f, // N
    15.99903f, // O
    18.9984f, // F
    20.1797f // Ne
};

#define ATOM_BUFF_SIZE 128
Atom *all_ats = NULL;
size_t num_ats = 0;

// TODO make pure version
Atom *make_atom(size_t atomic_num, Vector3 pos) {
    if ((1+num_ats) % ATOM_BUFF_SIZE == 0) all_ats = realloc(all_ats, sizeof (Atom) * (num_ats + ATOM_BUFF_SIZE));
    all_ats[num_ats] = (Atom){
        atomic_num, e_q_table[atomic_num], mass_table[atomic_num], pos, 0, {{0}}
    };
    return all_ats + num_ats++;
}

int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Molecule Builder");

    all_ats = malloc(sizeof (Atom) * ATOM_BUFF_SIZE);

    Atom *test_hydros[2] = {
        make_atom(1, ((Vector3){200., 200., 0.})),
        make_atom(1, ((Vector3){400., 200., 0.}))
    };

    Bond test_hydrobond = (Bond){test_hydros[0], test_hydros[1]};
    test_hydros[0]->num_bonds = 1;
    test_hydros[0]->bonds[0] = test_hydrobond;
    test_hydros[1]->num_bonds = 1;
    test_hydros[1]->bonds[0] = test_hydrobond;

    double theta = 0.f; int inverter = 0;

    while (!WindowShouldClose()) {
        theta -= GetFrameTime();

        if (IsKeyReleased(KEY_SPACE)) inverter = (inverter) ? 0 : 3;

        BeginDrawing();
            ClearBackground(WHITE);

            for (size_t at = 0; at < num_ats; at++)
                DrawCircle(all_ats[at].position.x, all_ats[at].position.y, 
                           all_ats[at].atomic_num * 4, colours[all_ats[at].atomic_num]);

        EndDrawing();

    }
    CloseWindow();
    return 0;
}