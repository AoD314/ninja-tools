
#ifndef __RECORD_H__
#define __RECORD_H__

#include "project_types.h"

struct record {
    uint_t begin;
    uint_t end;
    string filename;
    struct record * record_next;
    struct record * record_back;
};

typedef struct record * p_record;

p_record create_record(uint_t s, uint_t e, const string name);
void     delete_record(p_record r);

#endif
