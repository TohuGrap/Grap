#pragma once
class Vector3D {
public:
	Vector3D();
	Vector3D(float x, float y, float z);
	Vector3D(float v1[3]);
	~Vector3D();

	float v[3];
public:
	const Vector3D &operator + (Vector3D &v1) {
		Vector3D temp;
		temp.v[0] = v[0] + v1.v[0];
		temp.v[1] = v[1] + v1.v[1];
		temp.v[2] = v[2] + v1.v[2];
		return temp;
}

 const Vector3D &operator - (Vector3D &v1) {
		Vector3D temp;
		temp.v[0] = v[0] - v1.v[0];
		temp.v[1] = v[1] - v1.v[1];
		temp.v[2] = v[2] - v1.v[2];
		return temp;
  }

const Vector3D &operator * (Vector3D &v1) {
  Vector3D temp;
	temp.v[0] = v[1]*v1.v[2] - v[2]*v1.v[1];
	temp.v[1] = v[2]*v1.v[0] - v[0]*v1.v[2];
	temp.v[2] = v[0]*v1.v[1] - v[1]*v1.v[0];
	return temp;
	}

double scalar(const Vector3D & v1);

const Vector3D &operator * (double a) {
	 Vector3D temp;
	 temp.v[0] = v[0]*a;
	 temp.v[1] = v[1]*a;
	 temp.v[2] = v[2]*a;
	 return temp;
	}
	double abs();
	Vector3D Unit();
public:
	void Set(double x, double y, double z);
private:
	//float v[3];

};