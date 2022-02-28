// Include the most common headers from the C standard library
#include <time.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

// Include the main libnx system header, for Switch development
#include <switch.h>
// #include <curl/curl.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>


#define SCREEN_W 1280
#define SCREEN_H 720

// some switch buttons
#define JOY_A 0
#define JOY_B 1
#define JOY_X 2
#define JOY_Y 3
#define JOY_PLUS 10
#define JOY_MINUS 11
#define JOY_LEFT 12
#define JOY_UP 13
#define JOY_RIGHT 14
#define JOY_DOWN 15

SDL_Texture *render_text(SDL_Renderer *renderer, const char *text, TTF_Font *font, SDL_Color color, SDL_Rect *rect)
{
    SDL_Surface *surface;
    SDL_Texture *texture;

    surface = TTF_RenderUTF8_Solid(font, text, color);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    rect->w = surface->w;
    rect->h = surface->h;

    SDL_FreeSurface(surface);

    return texture;
}

SDL_Texture *render_image(SDL_Renderer *renderer, const char *path, SDL_Rect *rect)
{
    SDL_Surface *surface;
    SDL_Texture *texture;
    surface = IMG_Load(path);
    rect->w = surface->w;
    rect->h = surface->h;
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}

// Main program entrypoint
int main(int argc, char *argv[])
{

    // socketInitializeDefault();

    printf("sdl init\n");
    // curl_global_init(CURL_GLOBAL_DEFAULT);
    romfsInit();
    
    int wait = 25;
    int exit_requested = 0;

    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);
    IMG_Init(IMG_INIT_PNG);
    TTF_Init();

    SDL_InitSubSystem(SDL_INIT_JOYSTICK);
    SDL_JoystickEventState(SDL_ENABLE);
    SDL_JoystickOpen(0);

    SDL_Event event;
    SDL_Color colors[] = {
        {128, 128, 128, 0}, // gray
        {255, 255, 255, 0}, // white
        {255, 0, 0, 0},     // red
        {0, 255, 0, 0},     // green
        {0, 0, 255, 0},     // blue
        {255, 255, 0, 0},   // brown
        {0, 255, 255, 0},   // cyan
        {255, 0, 255, 0},   // purple
    };

    // load font from romfs
    TTF_Font *font = TTF_OpenFont("romfs:/data/simhei.ttf", 36);

    SDL_Window *window = SDL_CreateWindow("music", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_W, SCREEN_H, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);

    SDL_Rect t1_pos = {0, 0, 0, 0};
    SDL_Texture *t1 = render_text(renderer, "1", font, colors[1], &t1_pos);

    SDL_Rect t2_pos = {0, 44, 0, 0};
    SDL_Texture *t2 = render_text(renderer, "2", font, colors[1], &t2_pos);

    SDL_Rect t3_pos = {0, 88, 0, 0};
    SDL_Texture *t3 = render_text(renderer, "中文3", font, colors[1], &t3_pos);

    SDL_Rect t4_pos = {0, 132, 0, 0};
    SDL_Texture *t4 = render_text(renderer, "4", font, colors[1], &t4_pos);

    SDL_Rect i1_pos = {0, 176, 0, 0};
    SDL_Texture *i1 = render_image(renderer, "romfs:/data/sdl.png", &i1_pos);
    
    // Main loop
    while (!exit_requested && appletMainLoop())
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
                exit_requested = 1;

            // use joystick
            if (event.type == SDL_JOYBUTTONDOWN)
            {
                if (event.jbutton.button == JOY_PLUS)
                    exit_requested = 1;
            }
        }

        SDL_RenderClear(renderer);

        
        SDL_RenderCopy(renderer, t1, NULL, &t1_pos);

       
        SDL_RenderCopy(renderer, t2, NULL, &t2_pos);

     
        SDL_RenderCopy(renderer, t3, NULL, &t3_pos);


        SDL_RenderCopy(renderer, t4, NULL, &t4_pos);


        SDL_RenderCopy(renderer, i1, NULL, &i1_pos);

        SDL_RenderPresent(renderer);

        SDL_Delay(wait);
    }
    TTF_CloseFont(font);
    // clean up your textures when you are done with them
 

    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);

    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
    romfsExit();
    // curl_global_cleanup();
    // socketExit();
    return 0;
}