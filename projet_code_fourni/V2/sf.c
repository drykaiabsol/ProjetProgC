/**
 * ProgC - Projet Automne 25-26 : Gestion de systèmes de fichiers
 * VERSION 2
 * Fichier : sf.c
 * Module de gestion d'un systèmes de fichiers (simulé)
 **/

#include "sf.h"
#include "bloc.h"

// Taille maximale du nom du SF (ou nom du disque)
#define TAILLE_NOM_DISQUE 24

// Définition du super-bloc
struct sSuperBloc
{
  // Le nom du disque (ou du système de fichiers) (avec le '\0')
  char nomDisque[TAILLE_NOM_DISQUE+1];
  // La date de dernière modification du système de fichiers
  time_t dateDerModif;
};

// Type représentant le super-bloc
typedef struct sSuperBloc *tSuperBloc;

// Définition de la liste chaînée des inodes
struct sListeInodes
{
  // Références vers le premier et le dernier élément de la liste chaînée
  struct sListeInodesElement *premier, *dernier;
  // Nombre d'inodes dans la liste
  int nbInodes;
};

// Définition d'un élement de la liste chaînée des inodes dans le SF
struct sListeInodesElement
{
  // L'inode proprement dit
  tInode inode;
  // L'élément suivant dans la liste chaînée
  struct sListeInodesElement *suivant;
};

// Définition d'un système de fichiers (simplifié)
struct sSF
{
  // Le super-bloc
  tSuperBloc superBloc;
  // La liste chaînée des inodes
  struct sListeInodes listeInodes;
};

/* V2
*  Crée et retourne un super-bloc.
* Fonction non publique (static)
* Entrée : le nom du disque (ou du SF)
* Sortie : le super-bloc, ou NULL en cas de problème
*/
static tSuperBloc CreerSuperBloc(char nomDisque[]) {
  // A COMPLETER
  tSuperBloc nouveauSB = (tSuperBloc) malloc(sizeof(struct sSuperBloc));

  if (nouveauSB == NULL)
  {
    fprintf(stderr, "CreerSuperBloc probleme creation\n");
    return NULL;
  }

  strncpy(nouveauSB->nomDisque, nomDisque, TAILLE_NOM_DISQUE);
  nouveauSB->nomDisque[TAILLE_NOM_DISQUE] = '\0';
  nouveauSB->dateDerModif = time(NULL);

  return nouveauSB;
}

/* V2
*  Détruit un super-bloc.
* Fonction non publique (static)
* Entrée : le super-bloc à détruire
* Sortie : aucune
*/
static void DetruireSuperBloc(tSuperBloc *pSuperBloc) {
  // A COMPLETER
  if (pSuperBloc != NULL && *pSuperBloc != NULL)
  {
    free(*pSuperBloc);

    *pSuperBloc = NULL;
  }
}

/* V2
*  Affiche le contenu d'un super-bloc.
* Fonction non publique (static)
* Entrée : le super-bloc à afficher
* Sortie : aucune
*/
static void AfficherSuperBloc(tSuperBloc superBloc) {
  // A COMPLETER
    if (superBloc == NULL)
  {
    printf("SuperBloc : NULL\n");
    return ;
  }
  
  char *dateStr;

  dateStr = ctime(&(superBloc->dateDerModif));
  dateStr[strlen(dateStr)-1]='\0';

  printf("Sf de nom %s, super bloc :\n", superBloc->nomDisque);
  printf("taille bloc = %d, date der modif = %s\n", TAILLE_BLOC, dateStr);
}

/* V2
 * Crée un nouveau système de fichiers.
 * Entrée : nom du disque à associer au système de fichiers créé
 * Retour : le système de fichiers créé, ou NULL en cas d'erreur
 */
