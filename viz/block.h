
#ifndef __BLOCK_H__
#define __BLOCK_H__

#include "project_types.h"

struct block {
    int x;
    int y;
    int w;
    int h;
    string txt;
};

typedef struct block * p_block;

#endif
