#pragma once

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include <SDL_mixer.h>
#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iterator>

using namespace std;

//Structures
struct Obj_paddle
{
    double x;
    double y;
    double dx = 20;
    double w;
    double h;
};

struct Obj_ball
{
    double x;
    double y;
    double dx;
    double dy;
    bool dead;
    int damage;
};

struct brick
{
    int color; //1 is red. 2 is blue. 3 is green.
    int initial_damage;
    int damage;
};


//Global Variables
int level = 1;
int lives = 3;
int scores = 0;
int color = 1;
int num_rows = 3;
int ball_damage = 1;
double speed = 1.7; //optional
bool found = false;
bool quit;
bool load = false;
bool level_up;
bool paused = false;
bool paddledk = false;

Obj_paddle paddle;
Obj_ball ball;
SDL_Texture* paddle_tex;
SDL_Texture* ball_tex;
SDL_Rect paddle_rect;
SDL_Rect ball_rect;
SDL_Rect dstRect;
SDL_Texture* RedBrick_tex;
SDL_Texture* GreenBrick_tex;
SDL_Texture* BlueBrick_tex;
SDL_Texture* vbomb_tex;
SDL_Texture* life_tex;
SDL_Texture* hbomb_tex;
SDL_Texture* paddledk_tex;
SDL_Texture* tenpoints_tex;
SDL_Texture* bomb_tex;
SDL_Texture* lives_Tex;
SDL_Texture* Background_tex;
SDL_Texture* MenuBackground_tex;

Mix_Music* BackgMusic = NULL;
Mix_Chunk* gCollision = NULL;
Mix_Chunk* gBreak = NULL;
Mix_Chunk* gDie = NULL;
Mix_Chunk* gLevelUp = NULL;
Mix_Chunk* gBomb = NULL;
Mix_Chunk* gLife = NULL;
Mix_Chunk* gPDKon = NULL;
Mix_Chunk* gTenpoint = NULL;

string username;
vector <string> users;

//function declarations
void Initialize_paddle(SDL_Renderer* m_renderer);
void update_paddle(SDL_Renderer* ren);

void handle_events(SDL_Renderer* rend, SDL_Event event);

void Initialize_ball(SDL_Renderer* m_renderer);
void update_ball(SDL_Renderer* ren, brick** Bricks);

void Initialize_bricks(SDL_Renderer* m_renderer, brick** Bricks);
void update_bricks(SDL_Renderer* m_renderer, brick** Bricks);
bool check_empty(int row, brick** Bricks);

void import_users();
void Initial_Game(SDL_Renderer* m_renderer);
void show_scores(SDL_Renderer* m_renderer);
bool load_menu(SDL_Renderer* m_renderer);
void Quit_Menu(SDL_Renderer* m_renderer, brick** Bricks);
void show_lives(SDL_Renderer* m_renderer);
void PauseMenu(SDL_Renderer* m_renderer);

bool loadMedia();

void SDL_wait_key();

template <class Container>
void split(const std::string& str, Container& cont, char delim = ' ')
{
    std::stringstream ss(str);
    std::string token;
    while (std::getline(ss, token, delim)) {
        cont.push_back(token);
    }
}
void text_RGB(SDL_Renderer* m_renderer, string txt, int font_size, int x, int y, int R, int G, int B);
void window_color(SDL_Renderer* Renderer, int R, int G, int B);
void SDL_wait_space();
long int str_to_int(string s);
void rect(SDL_Renderer* Renderer, int x, int y, int w, int h, int R, int G, int B, int fill_boolian);
brick** Construct2Darray(int row, int col);



//Paddle Functions
void Initialize_paddle(SDL_Renderer* m_renderer)
{
    paddle.x = 500;
    paddle.y = 680;
    paddle.w = 200;
    paddle.h = 25;
    paddle_rect.x = 500;
    paddle_rect.y = 680;
    paddle_rect.w = paddle.w;
    paddle_rect.h = paddle.h;
    SDL_Surface* tmpSurf = IMG_Load("assets\\paddle.png");
    paddle_tex = SDL_CreateTextureFromSurface(m_renderer, tmpSurf);
    SDL_RenderCopy(m_renderer, paddle_tex, NULL, &paddle_rect);
    tmpSurf = IMG_Load("assets\\Heart.png");
    lives_Tex = SDL_CreateTextureFromSurface(m_renderer, tmpSurf);
    return;
}

void update_paddle(SDL_Renderer* ren)
{
    paddle_rect.x = paddle.x;
    SDL_RenderCopy(ren, paddle_tex, NULL, &paddle_rect);
    return;
}


//Handle Events
void handle_events(SDL_Renderer* rend, SDL_Event event)
{
    if (event.type == SDL_KEYDOWN)
    {
        switch (event.key.keysym.sym)
        {
        case SDLK_ESCAPE:
            quit = true;
            cout << "quit command\n";
            break;
        case SDLK_p:
            PauseMenu(rend);
            break;
        case SDLK_a:
            paddle.x -= (paddle.x - paddle.dx <= 0) ? paddle.x : paddle.dx;
            break;
        case SDLK_d:
            paddle.x += (paddle.x + paddle.w + paddle.dx >= 1200) ? 1200 - paddle.w - paddle.x : paddle.dx;
            break;
        default:
            break;
        }
    }
    return;
}


