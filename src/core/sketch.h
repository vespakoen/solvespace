#ifndef SOLVESPACE_SKETCH_H
#define SOLVESPACE_SKETCH_H

#include "data.h"
#include "idlist.h"
#include "constraint.h"
#include "entity.h"

// template<class C, class E>
class Sketch {
public:
    // These are user-editable, and define the sketch.
    // IdList<Group, hGroup> group;
    // List<hGroup> groupOrder;
    IdList<ConstraintBase, hConstraint> constraint;
    // IdList<Request, hRequest> request;
    // IdList<Style, hStyle> style;

    // These are generated from the above.
    IdList<EntityBase, hEntity> entity;
    IdList<Param, hParam> param;

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
    // inline Group *GetGroup(hGroup h) {
    //     return group.FindById(h);
    // }
    // Styles are handled a bit differently.

    void Clear();

    // BBox CalculateEntityBBox(bool includingInvisible);
    // Group *GetRunningMeshGroupFor(hGroup h);
};

#endif
