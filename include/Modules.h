#ifndef MODULES_H
#define MODULES_H

#define SCRIPT "./files/script.txt"
#define IMAGES "./files/images.txt"
#define MODULES "./files/modules.txt"
#define SAVE "./files/save.txt"
#define OBJETS "./files/objets.txt"
#define COMBATS "./files/combats.txt"

// caractères spéciaux pour lecture module
#define LIRESCENE 'S'   // une image, puis un texte, finit par --> | format : S i%id_image p%id_passage
#define LIRESCENE2 'Z'  // type "dialogue jeu vidéo", finit par --> | format : Z i%id_image p%id_passage
#define LIREIMAGE 'I'   // lit une image | format : I %id_image
#define LIREPASSAGE 'P' // lit du texte uniquement depuis le fichier de texte | format : P %id_passage
#define CHOIX1 'C'      // ce choix fait lire une variation ou une autre d'une même scène selon le choix
#define CHOIX2 'D'      // ce choix amène à un module différent selon le choix
#define CHOIX3 'Y'      // comme choix 2, mais avec 3 options à la place de 2
#define CHOIX3INSTANT 'X' // comme choix 3 mais affiche l'image instantanément
#define CHOIX4INSTANT 'W' // comme choix 4 mais affiche l'image instantanément
#define CHOIX4 'A' // a choice menu with 4 options
#define EVENT 'E'       // change le statut de l'évènement en '1'. cela permet de savoir si quelque chose s'est produit.
#define OBJET 'O'       // change le statut de l'objet en '0' ou '1' cela permet de retirer ou donner un objet au joueur.
#define CHECKEVENT2 'Q' // verifie si un évenement s'est produit. si oui, envoie vers un module donné
#define CHECKEVENT 'U'  // lit un passage ou un autre selon si un évènement se vérifie
#define CHECKINVENT 'T' // vérifie si un objet est dans l'inventaire. si oui, envoie vers un module donné, sinon renvoie vers un autre.
#define CHECKINVENT2 'H'// vérifie si un objet est dans l'inventaire. si oui, envoie vers un module donné.
#define INVENTAIRE 'K'  // renvoie vers un menu permettant de consulter l'inventaire.
#define RETOUR 'J'      // revient au module précédent. cette fonction n'existe que pour pouvoir consulter l'inventaire lol
#define SAVING 'V'      // sauvegarde la progression
#define GOTOMODULE 'M'  // rembobine le fichier, cherche le module indiqué et commence la lecture | format : M %id_module
#define FINMODULE '$'   // ce symbole ne devrait pas être lu : on passe à un autre module avant. si il est atteint, faire apparaître un message d'erreur, puis quitter le programme
#define RANDOMSCENE 'R' // joue une scène ou une autre selon un tirage aléatoire
#define RANDOMMODULE 'L' // va vers le module 1 si le tirage aléatoire est réussi, vers le module 2 sinon
#define FIGHT 'F'       // démarre le combat de numéro donné | format : F 30
#define COMMENTAIRE '#' // le lecteur de module ignorera la ligne à partir de ce caractère
#define EVENTANDEVENT 'N' // vérifie la condition event1 ET event2, et envoie vers un module si la condition est remplie
#define ID_END -1 // ce marqueur indique si tous les modules ont été parcourus. pratique si on veut garder des modules invisibles en fin de fichier .??? peut être remplacé par EOF

void lireModule(int id_module, char *modules_file, char *images_file, char *text_file, char *events, char *inventaire);

// si cette fonction est utilisée avec l'inventaire comme paramètre, alors elle vérifie la présence d'un objet dans l'inventaire. renvoie 1 si l'évènement vaut 1.
int CheckForEvent(int id_event, char *events);
// si cette fonction est utilisée avec "inventaire" à la place de "events" alors cela revient à mettre quelque chose dans l'inventaire (ou retirer, selon la valeur (1 = présent, 0 = absent)
void EventTrigger(int id_event, char *events, char value);


#endif