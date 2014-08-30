
#ifndef __BLOCK_H__
#define __BLOCK_H__

#include "project_types.h"

struct block {
    uint_t x;
    uint_t y;
    uint_t w;
    uint_t h;
    string txt;
};

typedef struct block * p_block;

#endif
