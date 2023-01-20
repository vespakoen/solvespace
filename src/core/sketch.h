#ifndef SOLVESPACE_SKETCH_H
#define SOLVESPACE_SKETCH_H

#include <stdexcept>
#include "data.h"
#include "idlist.h"
#include "group.h"
#include "vector.h"
#include "quaternion.h"

template<class CONSTRAINT, class ENTITY>
class Sketch {
public:
    // These are user-editable, and define the sketch.
    IdList<Group, hGroup> group;
    // List<hGroup> groupOrder;
    IdList<CONSTRAINT, hConstraint> constraint;
    // IdList<Request, hRequest> request;
    // IdList<Style, hStyle> style;

    // These are generated from the above.
    IdList<ENTITY, hEntity> entity;
    IdList<Param, hParam> param;

    Group* activeGroup;

    inline CONSTRAINT *GetConstraint(hConstraint h) {
        return constraint.FindById(h);
    }

    inline ENTITY *GetEntity(hEntity h) {
        return entity.FindById(h);
    }

    inline Param *GetParam(hParam h) {
        return param.FindById(h);
    }

    // inline Request *GetRequest(hRequest h) {
    //     return request.FindById(h);
    // }

    inline Group *GetGroup(hGroup h) {
        return group.FindById(h);
    }

    inline std::vector<Param> GetParams() {
        std::vector<Param> params;
        for (Param &p : param) {
            params.push_back(p);
        }
        return params;
    }

    inline std::vector<double> GetParams(std::vector<hParam> phs) {
        std::vector<double> params;
        for (hParam &ph : phs) {
            Param *par = param.FindById(ph);
            params.push_back(par->val);
        }
        return params;
    }

    inline std::vector<ENTITY> GetEntities() {
        std::vector<ENTITY> entities;
        for (ENTITY &e : entity) {
            entities.push_back(e);
        }
        return entities;
    }

    inline std::vector<CONSTRAINT> GetConstraints() {
        std::vector<CONSTRAINT> constraints;
        for (CONSTRAINT &c : constraint) {
            constraints.push_back(c);
        }
        return constraints;
    }

    // group

    inline void AddGroup(Group *g) {
        group.AddAndAssignId(g);
    }

    inline void SetActiveGroup(Group *g) {
        activeGroup = g;
    }

    // param

    inline Param AddParam(double val) {
        Param pa = {};
        pa.val = val;
        param.AddAndAssignId(&pa);
        return pa;
    }

    // entities

    inline ENTITY AddPoint2D(double u, double v, ENTITY workplane)
    {
        if(!activeGroup) {
            throw std::invalid_argument("missing active group! aborting...");
        }
        Param up = AddParam(u);
        Param vp = AddParam(v);
        ENTITY e = {};
        e.type = ENTITY::Type::POINT_IN_2D;
        e.group.v     = activeGroup->h.v;
        e.workplane.v = workplane.h.v;
        e.param[0].v  = up.h.v;
        e.param[1].v  = vp.h.v;
        entity.AddAndAssignId(&e);
        return e;
    }

    inline ENTITY AddPoint3D(double x, double y, double z)
    {
        if(!activeGroup) {
            throw std::invalid_argument("missing active group! aborting...");
        }
        Param xp = AddParam(x);
        Param yp = AddParam(y);
        Param zp = AddParam(z);
        ENTITY e = {};
        e.type = ENTITY::Type::POINT_IN_3D;
        e.group.v     = activeGroup->h.v;
        e.workplane.v = ENTITY::_FREE_IN_3D.h.v;
        e.param[0].v  = xp.h.v;
        e.param[1].v  = yp.h.v;
        e.param[2].v  = zp.h.v;
        entity.AddAndAssignId(&e);
        return e;
    }

