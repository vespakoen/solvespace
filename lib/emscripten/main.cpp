// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <stdint.h>
#include "system.h"
#include <emscripten/bind.h>

using namespace emscripten;

EMSCRIPTEN_BINDINGS(solvespace) {
    class_<System>("System")
    .constructor()
    .function("solve", &System::Solve)
}

// static Slvs_Param Slvs_System_param_getter(const Slvs_System &sys) {
//     return *sys.param;
// }

// static void Slvs_System_param_setter(Slvs_System &sys, const Slvs_Param &param) {
//     sys.param = new Slvs_Param(param);
// }

// static Slvs_Entity Slvs_System_entity_getter(const Slvs_System &sys) {
//     return *sys.entity;
// }

// static void Slvs_System_entity_setter(Slvs_System &sys, const Slvs_Entity &entity) {
//     sys.entity = new Slvs_Entity(entity);
// }

// static Slvs_Constraint Slvs_System_constraint_getter(const Slvs_System &sys) {
//     return *sys.constraint;
// }

// static void Slvs_System_constraint_setter(Slvs_System &sys, const Slvs_Constraint &constraint) {
//     sys.constraint = new Slvs_Constraint(constraint);
// }

// static Slvs_hConstraint Slvs_System_failed_getter(const Slvs_System &sys) {
//     return *sys.failed;
// }

// static void Slvs_System_failed_setter(Slvs_System &sys, const Slvs_hConstraint &failed) {
//     sys.failed = new Slvs_hConstraint(failed);
// }

// EMSCRIPTEN_BINDINGS(solvespace) {
//     constant("SLVS_C_POINTS_COINCIDENT", SLVS_C_POINTS_COINCIDENT);
//     constant("SLVS_C_PT_PT_DISTANCE", SLVS_C_PT_PT_DISTANCE);
//     constant("SLVS_C_PT_PLANE_DISTANCE", SLVS_C_PT_PLANE_DISTANCE);
//     constant("SLVS_C_PT_LINE_DISTANCE", SLVS_C_PT_LINE_DISTANCE);
//     constant("SLVS_C_PT_FACE_DISTANCE", SLVS_C_PT_FACE_DISTANCE);
//     constant("SLVS_C_PT_IN_PLANE", SLVS_C_PT_IN_PLANE);
//     constant("SLVS_C_PT_ON_LINE", SLVS_C_PT_ON_LINE);
//     constant("SLVS_C_PT_ON_FACE", SLVS_C_PT_ON_FACE);
//     constant("SLVS_C_EQUAL_LENGTH_LINES", SLVS_C_EQUAL_LENGTH_LINES);
//     constant("SLVS_C_LENGTH_RATIO", SLVS_C_LENGTH_RATIO);
//     constant("SLVS_C_EQ_LEN_PT_LINE_D", SLVS_C_EQ_LEN_PT_LINE_D);
//     constant("SLVS_C_EQ_PT_LN_DISTANCES", SLVS_C_EQ_PT_LN_DISTANCES);
//     constant("SLVS_C_EQUAL_ANGLE", SLVS_C_EQUAL_ANGLE);
//     constant("SLVS_C_EQUAL_LINE_ARC_LEN", SLVS_C_EQUAL_LINE_ARC_LEN);
//     constant("SLVS_C_SYMMETRIC", SLVS_C_SYMMETRIC);
//     constant("SLVS_C_SYMMETRIC_HORIZ", SLVS_C_SYMMETRIC_HORIZ);
//     constant("SLVS_C_SYMMETRIC_VERT", SLVS_C_SYMMETRIC_VERT);
//     constant("SLVS_C_SYMMETRIC_LINE", SLVS_C_SYMMETRIC_LINE);
//     constant("SLVS_C_AT_MIDPOINT", SLVS_C_AT_MIDPOINT);
//     constant("SLVS_C_HORIZONTAL", SLVS_C_HORIZONTAL);
//     constant("SLVS_C_VERTICAL", SLVS_C_VERTICAL);
//     constant("SLVS_C_DIAMETER", SLVS_C_DIAMETER);
//     constant("SLVS_C_PT_ON_CIRCLE", SLVS_C_PT_ON_CIRCLE);
//     constant("SLVS_C_SAME_ORIENTATION", SLVS_C_SAME_ORIENTATION);
//     constant("SLVS_C_ANGLE", SLVS_C_ANGLE);
//     constant("SLVS_C_PARALLEL", SLVS_C_PARALLEL);
//     constant("SLVS_C_PERPENDICULAR", SLVS_C_PERPENDICULAR);
//     constant("SLVS_C_ARC_LINE_TANGENT", SLVS_C_ARC_LINE_TANGENT);
//     constant("SLVS_C_CUBIC_LINE_TANGENT", SLVS_C_CUBIC_LINE_TANGENT);
//     constant("SLVS_C_EQUAL_RADIUS", SLVS_C_EQUAL_RADIUS);
//     constant("SLVS_C_PROJ_PT_DISTANCE", SLVS_C_PROJ_PT_DISTANCE);
//     constant("SLVS_C_WHERE_DRAGGED", SLVS_C_WHERE_DRAGGED);
//     constant("SLVS_C_CURVE_CURVE_TANGENT", SLVS_C_CURVE_CURVE_TANGENT);
//     constant("SLVS_C_LENGTH_DIFFERENCE", SLVS_C_LENGTH_DIFFERENCE);

