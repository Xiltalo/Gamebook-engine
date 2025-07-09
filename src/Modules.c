#include <Modules.h>
#include <Images.h>
#include <Scripts.h>
#include <Combats.h>
#include <Menus.h>

#include <stdio.h>
#include <time.h>
#include <stdlib.h>

// si cette fonction est utilisée avec l'inventaire comme paramètre, alors elle vérifie la présence d'un objet dans l'inventaire. renvoie 1 si l'évènement vaut 1.
int CheckForEvent(int id_event, char *events) // renvoie 0(true), si l'event indiqué est dans a sauvegarde, renvoie 1(false) sinon (à implémenter)
{
    int ret;
    if (events[id_event] != '0')
        ret = 1; // true
    else
        ret = 0; //false

    return ret;
}

// si cette fonction est utilisée avec "inventaire" à la place de "events" alors cela revient à mettre quelque chose dans l'inventaire (ou retirer, selon la valeur (1 = présent, 0 = absent)
void EventTrigger(int id_event, char *events, char value)
{
    if (value == '0')
    {
        events[id_event] = '0';
    }
    else
        events[id_event] = '1';
}

// parcourt un fichier texte et éxecute des actions en fonction des caractères rencontrés.
void lireModule(int id_module, char *modules_file, char *images_file, char *text_file, char *events, char *inventaire)
{
    FILE *f;
    
    if (f = fopen(modules_file, "r"))
    {

        srand(time(NULL)); // initiaise le générateur de nombre aléatoire

    

    
        int id, random, choix;
        int id_event, value_event; // il n'y a probablement pas besoin de déclarer autant de variables, certaines peuvent être réutilisées. à optimiser
        char value;
        int id_module1, id_module2, id_module3, id_module4, id_combat, rez;

        int id_module_precedent = 0; // sauvegarde en mémoire le module précédent.

        char str1[50], str2[50], str3[50], str4[50];
        char chr1,chr2;
        int id_image;
        int id_passage;
        int id_passage1, id_image1, id_passage2, id_image2; // id1 et id2 servent pour les scènes alternatives

        fscanf(f, ":%d:", &id);
        skipUntilChar(f, '\n');
        while (id != id_module && id != ID_END)
        {
            skipUntilChar(f, FINMODULE);
            skipUntilChar(f, '\n');
            fscanf(f, ":%d:", &id);
            skipUntilChar(f, '\n');
        }

        char c = fgetc(f);
        while (c != FINMODULE && c != EOF)
        {
            if (c == COMMENTAIRE)
                skipUntilChar(f, '\n');
            else if (c == SAVING)
                Save(SAVE, id_module, events, inventaire);
            else if (c == LIRESCENE)
            {
                fscanf(f, " i%d p%d", &id_image, &id_passage);
                LireScene(id_image, id_passage, IMAGES, SCRIPT);
            }
            else if (c == LIRESCENE2)
            {
                fscanf(f, " i%d p%d", &id_image, &id_passage);
                LireScene2(id_image, id_passage, IMAGES, SCRIPT);
            }
            else if (c == LIREIMAGE)
            {
                fscanf(f, " %d", &id_image);
                LireImage(id_image, IMAGES);
            }
            else if (c == LIREPASSAGE)
            {
                fscanf(f, " %d", &id_passage);
                LirePassage(id_passage, SCRIPT);
            }
            else if (c == CHOIX1)
            {
                fscanf(f, " p%d i%d [%49[^]]] [%49[^]]] 1 p%d i%d 2 p%d i%d", &id_passage, &id_image, str1, str2, &id_passage1, &id_image1, &id_passage2, &id_image2);
                if (MENUCHOIX(str1, str2, SCRIPT, id_passage, IMAGES, id_image) == 1)
                {
                    LireScene2(id_image1, id_passage1, IMAGES, SCRIPT);
                }
                else
                    LireScene2(id_image2, id_passage2, IMAGES, SCRIPT);
            }
            else if (c == GOTOMODULE)
            {
                id_module_precedent = id_module;
                fscanf(f, " %d", &id_module);

                rewind(f);              // permet de revenir au début du fichier
                fscanf(f, ":%d:", &id); // cherche le bon module
                skipUntilChar(f, '\n');
                while (id != id_module && id != ID_END)
                {
                    skipUntilChar(f, FINMODULE);
                    skipUntilChar(f, '\n');
                    fscanf(f, ":%d:", &id);
                    skipUntilChar(f, '\n');
                }
            }
            else if (c == CHOIX2)
            {
                id_module_precedent = id_module;
                fscanf(f, " p%d i%d [%49[^]]] [%49[^]]] m%d m%d", &id_passage, &id_image, str1, str2, &id_module1, &id_module2);
                
                if (MENUCHOIX(str1, str2, SCRIPT, id_passage, IMAGES, id_image) == 1)
                    id_module = id_module1;
                else 
                    id_module = id_module2;

                rewind(f);              // permet de revenir au début du fichier
                fscanf(f, ":%d:", &id); // cherche le bon module
                skipUntilChar(f, '\n'); // skips module title/description
                while (id != id_module && id != ID_END)
                {
                    skipUntilChar(f, FINMODULE);
                    skipUntilChar(f, '\n');
                    fscanf(f, ":%d:", &id);
                    skipUntilChar(f, '\n');
                }
            }
            else if (c == CHOIX3 || c == CHOIX3INSTANT)
            {
                id_module_precedent = id_module;
                fscanf(f, " p%d i%d [%49[^]]] [%49[^]]] [%49[^]]] m%d m%d m%d", &id_passage, &id_image, str1, str2, str3, &id_module1, &id_module2, &id_module3);
                if (c == CHOIX3) 
                    choix = MENUFOURCHE(str1, str2, str3, SCRIPT, id_passage, IMAGES, id_image, 0);
                else choix = MENUFOURCHE(str1, str2, str3, SCRIPT, id_passage, IMAGES, id_image, 1);
                
                if ( choix == 1)
                    id_module = id_module1;
                else if (choix == 2)
                    id_module = id_module2;
                else 
                    id_module = id_module3;

                rewind(f);              // permet de revenir au début du fichier
                fscanf(f, ":%d:", &id); // cherche le bon module
                skipUntilChar(f, '\n'); // skips module title/description
                while (id != id_module && id != ID_END)
                {
                    skipUntilChar(f, FINMODULE);
                    skipUntilChar(f, '\n');
                    fscanf(f, ":%d:", &id);
                    skipUntilChar(f, '\n');
                }
            }
            else if (c == CHOIX4 || c == CHOIX4INSTANT)
            {
                id_module_precedent = id_module;
                fscanf(f, " p%d i%d [%49[^]]] [%49[^]]] [%49[^]]] [%49[^]]] m%d m%d m%d m%d", &id_passage, &id_image, str1, str2, str3, str4, &id_module1, &id_module2, &id_module3, &id_module4);
                if (c == CHOIX3) 
                    choix = MENUQUATRE(str1, str2, str3, str4, SCRIPT, id_passage, IMAGES, id_image, 0);
                else choix = MENUQUATRE(str1, str2, str3, str4, SCRIPT, id_passage, IMAGES, id_image, 1);
                
                if ( choix == 1)
                    id_module = id_module1;
                else if (choix == 2)
                    id_module = id_module2;
                else  if (choix == 3)
                    id_module = id_module3;
                else
                    id_module = id_module4;

                rewind(f);              // permet de revenir au début du fichier
                fscanf(f, ":%d:", &id); // cherche le bon module
                skipUntilChar(f, '\n'); // skips module title/description
                while (id != id_module && id != ID_END)
                {
                    skipUntilChar(f, FINMODULE);
                    skipUntilChar(f, '\n');
                    fscanf(f, ":%d:", &id);
                    skipUntilChar(f, '\n');
                }
            }
            else if (c == EVENT)
            {
                fscanf(f, " e%d v%c", &id_event, &value); // la value est lue comme "zéro ou un". n'importe quoi d'autre que zéro sera lu comme un "1"
                EventTrigger(id_event, events, value);
            }
            else if (c == OBJET)
            {
                fscanf(f, " o%d v%c", &id_event, &value); // la value est lue comme "zéro ou un". n'importe quoi d'autre que zéro sera lu comme un "1"
                EventTrigger(id_event, inventaire, value);
            }
            else if (c == CHECKEVENT)
            {
                fscanf(f, " e%d 0 p%d i%d 1 p%d i%d", &id_event, &id_passage1, &id_image1, &id_passage2, &id_image2);
                if (CheckForEvent(id_event, events) == 0)
                {
                    LireScene2(id_image1, id_passage1, IMAGES, SCRIPT);
                }
                else
                    LireScene2(id_image2, id_passage2, IMAGES, SCRIPT);
            }
            else if (c == CHECKEVENT2)
            {
                id_module_precedent = id_module;
                fscanf(f, " e%d v%d m%d", &id_event,&value_event, &id_module1);
                if (CheckForEvent(id_event, events) == value_event)
                {
                    id_module = id_module1;
                    rewind(f);              // permet de revenir au début du fichier
                    fscanf(f, ":%d:", &id); // cherche le bon module
                    skipUntilChar(f, '\n'); // skips module title/description
                    while (id != id_module && id != ID_END)
                    {
                        skipUntilChar(f, FINMODULE);
                        skipUntilChar(f, '\n');
                        fscanf(f, ":%d:", &id);
                        skipUntilChar(f, '\n');
                    }
                }
            }
            else if (c == CHECKINVENT)
            {
                id_module_precedent = id_module;
                fscanf(f, " o%d m%d m%d", &id_event, &id_module1, &id_module2);
                if (CheckForEvent(id_event, inventaire) == 1)
                    id_module = id_module1;
                else
                    id_module = id_module2;

                rewind(f);              // permet de revenir au début du fichier
                fscanf(f, ":%d:", &id); // cherche le bon module
                skipUntilChar(f, '\n'); // skips module title/description
                while (id != id_module && id != ID_END)
                {
                    skipUntilChar(f, FINMODULE);
                    skipUntilChar(f, '\n');
                    fscanf(f, ":%d:", &id);
                    skipUntilChar(f, '\n');
                }
            }
            else if (c == CHECKINVENT2)
            {
                id_module_precedent = id_module;
                fscanf(f, " o%d v%d m%d", &id_event,&value_event, &id_module1);
                if (CheckForEvent(id_event, inventaire) == value_event)
                {
                    id_module = id_module1;
                    rewind(f);              // permet de revenir au début du fichier
                    fscanf(f, ":%d:", &id); // cherche le bon module
                    skipUntilChar(f, '\n'); // skips module title/description
                    while (id != id_module && id != ID_END)
                    {
                        skipUntilChar(f, FINMODULE);
                        skipUntilChar(f, '\n');
                        fscanf(f, ":%d:", &id);
                        skipUntilChar(f, '\n');
                    }
                }
            }
            else if (c== FIGHT)
            {
                id_module_precedent = id_module;
                fscanf(f, " c%d d%d v%d", &id_combat, &id_module1, &id_module2);
                //printf("\n%d %d %d",id_combat, id_module1, id_module2);
                //pressEnter();
                if(LireCombat(COMBATS, id_combat, inventaire) == 1)
                    id_module = id_module2;
                else 
                    id_module = id_module1;
                
                rewind(f);              // permet de revenir au début du fichier
                fscanf(f, ":%d:", &id); // cherche le bon module
                skipUntilChar(f, '\n'); // skips module title/description
                while (id != id_module && id != ID_END)
                {
                    skipUntilChar(f, FINMODULE);
                    skipUntilChar(f, '\n');
                    fscanf(f, ":%d:", &id);
                    skipUntilChar(f, '\n');
                }
            }
            else if (c == RANDOMSCENE)
            {
                fscanf(f, " %d p%d i%d p%d i%d", &random, &id_passage1, &id_image1, &id_passage2, &id_image2);
                if (rand()%100 <= random)
                {
                    LireScene2(id_image1, id_passage1, IMAGES, SCRIPT);
                }
                else
                    LireScene2(id_image2, id_passage2, IMAGES, SCRIPT);
            }
            else if (c == RANDOMMODULE)
            {
                fscanf(f, " %d m%d m%d", &random, &id_module1, &id_module2);
                if (rand()%100 <= random)
                {
                    id_module = id_module1;
                }
                else
                    id_module = id_module2;
                rewind(f);              // permet de revenir au début du fichier
                fscanf(f, ":%d:", &id); // cherche le bon module
                skipUntilChar(f, '\n'); // skips module title/description
                while (id != id_module && id != ID_END)
                {
                    skipUntilChar(f, FINMODULE);
                    skipUntilChar(f, '\n');
                    fscanf(f, ":%d:", &id);
                    skipUntilChar(f, '\n');
                }
            }
            else if (c == EVENTANDEVENT)
            {
                fscanf(f, " %c%d AND %c%d m%d", &chr1, &id_event, &chr2, &id_passage1, &id_module1);

                if (chr1 == 'e') id_image1 = CheckForEvent(id_event, events); 
                else if (chr1 == 'o') id_image1 = CheckForEvent(id_event, inventaire);
                if (chr2 == 'e') id_image2 = CheckForEvent(id_passage1, events);
                else if (chr2 == 'o') id_image2 = CheckForEvent(id_passage1, inventaire);

                if(id_image1 && id_image2)
                {
                    id_module = id_module1;
                    rewind(f);              // permet de revenir au début du fichier
                    fscanf(f, ":%d:", &id); // cherche le bon module
                    skipUntilChar(f, '\n'); // skips module title/description
                    while (id != id_module && id != ID_END)
                    {
                        skipUntilChar(f, FINMODULE);
                        skipUntilChar(f, '\n');
                        fscanf(f, ":%d:", &id);
                        skipUntilChar(f, '\n');
                    }
                }
            }
            else if (c == INVENTAIRE)
            {
                ConsulterInventaire(inventaire);
            }
            else if (c == RETOUR)
            {
                id_module = id_module_precedent;
                rewind(f);              // permet de revenir au début du fichier
                fscanf(f, ":%d:", &id); // cherche le bon module
                skipUntilChar(f, '\n'); // skips module title/description
                while (id != id_module && id != ID_END)
                {
                    skipUntilChar(f, FINMODULE);
                    skipUntilChar(f, '\n');
                    fscanf(f, ":%d:", &id);
                    skipUntilChar(f, '\n');
                }
                
            }
            // rewind(f) //permet de revenir au début du fichier

            c = fgetc(f);
        }

        pressEnter();
        fclose(f); // fermeture du fichier modules
    }
    else
    {
        printf("\nerror, file not found :");
        printf("%s", modules_file);
        printf("\n");
    }
}