    inline ENTITY AddNormal2D(ENTITY workplane) {
        if(!activeGroup) {
            throw std::invalid_argument("missing active group! aborting...");
        } else if(!workplane.IsWorkplane()) {
            throw std::invalid_argument("workplane argument is not a workplane");
        }
        ENTITY e      = {};
        e.type        = ENTITY::Type::NORMAL_IN_2D;
        e.group.v     = activeGroup->h.v;
        e.workplane.v = workplane.h.v;
        entity.AddAndAssignId(&e);
        return e;
    }

    inline ENTITY AddNormal3D(double qw, double qx, double qy, double qz) {
        if(!activeGroup) {
            throw std::invalid_argument("missing active group! aborting...");
        }
        Param wp      = AddParam(qw);
        Param xp      = AddParam(qx);
        Param yp      = AddParam(qy);
        Param zp      = AddParam(qz);
        ENTITY e      = {};
        e.type        = ENTITY::Type::NORMAL_IN_3D;
        e.group.v     = activeGroup->h.v;
        e.workplane.v = ENTITY::_FREE_IN_3D.h.v;
        e.param[0].v  = wp.h.v;
        e.param[1].v  = xp.h.v;
        e.param[2].v  = yp.h.v;
        e.param[3].v  = zp.h.v;
        entity.AddAndAssignId(&e);
        return e;
    }

    inline ENTITY AddDistance(double value, ENTITY workplane) {
        if(!activeGroup) {
            throw std::invalid_argument("missing active group! aborting...");
        } else if(!workplane.IsWorkplane()) {
            throw std::invalid_argument("workplane argument is not a workplane");
        }
        Param valuep  = AddParam(value);
        ENTITY e      = {};
        e.type        = ENTITY::Type::DISTANCE;
        e.group.v     = activeGroup->h.v;
        e.workplane.v = workplane.h.v;
        e.param[0].v  = valuep.h.v;
        entity.AddAndAssignId(&e);
        return e;
    }

    inline ENTITY AddLine2D(ENTITY ptA, ENTITY ptB, ENTITY workplane) {
        if(!activeGroup) {
            throw std::invalid_argument("missing active group! aborting...");
        } else if(!workplane.IsWorkplane()) {
            throw std::invalid_argument("workplane argument is not a workplane");
        } else if(!ptA.IsPoint2D()) {
            throw std::invalid_argument("ptA argument is not a 2d point");
        } else if(!ptB.IsPoint2D()) {
            throw std::invalid_argument("ptB argument is not a 2d point");
        }
        ENTITY e      = {};
        e.type        = ENTITY::Type::LINE_SEGMENT;
        e.group.v     = activeGroup->h.v;
        e.workplane.v = workplane.h.v;
        e.point[0].v  = ptA.h.v;
        e.point[1].v  = ptB.h.v;
        entity.AddAndAssignId(&e);
        return e;
    }

    inline ENTITY AddLine3D(ENTITY ptA, ENTITY ptB) {
        if(!activeGroup) {
            throw std::invalid_argument("missing active group! aborting...");
        } else if(!ptA.IsPoint3D()) {
            throw std::invalid_argument("ptA argument is not a 3d point");
        } else if(!ptB.IsPoint3D()) {
            throw std::invalid_argument("ptB argument is not a 3d point");
        }
        ENTITY e      = {};
        e.type        = ENTITY::Type::LINE_SEGMENT;
        e.group.v     = activeGroup->h.v;
        e.workplane.v = ENTITY::_FREE_IN_3D.h.v;
        e.point[0].v  = ptA.h.v;
        e.point[1].v  = ptB.h.v;
        entity.AddAndAssignId(&e);
        return e;
    }

