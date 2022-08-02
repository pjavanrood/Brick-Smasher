#include "Declaration.h"

int main(int argc, char* argv[])
{
    int W = 1200;
    int L = 800;

    Uint32 SDL_flags = SDL_INIT_VIDEO | SDL_INIT_TIMER;
    Uint32 WND_flags = SDL_WINDOW_SHOWN;//| SDL_WINDOW_FULLSCREEN_DESKTOP;//SDL_WINDOW_BORDERLESS ;
    SDL_Window* m_window;
    SDL_Renderer* m_renderer;
    SDL_Init(SDL_flags);
    SDL_CreateWindowAndRenderer(W, L, WND_flags, &m_window, &m_renderer);
    
    //Pass the focus to the drawing window
    SDL_RaiseWindow(m_window);
    //Get screen resolution
    SDL_DisplayMode DM;
    int xc = W / 2, yc = L / 2;
    SDL_GetCurrentDisplayMode(0, &DM);
    SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
    Background_tex = IMG_LoadTexture(m_renderer, "assets\\Background.PNG");
    MenuBackground_tex = IMG_LoadTexture(m_renderer, "assets\\MenuBackground.PNG");
    Uint32 time = SDL_GetTicks();
    int delta, fps = 1000 / 60;
    //

    
    

    brick **Bricks;
    Bricks = Construct2Darray(18, 8);
    for (int i = 0; i < 18; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            Bricks[i][j].color = 0;
            Bricks[i][j].damage = 0;
            Bricks[i][j].initial_damage = 0;
        }
    }
    bool load_media = loadMedia();

    Mix_PlayMusic(BackgMusic, -1);
    
    Initial_Game(m_renderer);

    
    if (found)
        load = load_menu(m_renderer);
    //Initialize_Background(m_renderer);
    SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
    SDL_RenderClear(m_renderer);
    SDL_RenderCopy(m_renderer, Background_tex, NULL, NULL);
    Initialize_paddle(m_renderer);
    Initialize_bricks(m_renderer, Bricks);
    text_RGB(m_renderer, "Score: " + to_string(scores), 20, 60, 780, 0, 0, 0);
    show_lives(m_renderer);
    
    Initialize_ball(m_renderer);
    

    quit = false;
    SDL_Event event;
    while (!quit)
    {
        if (Mix_PlayingMusic() == 0)
        {
            //Play the music
            Mix_PlayMusic(BackgMusic, -1);
        }

        
        SDL_RenderCopy(m_renderer, Background_tex, NULL, NULL);
        if (ball.dead)
        {
            
            text_RGB(m_renderer, "Score: " + to_string(scores), 20, 60, 780, 0, 0, 0);
            show_lives(m_renderer);
            update_bricks(m_renderer, Bricks);
            Initialize_paddle(m_renderer);
            Initialize_ball(m_renderer);
        }
        time = SDL_GetTicks();
        SDL_PollEvent(&event);
        handle_events(m_renderer, event);
        
        update_paddle(m_renderer);
        update_ball(m_renderer, Bricks);
        update_bricks(m_renderer, Bricks);
        text_RGB(m_renderer, "Score: " + to_string(scores), 20, 60, 780, 0, 0, 0);
        
        show_lives(m_renderer);
        
        SDL_RenderPresent(m_renderer);
        
        //limit refresh rate
        delta = SDL_GetTicks() - time;
        if (delta > fps && delta < 1500)
            SDL_Delay(delta - fps);
      
        SDL_RenderClear(m_renderer);
    }

    Quit_Menu(m_renderer, Bricks);
    SDL_RenderPresent(m_renderer);

    //Quit the program
    SDL_DestroyWindow(m_window);
    SDL_DestroyRenderer(m_renderer);
    IMG_Quit();
    SDL_Quit();
    return 0;
}
