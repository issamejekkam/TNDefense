#include "game.h"

//function for managing errors
// { }

void SDL_ExitWithError(const char *message){ 
    SDL_Log("ERREUR : %s > %s\n",message,SDL_GetError());
    SDL_Quit();
    exit(EXIT_FAILURE);
}


void drawMap(SDL_Renderer* renderer, SDL_Texture** textures, int map[35][50]) {
    for (int i = 0; i < 35; i++) {
        for (int j = 0; j < 50; j++) {
            SDL_Rect srcRect = {0, 0, CELL_SIZE, CELL_SIZE};
            SDL_Rect destRect = {j * CELL_SIZE, i * CELL_SIZE, CELL_SIZE, CELL_SIZE};
            SDL_RenderCopy(renderer, textures[map[i][j]], &srcRect, &destRect);
        }
    }
}


void controlFps(int current_image_time, int last_image_time) { 
        int difference_images_time = current_image_time-last_image_time;
        if (difference_images_time<FPS_LIMIT){
            SDL_Delay(FPS_LIMIT-difference_images_time);
        }
}






bool isBlocked(int value) {
    return value >= 1 && value <= 25 && value!=23  && value!=2 && value!= 8&& value!= 7 && value!= 4;
}

bool isBlocked2(int value) {
    return value >= 1 && value <= 25 &&  value!= 8 && value!= 7 && value!=23 ;
}

int canMove(int player_x, int player_y, int map[ROWS][COLS], int mvt) {
    int i = player_x / CELL_SIZE;
    int j = player_y / CELL_SIZE;

    // Handle edge alignment issues.
    if (player_x % 20 != 0) {
        if (mvt == 2 || mvt == 3) {
            return 1;
        } else if (mvt == 1) {
            if (isBlocked(map[j+3][i]) || isBlocked(map[j+3][i+1]) || isBlocked(map[j+3][i+2]) || isBlocked(map[j+3][i+3])) {
                return 0;
            }
        } else if (mvt == 4) {
            if (isBlocked(map[j-1][i]) || isBlocked(map[j-1][i+1]) || isBlocked(map[j-1][i+2]) || isBlocked(map[j-1][i+3])) {
                return 0;
            }
        }
    } else if (player_y % 20 != 0) {
        if (mvt == 1 || mvt == 4) {
            return 1;
        } else if (mvt == 3) {
            if (isBlocked(map[j][i+3]) || isBlocked(map[j+1][i+3]) || isBlocked(map[j+2][i+3]) || isBlocked(map[j+3][i+3])) {
                return 0;
            }
        } else if (mvt == 2) {
            if (isBlocked(map[j][i-1]) || isBlocked(map[j+1][i-1]) || isBlocked(map[j+2][i-1]) || isBlocked(map[j+3][i-1])) {
                return 0;
            }
        }
    }

    // Direction-specific checks
    if (mvt == 1) { // Move Down
        if (isBlocked(map[j+3][i]) || isBlocked(map[j+3][i+1]) || isBlocked(map[j+3][i+2])) {
            return 0;
        }
    } else if (mvt == 4) { // Move Up
        if (isBlocked(map[j-1][i]) || isBlocked(map[j-1][i+1]) || isBlocked(map[j-1][i+2])) {
            return 0;
        }
    } else if (mvt == 3) { // Move Right
        if (isBlocked(map[j][i+3]) || isBlocked(map[j+1][i+3]) || isBlocked(map[j+2][i+3])) {
            return 0;
        }
    } else if (mvt == 2) { // Move Left
        if (isBlocked(map[j][i-1]) || isBlocked(map[j+1][i-1]) || isBlocked(map[j+2][i-1])) {
            return 0;
        }
    }

    return 1; // Default return value indicating movement is possible
}



