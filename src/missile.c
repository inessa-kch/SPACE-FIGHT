#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <math.h>
#include "missile.h"
#include "graphicSDL.h"
#include <assert.h>





SDL_Texture *CreerTextureMissile(SDL_Renderer*ecran,int rayon, int w,int h,long couleur){
  SDL_Texture* textureMissile=NULL;
  textureMissile=SDL_CreateTexture(ecran,SDL_PIXELFORMAT_RGBA8888,SDL_TEXTUREACCESS_TARGET,w,h);
  SDL_RenderClear(ecran); // nettoie ecran
  SDL_SetTextureBlendMode(textureMissile, SDL_BLENDMODE_BLEND); 
  SDL_SetRenderTarget(ecran, textureMissile); //on modifie la texture
  dessine_fondtransparent(ecran,w,h);// dessine fond transparent

  dessine_disque(ecran,rayon,rayon,rayon,couleur);

  SDL_SetRenderTarget(ecran, NULL);

  return textureMissile;
}



void dessineMissile(SDL_Renderer* ecran, SDL_Texture* textureMissile,Missile* listeMissile,int w,int h){
    Missile *tmp = listeMissile;
    while (tmp != NULL) {
        SDL_Rect missile_rect = {
            (int) tmp->dx,
            (int) tmp->dy,
            w,   
            h    
        };
        SDL_QueryTexture(textureMissile,NULL,NULL,&missile_rect.w,&missile_rect.h);
        SDL_RenderCopy(ecran, textureMissile, NULL, &missile_rect);
        tmp = tmp->suivant;
    }
}



Missile* CreerMissile(){
    Missile* m=malloc(sizeof(Missile));
    assert(m);
    return m;
}





void ajouterMissileFin(Missile** listeMissile,Missile* m) {
    if (*listeMissile == NULL) {
        *listeMissile = m;
    } else {
        Missile* tmp = *listeMissile;
        while (tmp->suivant != NULL) {
            tmp = tmp->suivant;
        }
        tmp->suivant = m;
    }
}





void enlever_missile(Missile **listeMissile, Missile *m) {
  Missile *temp = *listeMissile; 
  Missile *prec = NULL;
  if (*listeMissile == NULL) return;

  while((temp) && (temp != m)){
    prec = temp;
    temp = temp ->suivant;
  }
  if (prec == NULL) {
    *listeMissile = (*listeMissile)->suivant;
  }
  else {
    prec->suivant = temp->suivant;
  }
  
  free(temp);
}




void effacerListeMissile(Missile** listeMissile) {
    Missile* tmp = *listeMissile;
    Missile* suiv;

    while (tmp != NULL) {
        suiv = tmp->suivant;
        free(tmp);
        tmp = suiv;
    }

    *listeMissile = NULL;  
}





int detectionMissile(SDL_Event e){
    int espace=0;
    switch(e.type){
        case SDL_KEYDOWN:
            switch(e.key.keysym.sym){
                case SDLK_SPACE:
                    espace=1;
                break;
            }
        break;
    }
    return espace;
}


int compteMissile(Missile** listeMissile){
    int nbMissile=0;
    Missile* tmp=*listeMissile;
    if (tmp==NULL){
        return 0;
    }
    while(tmp!=NULL){
        nbMissile++;
        tmp=tmp->suivant;
    }
    return nbMissile;
}


void bougerMissile(Vaisseau* nv,Missile** listeMissile,SDL_Event e) {
    if (detectionMissile(e)) {
        if (compteMissile(listeMissile) < MISSILE_MAX){
            Missile *m = CreerMissile();
            m->dx = nv->dx  + sin((nv->angle*PI)/180.0) * V_WIDTH/2;
            m->dy = nv->dy - cos((nv->angle*PI)/180.0) * V_HEIGHT/2;
            m->angle = nv->angle;
            m->dir[0] = sin((nv->angle*PI)/180.0) * VITESSE_MISSILE;
            m->dir[1] = -cos((nv->angle*PI)/180.0) * VITESSE_MISSILE;
            m->energie = 1; 
            m->suivant = NULL;
            ajouterMissileFin(listeMissile, m);
        }
    }

}



void deplacerMissile(Missile** listeMissile) {
    Missile* tmp = *listeMissile;
    while (tmp != NULL) {
        tmp->dx += tmp->dir[0];
        tmp->dy += tmp->dir[1];

        /*enelver les missiles qui sortent de l'ecran*/
        if (tmp->dx < 0 || tmp->dx > SCREEN_WIDTH || tmp->dy < 0 || tmp->dy > SCREEN_HEIGHT || tmp->energie == 0) {
            enlever_missile(listeMissile, tmp);
        }
        tmp=tmp->suivant;
    }
}





void collisionAsM(Asteroide** listeAsteroide, Missile** listeMissile,Vaisseau** nv){
    Asteroide* tmpA=*listeAsteroide;
    Missile* tmpM=*listeMissile;
    while(tmpA != NULL){
        while(tmpM != NULL){
            SDL_Rect missile_rect ={tmpM->dx,tmpM->dy,M_WIDTH,M_HEIGHT};
            if (tmpA->taille==0){
                SDL_Rect asteroide_rect ={tmpA->dx,tmpA->dy,A_WIDTH/2,A_HEIGHT/2};
                if(SDL_HasIntersection(&missile_rect,&asteroide_rect)){
                    tmpA->energie=0;
                    tmpM->energie=0;
                    (*nv)->score+=10;
                }
            }
            else if (tmpA->taille==1){
                SDL_Rect asteroide_rect ={tmpA->dx,tmpA->dy,A_WIDTH,A_HEIGHT};
                if(SDL_HasIntersection(&missile_rect,&asteroide_rect)){
                    tmpA->energie=0;
                    tmpM->energie=0;
                    (*nv)->score+=5;
                }
            }
            tmpM=tmpM->suivant;
        }
        tmpM=*listeMissile;
        tmpA=tmpA->suivant;
    }
}



void collisionDetecteM(Missile** listeMissile){
    Missile* tmp = *listeMissile;
    while (tmp != NULL) {
        if (tmp->energie == 0) {
            enlever_missile(listeMissile, tmp);
        }
        tmp = tmp->suivant;
    }
}


