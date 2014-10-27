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
