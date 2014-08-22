
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

#include "parse.h"

p_record get_max_compile_time(p_record first) {
    p_record max_compile_time = first;
    p_record it = first;

    while (it != NULL) {
        if ((it->end - it->begin) > (max_compile_time->end - max_compile_time->begin))
        {
            max_compile_time = it;
        }
        it = it->record_next;
    }

    return max_compile_time;
}

char help_msg[] =
        "Usage: ninja-stat [OPTIONS] path_to_ninja_log_file\n"
        "-h          - Print help and exit\n"
        "-n <int>    - Print file name and compile time sorted by compile time(by default: n = 32)\n";

int main(int argc, char ** argv) {

    if (argc == 1) {
        printf("%s", help_msg);
        return 0;
    }

    uint_t N = 32;
    string path_to_ninja_log = argv[argc - 1];

    int opt = 0;
    while ((opt = getopt(argc, argv, "hn:")) != -1) {
        switch (opt) {
        case 'h':
            printf("%s", help_msg);
            return 0;
        case 'n':
            N = atoi(optarg);
            break;
        default:
            break;
        }
    }

    p_record list = parse_ninja_log(path_to_ninja_log);
    p_record max_compile_time = list;

    uint_t total_time = 0;
    uint_t sum_time = 0;
    uint_t count = 0;
    p_record it = list;
    while (it != NULL) {
        if (it->end > total_time) {
            total_time = it->end;
        }
        sum_time += (it->end - it->begin);
        count++;
        it = it->record_next;
    }

    if (count < N) {
        N = count;
    }

    printf("\n");

    for (uint_t i = 0; i < N; i++) {
        max_compile_time = get_max_compile_time(list);

        printf("%12lld ms | %s\n", max_compile_time->end - max_compile_time->begin, max_compile_time->filename);

        if (max_compile_time->record_back != NULL && max_compile_time->record_next != NULL) {
                    p_record n =  max_compile_time->record_next;
                    p_record b =  max_compile_time->record_back;
                    b->record_next = n;
                    n->record_back = b;
        }
        else if (max_compile_time->record_back == NULL && max_compile_time->record_next != NULL) {
            list = list->record_next;
            list->record_back = NULL;
        }
        else if (max_compile_time->record_next == NULL && max_compile_time->record_back != NULL) {
            max_compile_time->record_back->record_next = NULL;
        }

        delete_record(max_compile_time);
    }

    char time[256];
    time_to_str(sum_time, time);
    printf("\nsum time (1-thread) : %s", time);
    time_to_str(total_time, time);
    printf("\ncompile time        : %s", time);
    printf("\nspeed ratio         : %.3f", (double)sum_time / (double)total_time);

    printf("\nfiles       : %lld", count);
    printf("\nfile per sec: %.2f\n\n", (double)count / ((double)total_time / 1000.0));

    return 0;
}
