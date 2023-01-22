#ifndef SOLVESPACE_SKETCH_H
#define SOLVESPACE_SKETCH_H

#include <stdexcept>
#include "data.h"
#include "idlist.h"
#include "group.h"
#include "vector.h"
#include "quaternion.h"

template<class ConstraintBase, class EntityBase>
class Sketch {
public:
    // These are user-editable, and define the sketch.
    IdList<Group, hGroup> group;
    // List<hGroup> groupOrder;
    IdList<ConstraintBase, hConstraint> constraint;
    // IdList<Request, hRequest> request;
    // IdList<Style, hStyle> style;

    // These are generated from the above.
    IdList<EntityBase, hEntity> entity;
    IdList<Param, hParam> param;

    Group *activeGroup;

    inline ConstraintBase *GetConstraint(hConstraint h) {
        return constraint.FindById(h);
    }

    inline EntityBase *GetEntity(hEntity h) {
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
        for(Param &p : param) {
            params.push_back(p);
        }
        return params;
    }

    inline std::vector<double> GetParams(std::vector<hParam> phs) {
        std::vector<double> params;
        for(hParam &ph : phs) {
            Param *par = param.FindById(ph);
            params.push_back(par->val);
        }
        return params;
    }

    inline std::vector<EntityBase> GetEntities() {
        std::vector<EntityBase> entities;
        for(EntityBase &e : entity) {
            entities.push_back(e);
        }
        return entities;
    }

