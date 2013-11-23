#pragma once
#include "Vector3D.h"
#include "Triangle3D.h"
class Shelf {
	struct Rec {
		Vector3D n;
		Vector3D p[4];
	};
public:
	Shelf();
	~Shelf();
	void DrawFaceShelf(Rec &rec);
	void DrawCube(double width, double length, double height);
	void DrawShelf();
	void ShelfStructure();
	void DrawShelfFloor();
	//void SetRect()
private:
	int length_;
	int height_;
	int width_;
	int face_heigth_;
	int height_floor_;
	int count_floor_;
	std::vector<int> list_height_floor_;
	std::vector<std::pair<int, std::vector<Triangle3D*>>> stocks_;


};