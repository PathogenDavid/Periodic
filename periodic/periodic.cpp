#include "periodic.h"

int strlen(const char* str)
{
    int ret = 0;
    while (str[ret] != '\0')
    {
        ret++;
    }
    return ret;
}

int strcmp(const char* a, const char* b)
{
    while (true)
    {
        if (*a < *b)
        {
            return -1;
        }
        else if (*a > *b)
        {
            return 1;
        }
        else if (*a == '\0')// *b == '\0' is implied
        {
            return 0;
        }

        a++;
        b++;
    }
}

int abs(int x)
{
    return x < 0 ? -x : x;
}
