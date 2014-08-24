
#ifndef __NODE_H__
#define __NODE_H__

#include "project_types.h"
#include "record.h"

struct node;

typedef struct node * p_node;

struct node {
    uint_t amount;
    p_node node_next;
    p_node node_prev;

    p_record record_begin;
    p_record record_end;
};

p_node create_node(p_record b, p_record e, p_node nxt, p_node prv);

#endif
