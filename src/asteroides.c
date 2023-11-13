
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <math.h>
#include "asteroides.h"
#include "graphicSDL.h"
#include <assert.h>




 void CreerTextureAsteroide(SDL_Renderer*ecran,int w,int h,long couleur,Asteroide** listeAsteroide){
  SDL_Texture* textureAsteroide=NULL;
  Asteroide* tmp = *listeAsteroide;
  while (tmp!=NULL){
    if (tmp->taille==1){
        textureAsteroide=SDL_CreateTexture(ecran,SDL_PIXELFORMAT_RGBA8888,SDL_TEXTUREACCESS_TARGET,w,h);
        SDL_RenderClear(ecran);
        SDL_SetTextureBlendMode(textureAsteroide, SDL_BLENDMODE_BLEND); 
        SDL_SetRenderTarget(ecran, textureAsteroide); //on modifie la texture
        // dessine fond transparent
        dessine_fondtransparent(ecran,w,h);
        // dessine ligne horizontale couleur
        dessine_ligne(ecran,0,SCREEN_HEIGHT/15,0,SCREEN_HEIGHT/7,couleur);
        dessine_ligne(ecran,0,SCREEN_HEIGHT/7,SCREEN_WIDTH/27,SCREEN_HEIGHT/5,couleur);
        dessine_ligne(ecran,SCREEN_WIDTH/27,SCREEN_HEIGHT/5,SCREEN_WIDTH/13,SCREEN_HEIGHT/7,couleur);
        dessine_ligne(ecran,SCREEN_WIDTH/13,SCREEN_HEIGHT/7,SCREEN_WIDTH/9,SCREEN_HEIGHT/7,couleur);
        dessine_ligne(ecran,SCREEN_WIDTH/9,SCREEN_HEIGHT/7,SCREEN_WIDTH/9,SCREEN_HEIGHT/15,couleur);
        dessine_ligne(ecran,SCREEN_WIDTH/9,SCREEN_HEIGHT/15,SCREEN_WIDTH/13,0,couleur);
        dessine_ligne(ecran,SCREEN_WIDTH/13,0,SCREEN_WIDTH/27,0,couleur);
        dessine_ligne(ecran,SCREEN_WIDTH/27,0,0,SCREEN_HEIGHT/15,couleur);
        SDL_SetRenderTarget(ecran, NULL);
        tmp->texture=textureAsteroide;
    }
    else if (tmp->taille==0){
        textureAsteroide=SDL_CreateTexture(ecran,SDL_PIXELFORMAT_RGBA8888,SDL_TEXTUREACCESS_TARGET,w/2,h/2);
        SDL_RenderClear(ecran);
        SDL_SetTextureBlendMode(textureAsteroide, SDL_BLENDMODE_BLEND); 
        SDL_SetRenderTarget(ecran, textureAsteroide); //on modifie la texture
        // dessine fond transparent
        dessine_fondtransparent(ecran,w/2,h/2);
        // dessine ligne horizontale couleur
        dessine_ligne(ecran,0,SCREEN_HEIGHT/30,0,SCREEN_HEIGHT/14,couleur);
        dessine_ligne(ecran,0,SCREEN_HEIGHT/14,SCREEN_WIDTH/54,SCREEN_HEIGHT/10,couleur);
        dessine_ligne(ecran,SCREEN_WIDTH/54,SCREEN_HEIGHT/10,SCREEN_WIDTH/26,SCREEN_HEIGHT/14,couleur);
        dessine_ligne(ecran,SCREEN_WIDTH/26,SCREEN_HEIGHT/14,SCREEN_WIDTH/18,SCREEN_HEIGHT/14,couleur);
        dessine_ligne(ecran,SCREEN_WIDTH/18,SCREEN_HEIGHT/14,SCREEN_WIDTH/18,SCREEN_HEIGHT/30,couleur);
        dessine_ligne(ecran,SCREEN_WIDTH/18,SCREEN_HEIGHT/30,SCREEN_WIDTH/26,0,couleur);
        dessine_ligne(ecran,SCREEN_WIDTH/26,0,SCREEN_WIDTH/54,0,couleur);
        dessine_ligne(ecran,SCREEN_WIDTH/54,0,0,SCREEN_HEIGHT/30,couleur);
        SDL_SetRenderTarget(ecran, NULL);
        tmp->texture=textureAsteroide;

    }
    tmp=tmp->suivant;
  }
}


void suppressionTextureAsteroide(Asteroide** listeAsteroide){
    Asteroide* tmp = *listeAsteroide;
    while (tmp!=NULL){
        SDL_DestroyTexture(tmp->texture);
        tmp=tmp->suivant;
    }
}


void dessineAsteroide(SDL_Renderer *ecran, Asteroide** listeAsteroide,int h,int w)
{
    Asteroide *tmp = *listeAsteroide;
    while (tmp != NULL) {
        SDL_Rect asteroid_rect = {
            (int) tmp->dx,
            (int) tmp->dy,
            w,   
            h    
        };
        SDL_QueryTexture(tmp->texture,NULL,NULL,&asteroid_rect.w,&asteroid_rect.h);
        SDL_RenderCopy(ecran, tmp->texture, NULL, &asteroid_rect);
        tmp = tmp->suivant;
    }
}


