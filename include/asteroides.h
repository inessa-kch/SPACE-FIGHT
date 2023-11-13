#ifndef _ASTEROIDES_h
#define _ASTEROIDES_h



#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>


#define ASTEROIDE_MAX 15 //nombre maximum d'asteroides sur l'ecran

#define A_WIDTH 180
#define A_HEIGHT 170


#define VITESSE_ASTEROIDE 2
#define ASTEROID_MAX_GENERER_TEMPS 6000 //temps maximum pour generer un asteroide
#define ASTEROID_MIN_GENERER_TEMPS 4000 //temps minimum pour generer un asteroide
#define DIFFICULTE_SCORE 100 //a chaque 100 points obtenus on augmente le nombre d'asteroides qu'on genere


typedef struct _asteroide{
   float dx,dy;
   float dir[2];
   int energie;
   int taille; //taille de l'asteroide (0 ou 1)
   SDL_Texture *texture;
   struct _asteroide* suivant;
} Asteroide;

/*Créer une texture pour l'astéroide*/
void CreerTextureAsteroide(SDL_Renderer*ecran,int w,int h,long couleur,Asteroide** listeAsteroide);
/*supprimer la texture des asteroides*/
void suppressionTextureAsteroide(Asteroide** listeAsteroide);
/*dessine les asteroides sur l'ecran*/
void dessineAsteroide(SDL_Renderer *ecran, Asteroide** listeAsteroide,int h,int w);
/*creer un nouvel asteroide*/
Asteroide* CreerAsteroide();
/*ajoute un asteroide a la fin de la liste*/
void ajouterAsteroideFin(Asteroide** listeAsteroide, Asteroide* na);
/*supprimer un asteroide de la liste*/
void enleverAsteroide(Asteroide **listeAsteroide, Asteroide *na);
/*supprimer tous les asteroides de la liste*/
void effacerListeAsteroide(Asteroide** listeAsteroide);
/*compte le nombre d'asteroides dans la liste*/
int compteAsteroides(Asteroide** listeAsteroide);
/*generer les asteroides selon le score: chaque 100 points obtenus on génere de plus en plus d'asteroides*/
void genererAsteroide(Asteroide** listeAsteroide,int score,int* prevScore,int* nbAsteroides);
/*Pour generer des asteroides au cours du temps de facon aleatoire*/
void miseAjour_asteroide_temps(SDL_Renderer* ecran,double* elapsed,int* asteroide_temps,Asteroide** listeAsteroide,int score,int* prevScore,int* nbAsteroides);
/*deplacer les asteroides*/
void deplacerAsteroide(Asteroide** listeAsteroide);
/*detecter les collisions entre le vaisseau et les asteroides ou entre les missiles et les asteroides*/
void collisionDetecteA(Asteroide** listeAsteroide);

#endif