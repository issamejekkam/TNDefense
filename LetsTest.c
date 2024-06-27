#include "game.h"
#include <assert.h>
#include <stdio.h>

void test_canMove() {
int map[ROWS][COLS] = {
    {0, 0, 0, 0, 0},
    {1, 0, 0, 0, 1},
    {1, 0, 0, 0, 1},
    {1, 0, 0, 0, 1},
    {1, 1, 1, 1, 1}
};


 

    // Vérifier les mouvements possibles
    assert(canMove(1, 1, map, 1) == 0); 
    printf("\nTest_canMove DOWN : passed! \n");
    assert(canMove(20, 20, map, 4) == 1);
    printf("Test_canMove UP : passed! \n");
    assert(canMove(20, 20, map, 2) == 0);
    printf("Test_canMove LEFT  : passed! \n");
    assert(canMove(20, 20, map, 3) == 0);
    printf("Test_canMove RIGHT :  passed! \n");


    printf("\nTest_canMove : passed! \n");
}

void test_bulletTouchEnnemy() {
    struct Enemy enemies[10] = {0};
    struct Ducky duckies[10] = {0};
    struct Bullet bullet = {.rect = {100, 100, BULLET_SIZE, BULLET_SIZE}, .fired = 1, .direction = 3, .damage = 10};

    enemies[0].rect.x = 120; // Position de l'ennemi
    enemies[0].rect.y = 100;
    enemies[0].rect.w = SPRITE_SIZE;
    enemies[0].rect.h = SPRITE_SIZE;
    enemies[0].spawned = 1;
    enemies[0].hp = 20;

    int hit = bulletTouchEnnemy(duckies, bullet, enemies);
    assert(hit == 1); // La balle doit toucher l'ennemi
    printf("\nTest player hit : passed! \n");
    assert(enemies[0].hp == 10); // Les HP de l'ennemi doivent diminuer
    printf("Test Hp decreaed : passed! \n");
    if (enemies[0].hp <= 0) {
        enemies[0].spawned = 0;
    }
    struct Bullet bullet1 = {.rect = {200, 200, BULLET_SIZE, BULLET_SIZE}, .fired = 1, .direction = 3, .damage = 10};
    hit = bulletTouchEnnemy(duckies, bullet1, enemies);
    assert(hit == 0); // La balle doit toucher l'ennemi
    printf("Test player not hit : passed! \n");
    assert(enemies[0].hp == 10); // Les HP de l'ennemi doivent diminuer
    printf("Test Hp not decreaed : passed! \n");
    if (enemies[0].hp <= 0) {
        enemies[0].spawned = 0;
    }

    printf("\nTest_bulletTouchEnnemy passed! \n");
}

void test_createDucky() {
    struct Ducky duckies[10] = {0};
    int death_time = SDL_GetTicks();

    createDucky(duckies, 100, 100, death_time);
    assert(duckies[0].spawned == 1); // Vérifie que le ducky a été créé
    printf("\nDucky created : passed! \n");
    assert(duckies[0].rect.x == 120); // Vérifie la position du ducky
    printf("Ducky placed correctly in x : passed! \n");
    assert(duckies[0].rect.y == 120);
    printf("Ducky placed correctly in y : passed! \n");
    assert(duckies[0].time_spawned == death_time); 
     printf("Ducky placed in the right time : passed! \n");

    printf("\nTest_createDucky passed! \n");
}

void test_createBullet() {
    struct Bullet bullets[10] = {0};
    createBullet(1,bullets,100,100);
    assert(bullets[0].direction == 1); 
    assert(bullets[0].fired == 1); 
    assert(bullets[0].rect.x == 120); 
    assert(bullets[0].rect.y == 140);

    printf("\ntest_createBullet passed! \n");
}

void test_EnemyMove() {
    int map[ROWS][COLS] = {
        {0, 0, 0, 0, 0},
        {1, 0, 0, 0, 1},
        {1, 0, 0, 0, 1},
        {1, 0, 0, 0, 1},
        {1, 0, 0, 0, 1},
        {1, 0, 0, 0, 1},
        {1, 0, 0, 0, 1},
        {1, 1, 1, 1, 1},
        {1, 0, 0, 0, 1}
    };
    struct Enemy enemies[10] = {0};
    enemies[0].rect.x = 20; // Position de l'ennemi
    enemies[0].rect.y = 100;
    enemies[0].rect.w = SPRITE_SIZE;
    enemies[0].rect.h = SPRITE_SIZE;
    enemies[0].spawned = 1;
    enemies[0].hp = 20;
    EnemyMove(&enemies[0],map,20,20);
    assert(enemies[0].rect.y == 90);
    EnemyMove(&enemies[0],map,20,20);
    assert(enemies[0].rect.y == 80);
    printf("\nEnemy move in the right direction : passed! \n");





    printf("\ntest_EnemyMove passed! \n");
}

