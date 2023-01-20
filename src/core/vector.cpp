#include "vector.h"

bool Vector::EqualsExactly(Vector v) const {
    return EXACT(x == v.x && y == v.y && z == v.z);
}

double Vector::DirectionCosineWith(Vector b) const {
    Vector a = this->WithMagnitude(1);
    b        = b.WithMagnitude(1);
    return a.Dot(b);
}

Vector Vector::Normal(int which) const {
    Vector n;

    // Arbitrarily choose one vector that's normal to us, pivoting
    // appropriately.
    double xa = fabs(x), ya = fabs(y), za = fabs(z);
    if(this->Equals(Vector::From(0, 0, 1))) {
        // Make DXFs exported in the XY plane work nicely...
        n = Vector::From(1, 0, 0);
    } else if(xa < ya && xa < za) {
        n.x = 0;
        n.y = z;
        n.z = -y;
    } else if(ya < za) {
        n.x = -z;
        n.y = 0;
        n.z = x;
    } else {
        n.x = y;
        n.y = -x;
        n.z = 0;
    }

    if(which == 0) {
        // That's the vector we return.
    } else if(which == 1) {
        n = this->Cross(n);
    } else
        ssassert(false, "Unexpected vector normal index");

    n = n.WithMagnitude(1);

    return n;
}

Vector Vector::RotatedAbout(Vector orig, Vector axis, double theta) const {
    Vector r = this->Minus(orig);
    r        = r.RotatedAbout(axis, theta);
    return r.Plus(orig);
}

Vector Vector::RotatedAbout(Vector axis, double theta) const {
    double c = cos(theta);
    double s = sin(theta);

    axis = axis.WithMagnitude(1);

    Vector r;

    r.x = (x) * (c + (1 - c) * (axis.x) * (axis.x)) +
          (y) * ((1 - c) * (axis.x) * (axis.y) - s * (axis.z)) +
          (z) * ((1 - c) * (axis.x) * (axis.z) + s * (axis.y));

    r.y = (x) * ((1 - c) * (axis.y) * (axis.x) + s * (axis.z)) +
          (y) * (c + (1 - c) * (axis.y) * (axis.y)) +
          (z) * ((1 - c) * (axis.y) * (axis.z) - s * (axis.x));

    r.z = (x) * ((1 - c) * (axis.z) * (axis.x) - s * (axis.y)) +
          (y) * ((1 - c) * (axis.z) * (axis.y) + s * (axis.x)) +
          (z) * (c + (1 - c) * (axis.z) * (axis.z));

    return r;
}

Vector Vector::DotInToCsys(Vector u, Vector v, Vector n) const {
    Vector r = {this->Dot(u), this->Dot(v), this->Dot(n)};
    return r;
}

Vector Vector::ScaleOutOfCsys(Vector u, Vector v, Vector n) const {
    Vector r = u.ScaledBy(x).Plus(v.ScaledBy(y).Plus(n.ScaledBy(z)));
    return r;
}

Vector Vector::InPerspective(Vector u, Vector v, Vector n, Vector origin, double cameraTan) const {
    Vector r = this->Minus(origin);
    r        = r.DotInToCsys(u, v, n);
    // yes, minus; we are assuming a csys where u cross v equals n, backwards
    // from the display stuff
    double w = (1 - r.z * cameraTan);
    r        = r.ScaledBy(1 / w);

    return r;
}

double Vector::DistanceToLine(Vector p0, Vector dp) const {
    double m = dp.Magnitude();
    return ((this->Minus(p0)).Cross(dp)).Magnitude() / m;
}

double Vector::DistanceToPlane(Vector normal, Vector origin) const {
    return this->Dot(normal) - origin.Dot(normal);
}

bool Vector::OnLineSegment(Vector a, Vector b, double tol) const {
    if(this->Equals(a, tol) || this->Equals(b, tol))
        return true;

    Vector d = b.Minus(a);

    double m      = d.MagSquared();
    double distsq = ((this->Minus(a)).Cross(d)).MagSquared() / m;

    if(distsq >= tol * tol)
        return false;

    double t = (this->Minus(a)).DivProjected(d);
    // On-endpoint already tested
    if(t < 0 || t > 1)
        return false;
    return true;
}

Vector Vector::ClosestPointOnLine(Vector p0, Vector dp) const {
    dp = dp.WithMagnitude(1);
    // this, p0, and (p0+dp) define a plane; the min distance is in
    // that plane, so calculate its normal
    Vector pn = (this->Minus(p0)).Cross(dp);
    // The minimum distance line is in that plane, perpendicular
    // to the line
    Vector n = pn.Cross(dp);

    // Calculate the actual distance
    double d = (dp.Cross(p0.Minus(*this))).Magnitude();
    return this->Plus(n.WithMagnitude(d));
}

