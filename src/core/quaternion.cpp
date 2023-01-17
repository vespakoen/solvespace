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
