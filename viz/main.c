
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

#include "project_types.h"
#include "record.h"
#include "config.h"
#include "table.h"
#include "block.h"

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

    p_table t = create_table(&list);

    p_node n = t->begin;

    uint_t amount = 0;
    while (n != NULL) {
        amount += n->amount;
        n = n->node_next;
    }
    n = t->begin;

    printf("amount = %d\n", amount);

    p_block * blocks = (p_block) malloc (amount * sizeof(struct block));

    p_record r = n->record_begin;

    int thread = 0;
    uint_t index = 0;

    while (r != NULL)
    {
        while (r != NULL)
        {
            // use r->begin, r->end;

            blocks[index]->x = r->begin;
            blocks[index]->w = r->end - r->begin;
            blocks[index]->y = 35 * thread;
            blocks[index]->h = 30;
            blocks[index]->txt = r->filename;

            r = r->record_next;
            index++;
        }
        n = n->node_next;
        if (n == NULL) {
            r = NULL;
        } else {
            r = n->record_begin;
        }
        thread ++;
    }


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

    SDL_Rect rect;
    rect.x = 10;
    rect.y = 400;
    rect.w = 1500;
    rect.h = 20;

    SDL_Rect r2 = rect;
    r2.y = 500;

    if( TTF_Init() == -1 )
    {
        printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
    }

    char txt2[] = "/HOME/SSD/CMakeFiles/qwertyuiopasdfghjklzxcvbnm/?!@#$%^&*+_=26354183134112351513242=_example_command_line_parser{_}(0123456789).cpp.o";
//    char txt2[] = "/HOME/SSD/CMake";

    size_t ls = strlen(txt2);

    while(e.type != SDL_QUIT) {

        SDL_PollEvent(&e);


        SDL_SetRenderDrawColor(render, 0, 0, 0, 255);
        SDL_RenderClear(render);
        SDL_SetRenderDrawColor(render, 255, 255, 255, 255);

        for (int i = 0; i < 25; i++) {
            rect.y = i * 35;

            for (int j = 0; j < 64; j++) {
                int rnd = rand() % ls;
                int let = rand() % 10;
                txt2[rnd] = let + '0';
            }

            SDL_RenderTextAtPos(render, rect, txt2);
        }



        SDL_RenderPresent(render);

    }

    delete_config(cfg);

    SDL_DestroyRenderer(render);
    SDL_DestroyWindow(window);

    SDL_Quit();

    return 0;
}
