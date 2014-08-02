
#ifndef __TABLE_H__
#define __TABLE_H__

#include "project_types.h"
#include "node.h"

struct table {
    p_node begin;
    p_node end;
};


typedef struct table * p_table;

p_table create_table(p_record *list);

#endif
