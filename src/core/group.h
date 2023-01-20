#ifndef SOLVESPACE_GROUP_H
#define SOLVESPACE_GROUP_H

#include "data.h"
#include "idlist.h"
// #include "utils.h"
// #include "expr.h"
// #include "entitybase.h"
// #include "quaternion.h"

// A set of requests. Every request must have an associated group.
class Group {
public:
    static const hGroup HGROUP_REFERENCES;

    // int tag;
    hGroup h;

    // enum class CopyAs {
    //     NUMERIC,
    //     N_TRANS,
    //     N_ROT_AA,
    //     N_ROT_TRANS,
    //     N_ROT_AXIS_TRANS,
    // };

    // enum class Type : uint32_t {
    //     DRAWING_3D        = 5000,
    //     DRAWING_WORKPLANE = 5001,
    //     EXTRUDE           = 5100,
    //     LATHE             = 5101,
    //     REVOLVE           = 5102,
    //     HELIX             = 5103,
    //     ROTATE            = 5200,
    //     TRANSLATE         = 5201,
    //     LINKED            = 5300
    // };
    // Group::Type type;

    int order;

    // hGroup opA;
    // hGroup opB;
    // bool visible;
    // bool suppress;
    bool relaxConstraints;
    bool allowRedundant;
    bool suppressDofCalculation;
    bool allDimsReference;
    // double scale;

    // bool clean;
    // bool dofCheckOk;
    // hEntity activeWorkplane;
    // double valA;
    // double valB;
    // double valC;
    // RgbaColor color;

    struct {
        SolveResult how;
        int dof;
        int findToFixTimeout;
        bool timeout;
        List<hConstraint> remove;
    } solved;

    // enum class Subtype : uint32_t {
    //     // For drawings in 2d
    //     WORKPLANE_BY_POINT_ORTHO   = 6000,
    //     WORKPLANE_BY_LINE_SEGMENTS = 6001,
    //     WORKPLANE_BY_POINT_NORMAL  = 6002,
    //     // WORKPLANE_BY_POINT_FACE    = 6003,
    //     // WORKPLANE_BY_FACE          = 6004,
    //     // For extrudes, translates, and rotates
    //     ONE_SIDED = 7000,
    //     TWO_SIDED = 7001
    // };
    // Group::Subtype subtype;

    // bool skipFirst; // for step and repeat ops

    // struct {
    //     Quaternion q;
    //     hEntity origin;
    //     hEntity entityB;
    //     hEntity entityC;
    //     bool swapUV;
    //     bool negateU;
    //     bool negateV;
    // } predef;

    // SPolygon polyLoops;
    // SBezierLoopSetSet bezierLoops;
    // SBezierLoopSet bezierOpens;

    // struct {
    //     PolyError how;
    //     SEdge notClosedAt;
    //     Vector errorPointAt;
    // } polyError;

    // bool booleanFailed;

    // SShell thisShell;
    // SShell runningShell;

    // SMesh thisMesh;
    // SMesh runningMesh;

    // bool displayDirty;
    // SMesh displayMesh;
    // SOutlineList displayOutlines;

    // enum class CombineAs : uint32_t { UNION = 0, DIFFERENCE = 1, ASSEMBLE = 2, INTERSECTION = 3 };
    // CombineAs meshCombine;

    // bool forceToMesh;

    // EntityMap remap;

    // Platform::Path linkFile;
    // SMesh impMesh;
    // SShell impShell;
    // EntityList impEntity;

    // std::string name;

    // void Activate();
    // std::string DescriptionString();
    void Clear();

    // static void AddParam(ParamList *param, hParam hp, double v);
    // void Generate(EntityList *entity, ParamList *param);
    // bool IsSolvedOkay();
    // void TransformImportedBy(Vector t, Quaternion q);
    // bool IsTriangleMeshAssembly() const;
    // bool IsForcedToMeshBySource() const;
    // bool IsForcedToMesh() const;
    // // When a request generates entities from entities, and the source
    // // entities may have come from multiple requests, it's necessary to
    // // remap the entity ID so that it's still unique. We do this with a
    // // mapping list.
    // enum {
    //     REMAP_LAST             = 1000,
    //     REMAP_TOP              = 1001,
    //     REMAP_BOTTOM           = 1002,
    //     REMAP_PT_TO_LINE       = 1003,
    //     REMAP_LINE_TO_FACE     = 1004,
    //     REMAP_LATHE_START      = 1006,
    //     REMAP_LATHE_END        = 1007,
    //     REMAP_PT_TO_ARC        = 1008,
    //     REMAP_PT_TO_NORMAL     = 1009,
    //     REMAP_LATHE_ARC_CENTER = 1010,
    // };
    // hEntity Remap(hEntity in, int copyNumber);
    // void MakeExtrusionLines(EntityList *el, hEntity in);
    // void MakeLatheCircles(IdList<Entity, hEntity> *el, IdList<Param, hParam> *param, hEntity in,
    //                       Vector pt, Vector axis);
    // void MakeLatheSurfacesSelectable(IdList<Entity, hEntity> *el, hEntity in, Vector axis);
    // void MakeRevolveEndFaces(IdList<Entity, hEntity> *el, hEntity pt, int ai, int af);
    // void MakeExtrusionTopBottomFaces(EntityList *el, hEntity pt);
    // void CopyEntity(EntityList *el, Entity *ep, int timesApplied, int remap, hParam dx, hParam dy,
    //                 hParam dz, hParam qw, hParam qvx, hParam qvy, hParam qvz, hParam dist,
    //                 CopyAs as);

    // void AddEq(IdList<Equation, hEquation> *l, Expr *expr, int index);
    void GenerateEquations(IdList<Equation, hEquation> *l);
    // bool IsVisible();
    // size_t GetNumConstraints();
    // Vector ExtrusionGetVector();
    // void ExtrusionForceVectorTo(const Vector &v);

    // // Assembling the curves into loops, and into a piecewise linear polygon
    // // at the same time.
    // void AssembleLoops(bool *allClosed, bool *allCoplanar, bool *allNonZeroLen);
    // void GenerateLoops();
    // // And the mesh stuff
    // Group *PreviousGroup() const;
    // Group *RunningMeshGroup() const;
    // bool IsMeshGroup();

    // void GenerateShellAndMesh();
    // template<class T>
    // void GenerateForStepAndRepeat(T *steps, T *outs, Group::CombineAs forWhat);
    // template<class T>
    // void GenerateForBoolean(T *a, T *b, T *o, Group::CombineAs how);
    // void GenerateDisplayItems();

    // enum class DrawMeshAs { DEFAULT, HOVERED, SELECTED };
    // void DrawMesh(DrawMeshAs how, Canvas *canvas);
    // void Draw(Canvas *canvas);
    // void DrawPolyError(Canvas *canvas);
    // void DrawFilledPaths(Canvas *canvas);
    // void DrawContourAreaLabels(Canvas *canvas);
    // bool ShouldDrawExploded() const;

    // SPolygon GetPolygon();

    // static void MenuGroup(Command id);
    // static void MenuGroup(Command id, Platform::Path linkFile);
};

#endif