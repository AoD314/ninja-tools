#include "node.h"
#include <stdlib.h>

p_node create_node(p_record b, p_record e, p_node nxt, p_node prv) {
    p_node n = (p_node) malloc( 1 * sizeof(struct node));
    n->amount = 1;
    n->record_begin = b;
    n->record_end   = e;
    n->node_next = nxt;
    n->node_prev = prv;
    return n;
}
