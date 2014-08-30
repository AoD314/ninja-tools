
#include "config.h"

static const char PATH_TO_FONT[] = "/usr/share/fonts/liberation-fonts/LiberationMono-Regular.ttf";

p_config create_config()
{
    p_config cfg = (p_config) malloc(1 * sizeof(struct config));

    cfg->scale_x = 1;
    cfg->scale_y = 1;
    cfg->font_size = 14;
    cfg->path_to_font = PATH_TO_FONT;
    cfg->screen_w = 1600;
    cfg->screen_h = 900;

    return cfg;
}


void delete_config(p_config cfg)
{
    free(cfg);
}
