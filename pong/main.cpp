#include <iostream>
#include <SDL2/SDL.h>

#define WIDTH 1280
#define HEIGHT 720 
#define TITLE "PONG"

#define Z 122
#define S 115

#define UP 1073741906
#define DOWN 1073741905

#define BALL_SPEED 2
#define PADDLE_SPEED 30

#define COLOR_WHITE 0xffffffff
#define COLOR_BLACK 0x00000000

using namespace std;

class Paddle
{
public:
    int x,y;
    int width,height;
    int speed;

    void Draw(SDL_Surface* surface, Uint32 color)
    {
        SDL_Rect rect = (SDL_Rect) {this->x, this->y, this->width, this->height};
        SDL_FillRect(surface, &rect, color);
    };
    
    void Up()
    {
        this->y -= this->speed;

        if (this->y <= 20)
        {
            this->y = 20;
        }
    };
    
    void Down()
    {
        this->y += this->speed;

        if (this->y + this->height >= HEIGHT-20)
        {
            this->y = (HEIGHT - this->height) -20;
        }
    };
};

class Ball
{
public:
    float x,y;
    int speed_x,speed_y;
    int radius;

    void Draw(SDL_Surface* surface, Uint32 color)
    {
        double radius_squared = this->radius*this->radius;

        for (int x = this->x - this->radius; x <= this->x + this->radius; x++)
        {
            for (int y = this->y - this->radius; y <= this->y + this->radius; y++)
            {
                double distance_squared = (x-this->x)*(x-this->x) + (y-this->y)*(y-this->y);
                if (distance_squared < radius_squared)
                {
                    SDL_Rect pixel = (SDL_Rect) {x, y, 1, 1};
                    SDL_FillRect(surface, &pixel, color);
                }
            }
        }   
    };

    void Update(Paddle* p1, Paddle* p2)
    {
        this->x += this->speed_x;
        this->y += this->speed_y;

        if (this->x - this->radius <= 0)
        {
            this->speed_x *= -1;
            this->x = WIDTH/2 - 5;
            this->y = HEIGHT/2 - 5;
        }
        if (this->x + this->radius >= WIDTH)
        {
            this->speed_x *= -1;
            this->x = WIDTH/2 - 5;
            this->y = HEIGHT/2 - 5;
        }
        if (this->y - this->radius <= 0)
        {
            this->speed_y *= -1;
        }
        if (this->y + this->radius >= HEIGHT)
        {
            this->speed_y *= -1;
        }

        if (this->x - this->radius <= p1->x + p1->width && this->y >= p1->y && this->y <= p1->y + p1->height)
        {
            this->speed_x *= -1;
        }

        if (this->x >= p2->x && this->y >= p2->y && this->y <= p2->y + p2->height)
        {
            this->speed_x *= -1;
        }
    };
};

Ball ball;
Paddle player1;
Paddle player2;

int WinMain()
{
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0);
    SDL_Surface* surface = SDL_GetWindowSurface(window);
    SDL_Rect erase_rect = {0,0, WIDTH, HEIGHT};

    int simulation_running = 1;
    SDL_Event event;

    ball.x = WIDTH/2 - 5;
    ball.y = HEIGHT/2 - 5;
    ball.radius = 10;
    ball.speed_x = BALL_SPEED;
    ball.speed_y = BALL_SPEED;

    player1.x = 20;
    player1.y = HEIGHT/2 - 60;
    player1.width = 20;
    player1.height = 120;
    player1.speed = PADDLE_SPEED;

    player2.x = WIDTH-40;
    player2.y = HEIGHT/2 - 60;
    player2.width = 20;
    player2.height = 120;
    player2.speed = PADDLE_SPEED;

    while (simulation_running)
    {
        while (SDL_PollEvent(&event)) 
        {
            if (event.type == SDL_QUIT) 
            {
                simulation_running = 0;
            }
            
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == Z) 
            {
                cout << "Z" << endl;
                player1.Up();
            }

            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == S) 
            {
                cout << "S" << endl;
                player1.Down();
            }

            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == UP)
            {
                cout << "UP" << endl;
                player2.Up();
            }
            
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == DOWN)
            {
                cout << "DOWN" << endl;
                player2.Down();
            }
        }

        SDL_FillRect(surface, &erase_rect, COLOR_BLACK);

        ball.Draw(surface, COLOR_WHITE);
        ball.Update(&player1, &player2);

        player1.Draw(surface, COLOR_WHITE);
        player2.Draw(surface, COLOR_WHITE);

        SDL_UpdateWindowSurface(window);
        SDL_Delay(5);
    }

    SDL_DestroyWindow(window);
    SDL_FreeSurface(surface);

    return 0;
}
