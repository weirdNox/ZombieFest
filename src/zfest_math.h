#ifndef ZFEST_MATH_H
#define ZFEST_MATH_H

#include <math.h>

union V2
{
    struct
    {
        real32 x, y;
    };
    real32 e[2];
};

union V3
{
    struct
    {
        real32 x, y, z;
    };
    struct
    {
        real32 r, g, b;
    };
    struct
    {
        V2 xy;
        real32 _ignored0_;
    };
    struct
    {
        real32 _ignored1_;
        V2 yz;
    };
    real32 e[3];
};

union V4
{
    struct
    {
        real32 x, y, z, w;
    };
    struct
    {
        V3 xyz;
        real32 _ignored0_;
    };
    struct
    {
        real32 r, g, b, a;
    };
    struct
    {
        V3 rgb;
        real32 _ignored1_;
    };
    real32 e[4];
};

struct Rectangle2
{
    V2 min;
    V2 max;
};

struct Rectangle3
{
    V3 min;
    V3 max;
};

//
// NOTE(nox): Scalars
//

#define sin sin_
inline real32 sin(real32 angle)
{
    return sinf(angle);
}

#define cos cos_
inline real32 cos(real32 angle)
{
    return cosf(angle);
}

inline real32 aTan2(real32 y, real32 x)
{
    return atan2f(y, x);
}

inline real32 square(real32 value)
{
    real32 result = value*value;

    return result;
}

inline real32 squareRoot(real32 value)
{
    real32 result = sqrtf(value);

    return result;
}

inline real32 lerp(real32 a, real32 b, real32 t)
{
    real32 result = (1.0f - t)*a + t*b;

    return result;
}

inline real32 clamp(real32 min, real32 max, real32 value)
{
    real32 result = value;

    if(result < min)
    {
        result = min;
    }
    else if(result > max)
    {
        result = max;
    }

    return result;
}

inline real32 clamp01(real32 value)
{
    real32 result = clamp(0.0f, 1.0f, value);

    return result;
}

inline real32 safeRatioN(real32 numerator, real32 divisor, real32 n)
{
    real32 result = n;

    if(divisor != 0.0f)
    {
        result = numerator / divisor;
    }

    return result;
}

inline real32 safeRatio0(real32 numerator, real32 divisor)
{
    return safeRatioN(numerator, divisor, 0.0f);
}

inline real32 safeRatio1(real32 numerator, real32 divisor)
{
    return safeRatioN(numerator, divisor, 1.0f);
}

//
// NOTE(nox): V2 operations
//

inline V2 v2(real32 x, real32 y)
{
    V2 result;

    result.x = x;
    result.y = y;

    return result;
}

inline V2 v2i(int32 x, int32 y)
{
    V2 result = v2((real32)x, (real32)y);

    return result;
}

inline V2 v2i(uint32 x, uint32 y)
{
    V2 result = v2((real32)x, (real32)y);

    return result;
}

inline V2 operator-(V2 a)
{
    V2 result;

    result.x = -a.x;
    result.y = -a.y;

    return result;
}

inline V2 operator+(V2 a, V2 b)
{
    V2 result;

    result.x = a.x + b.x;
    result.y = a.y + b.y;

    return result;
}

inline V2 &operator+=(V2 &a, V2 b)
{
    a = a + b;

    return a;
}

inline V2 operator-(V2 a, V2 b)
{
    V2 result;

    result = a + (-b);

    return result;
}

inline V2 &operator-=(V2 &a, V2 b)
{
    a = a - b;

    return a;
}

inline V2 operator*(real32 a, V2 b)
{
    V2 result;

    result.x = a * b.x;
    result.y = a * b.y;

    return result;
}

inline V2 operator*(V2 a, real32 b)
{
    V2 result;

    result = b * a;

    return result;
}

inline V2 &operator*=(V2 &a, real32 b)
{
    a = a * b;

    return a;
}

inline V2 hadamard(V2 a, V2 b)
{
    V2 result = {a.x*b.x, a.y*b.y};

    return result;
}

inline real32 dot(V2 a, V2 b)
{
    real32 result = a.x*b.x + a.y*b.y;

    return result;
}

inline real32 lengthSq(V2 a)
{
    real32 result = dot(a, a);

    return result;
}

inline real32 length(V2 a)
{
    real32 result = squareRoot(lengthSq(a));

    return result;
}

inline V2 clamp01(V2 value)
{
    V2 result;

    result.x = clamp(0.0f, 1.0f, value.x);
    result.y = clamp(0.0f, 1.0f, value.y);

    return result;
}

inline V2 perp(V2 a)
{
    V2 result = v2(-a.y, a.x);
    return result;
}

//
// NOTE(nox): V3 operations
//

inline V3 v3(real32 x, real32 y, real32 z)
{
    V3 result;

    result.x = x;
    result.y = y;
    result.z = z;

    return result;
}

