#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <math.h>
#include "vaisseau.h"
#include "graphicSDL.h"
#include <assert.h>



Vaisseau* CreerVaisseau(){
    Vaisseau* nv=malloc(sizeof(Vaisseau));
    assert(nv);
    nv->dx=SCREEN_WIDTH/2;
    nv->dy=SCREEN_HEIGHT/2;
    nv->dir[0]=0;
    nv->dir[1]=0;
    nv->angle=0;
    nv->energie=1;
    nv->health=5;
    nv->score=0;
    nv->vaisseau_passif=0;
    return nv;
}


void reinitialiserVaisseau(Vaisseau* nv){
    nv->dx=SCREEN_WIDTH/2;
    nv->dy=SCREEN_HEIGHT/2;
    nv->dir[0]=0;
    nv->dir[1]=0;
    nv->angle=0;
    nv->energie=1;
    nv->health=5;
    nv->score=0;
    nv->vaisseau_passif=0;
}


void effacerVaisseau(Vaisseau* nv){
    if (nv!=NULL){
        free(nv);
    }
    else return;
}



SDL_Texture *CreerTextureVaisseau(SDL_Renderer*ecran,int w,int h,long couleur){
  SDL_Texture* textureVaisseau=NULL;
  textureVaisseau=SDL_CreateTexture(ecran,SDL_PIXELFORMAT_RGBA8888,SDL_TEXTUREACCESS_TARGET,w,h);
  SDL_RenderClear(ecran); // nettoie ecran
  SDL_SetTextureBlendMode(textureVaisseau, SDL_BLENDMODE_BLEND); 
  SDL_SetRenderTarget(ecran, textureVaisseau); //on modifie la texture
  // dessine fond transparent
  dessine_fondtransparent(ecran,w,h);

  dessine_ligne(ecran,SCREEN_WIDTH/53,0,0,SCREEN_HEIGHT/15,couleur);
  dessine_ligne(ecran,SCREEN_WIDTH/53,0,SCREEN_WIDTH/27,SCREEN_HEIGHT/15,couleur);
  dessine_ligne(ecran,SCREEN_WIDTH/102,SCREEN_HEIGHT/30,SCREEN_WIDTH/35,SCREEN_HEIGHT/30,couleur);

  SDL_SetRenderTarget(ecran, NULL);

  return textureVaisseau;
}


void dessine_vaisseau(SDL_Renderer *ecran, Vaisseau** nv,SDL_Texture *textureVaisseau)
{
    SDL_Rect rect_vaisseau;

    rect_vaisseau.x = (*nv)->dx;
    rect_vaisseau.y = (*nv)->dy;
    SDL_QueryTexture(textureVaisseau, NULL, NULL, &rect_vaisseau.w, &rect_vaisseau.h);
    rect_vaisseau.x -= (rect_vaisseau.w / 2);
    rect_vaisseau.y -= (rect_vaisseau.h / 2);
    SDL_RenderCopyEx(ecran, textureVaisseau, NULL, &rect_vaisseau,(*nv)->angle,NULL, SDL_FLIP_NONE);
}

   


void bouger_vaisseau(Vaisseau** nv,SDL_Event e,int* quit){
    switch(e.type){
        case SDL_QUIT:
            *quit = TRUE;
            break;
        case SDL_KEYDOWN:
            switch(e.key.keysym.sym){
                case SDLK_w:
                    (*nv)->dir[0] += VITESSE_DEPLACEMENT*sin(((*nv)->angle*PI)/180.0);
                    (*nv)->dir[1] += -VITESSE_DEPLACEMENT*cos(((*nv)->angle*PI)/180.0);
                    break;
                case SDLK_s:
                    (*nv)->dir[0] += -VITESSE_DEPLACEMENT*sin(((*nv)->angle*PI)/180.0);
                    (*nv)->dir[1] += VITESSE_DEPLACEMENT*cos(((*nv)->angle*PI)/180.0);
                    break;
                case SDLK_a:
                    (*nv)->angle -= VITESSE_ROTATION;
                    break;
                case SDLK_d:
                    (*nv)->angle += VITESSE_ROTATION;
                    break;
            }
            break;
    }
    /*on diminue la vitesse du vaisseau au cas ou celle-ci depasse la vitesse maximale*/
    float vitesse= sqrt(((*nv)->dir[0]*(*nv)->dir[0])+((*nv)->dir[1]*(*nv)->dir[1]));
    if (vitesse > VITESSE_MAX){
        (*nv)->dir[0] *= VITESSE_MAX/vitesse;
        (*nv)->dir[1] *= VITESSE_MAX/vitesse;
    }
    if ((*nv)->angle > 360){
        (*nv)->angle-=360;
    }
    if ((*nv)->angle < -360){
        (*nv)->angle+=360;
    }
}