Asteroide* CreerAsteroide() {
    Asteroide* na = (Asteroide*) malloc(sizeof(Asteroide));
    assert(na != NULL);

    int side = rand() % 4; //pour les 4 cotés de l'ecran
    switch (side) {
        case 0: //en haut
            na->dx = rand() % SCREEN_WIDTH;
            na->dy = -A_HEIGHT/2;
            na->dir[0] = (rand() / (float)RAND_MAX - 0.5) * 2.0 * VITESSE_ASTEROIDE; //permet de generer une direction float aleatoire enter -VITESSE_ASTEROIDE et VITESSE_ASTEROIDE
            na->dir[1] = rand() / (float)RAND_MAX * VITESSE_ASTEROIDE;//permet de generer une direction float aleatoire enter 0 et VITESSE_ASTEROIDE
            break;
        case 1: // a droite
            na->dx = SCREEN_WIDTH + A_WIDTH/2;
            na->dy = rand() % SCREEN_HEIGHT;
            na->dir[0] = -rand() / (float)RAND_MAX *VITESSE_ASTEROIDE;
            na->dir[1] = (rand() / (float)RAND_MAX - 0.5) * 2.0 * VITESSE_ASTEROIDE;
            break;
        case 2: // en bas
            na->dx = rand() % SCREEN_WIDTH;
            na->dy = SCREEN_HEIGHT + A_HEIGHT/2;
            na->dir[0] = (rand() / (float)RAND_MAX - 0.5) * 2.0 * VITESSE_ASTEROIDE;
            na->dir[1] = -rand() / (float)RAND_MAX * VITESSE_ASTEROIDE;
            break;
        case 3: // a gauche
            na->dx = -A_WIDTH/2;
            na->dy = rand() % SCREEN_HEIGHT;
            na->dir[0] = rand() / (float)RAND_MAX * VITESSE_ASTEROIDE;
            na->dir[1] = (rand() / (float)RAND_MAX - 0.5) * 2.0 * VITESSE_ASTEROIDE;
            break;
    }
    na->suivant = NULL;
    na->taille = rand() % 2; //pour les 2 tailles d'asteroide
    na->energie = 1; //energie de l'asteroide devient 0 quand il y a collision avec un missile
    return na;
}


void ajouterAsteroideFin(Asteroide** listeAsteroide, Asteroide* na){
    if(*listeAsteroide==NULL){
        *listeAsteroide=na;
    }
    else{
        Asteroide* tmp=*listeAsteroide;
        while(tmp->suivant != NULL){
            tmp=tmp->suivant;
        }
        tmp->suivant = na;
    }
}




void enleverAsteroide(Asteroide **listeAsteroide, Asteroide *na) {
  Asteroide *temp = *listeAsteroide; 
  Asteroide *prec = NULL;
  if (*listeAsteroide == NULL) return;

  while((temp) && (temp != na)){
    prec = temp;
    temp = temp ->suivant;
  }
  if (prec == NULL) {
    *listeAsteroide = (*listeAsteroide)->suivant;
  }
  else {
    prec->suivant = temp->suivant;
  }
  
  free(temp);
}




void effacerListeAsteroide(Asteroide** listeAsteroide) {
    Asteroide* tmp = *listeAsteroide;
    Asteroide* suiv;

    while (tmp != NULL) {
        suiv = tmp->suivant;
        free(tmp);
        tmp = suiv;
    }

    *listeAsteroide = NULL;  
}  

int compteAsteroides(Asteroide** listeAsteroide){
    Asteroide* tmp = *listeAsteroide;
    int res=0;
    while (tmp) {
        res++;
        tmp = tmp->suivant;
    }
    return res;
}


void genererAsteroide(Asteroide** listeAsteroide,int score,int* prevScore,int* nbAsteroides){
    if (*prevScore != score && score%DIFFICULTE_SCORE==0){
        *nbAsteroides = *nbAsteroides + 1;
    }
    for (int i = 0; i < *nbAsteroides; i++) {
        if (compteAsteroides(listeAsteroide) < ASTEROIDE_MAX){
            Asteroide* na=CreerAsteroide();
            ajouterAsteroideFin(listeAsteroide, na);
        }
    }
    *prevScore = score;
}


void miseAjour_asteroide_temps(SDL_Renderer* ecran,double* elapsed,int* asteroide_temps,Asteroide** listeAsteroide,int score,int* prevScore,int* nbAsteroides) {
    *asteroide_temps -= *elapsed;
    if (*asteroide_temps <= 0) {
        genererAsteroide(listeAsteroide,score,prevScore,nbAsteroides);
        CreerTextureAsteroide(ecran,A_WIDTH,A_HEIGHT,White,listeAsteroide);
        *asteroide_temps = rand() % (ASTEROID_MAX_GENERER_TEMPS - ASTEROID_MIN_GENERER_TEMPS) + ASTEROID_MIN_GENERER_TEMPS;
    }
}


void deplacerAsteroide(Asteroide** listeAsteroide){
    Asteroide* tmp=*listeAsteroide;
    while(tmp != NULL){
        tmp->dx += tmp->dir[0];
        tmp->dy += tmp->dir[1];
        tmp->dx = (tmp->dx<0?SCREEN_WIDTH-A_WIDTH/2:(tmp->dx>=SCREEN_WIDTH-A_WIDTH/2? 0 : tmp->dx));
        tmp->dy = (tmp->dy<0?SCREEN_HEIGHT-A_HEIGHT/2:(tmp->dy>=SCREEN_HEIGHT-A_HEIGHT/2? 0 : tmp->dy));
        tmp=tmp->suivant;
    }
}



void collisionDetecteA(Asteroide** listeAsteroide){
    Asteroide* tmpA=*listeAsteroide;
    while(tmpA != NULL){
        if (tmpA->energie == 0){
            enleverAsteroide(listeAsteroide, tmpA);
        }
        tmpA=tmpA->suivant;}
}


