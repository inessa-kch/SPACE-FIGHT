#ifndef _MISSILE_h
#define _MISSILE_h

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>
#include "vaisseau.h"
#include "asteroides.h"

#define M_WIDTH 10
#define M_HEIGHT 10
#define M_RAYON 5

#define VITESSE_MISSILE 10

#define MISSILE_MAX 4 //nombre de missiles max sur l'ecran

typedef struct _missile{
   float dx,dy;
   float dir[2];
   float angle;
   int energie;//devient 0 si le missile sort de l'ecran (il est alors effacé de la liste),est a 1 sinon
   SDL_Texture *texture;
   struct _missile* suivant;
} Missile;



/*creer la texture du missile*/
SDL_Texture *CreerTextureMissile(SDL_Renderer*ecran,int rayon, int w,int h,long couleur);
/*dessiner les missiles sur l'ecran*/
void dessineMissile(SDL_Renderer* ecran, SDL_Texture* textureMissile,Missile* listeMissile,int w,int h);
/*cree un missile*/
Missile* CreerMissile();
/*ajouter un missile en fin dans la liste chainée*/
void ajouterMissileFin(Missile** listeMissile,Missile* m);
/*retirer un missile de la liste chainée*/
void enlever_missile(Missile **listeMissile, Missile *m);
/*effacer la liste chainée*/
void effacerListeMissile(Missile** listeMissile);
/*detecte si on a appuyé sur l'espace*/
int detectionMissile(SDL_Event e);
/*gestion du mouvement des missiles*/
void bougerMissile(Vaisseau* nv,Missile** listeMissile,SDL_Event e);
/*deplacer les missiles*/
void deplacerMissile(Missile** listeMissile);
/*compter le nombre de missiles sur l'ecran*/
int compteMissile(Missile** listeMissile);
/*detection de collision entre missile et asteroide*/
void collisionAsM(Asteroide** listeAsteroide, Missile** listeMissile,Vaisseau** nv);
/*collision detectée: on enleve supprime le missile de la liste*/
void collisionDetecteM(Missile** listeMissile);



#endif

