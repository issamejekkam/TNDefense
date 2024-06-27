#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>

#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 700

#define CELL_SIZE 20

#define ROWS 35
#define COLS 50

#define SPRITE_SIZE 60
#define SPRITE_SPEED 10

#define FPS_LIMIT 80 //pour fps

#define INF 99999  //pour astar

#define PLAYER_SPRITE_WIDTH 60
#define PLAYER_SPRITE_HEIGHT 60

#define MOUVEMENT_PLAYER 10

#define BULLET_SIZE 20


typedef struct Node {
    int x, y;
    int gCost, hCost, fCost;
    struct Node* parent;
} Node;

struct Bullet {
    SDL_Rect rect;
    SDL_Rect rect2;
    int fired;
    int direction;
    int damage;
    int offset;
};

typedef struct Enemy {
    //variable name ennemy
    //variable pour la texture
    SDL_Rect rect; //rect de destination
    SDL_Rect rect2; //rect de source
    int hp; //hp peut remplacer variable spawned
    int spawned;
    int attacking;
    int last_mvt;
    int offset_x;
    int offset_y;
    int currentDirection;

} Enemy;

struct Ducky { 
    SDL_Rect rect;
    SDL_Rect rect2;
    int spawned;
    int time_spawned;
    int value;
    int offset;
};


void SDL_ExitWithError(const char *message);

void drawMap(SDL_Renderer *renderer,SDL_Texture *textures[], int matrix[ROWS][COLS]);
int canMove(int player_x, int player_y,int map[ROWS][COLS],int mvt);

void controlFps(int current_image_time, int last_image_time);

int** createMatrixFromFile(const char* filename);

void createBullet(int last_mvt, struct Bullet bullets[], int player_x, int player_y);

void createDucky(struct Ducky duckies[], int enemy_x, int enemy_y, int death_time);

int bulletCanAdvance(struct Bullet bullet,int map[ROWS][COLS]); //gere collision avec que les obstacles

int bulletTouchEnnemy(struct Ducky duckies[],struct Bullet bullet,struct Enemy enemies[]);

int playerTouchDucky(struct Ducky ducky,SDL_Rect DestSpritePlayerRect);

bool isBlocked(int value);

bool isBlocked2(int value);
void drawMap(SDL_Renderer* renderer, SDL_Texture** textures, int map[35][50]);
void drawGrid(SDL_Renderer* renderer);




void moveEnemyRandomly(Enemy* enemy, int map[ROWS][COLS]) ;
bool canMoveEnemy(int x, int y, int map[ROWS][COLS], int move);

void initRandom() ;

// Génère un mouvement aléatoire (1: haut, 2: bas, 3: gauche, 4: droite)
int getRandomMove();
void EnemyMove(Enemy* enemy, int map[35][50], int player_x, int player_y);

void test_createDucky();
void test_bulletTouchEnnemy();
void test_canMove();
void test_createBullet();
void test_EnemyMove();