//Ball Functions
void Initialize_ball(SDL_Renderer* m_renderer)
{
    ball.x = 600 - 10;
    ball.y = 620;
    ball.damage = ball_damage;
    level_up = false;
    double theta = (rand() % 60 + 16) * M_PI / 180.0;
    ball.dx = (rand() % 2 == 0) ? speed * cos(theta) : -speed * cos(theta);
    ball.dy = -speed * sin(theta);
    ball.dead = false;
    ball_rect.x = ball.x;
    ball_rect.y = ball.y;
    ball_rect.w = 20;
    ball_rect.h = 20;
    SDL_Surface* tmpSurf = IMG_Load("assets\\ball.png");
    ball_tex = SDL_CreateTextureFromSurface(m_renderer, tmpSurf);
    SDL_RenderCopy(m_renderer, ball_tex, NULL, &ball_rect);
    SDL_RenderPresent(m_renderer);
    SDL_wait_space();
    return;
}

void update_ball(SDL_Renderer* ren, brick** Bricks)
{
    //check collision with walls
    if (ball.x < 0)
    {
        ball.dx = (-1.0) * ball.dx;
        Mix_PlayChannel(-1, gCollision, 0);
    }

    else if (ball.x > 1180)
    {
        ball.dx = (-1.0) * ball.dx;
        Mix_PlayChannel(-1, gCollision, 0);
    }

    if (ball.y < 0)
    {
        ball.dy = (-1.0) * ball.dy;
        Mix_PlayChannel(-1, gCollision, 0);
    }



    //check collision with paddle
    if (ball.dx > 0 && paddle.x < ball.x + 20 && ball.x + 20 < paddle.x + 20 && ball.y + 20 > paddle.y && ball.y + 20 < paddle.y + 5) {//tizi chape paddle /**/
        double alpha = (rand() % 30 + 20) * (M_PI / 180);
        ball.dy = -(speed * 1.5) * sin(alpha);
        ball.y += 5 * ball.dy;
        ball.dx = -(speed * 1.5) * cos(alpha);
        Mix_PlayChannel(-1, gCollision, 0);
    }
    if (ball.dx < 0 && paddle.x + paddle.w - 20 < ball.x && ball.x < paddle.x + paddle.w && ball.y + 20 > paddle.y && ball.y + 20 < paddle.y + 5) {//tizi raste paddle /**/
        double beta = (rand() % 30 + 20) * (M_PI / 180);
        ball.dy = -(speed * 1.5) * sin(beta);
        ball.y += 5 * ball.dy;
        ball.dx = (speed * 1.5) * cos(beta);
        Mix_PlayChannel(-1, gCollision, 0);
    }
    if (paddle.x - 20 < ball.x && ball.x < paddle.x + paddle.w && ball.y + 20 > 680 && ball.y + 20 < 685)  // vasate paddle
    {
        ball.dy = (-1.0) * ball.dy;
        ball.y += 5 * ball.dy;
        Mix_PlayChannel(-1, gCollision, 0);
    }
    if (ball.dx > 0 && ball.x + 20 > paddle.x - 3 && ball.x + 20 < paddle.x + 3 && ball.y + 20 > paddle.y && ball.y < paddle.y + 25)// baghale chape paddle
    {
        ball.dx *= -1;
        Mix_PlayChannel(-1, gCollision, 0);
    }
    if (ball.dx < 0 && ball.x > paddle.x + paddle.w - 3 && ball.x  < paddle.x + paddle.w + 3 && ball.y + 20 > paddle.y && ball.y < paddle.y + 25) {// baghale raste paddle
        ball.dx *= -1;
        Mix_PlayChannel(-1, gCollision, 0);
    }

    //check if ball passed the bottom of screen

    if (ball.y > 800)
    {
        ball.dead = true;
        ball_damage = ball.damage;
        lives--;
        Mix_PlayChannel(-1, gDie, 0);
        if (lives == 0)
            quit = true;
    }


    //check collision with bricks
    if (ball.y + 10 < (num_rows) * (50)) {

        int r, c;
        r = (int)(ball.y + 10) / 50 + 18 - num_rows;

        c = (int)(ball.x + 10) / 150;

        int x = 10 + 150 * c;
        int y = 20 * (r - (17 - num_rows)) + (r - (18 - num_rows)) * 30 - 10;

        if (Bricks[r][c].color != 0)
        {
            if (ball.y + 10 > y && ball.y + 10 < y + 30 && ball.x + 10 > x - 10 && ball.x + 10 < x) { //jenah chap
                ball.dx *= -1;
                ball.damage--;
            }
            else if (ball.y + 10 > y && ball.y + 10 < y + 30 && ball.x + 10 > x + 130 && ball.x + 10 < x + 140) { //jenah rast
                ball.dx *= -1;
                ball.damage--;
            }
            else if (ball.x + 10 >= x - 10 && ball.x + 10 <= x && ball.y + 10 <= y + 40 && ball.y + 10 >= y + 30) {
                double omega = (rand() % 30 + 20) * (M_PI / 180);                  //gooshe chap paiin
                ball.dx = -(speed * 1.5) * cos(omega);
                ball.dy = (speed * 1.5) * sin(omega);
                ball.damage--;
            }
            else if (ball.x + 10 >= x + 130 && ball.x <= x + 140 && ball.y + 10 <= y + 40 && ball.y + 10 >= y + 30) {
                double delta = (rand() % 30 + 20) * (M_PI / 180);                  //gooshe rast paiin
                ball.dx = (speed * 1.5) * cos(delta);
                ball.dy = (speed * 1.5) * sin(delta);
                ball.damage--;
            }
            else if (ball.x + 10 >= x - 10 && ball.x + 10 <= x && ball.y + 10 <= y && ball.y + 10 >= y - 10) {
                double omega = (rand() % 30 + 20) * (M_PI / 180);                  //gooshe chap bala
                ball.dx = -(speed * 1.5) * cos(omega);
                ball.dy = -(speed * 1.5) * sin(omega);
                ball.damage--;
            }
            else if (ball.x + 10 >= x + 130 && ball.x + 10 <= x + 140 && ball.y + 10 < y && ball.y + 10 > y - 10) {
                double delta = (rand() % 30 + 20) * (M_PI / 180);                  //gooshe rast bala
                ball.dx = (speed * 1.5) * cos(delta);
                ball.dy = -(speed * 1.5) * sin(delta);
                ball.damage--;
            }


            else if (ball.x + 10 > x && ball.x + 10 < x + 130 && ball.y + 10 > y + 30 && ball.y + 10 < y + 40) {
                ball.dy = -1.0 * ball.dy;
                ball.damage--;
            }
            else if (ball.x + 10 > x && ball.x + 10 < x + 130 && ball.y + 10 > y - 10 && ball.y + 10 < y) {
                ball.dy = -1.0 * ball.dy;
                ball.damage--;
            }


            //Emtiazi
            //Jun
            if (Bricks[r][c].color == 4) {
                lives += 1;
                Mix_PlayChannel(-1, gLife, 0);
            }

            //Amudi
            else if (Bricks[r][c].color == 5) {
                for (int p = 17; p > -1; p--) {
                    if (Bricks[p][c].color != 0) {
                        Bricks[p][c].color = 0;
                        Bricks[p][c].damage = 0;
                        scores += level;
                        Mix_PlayChannel(-1, gBomb, 0);
                    }
                }
            }

            //Ofoghi
            else if (Bricks[r][c].color == 6) {
                for (int u = 0; u < 8; u++) {
                    if (Bricks[r][u].color != 0) {
                        Bricks[r][u].color = 0;
                        Bricks[r][u].damage = 0;
                        scores += level;
                        Mix_PlayChannel(-1, gBomb, 0);
                    }
                }
            }

            //10 points
            else if (Bricks[r][c].color == 7) {
                scores += 10;
                Mix_PlayChannel(-1, gTenpoint, 0);
            }

            //paddle deraz kon
            else if (Bricks[r][c].color == 8) {
                paddle.w *= 1.5;
                paddle_rect.w *= 1.5;
                Mix_PlayChannel(-1, gPDKon, 0);
            }

            //bomb alam
            else if (Bricks[r][c].color == 9) {
                for (int a = r - 1; a <= r + 1; a++)
                    for (int b = c - 1; b <= c + 1; b++) {
                        if (a > -1 && a < 18 && b > -1 && b < 8)
                        {
                            if (Bricks[a][b].color != 0) {
                                Bricks[a][b].color = 0;
                                Bricks[a][b].damage = 0;
                                scores += level;
                                Mix_PlayChannel(-1, gBomb, 0);
                            }
                        }

                    }
            }


            Bricks[r][c].damage--;

            if (Bricks[r][c].damage <= 0)
            {
                if(Bricks[r][c].color < 4)
                    Mix_PlayChannel(-1, gBreak, 0);
                Bricks[r][c].color = 0;
                Bricks[r][c].damage = 0;
                scores += level;
            }
            else if(Bricks[r][c].color < 4)
                Mix_PlayChannel(-1, gCollision, 0);

        }
    }

    //level up
    if (ball.damage == 0)
    {
        level++;
        Mix_PlayChannel(-1, gLevelUp, 0);
        ball.damage = level;
        int r = 17 - num_rows;
        for (int col = 0; col < 8; col++)
        {
            Bricks[r][col].color = (color % 3 == 0) ? 3 : color % 3;
            Bricks[r][col].initial_damage = rand() % level + 1;
            Bricks[r][col].damage = Bricks[r][col].initial_damage;
        }
        color++;
        num_rows++;
        int c = rand() % 8;
        int halat = rand() % 6 + 4;
        if (halat == 4) {
            Bricks[r][c].color = 4;
            Bricks[r][c].damage = 1;
        }
        else if (halat == 5) {
            Bricks[r][c].color = 5;
            Bricks[r][c].damage = 1;
        }
        else if (halat == 6) {
            Bricks[r][c].color = 6;
            Bricks[r][c].damage = 1;
        }
        else if (halat == 7) {
            Bricks[r][c].color = 7;
            Bricks[r][c].damage = 1;
        }
        else if (halat == 8 && paddledk == false) {
            Bricks[r][c].color = 8;
            Bricks[r][c].damage = 1;
            paddledk = true;
        }
        else if (halat == 9) {
            Bricks[r][c].color = 9;
            Bricks[r][c].damage = 1;
        }

        int e = rand() % 8;
        Bricks[r][e].color = 0;

        if (num_rows == 14)
        {
            quit = true;
            lives = 0;
        }
    }

    ball.x += ball.dx;
    ball.y += ball.dy;
    //printf("%f %f\n", ball.x, ball.y);
    ball_rect.x = ball.x;
    ball_rect.y = ball.y;
    SDL_RenderCopy(ren, ball_tex, NULL, &ball_rect);
    return;
}

