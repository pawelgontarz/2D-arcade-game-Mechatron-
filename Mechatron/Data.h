//
//  Data.h
//  Crysis 5
//
//  Created by Mac Paweł on 14.12.2016.
//  Copyright © 2016 Macbook Pawel. All rights reserved.
//

#ifndef Data_h
#define Data_h


#endif /* Data_h */


#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define GRAVITY 0.40f

typedef struct
{
    int x,y;
} Option;

typedef struct
{
    Option options [5];
    SDL_Texture *menuOptions[4];
} Menu;

typedef struct
{
    float x,y;
    float dx,dy;
    short hp;
    short maxhp;
    short exp;
    short maxexp;
    short lvl;
    int onLedge;
    int mode, spaceForModeSwitch;
    int damage, rateOfFire;

    int animFrame, facingLeft; // numer obrazka z animacji i patrzenie lewo / prawo
} Man;

typedef struct
{
    int w,h;
    float x,y;
    float dx,dy;
    short hp;
    short maxhp;
    int isDead;
    char *name;

    int animFrame, facingLeft;
} Enemy;

typedef struct
{
    float x,y,dx,dy;
} Bullet;

//pociski
Bullet *bullets[1000] = {NULL};
SDL_Texture * bulletTexture;

typedef struct
{
    int x,y,w,h;
    char *name;
} Brick;

typedef struct
{
    //menu
    Menu menu;

    //Przewijanie ekranu
    float scrollX;

    //gracze
    Man man;

    //przeciwnicy
    Enemy enemies[10];

    //cegielki
    Brick bricks[300]; // od 0 do 159 to są wspolrzędne obrazkow ziemia, od 160 do 290 obrazkow wall

    //obrazki
    SDL_Texture *lvlup;
    SDL_Texture *newmap;
    SDL_Texture *tutorial;
    SDL_Texture *manFrames[14];
    SDL_Texture *ziemia;
    SDL_Texture *wall;
    SDL_Texture *fireFrames[3];
    SDL_Texture *robotFrames[4];
    SDL_Texture *shooterFrames[4];

    //dzwieki
    Mix_Chunk *shot_sound;

    int fireFrame;

    int time;
    int lvlUpTime, newMapTime;
    int mapa;

    SDL_Window *window;                 //Deklaracja okna
    SDL_Renderer *renderer;             //Deklaracja renderera

} GameState;

void loadMenu(GameState *game)
{
    SDL_Surface *surface = NULL;

    surface = IMG_Load("Pictures/play.bmp");
    if(surface == NULL)
    {
        printf("Cannot find play.bmp!\n\n");
        SDL_Quit();
        exit(1);
    }

    game->menu.menuOptions[0] = SDL_CreateTextureFromSurface(game->renderer, surface);

    surface = IMG_Load("Pictures/save.bmp");
    if(surface == NULL)
    {
        printf("Cannot find save.bmp!\n\n");
        SDL_Quit();
        exit(1);
    }

    game->menu.menuOptions[1] = SDL_CreateTextureFromSurface(game->renderer, surface);

    surface = IMG_Load("Pictures/load.bmp");
    if(surface == NULL)
    {
        printf("Cannot find load.bmp!\n\n");
        SDL_Quit();
        exit(1);
    }

    game->menu.menuOptions[2] = SDL_CreateTextureFromSurface(game->renderer, surface);

    surface = IMG_Load("Pictures/quit.bmp");
    if(surface == NULL)
    {
        printf("Cannot find quit.bmp!\n\n");
        SDL_Quit();
        exit(1);
    }

    game->menu.menuOptions[3] = SDL_CreateTextureFromSurface(game->renderer, surface);
    SDL_FreeSurface(surface);

    int i=0;
    for (i=0; i<4; i++)
    {
        game->menu.options[i].x = 300;
        game->menu.options[i].y = 20+i*170;
    }

    game->menu.options[4].x = 280;
    game->menu.options[4].y = 0;
}

void menuRender(GameState *game)
{
    // ustawienie koloru rendera na bialy
    SDL_SetRenderDrawColor(game->renderer, 255, 255, 255, 255);

    // czyszczenie rendera na bialo
    SDL_RenderClear(game->renderer);

    // wybor koloru do rysowania
    SDL_SetRenderDrawColor(game->renderer, 255, 0, 0, 255);


    SDL_Rect optionRect = {game->menu.options[4].x, game->menu.options[4].y, 640, 190};
    SDL_RenderFillRect(game->renderer, &optionRect);

    int i=0;
    for (i=0; i<4; i++)
    {
        SDL_Rect optionRect = {game->menu.options[i].x, game->menu.options[i].y, 600, 150};
        SDL_RenderCopy(game->renderer, game->menu.menuOptions[i], NULL, &optionRect);
    }

    // wyswietlanie stworzonego rysunku
    SDL_RenderPresent(game->renderer);
}

void quitMenu(GameState *game)
{
    int i=0;
    for (i=0; i<4; i++)
    {
        SDL_DestroyTexture(game->menu.menuOptions[i]);
    }
}

int menu(SDL_Window *window, GameState *game)
{
    while(!0)
    {
        SDL_Event event;

        while(SDL_PollEvent(&event))
        {
            switch(event.type)
            {
                case SDL_WINDOWEVENT_CLOSE:
                {
                    if(window)
                    {
                        SDL_DestroyWindow(window);
                        window = NULL;
                        return 4;
                    }
                }
                    break;
                case SDL_KEYDOWN:
                {
                    switch(event.key.keysym.sym)
                    {
                        case SDLK_ESCAPE:
                            return 4;
                            break;
                        case SDLK_UP:
                            if(game->menu.options[4].y == 0)
                            {
                                game->menu.options[4].y = 510;
                            }else game->menu.options[4].y -= 170;
                            break;
                        case SDLK_DOWN:
                            if(game->menu.options[4].y == 510)
                            {
                                game->menu.options[4].y = 0;
                            }else game->menu.options[4].y += 170;
                            break;
                        case SDLK_RETURN:
                            switch(game->menu.options[4].y)
                        {
                            case 0: return 1;
                            case 170: return 2;
                            case 340: return 3;
                            case 510: return 4;
                        }
                            break;
                    }
                }
                    break;
                case SDL_QUIT:
                    return 4;
                    break;
            }
        }

        menuRender(game);
    }
}

