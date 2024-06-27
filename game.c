#include "game.h"


int main(int argc, char** argv){

    test_canMove();
    test_bulletTouchEnnemy();
    test_createDucky();
    test_createBullet();
    test_EnemyMove();

    printf("\n \nAll tests PASSED! \n \n");

    //init
    srand(time(NULL));
    SDL_Window *window= NULL;
    SDL_Renderer *renderer = NULL;
    SDL_Texture *texture = NULL;
    int nombre_de_texture = 25;
    SDL_Texture *textures[nombre_de_texture];
    SDL_Texture *backgroundTexture = NULL;
    SDL_Texture *startImage = NULL; 
    int etape = 0; //niveau de jeu
    SDL_Surface *Surface= NULL;
    SDL_Surface *SurfaceEnemy= NULL;
    SDL_Surface *Surfaceprof=NULL;
    SDL_Texture *SpritePlayerTexture = NULL;
    SDL_Texture *SpriteEnemyTexture = NULL;
    SDL_Texture *SpritePlayerAttackingTexture = NULL;
    SDL_Texture *SpritePlayerBulletTexture = NULL;
    SDL_Texture *SpriteDuckyTexture = NULL;
    SDL_Texture *SpriteEnemyAttackingTexture=NULL;
    SDL_Texture *SpritePlayerBullettomTexture=NULL;

    if(SDL_Init(SDL_INIT_EVERYTHING) != 0){ 
        SDL_ExitWithError("Init sdl2 echouee");
    }

    window = SDL_CreateWindow("TNDefense", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    if (window == NULL){ 
        SDL_ExitWithError("Creation fenetre echouee");
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    if(renderer == NULL){ 
        SDL_DestroyWindow(window);
        SDL_ExitWithError("Creation renderer echouee");
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
        SDL_Quit();
        return 1;
    }

    Mix_Music* bgMusic = Mix_LoadMUS("./music/music.mp3");
    if (bgMusic == NULL) {
        printf("Failed to load music! SDL_mixer Error: %s\n", Mix_GetError());
        Mix_Quit();
        SDL_Quit();
        return 1;
    }
    Mix_Music* menuMusic = Mix_LoadMUS("./music/tnmenu.mp3");
    if (bgMusic == NULL) {
        printf("Failed to load music! SDL_mixer Error: %s\n", Mix_GetError());
        Mix_Quit();
        SDL_Quit();
        return 1;
    }
    Mix_Music* serveur = Mix_LoadMUS("./music/serveur.mp3");
    if (bgMusic == NULL) {
        printf("Failed to load music! SDL_mixer Error: %s\n", Mix_GetError());
        Mix_Quit();
        SDL_Quit();
        return 1;
    }

    //start
    SDL_Surface* imageSurface = SDL_LoadBMP("./static/picture.bmp");
    if (imageSurface == NULL) {
        SDL_ExitWithError("Failed to load background image");
    }
    backgroundTexture = SDL_CreateTextureFromSurface(renderer, imageSurface);
    SDL_FreeSurface(imageSurface);
    if (backgroundTexture == NULL) {
        SDL_ExitWithError("Failed to create background texture");
    }
    SDL_Surface* startSurface = SDL_LoadBMP("./static/TNDefense.bmp");
    if (startSurface == NULL) {
        SDL_ExitWithError("Failed to load start image");
    }
    
    startImage = SDL_CreateTextureFromSurface(renderer, startSurface);
    SDL_FreeSurface(startSurface);
    if (startImage == NULL) {
        SDL_ExitWithError("Failed to create start image texture");
    }
    if (TTF_Init() == -1) {
        SDL_ExitWithError("TTF_Init failed");
    }

    TTF_Font* font = TTF_OpenFont("./fonts/LoveDays-2v7Oe.ttf", 180);
    TTF_Font* fontstart = TTF_OpenFont("./fonts/NatureBeautyPersonalUse-9Y2DK.ttf", 100);
    TTF_Font* fontscore = TTF_OpenFont("./fonts/LoveDays-2v7Oe.ttf", 20);
    TTF_Font* fonthp = TTF_OpenFont("./fonts/LoveDays-2v7Oe.ttf", 15);



    if (!font) {
        SDL_ExitWithError("Failed to load font");
    }
    if (!fontstart) {
        SDL_ExitWithError("Failed to load font");
    }
    
    // texts
    SDL_Color textColor = {255, 255, 255, 255};
    SDL_Surface* slogantext = TTF_RenderText_Solid(font, "Let's help TN", textColor);
    if (!slogantext) {
        SDL_ExitWithError("Failed to create text surface");
    }

    SDL_Rect sloganRect;
    sloganRect.x = (WINDOW_WIDTH - slogantext->w) / 2;
    sloganRect.y = 450;
    sloganRect.w = slogantext->w;
    sloganRect.h = slogantext->h;

    SDL_Texture* slogan = SDL_CreateTextureFromSurface(renderer, slogantext);
    SDL_FreeSurface(slogantext); 
    if (!slogan) {
        SDL_ExitWithError("Failed to create text texture");
    }

    SDL_Color textColorstart = {0, 0, 0, 0};
    SDL_Surface* startsurface = TTF_RenderText_Solid(fontstart, "start", textColorstart);
    if (!startsurface) {
        SDL_ExitWithError("Failed to create text surface");
    }

    SDL_Rect startRect;
    startRect.x = (WINDOW_WIDTH - startsurface->w) / 2;
    startRect.y = 350;
    startRect.w = startsurface->w;
    startRect.h = startsurface->h;

    SDL_Texture* starttexture = SDL_CreateTextureFromSurface(renderer, startsurface);
    SDL_FreeSurface(startsurface); 
    if (!starttexture) {
        SDL_ExitWithError("Failed to create text texture");
    }
    int player_score=0;
    SDL_Color textColorscore = {0, 0, 0, 0};
    char scoreText[20];
    sprintf(scoreText, "Score: %d", player_score);

    SDL_Surface* scoresurface = TTF_RenderText_Solid(fontscore, scoreText, textColorscore);
    if (!scoresurface) {
        SDL_ExitWithError("Failed to create text surface");
    }

    SDL_Rect scoreRect;
    scoreRect.x = 10;
    scoreRect.y = 10;
    scoreRect.w = scoresurface->w;
    scoreRect.h = scoresurface->h;

    SDL_Texture* scoretexture = SDL_CreateTextureFromSurface(renderer, scoresurface);
    SDL_FreeSurface(scoresurface); 
    if (!scoretexture) {
        SDL_ExitWithError("Failed to create text texture");
    }


    
    SDL_Rect startButtonRect;
    startButtonRect.x = startRect.x - 20; 
    startButtonRect.y = startRect.y - 10;
    startButtonRect.w = startRect.w + 40;
    startButtonRect.h = startRect.h + 20;
    

    //variables pour fps

    int current_image_time;
    int last_image_time = 0;
    int start=0;

    //maps
int map[35][50] = {
    {6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 6, 6},
    {6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 1, 11, 12, 0, 0, 0, 0, 0, 0, 0, 0, 11, 12, 1, 6, 6},
    {6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 1, 13, 14, 0, 0, 0, 0, 0, 0, 0, 0, 13, 14, 1, 6, 6},
    {6, 6, 6, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 6, 6},
    {6, 6, 6, 1, 0, 0, 0, 0, 1, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 1, 4, 4, 4, 4, 4, 4, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 6, 6},
    {6, 6, 6, 1, 0, 0, 0, 0, 1, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 1, 4, 4, 4, 4, 4, 4, 1, 0, 0, 0, 0, 15, 16, 0, 0, 0, 0, 0, 0, 1, 6, 6},
    {6, 6, 6, 1, 0, 0, 0, 0, 0, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 1, 4, 4, 4, 4, 4, 4, 1, 0, 0, 0, 0, 17, 18, 0, 0, 0, 0, 0, 0, 1, 6, 6},
    {6, 6, 6, 1, 0, 0, 0, 0, 0, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 1, 4, 4, 4, 4, 4, 4, 1, 0, 0, 0, 0, 19, 20, 0, 0, 0, 0, 0, 0, 1, 6, 6},
    {6, 6, 6, 1, 0, 0, 0, 0, 0, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 1, 4, 4, 4, 4, 4, 4, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 6, 6},
    {6, 6, 6, 1, 0, 0, 0, 0, 0, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 6, 6},
    {6, 6, 6, 1, 0, 0, 0, 0, 1, 8, 8, 8, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 8, 8, 8, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 6, 6},
    {6, 6, 6, 1, 0, 0, 0, 0, 1, 8, 8, 8, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 8, 8, 8, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 15, 16, 0, 0, 0, 2, 6, 6},
    {6, 6, 6, 1, 0, 0, 0, 0, 1, 1, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 17, 18, 0, 0, 0, 2, 6, 6},
    {6, 6, 6, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 19, 20, 0, 0, 0, 2, 6, 6},
    {6, 6, 6, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 6, 6},
    {6, 6, 6, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 6, 6},
    {6, 6, 6, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 6, 6},
    {6, 6, 6, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 6, 6},
    {6, 6, 6, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 6, 6},
    {6, 6, 6, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 6, 6},
    {6, 6, 6, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 6, 6},
    {6, 6, 6, 1, 0, 0, 0, 0, 5, 5, 5, 7, 0, 0, 0, 5, 5, 5, 7, 0, 0, 0, 5, 5, 5, 7, 0, 0, 0, 0, 0, 0, 0, 0, 1, 8, 8, 8, 8, 8, 8, 1, 0, 0, 0, 0, 0, 1, 6, 6},
    {6, 6, 6, 1, 0, 0, 0, 0, 5, 5, 5, 0, 0, 0, 0, 5, 5, 5, 0, 0, 0, 0, 5, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 8, 8, 8, 8, 8, 8, 1, 0, 0, 0, 0, 0, 1, 6, 6},
    {6, 6, 6, 1, 0, 0, 0, 0, 5, 5, 5, 0, 0, 0, 0, 5, 5, 5, 0, 0, 0, 0, 5, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 8, 8, 8, 8, 8, 8, 3, 0, 0, 0, 0, 0, 1, 6, 6},
    {6, 6, 6, 1, 0, 0, 0, 0, 5, 5, 5, 0, 0, 0, 0, 5, 5, 5, 0, 0, 0, 0, 5, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 8, 8, 8, 8, 8, 8, 3, 0, 0, 0, 0, 0, 1, 6, 6},
    {6, 6, 6, 1, 0, 0, 0, 0, 5, 5, 5, 0, 0, 0, 0, 5, 5, 5, 0, 0, 0, 0, 5, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 8, 8, 8, 8, 8, 8, 3, 0, 0, 0, 0, 0, 1, 6, 6},
    {6, 6, 6, 1, 0, 0, 0, 0, 5, 5, 5, 0, 0, 0, 0, 5, 5, 5, 0, 0, 0, 0, 5, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 8, 8, 8, 8, 8, 8, 3, 0, 0, 0, 0, 0, 1, 6, 6},
    {6, 6, 6, 1, 0, 0, 0, 0, 5, 5, 5, 0, 0, 0, 0, 5, 5, 5, 0, 0, 0, 0, 5, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 8, 8, 8, 8, 8, 8, 3, 0, 0, 0, 0, 0, 1, 6, 6},
    {6, 6, 6, 1, 0, 0, 0, 0, 7, 0, 0, 7, 0, 0, 0, 7, 0, 0, 7, 0, 0, 0, 7, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 3, 8, 8, 8, 8, 8, 8, 3, 0, 0, 0, 0, 0, 1, 6, 6},
    {6, 6, 6, 1, 11, 12, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 11, 12, 1, 8, 8, 8, 8, 8, 8, 1, 11, 12, 0, 0, 0, 1, 6, 6},
    {6, 6, 6, 1, 13, 14, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 13, 14, 1, 8, 8, 8, 8, 8, 8, 1, 13, 14, 0, 0, 0, 1, 6, 6},
    {6, 6, 6, 1, 1, 1, 1, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 6, 6},
    {6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 21,21,21,21,21,21, 6, 6, 6, 6, 6, 6, 6, 6, 6},
    {6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 21, 21, 21, 21, 21, 21, 6, 6, 6, 6, 6, 6, 6, 6, 6},
    {6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 21, 21, 21, 21, 21, 21, 6, 6, 6, 6, 6, 6, 6, 6, 6},
    };
   
int map1[35][50] = {
    {6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6},
    {6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6},
    {6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6},
    {6, 6, 6, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 6, 6, 6},
    {6, 6, 6, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 6, 6, 6},
    {6, 6, 6, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 6, 6, 6},
    {6, 6, 6, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 6, 6, 6},
    {6, 6, 6, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 6, 6, 6},
    {6, 6, 6, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 6, 6, 6},
    {6, 6, 6, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 6, 6, 6},
    {6, 6, 6, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 6, 6, 6},
    {6, 6, 6, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 6, 6, 6},
    {6, 6, 6, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 22, 22, 22, 22, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 6, 6, 6},
    {6, 6, 6, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 23, 23, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 23, 23, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 6, 6, 6},
    {6, 6, 6, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 23, 23, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 23, 23, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 6, 6, 6},
    {6, 6, 6, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 23, 23, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 23, 23, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 6, 6, 6},
    {6, 6, 6, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 21, 21, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 23, 23, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 6, 6, 6},
    {6, 6, 6, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 21, 21, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 23, 23, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 6, 6, 6},
    {6, 6, 6, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 21, 21, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 23, 23, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 6, 6, 6},
    {6, 6, 6, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 21, 21, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 23, 23, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 6, 6, 6},
    {6, 6, 6, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 23, 23, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 23, 23, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 6, 6, 6},
    {6, 6, 6, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 23, 23, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 23, 23, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 6, 6, 6},
    {6, 6, 6, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 23, 23, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 21, 21, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 6, 6, 6},
    {6, 6, 6, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 23, 23, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 21, 21, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 6, 6, 6},
    {6, 6, 6, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 23, 23, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 21, 21, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 6, 6, 6},
    {6, 6, 6, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 23, 23, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 21, 21, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 6, 6, 6},
    {6, 6, 6, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 23, 23, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 23, 23, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 6, 6, 6},
    {6, 6, 6, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 23, 23, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 23, 23, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 6, 6, 6},
    {6, 6, 6, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 23, 23, 0, 0, 0, 1, 4, 4, 4, 4, 4, 4, 1, 0, 0, 0, 23, 23, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 6, 6, 6},
    {6, 6, 6, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 23, 23, 0, 0, 0, 1, 4, 4, 4, 4, 4, 4, 1, 0, 0, 0, 23, 23, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 6, 6, 6},
    {6, 6, 6, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 23, 23, 0, 0, 0, 1, 4, 4, 4, 4, 4, 4, 1, 0, 0, 0, 23, 23, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 6, 6, 6},
    {6, 6, 6, 1, 1, 1, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 1, 1, 2, 2, 2, 2, 2, 2, 1, 1, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 1, 1, 1, 6, 6, 6},
    {6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6},
    {6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6},
    {6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6},
    };



  
  
    //textures players

    Surface = SDL_LoadBMP("./static/bouthier.bmp");
    if(Surface == NULL){ 
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_ExitWithError("surface player chargement echouee");
    }
    SpritePlayerTexture = SDL_CreateTextureFromSurface(renderer,Surface);
    if(SpritePlayerTexture == NULL){ 
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_ExitWithError("texture player chargement echouee");
    }
    SurfaceEnemy=SDL_LoadBMP("./static/zombie-vf-16-deplacements.bmp");
    if(SurfaceEnemy == NULL){ 
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_ExitWithError("surface player chargement echouee");
    }
 
    SpriteEnemyTexture=SDL_CreateTextureFromSurface(renderer,SurfaceEnemy);
    if(SpriteEnemyTexture == NULL){ 
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_ExitWithError("texture player chargement echouee");
    } 

    Surface = SDL_LoadBMP("./static/bouthier_atck.bmp");
    if(Surface == NULL){ 
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_ExitWithError("surface player attacking chargement echouee");
    }
    SpritePlayerAttackingTexture = SDL_CreateTextureFromSurface(renderer,Surface);
    if(SpritePlayerAttackingTexture == NULL){ 
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_ExitWithError("texture player attacking chargement echouee");
    }

    SurfaceEnemy=SDL_LoadBMP("./static/zombie_atk.bmp");
    if(SurfaceEnemy == NULL){ 
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_ExitWithError("surface player chargement echouee");
    }

    SpriteEnemyAttackingTexture=SDL_CreateTextureFromSurface(renderer,SurfaceEnemy);
    if(SpriteEnemyAttackingTexture == NULL){ 
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_ExitWithError("texture player attacking chargement echouee");
    }
    
    
    Surface = SDL_LoadBMP("./static/bouthier_bullet.bmp");
    if(Surface == NULL){ 
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_ExitWithError("surface player bullet chargement echouee");
    }
    Surfaceprof = SDL_LoadBMP("./static/bullet_tomzak.bmp");
    if(Surfaceprof == NULL){ 
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_ExitWithError("surface player tom bullet chargement echouee");
    }

    SpritePlayerBulletTexture = SDL_CreateTextureFromSurface(renderer,Surface);
    SpritePlayerBullettomTexture = SDL_CreateTextureFromSurface(renderer,Surfaceprof);
    if(SpritePlayerBulletTexture == NULL){ 
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_ExitWithError("texture player bullet chargement echouee");
    }
    if(SpritePlayerBullettomTexture == NULL){ 
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_ExitWithError("texture player bullet chargement echouee");
    }
    
    Surface = SDL_LoadBMP("./static/duck_sprite.bmp");
    if(Surface == NULL){ 
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_ExitWithError("surface ducky chargement echouee");
    }
    SpriteDuckyTexture = SDL_CreateTextureFromSurface(renderer,Surface);
    if(SpriteDuckyTexture == NULL){ 
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_ExitWithError("texture ducky chargement echouee");
    }
    
    //textures map
textures[1] = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("./images/murtest.bmp"));
if (textures[1] == NULL) {
    SDL_ExitWithError("Failed to create texture from murtest.bmp");
}
textures[0] = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("./images/sol6.bmp"));
if (textures[0] == NULL) {
    SDL_ExitWithError("Failed to create texture from sol6.bmp");
}
textures[2] = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("./images/text_porte.bmp"));
if (textures[2] == NULL) {
    SDL_ExitWithError("Failed to create texture from text_porte.bmp");
}
textures[3] = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("./images/vitre4.bmp"));
if (textures[3] == NULL) {
    SDL_ExitWithError("Failed to create texture from vitre4.bmp");
}
textures[4] = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("./images/escalier.bmp"));
if (textures[4] == NULL) {
    SDL_ExitWithError("Failed to create texture from escalier.bmp");
}
textures[5] = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("./images/table.bmp"));
if (textures[5] == NULL) {
    SDL_ExitWithError("Failed to create texture from table.bmp");
}
textures[6] = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("./images/soldehors.bmp"));
if (textures[6] == NULL) {
    SDL_ExitWithError("Failed to create texture from soldehors.bmp");
}
textures[7] = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("./images/pieds.bmp"));
if (textures[7] == NULL) {
    SDL_ExitWithError("Failed to create texture from pieds.bmp");
}
textures[8] = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("./images/text_sol_int.bmp"));
if (textures[8] == NULL) {
    SDL_ExitWithError("Failed to create texture from text_sol_int.bmp");
}
textures[9] = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("./images/serveurs.bmp"));
if (textures[9] == NULL) {
    SDL_ExitWithError("Failed to create texture from serveurs.bmp");
}

