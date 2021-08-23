#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_image.h>
#pragma warning(disable:4996)
FILE* file;

struct Screen {
    int width = 20; // number of boxes
    int height = 20;
    int box = 25; // box pixels
    bool gameover = false;
    int bestScore;
    int speed = 5;
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

int randomInt(int max) {
    return rand() % max;
}
bool checkFruit(Screen* screen, Snake* snake, Fruit* fruit) {
    bool result = false;
    for (int i = 0; i <= snake->len; i++) {
        if (fruit->x == snake->x[i] && fruit->y == snake->y[i]) result = true;
    }
    return result;
}
void generateFruit(Screen* screen, Snake* snake, Fruit* fruit) {
    // generate random position
    do {
        fruit->x = randomInt(screen->width);
        fruit->y = randomInt(screen->height);
    }     
    while (checkFruit(screen, snake, fruit));
}


void setUp(Screen* screen, Snake* snake, Fruit* fruit) {
    for (int i = 0; i < 200; i++) {
        snake->x[i] = 0;
        snake->y[i] = 0;
    }
    // initiate head position in the middle of the screen
    snake->x[0] = (screen->width) / 2;
    snake->y[0] = (screen->height) / 2;
    // length does not include the head
    snake->len = 0;
    generateFruit(screen, snake, fruit);
}

void navigate(Screen* screen, Snake* snake, Fruit* fruit, int keycode) {
    if (!snake->cooldown)
    {
        if (keycode == ALLEGRO_KEY_A && snake->dir != snake->RIGHT) {
            snake->dir = snake->LEFT;
            snake->cooldown = true;
        }
        else if (keycode == ALLEGRO_KEY_D && snake->dir != snake->LEFT) {
            snake->dir = snake->RIGHT;
            snake->cooldown = true;
        }
        else if (keycode == ALLEGRO_KEY_W && snake->dir != snake->DOWN) {
            snake->dir = snake->UP;
            snake->cooldown = true;
        }
        else if (keycode == ALLEGRO_KEY_S && snake->dir != snake->UP) {
            snake->dir = snake->DOWN;
            snake->cooldown = true;
        }
    }

}

void move(Screen* screen, Snake* snake, Fruit* fruit) {
    // move snake body
    for (int i = snake->len; i >= 0; i--) {
        snake->x[i + 1] = snake->x[i];
        snake->y[i + 1] = snake->y[i];
    }

    // get current change in head position
    int directionX = 0;
    int directionY = 0;

    if (snake->dir == snake->LEFT) directionX = -1;
    else if (snake->dir == snake->RIGHT) directionX = 1;
    else if (snake->dir == snake->UP) directionY = -1;
    else if (snake->dir == snake->DOWN) directionY = 1;

    // calculate new head position
    snake->x[0] = snake->x[1] + directionX;
    snake->y[0] = snake->y[1] + directionY;
}

void drawSnake(Screen* screen, Snake* snake, Fruit* fruit) {
    al_draw_filled_rectangle(
        snake->x[0] * screen->box,
        snake->y[0] * screen->box,
        snake->x[0] * screen->box + screen->box,
        snake->y[0] * screen->box + screen->box,
        al_map_rgba_f(0.035f, 0.71f, 0.043f, 1)
    );
    for (int i = 1; i <= snake->len; i++) {
        if (i % 2) {
            al_draw_filled_rectangle(
                snake->x[i] * screen->box,
                snake->y[i] * screen->box,
                snake->x[i] * screen->box + screen->box,
                snake->y[i] * screen->box + screen->box,
                al_map_rgba_f(0.078f, 0.35f, 0.059f, 1)
            );
        }
        else {
            al_draw_filled_rectangle(
                snake->x[i] * screen->box,
                snake->y[i] * screen->box,
                snake->x[i] * screen->box + screen->box,
                snake->y[i] * screen->box + screen->box,
                al_map_rgba_f(0.06f, 0.5f, 0.06f, 1)
            );
        }

    }
}

void gameOver(Screen* screen, Snake* snake, Fruit* fruit) {
    screen->gameover = true;
    file = fopen("bestScore.txt", "r");
    if (file == NULL) {
        FILE* file2;
        file2 = fopen("bestScore.txt", "w+");
        fprintf(file2, "%d", snake->len);
        fclose(file2);
    }
    else {
        fscanf(file, "%d", &(screen->bestScore));
        if (snake->len > screen->bestScore) {
            file = fopen("bestScore.txt", "w");
            fprintf(file, "%d", snake->len);
            fclose(file);
            system("cls");
            printf("New best score: %d", snake->len);
        }
        else {
            system("cls");
            printf("Game Over with score %d", snake->len);
        }
    }
}

void collisionFruit(Screen* screen, Snake* snake, Fruit* fruit/*, ALLEGRO_FONT* font*/) {
    if (fruit->x == snake->x[0] && fruit->y == snake->y[0]/*abs(snake->x[0] - fruit->x*screen->box) < screen->box && abs(snake->y[0] - fruit->y * screen->box) < screen->box*/) {
        generateFruit(screen, snake, fruit);
        system("cls");
        printf("current score:%d\n", ++snake->len);
   }
}

void collisionWall(Screen* screen, Snake* snake, Fruit* fruit) {
    if (snake->x[0] < 0 || snake->y[0] < 0 || snake->x[0] >= screen->width || snake->y[0] >= screen->height) 
    {
        gameOver(screen, snake, fruit);
    }
}

void collisionTail(Screen* screen, Snake* snake, Fruit* fruit) {
    for (int i = 1; i <= snake->len; i++) 
    {
        if (snake->x[0] == snake->x[i] && snake->y[0] == snake->y[i])
        {
            gameOver(screen, snake, fruit);
        }
    }
    
}
void checkCollisions(Screen* screen, Snake* snake, Fruit* fruit) {
    collisionFruit(screen, snake, fruit);
    collisionWall(screen, snake, fruit);
    collisionTail(screen, snake, fruit);
}
int main() {
    time_t t;
    srand((unsigned)time(&t));

    Screen screen;
    Snake snake;
    Fruit fruit;

    // printf("before setup: snake %dx%d, fruit %dx%d\n", snake.x[0], snake.y[0], fruit.x, fruit.y);
    setUp(&screen, &snake, &fruit);
    // printf("after setup: snake %dx%d, fruit %dx%d", snake.x[0], snake.y[0], fruit.x, fruit.y);

    if (!al_init())
    {
        printf("couldn't initialize allegro\n");
        return 1;
    }

    if (!al_install_keyboard())
    {
        printf("couldn't initialize keyboard\n");
        return 1;
    }

    ALLEGRO_TIMER* timer = al_create_timer(1.0 / screen.speed);
    if (!timer)
    {
        printf("couldn't initialize timer\n");
        return 1;
    }

    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
    if (!queue)
    {
        printf("couldn't initialize queue\n");
        return 1;
    }

    ALLEGRO_DISPLAY* disp = al_create_display(screen.width * screen.box, screen.height * screen.box);
    if (!disp)
    {
        printf("couldn't initialize display\n");
        return 1;
    }

    ALLEGRO_FONT* font = al_create_builtin_font();
    if (!font)
    {
        printf("couldn't initialize font\n");
        return 1;
    }
    if (!al_init_primitives_addon())
    {
        printf("couldn't initialize primitives\n");
        return 1;
    }
    if (!al_init_image_addon())
    {
        printf("couldn't initialize image\n");
        return 1;
    }
    ALLEGRO_BITMAP* berry = al_load_bitmap("strawberry.png");

    ALLEGRO_KEYBOARD_STATE keyState;
    al_get_keyboard_state(&keyState);

    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(disp));
    al_register_event_source(queue, al_get_timer_event_source(timer));