inline V3 v3(V2 xy, real32 z)
{
    V3 result;

    result.x = xy.x;
    result.y = xy.y;
    result.z = z;

    return result;
}

inline V3 operator-(V3 a)
{
    V3 result;

    result.x = -a.x;
    result.y = -a.y;
    result.z = -a.z;

    return result;
}

inline V3 operator+(V3 a, V3 b)
{
    V3 result;

    result.x = a.x + b.x;
    result.y = a.y + b.y;
    result.z = a.z + b.z;

    return result;
}

inline V3 &operator+=(V3 &a, V3 b)
{
    a = a + b;

    return a;
}

inline V3 operator-(V3 a, V3 b)
{
    V3 result;

    result = a + (-b);

    return result;
}

inline V3 &operator-=(V3 &a, V3 b)
{
    a = a - b;

    return a;
}

inline V3 operator*(real32 a, V3 b)
{
    V3 result;

    result.x = a * b.x;
    result.y = a * b.y;
    result.z = a * b.z;

    return result;
}

inline V3 operator*(V3 a, real32 b)
{
    V3 result;

    result = b * a;

    return result;
}

inline V3 &operator*=(V3 &a, real32 b)
{
    a = a * b;

    return a;
}

inline V3 hadamard(V3 a, V3 b)
{
    V3 result = {a.x*b.x, a.y*b.y, a.z*b.z};

    return result;
}

inline real32 dot(V3 a, V3 b)
{
    real32 result = a.x*b.x + a.y*b.y + a.z*b.z;

    return result;
}

inline real32 lengthSq(V3 a)
{
    real32 result = dot(a, a);

    return result;
}

inline real32 length(V3 a)
{
    real32 result = squareRoot(lengthSq(a));

    return result;
}

inline V3 normalize(V3 a)
{
    V3 result = a * (1.0f / length(a));

    return result;
}

inline V3 clamp01(V3 value)
{
    V3 result;

    result.x = clamp(0.0f, 1.0f, value.x);
    result.y = clamp(0.0f, 1.0f, value.y);
    result.z = clamp(0.0f, 1.0f, value.z);

    return result;
}

inline V3 lerp(V3 a, V3 b, real32 t)
{
    V3 result = (1.0f - t)*a + t*b;

    return result;
}

//
// NOTE(nox): V4 operations
//

inline V4 v4(real32 x, real32 y, real32 z, real32 w)
{
    V4 result;

    result.x = x;
    result.y = y;
    result.z = z;
    result.w = w;

    return result;
}

inline V4 v4i(int32 x, int32 y, int32 z, int32 w)
{
    V4 result = v4((real32)x, (real32)y, (real32)z, (real32)w);

    return result;
}

inline V4 v4i(uint32 x, uint32 y, uint32 z, uint32 w)
{
    V4 result = v4((real32)x, (real32)y, (real32)z, (real32)w);

    return result;
}

inline V4 v4(V3 xyz, real32 w)
{
    V4 result;

    result.xyz = xyz;
    result.w = w;

    return result;
}

inline V4 operator-(V4 a)
{
    V4 result;

    result.x = -a.x;
    result.y = -a.y;
    result.z = -a.z;
    result.w = -a.w;

    return result;
}

inline V4 operator+(V4 a, V4 b)
{
    V4 result;

    result.x = a.x + b.x;
    result.y = a.y + b.y;
    result.z = a.z + b.z;
    result.w = a.w + b.w;

    return result;
}

inline V4 &operator+=(V4 &a, V4 b)
{
    a = a + b;

    return a;
}

inline V4 operator-(V4 a, V4 b)
{
    V4 result;

    result = a + (-b);

    return result;
}

inline V4 &operator-=(V4 &a, V4 b)
{
    a = a - b;

    return a;
}

inline V4 operator*(real32 a, V4 b)
{
    V4 result;

    result.x = a * b.x;
    result.y = a * b.y;
    result.z = a * b.z;
    result.w = a * b.w;

    return result;
}

inline V4 operator*(V4 a, real32 b)
{
    V4 result;

    result = b * a;

    return result;
}

inline V4 &operator*=(V4 &a, real32 b)
{
    a = a * b;

    return a;
}

inline V4 hadamard(V4 a, V4 b)
{
    V4 result = {a.x*b.x, a.y*b.y, a.z*b.z, a.w*b.w};

    return result;
}

inline real32 dot(V4 a, V4 b)
{
    real32 result = a.x*b.x + a.y*b.y + a.z*b.z + a.w*b.w;

    return result;
}

inline real32 lengthSq(V4 a)
{
    real32 result = dot(a, a);

    return result;
}

inline real32 length(V4 a)
{
    real32 result = squareRoot(lengthSq(a));

    return result;
}

inline V4 clamp01(V4 value)
{
    V4 result;

    result.x = clamp(0.0f, 1.0f, value.x);
    result.y = clamp(0.0f, 1.0f, value.y);
    result.z = clamp(0.0f, 1.0f, value.z);
    result.w = clamp(0.0f, 1.0f, value.w);

    return result;
}

