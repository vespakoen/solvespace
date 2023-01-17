#include "point2d.h"

Point2d Point2d::From(double x, double y) {
    return {x, y};
}

Point2d Point2d::FromPolar(double r, double a) {
    return {r * cos(a), r * sin(a)};
}

double Point2d::Angle() const {
    double a = atan2(y, x);
    return M_PI + remainder(a - M_PI, 2 * M_PI);
}

double Point2d::AngleTo(const Point2d &p) const {
    return p.Minus(*this).Angle();
}

Point2d Point2d::Plus(const Point2d &b) const {
    return {x + b.x, y + b.y};
}

Point2d Point2d::Minus(const Point2d &b) const {
    return {x - b.x, y - b.y};
}

Point2d Point2d::ScaledBy(double s) const {
    return {x * s, y * s};
}

double Point2d::DivProjected(Point2d delta) const {
    return (x * delta.x + y * delta.y) / (delta.x * delta.x + delta.y * delta.y);
}

double Point2d::MagSquared() const {
    return x * x + y * y;
}

double Point2d::Magnitude() const {
    return sqrt(x * x + y * y);
}

Point2d Point2d::WithMagnitude(double v) const {
    double m = Magnitude();
    if(m < 1e-20) {
        // dbp("!!! WithMagnitude() of zero vector");
        return {v, 0};
    }
    return {x * v / m, y * v / m};
}

double Point2d::DistanceTo(const Point2d &p) const {
    double dx = x - p.x;
    double dy = y - p.y;
    return sqrt(dx * dx + dy * dy);
}

double Point2d::Dot(Point2d p) const {
    return x * p.x + y * p.y;
}

double Point2d::DistanceToLine(const Point2d &p0, const Point2d &dp, bool asSegment) const {
    double m = dp.x * dp.x + dp.y * dp.y;
    if(m < LENGTH_EPS * LENGTH_EPS)
        return VERY_POSITIVE;

    // Let our line be p = p0 + t*dp, for a scalar t from 0 to 1
    double t = (dp.x * (x - p0.x) + dp.y * (y - p0.y)) / m;

    if(asSegment) {
        if(t < 0.0)
            return DistanceTo(p0);
        if(t > 1.0)
            return DistanceTo(p0.Plus(dp));
    }
    Point2d closest = p0.Plus(dp.ScaledBy(t));
    return DistanceTo(closest);
}

double Point2d::DistanceToLineSigned(const Point2d &p0, const Point2d &dp, bool asSegment) const {
    double m = dp.x * dp.x + dp.y * dp.y;
    if(m < LENGTH_EPS * LENGTH_EPS)
        return VERY_POSITIVE;

    Point2d n   = dp.Normal().WithMagnitude(1.0);
    double dist = n.Dot(*this) - n.Dot(p0);
    if(asSegment) {
        // Let our line be p = p0 + t*dp, for a scalar t from 0 to 1
        double t    = (dp.x * (x - p0.x) + dp.y * (y - p0.y)) / m;
        double sign = (dist > 0.0) ? 1.0 : -1.0;
        if(t < 0.0)
            return DistanceTo(p0) * sign;
        if(t > 1.0)
            return DistanceTo(p0.Plus(dp)) * sign;
    }

    return dist;
}

Point2d Point2d::Normal() const {
    return {y, -x};
}

bool Point2d::Equals(Point2d v, double tol) const {
    double dx = v.x - x;
    if(dx < -tol || dx > tol)
        return false;
    double dy = v.y - y;
    if(dy < -tol || dy > tol)
        return false;

    return (this->Minus(v)).MagSquared() < tol * tol;
}
