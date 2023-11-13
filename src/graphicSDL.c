#include "graphicSDL.h"
#include <string.h>

int init(SDL_Window** gWindow,SDL_Renderer** gRenderer, char* nameWindow)
{
	//Initialization flag
	int success = TRUE;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = FALSE;
	}
	else
	{
		//Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Warning: Linear texture filtering not enabled!" );
		}

		//Create window
		*gWindow = SDL_CreateWindow(nameWindow, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( *gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = FALSE;
		}
		else
		{
			//Create vsynced renderer for window
			*gRenderer = SDL_CreateRenderer( *gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
			if( *gRenderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = FALSE;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor( *gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
			}
		}
	}

	return success;
}


void close_app(SDL_Window** gWindow,SDL_Renderer** gRenderer)
{


	//Destroy window
	SDL_DestroyRenderer( *gRenderer );
	SDL_DestroyWindow(* gWindow );
	*gWindow = NULL;
	*gRenderer = NULL;

	//Quit SDL subsystems
	SDL_Quit();
}


// fcts de dessin




void dessine_ligne(SDL_Renderer *gRenderer,int x1, int y1, int x2, int y2,long c)
{
   int r = ( uint8_t )(c >> 16);
   int g = ( uint8_t )(c >>  8);
   int b = (  uint8_t  )c;
   SDL_SetRenderDrawColor( gRenderer,r,g,b,255);
   SDL_RenderDrawLine(gRenderer,x1,y1,x2,y2);
}



void ligneHorizontale(SDL_Renderer *gRenderer,int x, int y, int w, long c)
{
  SDL_Rect rect;
   int r = ( uint8_t )(c >> 16);
   int g = ( uint8_t )(c >>  8);
   int b = (  uint8_t  )c;

  rect.x = x;
  rect.y = y;
  rect.w = w;
  rect.h = 1;

   SDL_SetRenderDrawColor( gRenderer,r,g,b,255);
   SDL_RenderFillRect( gRenderer,&rect);
}


void dessine_disque(SDL_Renderer *gRenderer,int cx, int cy, int rayon, long coul)
{
  int d, y, x;

  d = 3 - (2 * rayon);
  x = 0;
  y = rayon;

  while (y >= x) {
    ligneHorizontale(gRenderer,cx - x, cy - y, 2 * x + 1, coul);
    ligneHorizontale(gRenderer,cx - x, cy + y, 2 * x + 1, coul);
    ligneHorizontale(gRenderer,cx - y, cy - x, 2 * y + 1, coul);
    ligneHorizontale(gRenderer,cx - y, cy + x, 2 * y + 1, coul);

    if (d < 0)
      d = d + (4 * x) + 6;
    else {
      d = d + 4 * (x - y) + 10;
      y--;
    }

    x++;
  }
}

double getCurrentTimeMS()
{
    double clo=clock();
    return 1000.*clo/CLOCKS_PER_SEC;
}

void dessine_fondtransparent(SDL_Renderer *gRenderer, int w, int h)
{
   SDL_Rect rect;
   rect.x=0;
   rect.y=0;
   rect.w=w;
   rect.h=h;
   SDL_SetRenderDrawColor( gRenderer,0,0,0,0);
   SDL_RenderFillRect( gRenderer,&rect);
}




SDL_Texture* creerTextureImage(SDL_Renderer *ecran,char *nom_file_bmp,int *size_w, int *size_h)
{
    SDL_Surface* image = SDL_LoadBMP(nom_file_bmp);
    if(!image)
    {
      printf("Erreur de chargement de l'image : %s",SDL_GetError());
      return NULL;
    }

    SDL_Texture* monImage = SDL_CreateTextureFromSurface(ecran,image);
    SDL_FreeSurface(image);

    if(NULL == monImage)
    {
        fprintf(stderr, "Erreur SDL_CreateTextureFromSurface : %s", SDL_GetError());
        return NULL;
    }
    SDL_QueryTexture(monImage, NULL, NULL,size_w,size_h); // on recup�re la taille de l'image


    return monImage;
}


void dessine_image(SDL_Renderer *ecran,SDL_Texture *imageTexture,int x, int y)
{
    SDL_Rect position;
    position.x = x;
    position.y = y;

    SDL_QueryTexture(imageTexture,NULL,NULL,&position.w,&position.h);// recupere la taille de l'image originelle
    SDL_RenderCopy(ecran, imageTexture, NULL, &position);
}

void dessine_image_redim(SDL_Renderer *ecran,SDL_Texture *imageTexture,int x, int y, int w_new,int h_new)
{
    SDL_Rect position;
    position.x = x;
    position.y = y;
    position.w=w_new;
    position.h=h_new;
    SDL_Rect pos_texture; // taille originale
    pos_texture.x=0;
    pos_texture.y=0;
    SDL_QueryTexture(imageTexture,NULL,NULL,&pos_texture.w,&pos_texture.h);// recupere la taille de l'image originelle
    SDL_RenderCopy(ecran, imageTexture,&pos_texture, &position);
}




int* digitPositionsFromScore(int score,int* numDigits){
    char scoreStr[10];
    sprintf(scoreStr, "%d", score);
    *numDigits = strlen(scoreStr);
    int* digitPositions=malloc(sizeof(int)*(10));
    for (int i = 0; i < *numDigits; i++) {
        digitPositions[i] = scoreStr[i] - '0';
    }
    return digitPositions;
}

