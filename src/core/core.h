#ifndef SOLVESPACE_CORE_H
#define SOLVESPACE_CORE_H

#include <string>
#include <cmath>
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
// class Vector4;
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

template<class CONSTRAINT, class ENTITY>
class Sketch;

extern Sketch<ConstraintBase, EntityBase> SK;

// A simple list
template<class T>
class List {
    T *elem            = nullptr;
    int elemsAllocated = 0;

public:
    int n = 0;

    bool IsEmpty() const {
        return n == 0;
    }

    void ReserveMore(int howMuch) {
        if(n + howMuch > elemsAllocated) {
            elemsAllocated = n + howMuch;
            T *newElem     = (T *)::operator new[]((size_t)elemsAllocated * sizeof(T));
            for(int i = 0; i < n; i++) {
                new(&newElem[i]) T(std::move(elem[i]));
                elem[i].~T();
            }
            ::operator delete[](elem);
            elem = newElem;
        }
    }

    void AllocForOneMore() {
        if(n >= elemsAllocated) {
            ReserveMore((elemsAllocated + 32) * 2 - n);
        }
    }

    void Add(const T *t) {
        AllocForOneMore();
        new(&elem[n++]) T(*t);
    }

    void AddToBeginning(const T *t) {
        AllocForOneMore();
        new(&elem[n]) T();
        std::move_backward(elem, elem + 1, elem + n + 1);
        elem[0] = *t;
        n++;
    }

    T *First() {
        return IsEmpty() ? nullptr : &(elem[0]);
    }
    const T *First() const {
        return IsEmpty() ? nullptr : &(elem[0]);
    }

    T *Last() {
        return IsEmpty() ? nullptr : &(elem[n - 1]);
    }
    const T *Last() const {
        return IsEmpty() ? nullptr : &(elem[n - 1]);
    }

    T *NextAfter(T *prev) {
        if(IsEmpty() || !prev)
            return NULL;
        if(prev - First() == (n - 1))
            return NULL;
        return prev + 1;
    }
    const T *NextAfter(const T *prev) const {
        if(IsEmpty() || !prev)
            return NULL;
        if(prev - First() == (n - 1))
            return NULL;
        return prev + 1;
    }

    T &Get(size_t i) {
        return elem[i];
    }
    T const &Get(size_t i) const {
        return elem[i];
    }
    T &operator[](size_t i) {
        return Get(i);
    }
    T const &operator[](size_t i) const {
        return Get(i);
    }

    T *begin() {
        return IsEmpty() ? nullptr : &elem[0];
    }
    T *end() {
        return IsEmpty() ? nullptr : &elem[n];
    }
    const T *begin() const {
        return IsEmpty() ? nullptr : &elem[0];
    }
    const T *end() const {
        return IsEmpty() ? nullptr : &elem[n];
    }
    const T *cbegin() const {
        return begin();
    }
    const T *cend() const {
        return end();
    }

    void ClearTags() {
        for(auto &elt : *this) {
            elt.tag = 0;
        }
    }

    void Clear() {
        for(int i = 0; i < n; i++)
            elem[i].~T();
        if(elem)
            ::operator delete[](elem);
        elem = NULL;
        n = elemsAllocated = 0;
    }

    void RemoveTagged() {
        auto newEnd = std::remove_if(this->begin(), this->end(), [](T &t) {
            if(t.tag) {
                return true;
            }
            return false;
        });
        auto oldEnd = this->end();
        n           = newEnd - begin();
        if(newEnd != nullptr && oldEnd != nullptr) {
            while(newEnd != oldEnd) {
                newEnd->~T();
                ++newEnd;
            }
        }
        // and elemsAllocated is untouched, because we didn't resize
    }

    void RemoveLast(int cnt) {
        ssassert(n >= cnt, "Removing more elements than the list contains");
        for(int i = n - cnt; i < n; i++)
            elem[i].~T();
        n -= cnt;
        // and elemsAllocated is untouched, same as in RemoveTagged
    }

    void Reverse() {
        int i;
        for(i = 0; i < (n / 2); i++) {
            swap(elem[i], elem[(n - 1) - i]);
        }
    }
};

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

inline bool IsReasonable(double x) {
    return std::isnan(x) || x > 1e11 || x < -1e11;
}

#endif
