#pragma once
struct Screen {
    int width = 20; // number of boxes
    int height = 20;
    int box = 25; // box pixels
    bool gameover = false;
    int bestScore;
    int speed = 5;
};
struct ColorScheme {
    int background[3];
    float snake[4];
};
struct Snake {
    int len = 0;
    int x[200];
    int y[200];
    enum eDirecton { STOP = 0, LEFT, RIGHT, UP, DOWN };
    eDirecton dir = LEFT;
    bool cooldown = false;
};
struct Fruit {
    int x = 0;
    int y = 0;
};

void chooseScheme(ColorScheme* colorScheme);
int randomInt(int max);
bool checkFruit(Screen* screen, Snake* snake, Fruit* fruit);
void generateFruit(Screen* screen, Snake* snake, Fruit* fruit);
void setUp(Screen* screen, Snake* snake, Fruit* fruit);
void navigate(Screen* screen, Snake* snake, Fruit* fruit, int keycode);
void move(Screen* screen, Snake* snake, Fruit* fruit);
void drawSnake(Screen* screen, Snake* snake, Fruit* fruit, ColorScheme* colorScheme);
void gameOver(Screen* screen, Snake* snake, Fruit* fruit);
void collisionFruit(Screen* screen, Snake* snake, Fruit* fruit, ALLEGRO_SAMPLE* point);
void collisionWall(Screen* screen, Snake* snake, Fruit* fruit);
void collisionTail(Screen* screen, Snake* snake, Fruit* fruit);
void checkCollisions(Screen* screen, Snake* snake, Fruit* fruit, ALLEGRO_SAMPLE* point);