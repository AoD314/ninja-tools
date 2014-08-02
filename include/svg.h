
#ifndef __SVG_H__
#define __SVG_H__

#include <stdlib.h>
#include <stdio.h>

#include "project_types.h"

struct rect {
    uint_t x, y, w, h;
};
struct text {
    uint_t x, y;
    char * t;
};
typedef struct rect * p_rect;
typedef struct text * p_text;

p_rect create_rect(uint_t x, uint_t y, uint_t width, uint_t height);
p_text create_text(uint_t x, uint_t y, const string txt);




struct svg {
    FILE * f;
};
typedef struct svg * p_svg;

p_svg svg_open(const string name);
void  svg_close(p_svg s);

void svg_add_rect(p_svg s, p_rect r);
void svg_add_text(p_svg s, p_text t);

#endif
