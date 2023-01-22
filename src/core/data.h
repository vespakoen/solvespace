#ifndef SOLVESPACE_DATA_H
#define SOLVESPACE_DATA_H

#include <string>
#include <type_traits>
#include <vector>
#include "core.h"
#include "idlist.h"

/// Trait indicating which types are handle types and should get the associated operators.
/// Specialize for each handle type and inherit from std::true_type.
template<typename T>
struct IsHandleOracle : std::false_type {};

// Equality-compare any two instances of a handle type.
template<typename T>
static inline typename std::enable_if<IsHandleOracle<T>::value, bool>::type
operator==(T const &lhs, T const &rhs) {
    return lhs.v == rhs.v;
}

// Inequality-compare any two instances of a handle type.
template<typename T>
static inline typename std::enable_if<IsHandleOracle<T>::value, bool>::type
operator!=(T const &lhs, T const &rhs) {
    return !(lhs == rhs);
}

// Less-than-compare any two instances of a handle type.
template<typename T>
static inline typename std::enable_if<IsHandleOracle<T>::value, bool>::type
operator<(T const &lhs, T const &rhs) {
    return lhs.v < rhs.v;
}

// All of the hWhatever handles are a 32-bit ID, that is used to represent
// some data structure in the sketch.
class hGroup {
public:
    // bits 15: 0   -- group index
    uint32_t v;

    inline hEntity entity(int i) const;
    inline hParam param(int i) const;
    inline hEquation equation(int i) const;
};

template<>
struct IsHandleOracle<hGroup> : std::true_type {};

// class hRequest {
// public:
//     // bits 15: 0   -- request index
//     uint32_t v;

//     inline hEntity entity(int i) const;
//     inline hParam param(int i) const;

//     inline bool IsFromReferences() const;
// };

// template<>
// struct IsHandleOracle<hRequest> : std::true_type {};

class hEntity {
public:
    // bits 15: 0   -- entity index
    //      31:16   -- request index
    uint32_t v;

    // inline bool isFromRequest() const;
    // inline hRequest request() const;
    inline hGroup group() const;
    inline hEquation equation(int i) const;
};

template<>
struct IsHandleOracle<hEntity> : std::true_type {};

class hParam {
public:
    // bits 15: 0   -- param index
    //      31:16   -- request index
    uint32_t v;

    // inline hRequest request() const;
};

template<>
struct IsHandleOracle<hParam> : std::true_type {};

class hConstraint {
public:
    uint32_t v;

    inline hEquation equation(int i) const;
    inline hParam param(int i) const;
};

template<>
struct IsHandleOracle<hConstraint> : std::true_type {};

class hEquation {
public:
    uint32_t v;

    inline bool isFromConstraint() const;
    inline hConstraint constraint() const;
};

template<>
struct IsHandleOracle<hEquation> : std::true_type {};

class Equation {
public:
    int tag;
    hEquation h;

    Expr *e;

    void Clear() {
    }
};

class Param {
public:
    int tag;
    hParam h;

    double val;
    bool known;
    bool free;

    // Used only in the solver
    Param *substd;

    static const hParam NO_PARAM;

    void Clear() {
    }

    inline std::string ToString() {
        std::string repr = "";
        repr += "h:" + std::to_string(h.v) +
            " val:" + std::to_string(val);
        return repr;
    }
};

typedef IdList<EntityBase, hEntity> EntityList;
typedef IdList<Param, hParam> ParamList;

enum class SolveResult : uint32_t {
    OKAY                     = 0,
    DIDNT_CONVERGE           = 10,
    REDUNDANT_OKAY           = 11,
    REDUNDANT_DIDNT_CONVERGE = 12,
    TOO_MANY_UNKNOWNS        = 20
};

// hGroup
inline hEntity hGroup::entity(int i) const {
    hEntity r;
    r.v = 0x80000000 | (v << 16) | (uint32_t)i;
    return r;
}

inline hParam hGroup::param(int i) const {
    hParam r;
    r.v = 0x80000000 | (v << 16) | (uint32_t)i;
    return r;
}

inline hEquation hGroup::equation(int i) const {
    hEquation r;
    r.v = (v << 16) | 0x80000000 | (uint32_t)i;
    return r;
}

// hRequest
// inline bool hRequest::IsFromReferences() const {
//     if(*this == Request::HREQUEST_REFERENCE_XY)
//         return true;
//     if(*this == Request::HREQUEST_REFERENCE_YZ)
//         return true;
//     if(*this == Request::HREQUEST_REFERENCE_ZX)
//         return true;
//     return false;
// }

// inline hEntity hRequest::entity(int i) const {
//     hEntity r;
//     r.v = (v << 16) | (uint32_t)i;
//     return r;
// }

// inline hParam hRequest::param(int i) const {
//     hParam r;
//     r.v = (v << 16) | (uint32_t)i;
//     return r;
// }

// hEntity
// inline bool hEntity::isFromRequest() const {
//     if(v & 0x80000000)
//         return false;
//     else
//         return true;
// }

// inline hRequest hEntity::request() const {
//     hRequest r;
//     r.v = (v >> 16);
//     return r;
// }

inline hGroup hEntity::group() const {
    hGroup r;
    r.v = (v >> 16) & 0x3fff;
    return r;
}

inline hEquation hEntity::equation(int i) const {
    hEquation r;
    r.v = v | 0x40000000 | (uint32_t)i;
    return r;
}

// hParam
// inline hRequest hParam::request() const {
//     hRequest r;
//     r.v = (v >> 16);
//     return r;
// }

// hConstraint
inline hEquation hConstraint::equation(int i) const {
    hEquation r;
    r.v = (v << 16) | (uint32_t)i;
    return r;
}

inline hParam hConstraint::param(int i) const {
    hParam r;
    r.v = v | 0x40000000 | (uint32_t)i;
    return r;
}

// hEquation
inline bool hEquation::isFromConstraint() const {
    if(v & 0xc0000000)
        return false;
    else
        return true;
}

inline hConstraint hEquation::constraint() const {
    hConstraint r;
    r.v = (v >> 16);
    return r;
}

#endif