    inline ENTITY AddCubic(ENTITY ptA, ENTITY ptB, ENTITY ptC, ENTITY ptD, ENTITY workplane) {
        if(!activeGroup) {
            throw std::invalid_argument("missing active group! aborting...");
        } else if(!workplane.IsWorkplane()) {
            throw std::invalid_argument("workplane argument is not a workplane");
        } else if(!ptA.IsPoint2D()) {
            throw std::invalid_argument("ptA argument is not a 2d point");
        } else if(!ptB.IsPoint2D()) {
            throw std::invalid_argument("ptB argument is not a 2d point");
        } else if(!ptC.IsPoint2D()) {
            throw std::invalid_argument("ptC argument is not a 2d point");
        } else if(!ptD.IsPoint2D()) {
            throw std::invalid_argument("ptD argument is not a 2d point");
        }
        ENTITY e      = {};
        e.type        = ENTITY::Type::CUBIC;
        e.group.v     = activeGroup->h.v;
        e.workplane.v = workplane.h.v;
        e.point[0].v  = ptA.h.v;
        e.point[1].v  = ptB.h.v;
        e.point[2].v  = ptC.h.v;
        e.point[3].v  = ptD.h.v;
        entity.AddAndAssignId(&e);
        return e;
    }

    inline ENTITY AddArc(ENTITY normal, ENTITY center, ENTITY start, ENTITY end, ENTITY workplane) {
        if(!activeGroup) {
            throw std::invalid_argument("missing active group! aborting...");
        } else if(!workplane.IsWorkplane()) {
            throw std::invalid_argument("workplane argument is not a workplane");
        } else if(!normal.IsNormal3D()) {
            throw std::invalid_argument("normal argument is not a 3d normal");
        } else if(!center.IsPoint2D()) {
            throw std::invalid_argument("center argument is not a 2d point");
        } else if(!start.IsPoint2D()) {
            throw std::invalid_argument("start argument is not a 2d point");
        } else if(!end.IsPoint2D()) {
            throw std::invalid_argument("end argument is not a 2d point");
        }
        ENTITY e      = {};
        e.type        = ENTITY::Type::ARC_OF_CIRCLE;
        e.group.v     = activeGroup->h.v;
        e.workplane.v = workplane.h.v;
        e.normal.v    = normal.h.v;
        e.point[0].v  = center.h.v;
        e.point[1].v  = start.h.v;
        e.point[2].v  = end.h.v;
        entity.AddAndAssignId(&e);
        return e;
    }

    inline ENTITY AddCircle(ENTITY normal, ENTITY center, ENTITY radius, ENTITY workplane) {
        if(!activeGroup) {
            throw std::invalid_argument("missing active group! aborting...");
        } else if(!workplane.IsWorkplane()) {
            throw std::invalid_argument("workplane argument is not a workplane");
        } else if(!normal.IsNormal3D()) {
            throw std::invalid_argument("normal argument is not a 3d normal");
        } else if(!center.IsPoint2D()) {
            throw std::invalid_argument("center argument is not a 2d point");
        } else if(!radius.IsDistance()) {
            throw std::invalid_argument("radius argument is not a distance");
        }
        ENTITY e      = {};
        e.type        = ENTITY::Type::CIRCLE;
        e.group.v     = activeGroup->h.v;
        e.workplane.v = workplane.h.v;
        e.normal.v    = normal.h.v;
        e.point[0].v  = center.h.v;
        e.distance.v  = radius.h.v;
        entity.AddAndAssignId(&e);
        return e;
    }

    inline ENTITY AddWorkplane(ENTITY origin, ENTITY nm) {
        if(!activeGroup) {
            throw std::invalid_argument("missing active group! aborting...");
        }
        ENTITY e      = {};
        e.type        = ENTITY::Type::WORKPLANE;
        e.group.v     = activeGroup->h.v;
        e.workplane.v = ENTITY::FREE_IN_3D.v;
        e.point[0].v  = origin.h.v;
        e.normal.v    = nm.h.v;
        entity.AddAndAssignId(&e);
        return e;
    }

