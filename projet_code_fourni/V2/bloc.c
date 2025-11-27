/**
 * ProgC - Projet Automne 25-26 : Gestion de systèmes de fichiers
 * VERSION 1
 * Fichier : bloc.c
 * Module de gestion des blocs de données.
 **/

#include "bloc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* V1
 * Crée et retourne un nouveau bloc de données.
 * Entrée : Aucune
 * Retour : le bloc créé ou NULL en cas de problème
 */
tBloc CreerBloc (void) {
  // A COMPLETER
  tBloc nouveauBloc;

  nouveauBloc = (tBloc) malloc(TAILLE_BLOC);

  if (nouveauBloc == NULL)
  {
    fprintf(stderr, "CreerBloc: probleme creation ");
    return NULL;
  }
  return nouveauBloc;
}

/* V1
 * Détruit un bloc de données.
 * Entrée : le bloc à détruire (libération mémoire allouée)
 * Retour : aucun
 */
void DetruireBloc(tBloc *pBloc) {
  // A COMPLETER
  if (pBloc != NULL && *pBloc != NULL)
  {
    free(*pBloc);

    *pBloc = NULL;
  }
}

/* V1
 * Copie dans un bloc les taille octets situés à l’adresse contenu.
 * Si taille est supérieure à la taille d’un bloc, seuls les TAILLE_BLOC premiers octets doivent être copiés.
 * Entrées : le bloc, l'adresse du contenu à copier et sa taille en octets
 * Retour : le nombre d'octets effectivement écrits dans le bloc
 */
long EcrireContenuBloc (tBloc bloc, unsigned char *contenu, long taille) {
  // A COMPLETER
  long tailleACopier;

  if (taille > TAILLE_BLOC)
  {
    tailleACopier = TAILLE_BLOC;
  }else{
    tailleACopier = taille;
  }

  memcpy(bloc, contenu, tailleACopier);

  return tailleACopier;
}

/* V1
 * Copie à l'adresse contenu, les taille octets stockés dans un bloc.
 * Si taille est supérieure à la taille d’un bloc, seuls les TAILLE_BLOC premiers octets doivent être copiés.
 * Entrées : le bloc, l'adresse contenu à laquelle recopier et la taille en octets du bloc
 * Retour : le nombre d'octets effectivement lus dans le bloc
 */
long LireContenuBloc(tBloc bloc, unsigned char *contenu, long taille) {
  // A COMPLETER
  long tailleALire;

  if (taille > TAILLE_BLOC)
  {
    tailleALire = TAILLE_BLOC;
  }else{
    tailleALire = taille;
  }

  memcpy(contenu, bloc, tailleALire);

return tailleALire;
}
