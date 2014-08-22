
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

#include "project_types.h"
#include "record.h"
#include "config.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

char help_msg[] =
        "Usage: ninja-viz [OPTIONS] path_to_ninja_log_file\n"
        "-h          - Print help and exit\n";

p_config cfg;

static TTF_Font* font = NULL;
static SDL_Color color_white = {255, 255, 255};


void SDL_RenderTextAtPos(SDL_Renderer * render, SDL_Rect pos, string txt) {

    if (font == NULL) {
        font = TTF_OpenFont(cfg->path_to_font, cfg->font_size);
    }
    if(font == NULL)
    {
        printf( "Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
        return;
    }

    SDL_Rect r_tex, r_tex_clip;

    SDL_Surface* surfaceMessage = TTF_RenderText_Blended(font, txt, color_white);

    SDL_Texture* tex = SDL_CreateTextureFromSurface(render, surfaceMessage);

    TTF_SizeUTF8(font, txt, &r_tex.w, &r_tex.h);

    r_tex_clip.x = pos.x;
    r_tex_clip.y = pos.y + (pos.h / 2 - r_tex.h / 2);
    r_tex_clip.h = r_tex.h;

    if (r_tex.w - pos.w > 0)
    {
        r_tex.x = r_tex.w - pos.w;
        r_tex_clip.w = pos.w;
    }
    else
    {
        r_tex.x = 0;
        r_tex_clip.w = r_tex.w;
    }

    SDL_RenderCopy(render, tex, &r_tex, &r_tex_clip);
    SDL_RenderDrawRect(render, &pos);

    SDL_DestroyTexture(tex);
    SDL_FreeSurface(surfaceMessage);
}

int main(int argc, char ** argv) {

    cfg = create_config();

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0){
        printf("SDL_Init Error: %s", SDL_GetError());
        return 1;
    }

    if (argc == 1) {
        printf("%s", help_msg);
        return 0;
    }

    string path_to_ninja_log = argv[argc - 1];

    int opt = 0;
    while ((opt = getopt(argc, argv, "h")) != -1) {
        switch (opt) {
        case 'h':
            printf("%s", help_msg);
            return 0;
        default:
            break;
        }
    }

    p_record list = parse_ninja_log(path_to_ninja_log);
    (void *)list;

    SDL_Window * window = NULL;
    window = SDL_CreateWindow("Hello World!", 0, 0, cfg->screen_w, cfg->screen_h, SDL_WINDOW_OPENGL);
    if (window == NULL){
        printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Renderer * render = NULL;
    render = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (render == NULL){
        printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Event e;

    SDL_Rect r;
    r.x = 10;
    r.y = 400;
    r.w = 1500;
    r.h = 20;

    SDL_Rect r2 = r;
    r2.y = 500;

    if( TTF_Init() == -1 )
    {
        printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
    }

    char txt[] = "/HOME/SSD/CMakeFiles/qwertyuiopasdfghjklzxcvbnm/?!@#$%^&*+_=26354183134112351513242=_example_command_line_parser{_}(0123456789).cpp.o";
    char txt2[] = "/HOME/SSD/CMake";

    size_t ls = strlen(txt2);

    while(e.type != SDL_QUIT) {

        SDL_PollEvent(&e);


        SDL_SetRenderDrawColor(render, 0, 0, 0, 255);
        SDL_RenderClear(render);
        SDL_SetRenderDrawColor(render, 255, 255, 255, 255);

        for (int i = 0; i < 25; i++) {
            r.y = i * 35;

            for (int j = 0; j < 64; j++) {
                int rnd = rand() % ls;
                int let = rand() % 10;
                txt2[rnd] = let + '0';
            }

            SDL_RenderTextAtPos(render, r, txt2);
        }



        SDL_RenderPresent(render);

    }

    delete_config(cfg);

    SDL_DestroyRenderer(render);
    SDL_DestroyWindow(window);

    SDL_Quit();

    return 0;
}