    inline ENTITY Create2DBase() {
        if(!activeGroup) {
            throw std::invalid_argument("missing active group! aborting...");
        }
        Vector u     = Vector::From(1, 0, 0);
        Vector v     = Vector::From(0, 1, 0);
        Quaternion q = Quaternion::From(u, v);
        ENTITY nm   = AddNormal3D(q.w, q.vx, q.vy, q.vz);
        return AddWorkplane(AddPoint3D(0, 0, 0), nm);
    }

    // constraints

    inline CONSTRAINT
    AddConstraint(typename CONSTRAINT::Type type, ENTITY workplane, double val, ENTITY ptA,
                   ENTITY ptB = ENTITY::_NO_ENTITY, ENTITY entityA = ENTITY::_NO_ENTITY,
                   ENTITY entityB = ENTITY::_NO_ENTITY, ENTITY entityC = ENTITY::_NO_ENTITY,
                   ENTITY entityD = ENTITY::_NO_ENTITY, int other = 0, int other2 = 0) {
        if(!activeGroup) {
            throw std::invalid_argument("missing active group! aborting...");
        }
        CONSTRAINT c  = {};
        c.type        = type;
        c.group.v     = activeGroup->h.v;
        c.workplane.v = workplane.h.v;
        c.valA        = val;
        c.ptA.v       = ptA.h.v;
        c.ptB.v       = ptB.h.v;
        c.entityA.v   = entityA.h.v;
        c.entityB.v   = entityB.h.v;
        c.entityC.v   = entityC.h.v;
        c.entityD.v   = entityD.h.v;
        c.other       = other ? true : false;
        c.other2      = other2 ? true : false;
        constraint.AddAndAssignId(&c);
        // c.Generate(&param);
        return c;
    }

    inline CONSTRAINT Coincident(ENTITY entityA,
                                 ENTITY entityB, ENTITY workplane = ENTITY::_FREE_IN_3D) {
        if(entityA.IsPoint() && entityB.IsPoint()) {
            return AddConstraint(CONSTRAINT::Type::POINTS_COINCIDENT, workplane, 0., entityA,
                                 entityB);
        } else if(entityA.IsPoint() && entityB.IsWorkplane()) {
            return AddConstraint(CONSTRAINT::Type::PT_IN_PLANE, ENTITY::_FREE_IN_3D, 0., entityA, ENTITY::_NO_ENTITY,
                                 entityB);
        } else if(entityA.IsPoint() && entityB.IsLine()) {
            return AddConstraint(CONSTRAINT::Type::PT_ON_LINE, ENTITY::_FREE_IN_3D, 0., entityA, ENTITY::_NO_ENTITY,
                                 entityB);
        } else if(entityA.IsPoint() && (entityB.IsCircle() || entityB.IsArc())) {
            return AddConstraint(CONSTRAINT::Type::PT_ON_CIRCLE, ENTITY::_FREE_IN_3D, 0., entityA, ENTITY::_NO_ENTITY,
                                 entityB);
        }
        throw std::invalid_argument("Invalid arguments for coincident constraint");
    }

    inline CONSTRAINT Distance(ENTITY entityA, ENTITY entityB, double value, ENTITY workplane) {
        if(entityA.IsPoint() && entityB.IsPoint()) {
            return AddConstraint(CONSTRAINT::Type::PT_PT_DISTANCE, workplane, value, entityA,
                                 entityB);
        } else if(entityA.IsPoint() && entityB.IsWorkplane() && workplane.Is3D()) {
            return AddConstraint(CONSTRAINT::Type::PT_PLANE_DISTANCE, entityB, value, entityA,
                                 ENTITY::_NO_ENTITY, entityB);
        } else if(entityA.IsPoint() && entityB.IsLine()) {
            return AddConstraint(CONSTRAINT::Type::PT_LINE_DISTANCE, workplane, value, entityA,
                                 ENTITY::_NO_ENTITY, entityB);
        }
        throw std::invalid_argument("Invalid arguments for distance constraint");
    }

