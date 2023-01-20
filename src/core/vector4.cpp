#include "vector4.h"

Vector4 Vector4::From(double w, double x, double y, double z) {
    Vector4 ret;
    ret.w = w;
    ret.x = x;
    ret.y = y;
    ret.z = z;
    return ret;
}

Vector4 Vector4::From(double w, Vector v) {
    return Vector4::From(w, w * v.x, w * v.y, w * v.z);
}

Vector4 Vector4::Blend(Vector4 a, Vector4 b, double t) {
    return (a.ScaledBy(1 - t)).Plus(b.ScaledBy(t));
}

Vector4 Vector4::Plus(Vector4 b) const {
    return Vector4::From(w + b.w, x + b.x, y + b.y, z + b.z);
}

Vector4 Vector4::Minus(Vector4 b) const {
    return Vector4::From(w - b.w, x - b.x, y - b.y, z - b.z);
}

Vector4 Vector4::ScaledBy(double s) const {
    return Vector4::From(w * s, x * s, y * s, z * s);
}

Vector Vector4::PerspectiveProject() const {
    return Vector::From(x / w, y / w, z / w);
}