void createBullet(int last_mvt, struct Bullet bullets[], int player_x, int player_y) { 
    int i;
    if (last_mvt == 2) {
        for (i = 0; i < 50; i++) { 
            if (bullets[i].fired == 0) {
                bullets[i].rect.x = player_x;
                bullets[i].rect.y = player_y + 20;
                bullets[i].rect.w = 20;
                bullets[i].rect.h = 20;
                bullets[i].fired = 1;
                bullets[i].direction = last_mvt;
                bullets[i].offset=0;
                bullets[i].rect2.x = 40;
                bullets[i].rect2.y = 0;
                bullets[i].rect2.w = 20;
                bullets[i].rect2.h = 20;
                break;
            }
        }
    }
    else if (last_mvt == 3) {
        for (i = 0; i < 50; i++) { 
            if (bullets[i].fired == 0) {
                bullets[i].rect.x = player_x+40;
                bullets[i].rect.y = player_y + 20;
                bullets[i].rect.w = 20;
                bullets[i].rect.h = 20;
                bullets[i].fired = 1;
                bullets[i].direction = last_mvt;
                bullets[i].offset=0;
                bullets[i].rect2.x = 80;
                bullets[i].rect2.y = 0;
                bullets[i].rect2.w = 20;
                bullets[i].rect2.h = 20;
                break;
            }
        }
    }
    else if (last_mvt == 4) {
        for (i = 0; i < 50; i++) { 
            if (bullets[i].fired == 0) {
                bullets[i].rect.x = player_x+20;
                bullets[i].rect.y = player_y;
                bullets[i].rect.w = 20;
                bullets[i].rect.h = 20;
                bullets[i].fired = 1;
                bullets[i].direction = last_mvt;
                bullets[i].offset=0;
                bullets[i].rect2.x = 120;
                bullets[i].rect2.y = 0;
                bullets[i].rect2.w = 20;
                bullets[i].rect2.h = 20;
                break;
            }
        }
    }
    else if (last_mvt == 1) {

        for (i = 0; i < 50; i++) { 
            if (bullets[i].fired == 0) {
                bullets[i].rect.x = player_x+20;
                bullets[i].rect.y = player_y+40;
                bullets[i].rect.w = 20;
                bullets[i].rect.h = 20;
                bullets[i].fired = 1;
                bullets[i].direction = last_mvt;
                bullets[i].offset=0;
                bullets[i].rect2.x = 0;
                bullets[i].rect2.y = 0;
                bullets[i].rect2.w = 20;
                bullets[i].rect2.h = 20;
                break;
            }
        }
    }
}

// retourne numero d avec qui ya collision sinon retourne 0
int bulletCanAdvance(struct Bullet bullet,int map[ROWS][COLS]){ 
    int i;
    int j;

    if(bullet.direction==2){ 
        i=(bullet.rect.x-20)/CELL_SIZE;
        j=(bullet.rect.y)/CELL_SIZE;
        if (isBlocked2(map[j][i])){ 
            return 0;
        }
    }
    else if(bullet.direction==3){ 
        i=(bullet.rect.x+20)/CELL_SIZE;
        j=(bullet.rect.y)/CELL_SIZE;
        if (isBlocked2(map[j][i])){ 
            return 0;
        }
    }
    if(bullet.direction==4){ 
        i=(bullet.rect.x)/CELL_SIZE;
        j=(bullet.rect.y-20)/CELL_SIZE;
        if (isBlocked2(map[j][i])){ 
            return 0;
        }
    }
    else if(bullet.direction==1){
        i=(bullet.rect.y+20)/CELL_SIZE;
        j=(bullet.rect.x)/CELL_SIZE;
        if (isBlocked2(map[i][j])){ 
            return 0;
        }
    }
    return 1;

}