Vector Vector::WithMagnitude(double v) const {
    double m = Magnitude();
    if(EXACT(m == 0)) {
        // We can do a zero vector with zero magnitude, but not any other cases.
        if(fabs(v) > 1e-100) {
            dbp("Vector::WithMagnitude(%g) of zero vector!", v);
        }
        return From(0, 0, 0);
    } else {
        return ScaledBy(v / m);
    }
}

// Vector Vector::ProjectVectorInto(EntityBase *w) const {
//     // EntityBase *w = SK.GetEntity(wrkpl);
//     Vector u      = w->Normal()->NormalU();
//     Vector v      = w->Normal()->NormalV();

//     double up = this->Dot(u);
//     double vp = this->Dot(v);

//     return (u.ScaledBy(up)).Plus(v.ScaledBy(vp));
// }

// Vector Vector::ProjectInto(EntityBase *w) const {
//     // EntityBase *w = SK.GetEntity(wrkpl);
//     Vector p0     = w->WorkplaneGetOffset();

//     Vector f = this->Minus(p0);

//     return p0.Plus(f.ProjectVectorInto(w));
// }

Point2d Vector::Project2d(Vector u, Vector v) const {
    Point2d p;
    p.x = this->Dot(u);
    p.y = this->Dot(v);
    return p;
}

Point2d Vector::ProjectXy() const {
    Point2d p;
    p.x = x;
    p.y = y;
    return p;
}

// Vector4 Vector::Project4d() const {
//     return Vector4::From(1, x, y, z);
// }

double Vector::DivProjected(Vector delta) const {
    return (x * delta.x + y * delta.y + z * delta.z) /
           (delta.x * delta.x + delta.y * delta.y + delta.z * delta.z);
}

Vector Vector::ClosestOrtho() const {
    double mx = fabs(x), my = fabs(y), mz = fabs(z);

    if(mx > my && mx > mz) {
        return From((x > 0) ? 1 : -1, 0, 0);
    } else if(my > mz) {
        return From(0, (y > 0) ? 1 : -1, 0);
    } else {
        return From(0, 0, (z > 0) ? 1 : -1);
    }
}

Vector Vector::ClampWithin(double minv, double maxv) const {
    Vector ret = *this;

    if(ret.x < minv)
        ret.x = minv;
    if(ret.y < minv)
        ret.y = minv;
    if(ret.z < minv)
        ret.z = minv;

    if(ret.x > maxv)
        ret.x = maxv;
    if(ret.y > maxv)
        ret.y = maxv;
    if(ret.z > maxv)
        ret.z = maxv;

    return ret;
}

bool Vector::OutsideAndNotOn(Vector maxv, Vector minv) const {
    return (x > maxv.x + LENGTH_EPS) || (x < minv.x - LENGTH_EPS) || (y > maxv.y + LENGTH_EPS) ||
           (y < minv.y - LENGTH_EPS) || (z > maxv.z + LENGTH_EPS) || (z < minv.z - LENGTH_EPS);
}

bool Vector::BoundingBoxesDisjoint(Vector amax, Vector amin, Vector bmax, Vector bmin) {
    int i;
    for(i = 0; i < 3; i++) {
        if(amax.Element(i) < bmin.Element(i) - LENGTH_EPS)
            return true;
        if(amin.Element(i) > bmax.Element(i) + LENGTH_EPS)
            return true;
    }
    return false;
}

bool Vector::BoundingBoxIntersectsLine(Vector amax, Vector amin, Vector p0, Vector p1,
                                       bool asSegment) {
    Vector dp = p1.Minus(p0);
    double lp = dp.Magnitude();
    dp        = dp.ScaledBy(1.0 / lp);

    int i, a;
    for(i = 0; i < 3; i++) {
        int j = WRAP(i + 1, 3), k = WRAP(i + 2, 3);
        if(lp * fabs(dp.Element(i)) < LENGTH_EPS)
            continue; // parallel to plane

        for(a = 0; a < 2; a++) {
            double d = (a == 0) ? amax.Element(i) : amin.Element(i);
            // n dot (p0 + t*dp) = d
            // (n dot p0) + t * (n dot dp) = d
            double t = (d - p0.Element(i)) / dp.Element(i);
            Vector p = p0.Plus(dp.ScaledBy(t));

            if(asSegment && (t < -LENGTH_EPS || t > (lp + LENGTH_EPS)))
                continue;

            if(p.Element(j) > amax.Element(j) + LENGTH_EPS)
                continue;
            if(p.Element(k) > amax.Element(k) + LENGTH_EPS)
                continue;

            if(p.Element(j) < amin.Element(j) - LENGTH_EPS)
                continue;
            if(p.Element(k) < amin.Element(k) - LENGTH_EPS)
                continue;

            return true;
        }
    }

    return false;
}

