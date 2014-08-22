
#include <gtest/gtest.h>

extern "C" {
    #include "parse.h"
}

TEST(Parser, time)
{
    uint_t t = 123456;
    char time[128];
    time_to_str(t, time);
    EXPECT_STREQ(time, "00:02:03.456 (      123456 ms)");
}