    inline CONSTRAINT Equal(ENTITY entityA, ENTITY entityB, ENTITY workplane = ENTITY::_FREE_IN_3D) {
        if(entityA.IsLine() && entityB.IsLine()) {
            return AddConstraint(CONSTRAINT::Type::EQUAL_LENGTH_LINES, workplane, 0.,
                                 ENTITY::_NO_ENTITY, ENTITY::_NO_ENTITY, entityA, entityB);
        } else if(entityA.IsLine() && (entityB.IsArc() || entityB.IsCircle())) {
            return AddConstraint(CONSTRAINT::Type::EQUAL_LINE_ARC_LEN, workplane, 0.,
                                 ENTITY::_NO_ENTITY, ENTITY::_NO_ENTITY, entityA, entityB);
        } else if((entityA.IsArc() || entityA.IsCircle()) && (entityB.IsArc() || entityB.IsCircle())) {
            return AddConstraint(CONSTRAINT::Type::EQUAL_RADIUS, workplane, 0., ENTITY::_NO_ENTITY,
                                 ENTITY::_NO_ENTITY, entityA, entityB);
        }
        throw std::invalid_argument("Invalid arguments for equal constraint");
    }

    inline CONSTRAINT EqualAngle(ENTITY entityA, ENTITY entityB, ENTITY entityC, ENTITY entityD,
                                 ENTITY workplane = ENTITY::_FREE_IN_3D) {
        if(entityA.IsLine2D() && entityB.IsLine2D() && entityC.IsLine2D() && entityD.IsLine2D()) {
            return AddConstraint(CONSTRAINT::Type::EQUAL_ANGLE, workplane, 0.,
                                 ENTITY::_NO_ENTITY, ENTITY::_NO_ENTITY, entityA, entityB, entityC, entityD);
        }
        throw std::invalid_argument("Invalid arguments for equal angle constraint");
    }

    inline CONSTRAINT EqualPointToLine(ENTITY entityA, ENTITY entityB, ENTITY entityC, ENTITY entityD,
                                 ENTITY workplane = ENTITY::_FREE_IN_3D) {
        if(entityA.IsPoint2D() && entityB.IsLine2D() && entityC.IsPoint2D() && entityD.IsLine2D()) {
            return AddConstraint(CONSTRAINT::Type::EQ_PT_LN_DISTANCES, workplane, 0., entityA, entityB, entityC, entityD);
        }
        throw std::invalid_argument("Invalid arguments for equal point to line constraint");
    }

    inline CONSTRAINT Ratio(ENTITY entityA, ENTITY entityB, double value, ENTITY workplane = ENTITY::_FREE_IN_3D) {
        if(entityA.IsLine2D() && entityB.IsPoint2D()) {
            return AddConstraint(CONSTRAINT::Type::LENGTH_RATIO, workplane, value,
                                 ENTITY::_NO_ENTITY, ENTITY::_NO_ENTITY, entityA, entityB);
        }
        throw std::invalid_argument("Invalid arguments for ratio constraint");
    }

    inline CONSTRAINT Symmetric(ENTITY entityA, ENTITY entityB, ENTITY entityC = ENTITY::_NO_ENTITY,
                                ENTITY workplane = ENTITY::_FREE_IN_3D) {
        if(entityA.IsPoint3D() && entityB.IsPoint3D() && entityC.IsWorkplane() && workplane.IsFreeIn3D()) {
            return AddConstraint(CONSTRAINT::Type::SYMMETRIC, workplane, 0., entityA, entityB, entityC);
        } else if(entityA.IsPoint2D() && entityB.IsPoint2D() && entityC.IsWorkplane() && workplane.IsFreeIn3D()) {
            return AddConstraint(CONSTRAINT::Type::SYMMETRIC, entityC, 0., entityA, entityB, entityC);
        } else if(entityA.IsPoint2D() && entityB.IsPoint2D() && entityC.IsLine()) {
            if (workplane.IsFreeIn3D()) {
                throw std::invalid_argument("3d workplane given for a 2d constraint");
            }
            return AddConstraint(CONSTRAINT::Type::SYMMETRIC_LINE, workplane, 0., entityA, entityB, entityC);
        }
        throw std::invalid_argument("Invalid arguments for symmetric constraint");
    }

