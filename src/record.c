
#include "record.h"
#include <stdlib.h>
#include <string.h>

p_record create_record(uint_t s, uint_t e, const string name) {

    p_record rec = (p_record) malloc( 1 * sizeof(struct record));
    rec->record_back = NULL;
    rec->record_next = NULL;

    rec->begin = s;
    rec->end = e;

    size_t len = strlen(name);
    rec->filename = (string) malloc( len + 1 );
    strcpy(rec->filename, name);

    return rec;
}


void delete_record(p_record r) {

    free(r->filename);
    free(r);
}
