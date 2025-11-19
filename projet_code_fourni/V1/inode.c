/**
 * ProgC - Projet Automne 25-26 : Gestion de systèmes de fichiers
 * VERSION 1
 * Fichier : inode.c
 * Module de gestion des inodes.
 **/
#include "inode.h"
#include "bloc.h"

// Nombre maximal de blocs dans un inode
#define NB_BLOCS_DIRECTS 10

// Définition d'un inode
struct sInode
{
  // Numéro de l'inode
  unsigned int numero;
  // Le type du fichier : ordinaire, répertoire ou autre
  natureFichier type;
  // La taille en octets du fichier
  long taille;
  // Les adresses directes vers les blocs (NB_BLOCS_DIRECTS au maximum)
  tBloc blocDonnees[NB_BLOCS_DIRECTS];
  // Les dates : dernier accès à l'inode, dernière modification du fichier
  // et de l'inode
  time_t dateDerAcces, dateDerModif, dateDerModifInode;
};

/* V1
 * Crée et retourne un inode.
 * Entrées : numéro de l'inode et le type de fichier qui y est associé
 * Retour : l'inode créé ou NULL en cas de problème
 */
tInode CreerInode(int numInode, natureFichier type) {
  // A COMPLETER
  tInode nouveauInode;

  nouveauInode = (tInode) malloc(sizeof(struct sInode));

  if (nouveauInode == NULL){
    fprintf(stderr, "CreerInode: probleme creation ");
    return NULL;
  }
  time_t dateActuelle = time(NULL);
  nouveauInode->numero = numInode;
  nouveauInode->type = type;
  nouveauInode->taille = 0;

  nouveauInode->dateDerAcces = dateActuelle;
  nouveauInode->dateDerModif = dateActuelle;
  nouveauInode->dateDerModifInode = dateActuelle;

  int i;
  for (i = 0; i < NB_BLOCS_DIRECTS; i++){
    nouveauInode->blocDonnees[i] = NULL;
  }

  return nouveauInode;
}

/* V1
 * Détruit un inode.
 * Entrée : l'inode à détruire
 * Retour : aucun
 */
void DetruireInode(tInode *pInode) {
  // A COMPLETER
  if (pInode != NULL &&  *pInode != NULL){
    int i;
    for (i = 0; i < NB_BLOCS_DIRECTS; i++){
      if ((*pInode)->blocDonnees[i] != NULL){
        DetruireBloc(&((*pInode)->blocDonnees[i]));
      }
    }
    free(*pInode);
    *pInode = NULL;
  }
}

/* V1
 * Récupère la date de dernier accès à un inode.
 * Entrée : l'inode pour lequel on souhaite connaître la date de dernier accès
 * Retour : la date de dernier accès à l'inode
 */
time_t DateDerAcces(tInode inode) {
  // A COMPLETER
  if (inode == NULL){
    return (time_t) 0;
  }
  return inode->dateDerAcces;
}

/* V1
 * Récupère la date de dernière modification d'un inode.
 * Entrée : l'inode pour lequel on souhaite connaître la date de dernière modification
 * Retour : la date de dernière modification de l'inode
 */
time_t DateDerModif(tInode inode) {
  // A COMPLETER
  if (inode == NULL){
    return (time_t) 0;
  }
  return inode->dateDerModifInode;
}

/* V1
 * Récupère la date de dernière modification d'u fichier associé à un inode.
 * Entrée : l'inode pour lequel on souhaite connaître la date de dernière modification du fichier associé
 * Retour : la date de dernière modification du fichier associé à l'inode
 */
time_t DateDerModifFichier(tInode inode) {
  // A COMPLETER
  if (inode == NULL){ 
    return (time_t) 0;
    }
  return inode->dateDerModif;
}

/* V1
 * Récupère le numéro d'un inode.
 * Entrée : l'inode pour lequel on souhaite connaître le numéro
 * Retour : le numéro de l'inode
 */
unsigned int Numero(tInode inode) {
  // A COMPLETER
  if (inode == NULL) return 0;
  return inode->numero;
}

/* V1
 * Récupère la taille en octets du fichier associé à un inode.
 * Entrée : l'inode pour lequel on souhaite connaître la taille
 * Retour : la taille en octets du fichier associé à l'inode
 */
long Taille(tInode inode) {
  // A COMPLETER
  if (inode == NULL) return 0;
  return inode->taille;
}

