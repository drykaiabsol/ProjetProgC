#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "sf.h"
#include "inode.h"

void CreerFichierTest(char *nom, char *contenu) {
    FILE *f = fopen(nom, "w");
    if (f != NULL) {
        fprintf(f, "%s", contenu);
        fclose(f);
    }
}

int main() {
    printf("--- Test Niveau 4 ---\n\n");

    CreerFichierTest("chat.txt", "Le chat miaule.");
    CreerFichierTest("lion.txt", "Le lion rugit fort.");
    CreerFichierTest("vide.txt", "");

    printf("-> Creation du SF...\n");
    tSF monSF = CreerSF("DisqueRoot");
    if (monSF == NULL) return EXIT_FAILURE;

    printf("-> Ajout de 3 fichiers...\n");
    EcrireFichierSF(monSF, "chat.txt", ORDINAIRE);
    EcrireFichierSF(monSF, "lion.txt", ORDINAIRE);
    EcrireFichierSF(monSF, "vide.txt", ORDINAIRE);

    printf("\n--- Test Ls (Simple) ---\n");
    Ls(monSF, false);

    printf("\n--- Test Ls (Detail) ---\n");
    Ls(monSF, true);

    printf("\n-> Sauvegarde et destruction...\n");
    SauvegarderSF(monSF, "sauvegarde_v4.dat");
    DetruireSF(&monSF);

    printf("-> Restauration...\n");
    tSF sfRestaurer = NULL;
    ChargerSF(&sfRestaurer, "sauvegarde_v4.dat");

    printf("\n--- Test Ls apres restauration ---\n");
    Ls(sfRestaurer, true);

    DetruireSF(&sfRestaurer);
    
    remove("chat.txt");
    remove("lion.txt");
    remove("vide.txt");

    printf("\n--- FIN TEST V4 ---\n");
    return 0;
}