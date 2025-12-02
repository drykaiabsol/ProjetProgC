/**
 * ProgC - Projet Automne 25-26 : Gestion de systèmes de fichiers
 * VERSION 4
 * Fichier : repertoire.c
 * Module de gestion d'un répertoire d'un systèmes de fichiers (simulé)
 **/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "repertoire.h"
#include "inode.h"
#include "bloc.h"

#define NB_BLOCS_DIRECTS 10

// Définition d'un répertoire
struct sRepertoire
{
  tEntreesRepertoire *table;
};

/* V4
 * Crée un nouveau répertoire.
 * Entrée : aucune
 * Sortie : le répertoire créé, ou NULL si problème
 */
tRepertoire CreerRepertoire(void)
{
  // A COMPLETER
  tRepertoire rep = (tRepertoire) malloc(sizeof(struct sRepertoire));
    if (rep == NULL) return NULL;

    long tailleMax = NB_BLOCS_DIRECTS * TAILLE_BLOC;
    int nbMax = tailleMax / sizeof(struct sEntreesRepertoire);

    rep->table = (tEntreesRepertoire *) calloc(nbMax, sizeof(tEntreesRepertoire));
    
    if (rep->table == NULL) {
        free(rep);
        return NULL;
    }

    return rep;
}

/* V4
 * Détruit un répertoire et libère la mémoire associée.
 * Entrée : le répertoire à détruire
 * Sortie : aucune
 */
void DetruireRepertoire(tRepertoire *pRep)
{
  // A COMPLETER
  if (pRep != NULL && *pRep != NULL) {
        
        long tailleMax = NB_BLOCS_DIRECTS * TAILLE_BLOC;
        int nbMax = tailleMax / sizeof(struct sEntreesRepertoire);

        if ((*pRep)->table != NULL) {
            int i;
            for (i = 0; i < nbMax; i++) {
                if ((*pRep)->table[i] != NULL) {
                    free((*pRep)->table[i]);
                }
            }
            free((*pRep)->table);
        }
        free(*pRep);
        *pRep = NULL;
    }
}

/* V4
 * Écrit une entrée dans un répertoire.
 * Si l'entrée existe déjà dans le répertoire, le numéro d'inode associé est mis à jour.
 * Si l'entrée n'existe pas, elle est ajoutée dans le répertoire.
 * Entrées : le répertoire destination, le nom de l'entrée à écrire,
 *           le numéro d'inode associé à l'entrée
 * Retour : 0 si l'entrée est écrite avec succès, -1 en cas d'erreur
 */
int EcrireEntreeRepertoire(tRepertoire rep, char nomEntree[], unsigned int numeroInode)
{
  // A COMPLETER
  if (rep == NULL) return -1;

    long tailleMax = NB_BLOCS_DIRECTS * TAILLE_BLOC;
    int nbMax = tailleMax / sizeof(struct sEntreesRepertoire);

    int i = 0;
    int indexLibre = -1;
    int trouve = 0;

    while (i < nbMax && trouve == 0) {
        if (rep->table[i] != NULL) {
            if (strcmp(rep->table[i]->nomEntree, nomEntree) == 0) {
                rep->table[i]->numeroInode = numeroInode;
                trouve = 1;
            }
        } 
        else {
            if (indexLibre == -1) indexLibre = i;
        }
        i++;
    }

    if (trouve == 1) return 0;

    if (indexLibre != -1) {
        rep->table[indexLibre] = (tEntreesRepertoire) malloc(sizeof(struct sEntreesRepertoire));
        if (rep->table[indexLibre] == NULL) return -1;

        strncpy(rep->table[indexLibre]->nomEntree, nomEntree, TAILLE_NOM_FICHIER);
        rep->table[indexLibre]->nomEntree[TAILLE_NOM_FICHIER] = '\0';
        rep->table[indexLibre]->numeroInode = numeroInode;
        return 0;
    }

    return -1;
}

/* V4
 * Lit le contenu d'un répertoire depuis un inode.
 * Entrées : le répertoire mis à jour avec le contenu lu,
 *           l'inode source.
 * Retour : 0 si le répertoire est lu avec succès, -1 en cas d'erreur
 */
