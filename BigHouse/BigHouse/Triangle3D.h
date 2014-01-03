

#ifndef TRIANGLE_3D_H_
#define TRIANGLE_3D_H_

#include "Vector3D.h"
class Triangle3D {
public:
	Triangle3D();
	Triangle3D(Vector3D v0, Vector3D v1, Vector3D v2);
	~Triangle3D();
public:
	Vector3D m_v0;		// one point of triangle
	Vector3D m_v1;		// second point of triangle
	Vector3D m_v2;		// third point of triangle
	Vector3D normal;  // normal vector of triangle
}; 
#endif // TRIANGLE_3D_H_