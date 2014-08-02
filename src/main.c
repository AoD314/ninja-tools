
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#define max(x, y) ((x) > (y)) ? (x) : (y)

#include "record.h"
#include "parse.h"
#include "svg.h"

//struct node {
//  struct node *next;
//  struct node *last;

//  struct record *begin;
//  struct record *end;
//};

//struct table {
//  struct node *begin;
//  struct node *end;
//};

//struct record * get_min_start(struct record * list) {
//    struct record * list_c = list;
//    uint32_t min_start = 1 << 31;
//    struct record * min = NULL;
//    struct record * back = NULL;

//    while (list_c != NULL) {
//        if (list_c->start < min_start) {
//            min_start = list_c->start;
//            min = list_c;
//        }
//        else
//            back = list_c;

//        list_c = list_c->next;
//    }

//    return min;
//}

//void table_add_record(struct table *table, struct record *rec, int index) {
//  if (index < 0) {
//    struct node *nn = (struct node *)malloc(1 * sizeof(struct node));
//    struct node *last = table->end;
//    last->next = nn;
//    last->last = NULL;
//    last->begin = rec;
//    last->end = rec;
//  } else {
//    struct node *curr = table->begin;
//    int i = 0;
//    while (i++ < index) {
//      curr = curr->next;
//    }
//    struct record *curr_rec = curr->end;
//    curr_rec->next = rec;
//    curr_rec->last = NULL;
//    rec->next = NULL;
//    rec->last = NULL;
//  }
//}

//int table_get_best_pos(struct table * table, struct record * rec)
//{
//    int best = -1;
//    int index = 0;
//    int best_min = 1 << 30;
//    struct node * curr = table->begin;
//    while (curr->next != NULL) {
//        struct record * r = curr->end;
//        if ((rec->start > r->end) && (abs(r->end - rec->start) < best_min))
//        {
//            best = index;
//            best_min = abs(r->end - rec->start);
//        }
//        curr = curr->next;
//        index++;
//    }
//    return best;
//}

//void get_max_rect(const struct record **recs, const uint32_t N, uint32_t *W,
//                  uint32_t *H) {
//  *W = 0;
//  *H = 25 * 314;
//  for (uint32_t i = 0; i < N; i++) {
//    *W = max(*W, recs[i]->end);
//  }
//}

int main() {
    char path_to_ninja_log[] = "/tmp/.ninja_log";
    char path_to_svg_file[] = "/tmp/out.svg";

    p_svg svg_file = svg_open(path_to_svg_file);
    if (svg_file == NULL) {
        printf("error: can't create svg file: [%s]\n", path_to_svg_file);
        fflush(NULL);
        return -1;
    }

    p_record list = parse_ninja_log(path_to_ninja_log);

    uint_t max_len = 0;
    string buff = (string)malloc(65536);
    p_record iter = list;
    while (iter != NULL) {
        if ((iter->end - iter->start) > max_len)
        {
            max_len = iter->end - iter->start;
            strcpy(buff, iter->filename);
        }
        iter = iter->next;
    }

    printf("%s : %llu ms\n", buff, max_len);
    free(buff);

    svg_add_rect(svg_file, create_rect(0, 0, 200, 50));
    svg_add_text(svg_file, create_text(25, 25, "Hello, World !!!!!"));

    svg_close(svg_file);

    return 0;
}
