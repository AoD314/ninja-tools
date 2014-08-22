
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "record.h"
#include "parse.h"
#include "svg.h"
#include "table.h"

void copy_short_name(string name, string short_name, uint_t time_in_ms)
{
    // 1082/900 or 362/300
    size_t max_len = (size_t)(((float)time_in_ms / 1082.0f) * 900.0f) - 1;
    size_t len = strlen(name);
    max_len = (max_len < len)? max_len : len;
    int i = 0;
    for (; i < max_len; i++)
    {
        short_name[i] = name[len - max_len + i];
    }
    if (max_len < len)
    {
        short_name[0] = '.';
        short_name[1] = '.';
    }
    short_name[i] = '\0';
}

int main() {
    char path_to_ninja_log[] = "/tmp/.ninja_log";
    char path_to_svg_file [] = "/tmp/output.svg";

    p_record list = parse_ninja_log(path_to_ninja_log);

    uint_t max_len = 0;
    uint_t max_end = 0;
//    string buff = (string)malloc(65536);
    p_record iter = list;
    while (iter != NULL) {
        if ((iter->end - iter->begin) > max_len) {
            max_len = iter->end - iter->begin;
//            strcpy(buff, iter->filename);
        }
        if (max_end < iter->end) {
            max_end = iter->end;
        }
        iter = iter->record_next;
    }
//    printf("%s : %llu ms\n", buff, max_len);
//    free(buff);

    const int height = 5;

    p_svg svg_file = svg_open(path_to_svg_file, max_end + 10, 24 * height);
    if (svg_file == NULL) {
        printf("error: can't create svg file: [%s]\n", path_to_svg_file);
        fflush(NULL);
        return -1;
    }

    p_table t = create_table(&list);

    p_node n = t->begin;
    p_record r = n->record_begin;
    int thread = 0;

    string short_name = (string)malloc(1024);
    for (int i = 0; i < 1024; i++) short_name[i] = 0;

    while (r != NULL)
    {
        while (r != NULL)
        {
//            printf("start:%5llu    end:%5llu    len:%5llu    file:%s\n", r->begin, r->end, r->end - r->begin, r->filename);

            copy_short_name(r->filename, short_name, r->end - r->begin);
            svg_add_rect(svg_file, create_rect(r->begin, (thread) * height , r->end - r->begin, height));
            svg_add_text(svg_file, create_text(r->begin + 1, (thread + 1) * height - height/2, short_name));

            r = r->record_next;
        }
        n = n->node_next;
        if (n == NULL) {
            r = NULL;
        } else {
            r = n->record_begin;
        }
        thread ++;
//        printf("\n");
    }

    // axis

    for (int i = 0; i < 150000; i += 250) {
        sprintf(short_name, "%d", i);
        svg_add_line(svg_file, create_line(i,     (thread + 1) * height, i, 0));
        svg_add_text(svg_file, create_text(i + 1, (thread + 1) * height - height/2, short_name));
    }

    svg_close(svg_file);

    return 0;
}