inline V4 lerp(V4 a, V4 b, real32 t)
{
    V4 result = (1.0f - t)*a + t*b;

    return result;
}

//
// NOTE(nox): Rectangle2 operations
//

inline Rectangle2 rectMinMax(V2 min, V2 max)
{
    Rectangle2 result;

    result.min = min;
    result.max = max;

    return result;
}

inline Rectangle2 rectCenterHalfDim(V2 center, V2 halfDim)
{
    Rectangle2 result;

    result.min = center - halfDim;
    result.max = center + halfDim;

    return result;
}

inline Rectangle2 rectCenterDim(V2 center, V2 dim)
{
    Rectangle2 result = rectCenterHalfDim(center, 0.5f*dim);

    return result;
}

inline Rectangle2 addRadiusTo(Rectangle2 rect, V2 radius)
{
    Rectangle2 result;

    result.min = rect.min - radius;
    result.max = rect.max + radius;

    return result;
}

inline Rectangle2 offset(Rectangle2 rect, V2 offset)
{
    Rectangle2 result;

    result.min = rect.min + offset;
    result.max = rect.max + offset;

    return result;
}

inline V2 getMinCorner(Rectangle2 rect)
{
    V2 result = rect.min;
    return result;
}

inline V2 getMaxCorner(Rectangle2 rect)
{
    V2 result = rect.max;
    return result;
}

inline V2 getCenter(Rectangle2 rect)
{
    V2 result = 0.5f*(rect.min + rect.max);
    return result;
}

inline bool32 isInRectangle(Rectangle2 rect, V2 test)
{
    bool32 result = ((test.x >= rect.min.x) &&
                     (test.y >= rect.min.y) &&
                     (test.x < rect.max.x) &&
                     (test.y < rect.max.y));

    return result;
}

inline bool32 rectanglesIntersect(Rectangle2 a, Rectangle2 b)
{
    bool32 result = !(b.max.x <= a.min.x ||
                      b.min.x >= a.max.x ||
                      b.max.y <= a.min.y ||
                      b.min.y >= a.max.y);
    return result;
}

inline V2 getBarycentric(Rectangle2 a, V2 pos)
{
    V2 result;

    result.x = safeRatio0(pos.x - a.min.x, a.max.x - a.min.x);
    result.y = safeRatio0(pos.y - a.min.y, a.max.y - a.min.y);

    return result;
}

//
// NOTE(nox): Rectangle3 operations
//

inline Rectangle3 rectMinMax(V3 min, V3 max)
{
    Rectangle3 result;

    result.min = min;
    result.max = max;

    return result;
}

inline Rectangle3 rectCenterHalfDim(V3 center, V3 halfDim)
{
    Rectangle3 result;

    result.min = center - halfDim;
    result.max = center + halfDim;

    return result;
}

inline Rectangle3 rectCenterDim(V3 center, V3 dim)
{
    Rectangle3 result = rectCenterHalfDim(center, 0.5f*dim);

    return result;
}

inline Rectangle3 addRadiusTo(Rectangle3 rect, V3 radius)
{
    Rectangle3 result;

    result.min = rect.min - radius;
    result.max = rect.max + radius;

    return result;
}

inline Rectangle3 offset(Rectangle3 rect, V3 offset)
{
    Rectangle3 result;

    result.min = rect.min + offset;
    result.max = rect.max + offset;

    return result;
}

inline V3 getMinCorner(Rectangle3 rect)
{
    V3 result = rect.min;
    return result;
}

inline V3 getMaxCorner(Rectangle3 rect)
{
    V3 result = rect.max;
    return result;
}

inline V3 getCenter(Rectangle3 rect)
{
    V3 result = 0.5f*(rect.min + rect.max);
    return result;
}

inline bool32 isInRectangle(Rectangle3 rect, V3 test)
{
    bool32 result = ((test.x >= rect.min.x) &&
                     (test.y >= rect.min.y) &&
                     (test.z >= rect.min.z) &&
                     (test.x < rect.max.x) &&
                     (test.y < rect.max.y) &&
                     (test.z < rect.max.z));

    return result;
}

inline bool32 rectanglesIntersect(Rectangle3 a, Rectangle3 b)
{
    bool32 result = !(b.max.x <= a.min.x ||
                      b.min.x >= a.max.x ||
                      b.max.y <= a.min.y ||
                      b.min.y >= a.max.y ||
                      b.max.z <= a.min.z ||
                      b.min.z >= a.max.z);
    return result;
}

inline V3 getBarycentric(Rectangle3 a, V3 pos)
{
    V3 result;

    result.x = safeRatio0(pos.x - a.min.x, a.max.x - a.min.x);
    result.y = safeRatio0(pos.y - a.min.y, a.max.y - a.min.y);
    result.z = safeRatio0(pos.z - a.min.z, a.max.z - a.min.z);

    return result;
}

#endif // ZFEST_MATH_H
