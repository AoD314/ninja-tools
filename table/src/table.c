
#include "table.h"
#include <stdlib.h>
#include <stdio.h>

uint_t abssub(uint_t a, uint_t b)
{
    if (a > b)
        return a - b;
    else
        return b - a;
}

p_record get_min_start_value(p_record * list) {
    uint_t min_start = 1 << 31;
    p_record min = NULL;

    p_record list_copy = *list;

    while (list_copy != NULL) {
        if (list_copy->begin < min_start) {
            min_start = list_copy->begin;
            min = list_copy;
        }
        list_copy = list_copy->record_next;
    }

    if (min == NULL)
        return min;

    if (min->record_back != NULL)
        min->record_back->record_next = min->record_next;
    else
        *list = min->record_next;

    if (min->record_next != NULL)
        min->record_next->record_back = min->record_back;

    return min;
}

int table_get_best_pos(p_table * table, p_record rec) {
    int best = -1;
    int index = 0;
    uint_t best_min = 1 << 31;

    p_node curr = (*table)->begin;

    while (curr != NULL) {

        p_record r = curr->record_end;

        if ((rec->begin >= r->end) && (abssub(r->end, rec->begin) <= best_min))
        {
            best = index;
            best_min = abs(r->end - rec->begin);
        }
        curr = curr->node_next;
        index++;
    }

    return best;
}

void table_add_record(p_table * table, p_record rec, int index) {
    if (index < 0) {

        p_node new_node = create_node(rec, rec, NULL, NULL);

        if ((*table)->end == NULL) {

            (*table)->begin = new_node;
            (*table)->end = new_node;

        } else {

            new_node->node_prev = (*table)->end;
            (*table)->end->node_next = new_node;
            (*table)->end = new_node;
        }

    }
    else {
        p_node curr_node = (*table)->begin;
        int i = 0;
        while (i++ < index) {
            curr_node = curr_node->node_next;
        }
        p_record last_rec = curr_node->record_end;

        p_record nrec = create_record(rec->begin, rec->end, rec->filename);

        last_rec->record_next = nrec;
        nrec->record_next = NULL;
        nrec->record_back = last_rec;
        curr_node->record_end = nrec;
        curr_node->amount++;
    }
}

p_table create_table(p_record * list) {
    p_table t = (p_table)malloc(1 * sizeof(struct table));
    t->begin = NULL;
    t->end = NULL;

    p_record min_record = get_min_start_value(list);
    while (min_record != NULL) {
//        printf("min(%p): %llu-%llu -> %s\n", (void *) min_record, min_record->begin, min_record->end, min_record->filename);
        int pos = table_get_best_pos(&t, min_record);
//        printf("find pos: %d\n", pos);
        table_add_record(&t, min_record, pos);
        min_record = get_min_start_value(list);
    }

    return t;
}

