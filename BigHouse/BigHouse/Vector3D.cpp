#include "stdafx.h"
#include "Vector3D.h"

Vector3D::Vector3D() {

}

Vector3D::Vector3D(float x, float y, float z) {
	v[0] = x;
	v[1] = y;
	v[2] = z;
}

Vector3D::Vector3D(float v1[3]) {
	v[0] = v1[0]; v[1] = v1[1]; v[2] = v1[2];
}

Vector3D::~Vector3D() {

}

double Vector3D::scalar(const Vector3D& v1) {
  return v[0]*v1.v[0] + v[1]*v1.v[1] + v[2]*v1.v[2];
}

void Vector3D::Set(double x, double y, double z) {
	v[0] = x;
	v[1] = y;
	v[2] = z;
}

// Get vector Unit
Vector3D Vector3D::Unit() {
	Vector3D temp;
	double d = sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
	temp.v[0] = v[0]/d;
	temp.v[1] = v[1]/d;
	temp.v[2] = v[2]/d;
	return temp;
}

// GetMoudle
double Vector3D::abs() {
  return sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
}