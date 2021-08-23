#pragma once
struct Screen {
    int width = 20; // number of boxes
    int height = 20;
    int box = 25; // box pixels
    bool gameover = false;
    int bestScore;
    int speed = 5;
};
// used to store numbers needed to RGB and RGBA functions, that set the color scheme
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

void chooseScheme(ColorScheme* colorScheme); // used to select the game color scheme
int randomInt(int max); // generate random number in range from 0 to max
bool checkFruit(Screen* screen, Snake* snake, Fruit* fruit); // used to check if generated fruit is located "under snake body"
void generateFruit(Screen* screen, Snake* snake, Fruit* fruit); // generate fruit coordination
void setUp(Screen* screen, Snake* snake, Fruit* fruit); // set beginning snake coordinations and generate first fruit
void navigate(Screen* screen, Snake* snake, Fruit* fruit, int keycode); // take keyboard input to set snake direction
void move(Screen* screen, Snake* snake, Fruit* fruit); // move snake based on direction
void drawSnake(Screen* screen, Snake* snake, Fruit* fruit, ColorScheme* colorScheme); // draw snake based on color scheme
void gameOver(Screen* screen, Snake* snake, Fruit* fruit); // end game with saving best score
void collisionFruit(Screen* screen, Snake* snake, Fruit* fruit, ALLEGRO_SAMPLE* point); // eating function also increasing and printing score 
void collisionWall(Screen* screen, Snake* snake, Fruit* fruit); // gameover after collision with wall
void collisionTail(Screen* screen, Snake* snake, Fruit* fruit); // gameover after collision with tail
void checkCollisions(Screen* screen, Snake* snake, Fruit* fruit, ALLEGRO_SAMPLE* point); // check all three collision types