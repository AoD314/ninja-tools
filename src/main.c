
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "record.h"
#include "parse.h"
#include "svg.h"
#include "table.h"

int main() {
//    sleep(1);
    char path_to_ninja_log[] = "/tmp/.ninja_log";
    char path_to_svg_file [] = "/tmp/output.svg";

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
        if ((iter->end - iter->begin) > max_len)
        {
            max_len = iter->end - iter->begin;
            strcpy(buff, iter->filename);
        }
        iter = iter->record_next;
    }
    printf("%s : %llu ms\n", buff, max_len);
    free(buff);

    p_table t = create_table(&list);

    p_node n = t->begin;
    p_record r = n->record_begin;
    int thread = 0;

    while (r != NULL)
    {
        while (r != NULL)
        {
            svg_add_rect(svg_file, create_rect(r->begin, (thread) * 40 , r->end - r->begin, 40));
            svg_add_text(svg_file, create_text(r->begin, (thread + 1) * 40 - 20, r->filename));
//            printf("start:%5llu    end:%5llu    len:%5llu    file:%s\n", r->begin, r->end, r->end - r->begin, r->filename);
            r = r->record_next;
        }
        n = n->node_next;
        if (n == NULL) {
            r = NULL;
        } else {
            r = n->record_begin;
        }
        thread ++;
    }

    svg_close(svg_file);

    return 0;
}
