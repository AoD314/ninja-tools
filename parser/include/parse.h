
#ifndef __PARSE_H__
#define __PARSE_H__

#include "project_types.h"
#include "record.h"

p_record parse_ninja_log(const string path_to_ninja_log);

void time_to_str(uint_t time_in_ms, string str);

#endif