Vector Vector::AtIntersectionOfPlanes(Vector n1, double d1, Vector n2, double d2) {
    double det = (n1.Dot(n1)) * (n2.Dot(n2)) - (n1.Dot(n2)) * (n1.Dot(n2));
    double c1  = (d1 * n2.Dot(n2) - d2 * n1.Dot(n2)) / det;
    double c2  = (d2 * n1.Dot(n1) - d1 * n1.Dot(n2)) / det;

    return (n1.ScaledBy(c1)).Plus(n2.ScaledBy(c2));
}

void Vector::ClosestPointBetweenLines(Vector a0, Vector da, Vector b0, Vector db, double *ta,
                                      double *tb) {
    // Make a semi-orthogonal coordinate system from those directions;
    // note that dna and dnb need not be perpendicular.
    Vector dn  = da.Cross(db); // normal to both
    Vector dna = dn.Cross(da); // normal to da
    Vector dnb = dn.Cross(db); // normal to db

    // At the intersection of the lines
    //    a0 + pa*da = b0 + pb*db (where pa, pb are scalar params)
    // So dot this equation against dna and dnb to get two equations
    // to solve for da and db
    *tb = ((a0.Minus(b0)).Dot(dna)) / (db.Dot(dna));
    *ta = -((a0.Minus(b0)).Dot(dnb)) / (da.Dot(dnb));
}

Vector Vector::AtIntersectionOfLines(Vector a0, Vector a1, Vector b0, Vector b1, bool *skew,
                                     double *parama, double *paramb) {
    Vector da = a1.Minus(a0), db = b1.Minus(b0);

    double pa, pb;
    Vector::ClosestPointBetweenLines(a0, da, b0, db, &pa, &pb);

    if(parama)
        *parama = pa;
    if(paramb)
        *paramb = pb;

    // And from either of those, we get the intersection point.
    Vector pi = a0.Plus(da.ScaledBy(pa));

    if(skew) {
        // Check if the intersection points on each line are actually
        // coincident...
        if(pi.Equals(b0.Plus(db.ScaledBy(pb)))) {
            *skew = false;
        } else {
            *skew = true;
        }
    }
    return pi;
}

Vector Vector::AtIntersectionOfPlaneAndLine(Vector n, double d, Vector p0, Vector p1,
                                            bool *parallel) {
    Vector dp = p1.Minus(p0);

    if(fabs(n.Dot(dp)) < LENGTH_EPS) {
        if(parallel)
            *parallel = true;
        return Vector::From(0, 0, 0);
    }

    if(parallel)
        *parallel = false;

    // n dot (p0 + t*dp) = d
    // (n dot p0) + t * (n dot dp) = d
    double t = (d - n.Dot(p0)) / (n.Dot(dp));

    return p0.Plus(dp.ScaledBy(t));
}

static double det2(double a1, double b1, double a2, double b2) {
    return (a1 * b2) - (b1 * a2);
}
static double det3(double a1, double b1, double c1, double a2, double b2, double c2, double a3,
                   double b3, double c3) {
    return a1 * det2(b2, c2, b3, c3) - b1 * det2(a2, c2, a3, c3) + c1 * det2(a2, b2, a3, b3);
}
Vector Vector::AtIntersectionOfPlanes(Vector na, double da, Vector nb, double db, Vector nc,
                                      double dc, bool *parallel) {
    double det = det3(na.x, na.y, na.z, nb.x, nb.y, nb.z, nc.x, nc.y, nc.z);
    if(fabs(det) < 1e-10) { // arbitrary tolerance, not so good
        *parallel = true;
        return Vector::From(0, 0, 0);
    }
    *parallel = false;

    double detx = det3(da, na.y, na.z, db, nb.y, nb.z, dc, nc.y, nc.z);

    double dety = det3(na.x, da, na.z, nb.x, db, nb.z, nc.x, dc, nc.z);

    double detz = det3(na.x, na.y, da, nb.x, nb.y, db, nc.x, nc.y, dc);

    return Vector::From(detx / det, dety / det, detz / det);
}

size_t VectorHash::operator()(const Vector &v) const {
    const size_t size = (size_t)pow(std::numeric_limits<size_t>::max(), 1.0 / 3.0) - 1;
    const double eps  = 4.0 * LENGTH_EPS;

    double x = fabs(v.x) / eps;
    double y = fabs(v.y) / eps;
    double z = fabs(v.y) / eps;

    size_t xs = size_t(fmod(x, (double)size));
    size_t ys = size_t(fmod(y, (double)size));
    size_t zs = size_t(fmod(z, (double)size));

    return (zs * size + ys) * size + xs;
}

bool VectorPred::operator()(Vector a, Vector b) const {
    return a.Equals(b, LENGTH_EPS);
}
