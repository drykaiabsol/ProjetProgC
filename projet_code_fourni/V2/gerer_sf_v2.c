#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sf.h"
#include "inode.h"

int main(){
    printf("--- Test Niveau 2 : Systeme de Fichiers ---\n\n");

    FILE *f = fopen("test_import.txt", "w");
    if (f != NULL)
    {
        fprintf(f, "Ceci est un petit texte pour tester le SF V2.");
        fclose(f);
    }
    printf("-> Creation du SF 'DisqueDur'...\n");
    tSF monSF = CreerSF("DisqueDur");

    if (monSF == NULL)
    {
        fprintf(stderr, "Erreur lors de la creation du SF.\n");
        return EXIT_FAILURE;
    }
    printf("-> Importation de 'test_import.txt' dans le SF...\n");
    long octets = Ecrire1BlocFichierSF(monSF, "test_import.txt", ORDINAIRE);

    if (octets == -1)
    {
        printf("Erreur lors de l'ecriture du fichier.\n");
    }else{
        printf("Succes : %ld octets ecrits dans le SF.\n", octets);
    }
    printf("\n-> Affichage du contenu du SF :\n");
    AfficherSF(monSF);

    printf("\n-> Destruction du SF...\n");
    DetruireSF(&monSF);

    if (monSF ==NULL)
    {
        printf("Le SF a bien ete detruit (pointeur NULL).\n");
    }
    printf("\n--- Fin du test ---\n");
    return EXIT_SUCCESS;
}