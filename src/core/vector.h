#ifndef SOLVESPACE_VECTOR_H
#define SOLVESPACE_VECTOR_H

#include <cmath>
#include "core.h"
#include "point2d.h"

class Vector {
public:
    double x, y, z;

    static Vector From(double x, double y, double z);
    // static Vector From(hParam x, hParam y, hParam z);
    static Vector AtIntersectionOfPlanes(Vector n1, double d1, Vector n2, double d2);
    static Vector AtIntersectionOfLines(Vector a0, Vector a1, Vector b0, Vector b1, bool *skew,
                                        double *pa = NULL, double *pb = NULL);
    static Vector AtIntersectionOfPlaneAndLine(Vector n, double d, Vector p0, Vector p1,
                                               bool *parallel);
    static Vector AtIntersectionOfPlanes(Vector na, double da, Vector nb, double db, Vector nc,
                                         double dc, bool *parallel);
    static void ClosestPointBetweenLines(Vector pa, Vector da, Vector pb, Vector db, double *ta,
                                         double *tb);

    double Element(int i) const;
    bool Equals(Vector v, double tol = LENGTH_EPS) const;
    bool EqualsExactly(Vector v) const;
    Vector Plus(Vector b) const;
    Vector Minus(Vector b) const;
    Vector Negated() const;
    Vector Cross(Vector b) const;
    double DirectionCosineWith(Vector b) const;
    double Dot(Vector b) const;
    Vector Normal(int which) const;
    Vector RotatedAbout(Vector orig, Vector axis, double theta) const;
    Vector RotatedAbout(Vector axis, double theta) const;
    Vector DotInToCsys(Vector u, Vector v, Vector n) const;
    Vector ScaleOutOfCsys(Vector u, Vector v, Vector n) const;
    double DistanceToLine(Vector p0, Vector dp) const;
    double DistanceToPlane(Vector normal, Vector origin) const;
    bool OnLineSegment(Vector a, Vector b, double tol = LENGTH_EPS) const;
    Vector ClosestPointOnLine(Vector p0, Vector deltal) const;
    double Magnitude() const;
    double MagSquared() const;
    Vector WithMagnitude(double s) const;
    Vector ScaledBy(double s) const;
    // Vector ProjectInto(EntityBase *w) const;
    // Vector ProjectVectorInto(EntityBase *w) const;
    double DivProjected(Vector delta) const;
    Vector ClosestOrtho() const;
    void MakeMaxMin(Vector *maxv, Vector *minv) const;
    Vector ClampWithin(double minv, double maxv) const;
    static bool BoundingBoxesDisjoint(Vector amax, Vector amin, Vector bmax, Vector bmin);
    static bool BoundingBoxIntersectsLine(Vector amax, Vector amin, Vector p0, Vector p1,
                                          bool asSegment);
    bool OutsideAndNotOn(Vector maxv, Vector minv) const;
    Vector InPerspective(Vector u, Vector v, Vector n, Vector origin, double cameraTan) const;
    Point2d Project2d(Vector u, Vector v) const;
    Point2d ProjectXy() const;
    // Vector4 Project4d() const;
};

struct VectorHash {
    size_t operator()(const Vector &v) const;
};

struct VectorPred {
    bool operator()(Vector a, Vector b) const;
};

#endif