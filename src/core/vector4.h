#ifndef SOLVESPACE_VECTOR4_H
#define SOLVESPACE_VECTOR4_H

class Vector4 {
public:
    double w, x, y, z;

    // static Vector4 From(double w, double x, double y, double z);
    // static Vector4 From(double w, Vector v3);
    static Vector4 Blend(Vector4 a, Vector4 b, double t);

    // Vector4 Plus(Vector4 b) const;
    // Vector4 Minus(Vector4 b) const;
    // Vector4 ScaledBy(double s) const;
    // Vector PerspectiveProject() const;
};

#endif
