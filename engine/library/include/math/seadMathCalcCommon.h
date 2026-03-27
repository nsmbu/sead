#ifndef SEAD_MATHCALCCOMMON_H_
#define SEAD_MATHCALCCOMMON_H_

#include <basis/seadTypes.h>

#include <numbers>

namespace sead {

template <typename T>
class MathCalcCommon
{
public:
    static constexpr u32 cQuarterRoundIdx = 0x40000000; //  90 degrees, PI/2 radians
    static constexpr u32 cHalfRoundIdx    = 0x80000000; // 180 degrees, PI   radians

public:
    struct SinCosSample
    {
        T sin_val;
        T sin_delta;
        T cos_val;
        T cos_delta;
    };

    struct AtanSample
    {
        u32 atan_val;
        T atan_delta;
    };

    struct ExpSample
    {
        T exp_val;
        T exp_delta;
    };

    struct LogSample
    {
        T log_val;
        T log_delta;
    };

public:
    static constexpr T piHalf() { return std::numbers::pi_v<T> / T(2); }
    static constexpr T pi()     { return std::numbers::pi_v<T>; }
    static constexpr T pi2()    { return std::numbers::pi_v<T> * T(2); }
    static constexpr T zero()   { return 0; }
    static constexpr T one()    { return 1; }
    static constexpr T ln2()    { return std::numbers::ln2_v<T>; }
    static constexpr T ln2Inv() { return std::numbers::log2e_v<T>; }

    static constexpr T neg(T t)
    {
        return -t;
    }

    static constexpr T inv(T t)
    {
        return 1 / t;
    }

    static constexpr T sign(T t)
    {
        if (t >= 0) return  1;
        else        return -1;
    }

    static constexpr T fitSign(T x, T y)
    {
        // Unsure
        return abs(x) * sign(y);
    }

    static constexpr T square(T t)
    {
        // Good for temp values
        return t * t;
    }

    static constexpr T sqrt(T t);
    static constexpr T rsqrt(T t);

    static constexpr T pow(T x, T y);
    static T powTable(T, T);

    static constexpr T sin(T t);
    static constexpr T cos(T t);
    static constexpr T tan(T t);

    static constexpr T asin(T s);
    static constexpr T acos(T c);
    static constexpr T atan(T t);
    static constexpr T atan2(T y, T x);

    static T sinIdx(u32 idx);
    static T cosIdx(u32 idx);
    static T tanIdx(u32 idx);

    static u32 asinIdx(T s);
    static u32 acosIdx(T c);
    static u32 atanIdx(T t);
    static u32 atan2Idx(T y, T x);

    static void sinCosIdx(T* p_sin, T* p_cos, u32 idx);

    static constexpr T exp(T t);
    static constexpr T log(T t);
    static constexpr T log2(T t);
    static constexpr T log10(T t);

    static T expTable(T x);
    static T logTable(T x);

    static constexpr T minNumber();
    static constexpr T maxNumber();
    static constexpr T infinity();
    static constexpr T nan();
    static constexpr T epsilon();

    static constexpr bool equalsEpsilon(T a, T b, T eps = epsilon())
    {
        return abs(a - b) <= eps;
    }

    static constexpr T abs(T x)
    {
        if (x > 0) return  x;
        else       return -x;
    }

    static constexpr T max(T a, T b)
    {
        if (a > b) return a;
        else       return b;
    }

    static constexpr T min(T a, T b)
    {
        if (a < b) return a;
        else       return b;
    }

    static constexpr T max3(T a, T b, T c)
    {
        return max(max(a, b), c);
    }

    static constexpr T min3(T a, T b, T c)
    {
        return min(min(a, b), c);
    }

    static constexpr T deg2rad(T a)
    {
        return a * (pi() / 180.0f);
    }

    static constexpr T rad2deg(T a)
    {
        return a * (180.0f / pi());
    }

    static constexpr u32 deg2idx(T a)
    {
        return (s32)(a * cHalfRoundIdx / 180.0f);
    }

    static constexpr u32 rad2idx(T a)
    {
        return (s32)(a * cHalfRoundIdx / pi());
    }

    static constexpr T idx2deg(u32 a)
    {
        return static_cast<float>(static_cast<s32>(a)) * 180.0f / cHalfRoundIdx;
    }

    static constexpr T idx2rad(u32 a)
    {
        return (s32)a * pi() / cHalfRoundIdx;
    }

    static T roundAngle(T);
    static T angleDist(T, T);

    static T random();
    static T getRand(T);
    static T getRandRange(T, T);
    static T getRandSign();

    static constexpr s32 roundOff(T val);
    static constexpr s32 floor(T val);
    static constexpr s32 ceil(T val);

    static T constexpr roundUpN(T val, s32 multNumber);
    static T constexpr roundUpPow2(T val, s32 base);
    static T constexpr roundDownN(T val, s32 multNumber);
    static T constexpr roundDownPow2(T val, s32 base);

    static T constexpr clampMax(T val, T max_);
    static T constexpr clampMin(T val, T min_);
    static T constexpr clamp2(T min_, T val, T max_);

    static T gcd(T m, T n);
    static T lcm(T m, T n);

    static bool isZero(T, T);
    static bool isNan(T);
    static bool isPow2(T);
    static bool isMultiplePow2(T, T);
    static bool isInfinity(T);
    static bool isIntersect1d(T, T, T, T);

    static bool chase(T*, T, T);
    static bool chaseAngle(T*, T, T);
    static bool chaseAngleIdx(u32*, u32, T);

    static constexpr T lerp(T a, T b, f32 ratio)
    {
        return a + (b - a) * ratio;
    }

protected:
    static u32 atanIdx_(T t);
    static T expLn2_(T x);
    static T log1_2_(T x);
    static void assertGreaterThanOrEqualToZero_(T);

public:
    static const SinCosSample cSinCosTbl[256 + 1];
    static const AtanSample     cAtanTbl[128 + 1];
    static const ExpSample       cExpTbl[ 32 + 1];
    static const LogSample       cLogTbl[256 + 1];
};

template <>
s32 MathCalcCommon<s32>::gcd(s32 m, s32 n);

template <>
s32 MathCalcCommon<s32>::lcm(s32 m, s32 n);

template <>
u32 MathCalcCommon<f32>::atanIdx_(f32 t);

template <>
f32 MathCalcCommon<f32>::expTable(f32 x);

template <>
f32 MathCalcCommon<f32>::logTable(f32 x);

template <>
const MathCalcCommon<f32>::SinCosSample MathCalcCommon<f32>::cSinCosTbl[];

template <>
const MathCalcCommon<f32>::AtanSample   MathCalcCommon<f32>::  cAtanTbl[];

template <>
const MathCalcCommon<f32>::ExpSample    MathCalcCommon<f32>::   cExpTbl[];

template <>
const MathCalcCommon<f32>::LogSample    MathCalcCommon<f32>::   cLogTbl[];

// For convenience
typedef MathCalcCommon<s32> Mathi;
typedef MathCalcCommon<u32> Mathu;
typedef MathCalcCommon<f32> Mathf;

} // namespace sead

#include <math/seadMathCalcCommon.hpp>

#endif // SEAD_MATHCALCCOMMON_H_
