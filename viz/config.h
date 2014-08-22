
#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <stdlib.h>
#include "project_types.h"

struct config {
    int screen_w;
    int screen_h;
    int scale_x;
    int scale_y;
    int font_size;
    string path_to_font;
};

typedef struct config * p_config;

p_config create_config();
void     delete_config(p_config cfg);

#endif