//Brick functions
void Initialize_bricks(SDL_Renderer* m_renderer, brick** Bricks)
{
    dstRect.w = 130;
    dstRect.h = 30;
    //Textures
    SDL_Surface* tmpSurf = IMG_Load("assets\\bricks\\1.png");
    RedBrick_tex = SDL_CreateTextureFromSurface(m_renderer, tmpSurf);
    tmpSurf = IMG_Load("assets\\bricks\\2.png");
    BlueBrick_tex = SDL_CreateTextureFromSurface(m_renderer, tmpSurf);
    tmpSurf = IMG_Load("assets\\bricks\\3.png");
    GreenBrick_tex = SDL_CreateTextureFromSurface(m_renderer, tmpSurf);

    tmpSurf = IMG_Load("assets\\bricks\\4.png");
    life_tex = SDL_CreateTextureFromSurface(m_renderer, tmpSurf);
    tmpSurf = IMG_Load("assets\\bricks\\5.png");
    vbomb_tex = SDL_CreateTextureFromSurface(m_renderer, tmpSurf);
    tmpSurf = IMG_Load("assets\\bricks\\6.png");
    hbomb_tex = SDL_CreateTextureFromSurface(m_renderer, tmpSurf);
    tmpSurf = IMG_Load("assets\\bricks\\7.png");
    tenpoints_tex = SDL_CreateTextureFromSurface(m_renderer, tmpSurf);
    tmpSurf = IMG_Load("assets\\bricks\\8.png");
    paddledk_tex = SDL_CreateTextureFromSurface(m_renderer, tmpSurf);
    tmpSurf = IMG_Load("assets\\bricks\\9.png");
    bomb_tex = SDL_CreateTextureFromSurface(m_renderer, tmpSurf);

    if (load)
    {
        ifstream import_file;
        import_file.open("History\\" + username + "_saved.txt");
        bool file_opened = false;
        if (import_file)
            file_opened = true;
        if (file_opened)
        {
            string tmp;
            getline(import_file, tmp);
            scores = str_to_int(tmp);
            cout << scores << endl;
            getline(import_file, tmp);
            lives = str_to_int(tmp);
            getline(import_file, tmp);
            level = str_to_int(tmp);
            vector <string> dst;
            for (int i = 0; i < 18; i++)
            {
                getline(import_file, tmp);
                split(tmp, dst);
                for (int j = 0; j < 24; j += 3)
                {
                    Bricks[i][j / 3].initial_damage = str_to_int(dst[j]);
                    Bricks[i][j / 3].damage = str_to_int(dst[j + 1]);
                    Bricks[i][j / 3].color = str_to_int(dst[j + 2]);

                    //cout << Bricks[i][j / 2].initial_damage << " " << Bricks[i][j / 2].damage << " " << Bricks[i][j / 2].color << "\t";
                }
                //cout << dst.size() << endl;
                dst.clear();

            }

            getline(import_file, tmp);
            ball.damage = str_to_int(tmp);
            getline(import_file, tmp);
            num_rows = str_to_int(tmp);
            getline(import_file, tmp);
            color = str_to_int(tmp);
            update_bricks(m_renderer, Bricks);
            SDL_RenderPresent(m_renderer);
            import_file.close();
            string s = "History\\" + username + "_saved.txt";
            int rem = remove(s.c_str());
            //cout << rem << endl;
            return;
        }

        SDL_RenderClear(m_renderer);
        text_RGB(m_renderer, "Saved File Not Found!", 36, 600, 400, 255, 255, 255);
        text_RGB(m_renderer, "Press Any key to continue", 28, 600, 480, 255, 255, 255);
        SDL_RenderPresent(m_renderer);
        SDL_wait_key();
    }

    for (int row = 17; row > 14; row--)
    {
        for (int col = 0; col < 8; col++)
        {
            dstRect.x = 10 + col * 150;
            dstRect.y = 20 * (row - (17 - num_rows)) + (row - (18 - num_rows)) * 30 - 10;

            int halat = rand() % 100;
            if (halat == 4) {
                Bricks[row][col].color = 4;
            }
            else if (halat == 5) {
                Bricks[row][col].color = 5;
            }
            else if (halat == 6) {
                Bricks[row][col].color = 6;
            }
            else if (halat == 7) {
                Bricks[row][col].color = 7;
            }
            else if (halat == 8) {
                Bricks[row][col].color = 8;
                paddledk = true;
            }
            else if (halat == 9) {
                Bricks[row][col].color = 9;
            }
            else {
                Bricks[row][col].color = color;
            }
            Bricks[row][col].initial_damage = 1;
            Bricks[row][col].damage = 1;

            switch (Bricks[row][col].color)
            {
                /*case 0:
                    break;*/
            case 1:
                SDL_RenderCopy(m_renderer, RedBrick_tex, NULL, &dstRect);
                break;
            case 2:
                SDL_RenderCopy(m_renderer, BlueBrick_tex, NULL, &dstRect);
                break;
            case 3:
                SDL_RenderCopy(m_renderer, GreenBrick_tex, NULL, &dstRect);
                break;
            case 4:
                SDL_RenderCopy(m_renderer, life_tex, NULL, &dstRect);
                break;
            case 5:
                SDL_RenderCopy(m_renderer, vbomb_tex, NULL, &dstRect);
                break;
            case 6:
                SDL_RenderCopy(m_renderer, hbomb_tex, NULL, &dstRect);
                break;
            case 7:
                SDL_RenderCopy(m_renderer, tenpoints_tex, NULL, &dstRect);
                break;
            case 8:
                SDL_RenderCopy(m_renderer, paddledk_tex, NULL, &dstRect);
                break;
            case 9:
                SDL_RenderCopy(m_renderer, bomb_tex, NULL, &dstRect);
                break;
            }
        }
        color++;
    }

    return;
}

