#pragma once
#include "Vector3D.h"
class Triangle3D {
public:
	Triangle3D();
	Triangle3D(Vector3D v0, Vector3D v1, Vector3D v2);
	~Triangle3D();
public:
	Vector3D m_v0;
	Vector3D m_v1;
	Vector3D m_v2;
	Vector3D normal;
private:

}; 