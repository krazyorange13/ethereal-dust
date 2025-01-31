#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>

typedef uint8_t ETHER_bool;
#define ETHER_FALSE 0
#define ETHER_TRUE 1

typedef struct ETHER_vec_u16 ETHER_vec_u16;
typedef struct ETHER_rect_u16 ETHER_rect_u16;

struct ETHER_vec_u16
{
    uint16_t x, y;
};

struct ETHER_rect_u16
{
    uint16_t x, y, w, h;
};

#endif