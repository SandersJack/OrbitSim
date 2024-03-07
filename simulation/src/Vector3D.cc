#include "Vector3D.hh"
#include <iostream>
#include <cmath>

ClassImp(Vector3D)

Vector3D::Vector3D(): fx(0), fy(0), fz(0) {

}

Vector3D::Vector3D(double in_x, double in_y, double in_z): fx(0), fy(0), fz(0) {
    fx = in_x;
    fy = in_y;
    fz = in_z;
}

Vector3D::Vector3D(Vector3D *val): fx(0), fy(0), fz(0) {
    fx = val->GetX();
    fy = val->GetY();
    fz = val->GetZ();
}

Vector3D& Vector3D::Add(Vector3D& other) {
    fx += other.GetX();
    fy += other.GetY();
    fz += other.GetZ();

    return *this;
}

Vector3D& Vector3D::Multiply(double scaler) {
    fx *= scaler;
    fy *= scaler;
    fz *= scaler;

    return *this;
}

void Vector3D::Normalize() {
    double magnitude = sqrt(fx * fx + fy * fy + fz * fz);

    if (magnitude != 0.0) {
        fx /= magnitude;
        fy /= magnitude;
        fz /= magnitude;
    }
}

Vector3D Vector3D::Cross(Vector3D& other) {
    return Vector3D(fy * other.GetZ() - fz * other.GetY(),
                    fz * other.GetX() - fx * other.GetZ(),
                    fx * other.GetY() - fy * other.GetX());
}

Vector3D Vector3D::operator*(double scalar) const{
    return Vector3D(fx * scalar, fy * scalar, fz * scalar);
}

Vector3D Vector3D::operator+(const Vector3D& other) const {
    return Vector3D(fx + other.fx, fy + other.fy, fz + other.fz);
}


Vector3D::~Vector3D(){
}

void Vector3D::Print(){
    std::cout << "{ " << fx << " , " << fy << " , " << fz << " }" << std::endl;
}