#pragma once

#include "stdint.h"

extern "C"
{
    void memcpy(void *dest, const void *src, uint64_t len);
    
    void memmove(void *dest, const void *src, uint64_t len);

    void memset(void *dest, uint8_t val, uint64_t len);

    void bzero(void *dest, uint64_t len);

    int strcmp(const char *s1, const char *s2);

    char *strcpy(char *dest, const char *src);

    char *strcat(char *dest, const char *src);

    int strlen(const char *src);
}