    inline CONSTRAINT SymmetricH(ENTITY ptA, ENTITY ptB, ENTITY workplane = ENTITY::_FREE_IN_3D) {
        if (workplane.IsFreeIn3D()) {
            throw std::invalid_argument("3d workplane given for a 2d constraint");
        } else if(ptA.IsPoint2D() && ptB.IsPoint2D()) {
            return AddConstraint(CONSTRAINT::Type::SYMMETRIC_HORIZ, workplane, 0., ptA, ptB);
        }
        throw std::invalid_argument("Invalid arguments for symmetric horizontal constraint");
    }

    inline CONSTRAINT SymmetricV(ENTITY ptA, ENTITY ptB, ENTITY workplane = ENTITY::_FREE_IN_3D) {
        if (workplane.IsFreeIn3D()) {
            throw std::invalid_argument("3d workplane given for a 2d constraint");
        } else if(ptA.IsPoint2D() && ptB.IsPoint2D()) {
            return AddConstraint(CONSTRAINT::Type::SYMMETRIC_VERT, workplane, 0., ptA, ptB);
        }
        throw std::invalid_argument("Invalid arguments for symmetric vertical constraint");
    }

    inline CONSTRAINT Midpoint(ENTITY ptA, ENTITY ptB, ENTITY workplane = ENTITY::_FREE_IN_3D) {
        if(ptA.IsPoint() && ptB.IsLine()) {
            return AddConstraint(CONSTRAINT::Type::AT_MIDPOINT, workplane, 0., ptA, ENTITY::_NO_ENTITY, ptB);
        }
        throw std::invalid_argument("Invalid arguments for midpoint constraint");
    }

    inline CONSTRAINT Horizontal(ENTITY entityA, ENTITY workplane, ENTITY entityB = ENTITY::_NO_ENTITY)
    {
        if (workplane.IsFreeIn3D()) {
            throw std::invalid_argument("Horizontal constraint is not supported in 3D");
        } else if (entityA.IsLine2D()) {
            return AddConstraint(CONSTRAINT::Type::HORIZONTAL, workplane, 0., ENTITY::_NO_ENTITY, ENTITY::_NO_ENTITY, entityA);
        } else if (entityA.IsPoint2D() && entityB.IsPoint2D()) {
            return AddConstraint(CONSTRAINT::Type::HORIZONTAL, workplane, 0., entityA, entityB);
        }
        throw std::invalid_argument("Invalid arguments for horizontal constraint");
    }

    inline CONSTRAINT Vertical(ENTITY entityA, ENTITY workplane, ENTITY entityB = ENTITY::_NO_ENTITY)
    {
        if (workplane.IsFreeIn3D()) {
            throw std::invalid_argument("Vertical constraint is not supported in 3D");
        } else if (entityA.IsLine2D()) {
            return AddConstraint(CONSTRAINT::Type::VERTICAL, workplane, 0., ENTITY::_NO_ENTITY, ENTITY::_NO_ENTITY, entityA);
        } else if (entityA.IsPoint2D() && entityB.IsPoint2D()) {
            return AddConstraint(CONSTRAINT::Type::VERTICAL, workplane, 0., entityA, entityB);
        }
        throw std::invalid_argument("Invalid arguments for horizontal constraint");
    }

    inline CONSTRAINT Diameter(ENTITY entityA, double value) {
        if(entityA.IsArc() || entityA.IsLine()) {
            return AddConstraint(CONSTRAINT::Type::DIAMETER, ENTITY::_FREE_IN_3D, value,
                                 ENTITY::_NO_ENTITY, ENTITY::_NO_ENTITY, entityA);
        }
        throw std::invalid_argument("Invalid arguments for diameter constraint");
    }

