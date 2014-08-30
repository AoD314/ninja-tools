
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

#include "project_types.h"
#include "record.h"
#include "config.h"
#include "table.h"
#include "block.h"
#include "parse.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

char help_msg[] =
        "Usage: ninja-viz path_to_ninja_log_file\n"
        "-h          - Print help and exit\n";

p_config cfg;

static TTF_Font* font = NULL;
static SDL_Color color_white = {255, 255, 255, 255};


void SDL_RenderTextAtPos(SDL_Renderer * render, SDL_Rect pos, string txt) {

    if (pos.x > cfg->screen_w || pos.x + pos.w < 0) return;

    if (font == NULL) {
        font = TTF_OpenFont(cfg->path_to_font, cfg->font_size);
    }
    if(font == NULL)
    {
        printf( "Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
        return;
    }

    SDL_Rect r_tex = {0, 0, 0, 0};
    SDL_Rect r_tex_clip = {0, 0, 0, 0};

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

    uint_t size_of_blocks = (amount + 1) * sizeof(struct block);
    p_block blocks = (p_block) malloc (size_of_blocks);

    p_record r = n->record_begin;

    int thread = 1;
    uint_t index = 0;
    uint_t max_width = 0;

    while (r != NULL)
    {
        for (uint_t i = 0; i < n->amount; i++)
        {
            // use r->begin, r->end;

            blocks[index].x = r->begin;
            blocks[index].w = r->end - r->begin;
            blocks[index].y = 50 * thread;
            blocks[index].h = 30;
            blocks[index].txt = r->filename;

            if (r->end > max_width)
                max_width = r->end;

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

    if( TTF_Init() == -1 )
    {
        printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
    }

    long long int shift_x = 0;
    long long int shift_y = 0;

    float scale = 1.0;

    while(e.type != SDL_QUIT) {

        SDL_PollEvent(&e);
        switch( e.type ){
            case SDL_KEYDOWN:
                switch(e.key.keysym.sym){
                    case SDLK_LEFT:
                        shift_x -= 100;
                        if (shift_x < 0)
                            shift_x = 0;
                        break;
                    case SDLK_RIGHT:
                        shift_x += 100;
                        int max_shift = 1 << 30;
                        if (shift_x > max_shift)
                            shift_x = max_shift;
                        break;
                    case SDLK_HOME:
                        shift_x = 0;
                        break;
                    case SDLK_END:
                        shift_x = (max_width * scale) - cfg->screen_w / 2;
                        break;

                    case SDLK_UP:
                        shift_y += 10;
                        break;
                    case SDLK_DOWN:
                        shift_y -= 10;
                        break;

                    case SDLK_PLUS:
                        scale *= 2;
                        break;

                    case SDLK_MINUS:
                        scale /= 2;
                        break;

                    default:
                        break;
                }
              default:
                break;
        }

        SDL_SetRenderDrawColor(render, 0, 0, 0, 255);
        SDL_RenderClear(render);
        SDL_SetRenderDrawColor(render, 255, 255, 255, 255);

        for (uint_t i = 0; i < amount; i++) {
            SDL_Rect rect;
            rect.x = (blocks[i].x * scale) - shift_x;
            rect.y = (blocks[i].y) + shift_y;
            rect.w = blocks[i].w * scale;
            rect.h = blocks[i].h;

            SDL_RenderTextAtPos(render, rect, blocks[i].txt);
        }

        SDL_RenderPresent(render);
    }

    delete_config(cfg);

    SDL_DestroyRenderer(render);
    SDL_DestroyWindow(window);

    SDL_Quit();

    return 0;
}
