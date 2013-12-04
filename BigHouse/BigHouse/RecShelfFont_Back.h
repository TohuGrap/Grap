#pragma once
#include "Shelf.h"
#include "RecShelf.h"

enum DirectionShelf {
	FONT_BACK = 0,
	LEFT_RIGHT
};

enum TypeFloor {
	FLOOR_FONT = 0,
	FLOOR_BACK
};

class RecShelfFont_Back :public Shelf, RecShelf {

public:
	RecShelfFont_Back(int width, int length, int height, int count_floor, DirectionShelf d_shelf /*,TypeRecShelf type*/);
  ~RecShelfFont_Back();

	virtual bool IsLineCutBody(const Vector3D &dir, const Vector3D& pos, Vector3D &p);
	virtual void DrawShelf();
	virtual void SetCadToShelf(std::pair<Floor , std::vector<Triangle3D*>> &body);
	virtual void GetOriginBody(Vector3D &p_origin);
	virtual void SetOriginBody(Vector3D &p_move);
	virtual void PointMouseOnFloor(Vector3D &dir, Vector3D &pos);
	virtual void ReSetSelectFloor();

private:
  int length_;
	int height_;
	int width_;
	double height_floor_;
	int count_floor_;
	TypeFloor type_;
	DirectionShelf d_shelf_;
	Vector3D bbmin_font_;
	Vector3D bbmax_font_;
	Vector3D bbmin_back_;
	Vector3D bbmax_back_;
	std::vector<std::pair<Floor, std::vector<Triangle3D*>>> stocks_font_;
	std::vector<std::pair<Floor, std::vector<Triangle3D*>>> stocks_back_;
};