    inline CONSTRAINT SameOrientation(ENTITY entityA, ENTITY entityB) {
        if(entityA.IsNormal3D() && entityB.IsNormal3D()) {
            return AddConstraint(CONSTRAINT::Type::SAME_ORIENTATION, ENTITY::_FREE_IN_3D, 0., ENTITY::_NO_ENTITY,
                                 ENTITY::_NO_ENTITY, entityA, entityB);
        }
        throw std::invalid_argument("Invalid arguments for same orientation constraint");
    }

    inline CONSTRAINT Angle(ENTITY entityA, ENTITY entityB, double value,
                            ENTITY workplane = ENTITY::_FREE_IN_3D, bool inverse = false) {
        if(entityA.IsLine2D() && entityB.IsLine2D()) {
            return AddConstraint(CONSTRAINT::Type::ANGLE, workplane, value, ENTITY::_NO_ENTITY,
                                 ENTITY::_NO_ENTITY, entityA, entityB, ENTITY::_NO_ENTITY,
                                 ENTITY::_NO_ENTITY, inverse);
        }
        throw std::invalid_argument("Invalid arguments for angle constraint");
    }

    inline CONSTRAINT Perpendicular(ENTITY entityA, ENTITY entityB, ENTITY workplane = ENTITY::_FREE_IN_3D, bool inverse = false) {
        if(entityA.IsLine2D() && entityB.IsLine2D()) {
            return AddConstraint(CONSTRAINT::Type::PERPENDICULAR, workplane, 0.,
                                 ENTITY::_NO_ENTITY, ENTITY::_NO_ENTITY, entityA, entityB,
                                 ENTITY::_NO_ENTITY, ENTITY::_NO_ENTITY, inverse);
        }
        throw std::invalid_argument("Invalid arguments for perpendicular constraint");
    }

    inline CONSTRAINT Parallel(ENTITY entityA, ENTITY entityB, ENTITY workplane = ENTITY::_FREE_IN_3D) {
        if(entityA.IsLine2D() && entityB.IsLine2D()) {
            return AddConstraint(CONSTRAINT::Type::PARALLEL, workplane, 0.,
                                 ENTITY::_NO_ENTITY, ENTITY::_NO_ENTITY, entityA, entityB);
        }
        throw std::invalid_argument("Invalid arguments for parallel constraint");
    }

