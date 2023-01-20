#ifndef SOLVESPACE_ENTITYBASE_H
#define SOLVESPACE_ENTITYBASE_H

#include <string>
#include <array>
#include "data.h"
#include "expr.h"
#include "quaternion.h"
#include "sketch.h"
#include "point2d.h"
#include "vector.h"
#include "constraintbase.h"

#define MAX_POINTS_IN_ENTITY (12)

class EntityBase {
public:
    int tag;
    hEntity h;

    static const hEntity FREE_IN_3D;
    static const hEntity NO_ENTITY;
    static const EntityBase _FREE_IN_3D;
    static const EntityBase _NO_ENTITY;

    enum class Type : uint32_t {
        POINT_IN_3D            = 2000,
        POINT_IN_2D            = 2001,
        POINT_N_TRANS          = 2010,
        POINT_N_ROT_TRANS      = 2011,
        POINT_N_COPY           = 2012,
        POINT_N_ROT_AA         = 2013,
        POINT_N_ROT_AXIS_TRANS = 2014,

        NORMAL_IN_3D    = 3000,
        NORMAL_IN_2D    = 3001,
        NORMAL_N_COPY   = 3010,
        NORMAL_N_ROT    = 3011,
        NORMAL_N_ROT_AA = 3012,

        DISTANCE        = 4000,
        DISTANCE_N_COPY = 4001,

        FACE_NORMAL_PT        = 5000,
        FACE_XPROD            = 5001,
        FACE_N_ROT_TRANS      = 5002,
        FACE_N_TRANS          = 5003,
        FACE_N_ROT_AA         = 5004,
        FACE_ROT_NORMAL_PT    = 5005,
        FACE_N_ROT_AXIS_TRANS = 5006,

        WORKPLANE      = 10000,
        LINE_SEGMENT   = 11000,
        CUBIC          = 12000,
        CUBIC_PERIODIC = 12001,
        CIRCLE         = 13000,
        ARC_OF_CIRCLE  = 14000,
        TTF_TEXT       = 15000,
        IMAGE          = 16000
    };

    Type type;

    hGroup group;
    hEntity workplane; // or Entity::FREE_IN_3D

    // When it comes time to draw an entity, we look here to get the
    // defining variables.
    std::array<hEntity, 12> point;
    int extraPoints;
    hEntity normal;
    hEntity distance;
    // The only types that have their own params are points, normals,
    // and directions.
    std::array<hParam, 8> param;

    // Transformed points/normals/distances have their numerical base
    Vector numPoint;
    Quaternion numNormal;
    double numDistance;

    std::string str;
    std::string font;
    // Platform::Path file;
    double aspectRatio;

    // For entities that are derived by a transformation, the number of
    // times to apply the transformation.
    int timesApplied;

    inline std::vector<hParam> GetParams() {
        std::vector<hParam> params;
        for (hParam &p : param) {
            if (p.v != 0) {
                params.push_back(p);
            }
        }
        return params;
    }

    inline bool IsFreeIn3D() {
        return h == _FREE_IN_3D.h;
    }

    inline bool Is3D() {
        return workplane == FREE_IN_3D;
    }

    inline bool IsNone() {
        return h.v == 0;
    }

    inline bool IsPoint2D() {
        return type == Type::POINT_IN_2D;
    }

    inline bool IsPoint3D() {
        return type == Type::POINT_IN_3D;
    }

    inline bool IsNormal2D() {
        return type == Type::NORMAL_IN_2D;
    }

    inline bool IsNormal3D() {
        return type == Type::NORMAL_IN_3D;
    }

    inline bool IsLine() {
        return type == Type::LINE_SEGMENT;
    }

    inline bool IsLine2D() {
        return type == Type::LINE_SEGMENT && !Is3D();
    }

    inline bool IsLine3D() {
        return type == Type::LINE_SEGMENT && Is3D();
    }

    inline bool IsCubic() {
        return type == Type::CUBIC;
    }

    inline bool IsArc() {
        return type == Type::ARC_OF_CIRCLE;
    }

    std::string ToString() const;

    Quaternion QuaternionFromParams(hParam w, hParam vx, hParam vy, hParam vz) const;

    Quaternion GetAxisAngleQuaternion(int param0) const;
    ExprQuaternion GetAxisAngleQuaternionExprs(int param0) const;

    bool IsCircle() const;
    Expr *CircleGetRadiusExpr() const;
    double CircleGetRadiusNum() const;
    void ArcGetAngles(double *thetaa, double *thetab, double *dtheta) const;

    bool HasVector() const;
    ExprVector VectorGetExprs() const;
    ExprVector VectorGetExprsInWorkplane(hEntity wrkpl) const;

    Vector VectorFromParams(hParam x, hParam y, hParam z) const;
    Vector VectorGetNum() const;
    Vector VectorGetRefPoint() const;
    Vector VectorGetStartPoint() const;

    // For distances
    bool IsDistance() const;
    double DistanceGetNum() const;
    Expr *DistanceGetExpr() const;
    void DistanceForceTo(double v);

    bool IsWorkplane() const;
    // The plane is points P such that P dot (xn, yn, zn) - d = 0
    void WorkplaneGetPlaneExprs(ExprVector *n, Expr **d) const;
    ExprVector WorkplaneGetOffsetExprs() const;
    Vector WorkplaneGetOffset() const;
    EntityBase *Normal() const;

    bool IsFace() const;
    ExprVector FaceGetNormalExprs() const;
    Vector FaceGetNormalNum() const;
    ExprVector FaceGetPointExprs() const;
    Vector FaceGetPointNum() const;

    bool IsPoint() const;
    // Applies for any of the point types
    Vector PointGetNum() const;
    ExprVector PointGetExprs() const;
    void PointGetExprsInWorkplane(hEntity wrkpl, Expr **u, Expr **v) const;
    ExprVector PointGetExprsInWorkplane(hEntity wrkpl) const;
    void PointForceTo(Vector v);
    void PointForceParamTo(Vector v);
    // These apply only the POINT_N_ROT_TRANS, which has an assoc rotation
    Quaternion PointGetQuaternion() const;
    void PointForceQuaternionTo(Quaternion q);

    bool IsNormal() const;
    // Applies for any of the normal types
    Quaternion NormalGetNum() const;
    ExprQuaternion NormalGetExprs() const;
    void NormalForceTo(Quaternion q);

    Vector NormalU() const;
    Vector NormalV() const;
    Vector NormalN() const;
    ExprVector NormalExprsU() const;
    ExprVector NormalExprsV() const;
    ExprVector NormalExprsN() const;

    Vector CubicGetStartNum() const;
    Vector CubicGetFinishNum() const;
    ExprVector CubicGetStartTangentExprs() const;
    ExprVector CubicGetFinishTangentExprs() const;
    Vector CubicGetStartTangentNum() const;
    Vector CubicGetFinishTangentNum() const;

    bool HasEndpoints() const;
    Vector EndpointStart() const;
    Vector EndpointFinish() const;
    bool IsInPlane(Vector norm, double distance) const;

    void RectGetPointsExprs(ExprVector *eap, ExprVector *ebp) const;

    void AddEq(IdList<Equation, hEquation> *l, Expr *expr, int index) const;
    void GenerateEquations(IdList<Equation, hEquation> *l) const;

    void Clear() {}
};

#endif