void update_bricks(SDL_Renderer* m_renderer, brick** Bricks)
{
    int erased = 0;
    for (int i = 0; i < num_rows; i++)
    {
        int r = 18 - num_rows + i;
        for (int col = 0; col < 8; col++)
        {
            dstRect.x = 10 + col * 150;
            dstRect.y = 10 + i * 50;
            switch (Bricks[r][col].color)
            {
                /*case 0:
                    break;*/
            case 1:
                SDL_RenderCopy(m_renderer, RedBrick_tex, NULL, &dstRect);
                break;
            case 2:
                SDL_RenderCopy(m_renderer, BlueBrick_tex, NULL, &dstRect);
                break;
            case 3:
                SDL_RenderCopy(m_renderer, GreenBrick_tex, NULL, &dstRect);
                break;
            case 4:
                SDL_RenderCopy(m_renderer, life_tex, NULL, &dstRect);
                break;
            case 5:
                SDL_RenderCopy(m_renderer, vbomb_tex, NULL, &dstRect);
                break;
            case 6:
                SDL_RenderCopy(m_renderer, hbomb_tex, NULL, &dstRect);
                break;
            case 7:
                SDL_RenderCopy(m_renderer, tenpoints_tex, NULL, &dstRect);
                break;
            case 8:
                SDL_RenderCopy(m_renderer, paddledk_tex, NULL, &dstRect);
                break;
            case 9:
                SDL_RenderCopy(m_renderer, bomb_tex, NULL, &dstRect);
                break;
            }
        }

        if (check_empty(r, Bricks))
        {
            for (int j = i - 1; j > -1; j--)
            {
                int row = 18 - num_rows + j;
                for (int c = 0; c < 8; c++)
                    Bricks[row + 1][c] = Bricks[row][c];
            }
            erased++;
        }

    }

    num_rows -= erased;

    return;
}