    inline CONSTRAINT Tangent(ENTITY entityA, ENTITY entityB, ENTITY workplane = ENTITY::_FREE_IN_3D) {
        if(entityA.IsArc() && entityB.IsLine2D()) {
            if(workplane.IsFreeIn3D()) {
                throw std::invalid_argument("3d workplane given for a 2d constraint");
            }
            Vector a1 = GetEntity(entityA.point[1])->PointGetNum(),
                   a2 = GetEntity(entityA.point[2])->PointGetNum();
            Vector l0 = GetEntity(entityB.point[0])->PointGetNum(),
                   l1 = GetEntity(entityB.point[1])->PointGetNum();
            bool other;
            if(l0.Equals(a1) || l1.Equals(a1)) {
                other = false;
            } else if(l0.Equals(a2) || l1.Equals(a2)) {
                other = true;
            } else {
                throw std::invalid_argument("The tangent arc and line segment must share an "
                                            "endpoint. Constrain them with Constrain -> "
                                            "On Point before constraining tangent.");
            }
            return AddConstraint(CONSTRAINT::Type::ARC_LINE_TANGENT, workplane, 0.,
                                 ENTITY::_NO_ENTITY, ENTITY::_NO_ENTITY, entityA, entityB,
                                 ENTITY::_NO_ENTITY, ENTITY::_NO_ENTITY, other);
        } else if(entityA.IsCubic() && entityB.IsLine2D() && workplane.IsFreeIn3D()) {
            Vector as = entityA.CubicGetStartNum(), af = entityA.CubicGetFinishNum();
            Vector l0 = GetEntity(entityB.point[0])->PointGetNum(),
                   l1 = GetEntity(entityB.point[1])->PointGetNum();
            bool other;
            if(l0.Equals(as) || l1.Equals(as)) {
                other = false;
            } else if(l0.Equals(af) || l1.Equals(af)) {
                other = true;
            } else {
                throw std::invalid_argument("The tangent cubic and line segment must share an "
                        "endpoint. Constrain them with Constrain -> "
                        "On Point before constraining tangent.");
            }
            return AddConstraint(CONSTRAINT::Type::CUBIC_LINE_TANGENT, workplane, 0.,
                                 ENTITY::_NO_ENTITY, ENTITY::_NO_ENTITY, entityA, entityB,
                                 ENTITY::_NO_ENTITY, ENTITY::_NO_ENTITY, other);
        } else if((entityA.IsArc() || entityA.IsCubic()) && (entityB.IsArc() || entityB.IsCubic())) {
            if(workplane.IsFreeIn3D()) {
                throw std::invalid_argument("3d workplane given for a 2d constraint");
            }
            Vector as = entityA.EndpointStart(), af = entityA.EndpointFinish(), bs = entityB.EndpointStart(),
                   bf = entityB.EndpointFinish();
            bool other;
            bool other2;
            if(as.Equals(bs)) {
                other  = false;
                other2 = false;
            } else if(as.Equals(bf)) {
                other  = false;
                other2 = true;
            } else if(af.Equals(bs)) {
                other  = true;
                other2 = false;
            } else if(af.Equals(bf)) {
                other  = true;
                other2 = true;
            } else {
                throw std::invalid_argument("The curves must share an endpoint. Constrain them "
                        "with Constrain -> On Point before constraining "
                        "tangent.");
            }
            return AddConstraint(CONSTRAINT::Type::CURVE_CURVE_TANGENT, workplane, 0.,
                                 ENTITY::_NO_ENTITY, ENTITY::_NO_ENTITY, entityA, entityB,
                                 ENTITY::_NO_ENTITY, ENTITY::_NO_ENTITY, other, other2);
        }
        throw std::invalid_argument("Invalid arguments for tangent constraint");
    }

    inline CONSTRAINT DistanceProj(ENTITY ptA, ENTITY ptB, double value) {
        if(ptA.IsPoint() && ptB.IsPoint()) {
            return AddConstraint(CONSTRAINT::Type::PROJ_PT_DISTANCE, ENTITY::_FREE_IN_3D, value, ptA, ptB);
        }
        throw std::invalid_argument("Invalid arguments for projected distance constraint");
    }

    inline CONSTRAINT LengthDiff(ENTITY entityA, ENTITY entityB, double value,
                                 ENTITY workplane = ENTITY::_FREE_IN_3D) {
        if(entityA.IsLine() && entityB.IsLine()) {
            return AddConstraint(CONSTRAINT::Type::LENGTH_DIFFERENCE, workplane, value,
                                 ENTITY::_NO_ENTITY, ENTITY::_NO_ENTITY, entityA, entityB);
        }
        throw std::invalid_argument("Invalid arguments for length difference constraint");
    }

    inline CONSTRAINT Dragged(ENTITY ptA, ENTITY workplane = ENTITY::_FREE_IN_3D) {
        if(ptA.IsPoint()) {
            return AddConstraint(CONSTRAINT::Type::WHERE_DRAGGED, workplane, 0., ptA);
        }
        throw std::invalid_argument("Invalid arguments for dragged constraint");
    }

    inline void Clear() {
        group.Clear();
        // groupOrder.Clear();
        // request.Clear();
        // style.Clear();
        constraint.Clear();
        entity.Clear();
        param.Clear();
    }

    // BBox CalculateEntityBBox(bool includingInvisible);
    // Group *GetRunningMeshGroupFor(hGroup h);
};

#endif
