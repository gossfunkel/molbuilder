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
    Vector3 position;
    RayCollision collision;
    size_t num_bonds;
    Bond bonds[8];
} Atom;

Color colours[10] = {
    BLUE, SKYBLUE, ORANGE, MAGENTA, GREEN, GOLD, DARKGREEN, RED, LIME, LIGHTGRAY
};

#define ATOM_BUFF_SIZE 128
Atom *all_ats = NULL;
size_t num_ats = 0;

Atom *make_atom(size_t atomic_num, Vector3 pos) {
    if ((1+num_ats) % ATOM_BUFF_SIZE == 0) all_ats = realloc(all_ats, sizeof (Atom) * (num_ats + ATOM_BUFF_SIZE));
    all_ats[num_ats] = (Atom){
        atomic_num, pos, {0}, 0, {{0}}
    };
    return all_ats + num_ats++;
}

/*
 * GAME LOGIC
 * 
 *  Simple rules to discover new molecules:
 *  - Hydrogenate: give every free electron on your molecule a Hydrogen
 *  - Saturate: remove a Hydrogen from an r-C-C, leaving an r-C=C
 *  - Carbonate: attach an r-CH=O to a free electron on a Carbon
 *  - Oxidise: remove any Hydrogen and replace with an r=O
 *  - Reduce: break an r=O double bond, leaving a free electron (r-O-e)
 */

int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Molecule Builder");

    Camera camera = { 0 };
    camera.position = (Vector3){ 0.0f, -50.0f, 15.0f }; // Camera position
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };      // Camera looking at point
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    camera.fovy = 45.0f;                                // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;             // Camera projection type

    Ray ray = { 0 };                    // Picking line ray

    all_ats = malloc(sizeof (Atom) * ATOM_BUFF_SIZE);

    Atom *test_hydros[2] = {
        make_atom(1, ((Vector3){ 5., 0., 0.})),
        make_atom(1, ((Vector3){-5., 0., 0.}))
    };

    Bond test_hydrobond = (Bond){test_hydros[0], test_hydros[1]};
    test_hydros[0]->num_bonds = 1;
    test_hydros[0]->bonds[0] = test_hydrobond;
    test_hydros[1]->num_bonds = 1;
    test_hydros[1]->bonds[0] = test_hydrobond;

    //double dt = 0.f;

    bool selected = false;
    size_t selected_atom = 0;

    while (!WindowShouldClose()) {
        //dt = GetFrameTime();
        //if (IsKeyReleased(KEY_SPACE)) break;

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            //if (!selected) {
                ray = GetScreenToWorldRay(GetMousePosition(), camera);
                selected = false;
                for (size_t at = 0; at < num_ats; at++) {
                    all_ats[at].collision = GetRayCollisionSphere(ray, all_ats[at].position, all_ats[at].atomic_num*4);
                    //any_coll = all_ats[at].collision;
                    if (all_ats[at].collision.hit) {
                        selected_atom = at;
                        selected = true;
                    }
                }
            //} else selected = false;
        }

        BeginDrawing();
            ClearBackground(BLACK);

            BeginMode3D(camera);

            for (size_t at = 0; at < num_ats; at++)
                DrawSphere(all_ats[at].position, all_ats[at].atomic_num, colours[all_ats[at].atomic_num]);

            if (selected)
                DrawSphereWires(all_ats[selected_atom].position, 
                                all_ats[selected_atom].atomic_num * 2,
                                6, 6, WHITE);

            EndMode3D();

        EndDrawing();

    }
    CloseWindow();
    return 0;
}