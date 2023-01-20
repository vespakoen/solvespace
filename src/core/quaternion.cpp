#include "quaternion.h"

const Quaternion Quaternion::IDENTITY = {1, 0, 0, 0};

Quaternion Quaternion::From(double w, double vx, double vy, double vz) {
    Quaternion q;
    q.w  = w;
    q.vx = vx;
    q.vy = vy;
    q.vz = vz;
    return q;
}

// Quaternion Quaternion::From(hParam w, hParam vx, hParam vy, hParam vz) {
//     Quaternion q;
//     q.w  = SK.GetParam(w)->val;
//     q.vx = SK.GetParam(vx)->val;
//     q.vy = SK.GetParam(vy)->val;
//     q.vz = SK.GetParam(vz)->val;
//     return q;
// }

Quaternion Quaternion::From(Vector axis, double dtheta) {
    Quaternion q;
    double c = cos(dtheta / 2), s = sin(dtheta / 2);
    axis = axis.WithMagnitude(s);
    q.w  = c;
    q.vx = axis.x;
    q.vy = axis.y;
    q.vz = axis.z;
    return q;
}

Quaternion Quaternion::From(Vector u, Vector v) {
    Vector n = u.Cross(v);

    Quaternion q;
    double s, tr = 1 + u.x + v.y + n.z;
    if(tr > 1e-4) {
        s    = 2 * sqrt(tr);
        q.w  = s / 4;
        q.vx = (v.z - n.y) / s;
        q.vy = (n.x - u.z) / s;
        q.vz = (u.y - v.x) / s;
    } else {
        if(u.x > v.y && u.x > n.z) {
            s    = 2 * sqrt(1 + u.x - v.y - n.z);
            q.w  = (v.z - n.y) / s;
            q.vx = s / 4;
            q.vy = (u.y + v.x) / s;
            q.vz = (n.x + u.z) / s;
        } else if(v.y > n.z) {
            s    = 2 * sqrt(1 - u.x + v.y - n.z);
            q.w  = (n.x - u.z) / s;
            q.vx = (u.y + v.x) / s;
            q.vy = s / 4;
            q.vz = (v.z + n.y) / s;
        } else {
            s    = 2 * sqrt(1 - u.x - v.y + n.z);
            q.w  = (u.y - v.x) / s;
            q.vx = (n.x + u.z) / s;
            q.vy = (v.z + n.y) / s;
            q.vz = s / 4;
        }
    }

    return q.WithMagnitude(1);
}

Quaternion Quaternion::Plus(Quaternion b) const {
    Quaternion q;
    q.w  = w + b.w;
    q.vx = vx + b.vx;
    q.vy = vy + b.vy;
    q.vz = vz + b.vz;
    return q;
}

Quaternion Quaternion::Minus(Quaternion b) const {
    Quaternion q;
    q.w  = w - b.w;
    q.vx = vx - b.vx;
    q.vy = vy - b.vy;
    q.vz = vz - b.vz;
    return q;
}

Quaternion Quaternion::ScaledBy(double s) const {
    Quaternion q;
    q.w  = w * s;
    q.vx = vx * s;
    q.vy = vy * s;
    q.vz = vz * s;
    return q;
}

double Quaternion::Magnitude() const {
    return sqrt(w * w + vx * vx + vy * vy + vz * vz);
}

Quaternion Quaternion::WithMagnitude(double s) const {
    return ScaledBy(s / Magnitude());
}

Vector Quaternion::RotationU() const {
    Vector v;
    v.x = w * w + vx * vx - vy * vy - vz * vz;
    v.y = 2 * w * vz + 2 * vx * vy;
    v.z = 2 * vx * vz - 2 * w * vy;
    return v;
}

Vector Quaternion::RotationV() const {
    Vector v;
    v.x = 2 * vx * vy - 2 * w * vz;
    v.y = w * w - vx * vx + vy * vy - vz * vz;
    v.z = 2 * w * vx + 2 * vy * vz;
    return v;
}

Vector Quaternion::RotationN() const {
    Vector v;
    v.x = 2 * w * vy + 2 * vx * vz;
    v.y = 2 * vy * vz - 2 * w * vx;
    v.z = w * w - vx * vx - vy * vy + vz * vz;
    return v;
}

Vector Quaternion::Rotate(Vector p) const {
    // Express the point in the new basis
    return (RotationU().ScaledBy(p.x))
        .Plus(RotationV().ScaledBy(p.y))
        .Plus(RotationN().ScaledBy(p.z));
}

Quaternion Quaternion::Inverse() const {
    Quaternion r;
    r.w  = w;
    r.vx = -vx;
    r.vy = -vy;
    r.vz = -vz;
    return r.WithMagnitude(1); // not that the normalize should be reqd
}

Quaternion Quaternion::ToThe(double p) const {
    // Avoid division by zero, or arccos of something not in its domain
    if(w >= (1 - 1e-6)) {
        return From(1, 0, 0, 0);
    } else if(w <= (-1 + 1e-6)) {
        return From(-1, 0, 0, 0);
    }

    Quaternion r;
    Vector axis  = Vector::From(vx, vy, vz);
    double theta = acos(w); // okay, since magnitude is 1, so -1 <= w <= 1
    theta *= p;
    r.w  = cos(theta);
    axis = axis.WithMagnitude(sin(theta));
    r.vx = axis.x;
    r.vy = axis.y;
    r.vz = axis.z;
    return r;
}

Quaternion Quaternion::Times(Quaternion b) const {
    double sa = w, sb = b.w;
    Vector va = {vx, vy, vz};
    Vector vb = {b.vx, b.vy, b.vz};

    Quaternion r;
    r.w       = sa * sb - va.Dot(vb);
    Vector vr = vb.ScaledBy(sa).Plus(va.ScaledBy(sb).Plus(va.Cross(vb)));
    r.vx      = vr.x;
    r.vy      = vr.y;
    r.vz      = vr.z;
    return r;
}

Quaternion Quaternion::Mirror() const {
    Vector u = RotationU(), v = RotationV();
    u = u.ScaledBy(-1);
    v = v.ScaledBy(-1);
    return Quaternion::From(u, v);
}
