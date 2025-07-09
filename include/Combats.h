#ifndef COMBATS_H
#define COMBATS_H


// caractères spéciaux pour lecture combat
#define PATHOS 'P'
#define LOGOS 'L'
#define STATS 'S'
#define CREDIBILITE 100
#define ID_END -1
#define FINCOMBAT '$'


// renvoie 0 si le combat est perdu, 1 si le combat est gagné
int LireCombat(char *combatfile, int id_combat, char * inventaire);

#endif