#include <Modules.h>
#include <Combats.h>
#include <Menus.h>

#include <stdio.h>
#include <stdlib.h>


// renvoie 0 si le combat est perdu, 1 si le combat est gagné
int LireCombat(char *combatfile, int id_combat, char * inventaire)
{
    //printf("start lecturecombat");
    int cred = 0;
    FILE *fc;
    fc = fopen(combatfile, "r");
    if (fc != NULL)
    {
        int b;
        int id;
        int reponse = -1;
        // variables générales du combat
        int damage; // points de vie de l'ennemi, et du joueur
        float ethos ;
        damage = 0;
        // variables pour pathos
        int bonne_reponse = 0;
        char str1[50], str2[50], str3[50], str4[50]; // réponses possibles
        int p0, i0;                                   // passage et image à lire avant de proposer le choix
        int p1, p2, p3, p4;                           // passages à lire pour chaque réponse
        int i1, i2, i3, i4;                           // images à afficher avec le passage pour chaque réponse
        int d1, d2, d3, d4;                           // dégats associés à chaque réponse (pour la bonne réponse, le dégat est infligé à l'ennemi)

        //transformer l'inventaire en un tableau de valeurs.
        int size_invent = TAILLEINVENTAIRE - 1;//sizeof(inventaire)/sizeof(inventaire[0]);
        int taillerelle = 0;
        int i;
        for (i = 0; i < size_invent; i++) // compter le nombre d'objets dans l'inventaire
        {
            if(inventaire[i] != '0') taillerelle ++;
        }
        int true_inventaire[taillerelle];
        taillerelle = 0;
        for (i = 0; i < size_invent; i++) // créer un tableau qui indique l'id de chaque élément de l'inventaire.
        {
            if(inventaire[i] != '0') 
            {
                true_inventaire[taillerelle] = i;
                taillerelle ++;
            }
        }

        // aller au bon module de comabt
        fscanf(fc, ":%d:", &id);
        skipUntilChar(fc, '\n');
        while (id != id_combat && id != ID_END)
        {
            skipUntilChar(fc, FINCOMBAT);
            skipUntilChar(fc, '\n');
            fscanf(fc, ":%d:", &id);
            skipUntilChar(fc, '\n');
        }
        //printf("combat trouvé");

        fscanf(fc,"ethos %f", &ethos); // récupérer les stats du combat
        // si ethos est inférieur à 1, cela diminue les dégats. sinon, cela les augmente

        cred = CREDIBILITE;

        char c = fgetc(fc);
        while (c != FINCOMBAT && c != EOF)
        {
            if (c == PATHOS && cred > 0)
            {
                // fscanf toutes les variables
                fscanf(fc, " i%d p%d\n[%49[^]]][i%d][p%d][d%d]\n[%49[^]]][i%d][p%d][d%d]\n[%49[^]]][i%d][p%d][d%d]\n[%49[^]]][i%d][p%d][d%d]\nr%d\n", &i0, &p0, str1, &i1, &p1, &d1, str2, &i2, &p2, &d2, str3, &i3, &p3, &d3, str4, &i4, &p4, &d4, &bonne_reponse);
                system(CLEAR);
                LireImage(i0, IMAGES);
                LirePassage(p0, SCRIPT);
                reponse = -1;
                while (reponse != bonne_reponse && cred > 0) // le tour continue tant que le joueur n'a pas trouvé la bonne réponse, ou échoué suffisamment
                {
                    system(CLEAR);
                    LireImage(i0, IMAGES);
                    LirePassageInstantane(p0, SCRIPT);
                    printf("\n--------------------------------------------------------\n");
                    printf("               Credibility : %d%%", cred);
                    printf("\n--------------------------------------------------------\n");
                    printf("\n   1 : | %s |\n   2 : | %s |    \n   3 : | %s |\n   4 : | %s |    \n\nchoice : ", str1, str2, str3, str4);
                    char verif = 0;
                    while (!verif)
                    {
                        c = 0;
                        switch ((c = getchar()))
                        {
                        case '1':
                            reponse = 1;
                            verif = 1;
                            damage = d1;
                            i0 = i1;
                            p0 = p1;
                            break;
                        case '2':
                            reponse = 2;
                            verif = 1;
                            damage = d2;
                            i0 = i2;
                            p0 = p2;
                            break;
                        case '3':
                            reponse = 3;
                            verif = 1;
                            damage = d3;
                            i0 = i3;
                            p0 = p3;
                            break;
                        case '4':
                            reponse = 4;
                            verif = 1;
                            damage = d4;
                            
                            i0 = i4;
                            p0 = p4;
                            break;
                        default:
                            puts("\n");
                            system(CLEAR);
                            LireImage(i0, IMAGES);
                            LirePassageInstantane(p0, SCRIPT);
                            printf("\n--------------------------------------------------------\n");
                            printf("               Credibility : %d%%", cred);
                            printf("\n--------------------------------------------------------\n");
                            printf("\n   1 : | %s |\n   2 : | %s |    \n   3 : | %s |\n   4 : | %s |    \n\nchoice : ", str1, str2, str3, str4);
                            fflush(stdin);
                        }
                    }
                    while ((c = getchar()) != '\n' && c != EOF){}
                    if (reponse == bonne_reponse)
                    {
                        LireScene2(i0,p0,IMAGES,SCRIPT);
                    }
                    else
                    {
                        cred = cred - (int)(damage*ethos);
                        if (cred < 0)
                            cred = 0;
                        
                        // affiche le résultat
                        system(CLEAR);
                        LireImage(i0, IMAGES);
                        LirePassage(p0, SCRIPT);
                        printf("\n--------------------------------------------------------\n");
                        printf("               Credibility : %d%%", cred);
                        printf("\n--------------------------------------------------------\n");
                    }

                }
            }
            else if (c == LOGOS && cred > 0)
            {
                // scanf les variables : i0, p0, bonne_reponse, i1, p1, d1 (image/passage/degats en cas d'erreur), i2, p2, d2 (image/passage/degats en cas de reussite) et image/ passage en cas d'abandon
                fscanf(fc, " i%d p%d\nx[i%d][p%d][d%d]\no[i%d][p%d][d%d]\n[i%d][p%d]\nr%d\n", &i0,&p0, &i1,&p1,&d1,&i2,&p2,&d2,&i4,&p4,&bonne_reponse);
                system(CLEAR);
                LireImage(i0, IMAGES);
                LirePassage(p0, SCRIPT);
                reponse = -1;
                i = 0;
                
                while (reponse != bonne_reponse && cred > 0) // le tour continue tant que le joueur n'a pas trouvé la bonne réponse, ou qu'il lui reste de la credibilité
                {
                    system(CLEAR);
                    LireImage(i0, IMAGES);
                    LirePassageInstantane(p0, SCRIPT);
                    printf("\n--------------------------------------------------------\n");
                    printf("               Credibility : %d%%", cred);
                    printf("\n--------------------------------------------------------\n                     OBJECT %d :\n",i+1);                    
                    AfficherObjet(true_inventaire[i], OBJETS);
                    printf("\n--------------------------------------------------------\n1 : | Next Object |           2 : | Previous Object |\n3 : | Choose |                   4 : | Yield |\n\nChoix: ");

                    char verif = 0;
                    while (!verif)
                    {
                        c = 0;
                        switch ((c = getchar()))
                        {
                        case '1': // "objet précédent"
                            if (i == 0) i = taillerelle - 1;
                            else i--;
                            verif = 1;
                            break;                         
                        case '2': // "objet suivant"
                            if (i == taillerelle - 1) i = 0;
                            else i++;
                            verif = 1;
                            break;
                        case '3':
                            while ((c = getchar()) != '\n' && c != EOF){}
                            if (true_inventaire[i] == bonne_reponse)
                            {
                                
                                reponse = bonne_reponse; 
                                LireScene2(i2,p2, IMAGES,SCRIPT);
                            }
                            else
                            {
                                cred = cred - (int)(d1*ethos);;
                                if (cred < 0)
                                    cred = 0;
                                LireScene2(i1,p1, IMAGES,SCRIPT);
                            }
                            //pressEnter();


                            verif = 1;
                            break;

                        case '4':
                            
                            while ((b = getchar()) != '\n' && c != EOF){}
                            verif = 1;
                            cred = 0;
                            //fflush(stdin);
                            //c = getchar();

                            LireScene2(i4,p4, IMAGES,SCRIPT);
                            break;
                            
                        default:
                            puts("\n");
                            system(CLEAR);
                            LireImage(i0, IMAGES);
                            LirePassageInstantane(p0, SCRIPT);
                            printf("\n--------------------------------------------------------\n");
                            printf("               Credibility : %d%%", cred);
                            printf("\n--------------------------------------------------------\n                     OBJECT %d :\n",i+1);                    
                            AfficherObjet(true_inventaire[i], OBJETS);
                            printf("\n--------------------------------------------------------\n1 : | Next Object |           2 : | Previous Object |\n3 : | Choose |                   4 : | Yield |\n\nChoix: ");

                            fflush(stdin);
                        }
                    }
                    
                }
                // à la fin du choix, afficher le résultat
            } else c = fgetc(fc);
        }
        //printf("fin du combat");
        fclose(fc);
    }
    else
    {
        printf("\nerror, file not found :");
        printf("%s", combatfile);
        printf("\n");
    }

    
    //pressEnter();
    if(cred == 0) 
    {
        pressEnter();
        return 0;
    }
    else return 1;
}