//     constant("SLVS_RESULT_OKAY", SLVS_RESULT_OKAY);
//     constant("SLVS_RESULT_INCONSISTENT", SLVS_RESULT_INCONSISTENT);
//     constant("SLVS_RESULT_DIDNT_CONVERGE", SLVS_RESULT_DIDNT_CONVERGE);
//     constant("SLVS_RESULT_TOO_MANY_UNKNOWNS", SLVS_RESULT_TOO_MANY_UNKNOWNS);

//     value_object<Slvs_Param>("Slvs_Param")
//         .field("h", &Slvs_Param::h)
//         .field("group", &Slvs_Param::group)
//         .field("val", &Slvs_Param::val);

//     value_object<Slvs_Entity>("Slvs_Entity")
//         .field("h", &Slvs_Entity::h)
//         .field("group", &Slvs_Entity::group)
//         .field("type", &Slvs_Entity::type)
//         .field("wrkpl", &Slvs_Entity::wrkpl)
//         .field("point", &Slvs_Entity::point)
//         .field("normal", &Slvs_Entity::normal)
//         .field("distance", &Slvs_Entity::distance)
//         .field("param", &Slvs_Entity::param);

//     value_object<Slvs_System>("Slvs_System")
//         .field("param", &Slvs_System_param_getter, &Slvs_System_param_setter)
//         .field("params", &Slvs_System::params)
//         .field("entity", &Slvs_System_entity_getter, &Slvs_System_entity_setter)
//         .field("entities", &Slvs_System::entities)
//         .field("constraint", &Slvs_System_constraint_getter, &Slvs_System_constraint_setter)
//         .field("constraints", &Slvs_System::constraints)
//         .field("dragged", &Slvs_System::dragged)
//         .field("calculateFaileds", &Slvs_System::calculateFaileds)
//         .field("failed", &Slvs_System_failed_getter, &Slvs_System_failed_setter)
//         .field("faileds", &Slvs_System::faileds)
//         .field("dof", &Slvs_System::dof)
//         .field("result", &Slvs_System::result);

//     function("Slvs_QuaternionU", &Slvs_QuaternionU, allow_raw_pointers());
//     function("Slvs_QuaternionV", &Slvs_QuaternionV, allow_raw_pointers());
//     function("Slvs_QuaternionN", &Slvs_QuaternionN, allow_raw_pointers());
//     function("Slvs_MakeQuaternion", &Slvs_MakeQuaternion, allow_raw_pointers());
//     function("Slvs_Solve", &Slvs_Solve, allow_raw_pointers());
//     function("Slvs_MakeParam", &Slvs_MakeParam);
//     function("Slvs_MakePoint2d", &Slvs_MakePoint2d);
//     function("Slvs_MakePoint3d", &Slvs_MakePoint3d);
//     function("Slvs_MakeNormal3d", &Slvs_MakeNormal3d);
//     function("Slvs_MakeNormal2d", &Slvs_MakeNormal2d);
//     function("Slvs_MakeDistance", &Slvs_MakeDistance);
//     function("Slvs_MakeLineSegment", &Slvs_MakeLineSegment);
//     function("Slvs_MakeCubic", &Slvs_MakeCubic);
//     function("Slvs_MakeArcOfCircle", &Slvs_MakeArcOfCircle);
//     function("Slvs_MakeCircle", &Slvs_MakeCircle);
//     function("Slvs_MakeWorkplane", &Slvs_MakeWorkplane);
//     function("Slvs_MakeConstraint", &Slvs_MakeConstraint);
// }