/* V1
 * Récupère le type du fichier associé à un inode.
 * Entrée : l'inode pour lequel on souhaite connaître le tyep de fichier associé
 * Retour : le type du fichier associé à l'inode
 */
natureFichier Type(tInode inode) {
  // A COMPLETER
  if (inode == NULL) return AUTRE;
  return inode->type;
}

/* V1
 * Affiche les informations d'un inode
 * Entrée : l'inode dont on souhaite afficher les informations
 * Retour : aucun
 */
void AfficherInode(tInode inode) {
  // A COMPLETER
  if (inode == NULL){
    printf("<<< vide >>>\n");
    return;
  }
  char *typeString;
  switch (inode->type){
    case ORDINAIRE: 
      typeString = "Ordinaire";
      break;
    case REPERTOIRE: 
      typeString = "Répertoire";
      break;
    default: typeString = "Autre";
      break;
  }
  char *dateStr;
  printf("-------Inode------[%u] :\n", inode->numero);
  printf("type : %s\n", typeString);
  printf("taille : %ld octets\n", inode->taille);
  dateStr = ctime(&(inode->dateDerAcces));
  dateStr[strlen(dateStr)-1] = '\0';
  printf("date dernier accès : %s\n", dateStr);

  dateStr = ctime(&(inode->dateDerModif));
  dateStr[strlen(dateStr)-1] = '\0';
  printf("date dernière modification : %s\n", dateStr);

  dateStr = ctime(&(inode->dateDerModifInode));
  dateStr[strlen(dateStr)-1] = '\0';
  printf("date dernière modification inode : %s\n", dateStr);

  printf("Données :\n");
  if (inode->blocDonnees[0] != NULL && inode->taille > 0){
    long i;
    for (i = 0; i < inode->taille; i++){
      printf("%c", inode->blocDonnees[0][i]);
    }
    printf("\n");

    printf("Nombre de d'octets lus: %ld\n", inode->taille);
  }
}

/* V1
 * Copie à l'adresse contenu les taille octets stockés dans un inode.
 * Si taille est supérieure à la taille d’un bloc, seuls les TAILLE_BLOC premiers octets doivent être copiés.
 * Entrées : l'inode, l'adresse de la zone où recopier et la taille en octets de l'inode
 * Retour : le nombre d'octets effectivement lus dans l'inode ou -1 en cas d'erreur
 */
long LireDonneesInode1bloc(tInode inode, unsigned char *contenu, long taille) {
  // A COMPLETER
  long nbOctetsLire;
  time_t now;

  if (inode == NULL){
    return -1;
  }

  if (taille > TAILLE_BLOC){
    nbOctetsLire = TAILLE_BLOC;
  }else{
    nbOctetsLire = taille;
  }

  if (nbOctetsLire > inode->taille){
    nbOctetsLire = inode->taille;
  }

  if (nbOctetsLire == 0){
    return 0;
  }

  LireContenuBloc(inode->blocDonnees[0], contenu, nbOctetsLire);

  now = time(NULL);

  inode->dateDerAcces = now;
  
  return nbOctetsLire;
}

/* V1
 * Copie dans un inode les taille octets situés à l’adresse contenu.
 * Si taille est supérieure à la taille d’un bloc, seuls les TAILLE_BLOC premiers octets doivent être copiés.
 * Entrées : l'inode, l'adresse de la zone à recopier et sa taille en octets
 * Retour : le nombre d'octets effectivement écrits dans l'inode ou -1 en cas d'erreur
 */
long EcrireDonneesInode1bloc(tInode inode, unsigned char *contenu, long taille) {
  // A COMPLETER
  long nbOctetsEcrits;
  time_t now;

  if (inode == NULL){
    return -1;
  }

  if (taille > TAILLE_BLOC){
    nbOctetsEcrits = TAILLE_BLOC;
  }else{
    nbOctetsEcrits = taille;
  }

  if (nbOctetsEcrits == 0){
    return 0;
  }

  if (inode->blocDonnees[0] == NULL){
    inode->blocDonnees[0] = CreerBloc();

    if (inode->blocDonnees[0] == NULL){
      return -1;
    }
  }

  EcrireContenuBloc(inode->blocDonnees[0], contenu, nbOctetsEcrits);

  now = time(NULL);

  inode->taille = nbOctetsEcrits;

  inode->dateDerAcces = now;
  inode->dateDerModif = now;
  inode->dateDerModifInode = now;

  return nbOctetsEcrits;
}
