
#ifndef _graphicSDL_h
#define _graphicSDL_h

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>

// taille fenetre
#define SCREEN_WIDTH 1612
#define SCREEN_HEIGHT 842
// vrai/faux
#define FALSE 0
#define TRUE 1
// pi
#define PI 3.14159265359
// MS per
#define MS_PER_UPDATE 60
//couleurs pr�d�finies
#define rgb2pixel(r,g,b) 			((long) ( (( uint8_t )r)<<8 | (( uint8_t )g))<<8 | (( uint8_t )b) )
#define Black						rgb2pixel(0  ,0   ,   0)
#define White						rgb2pixel(255,255 , 255)
#define Gray 						rgb2pixel(190, 190, 190)
#define LightGrey					rgb2pixel(211, 211, 211)



// init/close fenetre + ecran
int init(SDL_Window** gWindow,SDL_Renderer** gRenderer, char* nameWindow);
void close_app(SDL_Window** gWindow,SDL_Renderer** gRenderer);

// dessin de formes simples

void dessine_ligne(SDL_Renderer* gRenderer,int x1, int y1, int x2, int y2,long coul);
void ligneHorizontale(SDL_Renderer *gRenderer,int x, int y, int w, long c);
void dessine_disque(SDL_Renderer* gRenderer,int cx, int cy, int rayon, long coul);
void dessine_fondtransparent(SDL_Renderer* gRenderer, int w,int h);
SDL_Texture* creerTextureImage(SDL_Renderer *ecran,char *nom_file_bmp,int *size_w, int *size_h);
void dessine_image(SDL_Renderer *ecran,SDL_Texture *imageTexture,int x, int y);
void dessine_image_redim(SDL_Renderer *ecran,SDL_Texture *imageTexture,int x, int y, int w_new,int h_new);

// pour le controle du temps des mises � jour
double getCurrentTimeMS();

/*algorithme qui aide a l'affichage du score*/
int* digitPositionsFromScore(int score,int* numDigits);

#endif // _graphicSDL_h
