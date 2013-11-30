#include "stdafx.h"
#include "Triangle3D.h"
Triangle3D::Triangle3D() {

}

Triangle3D::Triangle3D(Vector3D v0, Vector3D v1, Vector3D v2) {
  m_v0 = v0;
	m_v1 = v1;
	m_v2 = v2;
}

Triangle3D::~Triangle3D() {
}