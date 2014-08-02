
#include "parse.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

p_record parse_ninja_log(const string path_to_ninja_log) {

    string buffer = (string)malloc(65536);

    p_record begin = create_record(0, 0, "empty");

    p_record point = begin;

    FILE * f = fopen(path_to_ninja_log, "r");
    fscanf(f, "#%*s %*s %*s");

    for (;;) {
        uint_t start, end;

        int err = fscanf(f, "%llu\t%llu\t%*llu\t%s %*s\n", &start, &end, buffer);
        if (err == EOF) {
            break;
        }

        p_record r = create_record(start, end, buffer);
        point->next = r;
        point = r;
    }

    fclose(f);

    p_record result = begin->next;

    free(begin);
    free(buffer);

    return result;
}