bool check_empty(int row, brick** Bricks)
{
    for (int i = 0; i < 8; i++)
        if (Bricks[row][i].color != 0)
            return false;
    return true;
}


//Menu functions
void Initial_Game(SDL_Renderer* m_renderer)
{

    SDL_RenderClear(m_renderer);
    SDL_RenderCopy(m_renderer, MenuBackground_tex, NULL, NULL);
    //Logo and names and etc.
    text_RGB(m_renderer, "Welcome to Brick Smasher!", 36, 600, 300, 255, 255, 255);

    text_RGB(m_renderer, "By PJ and HA", 22, 600, 350, 255, 255, 255);

    SDL_Surface* tmpSurf = IMG_Load("assets\\Logo.png");
    SDL_Texture* Logo_tex = SDL_CreateTextureFromSurface(m_renderer, tmpSurf);
    SDL_Rect dstRect;
    dstRect.x = 0, dstRect.y = 0, dstRect.w = 400, dstRect.h = 293;
    SDL_RenderCopy(m_renderer, Logo_tex, NULL, &dstRect);

    SDL_Event event;
    string txt = "Player name: ";
    text_RGB(m_renderer, txt, 22, 600, 400, 255, 0, 0);
    SDL_RenderPresent(m_renderer);
    bool enter = false;
    while (!enter)
    {

        SDL_PollEvent(&event);

        if (event.type == SDL_KEYDOWN && (event.key.keysym.sym == SDLK_ESCAPE || event.key.keysym.sym == SDLK_RETURN))
        {
            enter = true;
        }

        if (event.type == SDL_KEYDOWN && event.key.keysym.sym != SDLK_ESCAPE)
        {
            SDL_RenderClear(m_renderer);
            SDL_RenderCopy(m_renderer, MenuBackground_tex, NULL, NULL);
            SDL_RenderCopy(m_renderer, Logo_tex, NULL, &dstRect);
            text_RGB(m_renderer, "Welcome to Brick Smasher!", 36, 600, 300, 255, 255, 255);
            text_RGB(m_renderer, "By PJ and HA", 22, 600, 350, 255, 255, 255);
            if (3 < SDL_GetScancodeFromKey(event.key.keysym.sym) && SDL_GetScancodeFromKey(event.key.keysym.sym) < 30)
            {
                txt += (char)(SDL_GetScancodeFromKey(event.key.keysym.sym) - 4 + 65);
                text_RGB(m_renderer, txt, 22, 600, 400, 255, 0, 0);

                SDL_RenderPresent(m_renderer);
            }

            if (SDL_GetScancodeFromKey(event.key.keysym.sym) == 44)
            {
                txt += (char)32;
                text_RGB(m_renderer, txt, 22, 600, 400, 255, 0, 0);

                SDL_RenderPresent(m_renderer);
            }

            if (29 < SDL_GetScancodeFromKey(event.key.keysym.sym) && SDL_GetScancodeFromKey(event.key.keysym.sym) < 40)
            {
                if (SDL_GetScancodeFromKey(event.key.keysym.sym) == 39)
                    txt += (char)48;
                else
                    txt += (char)(SDL_GetScancodeFromKey(event.key.keysym.sym) - 30 + 49);

                text_RGB(m_renderer, txt, 22, 600, 400, 255, 0, 0);
                SDL_RenderPresent(m_renderer);
            }

            if (SDL_GetScancodeFromKey(event.key.keysym.sym) == 42)
            {
                txt = (txt.length() > 13) ? txt.substr(0, txt.length() - 1) : txt;

                text_RGB(m_renderer, txt, 22, 600, 400, 255, 0, 0);
                SDL_RenderPresent(m_renderer);
                SDL_Delay(350);
            }

        }
    }
    text_RGB(m_renderer, txt, 22, 600, 400, 255, 0, 0);
    username = txt.substr(13, txt.length());

    import_users();

    if (found)
    {
        string str = "Welcome back " + username + "!";
        text_RGB(m_renderer, str, 24, 600, 450, 255, 255, 255);
    }
    text_RGB(m_renderer, "Press any key to Start!", 24, 600, 500, 255, 255, 255);
    SDL_RenderPresent(m_renderer);
    SDL_wait_key();
    return;
}