tSF CreerSF (char nomDisque[]){
  // A COMPLETER
  tSF nouveauSF = (tSF) malloc(sizeof(struct sSF));

  if (nouveauSF == NULL)
  {
    fprintf(stderr, "CreerSF : probleme creation\n");
    return NULL;
  }

  nouveauSF->superBloc = CreerSuperBloc(nomDisque);

  if (nouveauSF->superBloc == NULL)
  {
    free(nouveauSF);
    return NULL;
  }

  nouveauSF->listeInodes.premier = NULL;
  nouveauSF->listeInodes.dernier = NULL;
  nouveauSF->listeInodes.nbInodes = 0;

  return nouveauSF;
}

/* V2
 * Détruit un système de fichiers et libère la mémoire associée.
 * Entrée : le SF à détruire
 * Sortie : aucune
 */
void DetruireSF(tSF *pSF) {
  // A COMPLETER
  if (pSF == NULL || *pSF == NULL)
  {
    return;
  }

  tSF sfADetruire = *pSF;

  DetruireSuperBloc(&(sfADetruire->superBloc));

  struct sListeInodesElement *courant = sfADetruire->listeInodes.premier;
  struct sListeInodesElement *suivant;

  while (courant != NULL)
  {
    suivant = courant->suivant;
    DetruireInode(&(courant->inode));
    free(courant);
    courant = suivant;
  }
  free(sfADetruire);
  *pSF = NULL;
}

/* V2
 * Affiche les informations relative à un système de fichiers i.e;
 * le contenu du super-bloc et celui des différents inodes du SF
 * Entrée : le SF à afficher
 * Sortie : aucune
 */
void AfficherSF (tSF sf){
  // A COMPLETER
  if (sf == NULL)
  {
    printf("Système de fichiers non initialisé (NULL).\n");
    return;
  }

  AfficherSuperBloc(sf->superBloc);
  printf("Inodes : \n");

  struct sListeInodesElement *courant = sf->listeInodes.premier;

  while (courant != NULL)
  {
    AfficherInode(courant->inode);
    courant = courant->suivant;
  }
}

/* V2
 * Ecrit un fichier d'un seul bloc dans le système de fichiers.
 * Entrées : le système de fichiers, le nom du fichier (sur disque) et son type dans le SF (simulé)
 * Sortie : le nombre d'octets effectivement écrits, -1 en cas d'erreur.
 */
long Ecrire1BlocFichierSF(tSF sf, char nomFichier[], natureFichier type) {
  // A COMPLETER
  if (sf == NULL) return -1;

    FILE *fichierSource = fopen(nomFichier, "rb");
    if (fichierSource == NULL) {
        return -1;
    }

    unsigned char buffer[TAILLE_BLOC];
    long octetsLus = fread(buffer, 1, TAILLE_BLOC, fichierSource);
    
    fclose(fichierSource);

    unsigned int nouveauNumero = sf->listeInodes.nbInodes;
    tInode nouvelInode = CreerInode(nouveauNumero, type);
    
    if (nouvelInode == NULL) return -1;

    long octetsEcrits = EcrireDonneesInode1bloc(nouvelInode, buffer, octetsLus);
    
    if (octetsEcrits == -1) {
        DetruireInode(&nouvelInode);
        return -1;
    }

    struct sListeInodesElement *nouvelElement = (struct sListeInodesElement*) malloc(sizeof(struct sListeInodesElement));
    
    if (nouvelElement == NULL) {
        DetruireInode(&nouvelInode);
        return -1;
    }

    nouvelElement->inode = nouvelInode;
    nouvelElement->suivant = NULL;

    if (sf->listeInodes.premier == NULL) {
        sf->listeInodes.premier = nouvelElement;
        sf->listeInodes.dernier = nouvelElement;
    } else {
        sf->listeInodes.dernier->suivant = nouvelElement;
        sf->listeInodes.dernier = nouvelElement;
    }

    sf->listeInodes.nbInodes++;
    sf->superBloc->dateDerModif = time(NULL);

    return octetsEcrits;
}
