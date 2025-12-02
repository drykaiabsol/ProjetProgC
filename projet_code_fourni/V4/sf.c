/**
 * ProgC - Projet Automne 25-26 : Gestion de systèmes de fichiers
 * VERSION 4
 * Fichier : sf.c
 * Module de gestion d'un systèmes de fichiers (simulé)
 **/

 #include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include "inode.h"    
#include "sf.h"
#include "bloc.h"
#include "repertoire.h"
#define MAX_FICHIER_OCTETS (10 * TAILLE_BLOC)

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

// Définirion d'un système de fichiers (simplifié)
struct sSF
{
  // Le super-bloc
  tSuperBloc superBloc;
  // La liste chaînée des inodes
  struct sListeInodes listeInodes;
};

/* V3
*  Crée et retourne un super-bloc.
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

/* V3
*  Détruit un super-bloc.
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

/* V3
*  Affiche le contenu d'un super-bloc.
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

/* V2 & V4
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

/* V3 & V4
 * Ecrit un fichier (d'un nombre de blocs quelconque) dans le système de fichiers.
 * Si la taille du fichier à écrire dépasse la taille maximale d'un fichier dans le SF(10 x 64 octets),
 * seuls les 640 premiers octets seront écrits dans le système de fichiers.
 * Entrées : le système de fichiers, le nom du fichier (sur disque) et son type dans le SF (simulé)
 * Sortie : le nombre d'octets effectivement écrits, -1 en cas d'erreur.
 */
long EcrireFichierSF(tSF sf, char nomFichier[], natureFichier type) {
  // A COMPLETER
  if (sf == NULL || nomFichier == NULL) return -1;

    FILE *f = fopen(nomFichier, "rb");
    if (f == NULL) return -1;

    long tailleMax = MAX_FICHIER_OCTETS;
    unsigned char buffer[640];

    long nbLus = fread(buffer, 1, tailleMax, f);
    fclose(f);

    if (nbLus < 0) nbLus = 0;

    int numInode = sf->listeInodes.nbInodes;
    tInode inode = CreerInode(numInode, type);
    if (inode == NULL) return -1;

    long nbEcrits = EcrireDonneesInode(inode, buffer, nbLus, 0);
    
    if (nbEcrits < 0) {
        DetruireInode(&inode);
        return -1;
    }

    struct sListeInodesElement *elem = malloc(sizeof(struct sListeInodesElement));
    if (elem == NULL) {
        DetruireInode(&inode);
        return -1;
    }

    elem->inode = inode;
    elem->suivant = NULL;

    if (sf->listeInodes.premier == NULL) {
        sf->listeInodes.premier = elem;
        sf->listeInodes.dernier = elem;
    } else {
        sf->listeInodes.dernier->suivant = elem;
        sf->listeInodes.dernier = elem;
    }

    sf->listeInodes.nbInodes++;
    sf->superBloc->dateDerModif = time(NULL);

    return nbEcrits;
}

/* V3
 * Sauvegarde un système de fichiers dans un fichier (sur disque).
 * Entrées : le système de fichiers, le nom du fichier sauvegarde (sur disque)
 * Sortie : 0 en cas de succèe, -1 en cas d'erreur
 */
int SauvegarderSF(tSF sf, char nomFichier[]) {
  // A COMPLETER
  if (sf == NULL) return -1;

    FILE *fichier = fopen(nomFichier, "wb");
    if (fichier == NULL) return -1;

    int erreur = 0;
    if (fwrite(sf->superBloc, sizeof(struct sSuperBloc), 1, fichier) != 1) {
        erreur = 1;
    }

    if (erreur == 0) {
        if (fwrite(&(sf->listeInodes.nbInodes), sizeof(int), 1, fichier) != 1) {
            erreur = 1;
        }
    }

    struct sListeInodesElement *courant = sf->listeInodes.premier;

    while (courant != NULL && erreur == 0) {
        if (SauvegarderInode(courant->inode, fichier) == -1) {
            erreur = 1;
        } else {
            courant = courant->suivant;
        }
    }

    fclose(fichier);

    if (erreur == 1) {
        return -1;
    }
    return 0;
}

