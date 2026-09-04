#include "raylib.h"
#include "raymath.h"
#include <stdlib.h>
#include <stdio.h>

#define SCREEN_WIDTH 600
#define SCREEN_HEIGHT 600

#define MAX_INPUT 32

// DATA TYPES

typedef struct Atom Atom;

typedef struct Bond {
    Atom *a;
    Atom *b;
} Bond;

typedef struct Atom {
    size_t atomic_num;
    Vector3 position;
    size_t num_bonds;
    Bond bonds[8];
} Atom;

typedef struct Mol {
    size_t num;
    Atom *atoms[];
} Mol;

typedef struct MolBag {
    size_t num;
    Mol *mols[];
} MolBag;

// GLOBAL STATE/MEMORY

char *names[10] = {
    "H", "He", "Li", "Be", "B", "C", "N", "O", "F", "Ne"
};

#define ATOM_BUFF_SIZE 128
Atom *all_ats = NULL;
size_t num_ats = 0;

bool running = true;

// UTILITY FNS

Atom *make_atom(size_t atomic_num, Vector3 pos) {
    if ((1+num_ats) % ATOM_BUFF_SIZE == 0) all_ats = realloc(all_ats, sizeof (Atom) * (num_ats + ATOM_BUFF_SIZE));
    all_ats[num_ats] = (Atom){
        atomic_num, pos, 0, {{0}}
    };
    return all_ats + num_ats++;
}

#define make_mol(num) (Mol *)malloc(sizeof (size_t) + sizeof (Atom *) * ((num > 8) ? num : 8);

void add_to_mol(Mol *mol, Atom *at) {
    mol->num++;
    if (mol->num > 8)
        mol = realloc(mol, sizeof (size_t) + sizeof (Atom *) * mol->num);
    mol->atoms[mol->num - 1] = at;
}

Atom **save_mol(MolBag *saved_mols, Mol *mol) {
    if (1 + saved_mols->num % ATOM_BUFF_SIZE < 0) 
        saved_mols = 
            realloc(saved_mols, sizeof (size_t) + sizeof (Mol *) * (saved_mols->num + ATOM_BUFF_SIZE + 1));
    saved_mols->mols[saved_mols->num] = mol;
    return saved_molecules->mols + saved_mols->num++;
}

// ACTIONS

void hydrogenate(Mol *mol) {

}

void saturate(Atom *at) {

}

void carbonate(Atom *at) {
    
}

void oxidise(Atom *at) {
    
}

void reduce(Atom *at) {
    
}

/*
 * GAME LOGIC
 * 
 *  Simple rules to discover new molecules:
 *  - Hydrogenate: give every free electron on your molecule a Hydrogen
 *  - Saturate: remove a Hydrogen from an r-C-C, leaving an r-C=C
 *  - Carbonate: attach an r-CH=O to a free electron 
 *  - Oxidise: remove any Hydrogen and replace with an r=O
 *  - Reduce: break an r=O double bond, leaving a free electron (r-O-e)
 */

void parse_command(char *in_buff, Mol **selected_mol, size_t selected_atom) {
    // TODO atom selection
    if (strstr(in_buff, "0") == in_buff 
     || strstr(in_buff, "hydrogenate") == in_buff) {
        hydrogenate(*selected_mol);
    } else if (strstr(in_buff, "1") == in_buff 
     || strstr(in_buff, "saturate") == in_buff) {
        saturate(all_ats+selected_atom);
    } else if (strstr(in_buff, "2") == in_buff 
     || strstr(in_buff, "carbonate") == in_buff) {
        carbonate(all_ats+selected_atom);
    } else if (strstr(in_buff, "3") == in_buff 
     || strstr(in_buff, "oxidise") == in_buff) {
        oxidise(all_ats+selected_atom);
    } else if (strstr(in_buff, "4") == in_buff 
     || strstr(in_buff, "reduce") == in_buff) {
        reduce(all_ats+selected_atom);
    } else if (strstr(in_buff, "?") == in_buff 
     || strstr(in_buff, "help") == in_buff) {
        printf("\t===== MolBuilder =====\n");
        printf("A string logic to build molecules.\n");
        printf("Using a few limited operations, your task is to collect as many");
        printf(" different molecules as you can.\n");
        printf("--- Commands:\n");
        printf("Hydrogenate: give every free electron on your molecule a Hydrogen.\n");
        printf("Saturate: remove a Hydrogen from an r-C-C, leaving an r-C=C.\n");
        printf("Carbonate: attach an r-CH=O to a free electron.\n");
        printf("Oxidise: remove any Hydrogen and replace with an r=O.\n");
        printf("Reduce: break an r=O double bond, leaving a free electron (r-O-e).\n");
        printf("Help: print commands.");
        printf("\t =====            =====\n")
    } else if (strstr(in_buff, "X") == in_buff 
     || strstr(in_buff, "exit") == in_buff) {
        saturate(all_ats+selected_atom);
    } else {
        printf("Command not recognised!\n");
    }
}

print_mol(Mol *mol) {
    // TODO show shape and bonds
    //      rather than exploring via storage, explore via links
    //      n.b. account for graph cycles
    for (size_t a = 0; a < mol->size; a++) {
        printf("%s", names[mol->atoms[a].atomic_num]);
    }
    // TODO show selected
}

int main() {

    all_ats = malloc(sizeof (Atom) * ATOM_BUFF_SIZE);

    Atom **saved_molecules = malloc(sizeof (Atom *) * ATOM_BUFF_SIZE);

    Mol *selected_mol = NULL;

    size_t selected_atom = 0;

    char in_buff[MAX_INPUT];

    while (running) {
        fgets(in_buff, MAX_INPUT, stdin);
        parse_command(in_buff, &selected_mol, selected_atom);
        print_mol(selected_mol);
    }
}