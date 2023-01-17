#ifndef SOLVESPACE_POINT2D_H
#define SOLVESPACE_POINT2D_H

#include "core.h"
#include <cmath>

class Point2d {
public:
    double x, y;

    static Point2d From(double x, double y);
    static Point2d FromPolar(double r, double a);

    Point2d Plus(const Point2d &b) const;
    Point2d Minus(const Point2d &b) const;
    Point2d ScaledBy(double s) const;
    double DivProjected(Point2d delta) const;
    double Dot(Point2d p) const;
    double DistanceTo(const Point2d &p) const;
    double DistanceToLine(const Point2d &p0, const Point2d &dp, bool asSegment) const;
    double DistanceToLineSigned(const Point2d &p0, const Point2d &dp, bool asSegment) const;
    double Angle() const;
    double AngleTo(const Point2d &p) const;
    double Magnitude() const;
    double MagSquared() const;
    Point2d WithMagnitude(double v) const;
    Point2d Normal() const;
    bool Equals(Point2d v, double tol = LENGTH_EPS) const;
};

#endif