/* V3
 * Restaure le contenu d'un système de fichiers depuis un fichier sauvegarde (sur disque).
 * Entrées : le système de fichiers où restaurer, le nom du fichier sauvegarde (sur disque)
 * Sortie : 0 en cas de succèe, -1 en cas d'erreur
 */
int ChargerSF(tSF *pSF, char nomFichier[]) {
  // A COMPLETER
  FILE *fichier = fopen(nomFichier, "rb");
    if (fichier == NULL) return -1;

    *pSF = CreerSF("temp");
    if (*pSF == NULL) {
        fclose(fichier);
        return -1;
    }

    int erreur = 0;

    if (fread((*pSF)->superBloc, sizeof(struct sSuperBloc), 1, fichier) != 1) {
        erreur = 1;
    }

    int nbInodesALire = 0;
    if (erreur == 0) {
        if (fread(&nbInodesALire, sizeof(int), 1, fichier) != 1) {
            erreur = 1;
        }
    }

    int i = 0;
    while (i < nbInodesALire && erreur == 0) {
        
        struct sListeInodesElement *nouvelElement = (struct sListeInodesElement*) malloc(sizeof(struct sListeInodesElement));
        if (nouvelElement == NULL) {
            erreur = 1;
        } 
        else {
            if (ChargerInode(&(nouvelElement->inode), fichier) == -1) {
                free(nouvelElement);
                erreur = 1;
            } 
            else {
                nouvelElement->suivant = NULL;
                
                if ((*pSF)->listeInodes.premier == NULL) {
                    (*pSF)->listeInodes.premier = nouvelElement;
                    (*pSF)->listeInodes.dernier = nouvelElement;
                } else {
                    (*pSF)->listeInodes.dernier->suivant = nouvelElement;
                    (*pSF)->listeInodes.dernier = nouvelElement;
                }
                
                (*pSF)->listeInodes.nbInodes++;
                
                i++;
            }
        }
    }

    fclose(fichier);

    if (erreur == 1) {
        DetruireSF(pSF);
        return -1;
    }

    return 0;
}

/* V4
 * Affiche le détail du répertoire racine d'un système de fichiers.
 * Entrées : le système de fichiers,
 * et un booléen indiquant si l'affichage doit être détaillé (true) ou non (false),
 * voir l'énoncé pour les consignes d'affichage à suivre
 * Sortie : 0 en cas de succès, -1 en cas d'erreur
 */
int Ls(tSF sf, bool detail)  {
  // A COMPLETER
  if (sf == NULL) {
        return 1; 
    }

    tInode racineInode = sf->listeInodes.premier->inode; 
    tRepertoire rep;
    
    if (LireRepertoireDepuisInode(&rep, racineInode) == -1) {
        return 1;
    }

    int nbFichiers = NbEntreesRepertoire(rep);
    printf("Nombre de fichiers dans le répertoire racine : %d\n", nbFichiers);
    long maxEntrees = TailleMaxFichier() / sizeof(struct sEntreesRepertoire);
    struct sEntreesRepertoire *tab = malloc(maxEntrees * sizeof(struct sEntreesRepertoire));
    EntreesContenuesDansRepertoire(rep, tab);

    for (int i = 0; i < nbFichiers; i++) {
        if (detail) {
            tInode inodeCible = NULL;
            struct sListeInodesElement *courant = sf->listeInodes.premier;
            while(courant != NULL) {
                if (Numero(courant->inode) == tab[i].numeroInode) {
                    inodeCible = courant->inode;
                    break;
                }
                courant = courant->suivant;
            }

            if (inodeCible) {
                char *typeStr = "INCONNU";
                if (Type(inodeCible) == ORDINAIRE) {
                    typeStr = "ORDINAIRE";
                } else if (Type(inodeCible) == REPERTOIRE) {
                    typeStr = "REPERTOIRE";
                }
                
                char *date = ctime((const time_t []){DateDerModif(inodeCible)});
                date[strlen(date)-1] = '\0';

                printf("%-3u %-12s %6ld %s %s\n", 
                    Numero(inodeCible), 
                    typeStr, 
                    Taille(inodeCible), 
                    date, 
                    tab[i].nomEntree);
            }
        } else {
            printf("%s\n", tab[i].nomEntree);
        }
    }

    free(tab);
    DetruireRepertoire(&rep);
    return 0;
}
