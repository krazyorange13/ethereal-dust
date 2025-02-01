#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>

#define ETHER_SCREEN_WIDTH 640
#define ETHER_SCREEN_HEIGHT 480
#define ETHER_WORLD_WIDTH 200//1280
#define ETHER_WORLD_HEIGHT 200//960

typedef uint8_t ETHER_bool;
#define ETHER_FALSE 0
#define ETHER_TRUE 1

#define CEIL_POS(X) ((X-(int)(X)) > 0 ? (int)(X+1) : (int)(X))
#define CEIL_NEG(X) (int)(X)
#define CEIL(X) (((X) > 0) ? CEIL_POS(X) : CEIL_NEG(X))
#define ROUND(X) (((X) < (int)(X) + 0.5) ? (int)(X) : (int)(X) + 1)
#define FLOOR(X) ((int)(X))
#define NUFFIN(X) (X)
#define ABS(X) (((X) < 0) ? -(X) : (X))
#define SIGN(X) (((X) < 0) ? -1 : 1)
#define SIGN2(X) (((X) == 0) ? 0 : (((X) < 0) ? -1 : 1))
#define CLAMP(X, MIN, MAX) ((((X) < (MIN)) ? (MIN) : (X)) > (MAX) ? (MAX) : (X))
#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))
#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))
#define SQUARE(X) ((X) * (X))

typedef struct ETHER_vec_u16 ETHER_vec_u16;
typedef struct ETHER_vec_s16 ETHER_vec_s16;
typedef struct ETHER_rect_u16 ETHER_rect_u16;
typedef struct ETHER_rect_s16 ETHER_rect_s16;

struct ETHER_vec_u16
{
    uint16_t x, y;
};

struct ETHER_vec_s16
{
    int16_t x, y;
};

struct ETHER_rect_u16
{
    uint16_t x, y, w, h;
};

struct ETHER_rect_s16
{
    int16_t x, y, w, h;
};

static inline ETHER_bool ETHER_vec_in_rect_u16(ETHER_vec_u16 pos, ETHER_rect_u16 rect)
{
    return pos.x >= rect.x && pos.x <= rect.x + rect.w
        && pos.y >= rect.y && pos.y <= rect.y + rect.h;
}

static inline ETHER_bool ETHER_rects_collide_u16(ETHER_rect_u16 a, ETHER_rect_u16 b)
{
    return a.x <= b.x + b.w
        && b.x <= a.x + a.w
        && a.y <= b.y + b.h
        && b.y <= a.y + a.h;
}

static inline ETHER_rect_s16 ETHER_rects_minkowski_difference(ETHER_rect_u16 a, ETHER_rect_u16 b)
{
    return (ETHER_rect_s16)
    {
        a.x - (b.x + b.w),
        a.y - (b.y + b.h),
        a.w + b.w,
        a.h + b.h
        // a.x - (b.x + b.w) + ((a.w + b.w) / 2),
        // a.y - (b.y + b.h) + ((a.h + b.h) / 2),
        // ((a.w + b.w) / 2),
        // ((a.h + b.h) / 2),
    };
}

static inline ETHER_vec_s16 ETHER_rect_closest_point_on_bounds_to_point(ETHER_rect_s16 rect, ETHER_vec_s16 vec)
{
    uint16_t min = ABS(vec.x - rect.x);
    ETHER_vec_s16 bounds_point = {rect.x, vec.y};
    if (ABS(rect.x + rect.w - vec.x) < min)
    {
        min = ABS(rect.x + rect.w - vec.x);
        bounds_point = (ETHER_vec_s16) {rect.x + rect.w, vec.y};
    }
    if (ABS(rect.y + rect.h - vec.y) < min)
    {
        min = ABS(rect.y + rect.h - vec.y);
        bounds_point = (ETHER_vec_s16) {vec.x, rect.y + rect.h};
    }
    if (ABS(rect.y - vec.y) < min)
    {
        min = ABS(rect.y - vec.y);
        bounds_point = (ETHER_vec_s16) {vec.x, rect.y};
    }
    return bounds_point;
}

static inline ETHER_vec_s16 ETHER_rects_penetration_vector(ETHER_rect_u16 a, ETHER_rect_u16 b)
{
    ETHER_rect_s16 md = ETHER_rects_minkowski_difference(a, b);
    ETHER_vec_s16 pen = ETHER_rect_closest_point_on_bounds_to_point(md, (ETHER_vec_s16) {0, 0});
    return pen;
}

/* taken from https://blog.hamaluik.ca/posts/swept-aabb-collision-using-minkowski-difference/
// taken from https://github.com/pgkelley4/line-segments-intersect/blob/master/js/line-segments-intersect.js
// which was adapted from http://stackoverflow.com/questions/563198/how-do-you-detect-where-two-line-segments-intersect
// returns the point where they intersect (if they intersect)
// returns Math.POSITIVE_INFINITY if they don't intersect
private function getRayIntersectionFractionOfFirstRay(originA:Vector, endA:Vector, originB:Vector, endB:Vector):Float
{
    var r:Vector = endA - originA;
    var s:Vector = endB - originB;

    var numerator:Float = (originB - originA) * r;
    var denominator:Float = r * s;

    if (numerator == 0 && denominator == 0)
    {
        // the lines are co-linear
        // check if they overlap
        // todo: calculate intersection point
        return Math.POSITIVE_INFINITY;
    }
    if (denominator == 0)
    {
        // lines are parallel
        return Math.POSITIVE_INFINITY;
    }

    var u:Float = numerator / denominator;
    var t:Float = ((originB - originA) * s) / denominator;
    if ((t >= 0) && (t <= 1) && (u >= 0) && (u <= 1))
    {
        return t;
    }
    return Math.POSITIVE_INFINITY;
}

public function getRayIntersectionFraction(origin:Vector, direction:Vector):Float
{
    var end:Vector = origin + direction;

    // for each of the AABB's four edges
    // calculate the minimum fraction of "direction"
    // in order to find where the ray FIRST intersects
    // the AABB (if it ever does)
    var minT:Float = getRayIntersectionFractionOfFirstRay(origin, end, new Vector(min.x, min.y), new Vector(min.x, max.y));
    var x:Float;
    x = getRayIntersectionFractionOfFirstRay(origin, end, new Vector(min.x, max.y), new Vector(max.x, max.y));
    if (x < minT)
        minT = x;
    x = getRayIntersectionFractionOfFirstRay(origin, end, new Vector(max.x, max.y), new Vector(max.x, min.y));
    if (x < minT)
        minT = x;
    x = getRayIntersectionFractionOfFirstRay(origin, end, new Vector(max.x, min.y), new Vector(min.x, min.y));
    if (x < minT)
        minT = x;

    // ok, now we should have found the fractional component along the ray where we collided
    return minT;
}
*/

#endif