void loadPictures(GameState *game)
{
    //Tworzenie okna gry przy uzyciu nastepujacych ustawien:
    game->window = SDL_CreateWindow("Crysis5",                    // tytul okna
                                    SDL_WINDOWPOS_UNDEFINED,      // x wspolrzedna okna
                                    SDL_WINDOWPOS_UNDEFINED,      // y wspolrzedna okna
                                    1200,                         // szerokosc, w pikselach
                                    700,                          // wysokosc, w pikselach
                                    0);                           //flagi

    game->renderer = SDL_CreateRenderer(game->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    SDL_Surface *surface = NULL;

    // wczytywanie obrazków i tworzenie z nich tekstur
    surface = IMG_Load("Pictures/lvlUp.bmp");
    if(surface == NULL)
    {
        printf("Cannot find lvlUp!\n\n");
        SDL_Quit();
        exit(1);
    }else
    {
        //jesli wsrzystko poszlo dobrze
        if( surface != NULL )
        {
            //wybor koloru, ktory ma byc przezroczysty
            Uint32 colorkey = SDL_MapRGB( surface->format, 255, 255, 255 );

            SDL_SetColorKey( surface, 1, colorkey );
        }
    }

    game->lvlup = SDL_CreateTextureFromSurface(game->renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("Pictures/newmap.bmp");
    if(surface == NULL)
    {
        printf("Cannot find newmap!\n\n");
        SDL_Quit();
        exit(1);
    }else
    {
        //jesli wsrzystko poszlo dobrze
        if( surface != NULL )
        {
            //wybor koloru, ktory ma byc przezroczysty
            Uint32 colorkey = SDL_MapRGB( surface->format, 255, 255, 255 );

            SDL_SetColorKey( surface, 1, colorkey );
        }
    }

    game->newmap = SDL_CreateTextureFromSurface(game->renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("Pictures/tutorial.bmp");
    if(surface == NULL)
    {
        printf("Cannot find tutorial!\n\n");
        SDL_Quit();
        exit(1);
    }else
    {
        //jesli wsrzystko poszlo dobrze
        if( surface != NULL )
        {
            //wybor koloru, ktory ma byc przezroczysty
            Uint32 colorkey = SDL_MapRGB( surface->format, 255, 255, 255 );

            SDL_SetColorKey( surface, 1, colorkey );
        }
    }

    game->tutorial = SDL_CreateTextureFromSurface(game->renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("Pictures/ziemia.bmp");
    if(surface == NULL)
    {
        printf("Cannot find ziemia.bmp!\n\n");
        SDL_Quit();
        exit(1);
    }else
    {
        //jesli wsrzystko poszlo dobrze
        if( surface != NULL )
        {
            //wybor koloru, ktory ma byc przezroczysty
            Uint32 colorkey = SDL_MapRGB( surface->format, 255, 255, 255 );

            SDL_SetColorKey( surface, 1, colorkey );
        }
    }

    game->ziemia = SDL_CreateTextureFromSurface(game->renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("Pictures/wall.bmp");
    if(surface == NULL)
    {
        printf("Cannot find wall.bmp!\n\n");
        SDL_Quit();
        exit(1);
    }else
    {
        //jesli wsrzystko poszlo dobrze
        if( surface != NULL )
        {
            //wybor koloru, ktory ma byc przezroczysty
            Uint32 colorkey = SDL_MapRGB( surface->format, 255, 255, 255 );

            SDL_SetColorKey( surface, 1, colorkey );
        }
    }

    game->wall = SDL_CreateTextureFromSurface(game->renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("Pictures/fire_0.bmp");
    if(surface == NULL)
    {
        printf("Cannot find fire_0.bmp!\n\n");
        SDL_Quit();
        exit(1);
    }else
    {
        //jesli wsrzystko poszlo dobrze
        if( surface != NULL )
        {
            //wybor koloru, ktory ma byc przezroczysty
            Uint32 colorkey = SDL_MapRGB( surface->format, 255, 255, 255 );

            SDL_SetColorKey( surface, 1, colorkey );
        }
    }

    game->fireFrames[0] = SDL_CreateTextureFromSurface(game->renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("Pictures/fire_1.bmp");
    if(surface == NULL)
    {
        printf("Cannot find fire_1.bmp!\n\n");
        SDL_Quit();
        exit(1);
    }else
    {
        //jesli wsrzystko poszlo dobrze
        if( surface != NULL )
        {
            //wybor koloru, ktory ma byc przezroczysty
            Uint32 colorkey = SDL_MapRGB( surface->format, 255, 255, 255 );

            SDL_SetColorKey( surface, 1, colorkey );
        }
    }

    game->fireFrames[1] = SDL_CreateTextureFromSurface(game->renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("Pictures/fire_2.bmp");
    if(surface == NULL)
    {
        printf("Cannot find fire_2.bmp!\n\n");
        SDL_Quit();
        exit(1);
    }else
    {
        //jesli wsrzystko poszlo dobrze
        if( surface != NULL )
        {
            //wybor koloru, ktory ma byc przezroczysty
            Uint32 colorkey = SDL_MapRGB( surface->format, 255, 255, 255 );

            SDL_SetColorKey( surface, 1, colorkey );
        }
    }

    game->fireFrames[2] = SDL_CreateTextureFromSurface(game->renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("Pictures/man_0.bmp");
    if(surface == NULL)
    {
        printf("Cannot find man_0.bmp!\n\n");
        SDL_Quit();
        exit(1);
    }else
    {
        //jesli wsrzystko poszlo dobrze
        if( surface != NULL )
        {
            //wybor koloru, ktory ma byc przezroczysty
            Uint32 colorkey = SDL_MapRGB( surface->format, 255, 255, 255 );

            SDL_SetColorKey( surface, 1, colorkey );
        }
    }

    game->manFrames[0] = SDL_CreateTextureFromSurface(game->renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("Pictures/man_1.bmp");
    if(surface == NULL)
    {
        printf("Cannot find man_1.bmp!\n\n");
        SDL_Quit();
        exit(1);
    }else
    {
        //jesli wsrzystko poszlo dobrze
        if( surface != NULL )
        {
            //wybor koloru, ktory ma byc przezroczysty
            Uint32 colorkey = SDL_MapRGB( surface->format, 255, 255, 255 );

            SDL_SetColorKey( surface, 1, colorkey );
        }
    }

    game->manFrames[1] = SDL_CreateTextureFromSurface(game->renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("Pictures/man_2.bmp");
    if(surface == NULL)
    {
        printf("Cannot find man_2.bmp!\n\n");
        SDL_Quit();
        exit(1);
    }else
    {
        //jesli wsrzystko poszlo dobrze
        if( surface != NULL )
        {
            //wybor koloru, ktory ma byc przezroczysty
            Uint32 colorkey = SDL_MapRGB( surface->format, 255, 255, 255 );

            SDL_SetColorKey( surface, 1, colorkey );
        }
    }

    game->manFrames[2] = SDL_CreateTextureFromSurface(game->renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("Pictures/man_3.bmp");
    if(surface == NULL)
    {
        printf("Cannot find man_3.bmp!\n\n");
        SDL_Quit();
        exit(1);
    }else
    {
        //jesli wsrzystko poszlo dobrze
        if( surface != NULL )
        {
            //wybor koloru, ktory ma byc przezroczysty
            Uint32 colorkey = SDL_MapRGB( surface->format, 255, 255, 255 );

            SDL_SetColorKey( surface, 1, colorkey );
        }
    }

    game->manFrames[3] = SDL_CreateTextureFromSurface(game->renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("Pictures/man_4.bmp");
    if(surface == NULL)
    {
        printf("Cannot find man_4.bmp!\n\n");
        SDL_Quit();
        exit(1);
    }else
    {
        //jesli wsrzystko poszlo dobrze
        if( surface != NULL )
        {
            //wybor koloru, ktory ma byc przezroczysty
            Uint32 colorkey = SDL_MapRGB( surface->format, 255, 255, 255 );

            SDL_SetColorKey( surface, 1, colorkey );
        }
    }

    game->manFrames[4] = SDL_CreateTextureFromSurface(game->renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("Pictures/man_5.bmp");
    if(surface == NULL)
    {
        printf("Cannot find man_5.bmp!\n\n");
        SDL_Quit();
        exit(1);
    }else
    {
        //jesli wsrzystko poszlo dobrze
        if( surface != NULL )
        {
            //wybor koloru, ktory ma byc przezroczysty
            Uint32 colorkey = SDL_MapRGB( surface->format, 255, 255, 255 );

            SDL_SetColorKey( surface, 1, colorkey );
        }
    }

    game->manFrames[5] = SDL_CreateTextureFromSurface(game->renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("Pictures/man_6.bmp");
    if(surface == NULL)
    {
        printf("Cannot find man_6.bmp!\n\n");
        SDL_Quit();
        exit(1);
    }else
    {
        //jesli wsrzystko poszlo dobrze
        if( surface != NULL )
        {
            //wybor koloru, ktory ma byc przezroczysty
            Uint32 colorkey = SDL_MapRGB( surface->format, 255, 255, 255 );

            SDL_SetColorKey( surface, 1, colorkey );
        }
    }

    game->manFrames[6] = SDL_CreateTextureFromSurface(game->renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("Pictures/man_7.bmp");
    if(surface == NULL)
    {
        printf("Cannot find man_7.bmp!\n\n");
        SDL_Quit();
        exit(1);
    }else
    {
        //jesli wsrzystko poszlo dobrze
        if( surface != NULL )
        {
            //wybor koloru, ktory ma byc przezroczysty
            Uint32 colorkey = SDL_MapRGB( surface->format, 255, 255, 255 );

            SDL_SetColorKey( surface, 1, colorkey );
        }
    }

    game->manFrames[7] = SDL_CreateTextureFromSurface(game->renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("Pictures/man_u.bmp");
    if(surface == NULL)
    {
        printf("Cannot find man_u.bmp!\n\n");
        SDL_Quit();
        exit(1);
    }else
    {
        //jesli wsrzystko poszlo dobrze
        if( surface != NULL )
        {
            //wybor koloru, ktory ma byc przezroczysty
            Uint32 colorkey = SDL_MapRGB( surface->format, 255, 255, 255 );

            SDL_SetColorKey( surface, 1, colorkey );
        }
    }

    game->manFrames[8] = SDL_CreateTextureFromSurface(game->renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("Pictures/man_d.bmp");
    if(surface == NULL)
    {
        printf("Cannot find man_d.bmp!\n\n");
        SDL_Quit();
        exit(1);
    }else
    {
        //jesli wsrzystko poszlo dobrze
        if( surface != NULL )
        {
            //wybor koloru, ktory ma byc przezroczysty
            Uint32 colorkey = SDL_MapRGB( surface->format, 255, 255, 255 );

            SDL_SetColorKey( surface, 1, colorkey );
        }
    }

    game->manFrames[9] = SDL_CreateTextureFromSurface(game->renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("Pictures/man_d_0.bmp");
    if(surface == NULL)
    {
        printf("Cannot find man_d_0.bmp!\n\n");
        SDL_Quit();
        exit(1);
    }else
    {
        //jesli wsrzystko poszlo dobrze
        if( surface != NULL )
        {
            //wybor koloru, ktory ma byc przezroczysty
            Uint32 colorkey = SDL_MapRGB( surface->format, 255, 255, 255 );

            SDL_SetColorKey( surface, 1, colorkey );
        }
    }

    game->manFrames[10] = SDL_CreateTextureFromSurface(game->renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("Pictures/man_d_1.bmp");
    if(surface == NULL)
    {
        printf("Cannot find man_d_1.bmp!\n\n");
        SDL_Quit();
        exit(1);
    }else
    {
        //jesli wsrzystko poszlo dobrze
        if( surface != NULL )
        {
            //wybor koloru, ktory ma byc przezroczysty
            Uint32 colorkey = SDL_MapRGB( surface->format, 255, 255, 255 );

            SDL_SetColorKey( surface, 1, colorkey );
        }
    }

    game->manFrames[11] = SDL_CreateTextureFromSurface(game->renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("Pictures/man_d_2.bmp");
    if(surface == NULL)
    {
        printf("Cannot find man_d_2.bmp!\n\n");
        SDL_Quit();
        exit(1);
    }else
    {
        //jesli wsrzystko poszlo dobrze
        if( surface != NULL )
        {
            //wybor koloru, ktory ma byc przezroczysty
            Uint32 colorkey = SDL_MapRGB( surface->format, 255, 255, 255 );

            SDL_SetColorKey( surface, 1, colorkey );
        }
    }

    game->manFrames[12] = SDL_CreateTextureFromSurface(game->renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("Pictures/man_d_3.bmp");
    if(surface == NULL)
    {
        printf("Cannot find man_d_3.bmp!\n\n");
        SDL_Quit();
        exit(1);
    }else
    {
        //jesli wsrzystko poszlo dobrze
        if( surface != NULL )
        {
            //wybor koloru, ktory ma byc przezroczysty
            Uint32 colorkey = SDL_MapRGB( surface->format, 255, 255, 255 );

            SDL_SetColorKey( surface, 1, colorkey );
        }
    }

    game->manFrames[13] = SDL_CreateTextureFromSurface(game->renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("Pictures/robot_0.bmp");
    if(surface == NULL)
    {
        printf("Cannot find robot_0.bmp!\n\n");
        SDL_Quit();
        exit(1);
    }else
    {
        //jesli wsrzystko poszlo dobrze
        if( surface != NULL )
        {
            //wybor koloru, ktory ma byc przezroczysty
            Uint32 colorkey = SDL_MapRGB( surface->format, 255, 255, 255 );

            SDL_SetColorKey( surface, 1, colorkey );
        }
    }

    game->robotFrames[0] = SDL_CreateTextureFromSurface(game->renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("Pictures/robot_1.bmp");
    if(surface == NULL)
    {
        printf("Cannot find robot_1.bmp!\n\n");
        SDL_Quit();
        exit(1);
    }else
    {
        //jesli wsrzystko poszlo dobrze
        if( surface != NULL )
        {
            //wybor koloru, ktory ma byc przezroczysty
            Uint32 colorkey = SDL_MapRGB( surface->format, 255, 255, 255 );

            SDL_SetColorKey( surface, 1, colorkey );
        }
    }

    game->robotFrames[1] = SDL_CreateTextureFromSurface(game->renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("Pictures/robot_2.bmp");
    if(surface == NULL)
    {
        printf("Cannot find robot_2.bmp!\n\n");
        SDL_Quit();
        exit(1);
    }else
    {
        //jesli wsrzystko poszlo dobrze
        if( surface != NULL )
        {
            //wybor koloru, ktory ma byc przezroczysty
            Uint32 colorkey = SDL_MapRGB( surface->format, 255, 255, 255 );

            SDL_SetColorKey( surface, 1, colorkey );
        }
    }

    game->robotFrames[2] = SDL_CreateTextureFromSurface(game->renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("Pictures/robot_3.bmp");
    if(surface == NULL)
    {
        printf("Cannot find robot_3.bmp!\n\n");
        SDL_Quit();
        exit(1);
    }else
    {
        //jesli wsrzystko poszlo dobrze
        if( surface != NULL )
        {
            //wybor koloru, ktory ma byc przezroczysty
            Uint32 colorkey = SDL_MapRGB( surface->format, 255, 255, 255 );

            SDL_SetColorKey( surface, 1, colorkey );
        }
    }

    game->robotFrames[3] = SDL_CreateTextureFromSurface(game->renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("Pictures/shooter_0.bmp");
    if(surface == NULL)
    {
        printf("Cannot find shooter_0.bmp!\n\n");
        SDL_Quit();
        exit(1);
    }else
    {
        //jesli wsrzystko poszlo dobrze
        if( surface != NULL )
        {
            //wybor koloru, ktory ma byc przezroczysty
            Uint32 colorkey = SDL_MapRGB( surface->format, 255, 255, 255 );

            SDL_SetColorKey( surface, 1, colorkey );
        }
    }

    game->shooterFrames[0] = SDL_CreateTextureFromSurface(game->renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("Pictures/shooter_1.bmp");
    if(surface == NULL)
    {
        printf("Cannot find shooter_1.bmp!\n\n");
        SDL_Quit();
        exit(1);
    }else
    {
        //jesli wsrzystko poszlo dobrze
        if( surface != NULL )
        {
            //wybor koloru, ktory ma byc przezroczysty
            Uint32 colorkey = SDL_MapRGB( surface->format, 255, 255, 255 );

            SDL_SetColorKey( surface, 1, colorkey );
        }
    }

    game->shooterFrames[1] = SDL_CreateTextureFromSurface(game->renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("Pictures/shooter_2.bmp");
    if(surface == NULL)
    {
        printf("Cannot find shooter_2.bmp!\n\n");
        SDL_Quit();
        exit(1);
    }else
    {
        //jesli wsrzystko poszlo dobrze
        if( surface != NULL )
        {
            //wybor koloru, ktory ma byc przezroczysty
            Uint32 colorkey = SDL_MapRGB( surface->format, 255, 255, 255 );

            SDL_SetColorKey( surface, 1, colorkey );
        }
    }

    game->shooterFrames[2] = SDL_CreateTextureFromSurface(game->renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("Pictures/shooter_3.bmp");
    if(surface == NULL)
    {
        printf("Cannot find shooter_3.bmp!\n\n");
        SDL_Quit();
        exit(1);
    }else
    {
        //jesli wsrzystko poszlo dobrze
        if( surface != NULL )
        {
            //wybor koloru, ktory ma byc przezroczysty
            Uint32 colorkey = SDL_MapRGB( surface->format, 255, 255, 255 );

            SDL_SetColorKey( surface, 1, colorkey );
        }
    }

    game->shooterFrames[3] = SDL_CreateTextureFromSurface(game->renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("Pictures/bullet.bmp");
    if(surface == NULL)
    {
        printf("Cannot find bullet.bmp!\n\n");
        SDL_Quit();
        exit(1);
    }else
    {
        //jesli wsrzystko poszlo dobrze
        if( surface != NULL )
        {
            //wybor koloru, ktory ma byc przezroczysty
            Uint32 colorkey = SDL_MapRGB( surface->format, 255, 255, 255 );

            SDL_SetColorKey( surface, 1, colorkey );
        }
    }

    bulletTexture = SDL_CreateTextureFromSurface(game->renderer, surface);
    SDL_FreeSurface(surface);
}

void loadGame(GameState *game)
{
    game->time = 0;
    game->mapa = 1;
    game->lvlUpTime = 0;
    game->newMapTime = 0;
    game->scrollX = 0;
    game->fireFrame = 0;


    game->shot_sound = Mix_LoadWAV("Sounds/shot.wav");
    if(game->shot_sound != NULL)
    {
        Mix_VolumeChunk(game->shot_sound, 100);
    }
}

void loadMap(GameState *game)
{
    switch (game->mapa)
    {
        case 1:
            loadMap1 (game);
            break;
        case 2:
            loadMap2 (game);
            break;
        case 3:
            loadMap3 (game);
            break;
    }
}

void loadMap1 (GameState *game)
{
    // poczatkowe definiowanie zmiennych
    game->man.x = 100;
    game->man.y = 610;
    game->man.dx = 0.1;
    game->man.dy = 0;
    game->man.onLedge = 1;
    game->man.animFrame = 2;
    game->man.facingLeft = 0;
    game->man.mode = 0;
    game->man.hp = 100;
    game->man.maxhp = 100;
    game->man.exp = 0;
    game->man.lvl = 1;
    game->man.maxexp = 100;
    game->man.damage = 10;
    game->man.rateOfFire = 13;

    int i=0;
    for(i=0; i<10; i++)
    {
        game->enemies[i].isDead = 1;
    }
    for(i=0; i<300; i++)
    {
        game->bricks[i].name = NULL;
    }

    game->enemies[0].w = 39;
    game->enemies[0].h = 50;
    game->enemies[0].x = 800;
    game->enemies[0].y = 300;
    game->enemies[0].dy = 0;
    game->enemies[0].dx = 0.1;
    game->enemies[0].hp = 100;
    game->enemies[0].maxhp = 100;
    game->enemies[0].isDead = 0;
    game->enemies[0].animFrame = 0;
    game->enemies[0].facingLeft = 0;
    game->enemies[0].name = "walker";

    game->enemies[1].w = 39;
    game->enemies[1].h = 50;
    game->enemies[1].x = 900;
    game->enemies[1].y = 550;
    game->enemies[1].dy = 0;
    game->enemies[1].dx = 0.1;
    game->enemies[1].hp = 100;
    game->enemies[1].maxhp = 100;
    game->enemies[1].isDead = 0;
    game->enemies[1].animFrame = 0;
    game->enemies[1].facingLeft = 0;
    game->enemies[1].name = "walker";

    game->enemies[2].w = 39;
    game->enemies[2].h = 50;
    game->enemies[2].x = 1300;
    game->enemies[2].y = 550;
    game->enemies[2].dy = 0;
    game->enemies[2].dx = 0.1;
    game->enemies[2].hp = 100;
    game->enemies[2].maxhp = 100;
    game->enemies[2].isDead = 0;
    game->enemies[2].animFrame = 0;
    game->enemies[2].facingLeft = 0;
    game->enemies[2].name = "walker";

    game->enemies[3].w = 39;
    game->enemies[3].h = 50;
    game->enemies[3].x = 1500;
    game->enemies[3].y = 300;
    game->enemies[3].dy = 0;
    game->enemies[3].dx = 0.1;
    game->enemies[3].hp = 100;
    game->enemies[3].maxhp = 100;
    game->enemies[3].isDead = 0;
    game->enemies[3].animFrame = 0;
    game->enemies[3].facingLeft = 0;
    game->enemies[3].name = "walker";

    game->enemies[4].w = 78;
    game->enemies[4].h = 100;
    game->enemies[4].x = 1000;
    game->enemies[4].y = 100;
    game->enemies[4].dy = 0;
    game->enemies[4].dx = 0.1;
    game->enemies[4].hp = 500;
    game->enemies[4].maxhp = 500;
    game->enemies[4].isDead = 0;
    game->enemies[4].animFrame = 0;
    game->enemies[4].facingLeft = 0;
    game->enemies[4].name = "shooter";

    // sciana po lewej
    for (i=0; i<14; i++)
    {
        game->bricks[i].x = -50;
        game->bricks[i].y = i*50;
        game->bricks[i].w = 50;
        game->bricks[i].h = 50;
        game->bricks[i].name = "ground";
    }

    // ziemia poczatek
    for (i=14; i<27; i++)
    {
        game->bricks[i].x = (i-14)*50;
        game->bricks[i].y = 650;
        game->bricks[i].w = 50;
        game->bricks[i].h = 50;
        game->bricks[i].name = "ground";
    }

    // schody wejciowe
    for (i=27; i<32; i++)
    {
        game->bricks[i].x = 620 + (i-27)*30;
        game->bricks[i].y = 620 - (i-27)*30;
        game->bricks[i].w = 30;
        game->bricks[i].h = 30;
        game->bricks[i].name = "wall";
    }

    // prawa sciana budynku
    for (i=32; i<52; i++)
    {
        game->bricks[i].x = 1520;
        game->bricks[i].y = 50 + (i-32)*30;
        game->bricks[i].w = 30;
        game->bricks[i].h = 30;
        game->bricks[i].name = "wall";
    }

    // pierwszy fragment podlogi parter
    for (i=52; i<55; i++)
    {
        game->bricks[i].x = 770 + (i-52)*30;
        game->bricks[i].y = 500;
        game->bricks[i].w = 30;
        game->bricks[i].h = 30;
        game->bricks[i].name = "wall";
    }

    // pierwszy fragment podlogi piwnica
    game->bricks[55].x = 860;
    game->bricks[55].y = 610;
    game->bricks[55].w = 30;
    game->bricks[55].h = 30;
    game->bricks[55].name = "wall";
    for (i=56; i<59; i++)
    {
        game->bricks[i].x = 860 + (i-55)*30;
        game->bricks[i].y = 620;
        game->bricks[i].w = 30;
        game->bricks[i].h = 30;
        game->bricks[i].name = "wall";
    }
    game->bricks[59].x = 980;
    game->bricks[59].y = 610;
    game->bricks[59].w = 30;
    game->bricks[59].h = 30;
    game->bricks[59].name = "wall";

    // drugi fragment podlogi parter
    for (i=60; i<67; i++)
    {
        game->bricks[i].x = 1010 + (i-59)*30;
        game->bricks[i].y = 500;
        game->bricks[i].w = 30;
        game->bricks[i].h = 30;
        game->bricks[i].name = "wall";
    }

    // drugi fragment podlogi piwnica
    for (i=67; i<70; i++)
    {
        game->bricks[i].x = 1100 + (i-67)*30;
        game->bricks[i].y = 620;
        game->bricks[i].w = 30;
        game->bricks[i].h = 30;
        game->bricks[i].name = "wall";
    }

    // trzeci fragment podlogi piwnica
    game->bricks[71].x = 1280;
    game->bricks[71].y = 610;
    game->bricks[71].w = 30;
    game->bricks[71].h = 30;
    game->bricks[71].name = "wall";
    for (i=72; i<75; i++)
    {
        game->bricks[i].x = 1280 + (i-71)*30;
        game->bricks[i].y = 620;
        game->bricks[i].w = 30;
        game->bricks[i].h = 30;
        game->bricks[i].name = "wall";
    }
    game->bricks[75].x = 1400;
    game->bricks[75].y = 610;
    game->bricks[75].w = 30;
    game->bricks[75].h = 30;
    game->bricks[75].name = "wall";

    // trzeci fragment podlogi parter
    for (i=76; i<79; i++)
    {
        game->bricks[i].x = 1430 + (i-76)*30;
        game->bricks[i].y = 500;
        game->bricks[i].w = 30;
        game->bricks[i].h = 30;
        game->bricks[i].name = "wall";
    }

    // pierwszy fragment podlogi pietro
    for (i=79; i<82; i++)
    {
        game->bricks[i].x = 770 + (i-79)*30;
        game->bricks[i].y = 350;
        game->bricks[i].w = 30;
        game->bricks[i].h = 30;
        game->bricks[i].name = "wall";
    }
    game->bricks[82].x = 860;
    game->bricks[82].y = 340;
    game->bricks[82].w = 30;
    game->bricks[82].h = 30;
    game->bricks[82].name = "wall";

    //sciana parter
    for (i=83; i<87; i++)
    {
        game->bricks[i].x = 1130;
        game->bricks[i].y = 380 + (i-83)*30;
        game->bricks[i].w = 30;
        game->bricks[i].h = 30;
        game->bricks[i].name = "wall";
    }

    // drugi fragment podlogi pietro
    for (i=87; i<94; i++)
    {
        game->bricks[i].x = 1040 + (i-87)*30;
        game->bricks[i].y = 350;
        game->bricks[i].w = 30;
        game->bricks[i].h = 30;
        game->bricks[i].name = "wall";
    }

    // schodki na pietro
    for (i=94; i<96; i++)
    {
        game->bricks[i].x = 1010 + (i-94)*240;
        game->bricks[i].y = 380;
        game->bricks[i].w = 30;
        game->bricks[i].h = 30;
        game->bricks[i].name = "wall";
    }

    // trzeci fragment podlogi pietro
    game->bricks[96].x = 1400;
    game->bricks[96].y = 340;
    game->bricks[96].w = 30;
    game->bricks[96].h = 30;
    game->bricks[96].name = "wall";
    for (i=97; i<100; i++)
    {
        game->bricks[i].x = 1400 + (i-96)*30;
        game->bricks[i].y = 350;
        game->bricks[i].w = 30;
        game->bricks[i].h = 30;
        game->bricks[i].name = "wall";
    }

    // balkon podloga
    for (i=100; i<103; i++)
    {
        game->bricks[i].x = 650 + (i-100)*30;
        game->bricks[i].y = 350;
        game->bricks[i].w = 30;
        game->bricks[i].h = 30;
        game->bricks[i].name = "wall";
    }

    // balkon barierka
    for (i=103; i<106; i++)
    {
        game->bricks[i].x = 650;
        game->bricks[i].y = 260 + (i-103)*30;
        game->bricks[i].w = 30;
        game->bricks[i].h = 30;
        game->bricks[i].name = "wall";
    }

    // strych podloga
    for (i=106; i<131; i++)
    {
        game->bricks[i].x = 770 + (i-106)*30;
        game->bricks[i].y = 200;
        game->bricks[i].w = 30;
        game->bricks[i].h = 30;
        game->bricks[i].name = "wall";
    }

    // dach
    for (i=131; i<156; i++)
    {
        game->bricks[i].x = 770 + (i-131)*30;
        game->bricks[i].y = 50;
        game->bricks[i].w = 30;
        game->bricks[i].h = 30;
        game->bricks[i].name = "wall";
    }

    // gorny fragment lewej sciany
    for (i=156; i<158; i++)
    {
        game->bricks[i].x = 740;
        game->bricks[i].y = 50 + (i-156)*30;
        game->bricks[i].w = 30;
        game->bricks[i].h = 30;
        game->bricks[i].name = "wall";
    }

    // drugi fragment lewej sciany
    for (i=158; i<161; i++)
    {
        game->bricks[i].x = 740;
        game->bricks[i].y = 170 + (i-158)*30;
        game->bricks[i].w = 30;
        game->bricks[i].h = 30;
        game->bricks[i].name = "wall";
    }

    // trzeci fragment lewej sciany
    for (i=161; i<163; i++)
    {
        game->bricks[i].x = 740;
        game->bricks[i].y = 350 + (i-161)*30;
        game->bricks[i].w = 30;
        game->bricks[i].h = 30;
        game->bricks[i].name = "wall";
    }

    // sciana pietro
    for (i=163; i<166; i++)
    {
        game->bricks[i].x = 1130;
        game->bricks[i].y = 230 + (i-163)*30;
        game->bricks[i].w = 30;
        game->bricks[i].h = 30;
        game->bricks[i].name = "wall";
    }

    // ogien
    for (i=166; i<210; i++)
    {
        game->bricks[i].x = 650 + (i-166)*50;
        game->bricks[i].y = 650;
        game->bricks[i].w = 50;
        game->bricks[i].h = 50;
        game->bricks[i].name = "fire";
    }
}

void loadMap2 (GameState *game)
{
    // poczatkowe definiowanie zmiennych
    game->man.x = 50;
    game->man.y = 100;
    game->man.dx = 0.1;
    game->man.dy = 0;
    game->man.onLedge = 1;
    game->man.animFrame = 2;
    game->man.facingLeft = 0;
    game->man.mode = 0;

    int i=0;
    for(i=0; i<10; i++)
    {
        game->enemies[i].isDead = 1;
    }
    for(i=0; i<300; i++)
    {
        game->bricks[i].name = NULL;
    }

    game->enemies[0].w = 39;
    game->enemies[0].h = 50;
    game->enemies[0].x = 250;
    game->enemies[0].y = 200;
    game->enemies[0].dy = 0;
    game->enemies[0].dx = 0.1;
    game->enemies[0].hp = 100;
    game->enemies[0].maxhp = 100;
    game->enemies[0].isDead = 0;
    game->enemies[0].animFrame = 0;
    game->enemies[0].facingLeft = 0;
    game->enemies[0].name = "walker";

    game->enemies[1].w = 39;
    game->enemies[1].h = 50;
    game->enemies[1].x = 850;
    game->enemies[1].y = 500;
    game->enemies[1].dy = 0;
    game->enemies[1].dx = 0.1;
    game->enemies[1].hp = 100;
    game->enemies[1].maxhp = 100;
    game->enemies[1].isDead = 0;
    game->enemies[1].animFrame = 0;
    game->enemies[1].facingLeft = 0;
    game->enemies[1].name = "walker";

    game->enemies[2].w = 39;
    game->enemies[2].h = 50;
    game->enemies[2].x = 1400;
    game->enemies[2].y = 450;
    game->enemies[2].dy = 0;
    game->enemies[2].dx = 0.1;
    game->enemies[2].hp = 100;
    game->enemies[2].maxhp = 100;
    game->enemies[2].isDead = 0;
    game->enemies[2].animFrame = 0;
    game->enemies[2].facingLeft = 0;
    game->enemies[2].name = "walker";

    game->enemies[3].w = 39;
    game->enemies[3].h = 50;
    game->enemies[3].x = 600;
    game->enemies[3].y = 130;
    game->enemies[3].dy = 0;
    game->enemies[3].dx = 0.1;
    game->enemies[3].hp = 100;
    game->enemies[3].maxhp = 100;
    game->enemies[3].isDead = 0;
    game->enemies[3].animFrame = 0;
    game->enemies[3].facingLeft = 0;
    game->enemies[3].name = "walker";

    game->enemies[4].w = 78;
    game->enemies[4].h = 100;
    game->enemies[4].x = 1200;
    game->enemies[4].y = 80;
    game->enemies[4].dy = 0;
    game->enemies[4].dx = 0.1;
    game->enemies[4].hp = 500;
    game->enemies[4].maxhp = 500;
    game->enemies[4].isDead = 0;
    game->enemies[4].animFrame = 0;
    game->enemies[4].facingLeft = 0;
    game->enemies[4].name = "shooter";

    game->bricks[0].x = 0;
    game->bricks[0].y = 135;
    game->bricks[0].w = 30;
    game->bricks[0].h = 30;
    game->bricks[0].name = "wall";
    game->bricks[1].x = 30;
    game->bricks[1].y = 135;
    game->bricks[1].w = 30;
    game->bricks[1].h = 30;
    game->bricks[1].name = "wall";
    game->bricks[2].x = 60;
    game->bricks[2].y = 135;
    game->bricks[2].w = 30;
    game->bricks[2].h = 30;
    game->bricks[2].name = "wall";
    game->bricks[3].x = 90;
    game->bricks[3].y = 135;
    game->bricks[3].w = 30;
    game->bricks[3].h = 30;
    game->bricks[3].name = "wall";
    game->bricks[4].x = 120;
    game->bricks[4].y = 135;
    game->bricks[4].w = 30;
    game->bricks[4].h = 30;
    game->bricks[4].name = "wall";
    game->bricks[5].x = 195;
    game->bricks[5].y = 255;
    game->bricks[5].w = 30;
    game->bricks[5].h = 30;
    game->bricks[5].name = "wall";
    game->bricks[6].x = 225;
    game->bricks[6].y = 270;
    game->bricks[6].w = 30;
    game->bricks[6].h = 30;
    game->bricks[6].name = "wall";
    game->bricks[7].x = 255;
    game->bricks[7].y = 270;
    game->bricks[7].w = 30;
    game->bricks[7].h = 30;
    game->bricks[7].name = "wall";
    game->bricks[8].x = 285;
    game->bricks[8].y = 270;
    game->bricks[8].w = 30;
    game->bricks[8].h = 30;
    game->bricks[8].name = "wall";
    game->bricks[9].x = 315;
    game->bricks[9].y = 270;
    game->bricks[9].w = 30;
    game->bricks[9].h = 30;
    game->bricks[9].name = "wall";
    game->bricks[10].x = 345;
    game->bricks[10].y = 270;
    game->bricks[10].w = 30;
    game->bricks[10].h = 30;
    game->bricks[10].name = "wall";
    game->bricks[11].x = 375;
    game->bricks[11].y = 255;
    game->bricks[11].w = 30;
    game->bricks[11].h = 30;
    game->bricks[11].name = "wall";
    game->bricks[12].x = 480;
    game->bricks[12].y = 370;
    game->bricks[12].w = 30;
    game->bricks[12].h = 30;
    game->bricks[12].name = "wall";
    game->bricks[13].x = 510;
    game->bricks[13].y = 370;
    game->bricks[13].w = 30;
    game->bricks[13].h = 30;
    game->bricks[13].name = "wall";
    game->bricks[14].x = 720;
    game->bricks[14].y = 570;
    game->bricks[14].w = 30;
    game->bricks[14].h = 30;
    game->bricks[14].name = "wall";
    game->bricks[15].x = 750;
    game->bricks[15].y = 585;
    game->bricks[15].w = 30;
    game->bricks[15].h = 30;
    game->bricks[15].name = "wall";
    game->bricks[16].x = 780;
    game->bricks[16].y = 585;
    game->bricks[16].w = 30;
    game->bricks[16].h = 30;
    game->bricks[16].name = "wall";
    game->bricks[17].x = 810;
    game->bricks[17].y = 585;
    game->bricks[17].w = 30;
    game->bricks[17].h = 30;
    game->bricks[17].name = "wall";
    game->bricks[18].x = 840;
    game->bricks[18].y = 585;
    game->bricks[18].w = 30;
    game->bricks[18].h = 30;
    game->bricks[18].name = "wall";
    game->bricks[19].x = 870;
    game->bricks[19].y = 585;
    game->bricks[19].w = 30;
    game->bricks[19].h = 30;
    game->bricks[19].name = "wall";
    game->bricks[20].x = 900;
    game->bricks[20].y = 585;
    game->bricks[20].w = 30;
    game->bricks[20].h = 30;
    game->bricks[20].name = "wall";
    game->bricks[21].x = 930;
    game->bricks[21].y = 570;
    game->bricks[21].w = 30;
    game->bricks[21].h = 30;
    game->bricks[21].name = "wall";
    game->bricks[22].x = 990;
    game->bricks[22].y = 555;
    game->bricks[22].w = 30;
    game->bricks[22].h = 30;
    game->bricks[22].name = "wall";
    game->bricks[23].x = 1020;
    game->bricks[23].y = 555;
    game->bricks[23].w = 30;
    game->bricks[23].h = 30;
    game->bricks[23].name = "wall";
    game->bricks[24].x = 990;
    game->bricks[24].y = 495;
    game->bricks[24].w = 30;
    game->bricks[24].h = 30;
    game->bricks[24].name = "wall";
    game->bricks[25].x = 1020;
    game->bricks[25].y = 495;
    game->bricks[25].w = 30;
    game->bricks[25].h = 30;
    game->bricks[25].name = "wall";
    game->bricks[26].x = 1050;
    game->bricks[26].y = 495;
    game->bricks[26].w = 30;
    game->bricks[26].h = 30;
    game->bricks[26].name = "wall";
    game->bricks[27].x = 1080;
    game->bricks[27].y = 495;
    game->bricks[27].w = 30;
    game->bricks[27].h = 30;
    game->bricks[27].name = "wall";
    game->bricks[28].x = 960;
    game->bricks[28].y = 555;
    game->bricks[28].w = 30;
    game->bricks[28].h = 30;
    game->bricks[28].name = "wall";
    game->bricks[29].x = 1050;
    game->bricks[29].y = 555;
    game->bricks[29].w = 30;
    game->bricks[29].h = 30;
    game->bricks[29].name = "wall";
    game->bricks[30].x = 1080;
    game->bricks[30].y = 555;
    game->bricks[30].w = 30;
    game->bricks[30].h = 30;
    game->bricks[30].name = "wall";
    game->bricks[31].x = 1110;
    game->bricks[31].y = 555;
    game->bricks[31].w = 30;
    game->bricks[31].h = 30;
    game->bricks[31].name = "wall";
    game->bricks[32].x = 1140;
    game->bricks[32].y = 555;
    game->bricks[32].w = 30;
    game->bricks[32].h = 30;
    game->bricks[32].name = "wall";
    game->bricks[33].x = 1170;
    game->bricks[33].y = 555;
    game->bricks[33].w = 30;
    game->bricks[33].h = 30;
    game->bricks[33].name = "wall";
    game->bricks[34].x = 1110;
    game->bricks[34].y = 495;
    game->bricks[34].w = 30;
    game->bricks[34].h = 30;
    game->bricks[34].name = "wall";
    game->bricks[35].x = 1200;
    game->bricks[35].y = 555;
    game->bricks[35].w = 30;
    game->bricks[35].h = 30;
    game->bricks[35].name = "wall";
    game->bricks[36].x = 1230;
    game->bricks[36].y = 555;
    game->bricks[36].w = 30;
    game->bricks[36].h = 30;
    game->bricks[36].name = "wall";
    game->bricks[37].x = 1260;
    game->bricks[37].y = 555;
    game->bricks[37].w = 30;
    game->bricks[37].h = 30;
    game->bricks[37].name = "wall";
    game->bricks[38].x = 1290;
    game->bricks[38].y = 555;
    game->bricks[38].w = 30;
    game->bricks[38].h = 30;
    game->bricks[38].name = "wall";
    game->bricks[39].x = 1350;
    game->bricks[39].y = 525;
    game->bricks[39].w = 30;
    game->bricks[39].h = 30;
    game->bricks[39].name = "wall";
    game->bricks[40].x = 1320;
    game->bricks[40].y = 540;
    game->bricks[40].w = 30;
    game->bricks[40].h = 30;
    game->bricks[40].name = "wall";
    game->bricks[41].x = 1380;
    game->bricks[41].y = 510;
    game->bricks[41].w = 30;
    game->bricks[41].h = 30;
    game->bricks[41].name = "wall";
    game->bricks[42].x = 1410;
    game->bricks[42].y = 525;
    game->bricks[42].w = 30;
    game->bricks[42].h = 30;
    game->bricks[42].name = "wall";
    game->bricks[43].x = 1440;
    game->bricks[43].y = 525;
    game->bricks[43].w = 30;
    game->bricks[43].h = 30;
    game->bricks[43].name = "wall";
    game->bricks[44].x = 1470;
    game->bricks[44].y = 525;
    game->bricks[44].w = 30;
    game->bricks[44].h = 30;
    game->bricks[44].name = "wall";
    game->bricks[45].x = 1500;
    game->bricks[45].y = 525;
    game->bricks[45].w = 30;
    game->bricks[45].h = 30;
    game->bricks[45].name = "wall";
    game->bricks[46].x = 1530;
    game->bricks[46].y = 525;
    game->bricks[46].w = 30;
    game->bricks[46].h = 30;
    game->bricks[46].name = "wall";
    game->bricks[47].x = 1560;
    game->bricks[47].y = 525;
    game->bricks[47].w = 30;
    game->bricks[47].h = 30;
    game->bricks[47].name = "wall";
    game->bricks[48].x = 1590;
    game->bricks[48].y = 525;
    game->bricks[48].w = 30;
    game->bricks[48].h = 30;
    game->bricks[48].name = "wall";
    game->bricks[49].x = 1620;
    game->bricks[49].y = 525;
    game->bricks[49].w = 30;
    game->bricks[49].h = 30;
    game->bricks[49].name = "wall";
    game->bricks[50].x = 1650;
    game->bricks[50].y = 495;
    game->bricks[50].w = 30;
    game->bricks[50].h = 30;
    game->bricks[50].name = "wall";
    game->bricks[51].x = 1650;
    game->bricks[51].y = 465;
    game->bricks[51].w = 30;
    game->bricks[51].h = 30;
    game->bricks[51].name = "wall";
    game->bricks[52].x = 1650;
    game->bricks[52].y = 435;
    game->bricks[52].w = 30;
    game->bricks[52].h = 30;
    game->bricks[52].name = "wall";
    game->bricks[53].x = 1650;
    game->bricks[53].y = 405;
    game->bricks[53].w = 30;
    game->bricks[53].h = 30;
    game->bricks[53].name = "wall";
    game->bricks[54].x = 1650;
    game->bricks[54].y = 375;
    game->bricks[54].w = 30;
    game->bricks[54].h = 30;
    game->bricks[54].name = "wall";
    game->bricks[55].x = 1680;
    game->bricks[55].y = 375;
    game->bricks[55].w = 30;
    game->bricks[55].h = 30;
    game->bricks[55].name = "wall";
    game->bricks[56].x = 1710;
    game->bricks[56].y = 375;
    game->bricks[56].w = 30;
    game->bricks[56].h = 30;
    game->bricks[56].name = "wall";
    game->bricks[57].x = 1710;
    game->bricks[57].y = 345;
    game->bricks[57].w = 30;
    game->bricks[57].h = 30;
    game->bricks[57].name = "wall";
    game->bricks[58].x = 1710;
    game->bricks[58].y = 315;
    game->bricks[58].w = 30;
    game->bricks[58].h = 30;
    game->bricks[58].name = "wall";
    game->bricks[59].x = 1710;
    game->bricks[59].y = 285;
    game->bricks[59].w = 30;
    game->bricks[59].h = 30;
    game->bricks[59].name = "wall";
    game->bricks[60].x = 1710;
    game->bricks[60].y = 255;
    game->bricks[60].w = 30;
    game->bricks[60].h = 30;
    game->bricks[60].name = "wall";
    game->bricks[61].x = 1680;
    game->bricks[61].y = 255;
    game->bricks[61].w = 30;
    game->bricks[61].h = 30;
    game->bricks[61].name = "wall";
    game->bricks[62].x = 1500;
    game->bricks[62].y = 315;
    game->bricks[62].w = 30;
    game->bricks[62].h = 30;
    game->bricks[62].name = "wall";
    game->bricks[63].x = 1620;
    game->bricks[63].y = 495;
    game->bricks[63].w = 30;
    game->bricks[63].h = 30;
    game->bricks[63].name = "wall";
    game->bricks[64].x = 1470;
    game->bricks[64].y = 315;
    game->bricks[64].w = 30;
    game->bricks[64].h = 30;
    game->bricks[64].name = "wall";
    game->bricks[65].x = 1470;
    game->bricks[65].y = 285;
    game->bricks[65].w = 30;
    game->bricks[65].h = 30;
    game->bricks[65].name = "wall";
    game->bricks[66].x = 1470;
    game->bricks[66].y = 255;
    game->bricks[66].w = 30;
    game->bricks[66].h = 30;
    game->bricks[66].name = "wall";
    game->bricks[67].x = 1470;
    game->bricks[67].y = 225;
    game->bricks[67].w = 30;
    game->bricks[67].h = 30;
    game->bricks[67].name = "wall";
    game->bricks[68].x = 1470;
    game->bricks[68].y = 195;
    game->bricks[68].w = 30;
    game->bricks[68].h = 30;
    game->bricks[68].name = "wall";
    game->bricks[69].x = 1500;
    game->bricks[69].y = 180;
    game->bricks[69].w = 30;
    game->bricks[69].h = 30;
    game->bricks[69].name = "wall";
    game->bricks[70].x = 1650;
    game->bricks[70].y = 225;
    game->bricks[70].w = 30;
    game->bricks[70].h = 30;
    game->bricks[70].name = "wall";
    game->bricks[71].x = 1680;
    game->bricks[71].y = 225;
    game->bricks[71].w = 30;
    game->bricks[71].h = 30;
    game->bricks[71].name = "wall";
    game->bricks[72].x = 1710;
    game->bricks[72].y = 225;
    game->bricks[72].w = 30;
    game->bricks[72].h = 30;
    game->bricks[72].name = "wall";
    game->bricks[73].x = 1530;
    game->bricks[73].y = 160;
    game->bricks[73].w = 30;
    game->bricks[73].h = 30;
    game->bricks[73].name = "wall";
    game->bricks[74].x = 1560;
    game->bricks[74].y = 140;
    game->bricks[74].w = 30;
    game->bricks[74].h = 30;
    game->bricks[74].name = "wall";
    game->bricks[75].x = 1560;
    game->bricks[75].y = 15;
    game->bricks[75].w = 30;
    game->bricks[75].h = 30;
    game->bricks[75].name = "wall";
    game->bricks[76].x = 1530;
    game->bricks[76].y = 15;
    game->bricks[76].w = 30;
    game->bricks[76].h = 30;
    game->bricks[76].name = "wall";
    game->bricks[77].x = 1500;
    game->bricks[77].y = 15;
    game->bricks[77].w = 30;
    game->bricks[77].h = 30;
    game->bricks[77].name = "wall";
    game->bricks[78].x = 1470;
    game->bricks[78].y = 15;
    game->bricks[78].w = 30;
    game->bricks[78].h = 30;
    game->bricks[78].name = "wall";
    game->bricks[79].x = 1440;
    game->bricks[79].y = 15;
    game->bricks[79].w = 30;
    game->bricks[79].h = 30;
    game->bricks[79].name = "wall";
    game->bricks[80].x = 1410;
    game->bricks[80].y = 15;
    game->bricks[80].w = 30;
    game->bricks[80].h = 30;
    game->bricks[80].name = "wall";
    game->bricks[81].x = 1380;
    game->bricks[81].y = 15;
    game->bricks[81].w = 30;
    game->bricks[81].h = 30;
    game->bricks[81].name = "wall";
    game->bricks[82].x = 1590;
    game->bricks[82].y = 15;
    game->bricks[82].w = 30;
    game->bricks[82].h = 30;
    game->bricks[82].name = "wall";
    game->bricks[83].x = 1620;
    game->bricks[83].y = 15;
    game->bricks[83].w = 30;
    game->bricks[83].h = 30;
    game->bricks[83].name = "wall";
    game->bricks[84].x = 1650;
    game->bricks[84].y = 15;
    game->bricks[84].w = 30;
    game->bricks[84].h = 30;
    game->bricks[84].name = "wall";
    game->bricks[85].x = 1680;
    game->bricks[85].y = 15;
    game->bricks[85].w = 30;
    game->bricks[85].h = 30;
    game->bricks[85].name = "wall";
    game->bricks[86].x = 1710;
    game->bricks[86].y = 15;
    game->bricks[86].w = 30;
    game->bricks[86].h = 30;
    game->bricks[86].name = "wall";
    game->bricks[87].x = 1710;
    game->bricks[87].y = 45;
    game->bricks[87].w = 30;
    game->bricks[87].h = 30;
    game->bricks[87].name = "wall";
    game->bricks[88].x = 1710;
    game->bricks[88].y = 75;
    game->bricks[88].w = 30;
    game->bricks[88].h = 30;
    game->bricks[88].name = "wall";
    game->bricks[89].x = 1710;
    game->bricks[89].y = 105;
    game->bricks[89].w = 30;
    game->bricks[89].h = 30;
    game->bricks[89].name = "wall";
    game->bricks[90].x = 1710;
    game->bricks[90].y = 135;
    game->bricks[90].w = 30;
    game->bricks[90].h = 30;
    game->bricks[90].name = "wall";
    game->bricks[91].x = 1710;
    game->bricks[91].y = 165;
    game->bricks[91].w = 30;
    game->bricks[91].h = 30;
    game->bricks[91].name = "wall";
    game->bricks[92].x = 1710;
    game->bricks[92].y = 195;
    game->bricks[92].w = 30;
    game->bricks[92].h = 30;
    game->bricks[92].name = "wall";
    game->bricks[93].x = 1500;
    game->bricks[93].y = 345;
    game->bricks[93].w = 30;
    game->bricks[93].h = 30;
    game->bricks[93].name = "wall";
    game->bricks[94].x = 1470;
    game->bricks[94].y = 375;
    game->bricks[94].w = 30;
    game->bricks[94].h = 30;
    game->bricks[94].name = "wall";
    game->bricks[95].x = 1440;
    game->bricks[95].y = 375;
    game->bricks[95].w = 30;
    game->bricks[95].h = 30;
    game->bricks[95].name = "wall";
    game->bricks[96].x = 1410;
    game->bricks[96].y = 375;
    game->bricks[96].w = 30;
    game->bricks[96].h = 30;
    game->bricks[96].name = "wall";
    game->bricks[97].x = 1560;
    game->bricks[97].y = 315;
    game->bricks[97].w = 30;
    game->bricks[97].h = 30;
    game->bricks[97].name = "wall";
    game->bricks[98].x = 1470;
    game->bricks[98].y = 345;
    game->bricks[98].w = 30;
    game->bricks[98].h = 30;
    game->bricks[98].name = "wall";
    game->bricks[99].x = 1380;
    game->bricks[99].y = 375;
    game->bricks[99].w = 30;
    game->bricks[99].h = 30;
    game->bricks[99].name = "wall";
    game->bricks[100].x = 1350;
    game->bricks[100].y = 375;
    game->bricks[100].w = 30;
    game->bricks[100].h = 30;
    game->bricks[100].name = "wall";
    game->bricks[101].x = 1530;
    game->bricks[101].y = 315;
    game->bricks[101].w = 30;
    game->bricks[101].h = 30;
    game->bricks[101].name = "wall";
    game->bricks[102].x = 1320;
    game->bricks[102].y = 375;
    game->bricks[102].w = 30;
    game->bricks[102].h = 30;
    game->bricks[102].name = "wall";
    game->bricks[103].x = 1290;
    game->bricks[103].y = 375;
    game->bricks[103].w = 30;
    game->bricks[103].h = 30;
    game->bricks[103].name = "wall";
    game->bricks[104].x = 1260;
    game->bricks[104].y = 375;
    game->bricks[104].w = 30;
    game->bricks[104].h = 30;
    game->bricks[104].name = "wall";
    game->bricks[105].x = 1350;
    game->bricks[105].y = 45;
    game->bricks[105].w = 30;
    game->bricks[105].h = 30;
    game->bricks[105].name = "wall";
    game->bricks[106].x = 1350;
    game->bricks[106].y = 75;
    game->bricks[106].w = 30;
    game->bricks[106].h = 30;
    game->bricks[106].name = "wall";
    game->bricks[107].x = 1350;
    game->bricks[107].y = 105;
    game->bricks[107].w = 30;
    game->bricks[107].h = 30;
    game->bricks[107].name = "wall";
    game->bricks[108].x = 1350;
    game->bricks[108].y = 135;
    game->bricks[108].w = 30;
    game->bricks[108].h = 30;
    game->bricks[108].name = "wall";
    game->bricks[109].x = 1350;
    game->bricks[109].y = 165;
    game->bricks[109].w = 30;
    game->bricks[109].h = 30;
    game->bricks[109].name = "wall";
    game->bricks[110].x = 1350;
    game->bricks[110].y = 195;
    game->bricks[110].w = 30;
    game->bricks[110].h = 30;
    game->bricks[110].name = "wall";
    game->bricks[111].x = 1350;
    game->bricks[111].y = 225;
    game->bricks[111].w = 30;
    game->bricks[111].h = 30;
    game->bricks[111].name = "wall";
    game->bricks[112].x = 1230;
    game->bricks[112].y = 375;
    game->bricks[112].w = 30;
    game->bricks[112].h = 30;
    game->bricks[112].name = "wall";
    game->bricks[113].x = 1200;
    game->bricks[113].y = 375;
    game->bricks[113].w = 30;
    game->bricks[113].h = 30;
    game->bricks[113].name = "wall";
    game->bricks[114].x = 1170;
    game->bricks[114].y = 375;
    game->bricks[114].w = 30;
    game->bricks[114].h = 30;
    game->bricks[114].name = "wall";
    game->bricks[115].x = 1140;
    game->bricks[115].y = 495;
    game->bricks[115].w = 30;
    game->bricks[115].h = 30;
    game->bricks[115].name = "wall";
    game->bricks[116].x = 1140;
    game->bricks[116].y = 465;
    game->bricks[116].w = 30;
    game->bricks[116].h = 30;
    game->bricks[116].name = "wall";
    game->bricks[117].x = 1140;
    game->bricks[117].y = 435;
    game->bricks[117].w = 30;
    game->bricks[117].h = 30;
    game->bricks[117].name = "wall";
    game->bricks[118].x = 1140;
    game->bricks[118].y = 405;
    game->bricks[118].w = 30;
    game->bricks[118].h = 30;
    game->bricks[118].name = "wall";
    game->bricks[119].x = 1140;
    game->bricks[119].y = 375;
    game->bricks[119].w = 30;
    game->bricks[119].h = 30;
    game->bricks[119].name = "wall";
    game->bricks[120].x = 990;
    game->bricks[120].y = 465;
    game->bricks[120].w = 30;
    game->bricks[120].h = 30;
    game->bricks[120].name = "wall";
    game->bricks[121].x = 990;
    game->bricks[121].y = 435;
    game->bricks[121].w = 30;
    game->bricks[121].h = 30;
    game->bricks[121].name = "wall";
    game->bricks[122].x = 990;
    game->bricks[122].y = 405;
    game->bricks[122].w = 30;
    game->bricks[122].h = 30;
    game->bricks[122].name = "wall";
    game->bricks[123].x = 990;
    game->bricks[123].y = 375;
    game->bricks[123].w = 30;
    game->bricks[123].h = 30;
    game->bricks[123].name = "wall";
    game->bricks[124].x = 1350;
    game->bricks[124].y = 315;
    game->bricks[124].w = 30;
    game->bricks[124].h = 30;
    game->bricks[124].name = "wall";
    game->bricks[125].x = 1350;
    game->bricks[125].y = 285;
    game->bricks[125].w = 30;
    game->bricks[125].h = 30;
    game->bricks[125].name = "wall";
    game->bricks[126].x = 1350;
    game->bricks[126].y = 255;
    game->bricks[126].w = 30;
    game->bricks[126].h = 30;
    game->bricks[126].name = "wall";
    game->bricks[127].x = 990;
    game->bricks[127].y = 345;
    game->bricks[127].w = 30;
    game->bricks[127].h = 30;
    game->bricks[127].name = "wall";
    game->bricks[128].x = 960;
    game->bricks[128].y = 330;
    game->bricks[128].w = 30;
    game->bricks[128].h = 30;
    game->bricks[128].name = "wall";
    game->bricks[129].x = 930;
    game->bricks[129].y = 315;
    game->bricks[129].w = 30;
    game->bricks[129].h = 30;
    game->bricks[129].name = "wall";
    game->bricks[130].x = 900;
    game->bricks[130].y = 300;
    game->bricks[130].w = 30;
    game->bricks[130].h = 30;
    game->bricks[130].name = "wall";
    game->bricks[131].x = 870;
    game->bricks[131].y = 285;
    game->bricks[131].w = 30;
    game->bricks[131].h = 30;
    game->bricks[131].name = "wall";
    game->bricks[132].x = 840;
    game->bricks[132].y = 270;
    game->bricks[132].w = 30;
    game->bricks[132].h = 30;
    game->bricks[132].name = "wall";
    game->bricks[133].x = 810;
    game->bricks[133].y = 255;
    game->bricks[133].w = 30;
    game->bricks[133].h = 30;
    game->bricks[133].name = "wall";
    game->bricks[134].x = 780;
    game->bricks[134].y = 240;
    game->bricks[134].w = 30;
    game->bricks[134].h = 30;
    game->bricks[134].name = "wall";
    game->bricks[135].x = 750;
    game->bricks[135].y = 225;
    game->bricks[135].w = 30;
    game->bricks[135].h = 30;
    game->bricks[135].name = "wall";
    game->bricks[136].x = 720;
    game->bricks[136].y = 210;
    game->bricks[136].w = 30;
    game->bricks[136].h = 30;
    game->bricks[136].name = "wall";
    game->bricks[137].x = 690;
    game->bricks[137].y = 225;
    game->bricks[137].w = 30;
    game->bricks[137].h = 30;
    game->bricks[137].name = "wall";
    game->bricks[138].x = 660;
    game->bricks[138].y = 225;
    game->bricks[138].w = 30;
    game->bricks[138].h = 30;
    game->bricks[138].name = "wall";
    game->bricks[139].x = 630;
    game->bricks[139].y = 225;
    game->bricks[139].w = 30;
    game->bricks[139].h = 30;
    game->bricks[139].name = "wall";
    game->bricks[140].x = 600;
    game->bricks[140].y = 225;
    game->bricks[140].w = 30;
    game->bricks[140].h = 30;
    game->bricks[140].name = "wall";
    game->bricks[141].x = 570;
    game->bricks[141].y = 225;
    game->bricks[141].w = 30;
    game->bricks[141].h = 30;
    game->bricks[141].name = "wall";
    game->bricks[142].x = 540;
    game->bricks[142].y = 165;
    game->bricks[142].w = 30;
    game->bricks[142].h = 30;
    game->bricks[142].name = "wall";
    game->bricks[143].x = 540;
    game->bricks[143].y = 135;
    game->bricks[143].w = 30;
    game->bricks[143].h = 30;
    game->bricks[143].name = "wall";
    game->bricks[144].x = 540;
    game->bricks[144].y = 105;
    game->bricks[144].w = 30;
    game->bricks[144].h = 30;
    game->bricks[144].name = "wall";
    game->bricks[145].x = 540;
    game->bricks[145].y = 75;
    game->bricks[145].w = 30;
    game->bricks[145].h = 30;
    game->bricks[145].name = "wall";
    game->bricks[146].x = 540;
    game->bricks[146].y = 45;
    game->bricks[146].w = 30;
    game->bricks[146].h = 30;
    game->bricks[146].name = "wall";
    game->bricks[147].x = 540;
    game->bricks[147].y = 15;
    game->bricks[147].w = 30;
    game->bricks[147].h = 30;
    game->bricks[147].name = "wall";
    game->bricks[148].x = 570;
    game->bricks[148].y = 15;
    game->bricks[148].w = 30;
    game->bricks[148].h = 30;
    game->bricks[148].name = "wall";
    game->bricks[149].x = 600;
    game->bricks[149].y = 15;
    game->bricks[149].w = 30;
    game->bricks[149].h = 30;
    game->bricks[149].name = "wall";
    game->bricks[150].x = 630;
    game->bricks[150].y = 15;
    game->bricks[150].w = 30;
    game->bricks[150].h = 30;
    game->bricks[150].name = "wall";
    game->bricks[151].x = 660;
    game->bricks[151].y = 15;
    game->bricks[151].w = 30;
    game->bricks[151].h = 30;
    game->bricks[151].name = "wall";
    game->bricks[152].x = 690;
    game->bricks[152].y = 15;
    game->bricks[152].w = 30;
    game->bricks[152].h = 30;
    game->bricks[152].name = "wall";
    game->bricks[153].x = 720;
    game->bricks[153].y = 15;
    game->bricks[153].w = 30;
    game->bricks[153].h = 30;
    game->bricks[153].name = "wall";
    game->bricks[154].x = 750;
    game->bricks[154].y = 15;
    game->bricks[154].w = 30;
    game->bricks[154].h = 30;
    game->bricks[154].name = "wall";
    game->bricks[155].x = 780;
    game->bricks[155].y = 15;
    game->bricks[155].w = 30;
    game->bricks[155].h = 30;
    game->bricks[155].name = "wall";
    game->bricks[156].x = 810;
    game->bricks[156].y = 15;
    game->bricks[156].w = 30;
    game->bricks[156].h = 30;
    game->bricks[156].name = "wall";
    game->bricks[157].x = 840;
    game->bricks[157].y = 15;
    game->bricks[157].w = 30;
    game->bricks[157].h = 30;
    game->bricks[157].name = "wall";
    game->bricks[158].x = 870;
    game->bricks[158].y = 15;
    game->bricks[158].w = 30;
    game->bricks[158].h = 30;
    game->bricks[158].name = "wall";
    game->bricks[159].x = 900;
    game->bricks[159].y = 15;
    game->bricks[159].w = 30;
    game->bricks[159].h = 30;
    game->bricks[159].name = "wall";
    game->bricks[160].x = 930;
    game->bricks[160].y = 15;
    game->bricks[160].w = 30;
    game->bricks[160].h = 30;
    game->bricks[160].name = "wall";
    game->bricks[161].x = 960;
    game->bricks[161].y = 15;
    game->bricks[161].w = 30;
    game->bricks[161].h = 30;
    game->bricks[161].name = "wall";
    game->bricks[162].x = 990;
    game->bricks[162].y = 15;
    game->bricks[162].w = 30;
    game->bricks[162].h = 30;
    game->bricks[162].name = "wall";
    game->bricks[163].x = 1020;
    game->bricks[163].y = 15;
    game->bricks[163].w = 30;
    game->bricks[163].h = 30;
    game->bricks[163].name = "wall";
    game->bricks[164].x = 1050;
    game->bricks[164].y = 15;
    game->bricks[164].w = 30;
    game->bricks[164].h = 30;
    game->bricks[164].name = "wall";
    game->bricks[165].x = 1080;
    game->bricks[165].y = 15;
    game->bricks[165].w = 30;
    game->bricks[165].h = 30;
    game->bricks[165].name = "wall";
    game->bricks[166].x = 1110;
    game->bricks[166].y = 15;
    game->bricks[166].w = 30;
    game->bricks[166].h = 30;
    game->bricks[166].name = "wall";
    game->bricks[167].x = 1140;
    game->bricks[167].y = 15;
    game->bricks[167].w = 30;
    game->bricks[167].h = 30;
    game->bricks[167].name = "wall";
    game->bricks[168].x = 1170;
    game->bricks[168].y = 15;
    game->bricks[168].w = 30;
    game->bricks[168].h = 30;
    game->bricks[168].name = "wall";
    game->bricks[169].x = 1200;
    game->bricks[169].y = 15;
    game->bricks[169].w = 30;
    game->bricks[169].h = 30;
    game->bricks[169].name = "wall";
    game->bricks[170].x = 1230;
    game->bricks[170].y = 15;
    game->bricks[170].w = 30;
    game->bricks[170].h = 30;
    game->bricks[170].name = "wall";
    game->bricks[171].x = 1260;
    game->bricks[171].y = 15;
    game->bricks[171].w = 30;
    game->bricks[171].h = 30;
    game->bricks[171].name = "wall";
    game->bricks[172].x = 1290;
    game->bricks[172].y = 15;
    game->bricks[172].w = 30;
    game->bricks[172].h = 30;
    game->bricks[172].name = "wall";
    game->bricks[173].x = 1320;
    game->bricks[173].y = 15;
    game->bricks[173].w = 30;
    game->bricks[173].h = 30;
    game->bricks[173].name = "wall";
    game->bricks[174].x = 540;
    game->bricks[174].y = 195;
    game->bricks[174].w = 30;
    game->bricks[174].h = 30;
    game->bricks[174].name = "wall";
    game->bricks[175].x = 450;
    game->bricks[175].y = 370;
    game->bricks[175].w = 30;
    game->bricks[175].h = 30;
    game->bricks[175].name = "wall";
    game->bricks[176].x = 1170;
    game->bricks[176].y = 195;
    game->bricks[176].w = 30;
    game->bricks[176].h = 30;
    game->bricks[176].name = "wall";
    game->bricks[177].x = 1350;
    game->bricks[177].y = 15;
    game->bricks[177].w = 30;
    game->bricks[177].h = 30;
    game->bricks[177].name = "wall";
    game->bricks[178].x = 1200;
    game->bricks[178].y = 195;
    game->bricks[178].w = 30;
    game->bricks[178].h = 30;
    game->bricks[178].name = "wall";
    game->bricks[179].x = 1230;
    game->bricks[179].y = 195;
    game->bricks[179].w = 30;
    game->bricks[179].h = 30;
    game->bricks[179].name = "wall";
    game->bricks[180].x = 1260;
    game->bricks[180].y = 195;
    game->bricks[180].w = 30;
    game->bricks[180].h = 30;
    game->bricks[180].name = "wall";
    game->bricks[181].x = 1290;
    game->bricks[181].y = 195;
    game->bricks[181].w = 30;
    game->bricks[181].h = 30;
    game->bricks[181].name = "wall";
    game->bricks[182].x = 1320;
    game->bricks[182].y = 195;
    game->bricks[182].w = 30;
    game->bricks[182].h = 30;
    game->bricks[182].name = "wall";
    game->bricks[183].x = 1140;
    game->bricks[183].y = 195;
    game->bricks[183].w = 30;
    game->bricks[183].h = 30;
    game->bricks[183].name = "wall";
    game->bricks[184].x = 1110;
    game->bricks[184].y = 195;
    game->bricks[184].w = 30;
    game->bricks[184].h = 30;
    game->bricks[184].name = "wall";
    game->bricks[185].x = 1080;
    game->bricks[185].y = 195;
    game->bricks[185].w = 30;
    game->bricks[185].h = 30;
    game->bricks[185].name = "wall";
    game->bricks[186].x = 1050;
    game->bricks[186].y = 195;
    game->bricks[186].w = 30;
    game->bricks[186].h = 30;
    game->bricks[186].name = "wall";
    game->bricks[187].x = 1020;
    game->bricks[187].y = 180;
    game->bricks[187].w = 30;
    game->bricks[187].h = 30;
    game->bricks[187].name = "wall";
    game->bricks[188].x = 990;
    game->bricks[188].y = 165;
    game->bricks[188].w = 30;
    game->bricks[188].h = 30;
    game->bricks[188].name = "wall";
    game->bricks[189].x = 960;
    game->bricks[189].y = 150;
    game->bricks[189].w = 30;
    game->bricks[189].h = 30;
    game->bricks[189].name = "wall";
    game->bricks[190].x = 930;
    game->bricks[190].y = 135;
    game->bricks[190].w = 30;
    game->bricks[190].h = 30;
    game->bricks[190].name = "wall";
    game->bricks[191].x = 900;
    game->bricks[191].y = 120;
    game->bricks[191].w = 30;
    game->bricks[191].h = 30;
    game->bricks[191].name = "wall";
    game->bricks[192].x = 870;
    game->bricks[192].y = 105;
    game->bricks[192].w = 30;
    game->bricks[192].h = 30;
    game->bricks[192].name = "wall";
    game->bricks[193].x = 840;
    game->bricks[193].y = 105;
    game->bricks[193].w = 30;
    game->bricks[193].h = 30;
    game->bricks[193].name = "wall";
    game->bricks[194].x = 810;
    game->bricks[194].y = 105;
    game->bricks[194].w = 30;
    game->bricks[194].h = 30;
    game->bricks[194].name = "wall";
    game->bricks[195].x = 780;
    game->bricks[195].y = 105;
    game->bricks[195].w = 30;
    game->bricks[195].h = 30;
    game->bricks[195].name = "wall";
    game->bricks[196].x = 750;
    game->bricks[196].y = 105;
    game->bricks[196].w = 30;
    game->bricks[196].h = 30;
    game->bricks[196].name = "wall";
    game->bricks[197].x = 720;
    game->bricks[197].y = 105;
    game->bricks[197].w = 30;
    game->bricks[197].h = 30;
    game->bricks[197].name = "wall";
    game->bricks[198].x = 690;
    game->bricks[198].y = 105;
    game->bricks[198].w = 30;
    game->bricks[198].h = 30;
    game->bricks[198].name = "wall";
    game->bricks[199].x = 660;
    game->bricks[199].y = 105;
    game->bricks[199].w = 30;
    game->bricks[199].h = 30;
    game->bricks[199].name = "wall";
    game->bricks[200].x = 540;
    game->bricks[200].y = 225;
    game->bricks[200].w = 30;
    game->bricks[200].h = 30;
    game->bricks[200].name = "wall";
    game->bricks[201].x = 1020;
    game->bricks[201].y = 435;
    game->bricks[201].w = 60;
    game->bricks[201].h = 60;
    game->bricks[201].name = "fire";
    game->bricks[202].x = 1080;
    game->bricks[202].y = 435;
    game->bricks[202].w = 60;
    game->bricks[202].h = 60;
    game->bricks[202].name = "fire";
    game->bricks[203].x = 0;
    game->bricks[203].y = 655;
    game->bricks[203].w = 45;
    game->bricks[203].h = 45;
    game->bricks[203].name = "fire";
    game->bricks[204].x = 45;
    game->bricks[204].y = 655;
    game->bricks[204].w = 45;
    game->bricks[204].h = 45;
    game->bricks[204].name = "fire";
    game->bricks[205].x = 90;
    game->bricks[205].y = 655;
    game->bricks[205].w = 45;
    game->bricks[205].h = 45;
    game->bricks[205].name = "fire";
    game->bricks[206].x = 135;
    game->bricks[206].y = 655;
    game->bricks[206].w = 45;
    game->bricks[206].h = 45;
    game->bricks[206].name = "fire";
    game->bricks[207].x = 180;
    game->bricks[207].y = 655;
    game->bricks[207].w = 45;
    game->bricks[207].h = 45;
    game->bricks[207].name = "fire";
    game->bricks[208].x = 225;
    game->bricks[208].y = 655;
    game->bricks[208].w = 45;
    game->bricks[208].h = 45;
    game->bricks[208].name = "fire";
    game->bricks[209].x = 270;
    game->bricks[209].y = 655;
    game->bricks[209].w = 45;
    game->bricks[209].h = 45;
    game->bricks[209].name = "fire";
    game->bricks[210].x = 315;
    game->bricks[210].y = 655;
    game->bricks[210].w = 45;
    game->bricks[210].h = 45;
    game->bricks[210].name = "fire";
    game->bricks[211].x = 360;
    game->bricks[211].y = 655;
    game->bricks[211].w = 45;
    game->bricks[211].h = 45;
    game->bricks[211].name = "fire";
    game->bricks[212].x = 405;
    game->bricks[212].y = 655;
    game->bricks[212].w = 45;
    game->bricks[212].h = 45;
    game->bricks[212].name = "fire";
    game->bricks[213].x = 450;
    game->bricks[213].y = 655;
    game->bricks[213].w = 45;
    game->bricks[213].h = 45;
    game->bricks[213].name = "fire";
    game->bricks[214].x = 495;
    game->bricks[214].y = 655;
    game->bricks[214].w = 45;
    game->bricks[214].h = 45;
    game->bricks[214].name = "fire";
    game->bricks[215].x = 540;
    game->bricks[215].y = 655;
    game->bricks[215].w = 45;
    game->bricks[215].h = 45;
    game->bricks[215].name = "fire";
    game->bricks[216].x = 585;
    game->bricks[216].y = 655;
    game->bricks[216].w = 45;
    game->bricks[216].h = 45;
    game->bricks[216].name = "fire";
    game->bricks[217].x = 630;
    game->bricks[217].y = 655;
    game->bricks[217].w = 45;
    game->bricks[217].h = 45;
    game->bricks[217].name = "fire";
    game->bricks[218].x = 675;
    game->bricks[218].y = 655;
    game->bricks[218].w = 45;
    game->bricks[218].h = 45;
    game->bricks[218].name = "fire";
    game->bricks[219].x = 720;
    game->bricks[219].y = 655;
    game->bricks[219].w = 45;
    game->bricks[219].h = 45;
    game->bricks[219].name = "fire";
    game->bricks[220].x = 765;
    game->bricks[220].y = 655;
    game->bricks[220].w = 45;
    game->bricks[220].h = 45;
    game->bricks[220].name = "fire";
    game->bricks[221].x = 810;
    game->bricks[221].y = 655;
    game->bricks[221].w = 45;
    game->bricks[221].h = 45;
    game->bricks[221].name = "fire";
    game->bricks[222].x = 855;
    game->bricks[222].y = 655;
    game->bricks[222].w = 45;
    game->bricks[222].h = 45;
    game->bricks[222].name = "fire";
    game->bricks[223].x = 900;
    game->bricks[223].y = 655;
    game->bricks[223].w = 45;
    game->bricks[223].h = 45;
    game->bricks[223].name = "fire";
    game->bricks[224].x = 945;
    game->bricks[224].y = 655;
    game->bricks[224].w = 45;
    game->bricks[224].h = 45;
    game->bricks[224].name = "fire";
    game->bricks[225].x = 990;
    game->bricks[225].y = 655;
    game->bricks[225].w = 45;
    game->bricks[225].h = 45;
    game->bricks[225].name = "fire";
    game->bricks[226].x = 1035;
    game->bricks[226].y = 655;
    game->bricks[226].w = 45;
    game->bricks[226].h = 45;
    game->bricks[226].name = "fire";
    game->bricks[227].x = 1080;
    game->bricks[227].y = 655;
    game->bricks[227].w = 45;
    game->bricks[227].h = 45;
    game->bricks[227].name = "fire";
    game->bricks[228].x = 1125;
    game->bricks[228].y = 655;
    game->bricks[228].w = 45;
    game->bricks[228].h = 45;
    game->bricks[228].name = "fire";
    game->bricks[229].x = 1170;
    game->bricks[229].y = 655;
    game->bricks[229].w = 45;
    game->bricks[229].h = 45;
    game->bricks[229].name = "fire";
    game->bricks[230].x = 1215;
    game->bricks[230].y = 655;
    game->bricks[230].w = 45;
    game->bricks[230].h = 45;
    game->bricks[230].name = "fire";
    game->bricks[231].x = 1260;
    game->bricks[231].y = 655;
    game->bricks[231].w = 45;
    game->bricks[231].h = 45;
    game->bricks[231].name = "fire";
    game->bricks[232].x = 1305;
    game->bricks[232].y = 655;
    game->bricks[232].w = 45;
    game->bricks[232].h = 45;
    game->bricks[232].name = "fire";
    game->bricks[233].x = 1350;
    game->bricks[233].y = 655;
    game->bricks[233].w = 45;
    game->bricks[233].h = 45;
    game->bricks[233].name = "fire";
    game->bricks[234].x = 1395;
    game->bricks[234].y = 655;
    game->bricks[234].w = 45;
    game->bricks[234].h = 45;
    game->bricks[234].name = "fire";
    game->bricks[235].x = 1440;
    game->bricks[235].y = 655;
    game->bricks[235].w = 45;
    game->bricks[235].h = 45;
    game->bricks[235].name = "fire";
    game->bricks[236].x = 1485;
    game->bricks[236].y = 655;
    game->bricks[236].w = 45;
    game->bricks[236].h = 45;
    game->bricks[236].name = "fire";
    game->bricks[237].x = 1530;
    game->bricks[237].y = 655;
    game->bricks[237].w = 45;
    game->bricks[237].h = 45;
    game->bricks[237].name = "fire";
    game->bricks[238].x = 2295;
    game->bricks[238].y = 655;
    game->bricks[238].w = 45;
    game->bricks[238].h = 45;
    game->bricks[238].name = "fire";
    game->bricks[239].x = 1575;
    game->bricks[239].y = 655;
    game->bricks[239].w = 45;
    game->bricks[239].h = 45;
    game->bricks[239].name = "fire";
    game->bricks[240].x = 1620;
    game->bricks[240].y = 655;
    game->bricks[240].w = 45;
    game->bricks[240].h = 45;
    game->bricks[240].name = "fire";
    game->bricks[241].x = 1665;
    game->bricks[241].y = 655;
    game->bricks[241].w = 45;
    game->bricks[241].h = 45;
    game->bricks[241].name = "fire";
    game->bricks[242].x = 1710;
    game->bricks[242].y = 655;
    game->bricks[242].w = 45;
    game->bricks[242].h = 45;
    game->bricks[242].name = "fire";
    game->bricks[243].x = 1755;
    game->bricks[243].y = 655;
    game->bricks[243].w = 45;
    game->bricks[243].h = 45;
    game->bricks[243].name = "fire";
    game->bricks[244].x = 1800;
    game->bricks[244].y = 655;
    game->bricks[244].w = 45;
    game->bricks[244].h = 45;
    game->bricks[244].name = "fire";
    game->bricks[245].x = 1845;
    game->bricks[245].y = 655;
    game->bricks[245].w = 45;
    game->bricks[245].h = 45;
    game->bricks[245].name = "fire";
    game->bricks[246].x = 1890;
    game->bricks[246].y = 655;
    game->bricks[246].w = 45;
    game->bricks[246].h = 45;
    game->bricks[246].name = "fire";
    game->bricks[247].x = 1935;
    game->bricks[247].y = 655;
    game->bricks[247].w = 45;
    game->bricks[247].h = 45;
    game->bricks[247].name = "fire";
    game->bricks[248].x = 1980;
    game->bricks[248].y = 655;
    game->bricks[248].w = 45;
    game->bricks[248].h = 45;
    game->bricks[248].name = "fire";
    game->bricks[249].x = 2025;
    game->bricks[249].y = 655;
    game->bricks[249].w = 45;
    game->bricks[249].h = 45;
    game->bricks[249].name = "fire";
    game->bricks[250].x = 2250;
    game->bricks[250].y = 655;
    game->bricks[250].w = 45;
    game->bricks[250].h = 45;
    game->bricks[250].name = "fire";
    game->bricks[251].x = 2205;
    game->bricks[251].y = 655;
    game->bricks[251].w = 45;
    game->bricks[251].h = 45;
    game->bricks[251].name = "fire";
    game->bricks[252].x = 2160;
    game->bricks[252].y = 655;
    game->bricks[252].w = 45;
    game->bricks[252].h = 45;
    game->bricks[252].name = "fire";
    game->bricks[253].x = 2115;
    game->bricks[253].y = 655;
    game->bricks[253].w = 45;
    game->bricks[253].h = 45;
    game->bricks[253].name = "fire";
    game->bricks[254].x = 2070;
    game->bricks[254].y = 655;
    game->bricks[254].w = 45;
    game->bricks[254].h = 45;
    game->bricks[254].name = "fire";
    game->bricks[255].x = -50;
    game->bricks[255].y = 0;
    game->bricks[255].w = 50;
    game->bricks[255].h = 50;
    game->bricks[255].name = "wall";
    game->bricks[256].x = -50;
    game->bricks[256].y = 50;
    game->bricks[256].w = 50;
    game->bricks[256].h = 50;
    game->bricks[256].name = "wall";
    game->bricks[257].x = -50;
    game->bricks[257].y = 100;
    game->bricks[257].w = 50;
    game->bricks[257].h = 50;
    game->bricks[257].name = "wall";
    game->bricks[258].x = -50;
    game->bricks[258].y = 150;
    game->bricks[258].w = 50;
    game->bricks[258].h = 50;
    game->bricks[258].name = "wall";
    game->bricks[259].x = -50;
    game->bricks[259].y = 200;
    game->bricks[259].w = 50;
    game->bricks[259].h = 50;
    game->bricks[259].name = "wall";
    game->bricks[260].x = -50;
    game->bricks[260].y = 250;
    game->bricks[260].w = 50;
    game->bricks[260].h = 50;
    game->bricks[260].name = "wall";
    game->bricks[261].x = -50;
    game->bricks[261].y = 300;
    game->bricks[261].w = 50;
    game->bricks[261].h = 50;
    game->bricks[261].name = "wall";
    game->bricks[262].x = -50;
    game->bricks[262].y = 350;
    game->bricks[262].w = 50;
    game->bricks[262].h = 50;
    game->bricks[262].name = "wall";
    game->bricks[263].x = -50;
    game->bricks[263].y = 400;
    game->bricks[263].w = 50;
    game->bricks[263].h = 50;
    game->bricks[263].name = "wall";
    game->bricks[264].x = -50;
    game->bricks[264].y = 450;
    game->bricks[264].w = 50;
    game->bricks[264].h = 50;
    game->bricks[264].name = "wall";
    game->bricks[265].x = -50;
    game->bricks[265].y = 500;
    game->bricks[265].w = 50;
    game->bricks[265].h = 50;
    game->bricks[265].name = "wall";
    game->bricks[266].x = -50;
    game->bricks[266].y = 550;
    game->bricks[266].w = 50;
    game->bricks[266].h = 50;
    game->bricks[266].name = "wall";
    game->bricks[267].x = -50;
    game->bricks[267].y = 600;
    game->bricks[267].w = 50;
    game->bricks[267].h = 50;
    game->bricks[267].name = "wall";
    game->bricks[268].x = -50;
    game->bricks[268].y = 650;
    game->bricks[268].w = 50;
    game->bricks[268].h = 50;
    game->bricks[268].name = "wall";
    game->bricks[269].x = 580;
    game->bricks[269].y = 465;
    game->bricks[269].w = 30;
    game->bricks[269].h = 30;
    game->bricks[269].name = "wall";
    game->bricks[270].x = 610;
    game->bricks[270].y = 465;
    game->bricks[270].w = 30;
    game->bricks[270].h = 30;
    game->bricks[270].name = "wall";
    game->bricks[271].x = 640;
    game->bricks[271].y = 465;
    game->bricks[271].w = 30;
    game->bricks[271].h = 30;
    game->bricks[271].name = "wall";

}

void loadMap3 (GameState *game)
{
    game->man.x = 50;
    game->man.y = 100;
    game->man.dx = 0.1;
    game->man.dy = 0;
    game->man.onLedge = 1;
    game->man.animFrame = 2;
    game->man.facingLeft = 0;
    game->man.mode = 0;

    int i=0;
    for(i=0; i<10; i++)
    {
        game->enemies[i].isDead = 1;
    }
    for(i=0; i<300; i++)
    {
        game->bricks[i].name = NULL;
    }

    game->enemies[0].w = 39;
    game->enemies[0].h = 50;
    game->enemies[0].x = 350;
    game->enemies[0].y = 150;
    game->enemies[0].dy = 0;
    game->enemies[0].dx = 0.1;
    game->enemies[0].hp = 100;
    game->enemies[0].maxhp = 100;
    game->enemies[0].isDead = 0;
    game->enemies[0].animFrame = 0;
    game->enemies[0].facingLeft = 0;
    game->enemies[0].name = "walker";

    game->enemies[1].w = 39;
    game->enemies[1].h = 50;
    game->enemies[1].x = 350;
    game->enemies[1].y = 350;
    game->enemies[1].dy = 0;
    game->enemies[1].dx = 0.1;
    game->enemies[1].hp = 100;
    game->enemies[1].maxhp = 100;
    game->enemies[1].isDead = 0;
    game->enemies[1].animFrame = 0;
    game->enemies[1].facingLeft = 0;
    game->enemies[1].name = "walker";

    game->enemies[2].w = 39;
    game->enemies[2].h = 50;
    game->enemies[2].x = 450;
    game->enemies[2].y = 50;
    game->enemies[2].dy = 0;
    game->enemies[2].dx = 0.1;
    game->enemies[2].hp = 100;
    game->enemies[2].maxhp = 100;
    game->enemies[2].isDead = 0;
    game->enemies[2].animFrame = 0;
    game->enemies[2].facingLeft = 0;
    game->enemies[2].name = "walker";

    game->enemies[3].w = 39;
    game->enemies[3].h = 50;
    game->enemies[3].x = 450;
    game->enemies[3].y = 280;
    game->enemies[3].dy = 0;
    game->enemies[3].dx = 0.1;
    game->enemies[3].hp = 100;
    game->enemies[3].maxhp = 100;
    game->enemies[3].isDead = 0;
    game->enemies[3].animFrame = 0;
    game->enemies[3].facingLeft = 0;
    game->enemies[3].name = "walker";

    game->enemies[4].w = 39;
    game->enemies[4].h = 50;
    game->enemies[4].x = 450;
    game->enemies[4].y = 510;
    game->enemies[4].dy = 0;
    game->enemies[4].dx = 0.1;
    game->enemies[4].hp = 100;
    game->enemies[4].maxhp = 100;
    game->enemies[4].isDead = 0;
    game->enemies[4].animFrame = 0;
    game->enemies[4].facingLeft = 0;
    game->enemies[4].name = "walker";

    game->enemies[5].w = 78;
    game->enemies[5].h = 100;
    game->enemies[5].x = 1300;
    game->enemies[5].y = 550;
    game->enemies[5].dy = 0;
    game->enemies[5].dx = 0.1;
    game->enemies[5].hp = 500;
    game->enemies[5].maxhp = 500;
    game->enemies[5].isDead = 0;
    game->enemies[5].animFrame = 0;
    game->enemies[5].facingLeft = 0;
    game->enemies[5].name = "shooter";

    game->enemies[6].w = 78;
    game->enemies[6].h = 100;
    game->enemies[6].x = 2000;
    game->enemies[6].y = 350;
    game->enemies[6].dy = 0;
    game->enemies[6].dx = 0.1;
    game->enemies[6].hp = 500;
    game->enemies[6].maxhp = 500;
    game->enemies[6].isDead = 0;
    game->enemies[6].animFrame = 0;
    game->enemies[6].facingLeft = 0;
    game->enemies[6].name = "shooter";

    game->enemies[7].w = 62;
    game->enemies[7].h = 80;
    game->enemies[7].x = 2310;
    game->enemies[7].y = 475;
    game->enemies[7].dy = 0;
    game->enemies[7].dx = 0.1;
    game->enemies[7].hp = 400;
    game->enemies[7].maxhp = 400;
    game->enemies[7].isDead = 0;
    game->enemies[7].animFrame = 0;
    game->enemies[7].facingLeft = 0;
    game->enemies[7].name = "shooter";


    game->bricks[0].x = 0;
    game->bricks[0].y = 180;
    game->bricks[0].w = 30;
    game->bricks[0].h = 30;
    game->bricks[0].name = "wall";
    game->bricks[1].x = 30;
    game->bricks[1].y = 180;
    game->bricks[1].w = 30;
    game->bricks[1].h = 30;
    game->bricks[1].name = "wall";
    game->bricks[2].x = 60;
    game->bricks[2].y = 180;
    game->bricks[2].w = 30;
    game->bricks[2].h = 30;
    game->bricks[2].name = "wall";
    game->bricks[3].x = 90;
    game->bricks[3].y = 180;
    game->bricks[3].w = 30;
    game->bricks[3].h = 30;
    game->bricks[3].name = "wall";
    game->bricks[4].x = 120;
    game->bricks[4].y = 180;
    game->bricks[4].w = 30;
    game->bricks[4].h = 30;
    game->bricks[4].name = "wall";
    game->bricks[5].x = 150;
    game->bricks[5].y = 180;
    game->bricks[5].w = 30;
    game->bricks[5].h = 30;
    game->bricks[5].name = "wall";
    game->bricks[6].x = 0;
    game->bricks[6].y = 150;
    game->bricks[6].w = 30;
    game->bricks[6].h = 30;
    game->bricks[6].name = "wall";
    game->bricks[7].x = 0;
    game->bricks[7].y = 120;
    game->bricks[7].w = 30;
    game->bricks[7].h = 30;
    game->bricks[7].name = "wall";
    game->bricks[8].x = 0;
    game->bricks[8].y = 90;
    game->bricks[8].w = 30;
    game->bricks[8].h = 30;
    game->bricks[8].name = "wall";
    game->bricks[9].x = 0;
    game->bricks[9].y = 60;
    game->bricks[9].w = 30;
    game->bricks[9].h = 30;
    game->bricks[9].name = "wall";
    game->bricks[10].x = 0;
    game->bricks[10].y = 30;
    game->bricks[10].w = 30;
    game->bricks[10].h = 30;
    game->bricks[10].name = "wall";
    game->bricks[11].x = 0;
    game->bricks[11].y = 0;
    game->bricks[11].w = 30;
    game->bricks[11].h = 30;
    game->bricks[11].name = "wall";
    game->bricks[12].x = 30;
    game->bricks[12].y = 0;
    game->bricks[12].w = 30;
    game->bricks[12].h = 30;
    game->bricks[12].name = "wall";
    game->bricks[13].x = 60;
    game->bricks[13].y = 0;
    game->bricks[13].w = 30;
    game->bricks[13].h = 30;
    game->bricks[13].name = "wall";
    game->bricks[14].x = 90;
    game->bricks[14].y = 0;
    game->bricks[14].w = 30;
    game->bricks[14].h = 30;
    game->bricks[14].name = "wall";
    game->bricks[15].x = 120;
    game->bricks[15].y = 0;
    game->bricks[15].w = 30;
    game->bricks[15].h = 30;
    game->bricks[15].name = "wall";
    game->bricks[16].x = 150;
    game->bricks[16].y = 0;
    game->bricks[16].w = 30;
    game->bricks[16].h = 30;
    game->bricks[16].name = "wall";
    game->bricks[17].x = 180;
    game->bricks[17].y = 0;
    game->bricks[17].w = 30;
    game->bricks[17].h = 30;
    game->bricks[17].name = "wall";
    game->bricks[18].x = 210;
    game->bricks[18].y = 0;
    game->bricks[18].w = 30;
    game->bricks[18].h = 30;
    game->bricks[18].name = "wall";
    game->bricks[19].x = 240;
    game->bricks[19].y = 0;
    game->bricks[19].w = 30;
    game->bricks[19].h = 30;
    game->bricks[19].name = "wall";
    game->bricks[20].x = 270;
    game->bricks[20].y = 0;
    game->bricks[20].w = 30;
    game->bricks[20].h = 30;
    game->bricks[20].name = "wall";
    game->bricks[21].x = 300;
    game->bricks[21].y = 0;
    game->bricks[21].w = 30;
    game->bricks[21].h = 30;
    game->bricks[21].name = "wall";
    game->bricks[22].x = 300;
    game->bricks[22].y = 30;
    game->bricks[22].w = 30;
    game->bricks[22].h = 30;
    game->bricks[22].name = "wall";
    game->bricks[23].x = 300;
    game->bricks[23].y = 60;
    game->bricks[23].w = 30;
    game->bricks[23].h = 30;
    game->bricks[23].name = "wall";
    game->bricks[24].x = 300;
    game->bricks[24].y = 90;
    game->bricks[24].w = 30;
    game->bricks[24].h = 30;
    game->bricks[24].name = "wall";
    game->bricks[25].x = 300;
    game->bricks[25].y = 120;
    game->bricks[25].w = 30;
    game->bricks[25].h = 30;
    game->bricks[25].name = "wall";
    game->bricks[26].x = 300;
    game->bricks[26].y = 150;
    game->bricks[26].w = 30;
    game->bricks[26].h = 30;
    game->bricks[26].name = "wall";
    game->bricks[27].x = 300;
    game->bricks[27].y = 180;
    game->bricks[27].w = 30;
    game->bricks[27].h = 30;
    game->bricks[27].name = "wall";
    game->bricks[28].x = 300;
    game->bricks[28].y = 210;
    game->bricks[28].w = 30;
    game->bricks[28].h = 30;
    game->bricks[28].name = "wall";
    game->bricks[29].x = 300;
    game->bricks[29].y = 240;
    game->bricks[29].w = 30;
    game->bricks[29].h = 30;
    game->bricks[29].name = "wall";
    game->bricks[30].x = 300;
    game->bricks[30].y = 270;
    game->bricks[30].w = 30;
    game->bricks[30].h = 30;
    game->bricks[30].name = "wall";
    game->bricks[31].x = 150;
    game->bricks[31].y = 210;
    game->bricks[31].w = 30;
    game->bricks[31].h = 30;
    game->bricks[31].name = "wall";
    game->bricks[32].x = 150;
    game->bricks[32].y = 240;
    game->bricks[32].w = 30;
    game->bricks[32].h = 30;
    game->bricks[32].name = "wall";
    game->bricks[33].x = 150;
    game->bricks[33].y = 270;
    game->bricks[33].w = 30;
    game->bricks[33].h = 30;
    game->bricks[33].name = "wall";
    game->bricks[34].x = 150;
    game->bricks[34].y = 300;
    game->bricks[34].w = 30;
    game->bricks[34].h = 30;
    game->bricks[34].name = "wall";
    game->bricks[35].x = 150;
    game->bricks[35].y = 330;
    game->bricks[35].w = 30;
    game->bricks[35].h = 30;
    game->bricks[35].name = "wall";
    game->bricks[36].x = 150;
    game->bricks[36].y = 360;
    game->bricks[36].w = 30;
    game->bricks[36].h = 30;
    game->bricks[36].name = "wall";
    game->bricks[37].x = 150;
    game->bricks[37].y = 390;
    game->bricks[37].w = 30;
    game->bricks[37].h = 30;
    game->bricks[37].name = "wall";
    game->bricks[38].x = 150;
    game->bricks[38].y = 420;
    game->bricks[38].w = 30;
    game->bricks[38].h = 30;
    game->bricks[38].name = "wall";
    game->bricks[39].x = 150;
    game->bricks[39].y = 450;
    game->bricks[39].w = 30;
    game->bricks[39].h = 30;
    game->bricks[39].name = "wall";
    game->bricks[40].x = 150;
    game->bricks[40].y = 480;
    game->bricks[40].w = 30;
    game->bricks[40].h = 30;
    game->bricks[40].name = "wall";
    game->bricks[41].x = 150;
    game->bricks[41].y = 510;
    game->bricks[41].w = 30;
    game->bricks[41].h = 30;
    game->bricks[41].name = "wall";
    game->bricks[42].x = 300;
    game->bricks[42].y = 300;
    game->bricks[42].w = 30;
    game->bricks[42].h = 30;
    game->bricks[42].name = "wall";
    game->bricks[43].x = 300;
    game->bricks[43].y = 330;
    game->bricks[43].w = 30;
    game->bricks[43].h = 30;
    game->bricks[43].name = "wall";
    game->bricks[44].x = 150;
    game->bricks[44].y = 540;
    game->bricks[44].w = 30;
    game->bricks[44].h = 30;
    game->bricks[44].name = "wall";
    game->bricks[45].x = 150;
    game->bricks[45].y = 570;
    game->bricks[45].w = 30;
    game->bricks[45].h = 30;
    game->bricks[45].name = "wall";
    game->bricks[46].x = 150;
    game->bricks[46].y = 600;
    game->bricks[46].w = 30;
    game->bricks[46].h = 30;
    game->bricks[46].name = "wall";
    game->bricks[47].x = 150;
    game->bricks[47].y = 630;
    game->bricks[47].w = 30;
    game->bricks[47].h = 30;
    game->bricks[47].name = "wall";
    game->bricks[48].x = 150;
    game->bricks[48].y = 690;
    game->bricks[48].w = 30;
    game->bricks[48].h = 30;
    game->bricks[48].name = "wall";
    game->bricks[49].x = 180;
    game->bricks[49].y = 690;
    game->bricks[49].w = 30;
    game->bricks[49].h = 30;
    game->bricks[49].name = "wall";
    game->bricks[50].x = 210;
    game->bricks[50].y = 690;
    game->bricks[50].w = 30;
    game->bricks[50].h = 30;
    game->bricks[50].name = "wall";
    game->bricks[51].x = 240;
    game->bricks[51].y = 690;
    game->bricks[51].w = 30;
    game->bricks[51].h = 30;
    game->bricks[51].name = "wall";
    game->bricks[52].x = 270;
    game->bricks[52].y = 690;
    game->bricks[52].w = 30;
    game->bricks[52].h = 30;
    game->bricks[52].name = "wall";
    game->bricks[53].x = 300;
    game->bricks[53].y = 690;
    game->bricks[53].w = 30;
    game->bricks[53].h = 30;
    game->bricks[53].name = "wall";
    game->bricks[54].x = 300;
    game->bricks[54].y = 360;
    game->bricks[54].w = 30;
    game->bricks[54].h = 30;
    game->bricks[54].name = "wall";
    game->bricks[55].x = 300;
    game->bricks[55].y = 390;
    game->bricks[55].w = 30;
    game->bricks[55].h = 30;
    game->bricks[55].name = "wall";
    game->bricks[56].x = 300;
    game->bricks[56].y = 420;
    game->bricks[56].w = 30;
    game->bricks[56].h = 30;
    game->bricks[56].name = "wall";
    game->bricks[57].x = 300;
    game->bricks[57].y = 450;
    game->bricks[57].w = 30;
    game->bricks[57].h = 30;
    game->bricks[57].name = "wall";
    game->bricks[58].x = 300;
    game->bricks[58].y = 480;
    game->bricks[58].w = 30;
    game->bricks[58].h = 30;
    game->bricks[58].name = "wall";
    game->bricks[59].x = 300;
    game->bricks[59].y = 510;
    game->bricks[59].w = 30;
    game->bricks[59].h = 30;
    game->bricks[59].name = "wall";
    game->bricks[60].x = 300;
    game->bricks[60].y = 540;
    game->bricks[60].w = 30;
    game->bricks[60].h = 30;
    game->bricks[60].name = "wall";
    game->bricks[61].x = 330;
    game->bricks[61].y = 690;
    game->bricks[61].w = 30;
    game->bricks[61].h = 30;
    game->bricks[61].name = "wall";
    game->bricks[62].x = 360;
    game->bricks[62].y = 690;
    game->bricks[62].w = 30;
    game->bricks[62].h = 30;
    game->bricks[62].name = "wall";
    game->bricks[63].x = 300;
    game->bricks[63].y = 570;
    game->bricks[63].w = 30;
    game->bricks[63].h = 30;
    game->bricks[63].name = "wall";
    game->bricks[64].x = 300;
    game->bricks[64].y = 600;
    game->bricks[64].w = 30;
    game->bricks[64].h = 30;
    game->bricks[64].name = "wall";
    game->bricks[65].x = 210;
    game->bricks[65].y = 300;
    game->bricks[65].w = 30;
    game->bricks[65].h = 30;
    game->bricks[65].name = "wall";
    game->bricks[66].x = 180;
    game->bricks[66].y = 300;
    game->bricks[66].w = 30;
    game->bricks[66].h = 30;
    game->bricks[66].name = "wall";
    game->bricks[67].x = 270;
    game->bricks[67].y = 420;
    game->bricks[67].w = 30;
    game->bricks[67].h = 30;
    game->bricks[67].name = "wall";
    game->bricks[68].x = 240;
    game->bricks[68].y = 420;
    game->bricks[68].w = 30;
    game->bricks[68].h = 30;
    game->bricks[68].name = "wall";
    game->bricks[69].x = 210;
    game->bricks[69].y = 540;
    game->bricks[69].w = 30;
    game->bricks[69].h = 30;
    game->bricks[69].name = "wall";
    game->bricks[70].x = 180;
    game->bricks[70].y = 540;
    game->bricks[70].w = 30;
    game->bricks[70].h = 30;
    game->bricks[70].name = "wall";
    game->bricks[71].x = 390;
    game->bricks[71].y = 690;
    game->bricks[71].w = 30;
    game->bricks[71].h = 30;
    game->bricks[71].name = "wall";
    game->bricks[72].x = 420;
    game->bricks[72].y = 690;
    game->bricks[72].w = 30;
    game->bricks[72].h = 30;
    game->bricks[72].name = "wall";
    game->bricks[73].x = 450;
    game->bricks[73].y = 690;
    game->bricks[73].w = 30;
    game->bricks[73].h = 30;
    game->bricks[73].name = "wall";
    game->bricks[74].x = 480;
    game->bricks[74].y = 690;
    game->bricks[74].w = 30;
    game->bricks[74].h = 30;
    game->bricks[74].name = "wall";
    game->bricks[75].x = 510;
    game->bricks[75].y = 660;
    game->bricks[75].w = 30;
    game->bricks[75].h = 30;
    game->bricks[75].name = "wall";
    game->bricks[76].x = 510;
    game->bricks[76].y = 630;
    game->bricks[76].w = 30;
    game->bricks[76].h = 30;
    game->bricks[76].name = "wall";
    game->bricks[77].x = 510;
    game->bricks[77].y = 600;
    game->bricks[77].w = 30;
    game->bricks[77].h = 30;
    game->bricks[77].name = "wall";
    game->bricks[78].x = 510;
    game->bricks[78].y = 570;
    game->bricks[78].w = 30;
    game->bricks[78].h = 30;
    game->bricks[78].name = "wall";
    game->bricks[79].x = 510;
    game->bricks[79].y = 540;
    game->bricks[79].w = 30;
    game->bricks[79].h = 30;
    game->bricks[79].name = "wall";
    game->bricks[80].x = 510;
    game->bricks[80].y = 510;
    game->bricks[80].w = 30;
    game->bricks[80].h = 30;
    game->bricks[80].name = "wall";
    game->bricks[81].x = 510;
    game->bricks[81].y = 480;
    game->bricks[81].w = 30;
    game->bricks[81].h = 30;
    game->bricks[81].name = "wall";
    game->bricks[82].x = 510;
    game->bricks[82].y = 450;
    game->bricks[82].w = 30;
    game->bricks[82].h = 30;
    game->bricks[82].name = "wall";
    game->bricks[83].x = 510;
    game->bricks[83].y = 420;
    game->bricks[83].w = 30;
    game->bricks[83].h = 30;
    game->bricks[83].name = "wall";
    game->bricks[84].x = 510;
    game->bricks[84].y = 390;
    game->bricks[84].w = 30;
    game->bricks[84].h = 30;
    game->bricks[84].name = "wall";
    game->bricks[85].x = 510;
    game->bricks[85].y = 360;
    game->bricks[85].w = 30;
    game->bricks[85].h = 30;
    game->bricks[85].name = "wall";
    game->bricks[86].x = 510;
    game->bricks[86].y = 330;
    game->bricks[86].w = 30;
    game->bricks[86].h = 30;
    game->bricks[86].name = "wall";
    game->bricks[87].x = 510;
    game->bricks[87].y = 300;
    game->bricks[87].w = 30;
    game->bricks[87].h = 30;
    game->bricks[87].name = "wall";
    game->bricks[88].x = 510;
    game->bricks[88].y = 270;
    game->bricks[88].w = 30;
    game->bricks[88].h = 30;
    game->bricks[88].name = "wall";
    game->bricks[89].x = 510;
    game->bricks[89].y = 240;
    game->bricks[89].w = 30;
    game->bricks[89].h = 30;
    game->bricks[89].name = "wall";
    game->bricks[90].x = 510;
    game->bricks[90].y = 210;
    game->bricks[90].w = 30;
    game->bricks[90].h = 30;
    game->bricks[90].name = "wall";
    game->bricks[91].x = 510;
    game->bricks[91].y = 180;
    game->bricks[91].w = 30;
    game->bricks[91].h = 30;
    game->bricks[91].name = "wall";
    game->bricks[92].x = 510;
    game->bricks[92].y = 150;
    game->bricks[92].w = 30;
    game->bricks[92].h = 30;
    game->bricks[92].name = "wall";
    game->bricks[93].x = 510;
    game->bricks[93].y = 120;
    game->bricks[93].w = 30;
    game->bricks[93].h = 30;
    game->bricks[93].name = "wall";
    game->bricks[94].x = 510;
    game->bricks[94].y = 90;
    game->bricks[94].w = 30;
    game->bricks[94].h = 30;
    game->bricks[94].name = "wall";
    game->bricks[95].x = 390;
    game->bricks[95].y = 590;
    game->bricks[95].w = 30;
    game->bricks[95].h = 30;
    game->bricks[95].name = "wall";
    game->bricks[96].x = 420;
    game->bricks[96].y = 600;
    game->bricks[96].w = 30;
    game->bricks[96].h = 30;
    game->bricks[96].name = "wall";
    game->bricks[97].x = 450;
    game->bricks[97].y = 600;
    game->bricks[97].w = 30;
    game->bricks[97].h = 30;
    game->bricks[97].name = "wall";
    game->bricks[98].x = 450;
    game->bricks[98].y = 0;
    game->bricks[98].w = 30;
    game->bricks[98].h = 30;
    game->bricks[98].name = "wall";
    game->bricks[99].x = 480;
    game->bricks[99].y = 600;
    game->bricks[99].w = 30;
    game->bricks[99].h = 30;
    game->bricks[99].name = "wall";
    game->bricks[100].x = 420;
    game->bricks[100].y = 470;
    game->bricks[100].w = 30;
    game->bricks[100].h = 30;
    game->bricks[100].name = "wall";
    game->bricks[101].x = 390;
    game->bricks[101].y = 480;
    game->bricks[101].w = 30;
    game->bricks[101].h = 30;
    game->bricks[101].name = "wall";
    game->bricks[102].x = 360;
    game->bricks[102].y = 480;
    game->bricks[102].w = 30;
    game->bricks[102].h = 30;
    game->bricks[102].name = "wall";
    game->bricks[103].x = 480;
    game->bricks[103].y = 0;
    game->bricks[103].w = 30;
    game->bricks[103].h = 30;
    game->bricks[103].name = "wall";
    game->bricks[104].x = 330;
    game->bricks[104].y = 480;
    game->bricks[104].w = 30;
    game->bricks[104].h = 30;
    game->bricks[104].name = "wall";
    game->bricks[105].x = 510;
    game->bricks[105].y = 0;
    game->bricks[105].w = 30;
    game->bricks[105].h = 30;
    game->bricks[105].name = "wall";
    game->bricks[106].x = 480;
    game->bricks[106].y = 360;
    game->bricks[106].w = 30;
    game->bricks[106].h = 30;
    game->bricks[106].name = "wall";
    game->bricks[107].x = 540;
    game->bricks[107].y = 0;
    game->bricks[107].w = 30;
    game->bricks[107].h = 30;
    game->bricks[107].name = "wall";
    game->bricks[108].x = 450;
    game->bricks[108].y = 360;
    game->bricks[108].w = 30;
    game->bricks[108].h = 30;
    game->bricks[108].name = "wall";
    game->bricks[109].x = 420;
    game->bricks[109].y = 360;
    game->bricks[109].w = 30;
    game->bricks[109].h = 30;
    game->bricks[109].name = "wall";
    game->bricks[110].x = 570;
    game->bricks[110].y = 0;
    game->bricks[110].w = 30;
    game->bricks[110].h = 30;
    game->bricks[110].name = "wall";
    game->bricks[111].x = 600;
    game->bricks[111].y = 0;
    game->bricks[111].w = 30;
    game->bricks[111].h = 30;
    game->bricks[111].name = "wall";
    game->bricks[112].x = 390;
    game->bricks[112].y = 350;
    game->bricks[112].w = 30;
    game->bricks[112].h = 30;
    game->bricks[112].name = "wall";
    game->bricks[113].x = 420;
    game->bricks[113].y = 230;
    game->bricks[113].w = 30;
    game->bricks[113].h = 30;
    game->bricks[113].name = "wall";
    game->bricks[114].x = 390;
    game->bricks[114].y = 240;
    game->bricks[114].w = 30;
    game->bricks[114].h = 30;
    game->bricks[114].name = "wall";
    game->bricks[115].x = 360;
    game->bricks[115].y = 240;
    game->bricks[115].w = 30;
    game->bricks[115].h = 30;
    game->bricks[115].name = "wall";
    game->bricks[116].x = 330;
    game->bricks[116].y = 240;
    game->bricks[116].w = 30;
    game->bricks[116].h = 30;
    game->bricks[116].name = "wall";
    game->bricks[117].x = 630;
    game->bricks[117].y = 0;
    game->bricks[117].w = 30;
    game->bricks[117].h = 30;
    game->bricks[117].name = "wall";
    game->bricks[118].x = 330;
    game->bricks[118].y = 0;
    game->bricks[118].w = 30;
    game->bricks[118].h = 30;
    game->bricks[118].name = "wall";
    game->bricks[119].x = 360;
    game->bricks[119].y = 0;
    game->bricks[119].w = 30;
    game->bricks[119].h = 30;
    game->bricks[119].name = "wall";
    game->bricks[120].x = 390;
    game->bricks[120].y = 0;
    game->bricks[120].w = 30;
    game->bricks[120].h = 30;
    game->bricks[120].name = "wall";
    game->bricks[121].x = 420;
    game->bricks[121].y = 0;
    game->bricks[121].w = 30;
    game->bricks[121].h = 30;
    game->bricks[121].name = "wall";
    game->bricks[122].x = 660;
    game->bricks[122].y = 30;
    game->bricks[122].w = 30;
    game->bricks[122].h = 30;
    game->bricks[122].name = "wall";
    game->bricks[123].x = 660;
    game->bricks[123].y = 0;
    game->bricks[123].w = 30;
    game->bricks[123].h = 30;
    game->bricks[123].name = "wall";
    game->bricks[124].x = 660;
    game->bricks[124].y = 60;
    game->bricks[124].w = 30;
    game->bricks[124].h = 30;
    game->bricks[124].name = "wall";
    game->bricks[125].x = 660;
    game->bricks[125].y = 90;
    game->bricks[125].w = 30;
    game->bricks[125].h = 30;
    game->bricks[125].name = "wall";
    game->bricks[126].x = 660;
    game->bricks[126].y = 120;
    game->bricks[126].w = 30;
    game->bricks[126].h = 30;
    game->bricks[126].name = "wall";
    game->bricks[127].x = 660;
    game->bricks[127].y = 150;
    game->bricks[127].w = 30;
    game->bricks[127].h = 30;
    game->bricks[127].name = "wall";
    game->bricks[128].x = 660;
    game->bricks[128].y = 180;
    game->bricks[128].w = 30;
    game->bricks[128].h = 30;
    game->bricks[128].name = "wall";
    game->bricks[129].x = 660;
    game->bricks[129].y = 210;
    game->bricks[129].w = 30;
    game->bricks[129].h = 30;
    game->bricks[129].name = "wall";
    game->bricks[130].x = 660;
    game->bricks[130].y = 240;
    game->bricks[130].w = 30;
    game->bricks[130].h = 30;
    game->bricks[130].name = "wall";
    game->bricks[131].x = 660;
    game->bricks[131].y = 270;
    game->bricks[131].w = 30;
    game->bricks[131].h = 30;
    game->bricks[131].name = "wall";
    game->bricks[132].x = 660;
    game->bricks[132].y = 300;
    game->bricks[132].w = 30;
    game->bricks[132].h = 30;
    game->bricks[132].name = "wall";
    game->bricks[133].x = 660;
    game->bricks[133].y = 330;
    game->bricks[133].w = 30;
    game->bricks[133].h = 30;
    game->bricks[133].name = "wall";
    game->bricks[134].x = 660;
    game->bricks[134].y = 360;
    game->bricks[134].w = 30;
    game->bricks[134].h = 30;
    game->bricks[134].name = "wall";
    game->bricks[135].x = 660;
    game->bricks[135].y = 390;
    game->bricks[135].w = 30;
    game->bricks[135].h = 30;
    game->bricks[135].name = "wall";
    game->bricks[136].x = 660;
    game->bricks[136].y = 420;
    game->bricks[136].w = 30;
    game->bricks[136].h = 30;
    game->bricks[136].name = "wall";
    game->bricks[137].x = 660;
    game->bricks[137].y = 450;
    game->bricks[137].w = 30;
    game->bricks[137].h = 30;
    game->bricks[137].name = "wall";
    game->bricks[138].x = 660;
    game->bricks[138].y = 480;
    game->bricks[138].w = 30;
    game->bricks[138].h = 30;
    game->bricks[138].name = "wall";
    game->bricks[139].x = 660;
    game->bricks[139].y = 510;
    game->bricks[139].w = 30;
    game->bricks[139].h = 30;
    game->bricks[139].name = "wall";
    game->bricks[140].x = 660;
    game->bricks[140].y = 540;
    game->bricks[140].w = 30;
    game->bricks[140].h = 30;
    game->bricks[140].name = "wall";
    game->bricks[141].x = 660;
    game->bricks[141].y = 570;
    game->bricks[141].w = 30;
    game->bricks[141].h = 30;
    game->bricks[141].name = "wall";
    game->bricks[142].x = 540;
    game->bricks[142].y = 660;
    game->bricks[142].w = 45;
    game->bricks[142].h = 45;
    game->bricks[142].name = "ground";
    game->bricks[143].x = 585;
    game->bricks[143].y = 660;
    game->bricks[143].w = 45;
    game->bricks[143].h = 45;
    game->bricks[143].name = "ground";
    game->bricks[144].x = 630;
    game->bricks[144].y = 660;
    game->bricks[144].w = 45;
    game->bricks[144].h = 45;
    game->bricks[144].name = "ground";
    game->bricks[145].x = 675;
    game->bricks[145].y = 660;
    game->bricks[145].w = 45;
    game->bricks[145].h = 45;
    game->bricks[145].name = "ground";
    game->bricks[146].x = 720;
    game->bricks[146].y = 660;
    game->bricks[146].w = 45;
    game->bricks[146].h = 45;
    game->bricks[146].name = "ground";
    game->bricks[147].x = 765;
    game->bricks[147].y = 660;
    game->bricks[147].w = 45;
    game->bricks[147].h = 45;
    game->bricks[147].name = "ground";
    game->bricks[148].x = 810;
    game->bricks[148].y = 660;
    game->bricks[148].w = 45;
    game->bricks[148].h = 45;
    game->bricks[148].name = "ground";
    game->bricks[149].x = 855;
    game->bricks[149].y = 660;
    game->bricks[149].w = 45;
    game->bricks[149].h = 45;
    game->bricks[149].name = "ground";
    game->bricks[150].x = 900;
    game->bricks[150].y = 660;
    game->bricks[150].w = 45;
    game->bricks[150].h = 45;
    game->bricks[150].name = "ground";
    game->bricks[151].x = 945;
    game->bricks[151].y = 660;
    game->bricks[151].w = 45;
    game->bricks[151].h = 45;
    game->bricks[151].name = "ground";
    game->bricks[152].x = 990;
    game->bricks[152].y = 660;
    game->bricks[152].w = 45;
    game->bricks[152].h = 45;
    game->bricks[152].name = "ground";
    game->bricks[153].x = 1215;
    game->bricks[153].y = 660;
    game->bricks[153].w = 45;
    game->bricks[153].h = 45;
    game->bricks[153].name = "ground";
    game->bricks[154].x = 1260;
    game->bricks[154].y = 660;
    game->bricks[154].w = 45;
    game->bricks[154].h = 45;
    game->bricks[154].name = "ground";
    game->bricks[155].x = 1305;
    game->bricks[155].y = 660;
    game->bricks[155].w = 45;
    game->bricks[155].h = 45;
    game->bricks[155].name = "ground";
    game->bricks[156].x = 1350;
    game->bricks[156].y = 660;
    game->bricks[156].w = 45;
    game->bricks[156].h = 45;
    game->bricks[156].name = "ground";
    game->bricks[157].x = 1395;
    game->bricks[157].y = 660;
    game->bricks[157].w = 45;
    game->bricks[157].h = 45;
    game->bricks[157].name = "ground";
    game->bricks[158].x = 1440;
    game->bricks[158].y = 660;
    game->bricks[158].w = 45;
    game->bricks[158].h = 45;
    game->bricks[158].name = "ground";
    game->bricks[159].x = 1035;
    game->bricks[159].y = 660;
    game->bricks[159].w = 45;
    game->bricks[159].h = 45;
    game->bricks[159].name = "fire";
    game->bricks[160].x = 1080;
    game->bricks[160].y = 660;
    game->bricks[160].w = 45;
    game->bricks[160].h = 45;
    game->bricks[160].name = "fire";
    game->bricks[161].x = 1125;
    game->bricks[161].y = 660;
    game->bricks[161].w = 45;
    game->bricks[161].h = 45;
    game->bricks[161].name = "fire";
    game->bricks[162].x = 1170;
    game->bricks[162].y = 660;
    game->bricks[162].w = 45;
    game->bricks[162].h = 45;
    game->bricks[162].name = "fire";
    game->bricks[163].x = 1485;
    game->bricks[163].y = 660;
    game->bricks[163].w = 45;
    game->bricks[163].h = 45;
    game->bricks[163].name = "fire";
    game->bricks[164].x = 1530;
    game->bricks[164].y = 660;
    game->bricks[164].w = 45;
    game->bricks[164].h = 45;
    game->bricks[164].name = "fire";
    game->bricks[165].x = 1575;
    game->bricks[165].y = 660;
    game->bricks[165].w = 45;
    game->bricks[165].h = 45;
    game->bricks[165].name = "fire";
    game->bricks[166].x = 1620;
    game->bricks[166].y = 660;
    game->bricks[166].w = 45;
    game->bricks[166].h = 45;
    game->bricks[166].name = "fire";
    game->bricks[167].x = 1215;
    game->bricks[167].y = 630;
    game->bricks[167].w = 30;
    game->bricks[167].h = 30;
    game->bricks[167].name = "wall";
    game->bricks[168].x = 1455;
    game->bricks[168].y = 630;
    game->bricks[168].w = 30;
    game->bricks[168].h = 30;
    game->bricks[168].name = "wall";
    game->bricks[169].x = 1005;
    game->bricks[169].y = 630;
    game->bricks[169].w = 30;
    game->bricks[169].h = 30;
    game->bricks[169].name = "wall";
    game->bricks[170].x = 1665;
    game->bricks[170].y = 660;
    game->bricks[170].w = 45;
    game->bricks[170].h = 45;
    game->bricks[170].name = "ground";
    game->bricks[171].x = 1710;
    game->bricks[171].y = 660;
    game->bricks[171].w = 45;
    game->bricks[171].h = 45;
    game->bricks[171].name = "ground";
    game->bricks[172].x = 1755;
    game->bricks[172].y = 660;
    game->bricks[172].w = 45;
    game->bricks[172].h = 45;
    game->bricks[172].name = "ground";
    game->bricks[173].x = 1665;
    game->bricks[173].y = 630;
    game->bricks[173].w = 30;
    game->bricks[173].h = 30;
    game->bricks[173].name = "wall";
    game->bricks[174].x = 1800;
    game->bricks[174].y = 660;
    game->bricks[174].w = 45;
    game->bricks[174].h = 45;
    game->bricks[174].name = "ground";
    game->bricks[175].x = 1845;
    game->bricks[175].y = 660;
    game->bricks[175].w = 45;
    game->bricks[175].h = 45;
    game->bricks[175].name = "ground";
    game->bricks[176].x = 1890;
    game->bricks[176].y = 660;
    game->bricks[176].w = 45;
    game->bricks[176].h = 45;
    game->bricks[176].name = "ground";
    game->bricks[177].x = 1935;
    game->bricks[177].y = 660;
    game->bricks[177].w = 45;
    game->bricks[177].h = 45;
    game->bricks[177].name = "ground";
    game->bricks[178].x = 1980;
    game->bricks[178].y = 660;
    game->bricks[178].w = 45;
    game->bricks[178].h = 45;
    game->bricks[178].name = "ground";
    game->bricks[179].x = 2025;
    game->bricks[179].y = 660;
    game->bricks[179].w = 45;
    game->bricks[179].h = 45;
    game->bricks[179].name = "ground";
    game->bricks[180].x = 2070;
    game->bricks[180].y = 660;
    game->bricks[180].w = 45;
    game->bricks[180].h = 45;
    game->bricks[180].name = "ground";
    game->bricks[181].x = 2115;
    game->bricks[181].y = 660;
    game->bricks[181].w = 45;
    game->bricks[181].h = 45;
    game->bricks[181].name = "ground";
    game->bricks[182].x = 2160;
    game->bricks[182].y = 660;
    game->bricks[182].w = 45;
    game->bricks[182].h = 45;
    game->bricks[182].name = "ground";
    game->bricks[183].x = 2205;
    game->bricks[183].y = 660;
    game->bricks[183].w = 45;
    game->bricks[183].h = 45;
    game->bricks[183].name = "ground";
    game->bricks[184].x = 2250;
    game->bricks[184].y = 660;
    game->bricks[184].w = 45;
    game->bricks[184].h = 45;
    game->bricks[184].name = "ground";
    game->bricks[185].x = 2295;
    game->bricks[185].y = 660;
    game->bricks[185].w = 45;
    game->bricks[185].h = 45;
    game->bricks[185].name = "ground";
    game->bricks[186].x = 2340;
    game->bricks[186].y = 660;
    game->bricks[186].w = 45;
    game->bricks[186].h = 45;
    game->bricks[186].name = "ground";
    game->bricks[187].x = 2385;
    game->bricks[187].y = 660;
    game->bricks[187].w = 45;
    game->bricks[187].h = 45;
    game->bricks[187].name = "ground";
    game->bricks[188].x = 1860;
    game->bricks[188].y = 570;
    game->bricks[188].w = 30;
    game->bricks[188].h = 30;
    game->bricks[188].name = "wall";
    game->bricks[189].x = 1860;
    game->bricks[189].y = 540;
    game->bricks[189].w = 30;
    game->bricks[189].h = 30;
    game->bricks[189].name = "wall";
    game->bricks[190].x = 1860;
    game->bricks[190].y = 510;
    game->bricks[190].w = 30;
    game->bricks[190].h = 30;
    game->bricks[190].name = "wall";
    game->bricks[191].x = 1860;
    game->bricks[191].y = 480;
    game->bricks[191].w = 30;
    game->bricks[191].h = 30;
    game->bricks[191].name = "wall";
    game->bricks[192].x = 1890;
    game->bricks[192].y = 480;
    game->bricks[192].w = 30;
    game->bricks[192].h = 30;
    game->bricks[192].name = "wall";
    game->bricks[193].x = 1920;
    game->bricks[193].y = 480;
    game->bricks[193].w = 30;
    game->bricks[193].h = 30;
    game->bricks[193].name = "wall";
    game->bricks[194].x = 1950;
    game->bricks[194].y = 480;
    game->bricks[194].w = 30;
    game->bricks[194].h = 30;
    game->bricks[194].name = "wall";
    game->bricks[195].x = 1980;
    game->bricks[195].y = 480;
    game->bricks[195].w = 30;
    game->bricks[195].h = 30;
    game->bricks[195].name = "wall";
    game->bricks[196].x = 2010;
    game->bricks[196].y = 480;
    game->bricks[196].w = 30;
    game->bricks[196].h = 30;
    game->bricks[196].name = "wall";
    game->bricks[197].x = 1860;
    game->bricks[197].y = 600;
    game->bricks[197].w = 30;
    game->bricks[197].h = 30;
    game->bricks[197].name = "wall";
    game->bricks[198].x = 2040;
    game->bricks[198].y = 480;
    game->bricks[198].w = 30;
    game->bricks[198].h = 30;
    game->bricks[198].name = "wall";
    game->bricks[199].x = 2070;
    game->bricks[199].y = 480;
    game->bricks[199].w = 30;
    game->bricks[199].h = 30;
    game->bricks[199].name = "wall";
    game->bricks[200].x = 2100;
    game->bricks[200].y = 480;
    game->bricks[200].w = 30;
    game->bricks[200].h = 30;
    game->bricks[200].name = "wall";
    game->bricks[201].x = 2400;
    game->bricks[201].y = 630;
    game->bricks[201].w = 30;
    game->bricks[201].h = 30;
    game->bricks[201].name = "wall";
    game->bricks[202].x = 2400;
    game->bricks[202].y = 600;
    game->bricks[202].w = 30;
    game->bricks[202].h = 30;
    game->bricks[202].name = "wall";
    game->bricks[203].x = 2400;
    game->bricks[203].y = 570;
    game->bricks[203].w = 30;
    game->bricks[203].h = 30;
    game->bricks[203].name = "wall";
    game->bricks[204].x = 2400;
    game->bricks[204].y = 540;
    game->bricks[204].w = 30;
    game->bricks[204].h = 30;
    game->bricks[204].name = "wall";
    game->bricks[205].x = 2400;
    game->bricks[205].y = 510;
    game->bricks[205].w = 30;
    game->bricks[205].h = 30;
    game->bricks[205].name = "wall";
    game->bricks[206].x = 2130;
    game->bricks[206].y = 480;
    game->bricks[206].w = 30;
    game->bricks[206].h = 30;
    game->bricks[206].name = "wall";
    game->bricks[207].x = 2160;
    game->bricks[207].y = 480;
    game->bricks[207].w = 30;
    game->bricks[207].h = 30;
    game->bricks[207].name = "wall";
    game->bricks[208].x = 2190;
    game->bricks[208].y = 480;
    game->bricks[208].w = 30;
    game->bricks[208].h = 30;
    game->bricks[208].name = "wall";
    game->bricks[209].x = 2220;
    game->bricks[209].y = 480;
    game->bricks[209].w = 30;
    game->bricks[209].h = 30;
    game->bricks[209].name = "wall";
    game->bricks[210].x = 2250;
    game->bricks[210].y = 480;
    game->bricks[210].w = 30;
    game->bricks[210].h = 30;
    game->bricks[210].name = "wall";
    game->bricks[211].x = 2280;
    game->bricks[211].y = 470;
    game->bricks[211].w = 30;
    game->bricks[211].h = 30;
    game->bricks[211].name = "wall";
    game->bricks[212].x = 2400;
    game->bricks[212].y = 480;
    game->bricks[212].w = 30;
    game->bricks[212].h = 30;
    game->bricks[212].name = "wall";
    game->bricks[213].x = 1980;
    game->bricks[213].y = 590;
    game->bricks[213].w = 30;
    game->bricks[213].h = 30;
    game->bricks[213].name = "wall";
    game->bricks[214].x = 2010;
    game->bricks[214].y = 600;
    game->bricks[214].w = 30;
    game->bricks[214].h = 30;
    game->bricks[214].name = "wall";
    game->bricks[215].x = 2040;
    game->bricks[215].y = 600;
    game->bricks[215].w = 30;
    game->bricks[215].h = 30;
    game->bricks[215].name = "wall";
    game->bricks[216].x = 2070;
    game->bricks[216].y = 600;
    game->bricks[216].w = 30;
    game->bricks[216].h = 30;
    game->bricks[216].name = "wall";
    game->bricks[217].x = 2100;
    game->bricks[217].y = 600;
    game->bricks[217].w = 30;
    game->bricks[217].h = 30;
    game->bricks[217].name = "wall";
    game->bricks[218].x = 2130;
    game->bricks[218].y = 600;
    game->bricks[218].w = 30;
    game->bricks[218].h = 30;
    game->bricks[218].name = "wall";
    game->bricks[219].x = 2160;
    game->bricks[219].y = 600;
    game->bricks[219].w = 30;
    game->bricks[219].h = 30;
    game->bricks[219].name = "wall";
    game->bricks[220].x = 2190;
    game->bricks[220].y = 600;
    game->bricks[220].w = 30;
    game->bricks[220].h = 30;
    game->bricks[220].name = "wall";
    game->bricks[221].x = 2220;
    game->bricks[221].y = 600;
    game->bricks[221].w = 30;
    game->bricks[221].h = 30;
    game->bricks[221].name = "wall";
    game->bricks[222].x = 2250;
    game->bricks[222].y = 600;
    game->bricks[222].w = 30;
    game->bricks[222].h = 30;
    game->bricks[222].name = "wall";
    game->bricks[223].x = 2280;
    game->bricks[223].y = 600;
    game->bricks[223].w = 30;
    game->bricks[223].h = 30;
    game->bricks[223].name = "wall";
    game->bricks[224].x = 2310;
    game->bricks[224].y = 600;
    game->bricks[224].w = 30;
    game->bricks[224].h = 30;
    game->bricks[224].name = "wall";
    game->bricks[225].x = 2340;
    game->bricks[225].y = 600;
    game->bricks[225].w = 30;
    game->bricks[225].h = 30;
    game->bricks[225].name = "wall";
    game->bricks[226].x = 2370;
    game->bricks[226].y = 600;
    game->bricks[226].w = 30;
    game->bricks[226].h = 30;
    game->bricks[226].name = "wall";
    game->bricks[227].x = 1860;
    game->bricks[227].y = 450;
    game->bricks[227].w = 30;
    game->bricks[227].h = 30;
    game->bricks[227].name = "wall";
    game->bricks[228].x = 1860;
    game->bricks[228].y = 420;
    game->bricks[228].w = 30;
    game->bricks[228].h = 30;
    game->bricks[228].name = "wall";
    game->bricks[229].x = 1890;
    game->bricks[229].y = 390;
    game->bricks[229].w = 30;
    game->bricks[229].h = 30;
    game->bricks[229].name = "wall";
    game->bricks[230].x = 1920;
    game->bricks[230].y = 360;
    game->bricks[230].w = 30;
    game->bricks[230].h = 30;
    game->bricks[230].name = "wall";
    game->bricks[231].x = 1950;
    game->bricks[231].y = 330;
    game->bricks[231].w = 30;
    game->bricks[231].h = 30;
    game->bricks[231].name = "wall";
    game->bricks[232].x = 1980;
    game->bricks[232].y = 300;
    game->bricks[232].w = 30;
    game->bricks[232].h = 30;
    game->bricks[232].name = "wall";
    game->bricks[233].x = 2010;
    game->bricks[233].y = 270;
    game->bricks[233].w = 30;
    game->bricks[233].h = 30;
    game->bricks[233].name = "wall";
    game->bricks[234].x = 2400;
    game->bricks[234].y = 450;
    game->bricks[234].w = 30;
    game->bricks[234].h = 30;
    game->bricks[234].name = "wall";
    game->bricks[235].x = 2400;
    game->bricks[235].y = 420;
    game->bricks[235].w = 30;
    game->bricks[235].h = 30;
    game->bricks[235].name = "wall";
    game->bricks[236].x = 2370;
    game->bricks[236].y = 390;
    game->bricks[236].w = 30;
    game->bricks[236].h = 30;
    game->bricks[236].name = "wall";
    game->bricks[237].x = 2340;
    game->bricks[237].y = 360;
    game->bricks[237].w = 30;
    game->bricks[237].h = 30;
    game->bricks[237].name = "wall";
    game->bricks[238].x = 2310;
    game->bricks[238].y = 330;
    game->bricks[238].w = 30;
    game->bricks[238].h = 30;
    game->bricks[238].name = "wall";
    game->bricks[239].x = 2280;
    game->bricks[239].y = 300;
    game->bricks[239].w = 30;
    game->bricks[239].h = 30;
    game->bricks[239].name = "wall";
    game->bricks[240].x = 2250;
    game->bricks[240].y = 270;
    game->bricks[240].w = 30;
    game->bricks[240].h = 30;
    game->bricks[240].name = "wall";
    game->bricks[241].x = 2220;
    game->bricks[241].y = 240;
    game->bricks[241].w = 30;
    game->bricks[241].h = 30;
    game->bricks[241].name = "wall";
    game->bricks[242].x = 2040;
    game->bricks[242].y = 240;
    game->bricks[242].w = 30;
    game->bricks[242].h = 30;
    game->bricks[242].name = "wall";
    game->bricks[243].x = 2070;
    game->bricks[243].y = 210;
    game->bricks[243].w = 30;
    game->bricks[243].h = 30;
    game->bricks[243].name = "wall";
    game->bricks[244].x = 2190;
    game->bricks[244].y = 210;
    game->bricks[244].w = 30;
    game->bricks[244].h = 30;
    game->bricks[244].name = "wall";
    game->bricks[245].x = 2100;
    game->bricks[245].y = 180;
    game->bricks[245].w = 30;
    game->bricks[245].h = 30;
    game->bricks[245].name = "wall";
    game->bricks[246].x = 2160;
    game->bricks[246].y = 180;
    game->bricks[246].w = 30;
    game->bricks[246].h = 30;
    game->bricks[246].name = "wall";
    game->bricks[247].x = 2130;
    game->bricks[247].y = 150;
    game->bricks[247].w = 30;
    game->bricks[247].h = 30;
    game->bricks[247].name = "wall";
    game->bricks[248].x = 2430;
    game->bricks[248].y = 660;
    game->bricks[248].w = 45;
    game->bricks[248].h = 45;
    game->bricks[248].name = "fire";
    game->bricks[249].x = 2475;
    game->bricks[249].y = 660;
    game->bricks[249].w = 45;
    game->bricks[249].h = 45;
    game->bricks[249].name = "fire";
    game->bricks[250].x = 2520;
    game->bricks[250].y = 660;
    game->bricks[250].w = 45;
    game->bricks[250].h = 45;
    game->bricks[250].name = "fire";
    game->bricks[251].x = 2565;
    game->bricks[251].y = 660;
    game->bricks[251].w = 45;
    game->bricks[251].h = 45;
    game->bricks[251].name = "fire";
    game->bricks[252].x = 2610;
    game->bricks[252].y = 660;
    game->bricks[252].w = 45;
    game->bricks[252].h = 45;
    game->bricks[252].name = "fire";
    game->bricks[253].x = 2655;
    game->bricks[253].y = 660;
    game->bricks[253].w = 45;
    game->bricks[253].h = 45;
    game->bricks[253].name = "fire";
    game->bricks[254].x = 2700;
    game->bricks[254].y = 660;
    game->bricks[254].w = 45;
    game->bricks[254].h = 45;
    game->bricks[254].name = "fire";
    game->bricks[255].x = 2745;
    game->bricks[255].y = 660;
    game->bricks[255].w = 45;
    game->bricks[255].h = 45;
    game->bricks[255].name = "fire";
    game->bricks[256].x = 2790;
    game->bricks[256].y = 660;
    game->bricks[256].w = 45;
    game->bricks[256].h = 45;
    game->bricks[256].name = "fire";
    game->bricks[257].x = 2835;
    game->bricks[257].y = 660;
    game->bricks[257].w = 45;
    game->bricks[257].h = 45;
    game->bricks[257].name = "fire";
    game->bricks[258].x = 2880;
    game->bricks[258].y = 660;
    game->bricks[258].w = 45;
    game->bricks[258].h = 45;
    game->bricks[258].name = "fire";
    game->bricks[259].x = 2925;
    game->bricks[259].y = 660;
    game->bricks[259].w = 45;
    game->bricks[259].h = 45;
    game->bricks[259].name = "fire";
    game->bricks[260].x = 2970;
    game->bricks[260].y = 660;
    game->bricks[260].w = 45;
    game->bricks[260].h = 45;
    game->bricks[260].name = "fire";
    game->bricks[261].x = 390;
    game->bricks[261].y = 110;
    game->bricks[261].w = 30;
    game->bricks[261].h = 30;
    game->bricks[261].name = "wall";
    game->bricks[262].x = 420;
    game->bricks[262].y = 120;
    game->bricks[262].w = 30;
    game->bricks[262].h = 30;
    game->bricks[262].name = "wall";
    game->bricks[263].x = 450;
    game->bricks[263].y = 120;
    game->bricks[263].w = 30;
    game->bricks[263].h = 30;
    game->bricks[263].name = "wall";
    game->bricks[264].x = 480;
    game->bricks[264].y = 120;
    game->bricks[264].w = 30;
    game->bricks[264].h = 30;
    game->bricks[264].name = "wall";
    game->bricks[265].x = 150;
    game->bricks[265].y = 660;
    game->bricks[265].w = 30;
    game->bricks[265].h = 30;
    game->bricks[265].name = "wall";
    game->bricks[266].x = 510;
    game->bricks[266].y = 690;
    game->bricks[266].w = 30;
    game->bricks[266].h = 30;
    game->bricks[266].name = "wall";
    game->bricks[267].x = 300;
    game->bricks[267].y = 630;
    game->bricks[267].w = 30;
    game->bricks[267].h = 30;
    game->bricks[267].name = "wall";
}

void addBullet(float x, float y, float dx, float dy)
{
    int i, found = -1;
    for (i=0; i<1000; i++)
    {
        if(bullets[i] == NULL)
        {
            found = i;
            break;
        }
    }

    if (found >= 0)
    {
        bullets[found] = malloc(sizeof(Bullet));
        bullets[found]->x = x;
        bullets[found]->y = y;
        bullets[found]->dx = dx;
        bullets[found]->dy = dy;
    }

}

void removeBullet(int i)
{
    if(bullets [i])
    {
        free(bullets[i]);
        bullets[i] = NULL;
    }
}

void process(GameState *game)
{
    game->time++;

    //wzrost lvl
    if(game->man.exp >= game->man.maxexp && game->man.lvl < 5)
    {
        lvlUp(game);
    }

    //zmiana mapy
    int n=1, i=0;
    if(game->lvlUpTime <= 0)
    {
        for(i=0; i<10; i++)
        {
            n *= game->enemies[i].isDead;
        }
        if(n)
        {
            game->newMapTime = 150;
            game->mapa++;
            loadMap(game);
        }
    }

    //uplyw czasu wswietlania powiadomienia
    if(game->lvlUpTime)
    {
        game->lvlUpTime --;
    }
    if(game->newMapTime)
    {
        game->newMapTime --;
    }

    // zmiana obrazka ognia
    if(game->time % 10 == 0)
    {
        if (game->fireFrame < 2)
        {
            game->fireFrame++;
        }else game->fireFrame = 0;
    }

    //procesy ludzika
    Man *man = &game->man;
    man->x += man->dx;
    man->y += man->dy;

    if (man->y > 700)
    {
        man->hp = 0;
    }

    if(!man->onLedge && !man->mode)
    {
        if (man->dy > 0)
        {
            man->animFrame = 9;
        }else man->animFrame = 8;
    }else
        if(man->onLedge && man->dx && !man->mode)
        {
            if(game->time % 5 == 0)
            {
                if (man->animFrame > 6)
                {
                    man->animFrame = 0;
                }else man->animFrame++;
            }
        }else
            if(man->mode && man->dx)
            {
                if(game->time % 15 == 0){
                    if (man->animFrame < 13 && man->animFrame > 8)
                    {
                        man->animFrame++;
                    }else man->animFrame = 10;
                }
            }

    if(man->dx > 0)
    {
        man->facingLeft = 0;
    }
    else if(man->dx < 0)
    {
        man->facingLeft = 1;
    }

    if (man->dy != 0)
    {
        man->onLedge = 0;
    }

    game->man.spaceForModeSwitch = 0;

    man->dy += GRAVITY;

    //procesy przeciwnikow
    for (i=0; i<10; i++)
    {
        if(!game->enemies[i].isDead) // jeśli zyje
        {
            game->enemies[i].x += game->enemies[i].dx;
            game->enemies[i].y += game->enemies[i].dy;

            game->enemies[i].dy += GRAVITY;

            if(game->time % 15 == 0){
                if (game->enemies[i].animFrame < 3)
                {
                    game->enemies[i].animFrame++;
                }else game->enemies[i].animFrame = 0;
            }

            if(game->enemies[i].dx > 0)
            {
                game->enemies[i].facingLeft = 0;
            }
            else if(game->enemies[i].dx < 0) // idzie w lewo
            {
                game->enemies[i].facingLeft = 1;
            }

            if(!game->enemies[i].facingLeft)
            {
                game->enemies[i].dx += 0.2;
                if(game->enemies[i].dx > 1.5)
                {
                    game->enemies[i].dx = 1.5;
                }
            }else
            {
                game->enemies[i].dx -= 0.2;
                if(game->enemies[i].dx < -1.5)
                {
                    game->enemies[i].dx = -1.5;
                }
            }

            if(game->enemies[i].hp <= 0)
            {
                game->enemies[i].isDead = 1;

                if(game->enemies[i].name == "walker")
                {
                    game->man.exp += 15;
                }else
                if(game->enemies[i].name == "shooter")
                {
                    game->man.exp += 50;
                }
            }

            if(game->time % 10 == 0)
            {
                if(game->enemies[i].name == "shooter")
                {
                    if(!game->enemies[i].facingLeft)
                    {
                        addBullet(game->enemies[i].x + game->enemies[i].w + 1, game->enemies[i].y + game->enemies[i].h/2 + 1, 10, 0);
                    }else
                    {
                        addBullet(game->enemies[i].x - 11, game->enemies[i].y + game->enemies[i].h/2 + 3, -10, 0);
                    }
                }
            }
        }
    }

    // procesy pociskow
    for (i=0; i<1000; i++)
    {
        if(bullets[i])
        {
            bullets[i]->x += bullets[i]->dx;
            bullets[i]->y += bullets[i]->dy;
            bullets[i]->dy += GRAVITY/10;

            if(game->scrollX + bullets[i]->x < -100 || game->scrollX + bullets[i]->x > 1250 || bullets[i]->y > 800)
            {
                removeBullet(i);
            }
        }
    }

    //Przewijanie
    game->scrollX = -game->man.x+600;
    if(game->scrollX > 0)
        game->scrollX = 0;
}

void collisionDetect(GameState *game)
{
    float mw, mh;
    float mx = game->man.x, my = game->man.y, mdx = game->man.dx, mdy = game->man.dy; // wspolrzedne ludzika
    float bx=0, by=0, bw = 0, bh = 0; // wspolrzedne i rozmiary podloza

    int i=0;
    for (i=0; i<300; i++)
    {
        if(game->bricks[i].name == "ground" || game->bricks[i].name == "wall")
        {
            bx = game->bricks[i].x;
            by = game->bricks[i].y;
            bw = game->bricks[i].w;
            bh = game->bricks[i].h;


            if(!game->man.mode)
            {
                mw = 40, mh = 46; // wymiary ludzika
            }else
            {
                mw = 62, mh = 18;
            }


            if (mx+mw-mdx-1 > bx && mx-mdx+1 < bx+bw)
            {
                // uderzenie glowa
                if (my+mdy < by+bh && my+mdy > by && mdy < 0)
                {
                    // korekta y
                    game->man.y = by+bh;
                    my = by+bh;

                    // wyzerowanie predkosci spadania / skoku
                    game->man.dy = 0;
                }
                // wyladowanie na podlozu
                else if (my+mh+mdy > by && my+mdy < by && mdy > 0)
                {
                    // korekta y
                    game->man.y = by-mh;
                    my = by-mh;

                    // wyzerowanie predkosci spadania / skoku
                    game->man.dy = 0;
                    game->man.onLedge = 1;
                }
            }
            if (my+mh > by && my < by+bh)
            {
                // dotykanie lewej krawedzi sciany
                if (mx < bx+bw && mx+mw > bx+bw && mdx < 0)
                {
                    // korekta x
                    game->man.x = bx+bw;
                    mx = bx+bw;

                    game->man.dx = 0;
                }
                // dotykanie prawej krawedzi sciany
                else if (mx+mw > bx && mx < bx && mdx > 0)
                {
                    // korekta x
                    game->man.x = bx-mw;
                    mx = bx-mw;

                    game->man.dx = 0;
                }
            }

            if(!game->man.facingLeft)
            {
                if(game->man.mode == 1)
                {
                    if (mx+62 > bx && mx < bx+bw) // ma miejsce by wstac
                    {
                        if (my+18-46 < by+bh && my+18-46 > by)
                        {
                            game->man.spaceForModeSwitch = 1;
                        }
                    }
                }
                if(game->man.mode == 0)
                {
                    if (my-mdy+46 > by && my-mdy+46-18 < by+bh) // ma miejsce by sie polozyc
                    {
                        if (mx+mdx+62 > bx && mx+mdx+62 < bx+bw)
                        {
                            game->man.spaceForModeSwitch = 2;
                        }
                    }
                }
            }
            else
            {
                if(game->man.mode == 1)
                {
                    if (mx+40-mdx > bx && mx-mdx < bx+bw) // ma miejsce by wstac
                    {
                        if (my+18-46 < by+bh && my+18-46 > by)
                        {
                            game->man.spaceForModeSwitch = 1;
                        }
                    }
                }
                if(game->man.mode == 0)
                {
                    if (my+46 > by && my+46-18 < by+bh) // ma miejsce by sie polozyc
                    {
                        if (mx+62 > bx && mx < bx)
                        {
                            game->man.spaceForModeSwitch = 2;
                        }
                    }
                }
            }
        }else
        if(game->bricks[i].name == "fire")
        {
            bx = game->bricks[i].x;
            by = game->bricks[i].y;
            bw = game->bricks[i].w;
            bh = game->bricks[i].h;


            if(!game->man.mode)
            {
                mw = 40, mh = 46; // wymiary ludzika
            }else
            {
                mw = 62, mh = 18;
            }

            if (mx+mw-mdx > bx && mx-mdx < bx+bw)
            {
                // uderzenie glowa
                if (my+mdy < by+bh && my+mdy > by && mdy < 0)
                {
                    // zabranie 1/5 zycia
                    game->man.hp -= game->man.maxhp/5;
                }
                // wyladowanie na podlozu
                else if (my+mh+mdy > by && my+mdy < by && mdy > 0)
                {
                    // zabranie 1/5 zycia
                    game->man.hp -= game->man.maxhp/5;
                }
            }
            if (my+mh > by && my < by+bh)
            {
                // dotykanie lewej krawedzi sciany
                if (mx < bx+bw && mx+mw > bx+bw && mdx < 0)
                {
                    // zabranie 1/5 zycia
                    game->man.hp -= game->man.maxhp/5;
                }
                // dotykanie prawej krawedzi sciany
                else if (mx+mw > bx && mx < bx && mdx > 0)
                {
                    // zabranie 1/5 zycia
                    game->man.hp -= game->man.maxhp/5;
                }
            }
        }
    }
}

void collisionEnemies(GameState *game)
{
    float mw = 0, mh = 0;
    float mx = game->man.x, my = game->man.y, mdx = game->man.dx, mdy = game->man.dy;
    float ew = 0, eh = 0; // rozmiary wroga
    float ex = 0, ey = 0, edx = 0, edy = 0; // wspolrzedne wroga
    float bx=0, by=0, bw = 0, bh = 0; // wspolrzedne i rozmiary podloza

    int i,j;
    // dla kazdego przeciwnika
    for (j=0; j<10; j++)
    {
        if(!game->enemies[j].isDead) // jeśli zyje
        {
            // dla kazdego klocka
            // kolizja z otoczeniem
            for (i=0; i<300; i++)
            {
                if(game->bricks[i].name == "ground" || game->bricks[i].name == "wall")
                {
                    bx = game->bricks[i].x;
                    by = game->bricks[i].y;
                    bw = game->bricks[i].w;
                    bh = game->bricks[i].h;
                    ew = game->enemies[j].w;
                    eh = game->enemies[j].h;
                    ex = game->enemies[j].x;
                    ey = game->enemies[j].y;
                    edx = game->enemies[j].dx;
                    edy = game->enemies[j].dy;

                    if (ex+ew-edx > bx && ex-edx < bx+bw)
                    {
                        // wyladowanie na podlozu
                        if (ey+eh+edy > by+1 && ey+edy < by+1 && edy > 0)
                        {
                            // wyzerowanie predkosci spadania / skoku
                            game->enemies[j].dy = 0;
                        }
                    }
                    if (ey+eh > by+1 && ey < by+bh+1)
                    {
                        // dotykanie lewej krawedzi sciany
                        if (ex < bx+bw && ex+ew > bx+bw && edx < 0)
                        {
                            game->enemies[j].dx = 0.1;
                            edx = 0.1;
                        }
                        // dotykanie prawej krawedzi sciany
                        else if (ex+ew > bx && ex < bx && edx > 0)
                        {
                            game->enemies[j].dx = -0.1;
                            edx = -0.1;
                        }
                    }
                }
            }


            //kolizja z ludzikiem
            if(!game->man.mode)
            {
                mw = 40, mh = 46; // wymiary ludzika
            }else
            {
                mw = 62, mh = 18;
            }

            if (mx+mw-mdx > ex && mx-mdx < ex+ew)
            {
                // wyladowanie na wrogu
                if (my+mh+mdy > ey && my+mdy < ey && mdy > 0)
                {
                    // korekta y
                    game->man.y = ey-mh;
                    my = ey-mh;

                    // wyzerowanie predkosci spadania / skoku
                    game->man.dy = 0;
                    game->man.hp-=2;
                    game->man.onLedge = 1;
                }
            }

            if (my+mh > ey && my < ey+eh)
            {
                // dotykanie lewej krawedzi wroga
                if (mx < ex+ew && mx+mw > ex+ew)
                {
                    // korekta x
                    game->enemies[j].x = mx-ew;
                    ex = ex+ew;

                    game->man.dx = 0;
                    game->man.hp-=2;
                    game->enemies[j].dx = 0;
                }
                // dotykanie prawej krawedzi wroga
                else if (mx+mw > ex && mx < ex)
                {
                    // korekta x
                    game->enemies[j].x = mx+mw;
                    ex = mx-ew;

                    game->man.dx = 0;
                    game->man.hp-=2;
                    game->enemies[j].dx = 0;
                }
            }
        }
    }
}

void collisionBullets(GameState *game)
{
    float mw = 0, mh = 0, mx = game->man.x, my = game->man.y;
    float pw = 10, ph = 10, px = 0, py = 0; // rozmiary, wspolrzedne i predkosci pocisku
    float ew = 0, eh = 0, ex = 0, ey = 0; // rozmiary, wspolrzedne i predkosci wroga
    float bw = 0, bh = 0, bx = 0, by = 0; // rozmiary i wspolrzedne blokow

    int i=0, j=0;

    for (i=0; i<1000; i++)
    {
        if(bullets[i])
        {
            px = bullets[i]->x;
            py = bullets[i]->y;

            for (j=0; j<300; j++)
            {
                if(game->bricks[j].name == "ground" || game->bricks[j].name == "wall")
                {
                    bx = game->bricks[j].x;
                    by = game->bricks[j].y;
                    bw = game->bricks[j].w;
                    bh = game->bricks[j].h;
                    if (px+pw/2 > bx &&  px+pw/2 < bx+bw)
                    {
                        if(py+ph/2 > by && py+ph/2 < by+bh)
                        {
                            removeBullet(i);
                        }
                    }
                }
            }
            for (j=0; j<10; j++)
            {
                if(!game->enemies[j].isDead) // jeśli zyje
                {
                    ex = game->enemies[j].x;
                    ey = game->enemies[j].y;
                    ew = game->enemies[j].w;
                    eh = game->enemies[j].h;

                    if (px+pw/2 > ex &&  px+pw/2 < ex+ew)
                    {
                        if(py+ph/2 > ey && py+ph/2 < ey+eh)
                        {
                            removeBullet(i);
                            game->enemies[j].hp -= game->man.damage;
                        }
                    }
                }
            }

            //kolizja z ludzikiem
            if(!game->man.mode)
            {
                mw = 40, mh = 46; // wymiary ludzika
            }else
            {
                mw = 62, mh = 18;
            }
            if (px+pw/2 > mx &&  px+pw/2 < mx+mw)
            {
                if(py+ph/2 > my && py+ph/2 < my+mh)
                {
                    removeBullet(i);
                    game->man.hp -= 20;
                }
            }
        }
    }
}

int processEvents(SDL_Window *window, GameState *game)
{
    SDL_Event event;
    int done=0;

    // sprawdzanie zdarzen
    while(SDL_PollEvent(&event))
    {
        switch(event.type)
        {
            case SDL_WINDOWEVENT_CLOSE:
            {
                if(window)
                {
                    SDL_DestroyWindow(window);
                    window = NULL;
                    done=1;
                }
            }
                break;
            case SDL_KEYDOWN:
            {
                switch(event.key.keysym.sym)
                {
                    case SDLK_ESCAPE:
                        done=1;
                        break;
                    case SDLK_UP:
                        if(!game->man.mode)
                        {
                            if(game->man.onLedge)
                            {
                                game->man.dy = -10.5;
                                game->man.onLedge = 0;
                            }
                        }
                        else
                        {
                            if(game->man.spaceForModeSwitch != 1)
                            {
                                game->man.mode = 0;
                                game->man.animFrame = 2;
                            }
                        }
                        break;
                    case SDLK_DOWN:
                        if(!game->man.mode)
                        {
                            if(game->man.spaceForModeSwitch != 2)
                            {
                                game->man.mode=1;
                                game->man.animFrame = 12;
                            }
                        }
                        break;
                }
            }
                break;
            case SDL_QUIT:
                done=1;
                break;
        }
    }

    const Uint8 *state = SDL_GetKeyboardState(NULL);
    if (state[SDL_SCANCODE_LEFT])
    {
        if(!game->man.mode)
        {
            game->man.dx -= 0.5;
            if(game->man.dx < -6)
            {
                game->man.dx = -6;
            }
        }else
        {
            game->man.dx -= 0.4;
            if(game->man.dx < -1)
            {
                game->man.dx = -1;
            }
        }
    }
    else if (state[SDL_SCANCODE_RIGHT])
    {
        if(!game->man.mode)
        {
            game->man.dx += 0.5;
            if(game->man.dx > 6)
            {
                game->man.dx = 6;
            }
        }else
        {
            game->man.dx += 0.4;
            if(game->man.dx > 1)
            {
                game->man.dx = 1;
            }
        }
    }
    else
    {
        game->man.dx *= 0.8f;
        if(abs(game->man.dx) < 0.1f)
        {
            game->man.dx = 0;
        }
    }

    if (state[SDL_SCANCODE_SPACE])
    {
        if(game->time % game->man.rateOfFire == 0)
        {
            Mix_PlayChannel(-1, game->shot_sound, 0);

            if(!game->man.mode)
            {
                if(!game->man.facingLeft)
                {
                    if(game->man.animFrame == 8) // wznosi sie
                    {
                        addBullet(game->man.x + 40, game->man.y, 10, -3);
                    }else
                    if(game->man.animFrame == 9) // wznosi sie
                    {
                        addBullet(game->man.x + 40, game->man.y + 23, 10, +3);
                    }else
                    {
                        addBullet(game->man.x + 40, game->man.y + 10, 10, 0);
                    }
                }
                else
                {
                    if(game->man.animFrame == 8) // wznosi sie
                    {
                        addBullet(game->man.x - 10, game->man.y, -10, -3);
                    }else
                    if(game->man.animFrame == 9) // wznosi sie
                    {
                        addBullet(game->man.x - 10, game->man.y + 23, -10, +3);
                    }else
                    {
                        addBullet(game->man.x - 10, game->man.y + 10, -10, 0);
                    }
                }
            }
            else
            {
                if(!game->man.facingLeft)
                {
                    addBullet(game->man.x + 62, game->man.y + 5, 10, 0);
                }
                else
                {
                    addBullet(game->man.x - 10, game->man.y + 5, -10, 0);
                }
            }
        }
    }

    return done;
}

void doRender(GameState *game)
{
    // ustawienie koloru rendera na niebieski
    SDL_SetRenderDrawColor(game->renderer, 128, 191, 255, 255);

    // czyszczenie rendera na bialo
    SDL_RenderClear(game->renderer);

    // wybor koloru do rysowania
    SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 0);

    if(game->mapa == 1)
    {
        SDL_Rect tutorrect = {game->scrollX + 50, 200, 400, 200};
        SDL_RenderCopy(game->renderer, game->tutorial, NULL, &tutorrect);
    }

    //rysowanie pociskow
    int i=0;
    for (i=0; i<1000; i++)
    {
        if(bullets[i])
        {
            SDL_Rect rect = {game->scrollX + bullets[i]->x, bullets[i]->y, 10, 10};        ///z przesunieciem
            SDL_RenderCopy(game->renderer, bulletTexture, NULL, &rect);
        }
    }

    //rysowanie sciany
    for (i=0; i<300; i++)   // ziemia
    {
        if(game->bricks[i].name == "ground")
        {
            SDL_Rect brickRect = {game->scrollX + game->bricks[i].x, game->bricks[i].y, game->bricks[i].w, game->bricks[i].h};    ///z przesunieciem
            SDL_RenderCopy(game->renderer, game->ziemia, NULL, &brickRect);
        }else
        if(game->bricks[i].name == "wall")
        {
            SDL_Rect brickRect = {game->scrollX + game->bricks[i].x, game->bricks[i].y, game->bricks[i].w, game->bricks[i].h};    ///z przesunieciem
            SDL_RenderCopy(game->renderer, game->wall, NULL, &brickRect);
        }
    }

    //wyswietlanie przeciwnikow
    for(i=0; i<10; i++)
    {
        if(!game->enemies[i].isDead)
        {
            if(game->enemies[i].name == "walker")
            {
                if(!game->enemies[i].isDead) // jeśli zyje
                {
                    SDL_Rect rect = {game->scrollX + game->enemies[i].x, game->enemies[i].y, game->enemies[i].w, game->enemies[i].h};        ///z przesunieciem
                    SDL_RenderCopyEx(game->renderer, game->robotFrames[game->enemies[i].animFrame], NULL, &rect, 0, NULL, (!(game->enemies[i].facingLeft == 0)));
                }
            }else
            if(game->enemies[i].name == "shooter")
            {
                if(!game->enemies[i].isDead) // jeśli zyje
                {
                    SDL_Rect rect = {game->scrollX + game->enemies[i].x, game->enemies[i].y, game->enemies[i].w, game->enemies[i].h};        ///z przesunieciem
                    SDL_RenderCopyEx(game->renderer, game->shooterFrames[game->enemies[i].animFrame], NULL, &rect, 0, NULL, (!(game->enemies[i].facingLeft == 0)));
                }
            }
        }
    }

    //wyswietlanie ludzika
    if(!game->man.mode)
    {
        SDL_Rect rect = {game->scrollX + game->man.x, game->man.y, 40, 46};        ///z przesunieciem
        SDL_RenderCopyEx(game->renderer, game->manFrames[game->man.animFrame], NULL, &rect, 0, NULL, (!(game->man.facingLeft == 0)));
    }else
    {
        SDL_Rect rect = {game->scrollX + game->man.x, game->man.y, 62, 18};        ///z przesunieciem
        SDL_RenderCopyEx(game->renderer, game->manFrames[game->man.animFrame], NULL, &rect, 0, NULL, (!(game->man.facingLeft == 0)));
    }

    for (i=0; i<300; i++)
    {
        if(game->bricks[i].name == "fire")
        {
            SDL_Rect rect = {game->scrollX + game->bricks[i].x, game->bricks[i].y, game->bricks[i].w, game->bricks[i].h};        ///z przesunieciem
            SDL_RenderCopyEx(game->renderer, game->fireFrames[game->fireFrame], NULL, &rect, 0, NULL, 0);
        }
    }

    // rysowanie interfejsu
    // wybor koloru do czarnego obramowania
    SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 0);

    // pasek zycia gracza
    SDL_Rect brect = {10-1, 650-1, 100+2, 10+2};
    SDL_RenderFillRect(game->renderer, &brect);

    // pasek expa gracza
    SDL_Rect expbrect = {10-1, 670-1, 100+2, 10+2};
    SDL_RenderFillRect(game->renderer, &expbrect);

    // paski zycia wrogow
    for (i=0; i<10; i++)
    {
        if(!game->enemies[i].isDead) // jeśli zyje
        {
            SDL_Rect brect = {game->scrollX + game->enemies[i].x - 1, game->enemies[i].y - 26 , game->enemies[i].w + 2, 10+2};
            SDL_RenderFillRect(game->renderer, &brect);
        }
    }

    // wybor koloru do rysowania paska zycia
    SDL_SetRenderDrawColor(game->renderer, 255, 0, 0, 255);

    // pasek zycia gracza
    SDL_Rect rrect = {10, 650, game->man.hp*100/game->man.maxhp, 10};
    SDL_RenderFillRect(game->renderer, &rrect);

    // paski zycia wrogow
    for (i=0; i<10; i++)
    {
        if(!game->enemies[i].isDead) // jeśli zyje
        {
            SDL_Rect rrect = {game->scrollX + game->enemies[i].x, game->enemies[i].y - 25 , (game->enemies[i].hp*game->enemies[i].w)/game->enemies[i].maxhp, 10};
            SDL_RenderFillRect(game->renderer, &rrect);
        }
    }

    // wybor koloru do rysowania paska expa
    SDL_SetRenderDrawColor(game->renderer, 255, 255, 0, 255);//zolty

    if (game->man.exp <= game->man.maxexp)
    {
        SDL_Rect yrect = {10, 670, game->man.exp*100/game->man.maxexp, 10};
        SDL_RenderFillRect(game->renderer, &yrect);
    }else
    {
        SDL_Rect yrect = {10, 670, 100, 10};
        SDL_RenderFillRect(game->renderer, &yrect);
    }

    //wyswietlanie powiedomiania o wzroscie lvl
    if(game->lvlUpTime)
    {
        SDL_Rect lvlrect = {400, 0, 400, 150};
        SDL_RenderCopy(game->renderer, game->lvlup, NULL, &lvlrect);
    }
    //wyswietlanie powiedomiania o zmianie mapy
    if(game->newMapTime)
    {
        SDL_Rect lvlrect = {400, 0, 400, 150};
        SDL_RenderCopy(game->renderer, game->newmap, NULL, &lvlrect);
    }

    // wyswietlanie stworzonego rysunku
    SDL_RenderPresent(game->renderer);
}

void quitGame(GameState *game)
{
    int i=0;
    //wylaczanie gry i oproznianie pamieci
    SDL_DestroyTexture(game->lvlup);
    SDL_DestroyTexture(game->newmap);
    SDL_DestroyTexture(game->ziemia);
    SDL_DestroyTexture(game->wall);

    for (i=0; i<3; i++)
    {
        SDL_DestroyTexture(game->fireFrames[i]);
    }

    for (i=0; i<14; i++)
    {
        SDL_DestroyTexture(game->manFrames[i]);
    }

    for (i=0; i<1000; i++)
    {
        removeBullet(i);
    }

    SDL_DestroyTexture(bulletTexture);

    Mix_FreeChunk(game->shot_sound);

    // zamknij i usun okno
    SDL_DestroyWindow(game->window);
    SDL_DestroyRenderer(game->renderer);
}

void lvlUp(GameState *game)
{
    game->lvlUpTime = 150;

    game->man.lvl ++;
    game->man.maxhp += 50;
    game->man.hp = game->man.maxhp;
    game->man.exp -= game->man.maxexp;
    game->man.maxexp *= 2;
    game->man.damage += 5;
    game->man.rateOfFire -= 2;
}

int death(GameState *game)
{
    int done = 0;

    while (!done)
    {
//load
        SDL_Surface *surface = NULL;
        SDL_Texture *texture = NULL;

        surface = IMG_Load("Pictures/death.bmp");
        if(surface == NULL)
        {
            printf("Cannot find death.bmp!\n\n");
            SDL_Quit();
            exit(1);
        }

        texture = SDL_CreateTextureFromSurface(game->renderer, surface);
        SDL_FreeSurface(surface);
//eventy
        SDL_Event event;

        // sprawdzanie zdarzen
        while(SDL_PollEvent(&event))
        {
            switch(event.type)
            {
                case SDL_KEYDOWN:
                {
                    switch(event.key.keysym.sym)
                    {
                        case SDLK_ESCAPE:
                            done=1;
                            return done;
                            break;

                        case SDLK_RETURN:
                            done=1;
                            return done;
                            break;
                    }
                }
                    break;
                case SDL_QUIT:
                    done=1;
                    return done;
                    break;
            }
            SDL_Delay(10);
        }
//render
        // ustawienie koloru rendera na bialy
        SDL_SetRenderDrawColor(game->renderer, 255, 255, 255, 255);

        // czyszczenie rendera na bialo
        SDL_RenderClear(game->renderer);

        SDL_Rect deathRect = {300, 150, 600, 350};
        SDL_RenderCopy(game->renderer, texture, NULL, &deathRect);

        // wyswietlanie stworzonego rysunku
        SDL_RenderPresent(game->renderer);

        SDL_DestroyTexture(texture);
    }
}

int save(GameState *game)
{
    FILE *save = fopen("save.txt", "w");

    fprintf(save, "%d\n", game->mapa);
    fprintf(save, "%.2f\n", game->man.x);
    fprintf(save, "%.2f\n", game->man.y);
    fprintf(save, "%.2f\n", game->man.dx);
    fprintf(save, "%.2f\n", game->man.dy);
    fprintf(save, "%d\n", game->man.hp);
    fprintf(save, "%d\n", game->man.maxhp);
    fprintf(save, "%d\n", game->man.exp);
    fprintf(save, "%d\n", game->man.maxexp);
    fprintf(save, "%d\n", game->man.lvl);
    fprintf(save, "%d\n", game->man.onLedge);
    fprintf(save, "%d\n", game->man.mode);
    fprintf(save, "%d\n", game->man.spaceForModeSwitch);
    fprintf(save, "%d\n", game->man.animFrame);
    fprintf(save, "%d\n", game->man.facingLeft);
    fprintf(save, "%d\n", game->man.damage);
    fprintf(save, "%d\n", game->man.rateOfFire);

    int i;
    for (i=0; i<10; i++)
    {
        fprintf(save, "%d\n",game->enemies[i].w);
        fprintf(save, "%d\n",game->enemies[i].h);
        fprintf(save, "%.2f\n",game->enemies[i].x);
        fprintf(save, "%.2f\n",game->enemies[i].y - 1);
        fprintf(save, "%.2f\n",game->enemies[i].dx);
        fprintf(save, "%.2f\n",game->enemies[i].dy);
        fprintf(save, "%d\n",game->enemies[i].hp);
        fprintf(save, "%d\n",game->enemies[i].maxhp);
        fprintf(save, "%d\n",game->enemies[i].isDead);
        fprintf(save, "%d\n",game->enemies[i].animFrame);
        fprintf(save, "%d\n",game->enemies[i].facingLeft);
    }

    fclose(save);

    int done = 0;
    while (!done)
    {
//load
        SDL_Surface *surface = NULL;
        SDL_Texture *texture = NULL;

        surface = IMG_Load("Pictures/saved.bmp");
        if(surface == NULL)
        {
            printf("Cannot find saved.bmp!\n\n");
            SDL_Quit();
            exit(1);
        }

        texture = SDL_CreateTextureFromSurface(game->renderer, surface);
        SDL_FreeSurface(surface);
//eventy
        SDL_Event event;

        // sprawdzanie zdarzen
        while(SDL_PollEvent(&event))
        {
            switch(event.type)
            {
                case SDL_KEYDOWN:
                {
                    switch(event.key.keysym.sym)
                    {
                        case SDLK_ESCAPE:
                            done=1;
                            return done;
                            break;

                        case SDLK_RETURN:
                            done=1;
                            return done;
                            break;
                    }
                }
                    break;
                case SDL_QUIT:
                    done=1;
                    return done;
                    break;
            }

            SDL_Delay(10);
        }
//render
        // ustawienie koloru rendera na bialy
        SDL_SetRenderDrawColor(game->renderer, 255, 255, 255, 255);

        // czyszczenie rendera na bialo
        SDL_RenderClear(game->renderer);

        SDL_Rect deathRect = {300, 150, 600, 350};
        SDL_RenderCopy(game->renderer, texture, NULL, &deathRect);

        // wyswietlanie stworzonego rysunku
        SDL_RenderPresent(game->renderer);

        SDL_DestroyTexture(texture);
    }
}

int load (GameState *game)
{
    FILE *save;
    char tekst[20];

    if ((save = fopen("save.txt", "r"))==NULL)
    {
        printf ("Błąd otwarcia pliku\n");
        exit(1);
    }

    fscanf(save, "%s", tekst);
    game->mapa = atoi(tekst);

    fclose(save);

    loadMap(game);

    if ((save = fopen("save.txt", "r"))==NULL)
    {
        printf ("Błąd otwarcia pliku\n");
        exit(1);
    }

    fscanf(save, "%s", tekst);
    game->mapa = atoi(tekst);

    fscanf(save, "%s", tekst);
    game->man.x = atof(tekst);

    fscanf(save, "%s", tekst);
    game->man.y = atof(tekst);

    fscanf(save, "%s", tekst);
    game->man.dx = atof(tekst);

    fscanf(save, "%s", tekst);
    game->man.dy = atof(tekst);

    fscanf(save, "%s", tekst);
    game->man.hp = atoi(tekst);

    fscanf(save, "%s", tekst);
    game->man.maxhp = atoi(tekst);

    fscanf(save, "%s", tekst);
    game->man.exp = atoi(tekst);

    fscanf(save, "%s", tekst);
    game->man.maxexp = atoi(tekst);

    fscanf(save, "%s", tekst);
    game->man.lvl = atoi(tekst);

    fscanf(save, "%s", tekst);
    game->man.onLedge = atoi(tekst);

    fscanf(save, "%s", tekst);
    game->man.mode = atoi(tekst);

    fscanf(save, "%s", tekst);
    game->man.spaceForModeSwitch = atoi(tekst);

    fscanf(save, "%s", tekst);
    game->man.animFrame = atoi(tekst);

    fscanf(save, "%s", tekst);
    game->man.facingLeft = atoi(tekst);

    fscanf(save, "%s", tekst);
    game->man.damage = atoi(tekst);

    fscanf(save, "%s", tekst);
    game->man.rateOfFire = atoi(tekst);

    int i=0;

    for (i=0; i<10; i++) //fscanf(save, "%s", tekst) != EOF)
    {
        fscanf(save, "%s", tekst);
        game->enemies[i].w = atoi(tekst);

        fscanf(save, "%s", tekst);
        game->enemies[i].h = atoi(tekst);

        fscanf(save, "%s", tekst);
        game->enemies[i].x = atof(tekst);

        fscanf(save, "%s", tekst);
        game->enemies[i].y = atof(tekst);

        fscanf(save, "%s", tekst);
        game->enemies[i].dx = atof(tekst);

        fscanf(save, "%s", tekst);
        game->enemies[i].dy = atof(tekst);

        fscanf(save, "%s", tekst);
        game->enemies[i].hp = atoi(tekst);

        fscanf(save, "%s", tekst);
        game->enemies[i].maxhp = atoi(tekst);

        fscanf(save, "%s", tekst);
        game->enemies[i].isDead = atoi(tekst);

        fscanf(save, "%s", tekst);
        game->enemies[i].animFrame = atoi(tekst);

        fscanf(save, "%s", tekst);
        game->enemies[i].facingLeft = atoi(tekst);
    }

    fclose (save);

    int done = 0;
    while (!done)
    {
//load
        SDL_Surface *surface = NULL;
        SDL_Texture *texture = NULL;

        surface = IMG_Load("Pictures/loaded.bmp");
        if(surface == NULL)
        {
            printf("Cannot find loaded.bmp!\n\n");
            SDL_Quit();
            exit(1);
        }

        texture = SDL_CreateTextureFromSurface(game->renderer, surface);
        SDL_FreeSurface(surface);
//eventy
        SDL_Event event;

        // sprawdzanie zdarzen
        while(SDL_PollEvent(&event))
        {
            switch(event.type)
            {
                case SDL_KEYDOWN:
                {
                    switch(event.key.keysym.sym)
                    {
                        case SDLK_ESCAPE:
                            done=1;
                            return done;
                            break;

                        case SDLK_RETURN:
                            done=1;
                            return done;
                            break;
                    }
                }
                    break;
                case SDL_QUIT:
                    done=1;
                    return done;
                    break;
            }

            SDL_Delay(10);
        }
//render
        // ustawienie koloru rendera na bialy
        SDL_SetRenderDrawColor(game->renderer, 255, 255, 255, 255);

        // czyszczenie rendera na bialo
        SDL_RenderClear(game->renderer);

        SDL_Rect deathRect = {300, 150, 600, 350};
        SDL_RenderCopy(game->renderer, texture, NULL, &deathRect);

        // wyswietlanie stworzonego rysunku
        SDL_RenderPresent(game->renderer);

        SDL_DestroyTexture(texture);
    }
}