    inline std::vector<ConstraintBase> GetConstraints() {
        std::vector<ConstraintBase> constraints;
        for(ConstraintBase &c : constraint) {
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

    inline Group* GetActiveGroup() {
        return activeGroup;
    }

    // param

    inline Param AddParam(double val) {
        Param pa = {};
        pa.val   = val;
        param.AddAndAssignId(&pa);
        return pa;
    }

    // entities

    inline EntityBase AddPoint2D(double u, double v, EntityBase workplane) {
        if(!activeGroup) {
            throw std::invalid_argument("missing active group! aborting...");
        }
        Param up      = AddParam(u);
        Param vp      = AddParam(v);
        EntityBase e  = {};
        e.type        = EntityBase::Type::POINT_IN_2D;
        e.group.v     = activeGroup->h.v;
        e.workplane.v = workplane.h.v;
        e.param[0].v  = up.h.v;
        e.param[1].v  = vp.h.v;
        entity.AddAndAssignId(&e);
        return e;
    }

    inline EntityBase AddPoint3D(double x, double y, double z) {
        if(!activeGroup) {
            throw std::invalid_argument("missing active group! aborting...");
        }
        Param xp      = AddParam(x);
        Param yp      = AddParam(y);
        Param zp      = AddParam(z);
        EntityBase e  = {};
        e.type        = EntityBase::Type::POINT_IN_3D;
        e.group.v     = activeGroup->h.v;
        e.workplane.v = EntityBase::_FREE_IN_3D.h.v;
        e.param[0].v  = xp.h.v;
        e.param[1].v  = yp.h.v;
        e.param[2].v  = zp.h.v;
        entity.AddAndAssignId(&e);
        return e;
    }

    inline EntityBase AddNormal2D(EntityBase workplane) {
        if(!activeGroup) {
            throw std::invalid_argument("missing active group! aborting...");
        } else if(!workplane.IsWorkplane()) {
            throw std::invalid_argument("workplane argument is not a workplane");
        }
        EntityBase e  = {};
        e.type        = EntityBase::Type::NORMAL_IN_2D;
        e.group.v     = activeGroup->h.v;
        e.workplane.v = workplane.h.v;
        entity.AddAndAssignId(&e);
        return e;
    }

    inline EntityBase AddNormal3D(double qw, double qx, double qy, double qz) {
        if(!activeGroup) {
            throw std::invalid_argument("missing active group! aborting...");
        }
        Param wp      = AddParam(qw);
        Param xp      = AddParam(qx);
        Param yp      = AddParam(qy);
        Param zp      = AddParam(qz);
        EntityBase e  = {};
        e.type        = EntityBase::Type::NORMAL_IN_3D;
        e.group.v     = activeGroup->h.v;
        e.workplane.v = EntityBase::_FREE_IN_3D.h.v;
        e.param[0].v  = wp.h.v;
        e.param[1].v  = xp.h.v;
        e.param[2].v  = yp.h.v;
        e.param[3].v  = zp.h.v;
        entity.AddAndAssignId(&e);
        return e;
    }

    inline EntityBase AddDistance(double value, EntityBase workplane) {
        if(!activeGroup) {
            throw std::invalid_argument("missing active group! aborting...");
        } else if(!workplane.IsWorkplane()) {
            throw std::invalid_argument("workplane argument is not a workplane");
        }
        Param valuep  = AddParam(value);
        EntityBase e  = {};
        e.type        = EntityBase::Type::DISTANCE;
        e.group.v     = activeGroup->h.v;
        e.workplane.v = workplane.h.v;
        e.param[0].v  = valuep.h.v;
        entity.AddAndAssignId(&e);
        return e;
    }

    inline EntityBase AddLine2D(EntityBase ptA, EntityBase ptB, EntityBase workplane) {
        if(!activeGroup) {
            throw std::invalid_argument("missing active group! aborting...");
        } else if(!workplane.IsWorkplane()) {
            throw std::invalid_argument("workplane argument is not a workplane");
        } else if(!ptA.IsPoint2D()) {
            throw std::invalid_argument("ptA argument is not a 2d point");
        } else if(!ptB.IsPoint2D()) {
            throw std::invalid_argument("ptB argument is not a 2d point");
        }
        EntityBase e  = {};
        e.type        = EntityBase::Type::LINE_SEGMENT;
        e.group.v     = activeGroup->h.v;
        e.workplane.v = workplane.h.v;
        e.point[0].v  = ptA.h.v;
        e.point[1].v  = ptB.h.v;
        entity.AddAndAssignId(&e);
        return e;
    }

    inline EntityBase AddLine3D(EntityBase ptA, EntityBase ptB) {
        if(!activeGroup) {
            throw std::invalid_argument("missing active group! aborting...");
        } else if(!ptA.IsPoint3D()) {
            throw std::invalid_argument("ptA argument is not a 3d point");
        } else if(!ptB.IsPoint3D()) {
            throw std::invalid_argument("ptB argument is not a 3d point");
        }
        EntityBase e  = {};
        e.type        = EntityBase::Type::LINE_SEGMENT;
        e.group.v     = activeGroup->h.v;
        e.workplane.v = EntityBase::_FREE_IN_3D.h.v;
        e.point[0].v  = ptA.h.v;
        e.point[1].v  = ptB.h.v;
        entity.AddAndAssignId(&e);
        return e;
    }

    inline EntityBase AddCubic(EntityBase ptA, EntityBase ptB, EntityBase ptC, EntityBase ptD,
                               EntityBase workplane) {
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
        EntityBase e  = {};
        e.type        = EntityBase::Type::CUBIC;
        e.group.v     = activeGroup->h.v;
        e.workplane.v = workplane.h.v;
        e.point[0].v  = ptA.h.v;
        e.point[1].v  = ptB.h.v;
        e.point[2].v  = ptC.h.v;
        e.point[3].v  = ptD.h.v;
        entity.AddAndAssignId(&e);
        return e;
    }

    inline EntityBase AddArc(EntityBase normal, EntityBase center, EntityBase start, EntityBase end,
                             EntityBase workplane) {
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
        EntityBase e  = {};
        e.type        = EntityBase::Type::ARC_OF_CIRCLE;
        e.group.v     = activeGroup->h.v;
        e.workplane.v = workplane.h.v;
        e.normal.v    = normal.h.v;
        e.point[0].v  = center.h.v;
        e.point[1].v  = start.h.v;
        e.point[2].v  = end.h.v;
        entity.AddAndAssignId(&e);
        return e;
    }

    inline EntityBase AddCircle(EntityBase normal, EntityBase center, EntityBase radius,
                                EntityBase workplane) {
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
        EntityBase e  = {};
        e.type        = EntityBase::Type::CIRCLE;
        e.group.v     = activeGroup->h.v;
        e.workplane.v = workplane.h.v;
        e.normal.v    = normal.h.v;
        e.point[0].v  = center.h.v;
        e.distance.v  = radius.h.v;
        entity.AddAndAssignId(&e);
        return e;
    }

    inline EntityBase AddWorkplane(EntityBase origin, EntityBase nm) {
        if(!activeGroup) {
            throw std::invalid_argument("missing active group! aborting...");
        }
        EntityBase e  = {};
        e.type        = EntityBase::Type::WORKPLANE;
        e.group.v     = activeGroup->h.v;
        e.workplane.v = EntityBase::_FREE_IN_3D.h.v;
        e.point[0].v  = origin.h.v;
        e.normal.v    = nm.h.v;
        entity.AddAndAssignId(&e);
        return e;
    }

    inline EntityBase Create2DBase() {
        if(!activeGroup) {
            throw std::invalid_argument("missing active group! aborting...");
        }
        Vector u      = Vector::From(1, 0, 0);
        Vector v      = Vector::From(0, 1, 0);
        Quaternion q  = Quaternion::From(u, v);
        EntityBase nm = AddNormal3D(q.w, q.vx, q.vy, q.vz);
        return AddWorkplane(AddPoint3D(0, 0, 0), nm);
    }

    // constraints

    inline ConstraintBase AddConstraint(
        typename ConstraintBase::Type type, EntityBase workplane, double val, EntityBase ptA,
        EntityBase ptB = EntityBase::_NO_ENTITY, EntityBase entityA = EntityBase::_NO_ENTITY,
        EntityBase entityB = EntityBase::_NO_ENTITY, EntityBase entityC = EntityBase::_NO_ENTITY,
        EntityBase entityD = EntityBase::_NO_ENTITY, int other = 0, int other2 = 0) {
        if(!activeGroup) {
            throw std::invalid_argument("missing active group! aborting...");
        }
        ConstraintBase c = {};
        c.type           = type;
        c.group.v        = activeGroup->h.v;
        c.workplane.v    = workplane.h.v;
        c.valA           = val;
        c.ptA.v          = ptA.h.v;
        c.ptB.v          = ptB.h.v;
        c.entityA.v      = entityA.h.v;
        c.entityB.v      = entityB.h.v;
        c.entityC.v      = entityC.h.v;
        c.entityD.v      = entityD.h.v;
        c.other          = other ? true : false;
        c.other2         = other2 ? true : false;
        constraint.AddAndAssignId(&c);
        return c;
    }

    inline ConstraintBase Coincident(EntityBase entityA, EntityBase entityB,
                                     EntityBase workplane = EntityBase::_FREE_IN_3D) {
        if(entityA.IsPoint() && entityB.IsPoint()) {
            return AddConstraint(ConstraintBase::Type::POINTS_COINCIDENT, workplane, 0., entityA,
                                 entityB);
        } else if(entityA.IsPoint() && entityB.IsWorkplane()) {
            return AddConstraint(ConstraintBase::Type::PT_IN_PLANE, EntityBase::_FREE_IN_3D, 0., entityA, EntityBase::_NO_ENTITY, entityB);
        } else if(entityA.IsPoint() && entityB.IsLine()) {
            return AddConstraint(ConstraintBase::Type::PT_ON_LINE, workplane, 0., entityA, EntityBase::_NO_ENTITY, entityB);
        } else if(entityA.IsPoint() && (entityB.IsCircle() || entityB.IsArc())) {
            return AddConstraint(ConstraintBase::Type::PT_ON_CIRCLE, workplane, 0., entityA, EntityBase::_NO_ENTITY, entityB);
        }
        throw std::invalid_argument("Invalid arguments for coincident constraint");
    }

    inline ConstraintBase Distance(EntityBase entityA, EntityBase entityB, double value,
                                   EntityBase workplane) {
        if(entityA.IsPoint() && entityB.IsPoint()) {
            return AddConstraint(ConstraintBase::Type::PT_PT_DISTANCE, workplane, value, entityA,
                                 entityB);
        } else if(entityA.IsPoint() && entityB.IsWorkplane() && workplane.Is3D()) {
            return AddConstraint(ConstraintBase::Type::PT_PLANE_DISTANCE, entityB, value, entityA,
                                 EntityBase::_NO_ENTITY, entityB);
        } else if(entityA.IsPoint() && entityB.IsLine()) {
            return AddConstraint(ConstraintBase::Type::PT_LINE_DISTANCE, workplane, value, entityA,
                                 EntityBase::_NO_ENTITY, entityB);
        }
        throw std::invalid_argument("Invalid arguments for distance constraint");
    }

    inline ConstraintBase Equal(EntityBase entityA, EntityBase entityB,
                                EntityBase workplane = EntityBase::_FREE_IN_3D) {
        if(entityA.IsLine() && entityB.IsLine()) {
            return AddConstraint(ConstraintBase::Type::EQUAL_LENGTH_LINES, workplane, 0.,
                                 EntityBase::_NO_ENTITY, EntityBase::_NO_ENTITY, entityA, entityB);
        } else if(entityA.IsLine() && (entityB.IsArc() || entityB.IsCircle())) {
            return AddConstraint(ConstraintBase::Type::EQUAL_LINE_ARC_LEN, workplane, 0.,
                                 EntityBase::_NO_ENTITY, EntityBase::_NO_ENTITY, entityA, entityB);
        } else if((entityA.IsArc() || entityA.IsCircle()) &&
                  (entityB.IsArc() || entityB.IsCircle())) {
            return AddConstraint(ConstraintBase::Type::EQUAL_RADIUS, workplane, 0.,
                                 EntityBase::_NO_ENTITY, EntityBase::_NO_ENTITY, entityA, entityB);
        }
        throw std::invalid_argument("Invalid arguments for equal constraint");
    }

    inline ConstraintBase EqualAngle(EntityBase entityA, EntityBase entityB, EntityBase entityC,
                                     EntityBase entityD,
                                     EntityBase workplane = EntityBase::_FREE_IN_3D) {
        if(entityA.IsLine2D() && entityB.IsLine2D() && entityC.IsLine2D() && entityD.IsLine2D()) {
            return AddConstraint(ConstraintBase::Type::EQUAL_ANGLE, workplane, 0.,
                                 EntityBase::_NO_ENTITY, EntityBase::_NO_ENTITY, entityA, entityB,
                                 entityC, entityD);
        }
        throw std::invalid_argument("Invalid arguments for equal angle constraint");
    }

    inline ConstraintBase EqualPointToLine(EntityBase entityA, EntityBase entityB,
                                           EntityBase entityC, EntityBase entityD,
                                           EntityBase workplane = EntityBase::_FREE_IN_3D) {
        if(entityA.IsPoint2D() && entityB.IsLine2D() && entityC.IsPoint2D() && entityD.IsLine2D()) {
            return AddConstraint(ConstraintBase::Type::EQ_PT_LN_DISTANCES, workplane, 0., entityA,
                                 entityB, entityC, entityD);
        }
        throw std::invalid_argument("Invalid arguments for equal point to line constraint");
    }

    inline ConstraintBase Ratio(EntityBase entityA, EntityBase entityB, double value,
                                EntityBase workplane = EntityBase::_FREE_IN_3D) {
        if(entityA.IsLine2D() && entityB.IsLine2D()) {
            return AddConstraint(ConstraintBase::Type::LENGTH_RATIO, workplane, value,
                                 EntityBase::_NO_ENTITY, EntityBase::_NO_ENTITY, entityA, entityB);
        }
        throw std::invalid_argument("Invalid arguments for ratio constraint");
    }

    inline ConstraintBase Symmetric(EntityBase entityA, EntityBase entityB,
                                    EntityBase entityC   = EntityBase::_NO_ENTITY,
                                    EntityBase workplane = EntityBase::_FREE_IN_3D) {
        if(entityA.IsPoint3D() && entityB.IsPoint3D() && entityC.IsWorkplane() &&
           workplane.IsFreeIn3D()) {
            return AddConstraint(ConstraintBase::Type::SYMMETRIC, workplane, 0., entityA, entityB,
                                 entityC);
        } else if(entityA.IsPoint2D() && entityB.IsPoint2D() && entityC.IsWorkplane() &&
                  workplane.IsFreeIn3D()) {
            return AddConstraint(ConstraintBase::Type::SYMMETRIC, entityC, 0., entityA, entityB,
                                 entityC);
        } else if(entityA.IsPoint2D() && entityB.IsPoint2D() && entityC.IsLine()) {
            if(workplane.IsFreeIn3D()) {
                throw std::invalid_argument("3d workplane given for a 2d constraint");
            }
            return AddConstraint(ConstraintBase::Type::SYMMETRIC_LINE, workplane, 0., entityA,
                                 entityB, entityC);
        }
        throw std::invalid_argument("Invalid arguments for symmetric constraint");
    }

    inline ConstraintBase SymmetricH(EntityBase ptA, EntityBase ptB,
                                     EntityBase workplane = EntityBase::_FREE_IN_3D) {
        if(workplane.IsFreeIn3D()) {
            throw std::invalid_argument("3d workplane given for a 2d constraint");
        } else if(ptA.IsPoint2D() && ptB.IsPoint2D()) {
            return AddConstraint(ConstraintBase::Type::SYMMETRIC_HORIZ, workplane, 0., ptA, ptB);
        }
        throw std::invalid_argument("Invalid arguments for symmetric horizontal constraint");
    }

    inline ConstraintBase SymmetricV(EntityBase ptA, EntityBase ptB,
                                     EntityBase workplane = EntityBase::_FREE_IN_3D) {
        if(workplane.IsFreeIn3D()) {
            throw std::invalid_argument("3d workplane given for a 2d constraint");
        } else if(ptA.IsPoint2D() && ptB.IsPoint2D()) {
            return AddConstraint(ConstraintBase::Type::SYMMETRIC_VERT, workplane, 0., ptA, ptB);
        }
        throw std::invalid_argument("Invalid arguments for symmetric vertical constraint");
    }

    inline ConstraintBase Midpoint(EntityBase ptA, EntityBase ptB,
                                   EntityBase workplane = EntityBase::_FREE_IN_3D) {
        if(ptA.IsPoint() && ptB.IsLine()) {
            return AddConstraint(ConstraintBase::Type::AT_MIDPOINT, workplane, 0., ptA,
                                 EntityBase::_NO_ENTITY, ptB);
        }
        throw std::invalid_argument("Invalid arguments for midpoint constraint");
    }

    inline ConstraintBase Horizontal(EntityBase entityA, EntityBase workplane,
                                     EntityBase entityB = EntityBase::_NO_ENTITY) {
        if(workplane.IsFreeIn3D()) {
            throw std::invalid_argument("Horizontal constraint is not supported in 3D");
        } else if(entityA.IsLine2D()) {
            return AddConstraint(ConstraintBase::Type::HORIZONTAL, workplane, 0.,
                                 EntityBase::_NO_ENTITY, EntityBase::_NO_ENTITY, entityA);
        } else if(entityA.IsPoint2D() && entityB.IsPoint2D()) {
            return AddConstraint(ConstraintBase::Type::HORIZONTAL, workplane, 0., entityA, entityB);
        }
        throw std::invalid_argument("Invalid arguments for horizontal constraint");
    }

    inline ConstraintBase Vertical(EntityBase entityA, EntityBase workplane,
                                   EntityBase entityB = EntityBase::_NO_ENTITY) {
        if(workplane.IsFreeIn3D()) {
            throw std::invalid_argument("Vertical constraint is not supported in 3D");
        } else if(entityA.IsLine2D()) {
            return AddConstraint(ConstraintBase::Type::VERTICAL, workplane, 0.,
                                 EntityBase::_NO_ENTITY, EntityBase::_NO_ENTITY, entityA);
        } else if(entityA.IsPoint2D() && entityB.IsPoint2D()) {
            return AddConstraint(ConstraintBase::Type::VERTICAL, workplane, 0., entityA, entityB);
        }
        throw std::invalid_argument("Invalid arguments for horizontal constraint");
    }

    inline ConstraintBase Diameter(EntityBase entityA, double value) {
        if(entityA.IsArc() || entityA.IsCircle()) {
            return AddConstraint(ConstraintBase::Type::DIAMETER, EntityBase::_FREE_IN_3D, value,
                                 EntityBase::_NO_ENTITY, EntityBase::_NO_ENTITY, entityA);
        }
        throw std::invalid_argument("Invalid arguments for diameter constraint");
    }

    inline ConstraintBase SameOrientation(EntityBase entityA, EntityBase entityB) {
        if(entityA.IsNormal3D() && entityB.IsNormal3D()) {
            return AddConstraint(ConstraintBase::Type::SAME_ORIENTATION, EntityBase::_FREE_IN_3D,
                                 0., EntityBase::_NO_ENTITY, EntityBase::_NO_ENTITY, entityA,
                                 entityB);
        }
        throw std::invalid_argument("Invalid arguments for same orientation constraint");
    }

    inline ConstraintBase Angle(EntityBase entityA, EntityBase entityB, double value,
                                EntityBase workplane = EntityBase::_FREE_IN_3D,
                                bool inverse         = false) {
        if(entityA.IsLine2D() && entityB.IsLine2D()) {
            return AddConstraint(ConstraintBase::Type::ANGLE, workplane, value,
                                 EntityBase::_NO_ENTITY, EntityBase::_NO_ENTITY, entityA, entityB,
                                 EntityBase::_NO_ENTITY, EntityBase::_NO_ENTITY, inverse);
        }
        throw std::invalid_argument("Invalid arguments for angle constraint");
    }

    inline ConstraintBase Perpendicular(EntityBase entityA, EntityBase entityB,
                                        EntityBase workplane = EntityBase::_FREE_IN_3D,
                                        bool inverse         = false) {
        if(entityA.IsLine2D() && entityB.IsLine2D()) {
            return AddConstraint(ConstraintBase::Type::PERPENDICULAR, workplane, 0.,
                                 EntityBase::_NO_ENTITY, EntityBase::_NO_ENTITY, entityA, entityB,
                                 EntityBase::_NO_ENTITY, EntityBase::_NO_ENTITY, inverse);
        }
        throw std::invalid_argument("Invalid arguments for perpendicular constraint");
    }

    inline ConstraintBase Parallel(EntityBase entityA, EntityBase entityB,
                                   EntityBase workplane = EntityBase::_FREE_IN_3D) {
        if(entityA.IsLine2D() && entityB.IsLine2D()) {
            return AddConstraint(ConstraintBase::Type::PARALLEL, workplane, 0.,
                                 EntityBase::_NO_ENTITY, EntityBase::_NO_ENTITY, entityA, entityB);
        }
        throw std::invalid_argument("Invalid arguments for parallel constraint");
    }

    inline ConstraintBase Tangent(EntityBase entityA, EntityBase entityB,
                                  EntityBase workplane = EntityBase::_FREE_IN_3D) {
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
            return AddConstraint(ConstraintBase::Type::ARC_LINE_TANGENT, workplane, 0.,
                                 EntityBase::_NO_ENTITY, EntityBase::_NO_ENTITY, entityA, entityB,
                                 EntityBase::_NO_ENTITY, EntityBase::_NO_ENTITY, other);
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
            return AddConstraint(ConstraintBase::Type::CUBIC_LINE_TANGENT, workplane, 0.,
                                 EntityBase::_NO_ENTITY, EntityBase::_NO_ENTITY, entityA, entityB,
                                 EntityBase::_NO_ENTITY, EntityBase::_NO_ENTITY, other);
        } else if((entityA.IsArc() || entityA.IsCubic()) &&
                  (entityB.IsArc() || entityB.IsCubic())) {
            if(workplane.IsFreeIn3D()) {
                throw std::invalid_argument("3d workplane given for a 2d constraint");
            }
            Vector as = entityA.EndpointStart(), af = entityA.EndpointFinish(),
                   bs = entityB.EndpointStart(), bf = entityB.EndpointFinish();
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
            return AddConstraint(ConstraintBase::Type::CURVE_CURVE_TANGENT, workplane, 0.,
                                 EntityBase::_NO_ENTITY, EntityBase::_NO_ENTITY, entityA, entityB,
                                 EntityBase::_NO_ENTITY, EntityBase::_NO_ENTITY, other, other2);
        }
        throw std::invalid_argument("Invalid arguments for tangent constraint");
    }

    inline ConstraintBase DistanceProj(EntityBase ptA, EntityBase ptB, double value) {
        if(ptA.IsPoint() && ptB.IsPoint()) {
            return AddConstraint(ConstraintBase::Type::PROJ_PT_DISTANCE, EntityBase::_FREE_IN_3D,
                                 value, ptA, ptB);
        }
        throw std::invalid_argument("Invalid arguments for projected distance constraint");
    }

    inline ConstraintBase LengthDiff(EntityBase entityA, EntityBase entityB, double value,
                                     EntityBase workplane = EntityBase::_FREE_IN_3D) {
        if(entityA.IsLine() && entityB.IsLine()) {
            return AddConstraint(ConstraintBase::Type::LENGTH_DIFFERENCE, workplane, value,
                                 EntityBase::_NO_ENTITY, EntityBase::_NO_ENTITY, entityA, entityB);
        }
        throw std::invalid_argument("Invalid arguments for length difference constraint");
    }

    inline ConstraintBase Dragged(EntityBase ptA, EntityBase workplane = EntityBase::_FREE_IN_3D) {
        if(ptA.IsPoint()) {
            return AddConstraint(ConstraintBase::Type::WHERE_DRAGGED, workplane, 0., ptA);
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