textures[11] = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("./images/00.bmp"));
if (textures[11] == NULL) {
    SDL_ExitWithError("Failed to create texture from 00.bmp");
}
textures[12] = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("./images/10.bmp"));
if (textures[12] == NULL) {
    SDL_ExitWithError("Failed to create texture from 10.bmp");
}
textures[13] = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("./images/01.bmp"));
if (textures[13] == NULL) {
    SDL_ExitWithError("Failed to create texture from 01.bmp");
}
textures[14] = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("./images/11.bmp"));
if (textures[14] == NULL) {
    SDL_ExitWithError("Failed to create texture from 11.bmp");
}
textures[15] = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("./images/baby/00.bmp"));
if (textures[15] == NULL) {
    SDL_ExitWithError("Failed to create texture from baby/00.bmp");
}
textures[16] = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("./images/baby/10.bmp"));
if (textures[16] == NULL) {
    SDL_ExitWithError("Failed to create texture from baby/10.bmp");
}
textures[17] = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("./images/baby/01.bmp"));
if (textures[17] == NULL) {
    SDL_ExitWithError("Failed to create texture from baby/01.bmp");
}
textures[18] = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("./images/baby/11.bmp"));
if (textures[18] == NULL) {
    SDL_ExitWithError("Failed to create texture from baby/11.bmp");
}
textures[19] = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("./images/baby/02.bmp"));
if (textures[19] == NULL) {
    SDL_ExitWithError("Failed to create texture from baby/02.bmp");
}
textures[20] = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("./images/baby/12.bmp"));
if (textures[20] == NULL) {
    SDL_ExitWithError("Failed to create texture from baby/12.bmp");
}
textures[21] = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("./images/chemin.bmp"));
if (textures[21] == NULL) {
    SDL_ExitWithError("Failed to create texture from chemin.bmp");
}
textures[22] = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("./images/ob.bmp"));
if (textures[22] == NULL) {
    SDL_ExitWithError("Failed to create texture from ob.bmp");
}
textures[23] = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("./images/lave.bmp"));
if (textures[23] == NULL) {
    SDL_ExitWithError("Failed to create texture from lave.bmp");
}



    SDL_FreeSurface(Surface);

    //variables pour dessiner le player
    int player_x=140;
    int player_y=300;
    int Enemy_x= 500;
    int Enemy_y= 500;
    int player_hp= 300;
    int player_hp2=300;

    int last_mvt=1;
    int mvt=0;
    int offset_x=0;
    int offset_y=0;

    SDL_Rect SrcSpritePlayerRect = {.x = 0, .y = 0, .w = PLAYER_SPRITE_WIDTH, .h = PLAYER_SPRITE_HEIGHT};
    SDL_Rect DestSpritePlayerRect = {.x = player_x, .y = player_y, .w = PLAYER_SPRITE_WIDTH, .h = PLAYER_SPRITE_HEIGHT};

    int professor=1;
    SDL_Texture* SpritePlayerTexture2= SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("./static/prof2_deplacement.png.bmp"));
    SDL_Texture* SpritePlayerAttackingTexture2= SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("./static/prof2_atk.png.bmp"));
    SDL_Texture* game_over= SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("./static/game_over.bmp"));
    SDL_Texture* cmd= SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("./static/cmd.bmp"));
    


  

    //variables attack player 
    int player_attacking=0;
    int i;
    struct Bullet bullets[50];
    for (i=0;i<50;i++){ 
        bullets[i].fired=0;
        bullets[i].damage=10;
        //change bullet damage
    }
    SDL_Rect SrcSpritePlayerAttackingRect = {.x = 0, .y = 0, .w = PLAYER_SPRITE_WIDTH, .h = PLAYER_SPRITE_HEIGHT};

    //variable Enemies

    struct Enemy enemies[10];
    for (i=0;i<10;i++){ 
        enemies[i].spawned=0;
        enemies[i].currentDirection = getRandomMove();
    
    }
    
    int lastUpdateTime = 0;
    SDL_Rect final1 = {.x = 0, .y = 0, .w = 1000, .h = 350};
    SDL_Rect final2 = {.x = 0, .y = 350, .w = 1000, .h = 350};



    for (i=0;i<10;i++){ 
        enemies[i].attacking==0;
        enemies[i].rect2.x = 0;
        enemies[i].rect2.y = 0;
        enemies[i].rect2.w = PLAYER_SPRITE_WIDTH;
        enemies[i].rect2.h = PLAYER_SPRITE_HEIGHT;
    
    }
  
    //variables duckies
    
    struct Ducky duckies[10];
    for (i=0;i<10;i++){ 
        duckies[i].spawned=0;
        duckies[i].value=10;
    }
    initRandom();
    int timerstart;
    int timer=0;

    
 
    //boucle du game
    SDL_bool program_launched = SDL_TRUE;
    while (program_launched){

        SDL_Event event;

        //Gestion des events
        while(SDL_PollEvent(&event)){  
            if(event.type == SDL_QUIT){ 
                program_launched = SDL_FALSE;
                break; 
            } 
            else if(event.type == SDL_KEYDOWN){
                if(etape == 1 || etape==2){
                if(event.key.keysym.sym==SDLK_DOWN){ 
                    last_mvt=1;
                    mvt=1;
                    offset_y=0; //permet aussi de savoir type de mvt
                    offset_x++;
                    offset_x %=4;  
                }
                else if(event.key.keysym.sym==SDLK_LEFT){
                    last_mvt=2;
                    mvt=2;
                    offset_y=1;
                    offset_x++;
                    offset_x%=4;  
                }
                else if(event.key.keysym.sym==SDLK_RIGHT){ 
                    last_mvt=3;
                    mvt=3;
                    offset_y=2;
                    offset_x++;
                    offset_x%=4;  
                }
                else if(event.key.keysym.sym==SDLK_UP){
                    last_mvt=4;
                    mvt=4;
                    offset_y=3;
                    offset_x++;
                    offset_x%=4;  
                }
                else if(event.key.keysym.sym==SDLK_s){
                   professor=1; 
                }
                else if(event.key.keysym.sym==SDLK_d){
                   professor=2; 
                }
                else if(event.key.keysym.sym==SDLK_a){ 
                    createBullet(last_mvt,bullets,player_x,player_y);
                    player_attacking=1;

                }
                    else if(event.key.keysym.sym == SDLK_ESCAPE){
                        etape = 0;
                        for(i=0;i<10;i++){ 
                            enemies[i].spawned=0;
                            bullets[i].fired=0;
                            duckies[i].spawned=0;
                        }
                        player_x=140;
                        player_y=300;
                        player_score=0;
                        player_hp=300;
                        player_hp2=300;
                        offset_x=0;
                        offset_y=0;
                    }
                }
        
                else if(etape == 0){
                    if(event.key.keysym.sym == SDLK_RETURN){
                        timerstart = SDL_GetTicks() / 1000;
                        etape = 1;
                        
                    }
                }
                else if(etape == 3){
                    if(event.key.keysym.sym == SDLK_RETURN){
                        timerstart = SDL_GetTicks() / 1000;
                        etape = 1;
                    }
                    else if(event.key.keysym.sym == SDLK_ESCAPE){
           
                        etape = 0;
                        for(i=0;i<10;i++){ 
                            enemies[i].spawned=0;
                            bullets[i].fired=0;
                            duckies[i].spawned=0;
                        }
                        player_x=140;
                        player_y=300;
                        player_score=0;
                        player_hp=300;
                        player_hp2=300;
                        offset_x=0;
                        offset_y=0;

                    }
                }
                else if(etape == 5){
                    if(event.key.keysym.sym == SDLK_ESCAPE){
                        etape = 0;
                    }
                }
            }
            else if(event.type == SDL_MOUSEBUTTONDOWN){//mouse to click the start button
                if(event.button.button == SDL_BUTTON_LEFT){
                    int x = event.button.x;
                    int y = event.button.y;
                    if(x >= startButtonRect.x && x <= (startButtonRect.x + startButtonRect.w) &&
                       y >= startButtonRect.y && y <= (startButtonRect.y + startButtonRect.h)){
                        timerstart = SDL_GetTicks() / 1000;
                        etape = 3;
                    }
                }
            }
        }


        static int previous_etape = -1;  // check if music is playing
        if (etape == 1 && previous_etape != 1) {
            if (Mix_PlayMusic(bgMusic, -1) == -1) {
                printf("Failed to play music! SDL_mixer Error: %s\n", Mix_GetError());
            }
        } else if (etape == 0 && previous_etape != 0) {
            if (Mix_PlayMusic(menuMusic, -1) == -1) {
                printf("Failed to play menu music! SDL_mixer Error: %s\n", Mix_GetError());
            }
        }
        else if (etape == 2 && previous_etape == 1) {
            if (Mix_PlayMusic(serveur, -1) == -1) {
                printf("Failed to play menu music! SDL_mixer Error: %s\n", Mix_GetError());
            }
        }
        previous_etape = etape;

        if (start==0){
            start=1;
            Uint32 startTime = SDL_GetTicks();
            while (SDL_GetTicks() - startTime < 2500) {
            SDL_RenderClear(renderer);
            SDL_RenderCopy(renderer, startImage, NULL, NULL);
            SDL_RenderPresent(renderer);

    }
        }

        if(etape == 0){
            SDL_RenderClear(renderer);
            SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);
           

            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); 
            SDL_RenderFillRect(renderer, &startButtonRect);
             SDL_RenderCopy(renderer, starttexture, NULL, &startRect);
         

            SDL_RenderCopy(renderer, slogan, NULL, &sloganRect);
            SDL_RenderPresent(renderer);
        }

    if (etape == 1) {

    // Clear renderer
    if (SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE) != 0) {
        SDL_ExitWithError("Failed to set background drawing color to white");
    }
    SDL_RenderClear(renderer);

    // Generate an enemy
    int p;
    for (i = 1; i < 10; i++) {
        
        if (enemies[i].spawned == 0 && SDL_GetTicks() - lastUpdateTime >= 5000) {
            p=rand()%3;
            if (p%3==0){
                enemies[i].rect.y = 580;
                enemies[i].rect.x = 700;
                enemies[i].rect.w = 60;
                enemies[i].rect.h = 60;
                enemies[i].spawned = 1;
                enemies[i].hp = 60;
                lastUpdateTime = SDL_GetTicks();
                
                break;
                
            }
            else if(p%3==1){
                enemies[i].rect.y = 300;
                enemies[i].rect.x = 850;
                enemies[i].rect.w = 60;
                enemies[i].rect.h = 60;
                enemies[i].spawned = 1;
                enemies[i].hp = 60;
                lastUpdateTime = SDL_GetTicks();
                
                break;

                
            }
            else if(p%3==2){
                enemies[i].rect.y = 300;
                enemies[i].rect.x = 50;
                enemies[i].rect.w = 60;
                enemies[i].rect.h = 60;
                enemies[i].spawned = 1;
                enemies[i].hp = 60;
                lastUpdateTime = SDL_GetTicks();
                
                break;

            }
            p++;





 
            
        }
    }

    // Drawing map
    drawMap(renderer, textures, map);

    // Draw score
    SDL_Color textColorscore = {0, 0, 0, 0};
    char scoreText[20];
    sprintf(scoreText, "Score: %d", player_score);

    SDL_Surface* scoresurface = TTF_RenderText_Solid(fontscore, scoreText, textColorscore);
    if (!scoresurface) {
        SDL_ExitWithError("Failed to create score text surface");
    }

    SDL_Rect scoreRect = {10, 10, scoresurface->w, scoresurface->h};
    SDL_Texture *scoretexture = SDL_CreateTextureFromSurface(renderer, scoresurface);
    SDL_FreeSurface(scoresurface);
    if (!scoretexture) {
        SDL_ExitWithError("Failed to create score text texture");
    }
    SDL_RenderCopy(renderer, scoretexture, NULL, &scoreRect);
    SDL_DestroyTexture(scoretexture);

    // Draw timer
    SDL_Color textColortimer = {0, 0, 0, 0};
    char timerText[20];
    int timer = SDL_GetTicks() / 1000 - timerstart; // Get the time in seconds
    sprintf(timerText, "Timer: %ds", timer);

    SDL_Surface *timersurface = TTF_RenderText_Solid(fontscore, timerText, textColortimer);
    if (!timersurface) {
        SDL_ExitWithError("Failed to create timer text surface");
    }

    SDL_Rect timerRect = {10, 30, timersurface->w, timersurface->h};
    SDL_Texture *timertexture = SDL_CreateTextureFromSurface(renderer, timersurface);
    SDL_FreeSurface(timersurface);
    if (!timertexture) {
        SDL_ExitWithError("Failed to create timer text texture");
    }
    SDL_RenderCopy(renderer, timertexture, NULL, &timerRect);
    SDL_DestroyTexture(timertexture);
    char hp_text_player[10];
    if(professor==1){
        snprintf(hp_text_player, sizeof(hp_text_player), "HP: %d", player_hp);
    }
    else if(professor==2){
        snprintf(hp_text_player, sizeof(hp_text_player), "HP: %d", player_hp2);
    }

    SDL_Color colorhp = {255, 255, 255}; 
    SDL_Surface *surfacehpplayer = TTF_RenderText_Solid(fontscore, hp_text_player, colorhp);
    SDL_Texture *texturehpplayer = SDL_CreateTextureFromSurface(renderer, surfacehpplayer);

    SDL_Rect dstrectplayer = {player_x, player_y-30, 80, 20};
    SDL_RenderCopy(renderer, texturehpplayer, NULL, &dstrectplayer);

    SDL_FreeSurface(surfacehpplayer);
    SDL_DestroyTexture(texturehpplayer);
    // Drawing player and handling collisions
        if(player_attacking==1){ 
                SrcSpritePlayerAttackingRect.x=(last_mvt-1)*PLAYER_SPRITE_WIDTH;
                if(professor==1){ 
                    if(SDL_RenderCopy(renderer,SpritePlayerAttackingTexture,&SrcSpritePlayerAttackingRect,&DestSpritePlayerRect)!=0){ 
                        SDL_DestroyRenderer(renderer);
                        SDL_DestroyWindow(window);
                        SDL_ExitWithError("dessin player attacking echouee");
                    }
                }
                if(professor==2){
                    if(SDL_RenderCopy(renderer,SpritePlayerAttackingTexture2,&SrcSpritePlayerAttackingRect,&DestSpritePlayerRect)!=0){ 
                        SDL_DestroyRenderer(renderer);
                        SDL_DestroyWindow(window);
                        SDL_ExitWithError("dessin player attacking echouee");
                    }
                }
                player_attacking=0;
        }
        else if (player_attacking==0) { 
            if (mvt==0){ // pour s arreter tjrs dans le premier sprite d un mvt
                offset_x=0;
            }
            else if (mvt==1 && canMove(player_x,player_y,map,mvt)){

                player_y+=MOUVEMENT_PLAYER;
            }
            else if (mvt==2 && canMove(player_x,player_y,map,mvt)){

                player_x-=MOUVEMENT_PLAYER;
            }
            else if (mvt==3 && canMove(player_x,player_y,map,mvt)){

                player_x+=MOUVEMENT_PLAYER;
            }
            else if (mvt==4 && canMove(player_x,player_y,map,mvt)){

                player_y-=MOUVEMENT_PLAYER;
            }

            DestSpritePlayerRect.x=player_x;
            DestSpritePlayerRect.y=player_y;
            SrcSpritePlayerRect.x=offset_x*PLAYER_SPRITE_WIDTH+4*offset_y*PLAYER_SPRITE_WIDTH;
            SrcSpritePlayerRect.y=0;
            mvt=0;
            if(professor==1){ 
                if(SDL_RenderCopy(renderer,SpritePlayerTexture,&SrcSpritePlayerRect,&DestSpritePlayerRect)!=0){ 
                    SDL_DestroyRenderer(renderer);
                    SDL_DestroyWindow(window);
                    SDL_ExitWithError("dessin player echouee");
                }
            }
            if(professor==2){ 
                if(SDL_RenderCopy(renderer,SpritePlayerTexture2,&SrcSpritePlayerRect,&DestSpritePlayerRect)!=0){ 
                    SDL_DestroyRenderer(renderer);
                    SDL_DestroyWindow(window);
                    SDL_ExitWithError("dessin player echouee");
                }
            }
        }



        //dessin ducky avant ennemy && collision avec player
        for(i=0;i<10;i++){ 
            if(duckies[i].spawned==1){ 
                duckies[i].rect2.x=20*duckies[i].offset;
                if(SDL_RenderCopy(renderer,SpriteDuckyTexture,&duckies[i].rect2,&duckies[i].rect)!=0){ 
                    SDL_DestroyRenderer(renderer);
                    SDL_DestroyWindow(window);
                    SDL_ExitWithError("dessin ducky echouee");
                }
                duckies[i].offset+=1;
                duckies[i].offset%=7;
                if(playerTouchDucky(duckies[i],DestSpritePlayerRect)){ 
                    duckies[i].spawned=0;
                    player_score+=duckies[i].value;
                }
                current_image_time = SDL_GetTicks();
                if(current_image_time-duckies[i].time_spawned>=10000){ 
                    duckies[i].spawned=0;
                }
            }
        }
	    //Drawing Bullets && collisions bullets

        if(SDL_SetRenderDrawColor(renderer,0,162,232,SDL_ALPHA_OPAQUE)!=0){ 
            SDL_ExitWithError("Choisir couleur dessin bullet echouee");
        } 

        for(i=0;i<50;i++){ 

            if (bullets[i].fired==1 && bullets[i].direction==2){ 
                bullets[i].rect2.x=40+20*bullets[i].offset;
                if(professor==1){
                if(SDL_RenderCopy(renderer,SpritePlayerBulletTexture,&bullets[i].rect2,&bullets[i].rect)!=0){ 

                    SDL_DestroyRenderer(renderer);
                    SDL_DestroyWindow(window);
                    SDL_ExitWithError("dessin player bullet echouee");
                }
                }
                else if(professor==2){
                if(SDL_RenderCopy(renderer,SpritePlayerBullettomTexture,&bullets[i].rect2,&bullets[i].rect)!=0){ 

                    SDL_DestroyRenderer(renderer);
                    SDL_DestroyWindow(window);
                    SDL_ExitWithError("dessin player bullet echouee");
                }
                }
                bullets[i].offset+=1;
                bullets[i].offset%=2;
                if ((bulletTouchEnnemy(duckies,bullets[i],enemies))){ 
                    bullets[i].fired=0;
                }
                else if ((bulletCanAdvance(bullets[i],map))){ 
                    bullets[i].rect.x-=20;
                }
                else if (!(bulletCanAdvance(bullets[i],map))){ 
                    bullets[i].fired=0;
                }
            }
            else if (bullets[i].fired==1 && bullets[i].direction==3){ 
                bullets[i].rect2.x=80+20*bullets[i].offset;
                                if(professor==1){
                if(SDL_RenderCopy(renderer,SpritePlayerBulletTexture,&bullets[i].rect2,&bullets[i].rect)!=0){ 

                    SDL_DestroyRenderer(renderer);
                    SDL_DestroyWindow(window);
                    SDL_ExitWithError("dessin player bullet echouee");
                }
                }
                else if(professor==2){
                if(SDL_RenderCopy(renderer,SpritePlayerBullettomTexture,&bullets[i].rect2,&bullets[i].rect)!=0){ 

                    SDL_DestroyRenderer(renderer);
                    SDL_DestroyWindow(window);
                    SDL_ExitWithError("dessin player bullet echouee");
                }
                }
                bullets[i].offset+=1;
                bullets[i].offset%=2;
                if ((bulletTouchEnnemy(duckies,bullets[i],enemies))){ 
                    bullets[i].fired=0;
                }
                else if ((bulletCanAdvance(bullets[i],map))){ 
                    bullets[i].rect.x+=20;
                }
                else if (!(bulletCanAdvance(bullets[i],map))){ 
                    bullets[i].fired=0;
                }
            }
            else if (bullets[i].fired==1 && bullets[i].direction==1){ 
                bullets[i].rect2.x=120+20*bullets[i].offset;
                                if(professor==1){
                if(SDL_RenderCopy(renderer,SpritePlayerBulletTexture,&bullets[i].rect2,&bullets[i].rect)!=0){ 

                    SDL_DestroyRenderer(renderer);
                    SDL_DestroyWindow(window);
                    SDL_ExitWithError("dessin player bullet echouee");
                }
                }
                else if(professor==2){
                if(SDL_RenderCopy(renderer,SpritePlayerBullettomTexture,&bullets[i].rect2,&bullets[i].rect)!=0){ 

                    SDL_DestroyRenderer(renderer);
                    SDL_DestroyWindow(window);
                    SDL_ExitWithError("dessin player bullet echouee");
                }
                }
                bullets[i].offset+=1;
                bullets[i].offset%=2;
                if ((bulletTouchEnnemy(duckies,bullets[i],enemies))){ 
                    bullets[i].fired=0;
                }
                else if ((bulletCanAdvance(bullets[i],map))){ 
                    bullets[i].rect.y+=20;
                }
                else if (!(bulletCanAdvance(bullets[i],map))){ 
                    bullets[i].fired=0;
                }
            }
            else if (bullets[i].fired==1 && bullets[i].direction==4){ 
                bullets[i].rect2.x=0+20*bullets[i].offset;
                                if(professor==1){
                if(SDL_RenderCopy(renderer,SpritePlayerBulletTexture,&bullets[i].rect2,&bullets[i].rect)!=0){ 

                    SDL_DestroyRenderer(renderer);
                    SDL_DestroyWindow(window);
                    SDL_ExitWithError("dessin player bullet echouee");
                }
                }
                else if(professor==2){
                if(SDL_RenderCopy(renderer,SpritePlayerBullettomTexture,&bullets[i].rect2,&bullets[i].rect)!=0){ 

                    SDL_DestroyRenderer(renderer);
                    SDL_DestroyWindow(window);
                    SDL_ExitWithError("dessin player bullet echouee");
                }
                }
                bullets[i].offset+=1;
                bullets[i].offset%=2;
               
                if ((bulletTouchEnnemy(duckies,bullets[i],enemies))){ 
                    bullets[i].fired=0;
                }
                else if ((bulletCanAdvance(bullets[i],map))){ 
                    bullets[i].rect.y-=20;
                }
                else if (!(bulletCanAdvance(bullets[i],map))){ 
                    bullets[i].fired=0;
                }
            }
        }

        //Drawing Ennemy && attack 

 
        if(SDL_SetRenderDrawColor(renderer,225,30,30,SDL_ALPHA_OPAQUE)!=0){ 
            SDL_ExitWithError("Choisir couleur dessin bullet echouee");
        } 

        for(i=0;i<10;i++){ 
            if(enemies[i].spawned==1){ 
                if (enemies[i].attacking!=20) { 
                    char hp_text[10];
                    snprintf(hp_text, sizeof(hp_text), "HP: %d", enemies[i].hp);
                    SDL_Color colorhp = {255, 255, 255}; // Text color in white
                    SDL_Surface* surfacehp = TTF_RenderText_Solid(fonthp, hp_text, colorhp);
                    SDL_Texture* texturehp = SDL_CreateTextureFromSurface(renderer, surfacehp);

                    SDL_Rect dstrect = {enemies[i].rect.x+5, enemies[i].rect.y-20, surfacehp->w, surfacehp->h};
                    SDL_RenderCopy(renderer, texturehp, NULL, &dstrect);

                    SDL_FreeSurface(surfacehp);
                    SDL_DestroyTexture(texturehp);
                            if(SDL_RenderCopy(renderer,SpriteEnemyTexture,&enemies[i].rect2,&enemies[i].rect)!=0){ 
                                SDL_DestroyRenderer(renderer);
                                SDL_DestroyWindow(window);
                                SDL_ExitWithError("Dessin ennemi echouee");
                            }
                        }
                else if(enemies[i].attacking==20){ 
                        enemies[i].rect2.x=(enemies[i].last_mvt-1)*PLAYER_SPRITE_WIDTH;
                        if(SDL_RenderCopy(renderer,SpriteEnemyAttackingTexture,&enemies[i].rect2,&enemies[i].rect)!=0){ 
                            SDL_DestroyRenderer(renderer);
                            SDL_DestroyWindow(window);
                            SDL_ExitWithError("dessin player attacking echouee");
                        }
                        enemies[i].rect2.x=4*(enemies[i].last_mvt-1)*PLAYER_SPRITE_WIDTH;

                        if(professor==1){
                            player_hp-=5;
                        
                        }
                        else if(professor==2){
                            player_hp2-=5;
                        }
                        enemies[i].attacking=0;
                }
// EnemyMove(&enemies[i], map, player_x, player_y);
                if (abs(player_x - enemies[i].rect.x) <= 40 && abs(player_y - enemies[i].rect.y) <= 40) {
                    enemies[i].attacking+=1;
                } else {
                    EnemyMove(&enemies[i], map, player_x, player_y); 
                            enemies[i].rect2.x=enemies[i].offset_x*PLAYER_SPRITE_WIDTH+4*enemies[i].offset_y*PLAYER_SPRITE_WIDTH;

                }
            }
        }



        if(player_hp<=0 || player_hp2<=0){ 
            etape=5;
            for(i=0;i<10;i++){ 
                enemies[i].spawned=0;
                bullets[i].fired=0;
                duckies[i].spawned=0;
            }
            player_x=140;
            player_y=300;
            player_score=0;
            player_hp=300;
            player_hp2=300;
            offset_x=0;
            offset_y=0;

        }
        if(player_score>=100){ 
            etape=2;
            for(i=0;i<10;i++){ 
                enemies[i].spawned=0;
                bullets[i].fired=0;
                duckies[i].spawned=0;
            }
            player_x=500;
            player_y=500;
            player_hp=300;
            player_hp2=300;
            offset_x=0;
            offset_y=0;
        }
        //Dessin des cases pour mieux comprendre

        // drawGrid(renderer);
        }
        //etape2

    else if (etape == 2) {

    // Clear renderer
    if (SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE) != 0) {
        SDL_ExitWithError("Failed to set background drawing color to white");
    }
    SDL_RenderClear(renderer);

    // Generate an enemy




