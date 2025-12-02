#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sf.h"
#include "inode.h"

int main() {
    printf("--- Test Niveau 3 : Multi-blocs et Persistance ---\n\n");

    FILE *f = fopen("test_v3.txt", "w");
    if (f != NULL) {
        int i = 0;
        while (i < 150) {
            fputc('A', f);
            i++;
        }
        fclose(f);
        printf("-> Fichier test_v3.txt cree (150 octets).\n");
    } else {
        fprintf(stderr, "Erreur creation fichier test.\n");
        return EXIT_FAILURE;
    }

    printf("-> Creation du SF 'DisqueV3'...\n");
    tSF monSF = CreerSF("DisqueV3");
    
    if (monSF == NULL) {
        fprintf(stderr, "Erreur lors de la creation du SF.\n");
        return EXIT_FAILURE;
    }

    printf("-> Importation de 'test_v3.txt' dans le SF...\n");
    long octets = EcrireFichierSF(monSF, "test_v3.txt", ORDINAIRE);
    
    if (octets == 150) {
        printf("Succes : 150 octets ecrits (sur 3 blocs).\n");
    } else {
        printf("Erreur : %ld octets ecrits au lieu de 150.\n", octets);
    }

    printf("\n-> Sauvegarde du SF dans 'sauvegarde.dat'...\n");
    if (SauvegarderSF(monSF, "sauvegarde.dat") == 0) {
        printf("Sauvegarde reussie.\n");
    } else {
        printf("Echec de la sauvegarde !\n");
    }

    printf("-> Destruction du SF en memoire...\n");
    DetruireSF(&monSF);

    if (monSF == NULL) {
        printf("Memoire nettoyee.\n");
    }

    printf("\n-> Restauration du SF depuis 'sauvegarde.dat'...\n");
    tSF sfRestaurer = NULL;
    
    if (ChargerSF(&sfRestaurer, "sauvegarde.dat") == 0) {
        printf("Chargement reussi !\n");
        
        printf("\nAffichage du SF restaure :\n");
        AfficherSF(sfRestaurer);
        
        DetruireSF(&sfRestaurer);
    } else {
        printf("Echec du chargement !\n");
    }

    printf("\n--- Fin du test V3 ---\n");
    return EXIT_SUCCESS;
}