int bulletTouchEnnemy(struct Ducky duckies[],struct Bullet bullet,struct Enemy enemies[]){ 
    int i;
    int x=bullet.rect.x ;
    int y=bullet.rect.y ;
    // u shoot from multiple 10 donc soit on a contact exterieur soit il rentre faiblement dans l ennemi
    /*
    if(bullet.direction==1){ 
        y+=20;
    }
    else if(bullet.direction==2){ 
        x-=20;
    }
    else if(bullet.direction==3){ 
        x+=20;
    }
    else if(bullet.direction==4){ 
        y-=20;
    }
    */
    int death_time;
    for(i=0;i<10;i++){ 
        if (enemies[i].spawned==1){ 
            if (x >= enemies[i].rect.x && x <= (enemies[i].rect.x + SPRITE_SIZE) &&
                y >= enemies[i].rect.y && y <= (enemies[i].rect.y + SPRITE_SIZE)) {

                    enemies[i].hp-=bullet.damage;
                    if(enemies[i].hp<=0){ 
                        enemies[i].spawned=0;
                        death_time=SDL_GetTicks();
                        createDucky(duckies,enemies[i].rect.x,enemies[i].rect.y,death_time);
                    }
                    return 1;
            }
            else if (x + BULLET_SIZE >= enemies[i].rect.x && x + BULLET_SIZE <= (enemies[i].rect.x + SPRITE_SIZE) &&
                y >= enemies[i].rect.y && y <= (enemies[i].rect.y + SPRITE_SIZE)) {

                    enemies[i].hp-=bullet.damage;
                    if(enemies[i].hp<=0){ 
                        enemies[i].spawned=0;
                        death_time=SDL_GetTicks();
                        createDucky(duckies,enemies[i].rect.x,enemies[i].rect.y,death_time);
                    }
                    return 1;
            }
            else if (x >= enemies[i].rect.x && x <= (enemies[i].rect.x + SPRITE_SIZE) &&
                y + BULLET_SIZE >= enemies[i].rect.y && y + BULLET_SIZE<= (enemies[i].rect.y + SPRITE_SIZE)) {
                    enemies[i].hp-=bullet.damage;
                    if(enemies[i].hp<=0){ 
                        enemies[i].spawned=0;
                        death_time=SDL_GetTicks();
                        createDucky(duckies,enemies[i].rect.x,enemies[i].rect.y,death_time);
                    }
                    return 1;
            }
            else if (x + BULLET_SIZE >= enemies[i].rect.x && x + BULLET_SIZE <= (enemies[i].rect.x + SPRITE_SIZE) &&
                y + BULLET_SIZE >= enemies[i].rect.y && y + BULLET_SIZE<= (enemies[i].rect.y + SPRITE_SIZE)) {
                    enemies[i].hp-=bullet.damage;
                    if(enemies[i].hp<=0){ 
                        enemies[i].spawned=0;
                        death_time=SDL_GetTicks();
                        createDucky(duckies,enemies[i].rect.x,enemies[i].rect.y,death_time);
                    }
                    return 1;
            }
        }
    }
    return 0;
}


void drawGrid(SDL_Renderer* renderer){ 
    if(SDL_SetRenderDrawColor(renderer,200,191,231,SDL_ALPHA_OPAQUE)!=0){ 
        SDL_ExitWithError("Choisir couleur dessinage grid echouee");
    }
    int j; 
    for(j=0;j<35;j++){ 
        if(SDL_RenderDrawLine(renderer,0,CELL_SIZE*j,1000,CELL_SIZE*j)!=0){ 
            SDL_ExitWithError("Dessin droite echouee");
        }
    }
    for(j=0;j<50;j++){ 
        if(SDL_RenderDrawLine(renderer,CELL_SIZE*j,0,CELL_SIZE*j,700)!=0){ 
            SDL_ExitWithError("Dessin droite echouee");
        }
    }
}

void createDucky(struct Ducky duckies[], int enemy_x, int enemy_y, int death_time){ 
    int i;
    for (i = 0; i < 10; i++) { 
        if (duckies[i].spawned == 0) {
            duckies[i].rect.x = enemy_x+20;
            duckies[i].rect.y = enemy_y + 20;
            duckies[i].rect.w = 20;
            duckies[i].rect.h = 20;
            duckies[i].spawned = 1;
            duckies[i].offset=0;
            duckies[i].time_spawned = death_time;
            duckies[i].rect2.x = 0;
            duckies[i].rect2.y = 0;
            duckies[i].rect2.w = 20;
            duckies[i].rect2.h = 20;
            break;
        }
    }

}