int a;
 
 for (i = 1; i < 10; i++) {
        
        if (enemies[i].spawned == 0 && SDL_GetTicks() - lastUpdateTime >= 5000) {
            a=rand()%3;
            if (a%3==0){
                enemies[i].rect.y = 580;
                enemies[i].rect.x = 500;
                enemies[i].rect.w = 60;
                enemies[i].rect.h = 60;
                enemies[i].spawned = 1;
                enemies[i].hp = 60;
                lastUpdateTime = SDL_GetTicks();
                
                break;
                
            }
            else if(a%3==1){
                enemies[i].rect.y = 200;
                enemies[i].rect.x = 870;
                enemies[i].rect.w = 60;
                enemies[i].rect.h = 60;
                enemies[i].spawned = 1;
                enemies[i].hp = 60;
                lastUpdateTime = SDL_GetTicks();
                
                break;

                
            }
            else if(a%3==2){
                enemies[i].rect.y = 270;
                enemies[i].rect.x = 50;
                enemies[i].rect.w = 60;
                enemies[i].rect.h = 60;
                enemies[i].spawned = 1;
                enemies[i].hp = 60;
                lastUpdateTime = SDL_GetTicks();
                
                break;

            }


a++;


 
            
        }
    }

    // Drawing map
    drawMap(renderer, textures, map1);

    // Draw score
    SDL_Color textColorscore = {0, 0, 0, 0};
    char scoreText[20];
    sprintf(scoreText, "Score: %d", player_score);

    SDL_Surface* scoresurface = TTF_RenderText_Solid(fontscore, scoreText, textColorscore);
    if (!scoresurface) {
        SDL_ExitWithError("Failed to create score text surface");
    }

    SDL_Rect scoreRect = {10, 10, scoresurface->w, scoresurface->h};
    SDL_Texture *scoretexture = SDL_CreateTextureFromSurface(renderer, scoresurface);
    SDL_FreeSurface(scoresurface);
    if (!scoretexture) {
        SDL_ExitWithError("Failed to create score text texture");
    }
    SDL_RenderCopy(renderer, scoretexture, NULL, &scoreRect);
    SDL_DestroyTexture(scoretexture);

    // Draw timer
    SDL_Color textColortimer = {0, 0, 0, 0};
    char timerText[20];
    int timer = SDL_GetTicks() / 1000 - timerstart; // Get the time in seconds
    sprintf(timerText, "Timer: %ds", timer);

    SDL_Surface *timersurface = TTF_RenderText_Solid(fontscore, timerText, textColortimer);
    if (!timersurface) {
        SDL_ExitWithError("Failed to create timer text surface");
    }

    SDL_Rect timerRect = {10, 30, timersurface->w, timersurface->h};
    SDL_Texture *timertexture = SDL_CreateTextureFromSurface(renderer, timersurface);
    SDL_FreeSurface(timersurface);
    if (!timertexture) {
        SDL_ExitWithError("Failed to create timer text texture");
    }
    SDL_RenderCopy(renderer, timertexture, NULL, &timerRect);
    SDL_DestroyTexture(timertexture);
    char hp_text_player[10];
    if(professor==1){

     
        snprintf(hp_text_player, sizeof(hp_text_player), "HP: %d", player_hp);
     }
    else if(professor==2){
        snprintf(hp_text_player, sizeof(hp_text_player), "HP: %d", player_hp2);
    }
    SDL_Color colorhp = {255, 255, 255}; // Text color in white
    SDL_Surface *surfacehpplayer = TTF_RenderText_Solid(fontscore, hp_text_player, colorhp);
    SDL_Texture *texturehpplayer = SDL_CreateTextureFromSurface(renderer, surfacehpplayer);

    SDL_Rect dstrectplayer = {player_x, player_y-30, 80, 20};
    SDL_RenderCopy(renderer, texturehpplayer, NULL, &dstrectplayer);

    SDL_FreeSurface(surfacehpplayer);
    SDL_DestroyTexture(texturehpplayer);
    // Drawing player and handling collisions
        if(player_attacking==1){ 
                SrcSpritePlayerAttackingRect.x=(last_mvt-1)*PLAYER_SPRITE_WIDTH;
                if(professor==1){ 
                    if(SDL_RenderCopy(renderer,SpritePlayerAttackingTexture,&SrcSpritePlayerAttackingRect,&DestSpritePlayerRect)!=0){ 
                        SDL_DestroyRenderer(renderer);
                        SDL_DestroyWindow(window);
                        SDL_ExitWithError("dessin player attacking echouee");
                    }
                }
                if(professor==2){
                    if(SDL_RenderCopy(renderer,SpritePlayerAttackingTexture2,&SrcSpritePlayerAttackingRect,&DestSpritePlayerRect)!=0){ 
                        SDL_DestroyRenderer(renderer);
                        SDL_DestroyWindow(window);
                        SDL_ExitWithError("dessin player attacking echouee");
                    }
                }
                player_attacking=0;
        }
        else if (player_attacking==0) { 
            if (mvt==0){ // pour s arreter tjrs dans le premier sprite d un mvt
                offset_x=0;
            }
            else if (mvt==1 && canMove(player_x,player_y,map1,mvt)){

                player_y+=MOUVEMENT_PLAYER;
            }
            else if (mvt==2 && canMove(player_x,player_y,map1,mvt)){

                player_x-=MOUVEMENT_PLAYER;
            }
            else if (mvt==3 && canMove(player_x,player_y,map1,mvt)){

                player_x+=MOUVEMENT_PLAYER;
            }
            else if (mvt==4 && canMove(player_x,player_y,map1,mvt)){

                player_y-=MOUVEMENT_PLAYER;
            }

            DestSpritePlayerRect.x=player_x;
            DestSpritePlayerRect.y=player_y;
            SrcSpritePlayerRect.x=offset_x*PLAYER_SPRITE_WIDTH+4*offset_y*PLAYER_SPRITE_WIDTH;
            SrcSpritePlayerRect.y=0;
            mvt=0;
            if(professor==1){ 
                if(SDL_RenderCopy(renderer,SpritePlayerTexture,&SrcSpritePlayerRect,&DestSpritePlayerRect)!=0){ 
                    SDL_DestroyRenderer(renderer);
                    SDL_DestroyWindow(window);
                    SDL_ExitWithError("dessin player echouee");
                }
            }
            if(professor==2){ 
                if(SDL_RenderCopy(renderer,SpritePlayerTexture2,&SrcSpritePlayerRect,&DestSpritePlayerRect)!=0){ 
                    SDL_DestroyRenderer(renderer);
                    SDL_DestroyWindow(window);
                    SDL_ExitWithError("dessin player echouee");
                }
            }
        }



        //dessin ducky avant ennemy && collision avec player
        for(i=0;i<10;i++){ 
            if(duckies[i].spawned==1){ 
                duckies[i].rect2.x=20*duckies[i].offset;
                if(SDL_RenderCopy(renderer,SpriteDuckyTexture,&duckies[i].rect2,&duckies[i].rect)!=0){ 
                    SDL_DestroyRenderer(renderer);
                    SDL_DestroyWindow(window);
                    SDL_ExitWithError("dessin ducky echouee");
                }
                duckies[i].offset+=1;
                duckies[i].offset%=7;
                if(playerTouchDucky(duckies[i],DestSpritePlayerRect)){ 
                    duckies[i].spawned=0;
                    player_score+=duckies[i].value;
                }
                current_image_time = SDL_GetTicks();
                if(current_image_time-duckies[i].time_spawned>=10000){ 
                    duckies[i].spawned=0;
                }
            }
        }
	    //Drawing Bullets && collisions bullets

        if(SDL_SetRenderDrawColor(renderer,0,162,232,SDL_ALPHA_OPAQUE)!=0){ 
            SDL_ExitWithError("Choisir couleur dessin bullet echouee");
        } 

        for(i=0;i<50;i++){ 

            if (bullets[i].fired==1 && bullets[i].direction==2){ 
                bullets[i].rect2.x=40+20*bullets[i].offset;
                if(professor==1){
                if(SDL_RenderCopy(renderer,SpritePlayerBulletTexture,&bullets[i].rect2,&bullets[i].rect)!=0){ 

                    SDL_DestroyRenderer(renderer);
                    SDL_DestroyWindow(window);
                    SDL_ExitWithError("dessin player bullet echouee");
                }
                }
                else if(professor==2){
                if(SDL_RenderCopy(renderer,SpritePlayerBullettomTexture,&bullets[i].rect2,&bullets[i].rect)!=0){ 

                    SDL_DestroyRenderer(renderer);
                    SDL_DestroyWindow(window);
                    SDL_ExitWithError("dessin player bullet echouee");
                }
                }
                bullets[i].offset+=1;
                bullets[i].offset%=2;
                if ((bulletTouchEnnemy(duckies,bullets[i],enemies))){ 
                    bullets[i].fired=0;
                }
                else if ((bulletCanAdvance(bullets[i],map1))){ 
                    bullets[i].rect.x-=20;
                }
                else if (!(bulletCanAdvance(bullets[i],map1))){ 
                    bullets[i].fired=0;
                }
            }
            else if (bullets[i].fired==1 && bullets[i].direction==3){ 
                bullets[i].rect2.x=80+20*bullets[i].offset;
                                if(professor==1){
                if(SDL_RenderCopy(renderer,SpritePlayerBulletTexture,&bullets[i].rect2,&bullets[i].rect)!=0){ 

                    SDL_DestroyRenderer(renderer);
                    SDL_DestroyWindow(window);
                    SDL_ExitWithError("dessin player bullet echouee");
                }
                }
                else if(professor==2){
                if(SDL_RenderCopy(renderer,SpritePlayerBullettomTexture,&bullets[i].rect2,&bullets[i].rect)!=0){ 

                    SDL_DestroyRenderer(renderer);
                    SDL_DestroyWindow(window);
                    SDL_ExitWithError("dessin player bullet echouee");
                }
                }
                bullets[i].offset+=1;
                bullets[i].offset%=2;
                if ((bulletTouchEnnemy(duckies,bullets[i],enemies))){ 
                    bullets[i].fired=0;
                }
                else if ((bulletCanAdvance(bullets[i],map1))){ 
                    bullets[i].rect.x+=20;
                }
                else if (!(bulletCanAdvance(bullets[i],map1))){ 
                    bullets[i].fired=0;
                }
            }
            else if (bullets[i].fired==1 && bullets[i].direction==1){ 
                bullets[i].rect2.x=120+20*bullets[i].offset;
                                if(professor==1){
                if(SDL_RenderCopy(renderer,SpritePlayerBulletTexture,&bullets[i].rect2,&bullets[i].rect)!=0){ 

                    SDL_DestroyRenderer(renderer);
                    SDL_DestroyWindow(window);
                    SDL_ExitWithError("dessin player bullet echouee");
                }
                }
                else if(professor==2){
                if(SDL_RenderCopy(renderer,SpritePlayerBullettomTexture,&bullets[i].rect2,&bullets[i].rect)!=0){ 

                    SDL_DestroyRenderer(renderer);
                    SDL_DestroyWindow(window);
                    SDL_ExitWithError("dessin player bullet echouee");
                }
                }
                bullets[i].offset+=1;
                bullets[i].offset%=2;
                if ((bulletTouchEnnemy(duckies,bullets[i],enemies))){ 
                    bullets[i].fired=0;
                }
                else if ((bulletCanAdvance(bullets[i],map1))){ 
                    bullets[i].rect.y+=20;
                }
                else if (!(bulletCanAdvance(bullets[i],map1))){ 
                    bullets[i].fired=0;
                }
            }
            else if (bullets[i].fired==1 && bullets[i].direction==4){ 
                bullets[i].rect2.x=0+20*bullets[i].offset;
                                if(professor==1){
                if(SDL_RenderCopy(renderer,SpritePlayerBulletTexture,&bullets[i].rect2,&bullets[i].rect)!=0){ 

                    SDL_DestroyRenderer(renderer);
                    SDL_DestroyWindow(window);
                    SDL_ExitWithError("dessin player bullet echouee");
                }
                }
                else if(professor==2){
                if(SDL_RenderCopy(renderer,SpritePlayerBullettomTexture,&bullets[i].rect2,&bullets[i].rect)!=0){ 

                    SDL_DestroyRenderer(renderer);
                    SDL_DestroyWindow(window);
                    SDL_ExitWithError("dessin player bullet echouee");
                }
                }
                bullets[i].offset+=1;
                bullets[i].offset%=2;
              
                if ((bulletTouchEnnemy(duckies,bullets[i],enemies))){ 
                    bullets[i].fired=0;
                }
                else if ((bulletCanAdvance(bullets[i],map1))){ 
                    bullets[i].rect.y-=20;
                }
                else if (!(bulletCanAdvance(bullets[i],map1))){ 
                    bullets[i].fired=0;
                }
            }
        }

        //Drawing Ennemy && context steering && attack 
        // { }
 
            if(SDL_SetRenderDrawColor(renderer,225,30,30,SDL_ALPHA_OPAQUE)!=0){ 
            SDL_ExitWithError("Choisir couleur dessin bullet echouee");
        } 

        for(i=0;i<10;i++){ 
            if(enemies[i].spawned==1){ 
                if (enemies[i].attacking!=7) { 
            char hp_text[10];
            snprintf(hp_text, sizeof(hp_text), "HP: %d", enemies[i].hp);
            SDL_Color colorhp = {255, 255, 255}; // Text color in white
            SDL_Surface* surfacehp = TTF_RenderText_Solid(fonthp, hp_text, colorhp);
            SDL_Texture* texturehp = SDL_CreateTextureFromSurface(renderer, surfacehp);

            SDL_Rect dstrect = {enemies[i].rect.x+5, enemies[i].rect.y-20, surfacehp->w, surfacehp->h};
            SDL_RenderCopy(renderer, texturehp, NULL, &dstrect);

            SDL_FreeSurface(surfacehp);
            SDL_DestroyTexture(texturehp);
                    if(SDL_RenderCopy(renderer,SpriteEnemyTexture,&enemies[i].rect2,&enemies[i].rect)!=0){ 
                        SDL_DestroyRenderer(renderer);
                        SDL_DestroyWindow(window);
                        SDL_ExitWithError("Dessin ennemi echouee");
                    }
                }
                else if(enemies[i].attacking==7){ 
                        enemies[i].rect2.x=(enemies[i].last_mvt-1)*PLAYER_SPRITE_WIDTH;
                        if(SDL_RenderCopy(renderer,SpriteEnemyAttackingTexture,&enemies[i].rect2,&enemies[i].rect)!=0){ 
                            SDL_DestroyRenderer(renderer);
                            SDL_DestroyWindow(window);
                            SDL_ExitWithError("dessin player attacking echouee");
                        }
                        enemies[i].rect2.x=4*(enemies[i].last_mvt-1)*PLAYER_SPRITE_WIDTH;
                        if(professor==1){
                            player_hp-=5;
                        
                        }
                        else if(professor==2){
                            player_hp2-=5;
                        
                        }
                        enemies[i].attacking=0;
                }
// EnemyMove(&enemies[i], map, player_x, player_y);
                if (abs(player_x - enemies[i].rect.x) <= 40 && abs(player_y - enemies[i].rect.y) <= 40) {
                    enemies[i].attacking+=1;
                } else {
                    EnemyMove(&enemies[i], map1, player_x, player_y); 
                            enemies[i].rect2.x=enemies[i].offset_x*PLAYER_SPRITE_WIDTH+4*enemies[i].offset_y*PLAYER_SPRITE_WIDTH;

                }
            }
        }









        if(player_hp<=0
        || player_hp2<=0){ 
            etape=5;
            for(i=0;i<10;i++){ 
                enemies[i].spawned=0;
                bullets[i].fired=0;
                duckies[i].spawned=0;
            }
            player_x=140;
            player_y=300;
            player_score=0;
            player_hp=300;
            player_hp2=300;
            offset_x=0;
            offset_y=0;

        }
        //Dessin des cases pour mieux comprendre
        // drawGrid(renderer);
        