void import_users()
{
    ifstream Read_players("History\\players.txt");
    string input;

    int users_num = 0;
    while (getline(Read_players, input))
    {
        users.push_back(input);
        users_num++;
        if (input == username)
            found = true;
    }

    if (!found)
    {
        ofstream fileOUT("History\\players.txt", ios::app); // open filename.txt in append mode

        fileOUT << username << endl; // append "some stuff" to the end of the file

        fileOUT.close(); // close the file

        string outp_filename = "History\\" + username + ".txt";

        ofstream history_file(outp_filename);

        history_file.close();
    }
    return;
}


void show_scores(SDL_Renderer* m_renderer)
{
    vector <string> vec_scores;
    text_RGB(m_renderer, "Recent Scores", 24, 150, 550, 255, 255, 255);
    string str = "History\\" + username + ".txt";
    //cout << str << endl;
    string score;
    ifstream score_history(str);
    int len = 0;
    while (getline(score_history, score))
    {
        len++;
        //cout << score << endl;
        vec_scores.push_back(score);
    }

    if (len >= 5)
    {
        int cnt = 1;
        for (int i = len - 1; i > len - 6; i--) {
            text_RGB(m_renderer, vec_scores[i], 22, 150, 550 + cnt * 40, 255, 255, 255);
            //cout << vec_scores[i] << endl;
            cnt++;
        }

    }

    else
    {
        int cnt = 1;
        for (int i = len - 1; i > -1; i--) {
            text_RGB(m_renderer, vec_scores[i], 22, 150, 550 + cnt * 40, 255, 255, 255);
            //cout << vec_scores[i] << endl;
            cnt++;
        }

    }
    score_history.close();
    return;
}

void show_lives(SDL_Renderer* m_renderer)
{
    SDL_Rect dstR;
    dstR.y = 740;
    dstR.w = 60;
    dstR.h = 60;
    if (lives <= 3)
    {
        for (int i = 0; i < lives; i++)
        {
            dstR.x = 1080 + i * 31;
            SDL_RenderCopy(m_renderer, lives_Tex, NULL, &dstR);
        }
        return;
    }

    dstR.x = 1090;
    text_RGB(m_renderer, to_string(lives) + " x ", 18, 1088, 770, 0, 0, 0);
    SDL_RenderCopy(m_renderer, lives_Tex, NULL, &dstR);
    return;
}

bool load_menu(SDL_Renderer* m_renderer)
{
    SDL_RenderClear(m_renderer);
    text_RGB(m_renderer, "New Game", 25, 600, 350, 255, 255, 255);
    text_RGB(m_renderer, "Load", 25, 600, 450, 255, 255, 255);
    SDL_Surface* tmpSurf = IMG_Load("assets\\pointer.png");
    SDL_Texture* pointer_tex = SDL_CreateTextureFromSurface(m_renderer, tmpSurf);
    SDL_Rect dstRect;
    dstRect.h = 25, dstRect.w = 16, dstRect.x = 510, dstRect.y = 336;
    SDL_RenderCopy(m_renderer, pointer_tex, NULL, &dstRect);
    show_scores(m_renderer);
    SDL_RenderPresent(m_renderer);
    bool up = true;
    bool exit = false;
    SDL_Event event;
    Uint32 time;
    int FPS = 1000 / 60, dt;
    int R = 255, B = 255, dR = -5, dB = -3;
    while (!exit)
    {
        time = SDL_GetTicks();
        SDL_PollEvent(&event);
        if (event.type == SDL_KEYDOWN)
        {
            switch (event.key.keysym.sym)
            {
            case SDLK_RETURN:
                exit = true;
                break;
            case SDLK_ESCAPE:
                exit = true;
                break;
            case SDLK_UP:
                up = true;
                break;
            case SDLK_DOWN:
                up = false;
                break;
            default:
                break;
            }
        }

        SDL_SetRenderDrawColor(m_renderer, R, 0, B, 255);
        SDL_RenderClear(m_renderer);
        if (up)
        {
            rect(m_renderer, 530, 284, 45, 10, R, 0, B, 1);
            dstRect.y = 336;
            dstRect.x = 510;
            SDL_RenderCopy(m_renderer, pointer_tex, NULL, &dstRect);
        }

        else
        {
            rect(m_renderer, 510, 336, 16, 25, R, 0, B, 1);
            dstRect.y = 436;
            dstRect.x = 545;
            SDL_RenderCopy(m_renderer, pointer_tex, NULL, &dstRect);
        }


        text_RGB(m_renderer, "New Game", 25, 600, 350, 255, 255, 255);
        text_RGB(m_renderer, "Load", 25, 600, 450, 255, 255, 255);
        show_scores(m_renderer);
        SDL_RenderPresent(m_renderer);
        dt = SDL_GetTicks() - time;
        if (FPS > dt)
            SDL_Delay(FPS - dt);
        R += dR;

        if (R == 255 || R == 0)
            dR = (-1) * dR;
        if (B == 255 || B == 0)
            dB = (-1) * dB;
    }

    if (up)
    {
        return false;
    }

    return true;
}