    bool done = false;
    bool redraw = true;

    al_start_timer(timer);

    int berryHeight = al_get_bitmap_height(berry);
    int berryWidth = al_get_bitmap_width(berry);

    while (!screen.gameover)
    {
        while (!al_is_event_queue_empty(queue)) {
            ALLEGRO_EVENT event;
            al_wait_for_event(queue, &event);
        

            switch (event.type)
            {
            case ALLEGRO_EVENT_TIMER:
                
                redraw = true;
                move(&screen, &snake, &fruit);
                checkCollisions(&screen, &snake, &fruit);
                snake.cooldown = false;
                break;

            case ALLEGRO_EVENT_KEY_DOWN:
                if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) done = true;
                else navigate(&screen, &snake, &fruit, event.keyboard.keycode);
                break;
            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                gameOver(&screen, &snake, &fruit);
                break;
            }
            /*
            if (screen.gameover)
                break;
            */
            if (redraw)
            {
                al_clear_to_color(al_map_rgb(255, 184, 237));
                drawSnake(&screen, &snake, &fruit);
                //printf("snake %dx%d, fruit %dx%d\n", snake.x[0], snake.y[0], fruit.x, fruit.y);
                al_draw_scaled_bitmap(berry, fruit.x * screen.box, fruit.y * screen.box, berryWidth, berryHeight, fruit.x * screen.box, fruit.y * screen.box, screen.box, screen.box, 0);
                //al_draw_textf(font, al_map_rgba(0, 0, 0, 0.5), (screen.width * screen.box) / 2, 0, ALLEGRO_ALIGN_CENTRE, "score:%d", snake.len);
                al_flip_display();

                //redraw = false;
            }
        }
    }
    al_destroy_bitmap(berry);
    al_destroy_font(font);
    al_destroy_display(disp);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);

    return 0;
}
