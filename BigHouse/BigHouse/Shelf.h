#pragma once
#include "Vector3D.h"
#include "Triangle3D.h"
#include "BigHouse.h"

class Shelf {
	struct Rec {
		Vector3D n;
		Vector3D p[4];
	};
	//struct Size_rec {
	//	int i;
	//	int j;
	//};
	//struct Floor {
	//	Vector3D color;
	//	int height_floor;
	//	Size_rec s_r;
	//	RectBody s_b;
	//};

public:
	Shelf(int width, int length, int height, int count_floor);
	~Shelf();
	void DrawFaceShelf(Rec &rec);
	void DrawCube(double width, double length, double height);
	void DrawShelf();
	void ShelfStructure();
	void DrawShelfFloor();
	void GetBoundingBox(Vector3D &bbmin, Vector3D &bbmax) const;
	void GetBBmin(Vector3D &bbmin) const;
  void SetBoundingBox(Vector3D &bbmin /*Vector3D &bbmax*/);
	void PointMouseOnFloor(Vector3D &dir, Vector3D &pos);
	bool LineCutSurface(Vector3D &dir,
	                    Vector3D &pos,
											Vector3D &n,
											Vector3D &A,
											Vector3D &B,
											Vector3D &E);
	void SetCadToShelf(std::pair<Floor , std::vector<Triangle3D*>> &body);
	void DrawPoint();
	void DrawCommodity();
	void ReSetSelectFloor();
private:
	int length_;
	int height_;
	int width_;
	int face_heigth_;
	int height_floor_;
	int count_floor_;
	std::vector<int> list_height_floor_;
	std::vector<std::pair<Floor, std::vector<Triangle3D*>>> stocks_;
	Vector3D bbmin_;
	Vector3D bbmax_;
	Vector3D point_on_floor_;
};