void Quit_Menu(SDL_Renderer* m_renderer, brick** Bricks)
{
    if (lives == 0)
    {
        ofstream fileOUT("History\\" + username + ".txt", ios::app); // open filename.txt in append mode
        //cout << "file opened\n";
        fileOUT << scores << endl; // append "some stuff" to the end of the file

        fileOUT.close(); // close the file

        SDL_RenderClear(m_renderer);
        text_RGB(m_renderer, "GAME OVER!", 50, 600, 400, 255, 255, 255);
        SDL_RenderPresent(m_renderer);
        SDL_wait_key();
        return;
    }



    text_RGB(m_renderer, "Do you want to save your game?!", 36, 600, 250, 0, 0, 255);
    text_RGB(m_renderer, "YES", 25, 600, 300, 0, 0, 255);
    text_RGB(m_renderer, "NO", 25, 600, 370, 0, 0, 255);
    SDL_Surface* tmpSurf = IMG_Load("assets\\pointer.png");
    SDL_Texture* pointer_tex = SDL_CreateTextureFromSurface(m_renderer, tmpSurf);
    SDL_Rect dstRect;
    dstRect.h = 25, dstRect.w = 16, dstRect.x = 550, dstRect.y = 284;
    SDL_RenderCopy(m_renderer, pointer_tex, NULL, &dstRect);
    bool up = true;
    SDL_Event event;
    bool exit = false;
    while (!exit)
    {
        SDL_PollEvent(&event);
        if (event.type == SDL_KEYDOWN)
        {
            switch (event.key.keysym.sym)
            {
            case SDLK_RETURN:
                exit = true;
                break;
            case SDLK_ESCAPE:
                exit = true;
                up = false;
                break;
            case SDLK_UP:
                up = true;
                break;
            case SDLK_DOWN:
                up = false;
                break;
            default:
                break;
            }
        }


        if (up)
        {
            rect(m_renderer, 530, 284, 45, 100, 0, 0, 0, 1);
            dstRect.y = 284;
            SDL_RenderCopy(m_renderer, pointer_tex, NULL, &dstRect);
        }

        else
        {
            rect(m_renderer, 530, 284, 45, 100, 0, 0, 0, 1);
            dstRect.y = 355;
            SDL_RenderCopy(m_renderer, pointer_tex, NULL, &dstRect);
        }

        SDL_RenderPresent(m_renderer);
    }

    if (up)
    {
        SDL_RenderClear(m_renderer);
        text_RGB(m_renderer, "Saving...", 36, 600, 400, 255, 0, 0);
        SDL_RenderPresent(m_renderer);
        string dir = "History\\" + username + "_saved.txt";
        ofstream Save_file(dir);
        Save_file << scores << endl; //first line is the score
        Save_file << lives << endl; //second line is lives
        Save_file << level << endl; //third line is level
        //next lines are bricks array;
        for (int i = 0; i < 18; i++)
        {
            int color = 0;
            for (int j = 0; j < 8; j++)
            {
                Save_file << Bricks[i][j].initial_damage << " " << Bricks[i][j].damage << " " << Bricks[i][j].color << " ";
            }
            Save_file << endl;
        }
        //last lines are ball damage and num_rows and color
        Save_file << ball.damage << endl;
        Save_file << num_rows << endl;
        Save_file << color << endl;
        Save_file.close();
        SDL_Delay(1500);
    }

    return;
}

void PauseMenu(SDL_Renderer* m_renderer)
{
    cout << "enter pause menu\n";
    SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
    SDL_RenderClear(m_renderer);
    text_RGB(m_renderer, "Hold Enter to Resume", 25, 600, 300, 0, 0, 255);
    text_RGB(m_renderer, "Press Esc to Quit", 25, 600, 370, 255, 0, 255);
    SDL_Surface* tmpSurf = IMG_Load("assets\\pointer.png");
    SDL_Texture* pointer_tex = SDL_CreateTextureFromSurface(m_renderer, tmpSurf);
    SDL_Rect dstRect;
    dstRect.h = 25, dstRect.w = 16, dstRect.x = 530, dstRect.y = 284;

    bool up = false;
    SDL_Event event;
    bool exit = false;
    while (!exit)
    {
        SDL_PollEvent(&event);
        if (event.type == SDL_KEYDOWN)
        {
            switch (event.key.keysym.sym)
            {
            case SDLK_RETURN:
                exit = true;
                up = true;
                break;
            case SDLK_ESCAPE:
                exit = true;
                up = false;
                break;

            default:
                break;
            }
        }

        SDL_RenderPresent(m_renderer);
    }

    if (!up)
    {
        quit = true;
    }
    SDL_RenderClear(m_renderer);
    SDL_RenderPresent(m_renderer);
    return;

}


