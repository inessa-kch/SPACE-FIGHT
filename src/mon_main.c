#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include "graphicSDL.h"
#include "asteroides.h"
#include "vaisseau.h"
#include "missile.h"

int nbAsteroides=1; //nombre d'asteroides genere au cours du temps au debut du jeu
int prevScore=0; //score precedent

int main()
{
    /* INITIALISATION */
    srand(time(NULL));
    SDL_Window* window=NULL;
    SDL_Renderer* ecran=NULL;

    init(&window,&ecran,"Asteroides>.<");

    //init asteroide
    Asteroide* listeAsteroide = NULL;


    //init vaisseau
    Vaisseau* nv=CreerVaisseau();
    SDL_Texture* textureVaisseau=CreerTextureVaisseau(ecran,V_WIDTH,V_HEIGHT,White);

    //init missile
    Missile* listeMissile = NULL;
    SDL_Texture* textureMissile=CreerTextureMissile(ecran,M_RAYON,M_WIDTH,M_HEIGHT,White);

    //init background
    SDL_Rect BackGround;
    SDL_Texture* BackgroundTexture=creerTextureImage(ecran,"./img/background.bmp",&BackGround.w,&BackGround.h);
    BackGround.x=0;
    BackGround.y=0;

    //init score
    SDL_Texture* score;
    SDL_Rect rect_scoreImage;
    score = creerTextureImage(ecran,"./img/score/score.bmp",&rect_scoreImage.w,&rect_scoreImage.h);
    rect_scoreImage.x=0;
    rect_scoreImage.y=0;

    SDL_Texture* scoreTexture[10];
    SDL_Rect rect_score;
    scoreTexture[1] = creerTextureImage(ecran,"./img/score/num1.bmp",&rect_score.w,&rect_score.h);
    scoreTexture[2] = creerTextureImage(ecran,"./img/score/num2.bmp",&rect_score.w,&rect_score.h);
    scoreTexture[3] = creerTextureImage(ecran,"./img/score/num3.bmp",&rect_score.w,&rect_score.h);
    scoreTexture[4] = creerTextureImage(ecran,"./img/score/num4.bmp",&rect_score.w,&rect_score.h);
    scoreTexture[5] = creerTextureImage(ecran,"./img/score/num5.bmp",&rect_score.w,&rect_score.h);
    scoreTexture[6] = creerTextureImage(ecran,"./img/score/num6.bmp",&rect_score.w,&rect_score.h);
    scoreTexture[7] = creerTextureImage(ecran,"./img/score/num7.bmp",&rect_score.w,&rect_score.h);
    scoreTexture[8] = creerTextureImage(ecran,"./img/score/num8.bmp",&rect_score.w,&rect_score.h);
    scoreTexture[9] = creerTextureImage(ecran,"./img/score/num9.bmp",&rect_score.w,&rect_score.h);
    scoreTexture[0] = creerTextureImage(ecran,"./img/score/num0.bmp",&rect_score.w,&rect_score.h);
    rect_score.x=rect_scoreImage.w+20;
    rect_score.y=0;


    //init healthbar
    SDL_Texture* healthbarTexture[5];
    SDL_Rect rect_healthbar;
    healthbarTexture[0] = creerTextureImage(ecran,"./img/healthbar/healthbar0.bmp",&rect_healthbar.w,&rect_healthbar.h);
    healthbarTexture[1] = creerTextureImage(ecran,"./img/healthbar/healthbar1.bmp",&rect_healthbar.w,&rect_healthbar.h);
    healthbarTexture[2] = creerTextureImage(ecran,"./img/healthbar/healthbar2.bmp",&rect_healthbar.w,&rect_healthbar.h);
    healthbarTexture[3] = creerTextureImage(ecran,"./img/healthbar/healthbar3.bmp",&rect_healthbar.w,&rect_healthbar.h);
    healthbarTexture[4] = creerTextureImage(ecran,"./img/healthbar/healthbar4.bmp",&rect_healthbar.w,&rect_healthbar.h);
    healthbarTexture[5] = creerTextureImage(ecran,"./img/healthbar/healthbar5.bmp",&rect_healthbar.w,&rect_healthbar.h);
    rect_healthbar.x=0;
    rect_healthbar.y=rect_score.h+20;

    //init ecran d'ouverture
    SDL_Rect rect_ecranOuverture;
    SDL_Texture* ecranOuvertureTexture=creerTextureImage(ecran,"./img/openingScreen2.bmp",&rect_ecranOuverture.w,&rect_ecranOuverture.h);
    rect_ecranOuverture.x=0;
    rect_ecranOuverture.y=0;


    //init gameover ecran
    SDL_Rect rect_ecranGameOver;
    SDL_Texture* ecranGameOverTexture=creerTextureImage(ecran,"./img/gameOver2.bmp",&rect_ecranGameOver.w,&rect_ecranGameOver.h);
    rect_ecranGameOver.x=0;
    rect_ecranGameOver.y=0;
  
    //variables globales
    int nb_frames=0;
    double previous = getCurrentTimeMS();
    double lag = 0.0;
    int quit = FALSE;
    int asteroide_temps=0;
    int scene = 0;
    //scene 0: ecran d'ouverture
    //scene 1: jeu
    //scene 2: game over
    int Highscore = 0;

    
    /*lecture du Highscore a partir depuis le fichier Highscore.txt*/
    lectureHighscore(&Highscore);

    /* BOUCLE DU JEU */
    while (!quit){
        SDL_Event e;
        double current = getCurrentTimeMS();
        double elapsed = current - previous;
        previous = current;
        lag += elapsed;

        if (nv->health==0){scene=2;}

        while (SDL_PollEvent(&e) != 0)
        { 
          if (e.type == SDL_KEYDOWN){
            switch( e.key.keysym.sym ){
              case SDLK_ESCAPE:
                  quit = TRUE;
                  break;
              case SDLK_RETURN:
                if (scene==0){
                  scene=1;
                }
                if (scene==2){
                  reinitialiserVaisseau(nv);
                  nbAsteroides = 1;
                  prevScore =  0;
                  effacerListeAsteroide(&listeAsteroide);
                  effacerListeMissile(&listeMissile);
                  scene=1;
                }
                break;
            }
          }
          if (scene==1){
          bougerMissile(nv,&listeMissile,e);
          bouger_vaisseau(&nv,e,&quit);}
        }
            
        if (nv->vaisseau_passif > 0) {
            nv->vaisseau_passif -= elapsed;
        }

        while (lag >= MS_PER_UPDATE){
            if (scene==1){
              //Mise a jour des positions
              miseAjour_asteroide_temps(ecran,&elapsed,&asteroide_temps,&listeAsteroide,nv->score,&prevScore,&nbAsteroides);
              deplacerVaisseau(&nv);
              deplacerMissile(&listeMissile);
              deplacerAsteroide(&listeAsteroide);
              //gestion des collisions
              collisionAsM(&listeAsteroide,&listeMissile,&nv);
              collisionDetecteM(&listeMissile);
              collisionDetecteA(&listeAsteroide);
              if (collisionAsV(&listeAsteroide,nv)){
                  repositionnerVaisseaau(&nv);
              }
            }
              lag -= MS_PER_UPDATE;
          }

        /* RENDER */
        SDL_SetRenderDrawColor( ecran, 0xFF, 0xFF, 0xFF, 0xFF );
        //nettoie l'ecran
        SDL_RenderClear(ecran);

        if (scene==0){
            dessine_image_redim(ecran,ecranOuvertureTexture,rect_ecranOuverture.x,rect_ecranOuverture.y,rect_ecranOuverture.w,rect_ecranOuverture.h);
        }

        else if (scene==1){
        dessine_image_redim(ecran,BackgroundTexture,BackGround.x,BackGround.y,BackGround.w,BackGround.h);

        dessine_image_redim(ecran,score,rect_scoreImage.x,rect_scoreImage.y,rect_scoreImage.w,rect_scoreImage.h);

        int numDigits;
        int* digitPositions = digitPositionsFromScore(nv->score,&numDigits);        
        for (int i = 0; i < numDigits; i++) {
          dessine_image_redim(ecran,scoreTexture[digitPositions[i]],rect_score.x+(i*rect_score.w),rect_score.y,rect_score.w,rect_score.h);
        }

        dessine_image_redim(ecran,healthbarTexture[nv->health],rect_healthbar.x,rect_healthbar.y,rect_healthbar.w,rect_healthbar.h);

        dessineAsteroide(ecran,&listeAsteroide,A_WIDTH,A_HEIGHT); 

        dessine_vaisseau(ecran,&nv,textureVaisseau);

        dessineMissile(ecran,textureMissile,listeMissile,M_WIDTH,M_HEIGHT);
        }

        else if (scene == 2){
        if (nv->score >= Highscore){
          Highscore = nv->score;
        }

        dessine_image_redim(ecran,ecranGameOverTexture,rect_ecranGameOver.x,rect_ecranGameOver.y,rect_ecranGameOver.w,rect_ecranGameOver.h); 
        
        //dessine score
        int numDigitsScore;
        int* digitPositionsScore = digitPositionsFromScore(nv->score,&numDigitsScore);
        for (int i = 0; i < numDigitsScore; i++) {
        dessine_image_redim(ecran,scoreTexture[digitPositionsScore[i]],rect_score.x+(i*rect_score.w)+400,rect_score.y+290,rect_score.w,rect_score.h);
        }

        //dessine highscore
        int numDigitsHighScore;
        int* digitPositionsHighScore = digitPositionsFromScore(Highscore,&numDigitsHighScore);

        for (int i = 0; i < numDigitsHighScore; i++) {
        dessine_image_redim(ecran,scoreTexture[digitPositionsHighScore[i]],rect_score.x+(i*rect_score.w)+550,rect_score.y+390,rect_score.w,rect_score.h);
        }
        }


        //Mise a jour de l'ecran
        SDL_RenderPresent(ecran);
        nb_frames=nb_frames+1;
  }
    /*sauvegarde du highscore*/
    sauvegardeHighscore(Highscore);

    /* DESTRUCTION DES DIFFERENTS ELEMENTS DU JEU*/
    //destruction des listes et structures
    effacerListeMissile(&listeMissile);
    effacerVaisseau(nv);
    effacerListeAsteroide(&listeAsteroide);

    // destruction des textures
    if (BackgroundTexture) {SDL_DestroyTexture(BackgroundTexture);BackgroundTexture=NULL;}
    suppressionTextureAsteroide(&listeAsteroide);
    if (textureVaisseau) {SDL_DestroyTexture(textureVaisseau);textureVaisseau=NULL;}
    if (textureMissile) {SDL_DestroyTexture(textureMissile);textureMissile=NULL;}
    for (int i=0; i<3; i++) {
      if (healthbarTexture[i]) {SDL_DestroyTexture(healthbarTexture[i]);healthbarTexture[i]=NULL;}
    }
    for (int j=0; j<10; j++) {
      if (scoreTexture[j]) {SDL_DestroyTexture(scoreTexture[j]);scoreTexture[j]=NULL;}
    }
    if (ecranOuvertureTexture) {SDL_DestroyTexture(ecranOuvertureTexture);ecranOuvertureTexture=NULL;}
    if (ecranGameOverTexture) {SDL_DestroyTexture(ecranGameOverTexture);ecranGameOverTexture=NULL;}
    if (score) {SDL_DestroyTexture(score);score=NULL;}

    // ferme ecran et fenetre
    close_app(&window,&ecran);
    SDL_Quit();

  return 0;
}