int LireRepertoireDepuisInode(tRepertoire *pRep, tInode inode)
{
  // A COMPLETER
  if (pRep == NULL || inode == NULL) return -1;

    *pRep = CreerRepertoire();
    if (*pRep == NULL) return -1;

    long tailleMax = NB_BLOCS_DIRECTS * TAILLE_BLOC;
    int nbMax = tailleMax / sizeof(struct sEntreesRepertoire);

    struct sEntreesRepertoire buffer[nbMax]; 
    
    long lus = LireDonneesInode(inode, (unsigned char *)buffer, tailleMax, 0);
    if (lus < 0) return -1;

    int i;
    for (i = 0; i < nbMax; i++) {
        if (buffer[i].numeroInode != 0) {
            (*pRep)->table[i] = (tEntreesRepertoire) malloc(sizeof(struct sEntreesRepertoire));
            *(*pRep)->table[i] = buffer[i];
        } else {
            (*pRep)->table[i] = NULL;
        }
    }
    return 0;
}

/* V4
 * Écrit un répertoire dans un inode.
 * Entrées : le répertoire source et l'inode destination
 * Sortie : 0 si le répertoire est écrit avec succès, -1 en cas d'erreur
 */
int EcrireRepertoireDansInode(tRepertoire rep, tInode inode)
{
  // A COMPLETER
  if (rep == NULL || inode == NULL) return -1;

    long tailleMax = NB_BLOCS_DIRECTS * TAILLE_BLOC;
    int nbMax = tailleMax / sizeof(struct sEntreesRepertoire);

    struct sEntreesRepertoire *buffer = calloc(nbMax, sizeof(struct sEntreesRepertoire));
    if (buffer == NULL) return -1;

    int i;
    for (i = 0; i < nbMax; i++) {
        if (rep->table[i] != NULL) {
            buffer[i] = *(rep->table[i]);
        }
    }

    long ecrits = EcrireDonneesInode(inode, (unsigned char *)buffer, tailleMax, 0);
    
    free(buffer);

    if (ecrits < 0) return -1;
    return 0;
}

/* V4
 * Récupère les entrées contenues dans un répertoire.
 * Entrées : le répertoire source, un tableau récupérant les numéros d'inodes des entrées du rpertoire
 * Retour : le nombre d'entrées dans le répertoire
 */
int EntreesContenuesDansRepertoire(tRepertoire rep, struct sEntreesRepertoire tabNumInodes[])
{
  // A COMPLETER
  if (rep == NULL || tabNumInodes == NULL) return 0;
    
    long tailleMax = NB_BLOCS_DIRECTS * TAILLE_BLOC;
    int nbMax = tailleMax / sizeof(struct sEntreesRepertoire);

    int k = 0, i = 0;
    while (i < nbMax) {
        if (rep->table[i] != NULL) {
            tabNumInodes[k] = *(rep->table[i]);
            k++;
        }
        i++;
    }
    return k;
}

/* V4
 * Compte le nombre d'entrées d'un répertoire.
 * Entrée : le répertoire source
 * Retour : le nombre d'entrées du répertoire
 */
int NbEntreesRepertoire(tRepertoire rep)
{
  // A COMPLETER
  if (rep == NULL) return 0;
    
    long tailleMax = NB_BLOCS_DIRECTS * TAILLE_BLOC;
    int nbMax = tailleMax / sizeof(struct sEntreesRepertoire);
    
    int k = 0, i = 0;
    while (i < nbMax) {
        if (rep->table[i] != NULL) k++;
        i++;
    }
    return k;
}

/* V4
 * Affiche le contenu d'un répertoire.
 * Entrée : le répertoire à afficher
 * Retour : aucun
 */
void AfficherRepertoire(tRepertoire rep)
{
  // A COMPLETER
  if (rep == NULL) {
        return;
    }

    long nbMaxEntrees = TailleMaxFichier() / sizeof(struct sEntreesRepertoire);
    
    for (int i = 0; i < nbMaxEntrees; i++) {
        if (rep->table[i] != NULL) {
            printf("%s : %u\n", rep->table[i]->nomEntree, rep->table[i]->numeroInode);
        }
    }
}