void deplacerVaisseau(Vaisseau** nv){
    /*si le vaisseau est en mode passif on le bouge pas*/
    /*if ((*nv)->vaisseau_passif > 0) {
        return;
    }*/
     /*on applique du frottement sur le vaisseau*/
    (*nv)->dir[0] *= FROTTEMENT;
    (*nv)->dir[1] *= FROTTEMENT;
    (*nv)->dx+=(*nv)->dir[0];
    (*nv)->dy+=(*nv)->dir[1];
    /*le vaisseau passe a l'autre coté de l'ecran s'il en sort*/
    (*nv)->dx = ((*nv)->dx<0?SCREEN_WIDTH-V_WIDTH/2:((*nv)->dx>=SCREEN_WIDTH-V_WIDTH/2? 0 : (*nv)->dx));
    (*nv)->dy = ((*nv)->dy<0?SCREEN_HEIGHT-V_HEIGHT/2:((*nv)->dy>=SCREEN_HEIGHT-V_HEIGHT/2? 0 : (*nv)->dy));
}



int collisionAsV(Asteroide** listeAsteroide, Vaisseau* nv){
    /*si le vaisseau est en mode passif on ne detecte pas la collision*/
    if (nv->vaisseau_passif > 0) {
        return 0;
    }
    int res=0;
    SDL_Rect rect_vaisseau={nv->dx,nv->dy,V_WIDTH,V_HEIGHT};
    Asteroide* tmpA=*listeAsteroide;
    while(tmpA!=NULL){
        if (tmpA->taille==0){
            SDL_Rect rect_asteroide={tmpA->dx,tmpA->dy,A_WIDTH/2,A_HEIGHT/2};
            if (SDL_HasIntersection(&rect_vaisseau,&rect_asteroide)){
                tmpA->energie=0;
                nv->energie=0;
                nv->vaisseau_passif=TEMPS_PASSIF;
                res=1;
            }
        }
        else if (tmpA->taille==1){
            SDL_Rect rect_asteroide={tmpA->dx,tmpA->dy,A_WIDTH,A_HEIGHT};
            if (SDL_HasIntersection(&rect_vaisseau,&rect_asteroide)){
                tmpA->energie=0;
                nv->energie=0;
                nv->vaisseau_passif=TEMPS_PASSIF;
                res=1;
            }
        }
        tmpA=tmpA->suivant;
    }
    return res;
}




void repositionnerVaisseaau(Vaisseau** nv){
    if((*nv)->energie==0){
        (*nv)->dx=SCREEN_WIDTH/2;
        (*nv)->dy=SCREEN_HEIGHT/2;
        (*nv)->dir[0]=0;
        (*nv)->dir[1]=0;
        (*nv)->angle=0;
        (*nv)->health--;
    }
}



void lectureHighscore(int* highscore){
    FILE* fichier = NULL;
    fichier = fopen("highscore.txt", "r");
    if (fichier != NULL){
        fscanf(fichier, "%d", highscore);
        fclose(fichier);
    }
    else{
        printf("Impossible d'ouvrir le fichier highscore.txt");
    }
}

void sauvegardeHighscore(int highscore){
    FILE* fichier = NULL;
    fichier = fopen("highscore.txt", "w");
    if (fichier != NULL){
        fprintf(fichier, "%d", highscore);
        fclose(fichier);
    }
    else{
        printf("Impossible d'ouvrir le fichier highscore.txt");
    }
}
