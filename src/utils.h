#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>
#include <math.h>

#define ETHER_SCREEN_WIDTH 640
#define ETHER_SCREEN_HEIGHT 480
#define ETHER_WORLD_WIDTH 1280
#define ETHER_WORLD_HEIGHT 960

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
#define WITHIN(X, MIN, MAX) (X > MIN && X < MAX)
#define WITHIN_INCLUSIVE(X, MIN, MAX) ((X) >= (MIN) && (X) <= (MAX))

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

/* i guess i will finally give in, and use normal types */
/* for the convenience and (small) performance lol */

typedef struct ETHER_rect ETHER_rect;
typedef struct ETHER_vec ETHER_vec;
typedef struct ETHER_line ETHER_line;

struct ETHER_rect
{
    int x, y, w, h;
};

struct ETHER_vec
{
    int x, y;
};

struct ETHER_line
{
    int x1, y1, x2, y2;
};

/* aw shucks */

// static inline ETHER_bool ETHER_vec_in_rect_u16(ETHER_vec_u16 pos, ETHER_rect_u16 rect)
// {
//     return pos.x > rect.x && pos.x < rect.x + rect.w
//         && pos.y > rect.y && pos.y < rect.y + rect.h;
// }

static inline ETHER_bool ETHER_vec_in_rect(ETHER_vec pos, ETHER_rect rect)
{
    return pos.x > rect.x && pos.x < rect.x + rect.w
        && pos.y > rect.y && pos.y < rect.y + rect.h;
}

// static inline ETHER_bool ETHER_rects_collide_u16(ETHER_rect_u16 a, ETHER_rect_u16 b)
// {
//     return a.x < b.x + b.w
//         && b.x < a.x + a.w
//         && a.y < b.y + b.h
//         && b.y < a.y + a.h;
// }

// static inline ETHER_bool ETHER_rects_collide_s16u16(ETHER_rect_s16 a, ETHER_rect_u16 b)
// {
//     return a.x < b.x + b.w
//         && b.x < a.x + a.w
//         && a.y < b.y + b.h
//         && b.y < a.y + a.h;
// }

static inline ETHER_bool ETHER_rects_collide(ETHER_rect a, ETHER_rect b)
{
    return a.x < b.x + b.w
        && b.x < a.x + a.w
        && a.y < b.y + b.h
        && b.y < a.y + a.h;
}

static inline ETHER_rect ETHER_rects_minkowski_difference(ETHER_rect a, ETHER_rect b)
{
    return (ETHER_rect)
    {
        a.x - (b.x + b.w),
        a.y - (b.y + b.h),
        a.w + b.w,
        a.h + b.h
    };
}

static inline ETHER_vec ETHER_rect_closest_point_on_bounds_to_point(ETHER_rect rect, ETHER_vec vec)
{
    int min = ABS(vec.x - rect.x);
    ETHER_vec bounds_point = {rect.x, vec.y};
    if (ABS(rect.x + rect.w - vec.x) < min)
    {
        min = ABS(rect.x + rect.w - vec.x);
        bounds_point = (ETHER_vec) {rect.x + rect.w, vec.y};
    }
    if (ABS(rect.y + rect.h - vec.y) < min)
    {
        min = ABS(rect.y + rect.h - vec.y);
        bounds_point = (ETHER_vec) {vec.x, rect.y + rect.h};
    }
    if (ABS(rect.y - vec.y) < min)
    {
        min = ABS(rect.y - vec.y);
        bounds_point = (ETHER_vec) {vec.x, rect.y};
    }
    return bounds_point;
}

static inline ETHER_vec ETHER_rects_penetration_vector(ETHER_rect a, ETHER_rect b)
{
    ETHER_rect md = ETHER_rects_minkowski_difference(a, b);
    ETHER_vec pen = ETHER_rect_closest_point_on_bounds_to_point(md, (ETHER_vec) {0, 0});
    return pen;
}

static inline int ETHER_vec_cross_product(ETHER_vec a, ETHER_vec b)
{
    return a.x * b.y - a.y * b.x;
}

