#pragma once
#include"Shelf.h"
#include "Struct.h"
//#include "RecShelf.h"
class CirclShelf: public Shelf {
public:
	CirclShelf(double r, double h, double sp, double ep, double angle, double count_floor);
  ~CirclShelf();

	virtual bool IsLineCutBody(const Vector3D &dir, const Vector3D& pos, Vector3D &p);
	virtual void DrawShelf();
	virtual void SetCadToShelf(std::pair<Floor , std::vector<Triangle3D*>> &body);
	virtual void GetOriginBody(Vector3D &p_origin);
	virtual void SetOriginBody(Vector3D &p_move);
	virtual void PointMouseOnFloor(Vector3D &dir, Vector3D &pos);
	virtual void ReSetSelectFloor();


private:
  void ShelfStructure(double r, double h, double sp, double ep, double angle);
	void DrawFloor(double r, double h, double sp, double ep, double angle);
	int radius_;
	int height_;
  int height_floor_;
	int count_floor_;
	Vector3D Origin_;
	Vector3D max_origin_;
	std::vector<std::pair<Floor, std::vector<Triangle3D*>>> stocks_;

};