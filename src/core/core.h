#ifndef SOLVESPACE_CORE_H
#define SOLVESPACE_CORE_H

#include <string>
#include "minimalplatform.h"

using namespace SolveSpace::Platform;

// static constexpr double ANGLE_COS_EPS = 1e-6;
static constexpr double LENGTH_EPS = 1e-6;
static constexpr double VERY_POSITIVE = 1e10;
// static constexpr double VERY_NEGATIVE = -1e10;

#define PI (3.1415926535897931)

// The few floating-point equality comparisons in SolveSpace have been
// carefully considered, so we disable the -Wfloat-equal warning for them
#ifdef __clang__
#    define EXACT(expr) \
        (_Pragma("clang diagnostic push") _Pragma("clang diagnostic ignored \"-Wfloat-equal\"")( \
            expr) _Pragma("clang diagnostic pop"))
#else
#    define EXACT(expr) (expr)
#endif

#define dbp DebugPrint

inline int WRAP(int v, int n) {
    // Clamp it to the range [0, n)
    while(v >= n)
        v -= n;
    while(v < 0)
        v += n;
    return v;
}

class Vector;
class Vector4;
class Expr;
class ExprVector;
class Quaternion;
class ExprQuaternion;

class hGroup;
class hRequest;
class hEntity;
class hParam;
// class hStyle;
class hConstraint;
class hEquation;

class ConstraintBase;
class EntityBase;

class Point2d;

// template<class C, class E>
class Sketch;

extern Sketch SK;

template<class T, class H>
class IdList;

// Comparison functor used by IdList and related classes
template<class T, class H>
struct CompareId {
    CompareId(const IdList<T, H> *list) {
        idlist = list;
    }

    bool operator()(int lhs, T const &rhs) const {
        return idlist->elemstore[lhs].h.v < rhs.h.v;
    }

    bool operator()(int lhs, H rhs) const {
        return idlist->elemstore[lhs].h.v < rhs.v;
    }

    bool operator()(T *lhs, int rhs) const {
        return lhs->h.v < idlist->elemstore[rhs].h.v;
    }

private:
    const IdList<T, H> *idlist;
};

#endif