static inline float ETHER_line_intersection(ETHER_line a, ETHER_line b)
{
    ETHER_vec r = {a.x2 - a.x1, a.y2 - a.y1};
    ETHER_vec s = {b.x2 - b.x1, b.y2 - b.y1};
    ETHER_vec e = {b.x1 - a.x1, b.y1 - a.y1};
    int num = ETHER_vec_cross_product(e, r);
    int dum = ETHER_vec_cross_product(r, s);
    int cum = ETHER_vec_cross_product(e, s);
    if (dum == 0) return INFINITY;
    float u = (float) num / (float) dum;
    float t = (float) cum / (float) dum;
    if (WITHIN_INCLUSIVE(t, 0, 1) && WITHIN_INCLUSIVE(u, 0, 1)) return t;
    return INFINITY;
}

typedef enum ETHER_intersection_side_t ETHER_intersection_side_t;
enum ETHER_intersection_side_t
{
    ETHER_INTERSECTION_SIDE_NONE,
    ETHER_INTERSECTION_SIDE_TOP,
    ETHER_INTERSECTION_SIDE_BOTTOM,
    ETHER_INTERSECTION_SIDE_LEFT,
    ETHER_INTERSECTION_SIDE_RIGHT,
};

typedef struct ETHER_intersection_data ETHER_intersection_data;
struct ETHER_intersection_data
{
    float t;
    ETHER_intersection_side_t side;
};

static inline ETHER_intersection_data ETHER_rect_line_intersection(ETHER_rect rect, ETHER_line line)
{
    ETHER_line left = {rect.x, rect.y, rect.x, rect.y + rect.h},
               right = {rect.x + rect.w, rect.y, rect.x + rect.w, rect.y + rect.h},
               top = {rect.x, rect.y, rect.x + rect.w, rect.y},
               bottom = {rect.x, rect.y + rect.h, rect.x + rect.w, rect.y + rect.h};
    float min = ETHER_line_intersection(line, left);
    ETHER_intersection_side_t side = ETHER_INTERSECTION_SIDE_LEFT;
    float x = ETHER_line_intersection(line, right);
    if (x < min) { min = x, side = ETHER_INTERSECTION_SIDE_RIGHT; }
    x = ETHER_line_intersection(line, top);
    if (x < min) { min = x, side = ETHER_INTERSECTION_SIDE_TOP; }
    x = ETHER_line_intersection(line, bottom);
    if (x < min) { min = x, side = ETHER_INTERSECTION_SIDE_BOTTOM; }
    ETHER_intersection_data data = {min, (min < INFINITY ? side : ETHER_INTERSECTION_SIDE_NONE)};
    return data;
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

// typedef uint8_t ETHER_raycast_side_t;
// #define ETHER_RAYCAST_SIDE_TOP 0
// #define ETHER_RAYCAST_SIDE_BOTTOM 1
// #define ETHER_RAYCAST_SIDE_LEFT 2
// #define ETHER_RAYCAST_SIDE_RIGHT 3

// static inline float ETHER_rect_ray_intersection(ETHER_rect_s16 rect, ETHER_vec_s16 a1, ETHER_vec_s16 a2, ETHER_raycast_side_t *side)
// {
//     float x, min = ETHER_lines_intersection(a1, a2, (ETHER_vec_s16) {rect.x, rect.y}, (ETHER_vec_s16) {rect.x, rect.y + rect.h});
//     (*side) = ETHER_RAYCAST_SIDE_LEFT;
//     x = ETHER_lines_intersection(a1, a2, (ETHER_vec_s16) {rect.x, rect.y + rect.h}, (ETHER_vec_s16) {rect.x + rect.w, rect.y + rect.h});
//     if (x < min) { min = x; (*side) = ETHER_RAYCAST_SIDE_BOTTOM; }
//     x = ETHER_lines_intersection(a1, a2, (ETHER_vec_s16) {rect.x + rect.w, rect.y + rect.h}, (ETHER_vec_s16) {rect.x + rect.w, rect.y});
//     if (x < min) { min = x; (*side) = ETHER_RAYCAST_SIDE_RIGHT; }
//     x = ETHER_lines_intersection(a1, a2, (ETHER_vec_s16) {rect.x + rect.w, rect.y}, (ETHER_vec_s16) {rect.x, rect.y});
//     if (x < min) { min = x; (*side) = ETHER_RAYCAST_SIDE_TOP; };
//     return min;
// }

#endif