/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


 



        //Dessin des cases pour mieux comprendre
        // drawGrid(renderer);
        }
        else if(etape==5){ 
            if(SDL_SetRenderDrawColor(renderer,108,36,168,SDL_ALPHA_OPAQUE)!=0){ 
                SDL_ExitWithError("Choisir couleur dessinage background blanc echouee");
            }
            SDL_RenderFillRect(renderer, &final1);
            if(SDL_SetRenderDrawColor(renderer,239,138,38,SDL_ALPHA_OPAQUE)!=0){ 
                SDL_ExitWithError("Choisir couleur dessinage background blanc echouee");
            }
            SDL_RenderFillRect(renderer, &final2);
            SDL_RenderCopy(renderer, game_over, NULL, NULL);;
        }
        else if(etape==3){ 
            if(SDL_SetRenderDrawColor(renderer,108,36,168,SDL_ALPHA_OPAQUE)!=0){ 
                SDL_ExitWithError("Choisir couleur dessinage background blanc echouee");
            }
            SDL_RenderClear(renderer);
            SDL_RenderCopy(renderer, cmd, NULL, NULL);
        }



        //Controling fps & showing renderer
        current_image_time = SDL_GetTicks();
        controlFps(current_image_time, last_image_time);
        SDL_RenderPresent(renderer);
        last_image_time = SDL_GetTicks();
    }

    //Closing renderer et window et sdl2
    for (int i = 0; i < nombre_de_texture; i++) {
        SDL_DestroyTexture(textures[i]);
    }
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return EXIT_SUCCESS;
}
