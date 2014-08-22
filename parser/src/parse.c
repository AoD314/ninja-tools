
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
        point->record_next = r;
        r->record_back = point;
        point = r;
    }

    fclose(f);

    p_record result = begin->record_next;
    result->record_back = NULL;

    free(begin);
    free(buffer);

    return result;
}


void time_to_str(uint_t time_in_ms, string str) {
    uint_t ms = time_in_ms;
    uint_t h = 0;
    uint_t m = 0;
    uint_t s = 0;

    h = ms / (3600 * 1000);
    ms -= (h * (3600 * 1000));
    m = ms / (60 * 1000);
    ms -= (m * (60 * 1000));
    s = ms / (1 * 1000);
    ms -= (s * (1 * 1000));

    sprintf (str, "%02llu:%02llu:%02llu.%03llu (%12llu ms)", h, m, s, ms, time_in_ms);
}
