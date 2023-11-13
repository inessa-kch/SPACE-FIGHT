#ifndef _VAISSEAU_h
#define _VAISSEAU_h

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <math.h>
#include "asteroides.h"


#define V_WIDTH 59
#define V_HEIGHT 56

#define VITESSE_ROTATION 15
#define VITESSE_DEPLACEMENT 2
#define VITESSE_MAX 10 //vitesse maximale du vaisseau
#define FROTTEMENT 0.99 //frottement du vaisseau
#define TEMPS_PASSIF 13000 //temps pendant lequel le vaisseau est passif (on ne détecte pas la collision) apres une collision avec un asteroide


typedef struct _vaisseau{
   float dx,dy;
   float dir[2];
   float angle;
   int energie; //devient 0 si il est touché par un asteroide,est a 1 sinon
   int health; //5 points de vie
   int score;//score du joueur :+5 grand asteroide, +10 petit asteroide
   int vaisseau_passif; //temps pendant lequel le vaisseau est passif apres une collision avec un asteroide
} Vaisseau;






/*cree un vaisseau*/ 
Vaisseau* CreerVaisseau();
/*si le joueur decide de continuer a jouer on reinitialise toutes les variables du vaisseau*/
void reinitialiserVaisseau(Vaisseau* nv);
/*si le joueur decide de quitter on efface le vaisseau*/
void effacerVaisseau(Vaisseau* nv);
/*cree une texture de vaisseau*/
SDL_Texture *CreerTextureVaisseau(SDL_Renderer*ecran,int w,int h,long couleur);
/*dessine le vaisseau sur l'ecran*/
void dessine_vaisseau(SDL_Renderer *ecran, Vaisseau** nv,SDL_Texture *textureVaisseau);
/*gestion du mouvement du vaisseau*/
void bouger_vaisseau(Vaisseau** nv,SDL_Event e,int* quit);
/*deplace le vaisseau sur l'ecran*/
void deplacerVaisseau(Vaisseau** nv);
/*detection de collision entre asteroide et vaisseau*/
int collisionAsV(Asteroide** listeAsteroide, Vaisseau* nv);
/*repositionne le vaisseau au milieu de l'ecran s'il y a eu collision avec un asteroide*/
void repositionnerVaisseaau(Vaisseau** nv);
/*lecture du Highscore*/
void lectureHighscore(int* highscore);
/*sauvegarde du Highscore*/
void sauvegardeHighscore(int highscore);


#endif