int playerTouchDucky(struct Ducky ducky,SDL_Rect DestSpritePlayerRect){
    int x = ducky.rect.x;
    int y = ducky.rect.y;
    if (x >= DestSpritePlayerRect.x && x <= (DestSpritePlayerRect.x + SPRITE_SIZE) &&
        y >= DestSpritePlayerRect.y && y <= (DestSpritePlayerRect.y + SPRITE_SIZE)) {
            return 1;
    }
    else if (x + BULLET_SIZE >= DestSpritePlayerRect.x && x + BULLET_SIZE <= (DestSpritePlayerRect.x + SPRITE_SIZE) &&
        y >= DestSpritePlayerRect.y && y <= (DestSpritePlayerRect.y + SPRITE_SIZE)) {
            return 1;
    }
    else if (x >= DestSpritePlayerRect.x && x <= (DestSpritePlayerRect.x + SPRITE_SIZE) &&
        y + BULLET_SIZE >= DestSpritePlayerRect.y && y + BULLET_SIZE<= (DestSpritePlayerRect.y + SPRITE_SIZE)) {
            return 1;
    }
    else if (x + BULLET_SIZE >= DestSpritePlayerRect.x && x + BULLET_SIZE <= (DestSpritePlayerRect.x + SPRITE_SIZE) &&
        y + BULLET_SIZE >= DestSpritePlayerRect.y && y + BULLET_SIZE<= (DestSpritePlayerRect.y + SPRITE_SIZE)) {
            return 1;
    }
    return 0;
}



// Initialise le générateur de nombres aléatoires
void initRandom() {
    srand(time(NULL));
}

// Génère un mouvement aléatoire (1: haut, 2: bas, 3: gauche, 4: droite)
int getRandomMove() {
    return 1 + rand() % 4;
}



// Déplace l'ennemi de manière aléatoire
void moveEnemyRandomly(Enemy* enemy, int map[ROWS][COLS]) {
    int move = getRandomMove();
    int x = enemy->rect.x / CELL_SIZE;
    int y = enemy->rect.y / CELL_SIZE;

    if (canMove(enemy->rect.x, enemy->rect.y, map, move)) {
        switch (move) {
            case 4: // haut
                enemy->rect.y -= CELL_SIZE/2;
                enemy->last_mvt=4;
                enemy->offset_y=3;
                enemy->offset_x++;     
                enemy->offset_x%=4;
                break;
            case 1: // bas
                enemy->rect.y += CELL_SIZE/2;
                enemy->last_mvt=1;
                enemy->offset_y=0;
                enemy->offset_x++;     
                enemy->offset_x%=4;
                break;
            case 2: // gauche
                enemy->rect.x -= CELL_SIZE/2;
                enemy->last_mvt=2;
                enemy->offset_y=1;
                enemy->offset_x++;     
                enemy->offset_x%=4;
                break;
            case 3: // droite
                enemy->rect.x += CELL_SIZE/2;
                enemy->last_mvt=3;
                enemy->offset_y=2;
                enemy->offset_x++;     
                enemy->offset_x%=4;
                break;
            default:
                break;
        }
    }
}
void EnemyMove(Enemy* enemy, int map[35][50], int player_x, int player_y) {
    // Calculer la vitesse en fonction du temps écoulé
    int moveAmount = CELL_SIZE / 2; // Déplacement lent

    // Déplacer horizontalement d'abord
    if (player_x > enemy->rect.x) {
        if (canMove(enemy->rect.x , enemy->rect.y, map, 3)) { // Vers la droite
            enemy->rect.x += moveAmount;
            enemy->last_mvt=3;
            enemy->offset_y=2;
            enemy->offset_x++;     
            enemy->offset_x%=4; 
            return;
        }
    } else if (player_x < enemy->rect.x) {
        if (canMove(enemy->rect.x , enemy->rect.y, map, 2)) { // Vers la gauche
            enemy->rect.x -= moveAmount;
            enemy->last_mvt=2;
            enemy->offset_y=1;
            enemy->offset_x++;     
            enemy->offset_x%=4;
            return;
        }
    }

    // Si aucun déplacement horizontal n'est possible, essayer de se déplacer verticalement
    if (player_y > enemy->rect.y) {
        if (canMove(enemy->rect.x, enemy->rect.y , map, 1)) { // Vers le bas
            enemy->rect.y += moveAmount;
            enemy->last_mvt=1;
            enemy->offset_y=0;
            enemy->offset_x++;     
            enemy->offset_x%=4;
            return;
        }
    } else if (player_y < enemy->rect.y) {
        if (canMove(enemy->rect.x, enemy->rect.y, map, 4)) { // Vers le haut
            enemy->rect.y -= moveAmount;
            enemy->last_mvt=4;
            enemy->offset_y=3;
            enemy->offset_x++;     
            enemy->offset_x%=4;
            return;
        }
    }

    // Si aucun déplacement n'est possible, déplacer l'ennemi de manière aléatoire
    moveEnemyRandomly(enemy, map);
}