//Audio
bool loadMedia()
{
    //Loading success flag
    bool success = true;

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
    {
        printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        success = false;
    }


    //Initialize SDL_mixer
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
        success = false;
    }


    //Load music
    BackgMusic = Mix_LoadMUS("assets\\Sounds\\BgroundMusic.wav");
    if (BackgMusic == NULL)
    {
        printf("Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError());
        success = false;
    }

    //Load sound effects
    gBomb = Mix_LoadWAV("assets\\Sounds\\Bomb.wav");
    if (gBomb == NULL)
    {
        printf("Failed to load scratch sound effect! SDL_mixer Error: %s\n", Mix_GetError());
        success = false;
    }

    gLife = Mix_LoadWAV("assets\\Sounds\\Life.wav");
    if (gLife == NULL)
    {
        printf("Failed to load scratch sound effect! SDL_mixer Error: %s\n", Mix_GetError());
        success = false;
    }

    gPDKon = Mix_LoadWAV("assets\\Sounds\\PDKon.wav");
    if (gPDKon == NULL)
    {
        printf("Failed to load scratch sound effect! SDL_mixer Error: %s\n", Mix_GetError());
        success = false;
    }

    gTenpoint = Mix_LoadWAV("assets\\Sounds\\Tenpoints.wav");
    if (gTenpoint == NULL)
    {
        printf("Failed to load scratch sound effect! SDL_mixer Error: %s\n", Mix_GetError());
        success = false;
    }



    gCollision = Mix_LoadWAV("assets\\Sounds\\collision.wav");
    if (gCollision == NULL)
    {
        printf("Failed to load scratch sound effect! SDL_mixer Error: %s\n", Mix_GetError());
        success = false;
    }

    gLevelUp = Mix_LoadWAV("assets\\Sounds\\LevelUp.wav");
    if (gLevelUp == NULL)
    {
        printf("Failed to load scratch sound effect! SDL_mixer Error: %s\n", Mix_GetError());
        success = false;
    }

    gDie = Mix_LoadWAV("assets\\Sounds\\die.wav");
    if (gDie == NULL)
    {
        printf("Failed to load scratch sound effect! SDL_mixer Error: %s\n", Mix_GetError());
        success = false;
    }

    gBreak = Mix_LoadWAV("assets\\Sounds\\Explosion.wav");
    if (gBreak == NULL)
    {
        printf("Failed to load high sound effect! SDL_mixer Error: %s\n", Mix_GetError());
        success = false;
    }

    return success;
}

//Used functions
long int str_to_int(string s)
{
    stringstream geek(s);
    int x = 0;
    geek >> x;
    return x;
}

void    SDL_wait_key()
{
    //Wait for a key to be pressed
    SDL_Event* e = new SDL_Event();
    while (e->type != SDL_KEYDOWN)
        SDL_PollEvent(e);
}

void    SDL_wait_space()
{
    //Wait for a key to be pressed
    SDL_Event* e = new SDL_Event();
    while (e->key.keysym.sym != SDLK_SPACE)
    {
        SDL_PollEvent(e);
        if (e->key.keysym.sym == SDLK_q)
        {
            quit = true;
            break;
        }
    }
}

brick** Construct2Darray(int row, int col)
{
    brick** A;
    A = new brick* [row];
    for (int i = 0; i < row; i++)
        A[i] = new brick[col];
    return A;
}


void rect(SDL_Renderer* Renderer, int x, int y, int w, int h, int R, int G, int B, int fill_boolian)
{
    SDL_Rect rectangle;
    rectangle.x = x;
    rectangle.y = y;
    rectangle.w = w;
    rectangle.h = h;


    SDL_SetRenderDrawColor(Renderer, R, G, B, 255);
    if (fill_boolian == 1)
        SDL_RenderFillRect(Renderer, &rectangle);
    SDL_RenderDrawRect(Renderer, &rectangle);
    //SDL_RenderPresent(Renderer);

}

void window_color(SDL_Renderer* Renderer, int R, int G, int B)
{
    SDL_SetRenderDrawColor(Renderer, R, G, B, 255);
    SDL_RenderClear(Renderer);

    // Show the window
    SDL_RenderPresent(Renderer);

}

void text_RGB(SDL_Renderer* m_renderer, string txt, int font_size, int x, int y, int R, int G, int B)
{
    const char* text = txt.c_str();
    TTF_Init();
    SDL_Color text_color = { R, G, B, 255 };
    const char* Font = "assets\\pixelFont.ttf";
    int mWidth, mHeight;
    SDL_Rect* clip = NULL;
    TTF_Font* gFont = NULL;
    double angle = 0.0;
    SDL_Point* center = NULL;
    SDL_RendererFlip flip = SDL_FLIP_NONE;
    SDL_Texture* mTexture = NULL;
    gFont = TTF_OpenFont(Font, font_size);

    SDL_Surface* textSurface = TTF_RenderText_Solid(gFont, text, text_color);
    mWidth = textSurface->w;
    mHeight = textSurface->h;
    mTexture = SDL_CreateTextureFromSurface(m_renderer, textSurface);
    SDL_FreeSurface(textSurface);


    SDL_Rect renderQuad = { x - mWidth / 2 , y - mHeight / 2 , mWidth, mHeight };

    SDL_RenderCopyEx(m_renderer, mTexture, clip, &renderQuad, angle, center, flip);

    SDL_DestroyTexture(mTexture);

    return;
}
