#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "inode.h" 

int main() {
    tInode monInode;
    unsigned char buffer[64];
    long octetsLus;

    printf("--- Test de la Version 1 (Gestion d'inodes et de blocs) ---\n\n");

    monInode = CreerInode(1, ORDINAIRE);
    if (monInode == NULL) {
        fprintf(stderr, "Echec de creation de l'inode.\n");
        return EXIT_FAILURE;
    }
    
    printf("-> Écriture du contenu...\n");
    char *contenu = "Bonjour Paul Sabatier ! Ceci est le test V1.";
    long taille_contenu = strlen(contenu);
    
    EcrireDonneesInode1bloc(monInode, (unsigned char*)contenu, taille_contenu);
    
    AfficherInode(monInode);

    octetsLus = LireDonneesInode1bloc(monInode, buffer, 64);
    
    printf("\n-> Lecture dans un buffer : %ld octets lus.\n", octetsLus);
    printf("Contenu : %.*s\n", (int)octetsLus, buffer); 

    printf("\n-> Destruction de l'inode...\n");
    DetruireInode(&monInode);
    
    AfficherInode(monInode);

    printf("\n--- Test terminé. Lancez valgrind pour vérifier les fuites. ---\n");
    return EXIT_SUCCESS;
}