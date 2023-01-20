#ifndef SOLVESPACE_CONSTRAINTBASE_H
#define SOLVESPACE_CONSTRAINTBASE_H

#include <string>
#include "data.h"
#include "expr.h"
#include "sketch.h"
#include "entitybase.h"

class ConstraintBase {
public:
    int         tag;
    hConstraint h;

    static const hConstraint NO_CONSTRAINT;

    enum class Type : uint32_t {
        POINTS_COINCIDENT      =  20,
        PT_PT_DISTANCE         =  30,
        PT_PLANE_DISTANCE      =  31,
        PT_LINE_DISTANCE       =  32,
        PT_FACE_DISTANCE       =  33,
        PROJ_PT_DISTANCE       =  34,
        PT_IN_PLANE            =  41,
        PT_ON_LINE             =  42,
        PT_ON_FACE             =  43,
        EQUAL_LENGTH_LINES     =  50,
        LENGTH_RATIO           =  51,
        EQ_LEN_PT_LINE_D       =  52,
        EQ_PT_LN_DISTANCES     =  53,
        EQUAL_ANGLE            =  54,
        EQUAL_LINE_ARC_LEN     =  55,
        LENGTH_DIFFERENCE      =  56,
        SYMMETRIC              =  60,
        SYMMETRIC_HORIZ        =  61,
        SYMMETRIC_VERT         =  62,
        SYMMETRIC_LINE         =  63,
        AT_MIDPOINT            =  70,
        HORIZONTAL             =  80,
        VERTICAL               =  81,
        DIAMETER               =  90,
        PT_ON_CIRCLE           = 100,
        SAME_ORIENTATION       = 110,
        ANGLE                  = 120,
        PARALLEL               = 121,
        PERPENDICULAR          = 122,
        ARC_LINE_TANGENT       = 123,
        CUBIC_LINE_TANGENT     = 124,
        CURVE_CURVE_TANGENT    = 125,
        EQUAL_RADIUS           = 130,
        WHERE_DRAGGED          = 200,
        ARC_ARC_LEN_RATIO      = 210,
        ARC_LINE_LEN_RATIO     = 211,
        ARC_ARC_DIFFERENCE     = 212,
        ARC_LINE_DIFFERENCE    = 213,
        COMMENT                = 1000
    };

    Type        type;

    hGroup      group;
    hEntity     workplane;

    // These are the parameters for the constraint.
    double      valA;
    hParam      valP;
    hEntity     ptA;
    hEntity     ptB;
    hEntity     entityA;
    hEntity     entityB;
    hEntity     entityC;
    hEntity     entityD;
    bool        other;
    bool        other2;

    bool        reference;  // a ref dimension, that generates no eqs
    std::string comment;    // since comments are represented as constraints

    bool Equals(const ConstraintBase &c) const {
        return type == c.type && group == c.group && workplane == c.workplane &&
            valA == c.valA && valP == c.valP && ptA == c.ptA && ptB == c.ptB &&
            entityA == c.entityA && entityB == c.entityB &&
            entityC == c.entityC && entityD == c.entityD &&
            other == c.other && other2 == c.other2 && reference == c.reference &&
            comment == c.comment;
    }

    bool HasLabel() const;
    bool IsProjectible() const;

    void Generate(IdList<Param, hParam> *param);

    void GenerateEquations(IdList<Equation,hEquation> *entity,
                           bool forReference = false) const;

    Vector ProjectVectorInto(Vector a, EntityBase *w) const;
    Vector ProjectInto(Vector a, EntityBase *w) const;

    // Some helpers when generating symbolic constraint equations
    void ModifyToSatisfy();
    void AddEq(IdList<Equation,hEquation> *l, Expr *expr, int index) const;
    void AddEq(IdList<Equation,hEquation> *l, const ExprVector &v, int baseIndex = 0) const;
    static Expr *DirectionCosine(hEntity wrkpl, ExprVector ae, ExprVector be);
    static Expr *Distance(hEntity workplane, hEntity pa, hEntity pb);
    static Expr *PointLineDistance(hEntity workplane, hEntity pt, hEntity ln);
    static Expr *PointPlaneDistance(ExprVector p, hEntity plane);
    static ExprVector VectorsParallel3d(ExprVector a, ExprVector b, hParam p);
    static ExprVector PointInThreeSpace(hEntity workplane, Expr *u, Expr *v);

    void Clear() {}
};

#endif